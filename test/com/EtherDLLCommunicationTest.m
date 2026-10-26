function tests = EtherDLLCommunicationTest
% Black-box communication tests for the EtherDLL TCP service.
%
% These tests follow the TSMW client protocol: JSON requests terminated by
% CRLF, with ACK/NACK and command responses returned as CRLF-delimited JSON.
% The EtherDLL service must already be running. The IDN and unknown-command
% cases do not require a connected receiver, so DEMO mode is suitable.

tests = functiontests(localfunctions);
end

function testFramingAndReconnect(testCase)
[host, port] = serviceEndpoint();
client = connectToService(host, port);
clientCleanup = onCleanup(@() closeClient(client));
rxBuffer = '';

% The TSMW test client sends the same command shape from its command JSON.
idnRequest = jsonencode(struct( ...
    'CODE', 4, ...
    'COMMAND', '*idn?', ...
    'ARGS', struct()));

% Split inside the JSON body, then split the configured CRLF delimiter itself.
wireRequest = [idnRequest sprintf('\r\n')];
splitAt = max(1, floor(numel(wireRequest) / 2));
write(client, uint8(wireRequest(1:splitAt)), 'uint8');
pause(0.05);
write(client, uint8(wireRequest(splitAt + 1:end - 1)), 'uint8');
pause(0.05);
write(client, uint8(wireRequest(end)), 'uint8');

frames = readUntil(client, rxBuffer, 1, 1, 8);
rxBuffer = frames.rxBuffer;
testCase.verifyGreaterThanOrEqual(frames.ackCount, 1, ...
    'A fragmented request should receive an ACK.');
testCase.verifyGreaterThanOrEqual(frames.idnCount, 1, ...
    'A fragmented IDN request should receive a command response.');

% Two requests written together exercise TCP coalescing and per-message
% framing. The unknown command follows the valid TSMW JSON envelope.
unknownRequest = jsonencode(struct( ...
    'CODE', 9999, ...
    'COMMAND', 'unknownTestCommand', ...
    'ARGS', struct()));
coalesced = [idnRequest sprintf('\r\n') unknownRequest sprintf('\r\n')];
write(client, uint8(coalesced), 'uint8');

frames = readUntil(client, rxBuffer, 2, 1, 8);
rxBuffer = frames.rxBuffer;
testCase.verifyGreaterThanOrEqual(frames.ackCount, 2, ...
    'Both coalesced requests should receive an ACK.');
testCase.verifyGreaterThanOrEqual(frames.idnCount, 1, ...
    'The valid request in the coalesced write should receive an IDN response.');
testCase.verifyTrue(frames.hasUnknownCommandError, ...
    'The unknown command should receive a structured error response.');

% Reject a frame above the hard limit and still process the following frame.
maxFrameBytes = 1000000;
oversizedPayload = repmat('X', 1, maxFrameBytes + 1);
write(client, uint8(oversizedPayload), 'uint8');
[pendingFrames, rxBuffer] = readAvailableFrames(client, rxBuffer, 0.25);
hasPrematureNack = false;
for k = 1:numel(pendingFrames)
    if isstruct(pendingFrames{k}) && isfield(pendingFrames{k}, 'NACK')
        hasPrematureNack = true;
    end
end
testCase.verifyFalse(hasPrematureNack, ...
    'An oversized but unterminated frame must not receive a NACK yet.');

write(client, uint8([sprintf('\r\n') idnRequest sprintf('\r\n')]), 'uint8');
frames = readUntil(client, rxBuffer, 1, 1, 20, 1);
testCase.verifyEqual(frames.nackCount, 1, ...
    'An oversized frame should produce exactly one NACK.');
testCase.verifyGreaterThanOrEqual(frames.ackCount, 1, ...
    'The valid frame after an oversized frame should still be processed.');
testCase.verifyGreaterThanOrEqual(frames.idnCount, 1, ...
    'The valid IDN frame after an oversized frame should receive a response.');

% STOP is a control command and must pass request validation even when no
% measurement is active (the integration test can run in DEMO mode).
stopRequest = jsonencode(struct( ...
    'CODE', 3, ...
    'COMMAND', 'stopPowerScanMeasurement', ...
    'ARGS', struct()));
write(client, uint8([stopRequest sprintf('\r\n')]), 'uint8');
frames = readUntil(client, rxBuffer, 1, 0, 8, 0, true);
testCase.verifyTrue(frames.hasStopMessage, ...
    'The STOP command should be accepted and return a status message.');

% Closing one client must not prevent the service from accepting another.
closeClient(client);
clear clientCleanup;

reconnected = false;
secondClient = [];
try
    secondClient = connectToService(host, port, 4);
    reconnected = true;
catch
    % Keep the test result bounded: a failure here indicates the service did
    % not return to accept() after the prior client disconnected.
end
if ~isempty(secondClient)
    closeClient(secondClient);
end
testCase.verifyTrue(reconnected, ...
    'EtherDLL should accept a new client after the previous client disconnects.');
end

function [host, port] = serviceEndpoint()
% Reuse the endpoint from the existing TSMW test client unless overridden.
testDir = fileparts(mfilename('fullpath'));
tsmwConfigPath = fullfile(testDir, '..', 'TSMW', 'appConfig.json');
config = jsondecode(fileread(tsmwConfigPath));

host = string(config.proxy.address);
port = double(config.service.port);

envHost = getenv('ETHERDLL_TEST_HOST');
envPort = getenv('ETHERDLL_TEST_PORT');
if ~isempty(envHost)
    host = string(envHost);
end
if ~isempty(envPort)
    parsedPort = str2double(envPort);
    if ~isfinite(parsedPort) || parsedPort < 1 || parsedPort > 65535 || parsedPort ~= fix(parsedPort)
        error('EtherDLL:InvalidTestPort', ...
            'ETHERDLL_TEST_PORT must be an integer from 1 to 65535.');
    end
    port = parsedPort;
end
end

function client = connectToService(host, port, connectTimeout)
if nargin < 3
    connectTimeout = 3;
end
client = tcpclient(char(host), port, ...
    'Timeout', 1, ...
    'ConnectTimeout', connectTimeout);
configureTerminator(client, 'CR/LF');
end

function result = readUntil(client, rxBuffer, expectedAcks, expectedIdn, timeoutSeconds, expectedNacks, expectedStop)
% Read CRLF-delimited JSON without relying on one TCP read per frame.
if nargin < 6
    expectedNacks = 0;
end
if nargin < 7
    expectedStop = false;
end
deadline = tic;
ackCount = 0;
nackCount = 0;
idnCount = 0;
hasUnknownCommandError = false;
hasStopMessage = false;

while toc(deadline) < timeoutSeconds
    [frame, rxBuffer] = readFrame(client, rxBuffer, max(0.01, timeoutSeconds - toc(deadline)));
    try
        message = jsondecode(frame);
    catch exception
        error('EtherDLL:InvalidResponseJSON', ...
            'The service returned a non-JSON frame: %s (%s)', frame, exception.message);
    end

    if isstruct(message) && isfield(message, 'ACK')
        ackCount = ackCount + 1;
    end
    if isstruct(message) && isfield(message, 'NACK')
        nackCount = nackCount + 1;
    end
    if isstruct(message) && isfield(message, 'model') && isfield(message, 'message')
        idnCount = idnCount + 1;
    end
    if isstruct(message) && isfield(message, 'COMMAND') && ...
            strcmp(string(message.COMMAND), "error_response")
        hasUnknownCommandError = true;
    end
    if isstruct(message) && isfield(message, 'message') && ...
            contains(string(message.message), "measurement")
        hasStopMessage = true;
    end

    if ackCount >= expectedAcks && nackCount >= expectedNacks && idnCount >= expectedIdn && ...
            (expectedAcks < 2 || hasUnknownCommandError) && ...
            (~expectedStop || hasStopMessage)
        break;
    end
end

result = struct( ...
    'ackCount', ackCount, ...
    'nackCount', nackCount, ...
    'idnCount', idnCount, ...
    'hasUnknownCommandError', hasUnknownCommandError, ...
    'hasStopMessage', hasStopMessage, ...
    'rxBuffer', rxBuffer);
end

function [messages, rxBuffer] = readAvailableFrames(client, rxBuffer, waitSeconds)
deadline = tic;
messages = {};
terminator = sprintf('\r\n');

while toc(deadline) < waitSeconds
    available = client.NumBytesAvailable;
    if available > 0
        bytes = read(client, available, 'uint8');
        rxBuffer = [rxBuffer char(bytes(:).')]; %#ok<AGROW>

        terminatorIndex = strfind(rxBuffer, terminator);
        while ~isempty(terminatorIndex)
            firstIndex = terminatorIndex(1);
            frame = rxBuffer(1:firstIndex - 1);
            rxBuffer = rxBuffer(firstIndex + numel(terminator):end);
            messages{end + 1} = jsondecode(frame); %#ok<AGROW>
            terminatorIndex = strfind(rxBuffer, terminator);
        end
    else
        pause(0.01);
    end
end
end

function [frame, rxBuffer] = readFrame(client, rxBuffer, timeoutSeconds)
deadline = tic;
terminator = sprintf('\r\n');

while toc(deadline) < timeoutSeconds
    terminatorIndex = strfind(rxBuffer, terminator);
    if ~isempty(terminatorIndex)
        firstIndex = terminatorIndex(1);
        frame = rxBuffer(1:firstIndex - 1);
        rxBuffer = rxBuffer(firstIndex + numel(terminator):end);
        return;
    end

    available = client.NumBytesAvailable;
    if available > 0
        bytes = read(client, available, 'uint8');
        rxBuffer = [rxBuffer char(bytes(:).')]; %#ok<AGROW>
    else
        pause(0.01);
    end
end

error('EtherDLL:ResponseTimeout', ...
    'Timed out waiting for a complete CRLF-delimited response frame.');
end

function closeClient(client)
if ~isempty(client)
    try
        delete(client);
    catch
        % The client may already have been closed by the test or peer.
    end
end
end

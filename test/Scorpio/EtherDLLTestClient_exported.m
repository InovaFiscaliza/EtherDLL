classdef EtherDLLTestClient_exported < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        EtherDLLTest           matlab.ui.Figure
        mainGrid               matlab.ui.container.GridLayout
        ACKButton              matlab.ui.control.Button
        PINGButton             matlab.ui.control.Button
        cleanParsedAreaIcon    matlab.ui.control.Image
        parsedData             matlab.ui.container.Tree
        connectButton          matlab.ui.control.StateButton
        bandSelectDropDown     matlab.ui.control.DropDown
        cleanReceivedAreaIcon  matlab.ui.control.Image
        repeatCmdIcon          matlab.ui.control.Image
        CommandDropDown        matlab.ui.control.DropDown
        sentMsg                matlab.ui.control.TextArea
        receivedMsg            matlab.ui.control.TextArea
        SPTAxes                matlab.ui.control.UIAxes
        OCCAxes                matlab.ui.control.UIAxes
        AOAAxes                matlab.ui.control.UIAxes
    end

    
    %-----------------------------------------------------------------%
    properties (Constant)
        % app constants -----------------------------------------%
        MHZ_MULTIPLIER = 1000000
        KHZ_MAX_VALUE = 1
        KHZ_FROM_MHZ = 1000
        GHZ_MIN_VALUE = 1000
        GHZ_FROM_MHZ = 0.001

        TOLERANCE = 1

        % EtherDLL Constants
        PACK_END = "CR/LF"

        % Scorpio Command Codes
        AVD = 15
        MEASURE = 84
        OCC = 12
        OCC_DF = 14
        PAN = 55
        SET_AUDIO = 16

    end

    %-----------------------------------------------------------------%
    properties (Access = public)
        config % Store configuration structure read from json file at startup
        connection % Store command channel socket connection
        data % Store plot data to allow band switching
        refresh = false; % store requests for graph refresh
        pendingConnection = true; % store connection state, good knows why state button stopped working 
        spectrumData = {}; % Store spectrum traces as cell array of structs with frequency and measurement pairs
        spectrumDataOrder = []; % Store the order in which the spectrumData must be plotted (FIFO)
    end
    
    %-----------------------------------------------------------------%
    methods (Access = public)
        
        %-----------------------------------------------------------------%
        function loadConf(app)
        %LOADCONF load app configuration from json file
        %   Use a default json file to define constants that control the
        %   application and further load command examples in json format to populate the command list.

            try
                % try to read the json file
                app.config = jsondecode(fileread('appConfig.json'));
            catch
                % use default values if the file is not found
                warning('Failed to read configuration from file appConfig.json');
                app.config = struct('log', struct('file', struct('enable', true, 'level', 'trace', 'path', 'log.txt')), ...
                                                'proxy', struct('address', 'localhost', 'timeout_s', 10), ...
                                                'service', struct('port', 30000, 'timeout_s', 10000, 'sleep_ms', 100, 'check_period', 10,'command_path','.D:/GitHub/EtherDLL/test/command'));
            end

            % Set absolute path for command_path
            relPath = app.config.service.command_path;
            if startsWith(relPath, '.\') || startsWith(relPath, './')
                scriptDir = fileparts(mfilename('fullpath'));
                relPath = relPath(3:end);
                app.config.service.command_path = fullfile(scriptDir, relPath);
            end

            if ~exist(app.config.service.command_path, 'dir') || exist(app.config.service.command_path, 'dir') ~= 7
                warning('Command path "%s" does not exist or is not a valid directory.\nApplication will exit.', app.config.service.command_path);
                
                % Show error dialog to user
                uialert(app.EtherDLLTest, errMsg, 'Configuration Error', 'Icon', 'error');
                
                % Close the app
                delete(app);
                return;
            end

            loadCommandList(app);
            app.connectButton.Tooltip = sprintf('Connect/Disconnect to EtherDLL on port %d', app.config.service.port);
        end

        %-----------------------------------------------------------------%
        function loadCommandList(app)
        %LOADCOMMANDLIST load commands to be sent to EtherDLL
        % use configured folder to get all commands and populate the
        % dropdown control
            

            % list json files present in the app folder
            files = dir(fullfile(app.config.service.command_path, 'cmd*.json'));
            
            items_size = length(files) + 1;
            cmd_items{items_size} = [];
            cmd_values{items_size} = [];
            
            cmd_items{1} = '<select a command>';
            cmd_values{1} = '';
            
            for i = 2:items_size
                try
                    filename = files(i-1).name;
                    fullpath = fullfile(app.config.service.command_path, filename);
                    cmd_items{i} = filename(4:end-5);
                    cmd_values{i} = jsonencode(jsondecode(fileread(fullpath)));
                catch
                    warning('Failed to read command list from file %s', filename);
                    continue;
                end
            end
            
            app.CommandDropDown.Items = cmd_items;
            app.CommandDropDown.ItemsData = cmd_values;
        end
        
        %-----------------------------------------------------------------%
        function doing_fine = connect(app)
            % Connect the various socket interfaces desginated as channels
            % and set corresponding lamp indication in the UI

            try
                app.connection = tcpclient(app.config.proxy.address,app.config.service.port, 'Timeout',app.config.service.timeout_s, 'ConnectTimeout',app.config.service.timeout_s, 'Tag', 'EtherDLL');
                configureCallback(app.connection,"terminator",@app.receivedData);
                configureTerminator(app.connection,app.PACK_END);
                loadCommandList(app);
                doing_fine = true;
            catch
                warning('Failed to connect to EtherDLL');
                doing_fine = false;
            end      
        end

        %-----------------------------------------------------------------%
        function cleanSentMsg(app)
            % Clean command sent area
            app.CommandDropDown.Value = '';
            app.sentMsg.Value = "";
            cla(app.SPTAxes);
            cla(app.OCCAxes);
            cla(app.AOAAxes);
            app.repeatCmdIcon.Enable = false;
        end

        %-----------------------------------------------------------------%
        function disconnect(app)
            % Disconnect all channels and reset lamps

            try
                flush(app.connection);
                clear app.connection;
                delete(app.connection);
                cleanSentMsg(app);
            catch
                warning('Failed to disconnect from EtherDLL.');
            end
        end

        %-----------------------------------------------------------------%
        function receivedData(app, src, ~)
            % Handle incomming data in the command channel
            raw_data = read(src,src.NumBytesAvailable,"string");
            struct_data = processRawData(app,raw_data);
            
            % Process each structure independently
            for i = 1:length(struct_data)
                processStructData(app,struct_data(i));
            end
        end

        %-----------------------------------------------------------------%
         function out_data = processRawData(app, in_data)
            try
                % Split on line terminators (CR/LF, LF, etc.)
                mid_data = splitlines(in_data);
                
                % Remove empty strings
                mid_data(strlength(mid_data) == 0) = [];
                
                % Convert each JSON string to structure - always return as array
                out_data = [];  % Start as empty array
                if ~isempty(mid_data)
                    for i = 1:length(mid_data)
                        try
                            decoded = jsondecode(mid_data(i));
                            out_data = [out_data; decoded];  % Concatenate vertically to build struct array
                        catch
                            message = newline + "Error - Non JSON data: <" + evalc('disp(mid_data(i))') + ">";
                            app.receivedMsg.Value = [message;app.receivedMsg.Value];
                        end
                    end
                end
                
                app.receivedMsg.Value = evalc('disp(in_data)');
            catch
                if isempty(in_data)
                    app.connectButton.Value = not(app.connectButton.Value);
                    message = newline + "Error - Not Connected to EtherDLL";
                else
                    message = newline + "Error processing raw data: <" + evalc('disp(in_data)') + ">";
                end
                app.receivedMsg.Value = [message;app.receivedMsg.Value];
            end
        end

        %-----------------------------------------------------------------%
        function processStructData(app, data)
            % Process different kinds of data from the received data into
            % adjusted structures. Dynamically identifies and processes keys
            % such as "spectrum", "occupancy", "measurement", "equipment", "settings".
            % Response may include none, one, many or all keys.
            
            % Define supported data keys and their corresponding handler methods
            supportedKeys = {'spectrum', 'aoa', 'occupancy', 'measure', 'equipment', 'setting', 'site', 'PING', 'ACK', 'NACK'};
            
            % Get all fields in the incoming data structure
            dataFields = fieldnames(data);
            
            % Process each field that matches a supported key
            for i = 1:length(dataFields)
                fieldName = dataFields{i};
                
                % Check if this field is in our supported keys
                if ismember(fieldName, supportedKeys)
                    try
                        % Call the appropriate handler function based on field name
                        switch fieldName
                            case 'spectrum'
                                if isfield(data.(fieldName), 'traceDataDF')                                    
                                    data.spectrum = buildAxis(app,data.spectrum, 'traceDataDF', 'DF Antenna Field Strength', false);
                                end
                                data.spectrum = buildAxis(app,data.spectrum, 'traceData', 'Field Strength', false);

                                plotData(app,app.SPTAxes, data.spectrum);
                                presentData(app,'spectrum',data.spectrum,["numBands","numTotalBins","numBins","firstBin","numTimeOfDays"])
                            case 'occupancy'
                                data.occupancy = buildAxis(app,data.occupancy, 'chanData', "Channel Data", false);
                                data.occupancy = buildAxis(app,data.occupancy, 'aveRange', "Average Range", false);
                                data.occupancy = buildAxis(app,data.occupancy, 'aveFldStr', "Average Field Strength", false);

                                plotData(app,app.OCCAxes, data.occupancy);
                            case 'aoa'
                                data.aoa = buildAxis(app,data.aoa, 'traceData', 'Azimuth', false);
                                data.aoa = buildAxis(app,data.aoa, 'confidence','Confidence', true);
                                plotData(app,app.AOAAxes, data.aoa);
                            case 'measure'
                                presentData(app,'measure',data.measure, ["noiseFloor", "powerDbm"]);
                            case 'equipment'
                                presentData(app,'equipment',data.equipment,["hostName","selectedAntenna"]);
                            case 'setting'
                                presentData(app,'setting',data.setting,["primaryThresholdAbsolute","primaryThresholdAboveNoise","secondaryThresholdAbsolute","secondaryThresholdAboveNoise","saveIntermediateData","useSecondaryThreshold","numAzimuths","attenuation"]);
                            case 'site'
                                presentData(app,'site',data.site,["dateTime","latitude","longitude","numSats"]);
                            case 'task'
                                presentData(app,'task',data.task,["dateTime","taskId","key","state","status","completionTime"])
                            case 'PING'
                                flashButtonColor(app,app.PINGButton, [0.9 0.9 0.9], 1.0);
                            case 'ACK'
                                flashButtonColor(app,app.ACKButton, [0.0 1.0 0.0], 1.0);
                            case 'NACK'
                                flashButtonAckNack(app,app.ACKButton, 1.0);
                        end
                    catch ME
                        warning(ME.identifier, 'Error processing field "%s": %s', fieldName, ME.message);
                    end
                end
            end
        end

        %-----------------------------------------------------------------%
        function data = buildAxis(app, data, arrayName, measureAxisName, isAlpha)
            % build x and y axis for different data types and accumulate spectrum data
            % data: structure containing data to be plotted
            % arrayName: Name of the data field, e.g. 'traceData', 'aoa', 'occupancy'.
            % measureAxisName: Name to be used as label for the measurement axis
            % isAlpha: if true, store data in alphaAxis instead of accumulating
            % Returns: modified data structure with measureAxis (or custom name) and frequencyAxis
            
            % Convert base64 data to float32 array
            newTrace = app.base64ToFloat32(data.(arrayName));
            
            % If this is alpha/confidence data, just store it and return
            if isAlpha
                data.alphaAxis = newTrace;
                return;
            end
            
            % Create frequency array for this trace's frequency range
            frequencyArray = linspace(data.startFrequency, data.stopFrequency, data.numBins);
            
            % Check if we already have data for this frequency range
            existingIdx = findSpectrumRangeIndex(app, data);
            
            if existingIdx > 0
                % Update existing frequency range's measurement data
                % Store with measurement axis name to track source and
                % update the spectrumDataOrder later used for plotting
                app.spectrumData{existingIdx}.measurements{end+1} = newTrace;
                app.spectrumData{existingIdx}.measurementNames{end+1} = measureAxisName;
                app.spectrumDataOrder = [existingIdx, app.spectrumDataOrder(app.spectrumDataOrder ~= existingIdx)];
            else
                % Create new entry for this frequency range
                newEntry = struct();
                newEntry.startFreq = data.startFrequency;
                newEntry.stopFreq = data.stopFrequency;
                newEntry.numBins = data.numBins;
                newEntry.frequency = frequencyArray;
                newEntry.measurements = {newTrace};
                newEntry.measurementNames = {measureAxisName};
                
                app.spectrumData{end+1} = newEntry;
                app.spectrumDataOrder = [length(app.spectrumData), app.spectrumDataOrder];
            end
            
            % Build combined data structure for legacy compatibility with plotData
            data = app.buildCombinedSpectrumData();
            
        end

        %-----------------------------------------------------------------%
        function idx = findSpectrumRangeIndex(app, data)
            % Find if a spectrum range already exists in app.spectrumData
            % Returns: index if found, 0 if not found
            
            idx = 0;
            if isempty(app.spectrumData)
                return;
            end
            
            for i = 1:length(app.spectrumData)
                if abs(app.spectrumData{i}.startFreq - data.startFrequency) < app.TOLERANCE && ...
                   abs(app.spectrumData{i}.stopFreq - data.stopFrequency) < app.TOLERANCE && ...
                   abs(app.spectrumData.numBins - data.numBins) < app.TOLERANCE
                    idx = i;
                    return;
                end
            end
        end

        %-----------------------------------------------------------------%
        function data = buildCombinedSpectrumData(app)
            % Merge all frequency ranges and their corresponding measurement arrays
            % Returns: combined data structure suitable for plotData
            
            data = struct();
            
            % Sort spectrum data by start frequency in ascending order
            startFreqs = cellfun(@(x) x.startFreq, app.spectrumData);
            [~, sortIdx] = sort(startFreqs);
            sortedSpectra = app.spectrumData(sortIdx);
            
            % Merge all frequency arrays and corresponding measurements
            combinedFrequency = [];
            combinedMeasurements = cell(0);
            combinedMeasurementNames = {};
            
            % Track which trace index each measurement corresponds to
            measurementTraceIdx = [];
            
            for i = 1:length(sortedSpectra)
                currentSpectrum = sortedSpectra{i};
                
                % Append frequency array
                combinedFrequency = [combinedFrequency; currentSpectrum.frequency(:)];
                
                % Append all measurements for this frequency range
                for j = 1:length(currentSpectrum.measurements)
                    combinedMeasurements{end+1} = currentSpectrum.measurements{j};
                    combinedMeasurementNames{end+1} = currentSpectrum.measurementNames{j};
                    measurementTraceIdx(end+1) = i; % Track which spectrum range this measurement came from
                end
            end
            
            % Build output structure
            data.frequencyAxis = combinedFrequency;
            data.measureAxis = combinedMeasurements;
            data.measureAxisNames = combinedMeasurementNames;
            data.combinedStartFreq = min(startFreqs);
            data.combinedStopFreq = max(startFreqs); % Get from actual spectra end freqs
            
            % Fix combinedStopFreq to use actual stop frequency
            data.combinedStopFreq = max(cellfun(@(x) x.stopFreq, app.spectrumData));
            
            data.numTraces = length(combinedMeasurements);
        end

        %-----------------------------------------------------------------%
        function plotData(app, axesHandle, data)
            % plot spectrum data on specified axes
            % axesHandle: UIAxes object (app.SPTAxes, app.AOAAxes, app.OCCAxes, etc.)
            % data: structure containing spectrum information with measureAxis (cell array)
            
            hold(axesHandle, "on");
            if app.refresh
                %hold(axesHandle, "off");
                app.refresh = false;
            else
                %hold(axesHandle, "on");
            end
            
            % Skip if no data
            if ~isfield(data, 'frequencyAxis') || isempty(data.frequencyAxis)
                return;
            end
            
            % Use combined frequency range
            xMin = data.combinedStartFreq;
            xMax = data.combinedStopFreq;
            xlim(axesHandle, [xMin xMax]);

            % Set 10 ticks on horizontal axis (frequency)
            xTicks = linspace(xMin, xMax, 10);
            axesHandle.XTick = xTicks;
            axesHandle.XTickLabel = app.formatEngineeringNotation(xTicks);
            
            % Determine Y-axis limits from all traces
            numTraces = length(data.measureAxis);
            allValues = [];
            for i = 1:numTraces
                trace = data.measureAxis{i};
                % Filter out NaN values when computing limits
                validValues = trace(~isnan(trace));
                if ~isempty(validValues)
                    allValues = [allValues; validValues(:)];
                end
            end
            
            if isempty(allValues)
                return;
            end
            
            yMin = min(allValues);
            yMax = max(allValues);
            
            % Set 10 ticks on vertical axis (measurement values)
            yTicks = linspace(yMin, yMax, 10);
            axesHandle.YTick = yTicks;
            axesHandle.YTickLabel = app.formatEngineeringNotation(yTicks);

            traceColors = [
                0.0000, 0.4470, 0.7410, 0.9;  % Blue
                0.8500, 0.3250, 0.0980, 0.9;  % Orange
                0.9290, 0.6940, 0.1250, 0.9;  % Yellow
                0.4940, 0.1840, 0.5560, 0.9;  % Purple
                0.4660, 0.6740, 0.1880, 0.9;  % Green
                0.3010, 0.7450, 0.9330, 0.9;  % Cyan
                0.6350, 0.0780, 0.1840, 0.9;  % Dark Red
                0.5, 0.5, 0.5, 0.9;            % Gray
            ];
            
            legendLabels = cell(numTraces, 1);
            for i = 1:numTraces
                currentTrace = data.measureAxis{i};
                
                % Select color based on trace index (cycle through predefined colors)
                colorIdx = mod(i-1, size(traceColors, 1)) + 1;
                currentColor = traceColors(colorIdx, :);
                
                % Check if this is confidence data (for transparency-based plotting)
                if isfield(data, 'alphaAxis')
                    % Convert measureValue (0-100) to alpha values (0-1)
                    alphaValues = data.alphaAxis / 100;
                    scatter(axesHandle, data.frequencyAxis, currentTrace, 36, ...
                        currentColor, 'filled', 'MarkerFaceAlpha', 'flat', ...
                        'AlphaData', alphaValues);
                else
                    % Standard line plot with predefined color
                    plot(axesHandle, data.frequencyAxis, currentTrace, '-', ...
                        'LineWidth', 2.5, 'Color', currentColor);
                end
                
                legendLabels{i} = data.measureAxisNames{i};
            end
            
            % Add legend if multiple traces
            if length(data.measureAxis) > 1
                legend(axesHandle, legendLabels, 'Location', 'northeast');
            end
            drawnow limiterate;
            hold(axesHandle, "off");
        end

        %-----------------------------------------------------------------%
        function presentData(app, parentNodeName, data, fieldsToPresent)
            % Present textual data by dynamically creating/updating parent and child nodes
            % parentNodeName: Name of the parent node (e.g., 'site', 'equipment', 'measure', etc.)
            % data: Structure containing field data
            % fieldsToPresent: Cell array of field names to display as child nodes
            %
            % This function creates parent and child nodes dynamically if they don't exist,
            % or updates them if they do. Each child node displays "fieldName: value".
            
            % Get or create the parent node
            parentNode = [];
            existingNodes = app.parsedData.Children;
            for i = 1:length(existingNodes)
                if strcmp(existingNodes(i).Text, parentNodeName)
                    parentNode = existingNodes(i);
                    break;
                end
            end
            
            if isempty(parentNode)
                % Create parent node dynamically
                parentNode = uitreenode(app.parsedData, 'Text', parentNodeName);
            end
            
            % Process each field to present
            for i = 1:length(fieldsToPresent)
                fieldName = fieldsToPresent{i};
                
                if isfield(data, fieldName)
                    % Convert field value to string (handle numeric, logical, string types)
                    if isnumeric(data.(fieldName))
                        valueStr = num2str(data.(fieldName));
                    elseif islogical(data.(fieldName))
                        valueStr = string(data.(fieldName));
                    else
                        valueStr = fieldName;
                    end

                    % Check if child node already exists
                    childNode = [];
                    existingChildren = parentNode.Children;
                    for j = 1:length(existingChildren)
                        if startsWith(existingChildren(j).Text, fieldName)
                            childNode = existingChildren(j);
                            break;
                        end
                    end
                    

                    message = [fieldName, ':', valueStr];
                    if isempty(childNode)
                        % Create new child node
                        childNode = uitreenode(parentNode, 'Text', message);
                    else
                        % Update existing child node
                        childNode.Text = message;
                    end
                end
            end
        end

        %-----------------------------------------------------------------%
        function tickLabels = formatEngineeringNotation(~, values)
            % Format numeric values using engineering notation
            % Engineering notation uses multiples of 10^3, 10^6, 10^9, etc.
            % Returns cell array of formatted strings
            
            tickLabels = cell(size(values));
            
            for i = 1:length(values)
                val = values(i);
                absVal = abs(val);
                
                % Determine the engineering unit
                if absVal >= 1e9
                    exponent = 9;
                    suffix = 'G';
                elseif absVal >= 1e6
                    exponent = 6;
                    suffix = 'M';
                elseif absVal >= 1e3
                    exponent = 3;
                    suffix = 'k';
                elseif absVal < 1e-9
                    exponent = -12;
                    suffix = 'p';
                elseif absVal < 1e-6
                    exponent = -9;
                    suffix = 'n';
                elseif absVal < 1e-3
                    exponent = -6;
                    suffix = 'μ';
                else
                    exponent = 0;
                    suffix = '';
                end
                
                % Scale value and format
                if exponent == 0
                    scaledVal = val;
                    tickLabels{i} = sprintf('%.1f', scaledVal);
                else
                    scaledVal = val / (10^exponent);
                    tickLabels{i} = sprintf('%.1f%s', scaledVal, suffix);
                end
            end
        end
        
        %-----------------------------------------------------------------%
        function float32Array = base64ToFloat32(~, base64String)
            % Convert base64 string to float32 array
            bytes = matlab.net.base64decode(base64String);
            
            if mod(length(bytes), 4) ~= 0
                error('Invalid base64 data: byte length must be divisible by 4 for float32');
            end
            
            float32Array = typecast(bytes, 'single');
        end

                %-----------------------------------------------------------------%
        function flashButtonColor(app, button, newColor, duration)
            % Temporarily change button background color for specified duration
            % button: the button to flash
            % newColor: RGB color array [R G B] with values 0-1
            % duration: time in seconds to display the color change
            
            % Store original background color
            originalColor = button.BackgroundColor;
            
            % Change to new color
            button.BackgroundColor = newColor;
            
            % Create a timer to restore original color after duration
            timer_obj = timer('TimerFcn', @(~,~) set(button, 'BackgroundColor', originalColor), ...
                              'StartDelay', duration, ...
                              'ExecutionMode', 'singleShot');
            start(timer_obj);
            
            % Ensure timer is deleted after execution
            timer_obj.StopFcn = @(~,~) delete(timer_obj);
        end

        %-----------------------------------------------------------------%
        function flashButtonAckNack(app, duration)
            % Change ACK button to red and text to NACK for specified duration
            % duration: time in seconds to display the color change
            
            % Store original properties
            originalColor = app.ACKButton.BackgroundColor;
            originalText = app.ACKButton.Text;
            
            % Change to red and update text
            app.ACKButton.BackgroundColor = [1.0 0.0 0.0];
            app.ACKButton.Text = 'NACK';
            
            % Create a timer to restore original properties after duration
            timer_obj = timer('TimerFcn', @(~,~) restoreButtonProperties(), ...
                              'StartDelay', duration, ...
                              'ExecutionMode', 'singleShot');
            
            % Define nested function to restore properties
            function restoreButtonProperties()
                app.ACKButton.BackgroundColor = originalColor;
                app.ACKButton.Text = originalText;
            end
            
            start(timer_obj);
            
            % Ensure timer is deleted after execution
            timer_obj.StopFcn = @(~,~) delete(timer_obj);
        end

    end

    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function startupFcn(app)
            % load client config, command samples and start app
            loadConf(app);
            pause on
        end

        % Value changed function: CommandDropDown
        function commandDropDownValueChanged(app, event)
            if app.CommandDropDown.ValueIndex > 1
                
                app.sentMsg.Value = app.CommandDropDown.Value;
                write(app.connection,string((app.sentMsg.Value{1})));
                app.repeatCmdIcon.Enable = true;
            else
                app.CommandDropDown.FontColor = [0.80,0.80,0.80];
                cleanSentMsg(app);
            end
        end

        % Image clicked function: repeatCmdIcon
        function repeatCmdClicked(app, event)
            write(app.connection,string((app.sentMsg.Value{1})));
        end

        % Close request function: EtherDLLTest
        function etherDLLTestCloseRequest(app, event)
            delete(app);
        end

        % Value changed function: sentMsg
        function sentMsgValueChanged(app, event)
            app.refresh = true;
        end

        % Image clicked function: cleanReceivedAreaIcon
        function cleanReceivedAreaClicked(app, event)
            app.receivedMsg.Value = "";
        end

        % Value changed function: bandSelectDropDown
        function bandSelectDropDownValueChanged(app, event)
            
            band = app.bandSelectDropDown.Value;

            app.plotSpectrum(app.SPTAxes, app.data.band(band));

        end

        % Value changed function: connectButton
        function connectConnectconnectButtonPushed(app, event)
            app.connectButton.Enable = false;
            if app.connectButton.Value % True value is associated with Disconnected state, i.e. User required to connect
                app.connectButton.Text = 'Connecting...';
                drawnow;              % force immediate UI update before holding into the connect function
                pause(0.01); 
            
                connected = connect(app);
                
                if connected
                    app.connectButton.Text = 'Disconnect';
                else
                    message = newline + "Error connecting to EtherDLL";
                    app.receivedMsg.Value = [message;app.receivedMsg.Value];
                    app.connectButton.Text = 'Connect';
                    app.connectButton.Value = false;
                end

            else % False value is associated with Connected state, i.e. User required to disconnect
                app.connectButton.Text = 'Disconnecting...';
                disconnect(app);
                app.connectButton.Text = 'Connect';
            end            
            app.connectButton.Enable = true;
        end

        % Clicked callback: CommandDropDown
        function commandDropDownClicked(app, event)
            %  change color and first item value to indicate that options
            %  are active.
            app.CommandDropDown.FontColor = [0.0, 0.0, 0.0];
            app.CommandDropDown.Items{1} = '<none>';
        end

        % Image clicked function: cleanParsedAreaIcon
        function cleanParsedAreaIClicked(app, event)
            app.parsedData.Children = [];
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Get the file path for locating images
            pathToMLAPP = fileparts(mfilename('fullpath'));

            % Create EtherDLLTest and hide until all components are created
            app.EtherDLLTest = uifigure('Visible', 'off');
            app.EtherDLLTest.Position = [92 92 854 836];
            app.EtherDLLTest.Name = 'EtherDLL Test';
            app.EtherDLLTest.Icon = fullfile(pathToMLAPP, 'EtherDLL_Icon.png');
            app.EtherDLLTest.CloseRequestFcn = createCallbackFcn(app, @etherDLLTestCloseRequest, true);
            app.EtherDLLTest.HandleVisibility = 'on';

            % Create mainGrid
            app.mainGrid = uigridlayout(app.EtherDLLTest);
            app.mainGrid.ColumnWidth = {'0.7x', 'fit', '3x', '10x'};
            app.mainGrid.RowHeight = {'fit', 'fit', '1x', '3x', 'fit', 'fit', '6x', 'fit', '6x', 'fit'};
            app.mainGrid.BackgroundColor = [1 1 1];

            % Create AOAAxes
            app.AOAAxes = uiaxes(app.mainGrid);
            xlabel(app.AOAAxes, {'Frequency'; '(Hz)'})
            ylabel(app.AOAAxes, {'Angle'; '(degrees from north)'})
            zlabel(app.AOAAxes, 'Z')
            app.AOAAxes.XTick = [];
            app.AOAAxes.YTick = [];
            app.AOAAxes.Layout.Row = [7 8];
            app.AOAAxes.Layout.Column = 4;

            % Create OCCAxes
            app.OCCAxes = uiaxes(app.mainGrid);
            xlabel(app.OCCAxes, {'Frequency'; '(Hz)'})
            ylabel(app.OCCAxes, {'Occupancy'; '(%)'})
            app.OCCAxes.XTick = [];
            app.OCCAxes.YTick = [];
            app.OCCAxes.Layout.Row = [9 10];
            app.OCCAxes.Layout.Column = 4;

            % Create SPTAxes
            app.SPTAxes = uiaxes(app.mainGrid);
            xlabel(app.SPTAxes, {'Frequency'; '(Hz)'})
            ylabel(app.SPTAxes, {'Level'; '(dBm)'})
            zlabel(app.SPTAxes, 'Z')
            app.SPTAxes.XTick = [];
            app.SPTAxes.XTickLabel = '';
            app.SPTAxes.YTick = [];
            app.SPTAxes.Layout.Row = [1 6];
            app.SPTAxes.Layout.Column = 4;

            % Create receivedMsg
            app.receivedMsg = uitextarea(app.mainGrid);
            app.receivedMsg.Tooltip = {'Messages received from EtherDLL'};
            app.receivedMsg.Placeholder = '< will display raw messages received from EtherDLL >';
            app.receivedMsg.Layout.Row = [7 8];
            app.receivedMsg.Layout.Column = [1 3];

            % Create sentMsg
            app.sentMsg = uitextarea(app.mainGrid);
            app.sentMsg.ValueChangedFcn = createCallbackFcn(app, @sentMsgValueChanged, true);
            app.sentMsg.Tooltip = {'Last command sent to  EtherDLL'};
            app.sentMsg.Placeholder = '< will display command allowing changes / repeat >';
            app.sentMsg.Layout.Row = [3 5];
            app.sentMsg.Layout.Column = [1 3];

            % Create CommandDropDown
            app.CommandDropDown = uidropdown(app.mainGrid);
            app.CommandDropDown.Items = {'<select a command>'};
            app.CommandDropDown.ValueChangedFcn = createCallbackFcn(app, @commandDropDownValueChanged, true);
            app.CommandDropDown.Tooltip = {'Select a command and send it to the station through EtherDLL'};
            app.CommandDropDown.FontColor = [0.8 0.8 0.8];
            app.CommandDropDown.Layout.Row = 2;
            app.CommandDropDown.Layout.Column = [1 3];
            app.CommandDropDown.ClickedFcn = createCallbackFcn(app, @commandDropDownClicked, true);
            app.CommandDropDown.Value = '<select a command>';

            % Create repeatCmdIcon
            app.repeatCmdIcon = uiimage(app.mainGrid);
            app.repeatCmdIcon.ScaleMethod = 'scaledown';
            app.repeatCmdIcon.ImageClickedFcn = createCallbackFcn(app, @repeatCmdClicked, true);
            app.repeatCmdIcon.Tooltip = {'Repeat the command'};
            app.repeatCmdIcon.Layout.Row = 5;
            app.repeatCmdIcon.Layout.Column = 1;
            app.repeatCmdIcon.HorizontalAlignment = 'left';
            app.repeatCmdIcon.ImageSource = fullfile(pathToMLAPP, 'redo.svg');

            % Create cleanReceivedAreaIcon
            app.cleanReceivedAreaIcon = uiimage(app.mainGrid);
            app.cleanReceivedAreaIcon.ImageClickedFcn = createCallbackFcn(app, @cleanReceivedAreaClicked, true);
            app.cleanReceivedAreaIcon.Tooltip = {'Clean the output area'};
            app.cleanReceivedAreaIcon.Layout.Row = 8;
            app.cleanReceivedAreaIcon.Layout.Column = 1;
            app.cleanReceivedAreaIcon.HorizontalAlignment = 'left';
            app.cleanReceivedAreaIcon.VerticalAlignment = 'bottom';
            app.cleanReceivedAreaIcon.ImageSource = fullfile(pathToMLAPP, 'sweep.svg');

            % Create bandSelectDropDown
            app.bandSelectDropDown = uidropdown(app.mainGrid);
            app.bandSelectDropDown.Items = {'Band 0'};
            app.bandSelectDropDown.ValueChangedFcn = createCallbackFcn(app, @bandSelectDropDownValueChanged, true);
            app.bandSelectDropDown.Enable = 'off';
            app.bandSelectDropDown.Tooltip = {'Select the band to display'};
            app.bandSelectDropDown.Layout.Row = 6;
            app.bandSelectDropDown.Layout.Column = 3;
            app.bandSelectDropDown.Value = 'Band 0';

            % Create connectButton
            app.connectButton = uibutton(app.mainGrid, 'state');
            app.connectButton.ValueChangedFcn = createCallbackFcn(app, @connectConnectconnectButtonPushed, true);
            app.connectButton.Tooltip = {'Press to connect to EtherDLL'};
            app.connectButton.Text = 'Connect';
            app.connectButton.Layout.Row = 1;
            app.connectButton.Layout.Column = [1 3];

            % Create parsedData
            app.parsedData = uitree(app.mainGrid);
            app.parsedData.Layout.Row = [9 10];
            app.parsedData.Layout.Column = [1 3];

            % Create cleanParsedAreaIcon
            app.cleanParsedAreaIcon = uiimage(app.mainGrid);
            app.cleanParsedAreaIcon.ImageClickedFcn = createCallbackFcn(app, @cleanParsedAreaIClicked, true);
            app.cleanParsedAreaIcon.Tooltip = {'Clean the output area'};
            app.cleanParsedAreaIcon.Layout.Row = 10;
            app.cleanParsedAreaIcon.Layout.Column = 1;
            app.cleanParsedAreaIcon.HorizontalAlignment = 'left';
            app.cleanParsedAreaIcon.VerticalAlignment = 'bottom';
            app.cleanParsedAreaIcon.ImageSource = fullfile(pathToMLAPP, 'sweep.svg');

            % Create PINGButton
            app.PINGButton = uibutton(app.mainGrid, 'push');
            app.PINGButton.Layout.Row = 6;
            app.PINGButton.Layout.Column = 1;
            app.PINGButton.Text = 'PING';

            % Create ACKButton
            app.ACKButton = uibutton(app.mainGrid, 'push');
            app.ACKButton.Layout.Row = 6;
            app.ACKButton.Layout.Column = 2;
            app.ACKButton.Text = 'ACK';

            % Show the figure after all components are created
            app.EtherDLLTest.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = EtherDLLTestClient_exported

            runningApp = getRunningApp(app);

            % Check for running singleton app
            if isempty(runningApp)

                % Create UIFigure and components
                createComponents(app)

                % Register the app with App Designer
                registerApp(app, app.EtherDLLTest)

                % Execute the startup function
                runStartupFcn(app, @startupFcn)
            else

                % Focus the running singleton app
                figure(runningApp.EtherDLLTest)

                app = runningApp;
            end

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.EtherDLLTest)
        end
    end
end
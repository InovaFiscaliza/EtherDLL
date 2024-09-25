function [conf, command_list] = loadConf()
%LOADCONF load app configuration from json file
%   Use a default json file to define constants that control the
%   application and further load command examples in json format to populate the command list
    try
        % try to read the json file
        conf = jsondecode(fileread('appConfig.json'));
    catch
        % use default values if the file is not found
        warning('Failed to read configuration from file appConfig.json');
        conf = struct('log', struct('file', struct('enable', true, 'level', 'trace', 'path', 'log.txt')), ...
                    'proxy', struct('address', 'localhost', 'timeout_s', 10), ...
                    'service', struct('command', struct('port', 3000, 'timeout_s', 10000, 'sleep_ms', 100, 'check_period', 10), ...
                                        'stream', struct('port', 3001, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200), ...
                                        'error', struct('port', 3002, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200), ...
                                        'realtime', struct('port', 3003, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200)));
    end

    % list json files present in the app folder
    files = dir('cmd*.json');

    % load the command list from the json files
    command_list = {};
    for i = 1:length(files)
        try
            % create a command name by removing the initial lettes 'cmd' and the extension '.json' from the file name
            command_name = files(i).name(4:end-5);

            command_list = [command_name, jsondecode(fileread(files(i).name))];
        catch
            % skip the file if it is not in json format
            warning('Failed to read command list from file %s', files(i).name);
            continue;
        end
    end

end
function [conf, drop_down_items,items_data] = loadConf()
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
                    'service', struct('port', 5555, 'timeout_s', 10000, 'sleep_ms', 100, 'check_period', 10));
    end

    % list json files present in the app folder
    files = dir('cmd*.json');

    % load the command list from the json files
    ddsize = length(files)+1;
    drop_down_items{ddsize} = [];
    items_data{ddsize} = [];
 
    drop_down_items{1} = "<select a command>";
    items_data{1} = "";
    for i = 2:ddsize
        try
            % create a command name by removing the initial lettes 'cmd' and the extension '.json' from the file name
            j = i-1;
            drop_down_items{i} = files(j).name(4:end-5);
            items_data{i} = fileread(files(j).name);

        catch
            % skip the file if it is not in json format
            warning('Failed to read command list from file %s', files(i).name);
            continue;
        end
    end

end
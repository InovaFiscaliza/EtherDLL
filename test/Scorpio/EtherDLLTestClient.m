classdef EtherDLLTestClient < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        EtherDLLTest     matlab.ui.Figure
        mainGrid         matlab.ui.container.GridLayout
        cleanArea        matlab.ui.control.Image
        receivedMsg      matlab.ui.control.TextArea
        Label            matlab.ui.control.Label
        repeatCmd        matlab.ui.control.Image
        ConnectButton    matlab.ui.control.StateButton
        CommandDropDown  matlab.ui.control.DropDown
        sentMsg          matlab.ui.control.TextArea
        SPTAxes          matlab.ui.control.UIAxes
        OCCAxes          matlab.ui.control.UIAxes
        AOAAxes          matlab.ui.control.UIAxes
    end

    
    %-----------------------------------------------------------------%
    properties (Constant)
        % app constants -----------------------------------------%
        MHZ_MULTIPLIER = 1000000
        KHZ_MAX_VALUE = 1
        KHZ_FROM_MHZ = 1000
        GHZ_MIN_VALUE = 1000
        GHZ_FROM_MHZ = 0.001

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
        refresh = false % store requests for graph refresh
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
                relPath = relPath(3:end); % Remove leading .\ or ./
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

            app.loadCommandList();
        end

        %-----------------------------------------------------------------%
        function loadCommandList(app)
        %LOADCOMMANDLIST load commands to be sent to EtherDLL
        % use configured folder to get all commands and populate the
        % dropdown control
            

            % list json files present in the app folder
            cd (app.config.service.command_path);
            files = dir('cmd*.json');
        
            % load the command list from the json files
            items_size = length(files)+1;
            cmd_items{items_size} = [];
            cmd_values{items_size} = [];
            
            cmd_items{1} =  '<select a command>';
            cmd_values{1} = '';
        
            for i = 2:items_size
                try
                    % create a command name by removing the initial lettes 'cmd' and the extension '.json' from the file name
                    filename = files(i-1).name;
                    cmd_items{i} = filename(4:end-5);
                    cmd_values{i} = jsonencode(jsondecode(fileread(filename)));
                catch
                    % skip the file if it is not in json format
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
                app.loadCommandList();
                doing_fine = true;
            catch
                warning('Failed to connect to EtherDLL');
                doing_fine = false;
            end      
        end

        %-----------------------------------------------------------------%
        function disconnect(app)
            % Disconnect all channels and reset lamps

            try
                flush(app.connection);
                clear app.connection;
                delete(app.connection);
            catch
                warning('Failed to disconnect from EtheDLL.');
            end
        end

        %-----------------------------------------------------------------%
        function receivedData(app, src, ~)
            % Handle incomming data in the command channel
            raw_data = read(src,src.NumBytesAvailable,"string");
            struct_data = app.processRawData(raw_data);
            app.processServiceData(struct_data);
        end

        %-----------------------------------------------------------------%
        function out_data = processRawData(app, in_data)
            % Process raw data from socket into structures
            
            try
                data = split(in_data,"\r\n");
                if isstring(data)
                    out_data = jsondecode(data);
                else
                    if iscell(data)
                        out_data = data;
                        for i = 1:length(data)
                            out_data(i) = jsondecode(data(i));
                        end
                    end
                end
                app.receivedMsg.Value = [evalc('disp(data)');app.receivedMsg.Value];
            catch
                if in_data == null
                    app.ConnectButton.Value = not(app.ConnectButton.Value);
                    return;
                end
                message = newline + "error parsing data: <" + evalc('disp(data)') + ">";
                app.receivedMsg.Value = [message;app.receivedMsg.Value];
            end
        end

        %-----------------------------------------------------------------%
        function processServiceData(app, in_data)
            % Check for specific keys
            if isfield(in_data, 'PING')
                % Handle PING response
                disp('Received PING');
            elseif isfield(in_data, 'ACK')
                % Handle ACK response
                disp('Received ACK');
            elseif isfield(in_data, 'NACK')
                % Handle NACK response
                disp('Received NACK');
            else
                app.processStationData(in_data);
            end
        end

        %-----------------------------------------------------------------%
            function processStationData(app,data)
            % Process diferent kinds of data from the received data into
            % adjusted structure
            
                switch data.CODE
                    case app.AVD
                        warning("No processing defined")
                    case app.MEASURE
                        warning("No processing defined")
                    case app.OCC
                        warning("No processing defined")
                    case app.OCC_DF
                        warning("No processing defined")
                    case app.PAN
                        data.spectrum.powerLevel = app.base64ToFloat32(data.spectrum.sweepData);
                        data.spectrum.frequencyAxis = linspace(data.spectrum.startFrequency, data.spectrum.stopFrequency, data.spectrum.numBins);
                        app.plotSpectrum(data);
                    case app.SET_AUDIO
                        warning("No processing defined")
                    otherwise
                        warning("Received " + data);
                        warning("No processing defined for received data type");
                end
            end

        %-----------------------------------------------------------------%
        function plotSpectrum(app, data)
            % plot spectrum data
            if app.refresh
                hold(app.SPTAxes,"off");
                app.refresh = false;
            else
                hold(app.SPTAxes,"on");
            end
            
            xlim(app.SPTAxes,[data.spectrum.startFrequency data.spectrum.stopFrequency]);
            xlabel(app.SPTAxes,{"Frequency",data.spectrum.frequencyUnit});
            plot(app.SPTAxes,data.spectrum.frequencyAxis,data.spectrum.powerLevel,'-', 'Color',[0.2 0.5 0.9 0.1], 'LineWidth',2.5);

            % fields = {'frequencyAxis', 'powerLevel','sweepData'};
            % data.spectrum = rmfield(data.spectrum, fields);

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
    end

    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function startupFcn(app)
            % load client config, command samples and start app
            app.loadConf();
            pause on
        end

        % Value changed function: ConnectButton
        function ConnectButtonValueChanged(app, ~)
            app.ConnectButton.Enable = false;
            if app.ConnectButton.Value % True value is associated with Disconnected state, i.e. User required to connect
                connected = app.connect();
                
                if connected
                    app.ConnectButton.Text = 'Disconnect';
                    app.ConnectButton.Enable = true;
                else
                    app.ConnectButton.Value = false;
                    app.ConnectButton.Enable = true;
                end

            else % False value is associated with Connected state, i.e. User required to disconnect
                app.ConnectButton.Enable = false;
                app.disconnect();
                app.ConnectButton.Text = 'Connect';
                app.ConnectButton.Enable = true;
            end
        end

        % Clicked callback: CommandDropDown
        function CommandDropDownClicked(app, ~)
            %  change color and first item value to indicate that options
            %  are active.
            app.CommandDropDown.FontColor = [0.0, 0.0, 0.0];
            app.CommandDropDown.Items{1} = '<none>';
        end

        % Value changed function: CommandDropDown
        function CommandDropDownValueChanged(app, ~)
            if app.CommandDropDown.ValueIndex > 1
                
                app.sentMsg.Value = app.CommandDropDown.Value;
                write(app.connection,string((app.sentMsg.Value{1})));
            else
                app.CommandDropDown.FontColor = [0.80,0.80,0.80];
                app.CommandDropDown.Items{1} = '<select a command>';
                app.sentMsg.Value = "";
                cla(app.SPTAxes);
                cla(app.OCCAxes);
                cla(app.AOAAxes);
            end
        end

        % Image clicked function: repeatCmd
        function repeatCmdClicked(app, ~)
            write(app.connection,string((app.sentMsg.Value{1})));
        end

        % Close request function: EtherDLLTest
        function EtherDLLTestCloseRequest(app, ~)
            delete(app);
        end

        % Value changed function: sentMsg
        function sentMsgValueChanged(app, ~)
            app.refresh = true;
        end

        % Image clicked function: cleanArea
        function cleanAreaClicked(app, ~)
            app.receivedMsg.Value = "";
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
            app.EtherDLLTest.CloseRequestFcn = createCallbackFcn(app, @EtherDLLTestCloseRequest, true);
            app.EtherDLLTest.HandleVisibility = 'on';

            % Create mainGrid
            app.mainGrid = uigridlayout(app.EtherDLLTest);
            app.mainGrid.ColumnWidth = {'fit', 'fit', 'fit', 'fit', 'fit', '10x'};
            app.mainGrid.RowHeight = {'fit', 'fit', '1x', '2x', 'fit', 'fit', '1x', '8x', '6x', 'fit'};
            app.mainGrid.BackgroundColor = [1 1 1];

            % Create AOAAxes
            app.AOAAxes = uiaxes(app.mainGrid);
            xlabel(app.AOAAxes, {'Frequency'; '(MHz)'})
            ylabel(app.AOAAxes, {'Angle'; '(degrees from north)'})
            zlabel(app.AOAAxes, 'Z')
            app.AOAAxes.Layout.Row = 8;
            app.AOAAxes.Layout.Column = 6;

            % Create OCCAxes
            app.OCCAxes = uiaxes(app.mainGrid);
            xlabel(app.OCCAxes, {'Frequency'; '(MHz)'})
            ylabel(app.OCCAxes, {'Occupancy'; '(%)'})
            app.OCCAxes.Layout.Row = [9 10];
            app.OCCAxes.Layout.Column = 6;

            % Create SPTAxes
            app.SPTAxes = uiaxes(app.mainGrid);
            xlabel(app.SPTAxes, {'Frequency'; '(MHz)'})
            ylabel(app.SPTAxes, {'Level'; '(dBm)'})
            zlabel(app.SPTAxes, 'Z')
            app.SPTAxes.Layout.Row = [1 7];
            app.SPTAxes.Layout.Column = 6;

            % Create sentMsg
            app.sentMsg = uitextarea(app.mainGrid);
            app.sentMsg.ValueChangedFcn = createCallbackFcn(app, @sentMsgValueChanged, true);
            app.sentMsg.Placeholder = '< will display command allowing changes / repeat >';
            app.sentMsg.Layout.Row = [3 6];
            app.sentMsg.Layout.Column = [1 5];

            % Create CommandDropDown
            app.CommandDropDown = uidropdown(app.mainGrid);
            app.CommandDropDown.Items = {'<select a command>'};
            app.CommandDropDown.ValueChangedFcn = createCallbackFcn(app, @CommandDropDownValueChanged, true);
            app.CommandDropDown.Tooltip = {'Select a command and send it to the station'};
            app.CommandDropDown.FontColor = [0.8 0.8 0.8];
            app.CommandDropDown.Layout.Row = 2;
            app.CommandDropDown.Layout.Column = [1 5];
            app.CommandDropDown.ClickedFcn = createCallbackFcn(app, @CommandDropDownClicked, true);
            app.CommandDropDown.Value = '<select a command>';

            % Create ConnectButton
            app.ConnectButton = uibutton(app.mainGrid, 'state');
            app.ConnectButton.ValueChangedFcn = createCallbackFcn(app, @ConnectButtonValueChanged, true);
            app.ConnectButton.Text = 'Connect';
            app.ConnectButton.Layout.Row = 1;
            app.ConnectButton.Layout.Column = [1 5];

            % Create repeatCmd
            app.repeatCmd = uiimage(app.mainGrid);
            app.repeatCmd.ScaleMethod = 'scaledown';
            app.repeatCmd.ImageClickedFcn = createCallbackFcn(app, @repeatCmdClicked, true);
            app.repeatCmd.Tooltip = {'Repeat the command'};
            app.repeatCmd.Layout.Row = 6;
            app.repeatCmd.Layout.Column = 1;
            app.repeatCmd.HorizontalAlignment = 'left';
            app.repeatCmd.ImageSource = fullfile(pathToMLAPP, 'redo.svg');

            % Create Label
            app.Label = uilabel(app.mainGrid);
            app.Label.HorizontalAlignment = 'right';
            app.Label.Layout.Row = [7 10];
            app.Label.Layout.Column = [1 5];
            app.Label.Text = '';

            % Create receivedMsg
            app.receivedMsg = uitextarea(app.mainGrid);
            app.receivedMsg.Placeholder = '< will display raw messages received from EtherDLL >';
            app.receivedMsg.Layout.Row = [7 10];
            app.receivedMsg.Layout.Column = [1 5];

            % Create cleanArea
            app.cleanArea = uiimage(app.mainGrid);
            app.cleanArea.ImageClickedFcn = createCallbackFcn(app, @cleanAreaClicked, true);
            app.cleanArea.Layout.Row = 10;
            app.cleanArea.Layout.Column = 1;
            app.cleanArea.HorizontalAlignment = 'left';
            app.cleanArea.VerticalAlignment = 'bottom';
            app.cleanArea.ImageSource = fullfile(pathToMLAPP, 'sweep.svg');

            % Show the figure after all components are created
            app.EtherDLLTest.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = EtherDLLTestClient

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
classdef EtherDLLTestClient_exported < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        EtherDLLTest        matlab.ui.Figure
        mainGrid            matlab.ui.container.GridLayout
        connectButton       matlab.ui.control.StateButton
        bandSelectDropDown  matlab.ui.control.DropDown
        cleanAreaIcon       matlab.ui.control.Image
        receivedMsg         matlab.ui.control.TextArea
        Label               matlab.ui.control.Label
        repeatCmdIcon       matlab.ui.control.Image
        CommandDropDown     matlab.ui.control.DropDown
        sentMsg             matlab.ui.control.TextArea
        AOAAxes             matlab.ui.control.UIAxes
        OCCAxes             matlab.ui.control.UIAxes
        SPTAxes             matlab.ui.control.UIAxes
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
        data % Store plot data to allow band switching
        refresh = false; % store requests for graph refresh
        pendingConnection = true; % store connection state, good knows why state button stopped working 
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

            app.loadCommandList();
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
                app.loadCommandList();
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
                app.cleanSentMsg;
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
                mid_data = split(in_data,"\r\n");
                if isstring(mid_data)
                    out_data = jsondecode(mid_data);
                else
                    out_data = mid_data;
                    if iscell(mid_data)
                        for i = 1:length(mid_data)
                            out_data(i) = jsondecode(mid_data(i));
                        end
                    end
                end
                app.receivedMsg.Value = [evalc('disp(in_data)');app.receivedMsg.Value];
            catch
                if in_data == null
                    app.connectButton.Value = not(app.connectButton.Value);
                    message = newline + "Error - Not Connected to EtherDLL";
                else
                    message = newline + "Error - Non JSON data: <" + evalc('disp(in_data)') + ">";
                end
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
                        
                        app.data = data;

                        for k = 1:numel(app.data.band)
                            s = data.band(k).spectrum;

                            s.powerLevel = app.base64ToFloat32(s.sweepData);
                            s.frequencyAxis = linspace(s.startFrequency, s.stopFrequency, s.numBins);

                            app.data.band(k).spectrum = s;
                        end

                        % Update band dropdown options (1..N)
                        nBands = numel(app.data.band);
                        if (nBands > 1)
                            app.bandSelectDropDown.Enable = "on";
                            app.bandSelectDropDown.Items = arrayfun(@(k) sprintf('Band %d', k-1), 1:nBands, 'UniformOutput', false);
                            app.bandSelectDropDown.ItemsData = 1:nBands;
                            app.bandSelectDropDown.Value =  "Band 0";
                        else
                            app.bandSelectDropDown.Items = {"Band 0"};
                            app.bandSelectDropDown.Value = "Band 0";
                            app.bandSelectDropDown.ItemsData = 0;
                            app.bandSelectDropDown.Enable = "off";
                        end

                        app.plotSpectrum(app.data.band(0));

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

        % Value changed function: CommandDropDown
        function CommandDropDownValueChanged(app, event)
            if app.CommandDropDown.ValueIndex > 1
                
                app.sentMsg.Value = app.CommandDropDown.Value;
                write(app.connection,string((app.sentMsg.Value{1})));
                app.repeatCmdIcon.Enable = true;
            else
                app.CommandDropDown.FontColor = [0.80,0.80,0.80];
                app.cleanSentMsg();
            end
        end

        % Image clicked function: repeatCmdIcon
        function repeatCmdClicked(app, event)
            write(app.connection,string((app.sentMsg.Value{1})));
        end

        % Close request function: EtherDLLTest
        function EtherDLLTestCloseRequest(app, event)
            delete(app);
        end

        % Value changed function: sentMsg
        function sentMsgValueChanged(app, event)
            app.refresh = true;
        end

        % Image clicked function: cleanAreaIcon
        function cleanAreaClicked(app, event)
            app.receivedMsg.Value = "";
        end

        % Value changed function: bandSelectDropDown
        function bandSelectDropDownValueChanged(app, event)
            
            band = app.bandSelectDropDown.Value;

            app.plotSpectrum(app.data.band(band));

        end

        % Value changed function: connectButton
        function ConnectConnectconnectButtonPushed(app, event)
            app.connectButton.Enable = false;
            if app.connectButton.Value % True value is associated with Disconnected state, i.e. User required to connect
                app.connectButton.Text = 'Connecting...';
                drawnow;              % force immediate UI update before holding into the connect function
                pause(0.01); 
            
                connected = app.connect();
                
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
                app.disconnect();
                app.connectButton.Text = 'Connect';
            end            
            app.connectButton.Enable = true;
        end

        % Clicked callback: CommandDropDown
        function CommandDropDownClicked(app, event)
            %  change color and first item value to indicate that options
            %  are active.
            app.CommandDropDown.FontColor = [0.0, 0.0, 0.0];
            app.CommandDropDown.Items{1} = '<none>';
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
            app.mainGrid.ColumnWidth = {'fit', 'fit', '3x', '10x'};
            app.mainGrid.RowHeight = {'fit', 'fit', '1x', '3x', 'fit', 'fit', '8x', '6x', 'fit'};
            app.mainGrid.BackgroundColor = [1 1 1];

            % Create SPTAxes
            app.SPTAxes = uiaxes(app.mainGrid);
            xlabel(app.SPTAxes, {'Frequency'; '(MHz)'})
            ylabel(app.SPTAxes, {'Level'; '(dBm)'})
            zlabel(app.SPTAxes, 'Z')
            app.SPTAxes.Layout.Row = [1 6];
            app.SPTAxes.Layout.Column = 4;

            % Create OCCAxes
            app.OCCAxes = uiaxes(app.mainGrid);
            xlabel(app.OCCAxes, {'Frequency'; '(MHz)'})
            ylabel(app.OCCAxes, {'Occupancy'; '(%)'})
            app.OCCAxes.Layout.Row = [8 9];
            app.OCCAxes.Layout.Column = 4;

            % Create AOAAxes
            app.AOAAxes = uiaxes(app.mainGrid);
            xlabel(app.AOAAxes, {'Frequency'; '(MHz)'})
            ylabel(app.AOAAxes, {'Angle'; '(degrees from north)'})
            zlabel(app.AOAAxes, 'Z')
            app.AOAAxes.Layout.Row = 7;
            app.AOAAxes.Layout.Column = 4;

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
            app.CommandDropDown.ValueChangedFcn = createCallbackFcn(app, @CommandDropDownValueChanged, true);
            app.CommandDropDown.Tooltip = {'Select a command and send it to the station through EtherDLL'};
            app.CommandDropDown.FontColor = [0.8 0.8 0.8];
            app.CommandDropDown.Layout.Row = 2;
            app.CommandDropDown.Layout.Column = [1 3];
            app.CommandDropDown.ClickedFcn = createCallbackFcn(app, @CommandDropDownClicked, true);
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

            % Create Label
            app.Label = uilabel(app.mainGrid);
            app.Label.HorizontalAlignment = 'right';
            app.Label.Layout.Row = [6 9];
            app.Label.Layout.Column = [1 3];
            app.Label.Text = '';

            % Create receivedMsg
            app.receivedMsg = uitextarea(app.mainGrid);
            app.receivedMsg.Tooltip = {'Messages received from EtherDLL'};
            app.receivedMsg.Placeholder = '< will display raw messages received from EtherDLL >';
            app.receivedMsg.Layout.Row = [7 9];
            app.receivedMsg.Layout.Column = [1 3];

            % Create cleanAreaIcon
            app.cleanAreaIcon = uiimage(app.mainGrid);
            app.cleanAreaIcon.ImageClickedFcn = createCallbackFcn(app, @cleanAreaClicked, true);
            app.cleanAreaIcon.Tooltip = {'Clean the output area'};
            app.cleanAreaIcon.Layout.Row = 9;
            app.cleanAreaIcon.Layout.Column = 1;
            app.cleanAreaIcon.HorizontalAlignment = 'left';
            app.cleanAreaIcon.VerticalAlignment = 'bottom';
            app.cleanAreaIcon.ImageSource = fullfile(pathToMLAPP, 'sweep.svg');

            % Create bandSelectDropDown
            app.bandSelectDropDown = uidropdown(app.mainGrid);
            app.bandSelectDropDown.Items = {'Band 0'};
            app.bandSelectDropDown.ValueChangedFcn = createCallbackFcn(app, @bandSelectDropDownValueChanged, true);
            app.bandSelectDropDown.Enable = 'off';
            app.bandSelectDropDown.Tooltip = {'Select the band to display'};
            app.bandSelectDropDown.Layout.Row = 6;
            app.bandSelectDropDown.Layout.Column = [1 3];
            app.bandSelectDropDown.Value = 'Band 0';

            % Create connectButton
            app.connectButton = uibutton(app.mainGrid, 'state');
            app.connectButton.ValueChangedFcn = createCallbackFcn(app, @ConnectConnectconnectButtonPushed, true);
            app.connectButton.Tooltip = {'Press to connect to EtherDLL'};
            app.connectButton.Text = 'Connect';
            app.connectButton.Layout.Row = 1;
            app.connectButton.Layout.Column = [1 3];

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
classdef appTestMiaer_v0_2 < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        MIAerTestUIFigure  matlab.ui.Figure
        mainGrid           matlab.ui.container.GridLayout
        cleanArea          matlab.ui.control.Image
        CMDText            matlab.ui.control.TextArea
        Label              matlab.ui.control.Label
        repeatCmd          matlab.ui.control.Image
        ConnectButton      matlab.ui.control.StateButton
        CommandDropDown    matlab.ui.control.DropDown
        LastCmdText        matlab.ui.control.TextArea
        AOAAxes            matlab.ui.control.UIAxes
        OCCAxes            matlab.ui.control.UIAxes
        SPTAxes            matlab.ui.control.UIAxes
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
    end

    %-----------------------------------------------------------------%
    properties (Access = public)
        config % Store configuration structure read from json file at startup
        connection = cell(5,1)% Store command channel socket connection
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
                                                'service', struct('command', struct('port', 3000, 'timeout_s', 10000, 'sleep_ms', 100, 'check_period', 10,'path','./GitHub/MIAerConnVS/test'), ...
                                                                    'stream', struct('port', 3001, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200), ...
                                                                    'error', struct('port', 3002, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200), ...
                                                                    'realtime', struct('port', 3003, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200)));
            end

            % Set absolute path for command.path
            relPath = app.config.service.command.path;
            if startsWith(relPath, '.\') || startsWith(relPath, './')
                scriptDir = fileparts(mfilename('fullpath'));
                relPath = relPath(3:end); % Remove leading .\ or ./
                app.config.service.command.path = fullfile(scriptDir, relPath);
            end

            if ~exist(app.config.service.command.path, 'dir') || exist(app.config.service.command.path, 'dir') ~= 7
                warning('Command path "%s" does not exist or is not a valid directory.\nApplication will exit.', app.config.service.command.path);
                
                % Show error dialog to user
                uialert(app.MIAerTestUIFigure, errMsg, 'Configuration Error', 'Icon', 'error');
                
                % Close the app
                delete(app);
                return;
            end

            % list json files present in the app folder
            cd (app.config.service.command.path);
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
                    cmd_values{i} = fileread(filename);
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
        function lamp = lampSwitch(app,lamp,colour)
            % Change lamp colour according to index and value
            % If no colour is passed, retrive the colour currently in use
            switch lamp
                case app.CHANNEL.CMD
                    if isempty(colour)
                        lamp = struct('lamp',app.CMDButton.BackgroundColor,'text',app.CMDButton.FontColor);
                    else
                        app.CMDButton.BackgroundColor = colour.lamp;
                        app.CMDButton.FontColor = colour.text;
                    end
                case app.CHANNEL.DCB
                    if isempty(colour)
                        lamp = struct('lamp',app.DCBButton.BackgroundColor,'text',app.DCBButton.FontColor);
                    else
                        app.DCBButton.BackgroundColor = colour.lamp;
                        app.DCBButton.FontColor = colour.text;
                    end
                case app.CHANNEL.RTD
                    if isempty(colour)
                        lamp = struct('lamp',app.RTDButton.BackgroundColor,'text',app.RTDButton.FontColor);
                    else
                         app.RTDButton.BackgroundColor = colour.lamp;
                         app.RTDButton.FontColor = colour.text;
                    end
                case app.CHANNEL.ERR
                    if isempty(colour)
                        lamp = struct('lamp',app.ERRButton.BackgroundColor,'text',app.ERRButton.FontColor);
                    else
                         app.ERRButton.BackgroundColor = colour.lamp;
                         app.ERRButton.FontColor = colour.text;
                    end
                case app.CHANNEL.SND
                    if isempty(colour)
                        lamp = struct('lamp',app.SNDButton.BackgroundColor,'text',app.SNDButton.FontColor);
                    else
                         app.SNDButton.BackgroundColor = colour.lamp;
                         app.SNDButton.FontColor = colour.text;
                    end
            end
        end

        %-----------------------------------------------------------------%
        function fakeTab(app, selection)
            % Simulate tab behaviour without using tab component

            app.Gauge.Visible = "off";
            app.Knob.Visible ="off";
            app.ERRText.Visible = "off";
            app.RTDText.Visible = "off";
            app.DCBText.Visible = "off";
            app.CMDText.Visible = "off";

            switch selection
                case app.CHANNEL.CMD
                    app.CMDText.Visible = "on";
                    app.selectedTab = app.CHANNEL.CMD;
                case app.CHANNEL.DCB
                    app.DCBText.Visible = "on";
                    app.selectedTab = app.CHANNEL.DCB;
                case app.CHANNEL.RTD
                    app.RTDText.Visible = "on";
                    app.selectedTab = app.CHANNEL.RTD;
                case app.CHANNEL.ERR
                    app.ERRText.Visible = "on";
                    app.selectedTab = app.CHANNEL.ERR;
                case app.CHANNEL.SND
                    app.Gauge.Visible = "on";
                    app.Knob.Visible ="on";
                    app.selectedTab = app.CHANNEL.SND;
            end
        end

        %-----------------------------------------------------------------%
        function doing_fine = connect(app)
            % Connect the various socket interfaces desginated as channels
            % and set corresponding lamp indication in the UI

            try
                app.connection{app.CHANNEL.CMD} = tcpclient(app.config.proxy.address, app.config.service.command.port, 'Timeout', app.config.service.command.timeout_s, 'ConnectTimeout', app.config.service.command.timeout_s, 'Tag', 'cmd');
                configureCallback(app.connection{app.CHANNEL.CMD},"terminator",@app.cmdChannelCB);
                configureTerminator(app.connection{app.CHANNEL.CMD},app.PACK_END);
                app.lampSwitch(app.CHANNEL.CMD,app.GREEN);
                doing_fine = true;
            catch
                doing_fine = false;
                warning('Failed to connect to the command channel.');
                app.lampSwitch(app.CHANNEL.CMD,app.RED);
                app.connection{app.CHANNEL.CMD} = [];
            end
        
            if doing_fine
                try
                    app.connection{app.CHANNEL.DCB} = tcpclient(app.config.proxy.address,app.config.service.data.port, 'Timeout',app.config.service.data.timeout_s, 'ConnectTimeout',app.config.service.data.timeout_s, 'Tag', 'dcb');
                    configureCallback(app.connection{app.CHANNEL.DCB},"terminator",@app.dcbChannelCB);
                    configureTerminator(app.connection{app.CHANNEL.DCB},app.PACK_END);
                    app.lampSwitch(app.CHANNEL.DCB,app.GREEN);
                catch
                    doing_fine = false;
                    warning('Failed to connect to the data channel');
                    app.lampSwitch(app.CHANNEL.DCB,app.RED);
                    app.connection{app.CHANNEL.DCB} = [];
                end
            end

            if doing_fine
                try
                    app.connection{app.CHANNEL.RTD} = tcpclient(app.config.proxy.address,app.config.service.realtime.port, 'Timeout',app.config.service.realtime.timeout_s, 'ConnectTimeout',app.config.service.realtime.timeout_s, 'Tag', 'rtdcb');
                    configureCallback(app.connection{app.CHANNEL.RTD},"terminator",@app.rtdChannelCB);
                    configureTerminator(app.connection{app.CHANNEL.RTD},app.PACK_END);
                    app.lampSwitch(app.CHANNEL.RTD,app.GREEN);
                catch
                    doing_fine = false;
                    warning('Failed to connect to the realtime channel');
                    app.lampSwitch(app.CHANNEL.RTD,app.RED);
                    app.connection{app.CHANNEL.RTD} = [];
                end
            end
        
            if doing_fine
                try
                    app.connection{app.CHANNEL.ERR} = tcpclient(app.config.proxy.address,app.config.service.error.port, 'Timeout',app.config.service.error.timeout_s, 'ConnectTimeout',app.config.service.error.timeout_s, 'Tag', 'ercb');
                    configureCallback(app.connection{app.CHANNEL.ERR},"terminator",@app.errChannelCB);
                    configureTerminator(app.connection{app.CHANNEL.ERR},app.PACK_END);
                    app.lampSwitch(app.CHANNEL.ERR,app.GREEN);
                catch
                    doing_fine = false;
                    warning('Failed to connect to the error channel');
                    app.lampSwitch(app.CHANNEL.ERR,app.RED);
                    app.connection{app.CHANNEL.ERR} = [];
                end
            end
        
            %{
            if doing_fine
                try
                    app.connection{app.CHANNEL.SND} = tcpclient(app.config.proxy.address,app.config.service.sound.port, 'Timeout',app.config.service.sound.timeout_s, 'ConnectTimeout',app.config.service.sound.timeout_s, 'Tag', 'snd');
                    configureCallback(app.connection{app.CHANNEL.SND},"terminator",@app.sndChannelCB);
                    configureTerminator(app.connection{app.CHANNEL.SND},app.PACK_END);                
                    app.lampSwitch(app.CHANNEL.SND,app.GREEN);
                catch
                    doing_fine = false;
                    warning('Failed to connect to the sound channel');
                    app.lampSwitch(app.CHANNEL.SND,app.RED);
                    app.connection{app.CHANNEL.SND} = [];
                end 
            end
            %}
        end

        %-----------------------------------------------------------------%
        function disconnect(app)
            % Disconnect all channels and reset lamps

            for i = app.CHANNEL.CMD:app.CHANNEL.ERR % CHANGE TO INCLUDE SOUND
                try
                    flush(app.connection{i});
                    clear app.connection{i};
                    delete(app.connection{i});
                    app.lampSwitch(i,app.RED);
                catch
                    warning('Failed to disconnect from %s channel.',app.CHANNEL_NAME{i});
                    app.lampSwitch(i,app.YELLOW_LAMP);
                end
            end
        end

        %-----------------------------------------------------------------%
        function cmdChannelCB(app, src, ~)
            % Handle incomming data in the command channel
            app.lampSwitch(app.CHANNEL.CMD,app.BLUE);
            data = read(src,src.NumBytesAvailable,"string");
            app.CMDText.Value = [data; app.CMDText.Value];
            % app.fakeTab(app.CHANNEL.CMD);
            pause(1);
            app.lampSwitch(app.CHANNEL.CMD,app.GREEN);
        end

        %-----------------------------------------------------------------%
        function dcbChannelCB(app, src, ~)
            % Handle incomming data in the data calback channel
            app.lampSwitch(app.CHANNEL.DCB,app.BLUE);
            data = read(src,src.NumBytesAvailable,"string");
            app.fakeTab(app.CHANNEL.DCB);
            data = app.processRawData(data);
            data = app.processDataTypes(data);
            app.presentData(data);
            
            app.lampSwitch(app.CHANNEL.DCB,app.GREEN);
        end

        %-----------------------------------------------------------------%
        function rtdChannelCB(app, src, ~)
            % Handle incomming data in the data calback channel
            app.lampSwitch(app.CHANNEL.RTD,app.BLUE);
            data = read(src,src.NumBytesAvailable,"string");
            app.RTDText.Value = [data;app.RTDText.Value];
            app.fakeTab(app.CHANNEL.RTD);
            app.lampSwitch(app.CHANNEL.RTD,app.GREEN);
        end

%{ Notes and Drafting area
%}
        %-----------------------------------------------------------------%
        function errChannelCB(app, src, ~)
            % Handle incomming data in the data calback channel
            app.lampSwitch(app.CHANNEL.ERR,app.BLUE);
            data = read(src,src.NumBytesAvailable,"string");
            app.ERRText.Value = [data;app.ERRText.Value];
            app.fakeTab(app.CHANNEL.ERR);
            app.lampSwitch(app.CHANNEL.ERR,app.GREEN);
        end

        %-----------------------------------------------------------------%
        function sndChannelCB(app, src, ~)
            % Handle incomming data in the data calback channel
            app.lampSwitch(app.CHANNEL.SND,app.BLUE);
            data =  read(src,src.NumBytesAvailable,"uint8");
            app.Gauge = data;
            app.fakeTab(app.CHANNEL.SND)
            app.lampSwitch(app.CHANNEL.SND,app.GREEN);
        end

        %-----------------------------------------------------------------%
        function out_data = processRawData(~, in_data)
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
            catch
                warning("error parsing JSON data")
            end
        end

        %-----------------------------------------------------------------%
        function out_data = processDataTypes(app,in_data)
            % Process diferent kinds of data from the received data into
            % adjusted structure

            out_data = in_data;

            switch in_data.respType
                case app.PAN
                    out_data.spectrum.powerLevel = app.base64ToFloat32(out_data.spectrum.sweepData);
                    out_data.spectrum.frequencyAxis = linspace(out_data.spectrum.startFrequency, out_data.spectrum.stopFrequency, out_data.spectrum.numBins);
                case app.S_STATE_RESP

                case app.S_SCAN_DF_VS_CHANNEL

                otherwise
                    out_data = json_data;
            end

        end
        %-----------------------------------------------------------------%
        function presentData(app,data)
            % Process diferent kinds of data from the received data into
            % adjusted structure
            
            switch data.respType
                case app.PAN
                    app.plotSpectrum(data);
                    fields = {'frequencyAxis', 'powerLevel','sweepData'};
                    data.spectrum = rmfield(data.spectrum, fields);
                    app.DCBText.Value = [jsonencode(data);app.DCBText.Value];
                otherwise
                    app.DCBText.Value = [data;app.DCBText.Value];
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
        function ConnectButtonValueChanged(app, event)
            app.ConnectButton.Enable = false;
            if app.ConnectButton.Value
                connected = app.connect();
                if connected                    
                    app.ConnectButton.Text = 'Disconnect';
                    app.ConnectButton.Enable = true;
                else
                    warndlg(sprintf("Could not connect to server.\nPlease check server connection and operation and try again."),'Connection Failed');
                    app.ConnectButton.Value = false;
                    app.ConnectButton.Enable = true;
                end
            else
                app.ConnectButton.Enable = false;
                app.disconnect();
                app.ConnectButton.Text = 'Connect';
                app.ConnectButton.Enable = true;
            end
        end

        % Clicked callback: CommandDropDown
        function CommandDropDownClicked(app, event)
            %  change color and first item value to indicate that options
            %  are active.
            app.CommandDropDown.FontColor = [0.0, 0.0, 0.0];
            app.CommandDropDown.Items{1} = '<none>';
        end

        % Value changed function: CommandDropDown
        function CommandDropDownValueChanged(app, event)
            if app.CommandDropDown.ValueIndex > 1
                
                app.LastCmdText.Value = app.CommandDropDown.Value;
                write(app.connection{app.CHANNEL.CMD},app.CommandDropDown.Value);
            else
                app.CommandDropDown.FontColor = [0.80,0.80,0.80];
                app.CommandDropDown.Items{1} = '<select a command>';
                app.LastCmdText.Value = "";
                cla(app.SPTAxes);
                cla(app.OCCAxes);
                cla(app.AOAAxes);
            end
        end

        % Image clicked function: repeatCmd
        function repeatCmdClicked(app, event)
            app.fakeTab(app.CHANNEL.CMD)
            write(app.connection{1},string((app.LastCmdText.Value{1})));
        end

        % Close request function: MIAerTestUIFigure
        function MIAerTestUIFigureCloseRequest(app, event)
            delete(app);
        end

        % Value changed function: LastCmdText
        function LastCmdTextValueChanged(app, event)
            app.refresh = true;
        end

        % Image clicked function: cleanArea
        function cleanAreaClicked(app, event)
            app.CMDText.Value = "";
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Get the file path for locating images
            pathToMLAPP = fileparts(mfilename('fullpath'));

            % Create MIAerTestUIFigure and hide until all components are created
            app.MIAerTestUIFigure = uifigure('Visible', 'off');
            app.MIAerTestUIFigure.Position = [92 92 854 836];
            app.MIAerTestUIFigure.Name = 'MIAerTest';
            app.MIAerTestUIFigure.Icon = fullfile(pathToMLAPP, 'networklocationicon-200x200.png');
            app.MIAerTestUIFigure.CloseRequestFcn = createCallbackFcn(app, @MIAerTestUIFigureCloseRequest, true);
            app.MIAerTestUIFigure.HandleVisibility = 'on';

            % Create mainGrid
            app.mainGrid = uigridlayout(app.MIAerTestUIFigure);
            app.mainGrid.ColumnWidth = {'fit', 'fit', 'fit', 'fit', 'fit', '10x'};
            app.mainGrid.RowHeight = {'fit', 'fit', '1x', '2x', 'fit', 'fit', '1x', '8x', '6x', 'fit'};
            app.mainGrid.BackgroundColor = [1 1 1];

            % Create SPTAxes
            app.SPTAxes = uiaxes(app.mainGrid);
            xlabel(app.SPTAxes, {'Frequency'; '(MHz)'})
            ylabel(app.SPTAxes, {'Level'; '(dBm)'})
            zlabel(app.SPTAxes, 'Z')
            app.SPTAxes.Layout.Row = [1 7];
            app.SPTAxes.Layout.Column = 6;

            % Create OCCAxes
            app.OCCAxes = uiaxes(app.mainGrid);
            xlabel(app.OCCAxes, {'Frequency'; '(MHz)'})
            ylabel(app.OCCAxes, {'Occupancy'; '(%)'})
            app.OCCAxes.Layout.Row = [9 10];
            app.OCCAxes.Layout.Column = 6;

            % Create AOAAxes
            app.AOAAxes = uiaxes(app.mainGrid);
            xlabel(app.AOAAxes, {'Frequency'; '(MHz)'})
            ylabel(app.AOAAxes, {'Angle'; '(degrees from north)'})
            zlabel(app.AOAAxes, 'Z')
            app.AOAAxes.Layout.Row = 8;
            app.AOAAxes.Layout.Column = 6;

            % Create LastCmdText
            app.LastCmdText = uitextarea(app.mainGrid);
            app.LastCmdText.ValueChangedFcn = createCallbackFcn(app, @LastCmdTextValueChanged, true);
            app.LastCmdText.Placeholder = '< will display command allowing changes / repeat >';
            app.LastCmdText.Layout.Row = [3 6];
            app.LastCmdText.Layout.Column = [1 5];

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

            % Create CMDText
            app.CMDText = uitextarea(app.mainGrid);
            app.CMDText.Placeholder = '< will display raw messages received from EtherDLL >';
            app.CMDText.Layout.Row = [7 10];
            app.CMDText.Layout.Column = [1 5];

            % Create cleanArea
            app.cleanArea = uiimage(app.mainGrid);
            app.cleanArea.ImageClickedFcn = createCallbackFcn(app, @cleanAreaClicked, true);
            app.cleanArea.Layout.Row = 10;
            app.cleanArea.Layout.Column = 1;
            app.cleanArea.HorizontalAlignment = 'left';
            app.cleanArea.VerticalAlignment = 'bottom';
            app.cleanArea.ImageSource = fullfile(pathToMLAPP, 'sweep.svg');

            % Show the figure after all components are created
            app.MIAerTestUIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = appTestMiaer_v0_2

            runningApp = getRunningApp(app);

            % Check for running singleton app
            if isempty(runningApp)

                % Create UIFigure and components
                createComponents(app)

                % Register the app with App Designer
                registerApp(app, app.MIAerTestUIFigure)

                % Execute the startup function
                runStartupFcn(app, @startupFcn)
            else

                % Focus the running singleton app
                figure(runningApp.MIAerTestUIFigure)

                app = runningApp;
            end

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.MIAerTestUIFigure)
        end
    end
end
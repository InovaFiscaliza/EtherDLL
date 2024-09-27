classdef appTestMiaer_exported < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure                   matlab.ui.Figure
        GridLayout                 matlab.ui.container.GridLayout
        LeftPanel                  matlab.ui.container.Panel
        GridLayout2                matlab.ui.container.GridLayout
        ConnectButton              matlab.ui.control.StateButton
        Tree                       matlab.ui.container.Tree
        Node                       matlab.ui.container.TreeNode
        Node2                      matlab.ui.container.TreeNode
        Node3                      matlab.ui.container.TreeNode
        Node4                      matlab.ui.container.TreeNode
        DataReceivedTextAreaLabel  matlab.ui.control.Label
        CommandDropDown            matlab.ui.control.DropDown
        CommandDropDownLabel       matlab.ui.control.Label
        SNDLamp                    matlab.ui.control.Lamp
        SNDLampLabel               matlab.ui.control.Label
        ERRLamp                    matlab.ui.control.Lamp
        ERRLampLabel               matlab.ui.control.Label
        DCBLamp                    matlab.ui.control.Lamp
        DCBLampLabel               matlab.ui.control.Label
        RTDLampLabel               matlab.ui.control.Label
        RTDLamp                    matlab.ui.control.Lamp
        CMDLamp                    matlab.ui.control.Lamp
        CMDLampLabel               matlab.ui.control.Label
        RightPanel                 matlab.ui.container.Panel
        GridLayout3                matlab.ui.container.GridLayout
        OCCAxes                    matlab.ui.control.UIAxes
        AOAAxes                    matlab.ui.control.UIAxes
        SPTAxes                    matlab.ui.control.UIAxes
    end

    % Properties that correspond to apps with auto-reflow
    properties (Access = private)
        onePanelWidth = 576;
    end

    
    %-----------------------------------------------------------------%
    properties (Constant)
        CHANNEL_NAME = {'Command','Data Callback','Real Time Data','Error','Sound'}
        
        RED_LAMP = [0.64,0.08,0.18]
        GREEN_LAMP = [0.47,0.67,0.19]
        BLUE_LAMP = [0.30,0.75,0.93]
        YELLOW_LAMP = [0.93,0.69,0.13]
    end

    %-----------------------------------------------------------------%
    properties (Access = public)
        config; % Store configuration structure read from json file at startup
        connection = cell(5,1)% Store command channel socket connection
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
                                                'service', struct('command', struct('port', 3000, 'timeout_s', 10000, 'sleep_ms', 100, 'check_period', 10,'path','D:/Documents/Anatel/Aplicativos/GitHub/MIAerConnVS/test'), ...
                                                                    'stream', struct('port', 3001, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200), ...
                                                                    'error', struct('port', 3002, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200), ...
                                                                    'realtime', struct('port', 3003, 'timeout_s', 10000, 'sleep_ms', 500, 'check_period', 200)));
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
        function lamp_switch(app,lamp,colour)
            % Change lamp colour according to index and value
            switch lamp
                case 1
                    app.CMDLamp.Color = colour;
                case 2
                    app.DCBLamp.Color = colour;
                case 3
                    app.RTDLamp.Color = colour;
                case 4
                    app.ERRLamp.Color = colour;
                case 5
                    app.SNDLamp.Color = colour;
            end
        end

        %-----------------------------------------------------------------%
        function connect(app)
            % Connect the various socket interfaces desginated as channels
            % and set corresponding lamp indication in the UI

            try
                app.connection{1} = tcpclient(app.config.proxy.address, app.config.service.command.port, 'Timeout', app.config.service.command.timeout_s, 'ConnectTimeout', app.config.service.command.timeout_s, 'Tag', 'cmd');
                app.lamp_switch(1,app.GREEN_LAMP);
            catch
                warning('Failed to connect to the command channel');
                app.lamp_switch(1,app.RED_LAMP);
                app.connection{1} = [];
            end
        
            try
                app.connection{2} = tcpclient(app.config.proxy.address,app.config.service.data.port, 'Timeout',app.config.service.data.timeout_s, 'ConnectTimeout',app.config.service.data.timeout_s, 'Tag', 'dcb');
                app.lamp_switch(2,app.GREEN_LAMP);
            catch
                warning('Failed to connect to the data channel');
                app.lamp_switch(2,app.RED_LAMP);
                app.connection{2} = [];
            end

            try
                app.connection{3} = tcpclient(app.config.proxy.address,app.config.service.realtime.port, 'Timeout',app.config.service.realtime.timeout_s, 'ConnectTimeout',app.config.service.realtime.timeout_s, 'Tag', 'rtdcb');
                app.lamp_switch(3,app.GREEN_LAMP);
            catch
                warning('Failed to connect to the realtime channel');
                app.lamp_switch(3,app.RED_LAMP);
                app.connection{3} = [];
            end        
        
            try
                app.connection{4} = tcpclient(app.config.proxy.address,app.config.service.error.port, 'Timeout',app.config.service.error.timeout_s, 'ConnectTimeout',app.config.service.error.timeout_s, 'Tag', 'ercb');
                app.lamp_switch(4,app.GREEN_LAMP);
            catch
                warning('Failed to connect to the error channel');
                app.lamp_switch(4,app.RED_LAMP);
                app.connection{4} = [];
            end
        
            try
                app.connection{5} = tcpclient(app.config.proxy.address,app.config.service.sound.port, 'Timeout',app.config.service.sound.timeout_s, 'ConnectTimeout',app.config.service.sound.timeout_s, 'Tag', 'snd');
                app.lamp_switch(5,app.GREEN_LAMP);
            catch
                warning('Failed to connect to the sound channel');
                app.lamp_switch(5,app.RED_LAMP);
                app.connection{5} = [];
            end            
        end

        %-----------------------------------------------------------------%
        function disconnect(app)
            % Disconnect all channels and reset lamps

            for i = 1:5
                try
                    flush(app.connection{i});
                    clear app.connection{i};
                    delete(app.connection{i});
                    app.lamp_switch(i,app.RED_LAMP);
                catch
                    warning('Failed to disconnect from %s channel.',app.CHANNEL_NAME{i});
                    app.lamp_switch(i,app.YELLOW_LAMP);
                end
            end
        end

    end


    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function startupFcn(app)
            % load client config, command samples and start app
            app.loadConf();
        end

        % Value changed function: ConnectButton
        function ConnectButtonValueChanged(app, event)
            if app.ConnectButton.Value
                app.ConnectButton.Enable = false;
                app.connect();
                app.ConnectButton.Text = 'Disconnect';
                app.ConnectButton.Enable = true;
            else
                app.ConnectButton.Enable = false;
                app.disconnect();
                app.ConnectButton.Text = 'Connect';
                app.ConnectButton.Enable = true;
            end
        end

        % Clicked callback: CommandDropDown
        function CommandDropDownClicked(app, event)
            app.CommandDropDown.FontColor = [0.0, 0.0, 0.0];
            app.CommandDropDown.Items{1} = '<none>';
        end

        % Value changed function: CommandDropDown
        function CommandDropDownValueChanged(app, event)
            command = app.CommandDropDown.Value;

            write(app.connection{1},command);
        end

        % Changes arrangement of the app based on UIFigure width
        function updateAppLayout(app, event)
            currentFigureWidth = app.UIFigure.Position(3);
            if(currentFigureWidth <= app.onePanelWidth)
                % Change to a 2x1 grid
                app.GridLayout.RowHeight = {717, 717};
                app.GridLayout.ColumnWidth = {'1x'};
                app.RightPanel.Layout.Row = 2;
                app.RightPanel.Layout.Column = 1;
            else
                % Change to a 1x2 grid
                app.GridLayout.RowHeight = {'1x'};
                app.GridLayout.ColumnWidth = {212, '1x'};
                app.RightPanel.Layout.Row = 1;
                app.RightPanel.Layout.Column = 2;
            end
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and hide until all components are created
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.AutoResizeChildren = 'off';
            app.UIFigure.Position = [100 100 733 717];
            app.UIFigure.Name = 'MATLAB App';
            app.UIFigure.SizeChangedFcn = createCallbackFcn(app, @updateAppLayout, true);

            % Create GridLayout
            app.GridLayout = uigridlayout(app.UIFigure);
            app.GridLayout.ColumnWidth = {212, '1x'};
            app.GridLayout.RowHeight = {'1x'};
            app.GridLayout.ColumnSpacing = 0;
            app.GridLayout.RowSpacing = 0;
            app.GridLayout.Padding = [0 0 0 0];
            app.GridLayout.Scrollable = 'on';

            % Create LeftPanel
            app.LeftPanel = uipanel(app.GridLayout);
            app.LeftPanel.Layout.Row = 1;
            app.LeftPanel.Layout.Column = 1;

            % Create GridLayout2
            app.GridLayout2 = uigridlayout(app.LeftPanel);
            app.GridLayout2.ColumnWidth = {'1x', '1x', '1x', '1x', '1x'};
            app.GridLayout2.RowHeight = {'1x', '1x', '1x', '1x', '1x', '1x', '20x'};

            % Create CMDLampLabel
            app.CMDLampLabel = uilabel(app.GridLayout2);
            app.CMDLampLabel.HorizontalAlignment = 'right';
            app.CMDLampLabel.Layout.Row = 2;
            app.CMDLampLabel.Layout.Column = 1;
            app.CMDLampLabel.Text = 'CMD';

            % Create CMDLamp
            app.CMDLamp = uilamp(app.GridLayout2);
            app.CMDLamp.Tooltip = {'Command connection activity. Channel used by the client to send commands'};
            app.CMDLamp.Layout.Row = 3;
            app.CMDLamp.Layout.Column = 1;
            app.CMDLamp.Color = [0.6353 0.0784 0.1843];

            % Create RTDLamp
            app.RTDLamp = uilamp(app.GridLayout2);
            app.RTDLamp.Tooltip = {'RealTime Callback connection activity. Used for binary streaming for traces, occupancy and DF sweeps'};
            app.RTDLamp.Layout.Row = 3;
            app.RTDLamp.Layout.Column = 3;
            app.RTDLamp.Color = [0.6353 0.0784 0.1843];

            % Create RTDLampLabel
            app.RTDLampLabel = uilabel(app.GridLayout2);
            app.RTDLampLabel.HorizontalAlignment = 'right';
            app.RTDLampLabel.Layout.Row = 2;
            app.RTDLampLabel.Layout.Column = 3;
            app.RTDLampLabel.Text = 'RTD';

            % Create DCBLampLabel
            app.DCBLampLabel = uilabel(app.GridLayout2);
            app.DCBLampLabel.HorizontalAlignment = 'right';
            app.DCBLampLabel.Layout.Row = 2;
            app.DCBLampLabel.Layout.Column = 2;
            app.DCBLampLabel.Text = 'DCB';

            % Create DCBLamp
            app.DCBLamp = uilamp(app.GridLayout2);
            app.DCBLamp.Tooltip = {'Data Callback connection activity. Used for measurements, bist and singular data'};
            app.DCBLamp.Layout.Row = 3;
            app.DCBLamp.Layout.Column = 2;
            app.DCBLamp.Color = [0.6353 0.0784 0.1843];

            % Create ERRLampLabel
            app.ERRLampLabel = uilabel(app.GridLayout2);
            app.ERRLampLabel.HorizontalAlignment = 'right';
            app.ERRLampLabel.Layout.Row = 2;
            app.ERRLampLabel.Layout.Column = 4;
            app.ERRLampLabel.Text = 'ERR';

            % Create ERRLamp
            app.ERRLamp = uilamp(app.GridLayout2);
            app.ERRLamp.Tooltip = {'Error Callback connection activity. Return erros from station and connection services to the client'};
            app.ERRLamp.Layout.Row = 3;
            app.ERRLamp.Layout.Column = 4;
            app.ERRLamp.Color = [0.6353 0.0784 0.1843];

            % Create SNDLampLabel
            app.SNDLampLabel = uilabel(app.GridLayout2);
            app.SNDLampLabel.HorizontalAlignment = 'right';
            app.SNDLampLabel.Layout.Row = 2;
            app.SNDLampLabel.Layout.Column = 5;
            app.SNDLampLabel.Text = 'SND';

            % Create SNDLamp
            app.SNDLamp = uilamp(app.GridLayout2);
            app.SNDLamp.Tooltip = {'Sound connection activity. Indicates that sound streaming is being performed.'};
            app.SNDLamp.Layout.Row = 3;
            app.SNDLamp.Layout.Column = 5;
            app.SNDLamp.Color = [0.6353 0.0784 0.1843];

            % Create CommandDropDownLabel
            app.CommandDropDownLabel = uilabel(app.GridLayout2);
            app.CommandDropDownLabel.VerticalAlignment = 'bottom';
            app.CommandDropDownLabel.Layout.Row = 4;
            app.CommandDropDownLabel.Layout.Column = [1 5];
            app.CommandDropDownLabel.Text = 'Command';

            % Create CommandDropDown
            app.CommandDropDown = uidropdown(app.GridLayout2);
            app.CommandDropDown.Items = {'<select a command>'};
            app.CommandDropDown.ValueChangedFcn = createCallbackFcn(app, @CommandDropDownValueChanged, true);
            app.CommandDropDown.FontColor = [0.8 0.8 0.8];
            app.CommandDropDown.Layout.Row = 5;
            app.CommandDropDown.Layout.Column = [1 5];
            app.CommandDropDown.ClickedFcn = createCallbackFcn(app, @CommandDropDownClicked, true);
            app.CommandDropDown.Value = '<select a command>';

            % Create DataReceivedTextAreaLabel
            app.DataReceivedTextAreaLabel = uilabel(app.GridLayout2);
            app.DataReceivedTextAreaLabel.VerticalAlignment = 'bottom';
            app.DataReceivedTextAreaLabel.Layout.Row = 6;
            app.DataReceivedTextAreaLabel.Layout.Column = [1 5];
            app.DataReceivedTextAreaLabel.Text = 'Data Received';

            % Create Tree
            app.Tree = uitree(app.GridLayout2);
            app.Tree.Layout.Row = 7;
            app.Tree.Layout.Column = [1 5];

            % Create Node
            app.Node = uitreenode(app.Tree);
            app.Node.Text = 'Node';

            % Create Node2
            app.Node2 = uitreenode(app.Node);
            app.Node2.Text = 'Node2';

            % Create Node3
            app.Node3 = uitreenode(app.Node);
            app.Node3.Text = 'Node3';

            % Create Node4
            app.Node4 = uitreenode(app.Node);
            app.Node4.Text = 'Node4';

            % Create ConnectButton
            app.ConnectButton = uibutton(app.GridLayout2, 'state');
            app.ConnectButton.ValueChangedFcn = createCallbackFcn(app, @ConnectButtonValueChanged, true);
            app.ConnectButton.Text = 'Connect';
            app.ConnectButton.Layout.Row = 1;
            app.ConnectButton.Layout.Column = [1 5];

            % Create RightPanel
            app.RightPanel = uipanel(app.GridLayout);
            app.RightPanel.Layout.Row = 1;
            app.RightPanel.Layout.Column = 2;

            % Create GridLayout3
            app.GridLayout3 = uigridlayout(app.RightPanel);
            app.GridLayout3.ColumnWidth = {'1x'};
            app.GridLayout3.RowHeight = {'1x', '1x', '1x'};

            % Create SPTAxes
            app.SPTAxes = uiaxes(app.GridLayout3);
            xlabel(app.SPTAxes, 'Frequency')
            ylabel(app.SPTAxes, 'Level')
            zlabel(app.SPTAxes, 'Z')
            app.SPTAxes.Layout.Row = 1;
            app.SPTAxes.Layout.Column = 1;

            % Create AOAAxes
            app.AOAAxes = uiaxes(app.GridLayout3);
            xlabel(app.AOAAxes, 'Frequency')
            ylabel(app.AOAAxes, 'Angle')
            zlabel(app.AOAAxes, 'Z')
            app.AOAAxes.Layout.Row = 2;
            app.AOAAxes.Layout.Column = 1;

            % Create OCCAxes
            app.OCCAxes = uiaxes(app.GridLayout3);
            xlabel(app.OCCAxes, 'Frequency')
            ylabel(app.OCCAxes, 'Occupancy')
            app.OCCAxes.Layout.Row = 3;
            app.OCCAxes.Layout.Column = 1;

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = appTestMiaer_exported

            runningApp = getRunningApp(app);

            % Check for running singleton app
            if isempty(runningApp)

                % Create UIFigure and components
                createComponents(app)

                % Register the app with App Designer
                registerApp(app, app.UIFigure)

                % Execute the startup function
                runStartupFcn(app, @startupFcn)
            else

                % Focus the running singleton app
                figure(runningApp.UIFigure)

                app = runningApp;
            end

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
    end
end
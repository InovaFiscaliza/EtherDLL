classdef EtherDLLTestClient_exported < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        EtherDLLTest           matlab.ui.Figure
        mainGrid               matlab.ui.container.GridLayout
        parsedData             matlab.ui.container.Tree
        cleanParsedAreaIcon    matlab.ui.control.Image
        receivedMsg            matlab.ui.control.TextArea
        connectButton          matlab.ui.control.StateButton
        bandSelectDropDown     matlab.ui.control.DropDown
        cleanReceivedAreaIcon  matlab.ui.control.Image
        repeatCmdIcon          matlab.ui.control.Image
        CommandDropDown        matlab.ui.control.DropDown
        sentMsg                matlab.ui.control.TextArea
        parsedMsg              matlab.ui.control.TextArea
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
                warning('Failed to disconnect from EtherDLL.');
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
        function processStationData(app, data)
            % Process different kinds of data from the received data into
            % adjusted structures. Dynamically identifies and processes keys
            % such as "spectrum", "occupancy", "measurement", "equipment", "settings".
            % Response may include none, one, many or all keys.
            
            % Define supported data keys and their corresponding handler methods
            supportedKeys = {'spectrum', 'aoa', 'occupancy', 'measure', 'equipment', 'setting', 'site'};
            
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
                                    data.spectrum = app.buildAxis(data.spectrum, 'traceDataDF', 'DF Antenna Field Strength', false);
                                end
                                data.spectrum = app.buildAxis(data.spectrum, 'traceData', 'Field Strength', false);

                                app.plotData(app.SPTAxes, data.spectrum);
                                app.presentData('spectrum',data.spectrum,["numBands","numTotalBins","numBins","firstBin","numTimeOfDays"])
                            case 'occupancy'
                                data.occupancy = app.buildAxis(data.occupancy, 'chanData', "Channel Data", false);
                                data.occupancy = app.buildAxis(data.occupancy, 'aveRange', "Average Range", false);
                                data.occupancy = app.buildAxis(data.occupancy, 'aveFldStr', "Average Field Strength", false);

                                app.plotData(app.OCCAxes, data.occupancy);
                            case 'aoa'
                                data.aoa = app.buildAxis(data.aoa, 'traceData', 'Azimuth', false);
                                data.aoa = app.buildAxis(data.aoa, 'confidence','Confidence', true);
                                app.plotData(app.AOAAxes, data.aoa);
                            case 'measure'
                                app.presentData('measure',data.measure, ["noiseFloor", "powerDbm"]);
                            case 'equipment'
                                app.presentData('equipment',data.equipment,["hostName","selectedAntenna"],false);
                            case 'setting'
                                app.presentData('setting',data.setting,["primaryThresholdAbsolute","primaryThresholdAboveNoise","secondaryThresholdAbsolute","secondaryThresholdAboveNoise","saveIntermediateData","useSecondaryThreshold","numAzimuths","attenuation"],false);
                            case 'site'
                                app.presentData('site',data.site,["dateTime","latitude","longitude","numSats"],false);
                            case 'task'
                                app.presentData('task',data.task,["dateTime","taskId","key","state","status","completionTime"],true)
                        end
                    catch ME
                        warning(ME.identifier, 'Error processing field "%s": %s', fieldName, ME.message);
                    end
                end
            end
        end

        %-----------------------------------------------------------------%
        function data = buildAxis(app, data, arrayName, measureAxisName, isAlpha)
            % build x and y axis for different data types
            % data: structure containing data to be plotted
            % arrayName: Name of the data filed, e.g. 'traceData', 'aoa', 'occupancy'.
            % measureAxisName: Name to be used as lable for the measurement axis
            % isAlpha: if true, store data in alphaAxis instead of measureAxis
            % Returns: modified data structure with measureAxis (or custom name) and frequencyAxis
            
            % Convert base64 data to float32 array
            newTrace = app.base64ToFloat32(data.(arrayName));
            
            if isAlpha
                data.alphaAxis = newTrace;
                return;
            end

            % Initialize or append to measureAxis array
            if ~isfield(data, 'measureAxis')
                % First trace - initialize as cell array
                data.measureAxis = {newTrace};
                data.measureAxisNames = {measureAxisName};
            else
                % Append new trace
                data.measureAxis{end+1} = newTrace;
                data.measureAxisNames{end+1} = measureAxisName;
            end
            
            % Create frequency axis if it doesn't exist
            if ~isfield(data, 'frequencyAxis')
                data.frequencyAxis = linspace(data.startFrequency, data.stopFrequency, data.numBins);
            end
            
        end

        %-----------------------------------------------------------------%
        function plotData(app, axesHandle, data)
            % plot spectrum data on specified axes
            % axesHandle: UIAxes object (app.SPTAxes, app.AOAAxes, app.OCCAxes, etc.)
            % data: structure containing spectrum information with measureAxis (cell array)
            
            if app.refresh
                hold(axesHandle, "off");
                app.refresh = false;
            else
                hold(axesHandle, "on");
            end
            
            xlim(axesHandle, [data.startFrequency data.stopFrequency]);

            % Set 10 ticks on horizontal axis (frequency)
            xTicks = linspace(data.startFrequency, data.stopFrequency, 10);
            axesHandle.XTick = xTicks;
            axesHandle.XTickLabel = app.formatEngineeringNotation(xTicks);
            
            % Determine Y-axis limits from all traces
            numTraces = length(data.measureAxis);
            allValues = [];
            for i = 1:numTraces
                allValues = [allValues; data.measureAxis{i}(:)];
            end
            yMin = min(allValues);
            yMax = max(allValues);
            
            % Set 10 ticks on vertical axis (measurement values)
            yTicks = linspace(yMin, yMax, 10);
            axesHandle.YTick = yTicks;
            axesHandle.YTickLabel = app.formatEngineeringNotation(yTicks);

            % Plot each trace
            legendLabels = cell(numTraces, 1);
            for i = 1:numTraces
                currentTrace = data.measureAxis{i};
                
                % Check if this is confidence data (for transparency-based plotting)
                if isfield(data, 'alphaAxis')
                    % Convert measureValue (0-100) to alpha values (0-1)
                    alphaValues = data.alphaAxis / 100;
                    scatter(axesHandle, data.frequencyAxis, currentTrace, 36, ...
                        [0.2 0.5 0.9], 'filled', 'MarkerFaceAlpha', 'flat', ...
                        'AlphaData', alphaValues);
                else
                    % Standard line plot
                    plot(axesHandle, data.frequencyAxis, currentTrace, '-', 'LineWidth', 2.5);
                end
            end
            
            % Add legend if multiple traces
            if length(data.measureAxis) > 1
                legend(axesHandle, legendLabels, 'Location', 'northeast');
            end
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
            if isprop(app, parentNodeName)
                parentNode = app.(parentNodeName);
            else
                % Create parent node dynamically
                parentNode = uitreenode(app.parsedData, 'Text', parentNodeName);
                app.(parentNodeName) = parentNode;
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
                        valueStr = char(data.(fieldName));
                    end
                    
                    message = sprintf('%s: %s', fieldName, valueStr);
                    
                    % Check if child node already exists
                    childNodeName = [parentNodeName, fieldName];  % Create unique property name
                    
                    if isprop(app, childNodeName)
                        % Update existing child node
                        childNode = app.(childNodeName);
                        childNode.Text = message;
                    else
                        % Create new child node
                        childNode = uitreenode(parentNode, 'Text', message);
                        app.addprop(childNodeName);  % Add dynamic property to app
                        app.(childNodeName) = childNode;
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
        function commandDropDownValueChanged(app, event)
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
        function commandDropDownClicked(app, event)
            %  change color and first item value to indicate that options
            %  are active.
            app.CommandDropDown.FontColor = [0.0, 0.0, 0.0];
            app.CommandDropDown.Items{1} = '<none>';
        end

        % Image clicked function: cleanParsedAreaIcon
        function cleanParsedAreaIClicked(app, event)
            app.parsedMsg.Value = "";
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
            app.mainGrid.ColumnWidth = {'fit', 'fit', '3x', '10x'};
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

            % Create parsedMsg
            app.parsedMsg = uitextarea(app.mainGrid);
            app.parsedMsg.Tooltip = {'Messages received from EtherDLL'};
            app.parsedMsg.Placeholder = '< will display raw messages parsed by the app>';
            app.parsedMsg.Layout.Row = [9 10];
            app.parsedMsg.Layout.Column = [1 3];

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
            app.bandSelectDropDown.Layout.Column = [1 3];
            app.bandSelectDropDown.Value = 'Band 0';

            % Create connectButton
            app.connectButton = uibutton(app.mainGrid, 'state');
            app.connectButton.ValueChangedFcn = createCallbackFcn(app, @connectConnectconnectButtonPushed, true);
            app.connectButton.Tooltip = {'Press to connect to EtherDLL'};
            app.connectButton.Text = 'Connect';
            app.connectButton.Layout.Row = 1;
            app.connectButton.Layout.Column = [1 3];

            % Create receivedMsg
            app.receivedMsg = uitextarea(app.mainGrid);
            app.receivedMsg.Tooltip = {'Messages received from EtherDLL'};
            app.receivedMsg.Placeholder = '< will display raw messages received from EtherDLL >';
            app.receivedMsg.Layout.Row = [7 8];
            app.receivedMsg.Layout.Column = [1 3];

            % Create cleanParsedAreaIcon
            app.cleanParsedAreaIcon = uiimage(app.mainGrid);
            app.cleanParsedAreaIcon.ImageClickedFcn = createCallbackFcn(app, @cleanParsedAreaIClicked, true);
            app.cleanParsedAreaIcon.Tooltip = {'Clean the output area'};
            app.cleanParsedAreaIcon.Layout.Row = 10;
            app.cleanParsedAreaIcon.Layout.Column = 1;
            app.cleanParsedAreaIcon.HorizontalAlignment = 'left';
            app.cleanParsedAreaIcon.VerticalAlignment = 'bottom';
            app.cleanParsedAreaIcon.ImageSource = fullfile(pathToMLAPP, 'sweep.svg');

            % Create parsedData
            app.parsedData = uitree(app.mainGrid);
            app.parsedData.Layout.Row = [9 10];
            app.parsedData.Layout.Column = [1 3];

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
function [cmd,rtdcb,dcb,ercb,snd] = connect(app)
%CONNECT stablish connection with the server
%   connection parameters from the conf structure are used to initialize the connection to the seveveral channels used
%         app.RED_LAMP = [0.64,0.08,0.18]
%         app.GREEN_LAMP = [0.47,0.67,0.19]
%         app.BLUE_LAMP = [0.30,0.75,0.93]
%         app.YELLOW_LAMP = [0.93,0.69,0.13]
%         app.
    try
        app.ConnectButton.Text
        cmd = tcpclient(conf.proxy.address, conf.service.command.port, 'Timeout', conf.service.command.timeout_s, 'ConnectTimeout', conf.service.command.timeout_s, 'Tag', 'cmd');
    catch
        warning('Failed to connect to the command channel');
        cmd = [];
    end

    try
        rtdcb = tcpclient(conf.proxy.address, conf.service.realtime.port, 'Timeout', conf.service.realtime.timeout_s, 'ConnectTimeout', conf.service.realtime.timeout_s, 'Tag', 'rtdcb');
    catch
        warning('Failed to connect to the realtime channel');
        rtdcb = [];
    end

    try
        dcb = tcpclient(conf.proxy.address, conf.service.data.port, 'Timeout', conf.service.data.timeout_s, 'ConnectTimeout', conf.service.data.timeout_s, 'Tag', 'dcb');
    catch
        warning('Failed to connect to the data channel');
        dcb = [];
    end

    try
        ercb = tcpclient(conf.proxy.address, conf.service.error.port, 'Timeout', conf.service.error.timeout_s, 'ConnectTimeout', conf.service.error.timeout_s, 'Tag', 'ercb');
    catch
        warning('Failed to connect to the error channel');
        ercb = [];
    end

    try
        snd = tcpclient(conf.proxy.address, conf.service.sound.port, 'Timeout', conf.service.sound.timeout_s, 'ConnectTimeout', conf.service.sound.timeout_s, 'Tag', 'snd');
    catch
        warning('Failed to connect to the sound channel');
        snd = [];
    end

end
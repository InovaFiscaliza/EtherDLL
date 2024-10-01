function [connection, lamp, lamp_status] = connect(conf, service_tag,red_lamp,green_lamp)
%CONNECT stablish connection with the server
%   connection parameters from the conf structure are used to initialize the connection to the seveveral channels used

switch service_tag
    case "cmd"
        try
            connection = tcpclient(conf.proxy.address, conf.service.command.port, 'Timeout', conf.service.command.timeout_s, 'ConnectTimeout', conf.service.command.timeout_s, 'Tag', service_tag);
            lamp = green_lamp;
            lamp_status = 1;
        catch
            warning('Failed to connect to the command channel');
            connection = [];
            lamp = red_lamp;
            lamp_status = 0;
        end
    case "rtd"
        try
            connection = tcpclient(conf.proxy.address, conf.service.realtime.port, 'Timeout', conf.service.realtime.timeout_s, 'ConnectTimeout', conf.service.realtime.timeout_s, 'Tag', service_tag);
            lamp = green_lamp;
            lamp_status = 1;
        catch
            warning('Failed to connect to the realtime channel');
            connection = [];
            lamp = red_lamp;
            lamp_status = 0;
        end
    case "dcb"
        try
            connection = tcpclient(conf.proxy.address, conf.service.data.port, 'Timeout', conf.service.data.timeout_s, 'ConnectTimeout', conf.service.data.timeout_s, 'Tag', service_tag);
            lamp = green_lamp;
            lamp_status = 1;
        catch
            warning('Failed to connect to the data channel');
            connection = [];
            lamp = red_lamp;
            lamp_status = 0;
        end
    case "err"
        try
            connection = tcpclient(conf.proxy.address, conf.service.error.port, 'Timeout', conf.service.error.timeout_s, 'ConnectTimeout', conf.service.error.timeout_s, 'Tag', service_tag);
            lamp = green_lamp;
            lamp_status = 1;
        catch
            warning('Failed to connect to the error channel');
            connection = [];
            lamp = red_lamp;
            lamp_status = 0;
        end
    case "snd"
        try
            connection = tcpclient(conf.proxy.address, conf.service.sound.port, 'Timeout', conf.service.sound.timeout_s, 'ConnectTimeout', conf.service.sound.timeout_s, 'Tag', service_tag);
            lamp = green_lamp;
            lamp_status = 1;
        catch
            warning('Failed to connect to the sound channel');
            connection = [];
            lamp = red_lamp;
            lamp_status = 0;
        end
    otherwise
        warning('Unknown service tag');
        connection = [];
        lamp = red_lamp;
        lamp_status = 0;
end
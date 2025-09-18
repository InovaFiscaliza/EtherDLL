#pragma once

#include <string>

namespace edll {

	using INT_CODE = int;
    
    struct Code {
        static constexpr int RUNNING = 0;
        static constexpr int KILL_INTERRUPT = 1;
        static constexpr int CTRL_C_INTERRUPT = 2;
        static constexpr int STATION_ERROR = 100;
        static constexpr int SERVICE_ERROR = 200;
        static constexpr int CLIENT_ERROR = 300;

        static constexpr const char* toString(int code) {
            switch (code) {
                case RUNNING: return "Service running";
                case KILL_INTERRUPT: return "Service interrupted by kill command";
                case CTRL_C_INTERRUPT: return "Service interrupted by Ctrl+C";
                case STATION_ERROR: return "Station communication error";
                case SERVICE_ERROR: return "EtherDLL service error";
                case CLIENT_ERROR: return "Client communication error";
                default: return "Undefined EtherDLL Code";
            }
        }
    };

    struct Form {
        static constexpr const char PING[] = "{\"PING\":1}\r\n";
        static constexpr const char ACK[] = "{\"ACK\":";
		static constexpr const char MSG_ID[] = "ID";
		static constexpr const char NACK[] = "{\"NACK\":\"Message is not a valid JSON\"";
        static constexpr const char MSG_END[] = "\r\n";
    };

	constexpr double MHZ_MULTIPLIER = 1000000.0;
	constexpr double KHZ_MAX_VALUE = 1.0;
	constexpr double KHZ_FROM_MHZ = 1000.0;
	constexpr double GHZ_MIN_VALUE = 1000.0;
	constexpr double GHZ_FROM_MHZ = 0.001;
	constexpr double PAN_OFFSET = 192.0;
	constexpr double FREQ_FACTOR = 1920.0;

    constexpr const char* DEFAULT_CONFIG_FILENAME = "EtherDLLConfig.json";
	constexpr const char* VERSION = "1.0.0";

	constexpr int SOCKET_BUFFER_SIZE = 4096;
}
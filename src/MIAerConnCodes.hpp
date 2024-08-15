#pragma once

#include <string>

namespace mcs {

    struct Code {
        static constexpr int RUNNING = 0;
        static constexpr int KILL_INTERRUPT = 1;
        static constexpr int CTRL_C_INTERRUPT = 2;
        static constexpr int STATION_ERROR = 100;
        static constexpr int SERVICE_ERROR = 200;
        static constexpr int COMMAND_ERROR = 300;
        static constexpr int STREAM_ERROR = 400;

        static constexpr const char* toString(int code) {
            switch (code) {
            case RUNNING: return "Service running";
            case KILL_INTERRUPT: return "Service interrupted by kill command";
            case CTRL_C_INTERRUPT: return "Service interrupted by Ctrl+C";
            case STATION_ERROR: return "Station communication error";
            case SERVICE_ERROR: return "Service error";
            case COMMAND_ERROR: return "Command channel communication error";
            case STREAM_ERROR: return "Stream channel error";
            default: return "Undefined MIAerConn Code";
            }
        }
    };

    struct Form {
        static constexpr const char SEP[] = "\r\n";
        static constexpr const char PING[] = "PING\r\n";
        static constexpr const char ACK[] = "ACK:";
        static constexpr const char BLOCK_END[] = { 39, 10, 13, 39, '\0' }; // Sequence of 4 bytes with null termination for string: '\n\r'
    };
}

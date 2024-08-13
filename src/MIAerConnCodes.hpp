// Codes used in MIAerConn Service

#pragma once

#include <string>

namespace MCService {
    enum class Code {
        RUNNING = 0,
        KILL_INTERRUPT = 1,
        CTRL_C_INTERRUPT = 2,
        STATION_ERROR = 100,
        SERVICE_ERROR = 200,
        COMMAND_ERROR = 300,
        STREAM_ERROR = 400
    };

    inline std::string toString(Code code) {
        switch (code) {
        case Code::RUNNING: return "Service running";
        case Code::KILL_INTERRUPT: return "Service interrupted by kill command";
        case Code::CTRL_C_INTERRUPT: return "Service interrupted by Ctrl+C";
        case Code::STATION_ERROR: return "Station communication error";
        case Code::COMMAND_ERROR: return "Command channel communication error";
        case Code::STREAM_ERROR: return "Stream channel error";
        default: return "Undefined MIAerConn Code";
        }
    }
}
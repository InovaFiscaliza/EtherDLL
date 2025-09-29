/**
* @file EtherDLLConstants.hpp
*
* @brief Heder file constants and types used across the core EtherDLL modules
*
* * @author fslobao
* * @date 2025-09-29
* * @version 1.0
*
* * @note Requires C++11 or later
* * @note Uses nlohmann/json library for JSON handling
*
**/
// ----------------------------------------------------------------------
#pragma once

// Include to DLL specific headers

// Include core EtherDLL libraries

// Include project libraries

// Include general C++ libraries
#include <string>

// For convenience

// ----------------------------------------------------------------------
namespace edll {

    // Current version of the EtherDLL
    constexpr const char* VERSION = "1.0.0";

	// Alias for interruption code type
	using INT_CODE = int;
    
	// Interruption codes used for tracking the reason for service interruption
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

	// Frequency conversion constants
	constexpr double MHZ_MULTIPLIER = 1000000.0;
	constexpr double KHZ_MAX_VALUE = 1.0;
	constexpr double KHZ_FROM_MHZ = 1000.0;
	constexpr double GHZ_MIN_VALUE = 1000.0;
	constexpr double GHZ_FROM_MHZ = 0.001;

	// Default configuration values allow application to run if no configuration file is provided or can not be created
    constexpr const char* DEFAULT_CONFIG_FILENAME = "EtherDLLConfig.json";
    constexpr int DEFAULT_SERVICE_PORT = 5555;
	constexpr int DEFAULT_TIMEOUT_S = 10;
	constexpr int DEFAULT_SLEEP_MS = 100;
	constexpr int DEFAULT_BUFFER_TTL_PERIOD = 5;

	constexpr int DEFAULT_PING_PERIOD = 10;
    constexpr bool DEFAULT_PING_STATE = true;
    constexpr const char* DEFAULT_PING_MSG = "PING";
    constexpr const char* DEFAULT_ACK_MSG = "ACK";
	constexpr const char* DEFAULT_NACK_MSG = "NACK";
	constexpr const char* DEFAULT_END_MSG = "\r\n";

	// Message keys used in JSON communication
    constexpr const char* MSG_KEY_CLIENT_ID = "ID";
	constexpr const char* MSG_KEY_QUEUE_ID = "queue_id";
    constexpr const char* MSG_KEY_CLIENT_IP = "client_ip";

	// Socket buffer size for receiving data
	constexpr int SOCKET_BUFFER_SIZE = 4096;
}
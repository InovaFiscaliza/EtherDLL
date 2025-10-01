/**
* @file EtherDLLConstants.hpp
*
* @brief Heder file constants and types used across the core EtherDLL modules
*
* * @author fslobao
* * @date 2025-09-29
* * @version 1.0
*
* * @note Requires C++17 or later
* * @note Uses nlohmann/json library for JSON handling
*
**/
// ----------------------------------------------------------------------
#pragma once

// Include to DLL specific headers

// Include core EtherDLL libraries

// Include project libraries
#include <nlohmann/json.hpp>

// Include general C++ libraries
#include <string>

// For convenience
using json = nlohmann::json;

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

	// JSON elements
	constexpr const char* JSON_START = "{\"";
	constexpr const char* JSON_MID = "\":";
	constexpr const char* JSON_END = "}";

	// Default configuration values allow application to run if no configuration file is provided or can not be created
	struct DefaultConfig {

		static constexpr const char* DEFAULT_CONFIG_FILENAME = "EtherDLLConfig.json";

		struct Log {
			static constexpr const char* KEY = "log";

			struct Name {
				static constexpr const char* KEY = "name";
				static constexpr const char* VALUE = "EtherDLL";
			};

			struct Console {
				static constexpr const char* KEY = "console";

				struct Level {
					static constexpr const char* KEY = "level";
					static constexpr const char* VALUE = "info";
				};
			};

			struct File {
				static constexpr const char* KEY = "file";

				struct Level {
					static constexpr const char* KEY = "level";
					static constexpr const char* VALUE = "debug";
				};
				struct Filename {
					static constexpr const char* KEY = "filename";
					static constexpr const char* VALUE = "EtherDLL.log";
				};
			};
		};

		struct Service {
			static constexpr const char* KEY = "service";

			struct Port {
				static constexpr const char* KEY = "port";
				static constexpr int VALUE = 5555;
			};
			struct BufferSize {
				static constexpr const char* KEY = "buffer";
				static constexpr int VALUE = 4096;
				static constexpr int MAX_VALUE = 1000000;
			};
			struct Timeout {
				static constexpr const char* KEY = "timeout_s";
				static constexpr int VALUE = 10;
			};
			struct Sleep {
				static constexpr const char* KEY = "sleep_ms";
				static constexpr int VALUE = 100;
			};
			struct BufferTTL {
				static constexpr const char* KEY = "buffer_ttl_period";
				static constexpr int VALUE = 5;
			};
			struct PingPeriod {
				static constexpr const char* KEY = "ping_period";
				static constexpr int VALUE = 10;
			};
			struct PingEnable {
				static constexpr const char* KEY = "ping_enable";
				static constexpr bool VALUE = true;
			};
			struct Msg {
				static constexpr const char* KEY = "msg_keys";

				struct End {
					static constexpr const char* KEY = "end";
					static constexpr const char* VALUE = "\r\n";
				};
				struct Ping {
					static constexpr const char* KEY = "ping";
					static constexpr const char* VALUE = "PING";
				};
				struct Ack {
					static constexpr const char* KEY = "ack";
					static constexpr const char* VALUE = "ACK";
				};
				struct Nack {
					static constexpr const char* KEY = "nack";
					static constexpr const char* VALUE = "NACK";
				};
			};
			struct Queue {
				struct ClientId {
					static constexpr const char* KEY = "id";
					static constexpr int VALUE = 0;
				};
				struct QueueId {
					static constexpr const char* KEY = "queue_id";
					static constexpr int VALUE = 0;
				};
				struct DLLId {
					static constexpr const char* KEY = "server_id";
					static constexpr int VALUE = 0;
				};
				struct ClientIp {
					static constexpr const char* KEY = "client_ip";
					static constexpr const char* VALUE = "172.24.3.15";
				};
				struct CommandCode {
					static constexpr const char* KEY = "commandCode";
					static constexpr int VALUE = -1;
				};
				struct CommandName {
					static constexpr const char* KEY = "commandName";
					static constexpr const char* VALUE = "Error";
				};
				struct Arguments {
					static constexpr const char* KEY = "arguments";
					static inline const json VALUE = json::object();
				};
			};
		};
	};
}

// ------------------------------------------------------
/** @brief Build a default configuration JSON object from the DefaultConfig struct
 *
 * @param  nlohmann::json: JSON object to be populated with default configuration, default is empty object
 * @return nlohmann::json: JSON object containing the default configuration
 * @throws NO EXCEPTION HANDLING
 **/
json buildCoreDefaultConfigJson(json default_config = json::object()) {

	default_config[edll::DefaultConfig::Log::KEY][edll::DefaultConfig::Log::Name::KEY] = edll::DefaultConfig::Log::Name::VALUE;
	default_config[edll::DefaultConfig::Log::KEY][edll::DefaultConfig::Log::Console::KEY][edll::DefaultConfig::Log::Console::Level::KEY] = edll::DefaultConfig::Log::Console::Level::VALUE;
	default_config[edll::DefaultConfig::Log::KEY][edll::DefaultConfig::Log::File::KEY][edll::DefaultConfig::Log::File::Level::KEY] = edll::DefaultConfig::Log::File::Level::VALUE;
	default_config[edll::DefaultConfig::Log::KEY][edll::DefaultConfig::Log::File::KEY][edll::DefaultConfig::Log::File::Filename::KEY] = edll::DefaultConfig::Log::File::Filename::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Port::KEY] = edll::DefaultConfig::Service::Port::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::BufferSize::KEY] = edll::DefaultConfig::Service::BufferSize::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Timeout::KEY] = edll::DefaultConfig::Service::Timeout::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Sleep::KEY] = edll::DefaultConfig::Service::Sleep::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::BufferTTL::KEY] = edll::DefaultConfig::Service::BufferTTL::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::PingPeriod::KEY] = edll::DefaultConfig::Service::PingPeriod::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::PingEnable::KEY] = edll::DefaultConfig::Service::PingEnable::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Msg::KEY][edll::DefaultConfig::Service::Msg::End::KEY] = edll::DefaultConfig::Service::Msg::End::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Msg::KEY][edll::DefaultConfig::Service::Msg::Ping::KEY] = edll::DefaultConfig::Service::Msg::Ping::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Msg::KEY][edll::DefaultConfig::Service::Msg::Ack::KEY] = edll::DefaultConfig::Service::Msg::Ack::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Msg::KEY][edll::DefaultConfig::Service::Msg::Nack::KEY] = edll::DefaultConfig::Service::Msg::Nack::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Queue::ClientId::KEY] = edll::DefaultConfig::Service::Queue::ClientId::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Queue::QueueId::KEY] = edll::DefaultConfig::Service::Queue::QueueId::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Queue::DLLId::KEY] = edll::DefaultConfig::Service::Queue::DLLId::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Queue::ClientIp::KEY] = edll::DefaultConfig::Service::Queue::ClientIp::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Queue::CommandCode::KEY] = edll::DefaultConfig::Service::Queue::CommandCode::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Queue::CommandName::KEY] = edll::DefaultConfig::Service::Queue::CommandName::VALUE;
	default_config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::Queue::Arguments::KEY] = edll::DefaultConfig::Service::Queue::Arguments::VALUE;

	return default_config;
}

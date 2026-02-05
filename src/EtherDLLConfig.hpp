/**
* @file EtherDLLConfig.hpp
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
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <string>
#include <iostream>

// For convenience
using json = nlohmann::json;

// Global variables
extern spdlog::logger* loggerPtr;

// ----------------------------------------------------------------------
namespace edll {

    // Current version of the EtherDLL
    constexpr const char* VERSION = "1.0.0";

	// Alias for interruption code type
	using INT_CODE = int;
    
	// Interruption codes used for tracking the reason for service interruption
    struct CODE {
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

			struct Socket {
				static constexpr const char* KEY = "socket";

				struct Port {
					static constexpr const char* KEY = "port";
					static constexpr int VALUE = 5555;
				};
				struct BufferSize {
					static constexpr const char* KEY = "bufferSizeBytes";
					static constexpr int VALUE = 4096;
					static constexpr int MAX_VALUE = 1000000;
				};
				struct Timeout {
					static constexpr const char* KEY = "timeoutS";
					static constexpr int VALUE = 10;
				};
				struct Sleep {
					static constexpr const char* KEY = "sleepMs";
					static constexpr int VALUE = 100;
				};
				struct BufferTTL {
					static constexpr const char* KEY = "bufferTTLMsgCount";
					static constexpr int VALUE = 5;
				};
			};
			struct Protocol {
				static constexpr const char* KEY = "protocol";

				struct PingEnable {
					static constexpr const char* KEY = "pingEnable";
					static constexpr bool VALUE = true;
				};
				struct PingPeriod {
					static constexpr const char* KEY = "pingPeriodS";
					static constexpr int VALUE = 30;
				};


				struct PeerMode {
					static constexpr const char* KEY = "peerMode";
					static constexpr bool VALUE = true;
				};
				struct ResponseBufferSize {
					static constexpr const char* KEY = "responseBuffer";
					static constexpr int VALUE = 4096;
					static constexpr int MAX_VALUE = 1000000000;
				};
			};
			struct DemoMode {
				static constexpr const char* KEY = "demoMode";
				static constexpr bool VALUE = false;
			};

			struct Msg {
				static constexpr const char* KEY = "msgKeys";

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
			struct TaskKeys {
				static constexpr const char* KEY = "taskKeys";

				struct ClientId {
					static constexpr const char* KEY = "clientId";
					static constexpr const char* VALUE = "ID";
					static constexpr int INIT_VALUE = 0;
				};
				struct QueueId {
					static constexpr const char* KEY = "queueId";
					static constexpr const char* VALUE = "QID";
					static constexpr int INIT_VALUE = 0;
				};
				struct DLLId {
					static constexpr const char* KEY = "serverId";
					static constexpr const char* VALUE = "SID";
					static constexpr int INIT_VALUE = 0;
				};
				struct ClientIp {
					static constexpr const char* KEY = "clientIP";
					static constexpr const char* VALUE = "REQUEST_SOURCE";
					static constexpr const char* INIT_VALUE = "Not Retrieved";
				};
				struct CommandCode {
					static constexpr const char* KEY = "commandCode";
					static constexpr const char* VALUE = "CODE";
					static constexpr int INIT_VALUE = -1;
				};
				struct CommandName {
					static constexpr const char* KEY = "commandName";
					static constexpr const char* VALUE = "COMMAND";
					static constexpr const char* INIT_VALUE = "Error";
				};
				struct Arguments {
					static constexpr const char* KEY = "arguments";
					static constexpr const char* VALUE = "ARGS";
					static inline const json INIT_VALUE = json::object();
				};
				struct Message {
					static constexpr const char* KEY = "message";
					static constexpr const char* VALUE = "MSG";
					static constexpr const char* INIT_VALUE = "No message";
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

	using log = edll::DefaultConfig::Log;
	using logConsole = log::Console;
	using logFile = log::File;
	using service = edll::DefaultConfig::Service;
	using socket = service::Socket;
	using protocol = service::Protocol;
	using msg = service::Msg;
	using taskKeys = service::TaskKeys;

	// Log section
	json& logSection = default_config[log::KEY];
	logSection[log::Name::KEY] = log::Name::VALUE;
	logSection[logConsole::KEY][logConsole::Level::KEY] = logConsole::Level::VALUE;
	logSection[logFile::KEY][logFile::Level::KEY] = logFile::Level::VALUE;
	logSection[logFile::KEY][logFile::Filename::KEY] = logFile::Filename::VALUE;

	// Service section
	json& svcSection = default_config[service::KEY];
	svcSection[socket::Port::KEY] = socket::Port::VALUE;
	svcSection[socket::BufferSize::KEY] = socket::BufferSize::VALUE;
	svcSection[socket::Timeout::KEY] = socket::Timeout::VALUE;
	svcSection[socket::Sleep::KEY] = socket::Sleep::VALUE;
	svcSection[socket::BufferTTL::KEY] = socket::BufferTTL::VALUE;
	svcSection[protocol::PingPeriod::KEY] = protocol::PingPeriod::VALUE;
	svcSection[protocol::PingEnable::KEY] = protocol::PingEnable::VALUE;
	svcSection[service::DemoMode::KEY] = service::DemoMode::VALUE;

	// Message keys subsection
	json& msgSection = svcSection[msg::KEY];
	msgSection[msg::End::KEY] = msg::End::VALUE;
	msgSection[msg::Ping::KEY] = msg::Ping::VALUE;
	msgSection[msg::Ack::KEY] = msg::Ack::VALUE;
	msgSection[msg::Nack::KEY] = msg::Nack::VALUE;

	// Task keys subsection
	json& taskSection = svcSection[taskKeys::KEY];
	taskSection[taskKeys::ClientId::KEY] = taskKeys::ClientId::VALUE;
	taskSection[taskKeys::QueueId::KEY] = taskKeys::QueueId::VALUE;
	taskSection[taskKeys::DLLId::KEY] = taskKeys::DLLId::VALUE;
	taskSection[taskKeys::ClientIp::KEY] = taskKeys::ClientIp::VALUE;
	taskSection[taskKeys::CommandCode::KEY] = taskKeys::CommandCode::VALUE;
	taskSection[taskKeys::CommandName::KEY] = taskKeys::CommandName::VALUE;
	taskSection[taskKeys::Arguments::KEY] = taskKeys::Arguments::VALUE;
	taskSection[taskKeys::Message::KEY] = taskKeys::Message::VALUE;

	return default_config;
}

// ----------------------------------------------------------------------
/** @brief Test all log configuration parameters are correctly defined
 * @param None
 * @return bool, True if configuration all parameters are valid, false otherwise
 * @throws std::runtime_error if configuration is invalid
**/
bool testLogConfig(json config) {

	// TODO: Refactor to use JsonValidator class

	using log = edll::DefaultConfig::Log;
	json log_config = config.value(log::KEY, json());
	std::string test_result = "";

	if (log_config.is_null()) {
		test_result.append("Missing 'log' configuration section. ");
	}
	std::string loggerName = log_config.value(log::Name::KEY, "");
	if (loggerName.empty()) {
		test_result.append("Invalid or missing logger name in configuration. ");
	}
	
	bool missing_sink = true;
	if (log_config.contains(log::Console::KEY)) {
		json console_config = log_config[log::Console::KEY];
		std::string console_level = console_config.value(log::Console::Level::KEY, "");
		if (console_level.empty()) {
			test_result.append("Invalid or missing console log level in configuration. ");
		}
		missing_sink = false;
	}
	if (log_config.contains(log::File::KEY)) {
		json file_config = log_config[log::File::KEY];
		std::string file_level = file_config.value(log::File::Level::KEY, "");
		if (file_level.empty()) {
			test_result.append("Invalid or missing file log level in configuration. ");
		}
		std::string filename = file_config.value(log::File::Filename::KEY, "");
		if (filename.empty()) {
			test_result.append("Invalid or missing log filename in configuration. ");
		}
		missing_sink = false;
	}
	if (missing_sink) {
		test_result.append("At least one logging method (console or file) must be defined in configuration. ");
	}

	if (!test_result.empty()) {
		std::cout << "Log configuration validation errors: " << test_result << std::endl;
		throw std::runtime_error("Log configuration validation failed. See previous errors.");
	}
}

// ----------------------------------------------------------------------
/** @brief Test all service configuration parameters are correctly defined
 * @param None
 * @return bool, True if configuration all parameters are valid, false otherwise
 * @throws NO EXCEPTION HANDLING
**/
bool validServiceParams(json config) {

	// TODO: Refactor to use JsonValidator class

	using service = edll::DefaultConfig::Service;
	using serviceSocket = service::Socket;
	using serviceProtocol = service::Protocol;

	json service_config = config.value(service::KEY, json());

	bool test_result = true;

	if (service_config.is_null()) {
		loggerPtr->error("Missing 'service' configuration section.");
		test_result = false;
	}
	int port = service_config.value(serviceSocket::Port::KEY, -1);
	if (port < 1 || port > 65535) {
		loggerPtr->error("Invalid port number in configuration. Expected between 1 and 65535. Received: " + std::to_string(port));
		test_result = false;
	}
	int BufferSize = service_config.value(serviceSocket::BufferSize::KEY, -1);
	if (BufferSize < 1 || BufferSize > serviceSocket::BufferSize::MAX_VALUE) {
		loggerPtr->error("Invalid buffer size in configuration. Expected between 1 and " + std::to_string(serviceSocket::BufferSize::MAX_VALUE) + ". Received: " + std::to_string(BufferSize));
		test_result = false;
	}
	int timeout = service_config.value(serviceSocket::Timeout::KEY, -1);
	if (timeout < 1) {
		loggerPtr->error("Invalid timeout value in configuration. Expected greater than 0. Received: " + std::to_string(timeout));
		test_result = false;
	}
	int sleepMs = service_config.value(serviceSocket::Sleep::KEY, -1);
	if (sleepMs < 1) {
		loggerPtr->error("Invalid sleep_ms value in configuration. Expected greater than 0. Received: " + std::to_string(sleepMs));
		test_result = false;
	}
	int bufferTTL = service_config.value(serviceSocket::BufferTTL::KEY, -1);
	if (bufferTTL < 1) {
		loggerPtr->error("Invalid buffer_ttl_period value in configuration. Expected greater than 0. Received: " + std::to_string(bufferTTL));
		test_result = false;
	}
	int pingPeriod = service_config.value(serviceProtocol::PingPeriod::KEY, -1);
	if (pingPeriod < 0) {
		loggerPtr->error("Invalid ping_period value in configuration. Expected 0 or greater. Received: " + std::to_string(pingPeriod));
		test_result = false;
	}
	if (service_config.contains(serviceProtocol::PingEnable::KEY)) {
		if (!service_config[serviceProtocol::PingEnable::KEY].is_boolean()) {
			loggerPtr->error("Invalid ping_enable value in configuration. Expected boolean type. Received: " +
				service_config[serviceProtocol::PingEnable::KEY].dump());
			test_result = false;
		}
	}

	json msgKeys = service_config.value(service::Msg::KEY, json());
	if (msgKeys.is_null()) {
		loggerPtr->error("Missing 'msg_keys' section in 'service' configuration.");
		test_result = false;
	}
	std::vector<std::string> requiredKeys = { service::Msg::End::KEY,
												service::Msg::Ping::KEY,
												service::Msg::Ack::KEY,
												service::Msg::Nack::KEY };
	for (const auto& key : requiredKeys) {
		if (!msgKeys.contains(key) || !msgKeys[key].is_string() || msgKeys[key].get<std::string>().empty()) {
			loggerPtr->error("Invalid or missing '" + key + "' in 'msg_keys' configuration.");
			test_result = false;
		}
	}

	json queueKeys = service_config.value(service::TaskKeys::KEY, json());
	if (msgKeys.is_null()) {
		loggerPtr->error("Missing 'queue' section in 'service' configuration.");
		test_result = false;
	}
	std::vector<std::string> requiredQueueKeys = { service::TaskKeys::ClientId::KEY,
													service::TaskKeys::QueueId::KEY,
													service::TaskKeys::DLLId::KEY,
													service::TaskKeys::ClientIp::KEY,
													service::TaskKeys::CommandCode::KEY,
													service::TaskKeys::CommandName::KEY,
													service::TaskKeys::Arguments::KEY };
	for (const auto& key : requiredQueueKeys) {
		if (!queueKeys.contains(key) || !queueKeys[key].is_string() || queueKeys[key].get<std::string>().empty()) {
			loggerPtr->error("Invalid or missing '" + key + "' in 'queue' configuration.");
			test_result = false;
		}
	}
	return test_result;
}

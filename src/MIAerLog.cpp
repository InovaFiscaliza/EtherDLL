#include "MIAerLog.h"

spdlog::logger logger = spdlog::logger("");

void MIAerLog::start(std::string name, bool consoleEnable, bool fileEnable, std::string consoleLevel, std::string logFilename, std::string fileLevel) {
	
	logger = spdlog::logger(name);
	logger.sinks().clear();

	if (consoleEnable) {
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		switch (consoleLevel[0]) {
		case 't':
			console_sink->set_level(spdlog::level::trace);
			break;
		case 'd':
			console_sink->set_level(spdlog::level::debug);
			break;
		case 'i':
			console_sink->set_level(spdlog::level::info);
			break;
		case 'w':
			console_sink->set_level(spdlog::level::warn);
			break;
		case 'e':
			console_sink->set_level(spdlog::level::err);
			break;
		case 'c':
			console_sink->set_level(spdlog::level::critical);
			break;
		default:
			console_sink->set_level(spdlog::level::info);
			break;
		}

		// Add the console sink to the logger
		logger.sinks().push_back(console_sink);
	}

	if (fileEnable) {
		// Create a logger sink that writes to a file
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilename, true);

		// Set the log levels for the console and file sinks
		switch (fileLevel[0]) {
		case 't':
			file_sink->set_level(spdlog::level::trace);
			break;
		case 'd':
			file_sink->set_level(spdlog::level::debug);
			break;
		case 'i':
			file_sink->set_level(spdlog::level::info);
			break;
		case 'w':
			file_sink->set_level(spdlog::level::warn);
			break;
		case 'e':
			file_sink->set_level(spdlog::level::err);
			break;
		case 'c':
			file_sink->set_level(spdlog::level::critical);
			break;
		default:
			file_sink->set_level(spdlog::level::info);
			break;
		}

		// Add the file sink to the logger
		logger.sinks().push_back(file_sink);
	}

	logger.info("Starting...");
}

void MIAerLog::logCommandExec(ERetCode errCode, std::string command) {
	if (errCode != ERetCode::API_SUCCESS)
	{
		std::string message = "[" + command + "] command failed with error code: " + ERetCodeToString(errCode);
		logger.error(message);
		errorBuffer.push_back(message);

		/*if (clientSocketCommand != NULL) {
			int iResult = send(clientSocketCommand,
				(ERetCodeToString(errCode) + "\n").c_str(),
				static_cast<int>(strlen(ERetCodeToString(errCode).c_str()) + 1),
				0);
			if (iResult == SOCKET_ERROR) {
				logger.warn("Failed to send socket. EC:" + std::to_string(WSAGetLastError()));
				logger.info("Connection with address" + std::to_string(clientSocketCommand) + " lost.");
				return;
			}
		}*/
	}
	else
	{
		logger.info("[" + command + "] command executed");
	}
}

void MIAerLog::info(std::string str) {
	logger.info(str);
}

void MIAerLog::warn(std::string str) {
	logger.warn(str);
}

void MIAerLog::error(std::string str) {
	logger.error(str);
}

MIAerLog::~MIAerLog() {
	logger.flush();
}
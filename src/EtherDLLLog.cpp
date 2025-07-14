#include "EtherDLLLog.h"

spdlog::logger logger = spdlog::logger("");

/*
* Start log in console and file
*/
void EtherDLLLog::start(std::string name, bool consoleEnable, bool fileEnable, std::string consoleLevel, std::string logFilename, std::string fileLevel) {
	
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

/*
* Felipe Machado
* 
* Log return of Scorpio API function called
*/
void EtherDLLLog::logCommandExec(ERetCode errCode, std::string command) {
	if (errCode != ERetCode::API_SUCCESS)
	{
		logger.error("[" + command + "] ERROR. " + ERetCodeToString(errCode));
	}
	else
	{
		logger.info("[" + command + "] command executed");
	}
}

/*
* Felipe Machado
*
* Informations
*/
void EtherDLLLog::info(std::string str) {
	logger.info(str);
}

/*
* Felipe Machado
*
* Warnings occured in application
*/
void EtherDLLLog::warn(std::string str) {
	logger.warn(str);
}

/*
* Felipe Machado
*
* Errors occured in application
*/
void EtherDLLLog::error(std::string str) {
	logger.error(str);
}

EtherDLLLog::~EtherDLLLog() {
	logger.flush();
}
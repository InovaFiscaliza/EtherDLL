/**
 * @file EtherDLLLog.cpp
 * @brief
 *
 * This header file contains function declarations for converting JSON objects
 * to various Scorpio API data structures used in the DLL interface.
 *
 * @author fslobao
 * @date 2025-09-10
 * @version 1.0
 *
 * @note Requires C++14 or later
 * @note Uses nlohmann/json library for JSON parsing
 *
 * Dependencies:
 * - nlohmann/json.hpp
 * - spdlog/spdlog.h
 *
 **/
// ----------------------------------------------------------------------
#pragma once

// Include core EtherDLL libraries
#include "EtherDLLConstants.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Include general C++ libraries
#include <string>

// For convenience
using json = nlohmann::json;


// ----------------------------------------------------------------------
/**
* @brief Test JSON object contains the required information for logging configuration.
*
* A valid logging configuration must contain:
* - A logger name;
* - At least one logging method (console or file) enabled;
* - If console logging is enabled, it must contain a log level;
* - If file logging is enabled, it must contain a filename and a log level.
*
* @param config: JSON object containing the logging configuration
* @return bool: true if the configurat ion is valid, false otherwise
* @throws NO EXCEPTION HANDLING
**/
bool validLogConfig(json config) {

	bool validLog = true;

	// at least one logging method (console or file) must be enabled
	if (config.contains("console")) {
		validLog &= config["console"].contains("level");
	}

	if (config.contains("file")) {
		validLog &= (config["file"].contains("filename") &&
			config["file"].contains("level"));
	}

	// logger must have a name
	validLog &= config.contains("name");

	return validLog;
}

// ----------------------------------------------------------------------
/**
 * @brief Initialize the logger based on the provided JSON configuration.
 *
 * @param config: JSON object containing valid logging configuration
 * @return std::shared_ptr<spdlog::logger>: Shared pointer to the initialized logger
 * @throws std::invalid_argument if the configuration is invalid
**/
void initializeLog(json config, spdlog::logger& logger) {

	if (!validLogConfig(config["log"])) {
		throw std::invalid_argument("Invalid log configuration. Check documentation or try removing the config file to obtain a working example.");
		exit(edll::Code::SERVICE_ERROR);
	}

	json log_config = config["log"].get<json>();

	logger.sinks().clear();

	// add the console sink to the logger
	if (log_config.contains("console")) {
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		char console_level = log_config["console"]["level"].get<std::string>()[0];
		switch (console_level) {
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
			throw std::invalid_argument("Invalid console log level. Use trace, debug, info, warning, error or critical.");
			break;
		}
		logger.sinks().push_back(console_sink);
	}

	// Add the file sink to the logger
	if (log_config.contains("file")) {
		std::string filename = log_config["file"]["filename"].get<std::string>();
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);

		char fileLevel = log_config["file"]["level"].get<std::string>()[0];
		switch (fileLevel) {
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

		logger.sinks().push_back(file_sink);
	}

	logger.info("Starting " + log_config["name"].get<std::string>() + "...");

}
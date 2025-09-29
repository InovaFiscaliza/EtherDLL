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

// Include to DLL specific headers

// Include core EtherDLL libraries

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries

// For convenience
using json = nlohmann::json;


// Logger object
std::unique_ptr<spdlog::logger> loggerPtr = nullptr;


// Function prototypes
bool validLogConfig(nlohmann::json config);
std::shared_ptr<spdlog::logger> initializeLog(nlohmann::json config);

// ----------------------------------------------------------------------
/** @brief Helper function to get logger reference
 *
 * This function returns a reference to the globally initialized logger.
 * If the logger is not initialized, it throws a runtime error.
 *
 * @return spdlog::logger&: Reference to the initialized logger
 * @throws std::runtime_error if the logger is not initialized
 **/
spdlog::logger& getLogger() {
	if (!loggerPtr) {
		throw std::runtime_error("Logger not initialized");
	}
	return *loggerPtr;
}



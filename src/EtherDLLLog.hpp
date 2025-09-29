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

// Function prototypes
bool validLogConfig(nlohmann::json config);
std::shared_ptr<spdlog::logger> initializeLog(nlohmann::json config);

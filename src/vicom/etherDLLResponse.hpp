/**
* @file etherDLLResponse.hpp
*
* @brief Header file for functions that convert Vicom DLL responses to JSON
*
* @author fslobao
* @date 2025-12-03
* @version 1.1
*
* @note Requires C++17 or later
* @note Uses nlohmann/json library for JSON handling
*
* * * Dependencies:
* * - nlohmann/json.hpp
**/

// ----------------------------------------------------------------------
#pragma once

// Include core EtherDLL libraries
#include "stdafx.h"
#include "EtherDLLClient.hpp"
#include "EtherDLLLog.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>


// For convenience
using json = nlohmann::json;

// Global variables
extern MessageQueue response;
extern spdlog::logger* loggerPtr;

// ----------------------------------------------------------------------
// NOTE:
// The Vicom power scan implementation is synchronous.
// A request is sent, and the function blocks until a result or timeout occurs.
// The result is then processed and pushed to the response queue directly within
// the `DLLFunctionCall` in `etherDLLRequest.hpp`.
//
// Therefore, unlike the Scorpio implementation, this file does not contain
// asynchronous callback functions (e.g., OnDataFunc, OnErrorFunc).
// It is kept for structural consistency and for future use if asynchronous
// features of the Vicom API are implemented.
// ----------------------------------------------------------------------

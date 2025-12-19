/**
 * @file EtherDLLCodes.hpp
 * @brief Declarations of functions to convert codes used in the API to human readable messages
 *
 * This header file contains function declarations for converting 
 * various Vicom API codes to humam readable formats.
 *
 * @author fslobao
 * @date 2025-09-12
 * @version 1.1
 *
 * @note Requires C++14 or later
 *
 * Special Dependencies:
 * - ViComError.h
 *
 **/

 // ----------------------------------------------------------------------
#pragma once

// Include provided DLL libraries
#include "stdafx.h"
#include <atlstr.h>
#include "ViComError.h"

// Include project libraries
#include <nlohmann/json.hpp>

// Include general C++ libraries
#include <string>

// For convenience
using json = nlohmann::json;
using namespace RohdeSchwarz::ViCom;

// ----------------------------------------------------------------------
/** @brief Function to convert CViComError to a JSON object
 *
 * @param err: CViComError object to be converted
 * @return json: JSON object representing the error
 * @throws NO EXCEPTION HANDLING
**/
json vicomErrorToJson(const CViComError& err)
{
	json errorJson;
	errorJson["error_code"] = err.GetErrorCode();
	CStringA ansiErrorString(err.GetErrorString());
	errorJson["error_message"] = ansiErrorString.GetString();
	return errorJson;
}

// ----------------------------------------------------------------------
/**
 * @brief Defines the command codes for the Vicom specific DLL.
 * 
 * These codes are used in the JSON messages to identify the command to be executed.
 */
struct VicomTask
{
	static constexpr const char* POWER_SCAN_NAME = "power_scan";
	static constexpr unsigned long POWER_SCAN_CODE = 1;

	static constexpr const char* GPS_GET_LOCATION_NAME = "gps_get_location";
	static constexpr unsigned long GPS_GET_LOCATION_CODE = 2;
};
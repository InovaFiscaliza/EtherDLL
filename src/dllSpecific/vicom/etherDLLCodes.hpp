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
	static constexpr const char* GPS_GET_LOCATION_NAME = "gpsGetLocation";
	static constexpr unsigned long GPS_GET_LOCATION_CODE = 2;

	static constexpr const char* STOP_MEASUMENT = "stopPowerScanMeasurement";
	static constexpr unsigned long STOP_MEASUMENT_CODE = 3;

	static constexpr const char* IDN_NAME = "idn";
	static constexpr unsigned long IDN_CODE = 4;

	static constexpr const char* GET_SETTINGS_NAME = "getSettings";
	static constexpr unsigned long GET_SETTINGS_CODE = 5;

	static constexpr const char* POWER_SCAN_CONFIG_NAME = "powerScanConfig";
	static constexpr unsigned long POWER_SCAN_CONFIG_CODE = 6;

	static constexpr const char* POWER_SCAN_MEASURE_NAME = "startPowerScanMeasurement";
	static constexpr unsigned long POWER_SCAN_MEASURE_CODE = 7;
};
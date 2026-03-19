/**
 * @file etherDLLCodes.hpp
 * @brief Error conversion and command code definitions for ViCom DLL
 * 
 * This header file contains:
 * - Function to convert CViComError to JSON format
 * - Command code definitions for Vicom-specific tasks
 * 
 * @author mlnunes@anatel.gov.br
 * @date 2026-3-19
 * @version 1.3
 * 
 * @note Requires C++14 or later
 * @note Uses nlohmann/json library for JSON handling
 * 
 * Dependencies:
 * - ViComError.h (from ViCom API)
 * 
 * @ingroup vicom
 */

// EtherDLLCodes.hpp
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
// Convert CViComError to JSON object
// @brief Transforms ViCom error object into JSON format for client response
// @param err CViComError object to be converted
// @return JSON object with error_code and error_message fields
// @ingroup vicom_error
// ----------------------------------------------------------------------
inline json vicomErrorToJson(const CViComError& err)
{
   json errorJson;
   errorJson["error_code"] = err.GetErrorCode();
   CStringA ansiErrorString(err.GetErrorString());
   errorJson["error_message"] = ansiErrorString.GetString();
   return errorJson;
}

// ----------------------------------------------------------------------
// Command codes for ViCom-specific DLL tasks
// @brief Defines command names and codes used in JSON messages
// @details These codes identify the command to be executed by the DLL
// @ingroup vicom_tasks
// ----------------------------------------------------------------------
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
/**
 * @file scorpioDLLRequest.h
 * @brief JSON to Scorpio API data structure conversion functions
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
 * - ScorpioAPITypes.h
 * 
 **/

#pragma once

#include "StdAfx.h"
// #include <sstream>
// #include <fstream>

#include <nlohmann/json.hpp>
#include "ScorpioAPITypes.h"

SOccupReqData* jsonToSOccupReqData(nlohmann::json jsonObj);
SOccDFReqData* jsonToSOccDFReqData(nlohmann::json jsonObj);
SGetPanParams jsonToSGetPanParams(nlohmann::json jsonObj);
SMeasReqData* jsonToSMeasReqData(nlohmann::json jsonObj);
SAudioParams jsonToSAudioParams(nlohmann::json jsonObj);
SAVDReqData* jsonToSAVDReqData(nlohmann::json jsonObj);
SPanParams jsonToSPanParams(nlohmann::json jsonObj);
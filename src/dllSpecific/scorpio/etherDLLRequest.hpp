/**
 * @file etherDLLRequest.hpp
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
 // ----------------------------------------------------------------------
#pragma once

 // Include core EtherDLL libraries
#include "EtherDLLClient.hpp"
#include "EtherDLLConstants.hpp"
#include "EtherDLLAudio.hpp"
#include "EtherDLLAudioCommon.h"

// Include DLL specific libraries
#include "etherDLLRequest.hpp"
#include "etherDLLCodes.hpp"
#include "etherDLLConfig.hpp"

// Include provided DLL libraries
#include "ScorpioAPITypes.h"
#include "ScorpioAPIDll.h"
/*
#include "stdafx.h"
#include "Units.h"
#include "EquipCtrlMsg.h"
*/

// Include general C++ libraries
#include <string>

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// For convenience
using json = nlohmann::json;

// ----------------------------------------------------------------------
// Function prototypes
SAudioParams jsonToSAudioParams(nlohmann::json jsonObj);
SGetPanParams jsonToSGetPanParams(nlohmann::json jsonObj);
SPanParams jsonToSPanParams(nlohmann::json jsonObj);
SMeasReqData* jsonToSMeasReqData(nlohmann::json jsonObj);
SOccupReqData* jsonToSOccupReqData(nlohmann::json jsonObj);
SOccDFReqData* jsonToSOccDFReqData(nlohmann::json jsonObj);
SAVDReqData* jsonToSAVDReqData(nlohmann::json jsonObj);

std::string validateRequest(json request, ECSMSDllMsgType msgType, spdlog::logger& log);
void DLLFunctionCall(DLLConnectionData DLLConnID, json request, CLoopbackCapture& loopbackCapture, spdlog::logger& log);
void processRequestQueue(DLLConnectionData DLLConnID, MessageQueue& request, edll::INT_CODE& interruptionCode, spdlog::logger& log);
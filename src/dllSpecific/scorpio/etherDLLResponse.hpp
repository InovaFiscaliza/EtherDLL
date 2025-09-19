/**
* @file etherDLLResponse.hpp
*
* @brief Header file for functions that convert Scorpio DLL responses to JSON
* * Define function prototypes for callback functions used by the Scorpio API
* * Define function prototypes for converting DLL data structures to JSON
*
* * @author fslobao
* * @date 2025-09-17
* * @version 1.0
*
* * @note Requires C++11 or later
* * @note Uses nlohmann/json library for JSON handling
*
* * * Dependencies:
* * - nlohmann/json.hpp
**/

// ----------------------------------------------------------------------
#pragma once

// Include core EtherDLL libraries

// Include DLL specific libraries

// Include provided DLL libraries
#include "ScorpioAPITypes.h"
#include "EquipCtrlMsg.h"
#include "SmsRealtimeMsg.h"

// Include general C++ libraries
#include <string>

// Include project libraries
#include <nlohmann/json.hpp>

// For convenience
using json = nlohmann::json;

constexpr double PAN_OFFSET = 192.0;
constexpr double FREQ_FACTOR = 1920.0;

// ----------------------------------------------------------------------
// Callback function prototypes
void OnDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ unsigned long desstAddr, _In_ SEquipCtrlMsg::UBody* data);
void OnErrorFunc(_In_  unsigned long serverId, _In_ const std::wstring& errorMsg);
void OnRealTimeDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data);

// Core processing functions
json processBITEResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* respdata);
json ProcessAntListResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
json processAutoViolateResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
json processMeasResponse(_In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ SEquipCtrlMsg::UBody* data);
json processDemodCtrlResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
json processPanResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
json processOccupancyResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
json processOccupancyDFResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data);
json ProcessRealTimeData(_In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data);

// Anxilary functions
json ProcessGpsData(SEquipCtrlMsg::SGpsResponse* gpsResponse);
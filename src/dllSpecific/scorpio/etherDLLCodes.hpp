/**
 * @file EtherDLLCodes.hpp
 * @brief Declarations of functions to convert codes used in the API to human readable messages
 *
 * This header file contains function declarations for converting 
 * various Scorpio API codes to humam readable formats.
 *
 * @author fslobao
 * @date 2025-09-12
 * @version 1.0
 *
 * @note Requires C++14 or later
 *
 * Special Dependencies:
 * - ScorpioAPITypes.h
 * - ScorpioAPIDll.h
 *
 **/

 // ----------------------------------------------------------------------
#pragma once

#include <string>

#include "ScorpioAPITypes.h"
#include "EquipCtrlMsg.h"

// ----------------------------------------------------------------------
// Function prototype
std::string ERetCodeToString(ERetCode code);
std::string ECSMSDllMsgTypeToString(ECSMSDllMsgType code);
std::string BISTResultToString(SEquipCtrlMsg::SGetBistResp::EResult code);
std::string eAntToString(SSmsMsg::EAnt code);
std::string eAntPolToString(SSmsMsg::EAntPol code);
std::string eStateRespToString(SEquipCtrlMsg::SStateResp::EState code);
std::string eStatusToString(SEquipCtrlMsg::SEquipTaskStatusResp::EStatus code);
std::string eErrorCodeToString(ErrorCodes::EErrorCode code);

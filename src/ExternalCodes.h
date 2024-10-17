// messages.h
#pragma once

#include <string>
#include "ScorpioAPIDll.h" // Include the external header where ERetCode is defined

// Function prototype
std::string ERetCodeToString(ERetCode code);
std::string ECSMSDllMsgTypeToString(ECSMSDllMsgType code);
std::string BISTResultToString(SEquipCtrlMsg::SGetBistResp::EResult code);
std::string eAntToString(SSmsMsg::EAnt code);
std::string eAntPolToString(SSmsMsg::EAntPol code);
std::string eStateRespToString(SEquipCtrlMsg::SStateResp::EState code);
std::string eErrorCodeToString(ErrorCodes::EErrorCode code);
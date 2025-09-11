#include "StdAfx.h"
#include <sstream>
#include <fstream>
#include <json/json.hpp>
#include "ScorpioAPITypes.h"

#include <EtherDLLLog.h>

extern EtherDLLLog logEtherDLL;
extern nlohmann::json config;

std::vector<std::string> split(std::string s, std::string delimiter);
SOccupReqData* jsonToSOccupReqData(nlohmann::json jsonObj);
SOccDFReqData* jsonToSOccDFReqData(nlohmann::json jsonObj);
SGetPanParams jsonToSGetPanParams(nlohmann::json jsonObj);
SMeasReqData* jsonToSMeasReqData(nlohmann::json jsonObj);
SAudioParams jsonToSAudioParams(nlohmann::json jsonObj);
SAVDReqData* jsonToSAVDReqData(nlohmann::json jsonObj);
std::wstring stringToWString(const std::string& str);
SPanParams jsonToSPanParams(nlohmann::json jsonObj);
unsigned long stringToUnsignedLong(std::string str);
bool stringToBool(std::string str);
void newDefaultConfigFile();
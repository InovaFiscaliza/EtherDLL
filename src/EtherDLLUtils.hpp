#include "StdAfx.h"
#include <sstream>
#include <fstream>
#include <json/json.hpp>
#include "ScorpioAPITypes.h"

#include <EtherDLLLog.h>

extern EtherDLLLog logEtherDLL;
extern nlohmann::json config;

std::vector<std::string> split(std::string s, std::string delimiter);
std::wstring stringToWString(const std::string& str);
unsigned long stringToUnsignedLong(std::string str);
bool stringToBool(std::string str);
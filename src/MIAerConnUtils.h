#include "stdafx.h"
#include <sstream>
#include <nlohmann/json.hpp>
#include "ScorpioAPITypes.h"

std::vector<std::string> split(std::string s, std::string delimiter);
SOccupReqData* stringToSOccupReqData(const std::string str);
SOccDFReqData* stringToSOccDFReqData(const std::string str);
SAVDReqData* stringToSAVDReqData(const std::string str);
std::wstring stringToWString(const std::string& str);
unsigned long stringToUnsignedLong(std::string str);
bool stringToBool(std::string str);
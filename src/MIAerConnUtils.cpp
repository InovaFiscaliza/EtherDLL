#include "MIAerConnUtils.h"

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

SOccupReqData* stringToSOccupReqData(const std::string str) {
	SOccupReqData* structSO;
	using json = nlohmann::json;
	json jsonObj = json::parse(str);

	structSO->ant = jsonObj["ant"].is_null() == true ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["ant"].get<SEquipCtrlMsg::EAnt>();
	
	SSmsMsg::SBandV4 band;

	if (jsonObj["band"]["channelBandwidth"].is_null() == false) {
		band.channelBandwidth = Units::Frequency(jsonObj["band"]["channelBandwidth"].get<unsigned long>()).GetRaw();
	}
	band.exclude = jsonObj["band"]["exclude"].is_null() ? NULL : jsonObj["band"]["exclude"].get<bool>();
	
	if (jsonObj["band"]["highFrequency"].is_null() == false) {
		band.highFrequency = Units::Frequency(jsonObj["band"]["highFrequency"].get<unsigned long>()).GetRaw();
	}
	
	if (jsonObj["band"]["lowFrequency"].is_null() == false) {
		band.lowFrequency = Units::Frequency(jsonObj["band"]["lowFrequency"].get<unsigned long>()).GetRaw();
	}

	band.sType.signalType.horizPol = jsonObj["band"]["sType"]["signalType"]["horizPol"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["horizPol"].get<unsigned long>();
	band.sType.signalType.narrow = jsonObj["band"]["sType"]["signalType"]["narrow"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["narrow"].get<unsigned long>();
	band.sType.signalType.unused0 = jsonObj["band"]["sType"]["signalType"]["unused0"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["unused0"].get<unsigned long>();
	band.sType.signalType.unused1 = jsonObj["band"]["sType"]["signalType"]["unused1"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["unused1"].get<unsigned long>();

	structSO->band[0] = band;
	structSO->confidenceLevel = jsonObj["confidenceLevel"].is_null() == true ? NULL : jsonObj["confidenceLevel"].get<unsigned char>();
	structSO->desiredAccuracy = jsonObj["desiredAccuracy"].is_null() == true ? NULL : jsonObj["desiredAccuracy"].get<unsigned char>();
	structSO->durationMethod = jsonObj["durationMethod"].is_null() == true ? (SEquipCtrlMsg::EDurationMethod)NULL : jsonObj["durationMethod"].get<SEquipCtrlMsg::EDurationMethod>();
	structSO->measurementTime = jsonObj["measurementTime"].is_null() == true ? NULL : jsonObj["measurementTime"].get<unsigned long>();
	structSO->numBands = jsonObj["numBands"].is_null() == true ? NULL : jsonObj["numBands"].get<unsigned long>();
	structSO->occPrimaryThreshold[0] = jsonObj["occPrimaryThreshold"][0].is_null() == true ? NULL : jsonObj["occPrimaryThreshold"][0].get<short>();
	structSO->occupancyMinGap = jsonObj["occupancyMinGap"].is_null() == true ? NULL : jsonObj["occupancyMinGap"].get<unsigned long>();
	structSO->storageTime = jsonObj["storageTime"].is_null() == true ? NULL : jsonObj["storageTime"].get<unsigned long>();
	structSO->thresholdMethod = jsonObj["thresholdMethod"].is_null() == true ? (SEquipCtrlMsg::EThresholdMethod)NULL : jsonObj["thresholdMethod"].get<SEquipCtrlMsg::EThresholdMethod>();
	
	return structSO;
}

SOccDFReqData* stringToSOccDFReqData(const std::string str) {
	SOccDFReqData* structSO;
	using json = nlohmann::json;
	json jsonObj = json::parse(str);

	SSmsMsg::SGetScanDfCmdV1::SBand band;

	if (jsonObj["band"]["channelBandwidth"].is_null() == false) {
		band.channelBandwidth = Units::Frequency(jsonObj["band"]["channelBandwidth"].get<unsigned long>()).GetRaw();
	}
	
	band.exclude = jsonObj["band"]["exclude"].is_null() == true ? NULL : jsonObj["band"]["exclude"].get<bool>();
	
	if (jsonObj["band"]["highFrequency"].is_null() == false) {
		band.highFrequency = Units::Frequency(jsonObj["band"]["highFrequency"].get<unsigned long>()).GetRaw();
	}
	
	if (jsonObj["band"]["lowFrequency"].is_null() == false) {
		band.lowFrequency = Units::Frequency(jsonObj["band"]["lowFrequency"].get<unsigned long>()).GetRaw();
	}
	
	band.signalType.gsm = jsonObj["band"]["signalType"]["gsm"].is_null() == true ? NULL : jsonObj["band"]["signalType"]["gsm"].get<unsigned char>();
	band.signalType.horizPol = jsonObj["band"]["signalType"]["horizPol"].is_null() == true ? NULL : jsonObj["band"]["signalType"]["horizPol"].get<unsigned char>();
	band.signalType.narrow = jsonObj["band"]["signalType"]["narrow"].is_null() == true ? NULL : jsonObj["band"]["signalType"]["narrow"].get<unsigned char>();
	band.signalType.unused = jsonObj["band"]["signalType"]["unused"].is_null() == true ? NULL : jsonObj["band"]["signalType"]["unused"].get<unsigned char>();

	//TODO
	//structSO->band[0] = band;
	
	structSO->storageTime = jsonObj["storageTime"].is_null() == true ? NULL : jsonObj["storageTime"].get<unsigned long>();
	structSO->measurementTime = jsonObj["measurementTime"].is_null() == true ? NULL : jsonObj["measurementTime"].get<unsigned long>();
	structSO->numAzimuths = jsonObj["numAzimuths"].is_null() == true ? NULL :jsonObj["numAzimuths"].get<unsigned long>();
	structSO->confidence = jsonObj["confidence"].is_null() == true ? NULL : jsonObj["confidence"].get<unsigned long>();
	structSO->recordHoldoff = jsonObj["recordHoldoff"].is_null() == true ? NULL : jsonObj["recordHoldoff"].get<unsigned long>();
	structSO->scanDfThreshold = jsonObj["scanDfThreshold"].is_null() == true ? NULL : jsonObj["scanDfThreshold"].get<unsigned char>();
	structSO->recordAudioDf = jsonObj["recordAudioDf"].is_null() == true ? NULL : jsonObj["recordAudioDf"].get<bool>();
	structSO->numBands = jsonObj["numBands"].is_null() == true ? NULL : jsonObj["numBands"].get<unsigned short>();

	return structSO;
}

SAVDReqData* stringToSAVDReqData(const std::string str) {
	SAVDReqData* structSO;
	using json = nlohmann::json;
	json jsonObj = json::parse(str);

	structSO->ant = jsonObj["ant"].is_null() == true ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["ant"].get<SEquipCtrlMsg::EAnt>();
	structSO->avdThreshold = jsonObj["avdThreshold"].is_null() == true ? NULL : jsonObj["avdThreshold"].get<unsigned char>();

	SSmsMsg::SBandV4 band;

	if (jsonObj["band"]["channelBandwidth"].is_null() == false) {
		band.channelBandwidth = Units::Frequency(jsonObj["band"]["channelBandwidth"].get<unsigned long>()).GetRaw();
	}

	band.exclude = jsonObj["band"]["exclude"].is_null() == true ? NULL : jsonObj["band"]["exclude"].get<bool>();

	if (jsonObj["band"]["highFrequency"].is_null() == false) {
		band.highFrequency = Units::Frequency(jsonObj["band"]["highFrequency"].get<unsigned long>()).GetRaw();
	}

	if (jsonObj["band"]["lowFrequency"].is_null() == false) {
		band.lowFrequency = Units::Frequency(jsonObj["band"]["lowFrequency"].get<unsigned long>()).GetRaw();
	}

	band.sType.signalType.horizPol = jsonObj["band"]["sType"]["signalType"]["horizPol"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["horizPol"].get<unsigned long>();
	band.sType.signalType.narrow = jsonObj["band"]["sType"]["signalType"]["narrow"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["narrow"].get<unsigned long>();
	band.sType.signalType.unused0 = jsonObj["band"]["sType"]["signalType"]["unused0"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["unused0"].get<unsigned long>();
	band.sType.signalType.unused1 = jsonObj["band"]["sType"]["signalType"]["unused1"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["unused1"].get<unsigned long>();

	structSO->band[0] = band;

	structSO->measurementRate = jsonObj["measurementRate"].is_null() == true ? (SEquipCtrlMsg::EAvdRate)NULL : jsonObj["measurementRate"].get<SEquipCtrlMsg::EAvdRate>();
	structSO->measurementTime = jsonObj["measurementTime"].is_null() == true ? NULL : jsonObj["measurementTime"].get<unsigned long>();
	structSO->numBands = jsonObj["numBands"].is_null() == true ? NULL : jsonObj["numBands"].get<unsigned long>();
	structSO->storageTime = jsonObj["storageTime"].is_null() == true ? NULL : jsonObj["storageTime"].get<unsigned long>();

	return structSO;
}

std::wstring stringToWString(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}

unsigned long stringToUnsignedLong(std::string str) {
	return strtoul(str.c_str(), NULL, 0);
}

bool stringToBool(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}

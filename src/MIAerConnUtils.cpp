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

//SOccupReqData* convertStringToSOccupReqData(const std::string str) {
	//SOccupReqData* structSO;
	/*using json = nlohmann::json;
	json config;
	config = json::parse(str);

	structSO->ant = config["ant"].get<SEquipCtrlMsg::EAnt>();

	SSmsMsg::SBandV4 band;

	band.channelBandwidth = Units::Frequency(config["band"]["channelBandwidth"].get<unsigned long>()).GetRaw();
	band.exclude = config["band"]["exclude"].get<bool>();
	band.highFrequency = Units::Frequency(config["band"]["highFrequency"].get<unsigned long>()).GetRaw();
	band.lowFrequency = Units::Frequency(config["band"]["lowFrequency"].get<unsigned long>()).GetRaw();
	band.sType.signalType.horizPol = config["band"]["sType"]["signalType"]["horizPol"].get<unsigned long>();
	band.sType.signalType.narrow = config["band"]["sType"]["signalType"]["narrow"].get<unsigned long>();
	band.sType.signalType.unused0 = config["band"]["sType"]["signalType"]["unused0"].get<unsigned long>();
	band.sType.signalType.unused1 = config["band"]["sType"]["signalType"]["unused1"].get<unsigned long>();

	structSO->band[0] = band;
	structSO->confidenceLevel = config["confidenceLevel"].get<unsigned char>();
	structSO->desiredAccuracy = config["desiredAccuracy"].get<unsigned char>();
	structSO->durationMethod = config["durationMethod"].get<SEquipCtrlMsg::EDurationMethod>();
	structSO->measurementTime = config["measurementTime"].get<unsigned long>();
	structSO->numBands = config["numBands"].get<unsigned long>();
	structSO->occPrimaryThreshold[0] = config["occPrimaryThreshold"][0].get<short>();
	structSO->occupancyMinGap = config["occupancyMinGap"].get<unsigned long>();
	structSO->storageTime = config["storageTime"].get<unsigned long>();
	structSO->thresholdMethod = config["thresholdMethod"].get<SEquipCtrlMsg::EThresholdMethod>();
	*/
	//return structSO;
//}

/*
SOccDFReqData* convertStringToSOccDFReqData(const std::string str) {
	SOccDFReqData* structSO;
	using json = nlohmann::json;
	json config;
	config = json::parse(str);

	SSmsMsg::SGetScanDfCmdV1::SBand band;

	band.channelBandwidth = Units::Frequency(config["band"]["channelBandwidth"].get<unsigned long>()).GetRaw();
	band.exclude = config["band"]["exclude"].get<bool>();
	band.highFrequency = Units::Frequency(config["band"]["highFrequency"].get<unsigned long>()).GetRaw();
	band.lowFrequency = Units::Frequency(config["band"]["lowFrequency"].get<unsigned long>()).GetRaw();
	band.signalType.gsm = config["band"]["signalType"]["gsm"].get<unsigned char>();
	band.signalType.horizPol = config["band"]["signalType"]["horizPol"].get<unsigned char>();
	band.signalType.narrow = config["band"]["signalType"]["narrow"].get<unsigned char>();
	band.signalType.unused = config["band"]["signalType"]["unused"].get<unsigned char>();

	structSO->band[0] = band;

	structSO->storageTime = config["storageTime"].get<unsigned long>();
	structSO->measurementTime = config["measurementTime"].get<unsigned long>();
	structSO->numAzimuths = config["numAzimuths"].get<unsigned long>();
	structSO->confidence = config["confidence"].get<unsigned long>();
	structSO->recordHoldoff = config["recordHoldoff"].get<unsigned long>();
	structSO->scanDfThreshold = config["scanDfThreshold"].get<unsigned char>();
	structSO->recordAudioDf = config["recordAudioDf"].get<bool>();
	structSO->numBands = config["numBands"].get<unsigned short>();

	SEquipCtrlMsg::SRcvrCtrlCmd rcvrCtrl;		// for recording (RA3 function)
	SSmsMsg::SGetScanDfCmdV1::SBand band[1]; // Variable length

	return structSO;
}

SAVDReqData* convertStringToSAVDReqData(const std::string str) {
	SAVDReqData* structSO;
	using json = nlohmann::json;
	json config;
	config = json::parse(str);

	structSO->ant = config["ant"].get<SEquipCtrlMsg::EAnt>();
	structSO->avdThreshold = config["avdThreshold"].get<unsigned char>();

	SSmsMsg::SBandV4 band;

	band.channelBandwidth = Units::Frequency(config["band"]["channelBandwidth"].get<unsigned long>()).GetRaw();
	band.exclude = config["band"]["exclude"].get<bool>();
	band.highFrequency = Units::Frequency(config["band"]["highFrequency"].get<unsigned long>()).GetRaw();
	band.lowFrequency = Units::Frequency(config["band"]["lowFrequency"].get<unsigned long>()).GetRaw();
	band.sType.signalType.horizPol = config["band"]["sType"]["signalType"]["horizPol"].get<unsigned long>();
	band.sType.signalType.narrow = config["band"]["sType"]["signalType"]["narrow"].get<unsigned long>();
	band.sType.signalType.unused0 = config["band"]["sType"]["signalType"]["unused0"].get<unsigned long>();
	band.sType.signalType.unused1 = config["band"]["sType"]["signalType"]["unused1"].get<unsigned long>();

	structSO->band[0] = band;

	structSO->measurementRate = config["measurementRate"].get<SEquipCtrlMsg::EAvdRate>();
	structSO->measurementTime = config["measurementTime"].get<unsigned long>();
	structSO->numBands = config["numBands"].get<unsigned long>();
	structSO->storageTime = config["storageTime"].get<unsigned long>();

	return structSO;
}

*/

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

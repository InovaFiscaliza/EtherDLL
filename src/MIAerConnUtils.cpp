#include "MIAerConnUtils.h"

void newDefaultConfigFile() {
	using json = nlohmann::json;
	json jsonObj;

	jsonObj["log"]["console"]["enable"] = true;
	jsonObj["log"]["console"]["level"] = "trace";

	jsonObj["log"]["file"]["enable"] = true;
	jsonObj["log"]["file"]["level"] = "trace";
	jsonObj["log"]["file"]["path"] = "log.txt";

	jsonObj["station"]["address"] = "166.139.112.178";
	jsonObj["station"]["port"] = 3303;
	jsonObj["station"]["timeout_s"] = 10;

	jsonObj["service"]["command"]["port"] = 3000;
	jsonObj["service"]["command"]["timeout_s"] = 10000;
	jsonObj["service"]["command"]["sleep_ms"] = 100;
	jsonObj["service"]["command"]["check_period"] = 10;

	jsonObj["service"]["stream"]["port"] = 3001;
	jsonObj["service"]["stream"]["timeout_s"] = 10000;
	jsonObj["service"]["stream"]["sleep_ms"] = 500;
	jsonObj["service"]["stream"]["check_period"] = 200;

	jsonObj["service"]["error"]["port"] = 3002;
	jsonObj["service"]["error"]["timeout_s"] = 10000;
	jsonObj["service"]["error"]["sleep_ms"] = 500;
	jsonObj["service"]["error"]["check_period"] = 200;

	jsonObj["service"]["realtime"]["port"] = 3002;
	jsonObj["service"]["realtime"]["timeout_s"] = 10000;
	jsonObj["service"]["realtime"]["sleep_ms"] = 500;
	jsonObj["service"]["realtime"]["check_period"] = 200;

	jsonObj["service"]["simulated"] = true;

	std::ofstream file("config.json");
	file << jsonObj;

}

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

SAudioParams jsonToSAudioParams(nlohmann::json jsonObj) {
	SAudioParams structSO;

	structSO.anyChannel = jsonObj["anyChannel"].is_null() == true ? NULL : jsonObj["anyChannel"].get<bool>();
	if (jsonObj["bandwidth"].is_null() == false) {
		structSO.bandwidth = Units::Frequency(jsonObj["bandwidth"].get<unsigned long>()).GetRaw();
	}
	if (jsonObj["bfo"].is_null() == false) {
		structSO.bandwidth = Units::Frequency(jsonObj["bfo"].get<unsigned long>()).GetRaw();
	}
	structSO.channel = jsonObj["channel"].is_null() == true ? NULL : jsonObj["channel"].get<unsigned long>();
	structSO.detMode = jsonObj["detMode"].is_null() == true ? (SSmsMsg::SRcvrCtrlCmdV1::EDetMode)NULL : jsonObj["detMode"].get<SSmsMsg::SRcvrCtrlCmdV1::EDetMode>();
	structSO.doModRec = jsonObj["doModRec"].is_null() == true ? NULL : jsonObj["doModRec"].get<bool>();;
	structSO.doRDS = jsonObj["doRDS"].is_null() == true ? NULL : jsonObj["doRDS"].get<bool>();
	if (jsonObj["freq"].is_null() == false) {
		structSO.freq = Units::Frequency(jsonObj["freq"].get<unsigned long>()).GetRaw();
	}
	if (jsonObj["doRDS"].is_null() == false) {
		strncpy_s(structSO.ipAddressRDSRadio, jsonObj["doRDS"].get<std::string>().c_str(), SSmsMsg::IP_ADDRESS_LEN);
	}
	structSO.streamID = jsonObj["streamID"].is_null() == true ? NULL : jsonObj["streamID"].get<unsigned long>();

	return structSO;
}

SGetPanParams jsonToSGetPanParams(nlohmann::json jsonObj) {
	SGetPanParams structSO;

	if (jsonObj["bandwidth"].is_null() == false) {
		structSO.bandwidth = Units::Frequency(jsonObj["bandwidth"].get<unsigned long>()).GetRaw();
	}
	if (jsonObj["freq"].is_null() == false) {
		structSO.freq = Units::Frequency(jsonObj["freq"].get<unsigned long>()).GetRaw();
	}
	structSO.rcvrAtten = jsonObj["rcvrAtten"].is_null() == true ? NULL : jsonObj["rcvrAtten"].get<unsigned char>();
	
	return structSO;
}

SPanParams jsonToSPanParams(nlohmann::json jsonObj) {
	SPanParams structSO;

	structSO.antenna = jsonObj["antenna"].is_null() == true ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["antenna"].get<SEquipCtrlMsg::EAnt>();
	
	SEquipCtrlMsg::SRcvrCtrlCmd rcvr;
	if (jsonObj["rcvr"]["freq"].is_null() == false) {
		rcvr.freq = Units::Frequency(jsonObj["rcvr"]["freq"].get<unsigned long>()).GetRaw();
	}
	if (jsonObj["rcvr"]["freq"].is_null() == false) {
		rcvr.bandwidth = Units::Frequency(jsonObj["rcvr"]["bandwidth"].get<unsigned long>()).GetRaw();
	}
	rcvr.detMode = jsonObj["rcvr"]["detMode"].is_null() == true ? (SSmsMsg::SRcvrCtrlCmdV1::EDetMode)NULL : jsonObj["rcvr"]["detMode"].get<SSmsMsg::SRcvrCtrlCmdV1::EDetMode>();
	rcvr.agcTime = jsonObj["rcvr"]["agcTime"].is_null() == true ? NULL : jsonObj["rcvr"]["agcTime"].get<unsigned long>();
	if (jsonObj["rcvr"]["bfo"].is_null() == false) {
		rcvr.bfo = Units::Frequency(jsonObj["rcvr"]["bfo"].get<unsigned long>()).GetRaw();
	}
	
	structSO.rcvr = rcvr;
	return structSO;
}

SMeasReqData* jsonToSMeasReqData(nlohmann::json jsonObj) {
	SMeasReqData structSO;
	
	if (jsonObj["freq"].is_null() == false) {
		structSO.freq = Units::Frequency(jsonObj["freq"].get<unsigned long>()).GetRaw();
	}
	if (jsonObj["bandwidth"].is_null() == false) {
		structSO.bandwidth = Units::Frequency(jsonObj["bandwidth"].get<unsigned long>()).GetRaw();
	}
	structSO.ant = jsonObj["ant"].is_null() == true ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["ant"].get<SEquipCtrlMsg::EAnt>();

	SSmsMsg::SGetBwCmd bwCmd;
	bwCmd.dwellTime = jsonObj["bwCmd"]["dwellTime"].is_null() == true ? NULL : jsonObj["bwCmd"]["dwellTime"].get<unsigned long>();
	bwCmd.betaParam = jsonObj["bwCmd"]["betaParam"].is_null() == true ? NULL : jsonObj["bwCmd"]["betaParam"].get<unsigned long>();
	bwCmd.yParam = jsonObj["bwCmd"]["yParam"].is_null() == true ? NULL : jsonObj["bwCmd"]["yParam"].get<unsigned long>();
	bwCmd.x1Param = jsonObj["bwCmd"]["x1Param"].is_null() == true ? NULL : jsonObj["bwCmd"]["x1Param"].get<unsigned long>();
	bwCmd.x2Param = jsonObj["bwCmd"]["x2Param"].is_null() == true ? NULL : jsonObj["bwCmd"]["x2Param"].get<unsigned long>();
	bwCmd.repeatCount = jsonObj["bwCmd"]["repeatCount"].is_null() == true ? NULL : jsonObj["bwCmd"]["repeatCount"].get<unsigned long>();
	bwCmd.aveMethod = jsonObj["bwCmd"]["aveMethod"].is_null() == true ? (SSmsMsg::EAveMethod)NULL : jsonObj["bwCmd"]["aveMethod"].get<SSmsMsg::EAveMethod>();
	bwCmd.outputType = jsonObj["bwCmd"]["outputType"].is_null() == true ? (SSmsMsg::EOutputType)NULL : jsonObj["bwCmd"]["outputType"].get<SSmsMsg::EOutputType>();
	
	structSO.bwCmd = bwCmd;
	
	SSmsMsg::SGetMeasCmdV5::SGetDfCmd dfCmd;
	dfCmd.confThreshold = jsonObj["dfCmd"]["confThreshold"].is_null() == true ? NULL : jsonObj["dfCmd"]["confThreshold"].get<unsigned char>();
	dfCmd.dwellTime = jsonObj["dfCmd"]["dwellTime"].is_null() == true ? NULL : jsonObj["dfCmd"]["dwellTime"].get<unsigned long>();
	dfCmd.repeatCount = jsonObj["dfCmd"]["repeatCount"].is_null() == true ? NULL : jsonObj["dfCmd"]["repeatCount"].get<unsigned long>();
	if (jsonObj["dfCmd"]["dfBandwidth"].is_null() == false) {
		dfCmd.dfBandwidth = Units::Frequency(jsonObj["dfCmd"]["dfBandwidth"].get<unsigned long>()).GetRaw();
	}
	dfCmd.outputType = jsonObj["dfCmd"]["outputType"].is_null() == true ? (SSmsMsg::SGetMeasCmdV5::SGetDfCmd::EOutputType)NULL : jsonObj["dfCmd"]["outputType"].get<SSmsMsg::SGetMeasCmdV5::SGetDfCmd::EOutputType>();
	dfCmd.srcOfRequest = jsonObj["dfCmd"]["srcOfRequest"].is_null() == true ? (SSmsMsg::SGetMeasCmdV5::SGetDfCmd::ESource)NULL : jsonObj["dfCmd"]["srcOfRequest"].get<SSmsMsg::SGetMeasCmdV5::SGetDfCmd::ESource>();

	structSO.dfCmd = dfCmd;

	SSmsMsg::SGetFieldStrengthCmd fieldStrengthCmd;
	fieldStrengthCmd.fieldMethod = jsonObj["fieldStrengthCmd"]["fieldMethod"].is_null() == true ? (SSmsMsg::EFieldMethod)NULL : jsonObj["fieldStrengthCmd"]["fieldMethod"].get<SSmsMsg::EFieldMethod>();
	fieldStrengthCmd.dwellTime = jsonObj["fieldStrengthCmd"]["dwellTime"].is_null() == true ? NULL : jsonObj["fieldStrengthCmd"]["dwellTime"].get<unsigned long>();
	fieldStrengthCmd.repeatCount = jsonObj["fieldStrengthCmd"]["repeatCount"].is_null() == true ? NULL : jsonObj["fieldStrengthCmd"]["repeatCount"].get<unsigned long>();
	fieldStrengthCmd.aveMethod = jsonObj["fieldStrengthCmd"]["aveMethod"].is_null() == true ? (SSmsMsg::EAveMethod)NULL : jsonObj["fieldStrengthCmd"]["aveMethod"].get<SSmsMsg::EAveMethod>();
	fieldStrengthCmd.outputType = jsonObj["fieldStrengthCmd"]["outputType"].is_null() == true ? (SSmsMsg::EOutputType)NULL : jsonObj["fieldStrengthCmd"]["outputType"].get<SSmsMsg::EOutputType>();

	structSO.fieldStrengthCmd = fieldStrengthCmd;
		
	SSmsMsg::SGetFreqCmd freqCmd;
	freqCmd.freqMethod = jsonObj["freqCmd"]["freqMethod"].is_null() == true ? (SSmsMsg::SGetFreqCmd::EFreqMethod)NULL : jsonObj["freqCmd"]["freqMethod"].get<SSmsMsg::SGetFreqCmd::EFreqMethod>();
	freqCmd.dwellTime = jsonObj["freqCmd"]["dwellTime"].is_null() == true ? NULL : jsonObj["freqCmd"]["dwellTime"].get<unsigned long>();
	freqCmd.repeatCount = jsonObj["freqCmd"]["repeatCount"].is_null() == true ? NULL : jsonObj["freqCmd"]["repeatCount"].get<unsigned long>();
	freqCmd.aveMethod = jsonObj["freqCmd"]["aveMethod"].is_null() == true ? (SSmsMsg::EAveMethod)NULL : jsonObj["freqCmd"]["aveMethod"].get<SSmsMsg::EAveMethod>();
	freqCmd.outputType = jsonObj["freqCmd"]["outputType"].is_null() == true ? (SSmsMsg::EOutputType)NULL : jsonObj["freqCmd"]["outputType"].get<SSmsMsg::EOutputType>();
	
	structSO.freqCmd = freqCmd;

	SSmsMsg::SGetMeasCmdV5::SGetIQCmd iqCmd;
	iqCmd.bwFactor = jsonObj["iqCmd"]["bwFactor"].is_null() == true ? NULL : jsonObj["iqCmd"]["bwFactor"].get<float>();
	iqCmd.numSamples = jsonObj["iqCmd"]["numSamples"].is_null() == true ? NULL : jsonObj["iqCmd"]["numSamples"].get<unsigned long>();
	iqCmd.outputType = jsonObj["iqCmd"]["outputType"].is_null() == true ? (SSmsMsg::EOutputType)NULL : jsonObj["iqCmd"]["outputType"].get<SSmsMsg::EOutputType>();
	iqCmd.startTime = jsonObj["iqCmd"]["startTime"].is_null() == true ? NULL : jsonObj["iqCmd"]["startTime"].get<DATE>();
	iqCmd.tdoa = jsonObj["iqCmd"]["tdoa"].is_null() == true ? NULL : jsonObj["iqCmd"]["tdoa"].get<bool>();

	structSO.iqCmd = iqCmd;

	SSmsMsg::SGetModulationCmd modulationCmd;
	modulationCmd.dwellTime = jsonObj["modulationCmd"]["dwellTime"].is_null() == true ? NULL : jsonObj["modulationCmd"]["dwellTime"].get<unsigned long>();
	modulationCmd.repeatCount = jsonObj["modulationCmd"]["repeatCount"].is_null() == true ? NULL : jsonObj["modulationCmd"]["repeatCount"].get<unsigned long>();
	modulationCmd.aveMethod = jsonObj["modulationCmd"]["aveMethod"].is_null() == true ? (SSmsMsg::EAveMethod)NULL : jsonObj["modulationCmd"]["aveMethod"].get<SSmsMsg::EAveMethod>();
	modulationCmd.outputType = jsonObj["modulationCmd"]["outputType"].is_null() == true ? (SSmsMsg::EOutputType)NULL : jsonObj["modulationCmd"]["outputType"].get<SSmsMsg::EOutputType>();

	structSO.modulationCmd = modulationCmd;
		
	SMeasReqData* m_measureReqMsg = (SMeasReqData*)malloc(sizeof(SMeasReqData));
	memcpy(m_measureReqMsg, &structSO, sizeof(SMeasReqData));
	return m_measureReqMsg;
}

SOccupReqData* jsonToSOccupReqData(nlohmann::json jsonObj) {
	SOccupReqData structSO;
	structSO.ant = jsonObj["ant"].is_null() == true ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["ant"].get<SEquipCtrlMsg::EAnt>();
	
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

	structSO.band[0] = band;
	structSO.confidenceLevel = jsonObj["confidenceLevel"].is_null() == true ? NULL : jsonObj["confidenceLevel"].get<unsigned char>();
	structSO.desiredAccuracy = jsonObj["desiredAccuracy"].is_null() == true ? NULL : jsonObj["desiredAccuracy"].get<unsigned char>();
	structSO.durationMethod = jsonObj["durationMethod"].is_null() == true ? (SEquipCtrlMsg::EDurationMethod)NULL : jsonObj["durationMethod"].get<SEquipCtrlMsg::EDurationMethod>();
	structSO.measurementTime = jsonObj["measurementTime"].is_null() == true ? NULL : jsonObj["measurementTime"].get<unsigned long>();
	structSO.numBands = jsonObj["numBands"].is_null() == true ? NULL : jsonObj["numBands"].get<unsigned long>();
	structSO.occPrimaryThreshold[0] = jsonObj["occPrimaryThreshold"][0].is_null() == true ? NULL : jsonObj["occPrimaryThreshold"][0].get<short>();
	structSO.occupancyMinGap = jsonObj["occupancyMinGap"].is_null() == true ? NULL : jsonObj["occupancyMinGap"].get<unsigned long>();
	structSO.storageTime = jsonObj["storageTime"].is_null() == true ? NULL : jsonObj["storageTime"].get<unsigned long>();
	structSO.thresholdMethod = jsonObj["thresholdMethod"].is_null() == true ? (SEquipCtrlMsg::EThresholdMethod)NULL : jsonObj["thresholdMethod"].get<SEquipCtrlMsg::EThresholdMethod>();
	
	auto occupbodySize = offsetof(SOccupReqData, band) + structSO.numBands * sizeof(SEquipCtrlMsg::SBandV4);
	SOccupReqData* m_occReqMsg = (SOccupReqData*)malloc(occupbodySize);
	memcpy(m_occReqMsg, &structSO, occupbodySize);
	
	return m_occReqMsg;
}

SOccDFReqData* jsonToSOccDFReqData(nlohmann::json jsonObj) {
	SOccDFReqData structSO;
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

	structSO.band[0] = band;
	
	structSO.storageTime = jsonObj["storageTime"].is_null() == true ? NULL : jsonObj["storageTime"].get<unsigned long>();
	structSO.measurementTime = jsonObj["measurementTime"].is_null() == true ? NULL : jsonObj["measurementTime"].get<unsigned long>();
	structSO.numAzimuths = jsonObj["numAzimuths"].is_null() == true ? NULL :jsonObj["numAzimuths"].get<unsigned long>();
	structSO.confidence = jsonObj["confidence"].is_null() == true ? NULL : jsonObj["confidence"].get<unsigned long>();
	structSO.recordHoldoff = jsonObj["recordHoldoff"].is_null() == true ? NULL : jsonObj["recordHoldoff"].get<unsigned long>();
	structSO.scanDfThreshold = jsonObj["scanDfThreshold"].is_null() == true ? NULL : jsonObj["scanDfThreshold"].get<unsigned char>();
	structSO.recordAudioDf = jsonObj["recordAudioDf"].is_null() == true ? NULL : jsonObj["recordAudioDf"].get<bool>();
	structSO.numBands = jsonObj["numBands"].is_null() == true ? NULL : jsonObj["numBands"].get<unsigned short>();

	SEquipCtrlMsg::SRcvrCtrlCmd rcvrCtrl;
	rcvrCtrl.agcTime = jsonObj["rcvrCtrl"]["agcTime"].is_null() == true ? NULL : jsonObj["rcvrCtrl"]["agcTime"].get<unsigned long>();
	if (jsonObj["rcvrCtrl"]["bandwidth"].is_null() == false) {
		rcvrCtrl.bandwidth = Units::Frequency(jsonObj["rcvrCtrl"]["bandwidth"].get<unsigned long>()).GetRaw();
	}
	if (jsonObj["rcvrCtrl"]["bfo"].is_null() == false) {
		rcvrCtrl.bfo = Units::Frequency(jsonObj["rcvrCtrl"]["bfo"].get<unsigned long>()).GetRaw();
	}
	rcvrCtrl.detMode = jsonObj["rcvrCtrl"]["detMode"].is_null() == true ? (SSmsMsg::SRcvrCtrlCmdV1::EDetMode)NULL : jsonObj["rcvrCtrl"]["detMode"].get<SSmsMsg::SRcvrCtrlCmdV1::EDetMode>();
	if (jsonObj["rcvrCtrl"]["freq"].is_null() == false) {
		rcvrCtrl.freq = Units::Frequency(jsonObj["rcvrCtrl"]["freq"].get<unsigned long>()).GetRaw();
	}

	structSO.rcvrCtrl = rcvrCtrl;

	auto occupDFbodySize = offsetof(SOccDFReqData, band) + jsonObj["numBands"].get<unsigned short>() * sizeof(SSmsMsg::SGetScanDfCmdV1::SBand);
	SOccDFReqData* occDFReqMsg;
	occDFReqMsg = (SOccDFReqData*)malloc(occupDFbodySize);
	if (occDFReqMsg != nullptr)
	{
		memcpy(occDFReqMsg, &structSO, occupDFbodySize);
	}

	return occDFReqMsg;
}

SAVDReqData* jsonToSAVDReqData(nlohmann::json jsonObj) {
	SAVDReqData structSO;
	
	structSO.ant = jsonObj["ant"].is_null() == true ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["ant"].get<SEquipCtrlMsg::EAnt>();
	structSO.avdThreshold = jsonObj["avdThreshold"].is_null() == true ? NULL : jsonObj["avdThreshold"].get<unsigned char>();

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

	structSO.band[0] = band;

	structSO.measurementRate = jsonObj["measurementRate"].is_null() == true ? (SEquipCtrlMsg::EAvdRate)NULL : jsonObj["measurementRate"].get<SEquipCtrlMsg::EAvdRate>();
	structSO.measurementTime = jsonObj["measurementTime"].is_null() == true ? NULL : jsonObj["measurementTime"].get<unsigned long>();
	structSO.numBands = jsonObj["numBands"].is_null() == true ? NULL : jsonObj["numBands"].get<unsigned short>();
	structSO.storageTime = jsonObj["storageTime"].is_null() == true ? NULL : jsonObj["storageTime"].get<unsigned long>();

	auto avdbodySize = offsetof(SAVDReqData, band) + structSO.numBands * sizeof(SEquipCtrlMsg::SBandV4);
	SAVDReqData* avdReqMsg;
	avdReqMsg = (SAVDReqData*)malloc(avdbodySize);
	memcpy(avdReqMsg, &structSO, avdbodySize);

	return avdReqMsg;
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

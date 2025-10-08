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

// Include provided DLL libraries
#include "StdAfx.h"
#include "ScorpioAPITypes.h"
#include "ScorpioAPIDll.h"

// Include DLL specific libraries
#include "etherDLLCodes.hpp"
#include "etherDLLInit.hpp"
#include "EtherDLLValidation.hpp"

// Include core EtherDLL libraries
#include "EtherDLLClient.hpp"
#include "EtherDLLConfig.hpp"
#include "EtherDLLUtils.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <string>

// For convenience
using json = nlohmann::json;
using TaskKeys = edll::DefaultConfig::Service::TaskKeys;

// Global variables
extern spdlog::logger* loggerPtr;


// ----------------------------------------------------------------------
/**
  * @brief Convert JSON object in SAudioParams struct
  *
  * @param jsonObj: JSON object containing the parameters
  * @return SAudioParams: structure populated with values from the JSON object
  * @throws NO EXCEPTION HANDLING
 **/
SAudioParams jsonToSAudioParams(nlohmann::json jsonObj) {
	SAudioParams structSO{};

	structSO.anyChannel = jsonObj["anyChannel"].is_null() == true ? NULL : jsonObj["anyChannel"].get<bool>();
	if (jsonObj["bandwidth"].is_null() == false) {
		structSO.bandwidth = Units::Frequency(jsonObj["bandwidth"].get<unsigned long>()).GetRaw();
	}
	// TODO: handle null cases and throw error if necessary
	if (jsonObj["bfo"].is_null() == false) {
		structSO.bfo = Units::Frequency(jsonObj["bfo"].get<unsigned long>()).GetRaw();
	}
	structSO.channel = jsonObj["channel"].is_null() == true ? NULL : jsonObj["channel"].get<unsigned long>();
	structSO.detMode = jsonObj["detMode"].is_null() == true ? (SSmsMsg::SRcvrCtrlCmdV1::EDetMode)NULL : jsonObj["detMode"].get<SSmsMsg::SRcvrCtrlCmdV1::EDetMode>();
	structSO.doModRec = jsonObj["doModRec"].is_null() == true ? NULL : jsonObj["doModRec"].get<bool>();;
	structSO.doRDS = jsonObj["doRDS"].is_null() == true ? NULL : jsonObj["doRDS"].get<bool>();
	if (jsonObj["freq"].is_null() == false) {
		structSO.freq = Units::Frequency(jsonObj["freq"].get<unsigned long>()).GetRaw();
	}

	structSO.streamID = jsonObj["streamID"].is_null() == true ? NULL : jsonObj["streamID"].get<unsigned long>();
	strcpy_s(structSO.ipAddressRDSRadio, "");
	return structSO;
}

// ----------------------------------------------------------------------
/**
* @brief Convert JSON object in SGetPanParams struct
* 
* @param jsonObj: JSON object containing the parameters
* @return SGetPanParams structure populated with values from the JSON object
* @throws NO EXCEPTION HANDLING
**/
SGetPanParams jsonToSGetPanParams(nlohmann::json jsonObj) {
	SGetPanParams structSO{};

	unsigned long bandwidth = jsonObj["bandwidth"].get<unsigned long>();
	//loggerPtr->debug("Bandwidth (Hz): {}", bandwidth);
	structSO.bandwidth = Units::Frequency(bandwidth).GetRaw();

	unsigned long freq = jsonObj["freq"].get<unsigned long>();
	//loggerPtr->debug("Frequency (Hz): {}", freq);
	structSO.freq = Units::Frequency(freq).GetRaw();

	unsigned char rcvrAtten = jsonObj["rcvrAtten"].get<unsigned char>();
	//loggerPtr->debug("Receiver Attenuation (dB): {}", (unsigned int)rcvrAtten);
	structSO.rcvrAtten = rcvrAtten;
	
	return structSO;
}

// ----------------------------------------------------------------------
/**
* @brief Convert JSON object in SPanParams struct
* 
* @param jsonObj: JSON object containing the parameters
* @return SPanParams structure populated with values from the JSON object
* @throws NO EXCEPTION HANDLING
**/
SPanParams jsonToSPanParams(nlohmann::json jsonObj) {
	SPanParams structSO{};

	structSO.antenna = jsonObj["antenna"].get<SEquipCtrlMsg::EAnt>();
	
	SEquipCtrlMsg::SRcvrCtrlCmd rcvr;
	rcvr.freq = Units::Frequency(jsonObj["rcvr"]["freq"].get<unsigned long>()).GetRaw();
	rcvr.bandwidth = Units::Frequency(jsonObj["rcvr"]["bandwidth"].get<unsigned long>()).GetRaw();
	rcvr.bfo = Units::Frequency(jsonObj["rcvr"]["bfo"].get<unsigned long>()).GetRaw();

	rcvr.detMode = jsonObj["rcvr"]["detMode"].is_null() == true ? (SSmsMsg::SRcvrCtrlCmdV1::EDetMode)NULL : jsonObj["rcvr"]["detMode"].get<SSmsMsg::SRcvrCtrlCmdV1::EDetMode>();
	rcvr.agcTime = jsonObj["rcvr"]["agcTime"].is_null() == true ? NULL : jsonObj["rcvr"]["agcTime"].get<unsigned long>();
	
	structSO.rcvr = rcvr;
	return structSO;
}

// ----------------------------------------------------------------------
/**
* @brief Convert JSON object in SMeasReqData struct
* 
* @param jsonObj: JSON object containing the parameters
* @return SMeasReqData structure populated with values from the JSON object
* @throws NO EXCEPTION HANDLING
**/
SMeasReqData* jsonToSMeasReqData(nlohmann::json jsonObj) {
	SMeasReqData structSO{};
	
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


// ----------------------------------------------------------------------
/**
* @brief Convert JSON object in SOccupReqData struct
* 
* @param jsonObj: JSON object containing the parameters
* @return SOccupReqData structure populated with values from the JSON object
* @throws NO EXCEPTION HANDLING
**/
SOccupReqData* jsonToSOccupReqData(nlohmann::json jsonObj) {
	SOccupReqData structSO;
	SSmsMsg::SBandV4 band;


	// If 'band' is an array, set the number of bands accordingly
	// otherwise set it to 1 and convert it to an array
	if (jsonObj["band"].is_array() == true) {
		structSO.numBands = jsonObj["band"].size();
	} else {
		structSO.numBands = 1; 
		json singleBand = jsonObj["band"];
		jsonObj["band"] = json::array({ singleBand });
	}

	// create a vector of band objects and fill it with data from the JSON object
	

	// TODO: Handle multiple bands, using vector and dynamic memory allocation
	// TODO: Handle default values from config, replacing NULL
	// TODO: Reorder the field handling to match the struct order


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

	band.sType.signalType.horizPol = jsonObj["band"]["signalType"]["horizPol"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["horizPol"].get<unsigned long>();
	band.sType.signalType.narrow = jsonObj["band"]["signalType"]["narrow"].is_null() == true ? NULL : jsonObj["band"]["sType"]["signalType"]["narrow"].get<unsigned long>();
	band.sType.signalType.unused0 = 0;
	band.sType.signalType.unused1 = 0;

	structSO.band[0] = band;

	structSO.ant = jsonObj["ant"].is_null() ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["ant"].get<SEquipCtrlMsg::EAnt>();
	structSO.confidenceLevel = jsonObj["confidenceLevel"].is_null() ? NULL : jsonObj["confidenceLevel"].get<unsigned char>();
	structSO.desiredAccuracy = jsonObj["desiredAccuracy"].is_null() ? NULL : jsonObj["desiredAccuracy"].get<unsigned char>();
	structSO.durationMethod = jsonObj["durationMethod"].is_null() ? (SEquipCtrlMsg::EDurationMethod)NULL : jsonObj["durationMethod"].get<SEquipCtrlMsg::EDurationMethod>();
	structSO.measurementTime = jsonObj["measurementTime"].is_null() ? NULL : jsonObj["measurementTime"].get<unsigned long>();
	structSO.occPrimaryThreshold[0] = jsonObj["occPrimaryThreshold"][0].is_null() ? NULL : jsonObj["occPrimaryThreshold"][0].get<short>();
	structSO.occupancyMinGap = jsonObj["occupancyMinGap"].is_null() ? NULL : jsonObj["occupancyMinGap"].get<unsigned long>();
	structSO.storageTime = jsonObj["storageTime"].is_null() ? NULL : jsonObj["storageTime"].get<unsigned long>();
	structSO.thresholdMethod = jsonObj["thresholdMethod"].is_null() ? (SEquipCtrlMsg::EThresholdMethod)NULL : jsonObj["thresholdMethod"].get<SEquipCtrlMsg::EThresholdMethod>();
	
	auto occupbodySize = offsetof(SOccupReqData, band) + structSO.numBands * sizeof(SEquipCtrlMsg::SBandV4);
	SOccupReqData* m_occReqMsg = (SOccupReqData*)malloc(occupbodySize);
	memcpy(m_occReqMsg, &structSO, occupbodySize);
	
	return m_occReqMsg;
}

// ----------------------------------------------------------------------
/**
* @brief Convert JSON object in SOccDFReqData struct
* 
* @param jsonObj: JSON object containing the parameters
* @return SOccDFReqData structure populated with values from the JSON object
* @throws NO EXCEPTION HANDLING
*/
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

// ----------------------------------------------------------------------
/**
* @brief Convert JSON object in SAVDReqData struct
* 
* @param jsonObj: JSON object containing the parameters
* @return SAVDReqData structure populated with values from the JSON object
* @throws NO EXCEPTION HANDLING
*/
SAVDReqData* jsonToSAVDReqData(nlohmann::json jsonObj) {
	SAVDReqData structSO{};
	
	structSO.ant = jsonObj["ant"].is_null() == true ? (SEquipCtrlMsg::EAnt)NULL : jsonObj["ant"].get<SEquipCtrlMsg::EAnt>();
	structSO.avdThreshold = jsonObj["avdThreshold"].is_null() == true ? NULL : jsonObj["avdThreshold"].get<unsigned char>();

	SSmsMsg::SBandV4 band{};

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

// ----------------------------------------------------------------------
/**
 * @brief Call the appropriate DLL function based on the request in JSON format
 *
 * Include the identification of the funciton based on request type
 * conversion from JSON to the appropriate struct for each function call.
 * 
 * @param DLLConnID: Connection ID obtained from the DLL during initialization
 * @param request: JSON object containing the parameters
 * @param msgType: Message type to be validated (see ECSMSDllMsgType enum)
 * @return void
 * @throws NO EXCEPTION HANDLING
**/
void DLLFunctionCall(DLLConnectionData DLLConnID, json request, unsigned long msgType)
{
	ERetCode errCode = ERetCode::API_SUCCESS;

	loggerPtr->debug("Processing request: " + request.dump());
	
	unsigned long requestID = request.value(TaskKeys::QueueId::VALUE, TaskKeys::QueueId::INIT_VALUE);

	json reqArguments = request.value(TaskKeys::Arguments::VALUE, json::object());

	switch (msgType) {
		case ECSMSDllMsgType::GET_OCCUPANCY:
		{
			SOccupReqData* occupDFReqMsg = jsonToSOccupReqData(reqArguments);
			errCode = RequestOccupancy(DLLConnID, occupDFReqMsg, &requestID);
			break;
		}
		case ECSMSDllMsgType::GET_OCCUPANCYDF:
		{
			SOccDFReqData* occDFReqMsg = jsonToSOccDFReqData(reqArguments);
			errCode = RequestOccupancyDF(DLLConnID, occDFReqMsg, &requestID);
			break;
		}
		case ECSMSDllMsgType::GET_AVD:
		{
			SAVDReqData* avdReqMsg = jsonToSAVDReqData(reqArguments);
			errCode = RequestAVD(DLLConnID, avdReqMsg, &requestID);
			break;
		}
		case ECSMSDllMsgType::GET_MEAS:
		{
			SMeasReqData* m_measureReqMsg = jsonToSMeasReqData(reqArguments);
			errCode = RequestMeasurement(DLLConnID, m_measureReqMsg, &requestID);
			break;
		}
		case ECSMSDllMsgType::GET_TASK_STATUS:
		{
			errCode = RequestTaskStatus(DLLConnID, requestID);
			break;
		}
		case ECSMSDllMsgType::GET_TASK_STATE:
		{
			errCode = RequestTaskState(DLLConnID, (ECSMSDllMsgType)reqArguments, requestID);
			break;
		}
		case ECSMSDllMsgType::TASK_SUSPEND:
		{
			errCode = SuspendTask(DLLConnID, (ECSMSDllMsgType)reqArguments, requestID);
			break;
		}
		case ECSMSDllMsgType::TASK_RESUME:
		{
			errCode = ResumeTask(DLLConnID, (ECSMSDllMsgType)reqArguments, requestID);
			break;
		}
		case ECSMSDllMsgType::TASK_TERMINATE:
		{
			errCode = TerminateTask(DLLConnID, requestID);
			break;
		}
		case ECSMSDllMsgType::GET_BIST:
		{
			errCode = RequestBist(DLLConnID, (EBistScope)reqArguments, &requestID);
			break;
		}
		case ECSMSDllMsgType::SET_AUDIO_PARAMS:
		{
			SAudioParams audioParams = jsonToSAudioParams(reqArguments);
			errCode = SetAudio(DLLConnID, audioParams, &requestID);

			/* Todo: Start audio streaming service and send link to client
			if (errCode == ERetCode::API_SUCCESS) {
				DWORD processId = wcstoul(L"123", nullptr, 0);
				HRESULT hr = loopbackCapture.StartCaptureAsync(processId, false, L"audio");
				if (FAILED(hr))
				{
					logger_ptr->error("Failed to start audio capture");
				}
				else {
					logger_ptr->info("Capturing audio.");
				}
			}
			*/
			break;
		}
		case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
		{
			errCode = FreeAudio(DLLConnID, reqArguments, &requestID);
			loggerPtr->info("Finished audio capture.");
			break;
		}
		case ECSMSDllMsgType::SET_PAN_PARAMS:
		{
			SPanParams panParams = jsonToSPanParams(reqArguments);
			errCode = SetPanParams(DLLConnID, panParams, &requestID);
			break;
		}
		case ECSMSDllMsgType::GET_PAN:
		{
			SGetPanParams panParamsGet = jsonToSGetPanParams(reqArguments);
			errCode = RequestPan(DLLConnID, panParamsGet, &requestID);
			break;
		}
		default:
		{
			loggerPtr->error("Unknown message type");
			errCode = ERetCode::CMD_SENT_ERROR;
			break;
		}
	}

	std::string reqName = request.value(TaskKeys::CommandName::VALUE, TaskKeys::CommandName::INIT_VALUE);
	if (errCode != ERetCode::API_SUCCESS)
	{
		loggerPtr->error("[" + reqName + "] ERROR. " + ERetCodeToString(errCode));
	}
	else
	{
		loggerPtr->info("[" + reqName + "] command executed");
		loggerPtr->debug("Request ID " + std::to_string(requestID) + ": " + reqArguments.dump() + "");
	}
}


// ----------------------------------------------------------------------
/** @brief Process messages from the request queue and call the appropriate DLL function
 *
 * This function will lock the thread. Must be run in a separate thread.
 * Messages are expected to be in JSON format and end with the defined message end sequence.
 * If no data is available to send, a PING message will be sent periodically to check if the connection is still alive.
 * PING message will contain the current timestamp in milliseconds since epoch.
 *
 * @param clientSocket: Socket connected to the client
 * @param config: JSON object containing configuration
 * @param response: Thread-safe message queue containing messages to be sent to the client
 * @param interruptionCode: Signal interruption for service interruption
 * @param logger: spdlog logger object for logging messages
 * @throws NO EXCEPTION HANDLING
*/
void processRequestQueue(DLLConnectionData DLLConnID, MessageQueue& request, MessageQueue& response, edll::INT_CODE& interruptionCode)
{
	const std::string funcName = "processRequestQueue";

	while (interruptionCode == edll::Code::RUNNING)
	{
		json oneRequest = request.waitAndPop(interruptionCode, funcName);

		unsigned long cmd = oneRequest.value(TaskKeys::CommandCode::VALUE, TaskKeys::CommandCode::INIT_VALUE);

		if (!validRequest(oneRequest, cmd, response)) {
			continue;
		}

		DLLFunctionCall(DLLConnID, oneRequest, cmd);
	}
}

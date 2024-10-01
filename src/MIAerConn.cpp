// MIAerConn.cpp : Defines the entry point for the application.
//

// Include the ScorpioAPI libraries 3h2cl5vTu8cg
#include <StdAfx.h>
#include <ScorpioAPIDll.h> 

// Include the standard C++ headers
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <csignal>
#include <chrono>
#include <fstream>

// Include the nlohmann JSON library
#include <nlohmann/json.hpp>

// Include to solution specific libraries
#include <ExternalCodes.h>
#include <MIAerConnCodes.hpp>
#include <MIAerConnUtils.h>
#include "MIAerLog.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

/*
* Felipe Machado - 29/08/2024
* In debug and release the name of ScorpioAPI is different, so checking is necessary to know which one to call

*/
#ifdef _X86_
	#pragma comment (lib, "ScorpioAPIDll.lib")
#else
	#ifdef NDEBUG
		#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE
	#else
		#pragma comment (lib, "ScorpioAPIDlld.lib") //DEBUG
	#endif
#endif

#pragma comment (lib, "AdvApi32.lib")

// For convenience
using json = nlohmann::json;

//
// Global variables related to the application
//

// JSON configuration object
json config;

// Atomic flag to signal application error
std::atomic<bool> running{ true }; 

// Code to represent the cause for not running
std::atomic<int> interruptionCode{ mcs::Code::RUNNING };

// Vector used for the command queue
std::vector<std::string> commandQueue;

// Command queue counter
std::atomic<unsigned long> commandQueueCounter{ 0 };

// Vector used for the data stream output
std::vector<std::string> streamBuffer;

// Vector used for the data error output
std::vector<std::string> errorBuffer;

// Vector used for the real time output
std::vector<std::string> realtimeBuffer;

// Mutex to protect queue
std::mutex MCCommandMutex;
std::mutex MCstreamMutex;
std::mutex MCStationMutex;
//
// Global variables related to the API
//

// API server ID. This service is intended to be used to connect to a single station, always 0.
unsigned long APIserverId = 0;

// Station connection parameters
SScorpioAPIClient station;

// Station capabilities
SCapabilities StationCapabilities;

//Socket connection for commands
SOCKET clientSocketCommand = NULL;

MIAerLog logMIAer;

//TODO refactor
void processBITEResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* respdata)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);
	if (respType == ECSMSDllMsgType::GET_BIST ||
		respType == ECSMSDllMsgType::GET_BIST_RESULT)
	{
		SEquipCtrlMsg::SGetBistResp* BITEResponse = (SEquipCtrlMsg::SGetBistResp*)respdata;
		jsonObj["SGetBistResp"]["last"] = BITEResponse->last;
		jsonObj["SGetBistResp"]["result"] = BITEResponse->result;
		jsonObj["SGetBistResp"]["text"] = BITEResponse->text;
		jsonObj["SGetBistResp"]["textLen"] = BITEResponse->textLen;
	}
	else if (respType == ECSMSDllMsgType::GET_DIAGNOSTICS)
	{
		SEquipCtrlMsg::SGetBistResp* BITEResponse = (SEquipCtrlMsg::SGetBistResp*)respdata;
		jsonObj["SGetBistResp"]["last"] = BITEResponse->last;
		jsonObj["SGetBistResp"]["result"] = BITEResponse->result;
		jsonObj["SGetBistResp"]["text"] = BITEResponse->text;
		jsonObj["SGetBistResp"]["textLen"] = BITEResponse->textLen;
	}
	else
	{
	}
	streamBuffer.push_back(jsonObj.dump());

}
void ProcessAntListResponse(_In_ SEquipCtrlMsg::UBody* data)
{
	SEquipCtrlMsg::SAntInfoListResp* antListResponse = (SEquipCtrlMsg::SAntInfoListResp*)data;
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	jsonObj["SAntInfoListResp"]["ant"]["ant"] = antListResponse->ant->ant;
	jsonObj["SAntInfoListResp"]["ant"]["antName"] = antListResponse->ant->antName;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthStart"] = antListResponse->ant->azimuthStart;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthStep"] = antListResponse->ant->azimuthStep;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthStop"] = antListResponse->ant->azimuthStop;
	jsonObj["SAntInfoListResp"]["ant"]["azimuthTolerance"] = antListResponse->ant->azimuthTolerance;
	jsonObj["SAntInfoListResp"]["ant"]["aziOrientation"] = antListResponse->ant->aziOrientation;
	jsonObj["SAntInfoListResp"]["ant"]["elevationStart"] = antListResponse->ant->elevationStart;
	jsonObj["SAntInfoListResp"]["ant"]["elevationStep"] = antListResponse->ant->elevationStep;
	jsonObj["SAntInfoListResp"]["ant"]["elevationStop"] = antListResponse->ant->elevationStop;
	jsonObj["SAntInfoListResp"]["ant"]["elevationTolerance"] = antListResponse->ant->elevationTolerance;
	jsonObj["SAntInfoListResp"]["ant"]["highFreq"]["internal"] = antListResponse->ant->highFreq.internal;
	jsonObj["SAntInfoListResp"]["ant"]["lowFreq"]["internal"] = antListResponse->ant->lowFreq.internal;
	jsonObj["SAntInfoListResp"]["ant"]["polarization"] = antListResponse->ant->polarization;
	jsonObj["SAntInfoListResp"]["ant"]["rotatorId"] = antListResponse->ant->rotatorId;
	jsonObj["SAntInfoListResp"]["numAnt"] = antListResponse->numAnt;

	streamBuffer.push_back(jsonObj.dump());
}
void processAutoViolateResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	CString tempstr;
	SEquipCtrlMsg::SOccupancyHeader* pOccHdr = nullptr;
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	if (respType == ECSMSDllMsgType::AVD_STATE_RESPONSE ||
		respType == ECSMSDllMsgType::AVD_SOLICIT_STATE_RESPONSE)
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		jsonObj["SStateResp"]["completionTime"] = Response->completionTime;
		jsonObj["SStateResp"]["state"] = Response->state;
	}
	else if (respType == ECSMSDllMsgType::AVD_FREQ_VS_CHANNEL)
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
		jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = Response->frequencies->internal;
		jsonObj["SFrequencyVsChannelResp"]["hostName"] = Response->hostName;
		jsonObj["SFrequencyVsChannelResp"]["numBands"] = Response->numBands;
		jsonObj["SFrequencyVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = Response->occPrimaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = Response->occSecondaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = Response->saveIntermediateData;
		jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = Response->selectedAntenna;
		jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = Response->useSecondaryThreshold;
	}
	else if (respType == ECSMSDllMsgType::AVD_FREQ_MEAS)
	{
		SEquipCtrlMsg::SAvdMeasureResult* Response = (SEquipCtrlMsg::SAvdMeasureResult*)data;
		SSmsMsg::SAvdMeasureResultV1::SMeasureData arrayMeasData[1000];
		// TODO - montar array de measData
		//jsonObj["SAvdMeasureResult"]["measData"] = Response->measData;
		jsonObj["SAvdMeasureResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["status"] = Response->occHdr.status;
	}
	else if (respType == ECSMSDllMsgType::AVD_BW_MEAS)
	{
		SEquipCtrlMsg::SAvdMeasureResult* Response = (SEquipCtrlMsg::SAvdMeasureResult*)data;
		// TODO - montar array de measData
		//jsonObj["SAvdMeasureResult"]["measData"] = Response->measData;
		jsonObj["SAvdMeasureResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SAvdMeasureResult"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SAvdMeasureResult"]["occHdr"]["status"] = Response->occHdr.status;
	}
	else if (respType == ECSMSDllMsgType::AVD_STATUS)
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
		jsonObj["SEquipTaskStatusResp"]["dateTime"] = Response->dateTime;
		jsonObj["SEquipTaskStatusResp"]["key"] = Response->key;
		jsonObj["SEquipTaskStatusResp"]["status"] = Response->status;
		jsonObj["SEquipTaskStatusResp"]["taskId"] = Response->taskId;

	}
	else if (respType == ECSMSDllMsgType::AVD_OCC_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"]["avg"] = Response->resultData->avg;
		jsonObj["SOccResult"]["resultData"]["avg"] = Response->resultData->max;
	}
	else	// not AUTOVIOLATE_RESPONSE
	{
		//tempstr.Format(_T("unexpected AVD message %u"),
			//respType);
	}

	streamBuffer.push_back(jsonObj.dump());

}
void processMeasResponse(_In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	if (respType == ECSMSDllMsgType::VALIDATE_MEAS)
	{
		SEquipCtrlMsg::SValidateMeasurementResp* DwellResponse = (SEquipCtrlMsg::SValidateMeasurementResp*)data;
		jsonObj["SValidateMeasurementResp"]["bwDwellTime"] = DwellResponse->bwDwellTime;
		jsonObj["SValidateMeasurementResp"]["dfDwellTime"] = DwellResponse->dfDwellTime;
		jsonObj["SValidateMeasurementResp"]["fieldStrengthDwellTime"] = DwellResponse->fieldStrengthDwellTime;
		jsonObj["SValidateMeasurementResp"]["freqDwellTime"] = DwellResponse->freqDwellTime;
		jsonObj["SValidateMeasurementResp"]["modulationDwellTime"] = DwellResponse->modulationDwellTime;
		jsonObj["SValidateMeasurementResp"]["status"] = DwellResponse->status;
		jsonObj["SValidateMeasurementResp"]["totalTime"] = DwellResponse->totalTime;
	}
	else if (respType == ECSMSDllMsgType::GET_MEAS)
	{
		SEquipCtrlMsg::SGetMeasResp* MeasResponse = (SEquipCtrlMsg::SGetMeasResp*)data;
		jsonObj["SGetMeasResp"]["amProbDist"] = MeasResponse->amProbDist;
		jsonObj["SGetMeasResp"]["bwBinData"] = MeasResponse->bwBinData;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaBw"] = MeasResponse->bwResponse.betaBw;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaFreqHigh"] = MeasResponse->bwResponse.betaFreqHigh;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaFreqLow"] = MeasResponse->bwResponse.betaFreqLow;
		jsonObj["SGetMeasResp"]["bwResponse"]["betaStdDev"] = MeasResponse->bwResponse.betaStdDev;
		jsonObj["SGetMeasResp"]["bwResponse"]["binSize"]["internal"] = MeasResponse->bwResponse.binSize.internal;
		jsonObj["SGetMeasResp"]["bwResponse"]["freq"]["internal"] = MeasResponse->bwResponse.freq.internal;
		jsonObj["SGetMeasResp"]["bwResponse"]["numBins"] = MeasResponse->bwResponse.numBins;
		jsonObj["SGetMeasResp"]["bwResponse"]["repeatCount"] = MeasResponse->bwResponse.repeatCount;
		jsonObj["SGetMeasResp"]["bwResponse"]["status"] = MeasResponse->bwResponse.status;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1Bw"] = MeasResponse->bwResponse.x1Bw;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1FreqHigh"] = MeasResponse->bwResponse.x1FreqHigh;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1FreqLow"] = MeasResponse->bwResponse.x1FreqLow;
		jsonObj["SGetMeasResp"]["bwResponse"]["x1StdDev"] = MeasResponse->bwResponse.x1StdDev;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2Bw"] = MeasResponse->bwResponse.x2Bw;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2FreqHigh"] = MeasResponse->bwResponse.x2FreqHigh;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2FreqLow"] = MeasResponse->bwResponse.x2FreqLow;
		jsonObj["SGetMeasResp"]["bwResponse"]["x2StdDev"] = MeasResponse->bwResponse.x2StdDev;
	}
	streamBuffer.push_back(jsonObj.dump());
}
void processDemodCtrlResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);

	if (respType == ECSMSDllMsgType::SET_RCVR)
	{
		SEquipCtrlMsg::SGenericResp* RcvrResponse = (SEquipCtrlMsg::SGenericResp*)data;
		jsonObj["SGenericResp"]["status"] = RcvrResponse->status;
	}
	else if (respType == ECSMSDllMsgType::SET_PAN_PARAMS)
	{
		SEquipCtrlMsg::SGenericResp* PanParaResponse = (SEquipCtrlMsg::SGenericResp*)data;
		jsonObj["SGenericResp"]["status"] = PanParaResponse->status;
	}
	else if (respType == ECSMSDllMsgType::SET_AUDIO_PARAMS)
	{
		SEquipCtrlMsg::SAudioParamsResp* AudioResponse = (SEquipCtrlMsg::SAudioParamsResp*)data;
		jsonObj["SAudioParamsResp"]["channel"] = AudioResponse->channel;
		jsonObj["SAudioParamsResp"]["ipAddress"] = AudioResponse->ipAddress;
		jsonObj["SAudioParamsResp"]["status"] = AudioResponse->status;
	}
	else if (respType == ECSMSDllMsgType::FREE_AUDIO_CHANNEL)
	{
		SEquipCtrlMsg::SGenericResp* FreeAudioResponse = (SEquipCtrlMsg::SGenericResp*)data;
		jsonObj["SGenericResp"]["status"] = FreeAudioResponse->status;
	}
	else
	{
		//tempstr.Format(_T("unexpected Demod Control message subType %lu"),
		//	m_Response.hdr.msgSubType);
	}
	streamBuffer.push_back(jsonObj.dump());
}
void processPanResponse(_In_ SEquipCtrlMsg::UBody* data)
{
	//error codes:
	// 0 = no error
	// 1 = server timeout
	// 2 = Carrier signal not detected
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(GET_ANT_LIST_INFO);
	SEquipCtrlMsg::SGetPanResp* PanResponse = (SEquipCtrlMsg::SGetPanResp*)data;
	jsonObj["SGetPanResp"]["audioPower"]["active"] = PanResponse->audioPower->active;
	jsonObj["SGetPanResp"]["audioPower"]["powerdBm"] = PanResponse->audioPower->powerdBm;
	jsonObj["SGetPanResp"]["binData"] = PanResponse->binData;
	jsonObj["SGetPanResp"]["binSize"]["internal"] = PanResponse->binSize.internal;
	jsonObj["SGetPanResp"]["conversionFactorForFS"] = PanResponse->conversionFactorForFS;
	jsonObj["SGetPanResp"]["dateTime"] = PanResponse->dateTime;
	jsonObj["SGetPanResp"]["freq"]["internal"] = PanResponse->freq.internal;
	jsonObj["SGetPanResp"]["nActiveAudioChannels"] = PanResponse->nActiveAudioChannels;
	jsonObj["SGetPanResp"]["numBins"] = PanResponse->numBins;
	jsonObj["SGetPanResp"]["powerDbm"] = PanResponse->powerDbm;
	jsonObj["SGetPanResp"]["rcvrAtten"] = PanResponse->rcvrAtten;
	jsonObj["SGetPanResp"]["status"] = PanResponse->status;
	streamBuffer.push_back(jsonObj.dump());
}
void processOccupancyResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);
	switch (respType)
	{
	case ECSMSDllMsgType::OCC_MSGLEN_DIST_RESPONSE:
	{
		SEquipCtrlMsg::SMsgLengthDistributionResp* Response = (SEquipCtrlMsg::SMsgLengthDistributionResp*)data;
		jsonObj["SMsgLengthDistributionResp"]["histData"]["channel"] = Response->histData->channel;
		jsonObj["SMsgLengthDistributionResp"]["histData"]["length"] = Response->histData->length;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SMsgLengthDistributionResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
	}
	break;
	case ECSMSDllMsgType::OCC_SPECTRUM_RESPONSE:
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	break;
	case ECSMSDllMsgType::OCC_STATE_RESPONSE:
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		jsonObj["SStateResp"]["completionTime"] = (unsigned long)Response->completionTime;
		jsonObj["SStateResp"]["state"] = (unsigned long)Response->state;
	}
	break;
	case ECSMSDllMsgType::OCC_SOLICIT_STATE_RESPONSE:
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		jsonObj["SStateResp"]["completionTime"] = (unsigned long)Response->completionTime;
		jsonObj["SStateResp"]["state"] = (unsigned long)Response->state;
	}
	break;
	case ECSMSDllMsgType::OCC_FREQ_VS_CHANNEL:
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
		jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = Response->frequencies->internal;
		jsonObj["SFrequencyVsChannelResp"]["hostName"] = Response->hostName;
		jsonObj["SFrequencyVsChannelResp"]["numBands"] = Response->numBands;
		jsonObj["SFrequencyVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = Response->occPrimaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = Response->occSecondaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = Response->saveIntermediateData;
		jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = Response->selectedAntenna;
		jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = Response->useSecondaryThreshold;
	}
	break;
	case ECSMSDllMsgType::OCC_CHANNEL_RESULT:
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	break;
	case ECSMSDllMsgType::OCC_EFLD_CHANNEL_RESULT:
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	break;
	case ECSMSDllMsgType::OCC_TIMEOFDAY_RESULT:
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	break;
	case ECSMSDllMsgType::OCC_MSGLEN_CHANNEL_RESULT:
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	break;
	case ECSMSDllMsgType::OCC_EFLD_TIMEOFDAY_RESULT:
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		jsonObj["SOccResult"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)Response->occHdr.numChannels;
		jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)Response->occHdr.numTimeOfDays;
		jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)Response->occHdr.numTotalChannels;
		jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)Response->occHdr.status;
		jsonObj["SOccResult"]["resultData"] = (unsigned long)Response->resultData;
	}
	break;
	case ECSMSDllMsgType::OCC_STATUS:
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
		jsonObj["SEquipTaskStatusResp"]["dateTime"] = Response->dateTime;
		jsonObj["SEquipTaskStatusResp"]["key"] = Response->key;
		jsonObj["SEquipTaskStatusResp"]["status"] = Response->status;
		jsonObj["SEquipTaskStatusResp"]["taskId"] = Response->taskId;
	}
	break;
	case ECSMSDllMsgType::VALIDATE_OCCUPANCY:
	{
		SEquipCtrlMsg::SValidateOccupancyResp* Response = (SEquipCtrlMsg::SValidateOccupancyResp*)data;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["ant"] = Response->occCmd.ant;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["channelBandwidth"]["internal"] = Response->occCmd.band[0].channelBandwidth.internal;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["exclude"] = Response->occCmd.band[0].exclude;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["highFrequency"]["internal"] = Response->occCmd.band[0].highFrequency.internal;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["lowFrequency"]["internal"] = Response->occCmd.band[0].lowFrequency.internal;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["horizPol"] = (unsigned long)Response->occCmd.band[0].sType.signalType.horizPol;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["narrow"] = (unsigned long)Response->occCmd.band[0].sType.signalType.narrow;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["unused0"] = (unsigned long)Response->occCmd.band[0].sType.signalType.unused0;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["unused1"] = (unsigned long)Response->occCmd.band[0].sType.signalType.unused1;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["confidenceLevel"] = Response->occCmd.confidenceLevel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["desiredAccuracy"] = Response->occCmd.desiredAccuracy;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["durationMethod"] = Response->occCmd.durationMethod;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["measurementTime"] = Response->occCmd.measurementTime;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["numBands"] = Response->occCmd.numBands;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["occPrimaryThreshold"] = Response->occCmd.occPrimaryThreshold;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["occSecondaryThreshold"] = Response->occCmd.occSecondaryThreshold;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["occupancyMinGap"] = Response->occCmd.occupancyMinGap;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["eFieldVsChannel"] = Response->occCmd.output.eFieldVsChannel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["msglengthDistribution"] = Response->occCmd.output.msglengthDistribution;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["msglengthVsChannel"] = Response->occCmd.output.msglengthVsChannel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["adjustAgcThres"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.adjustAgcThres;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["eFieldVsTOD"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.eFieldVsTOD;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["gainMode"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.gainMode;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["spurTest"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.spurTest;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["unused"] = (unsigned char)Response->occCmd.output.occupancyFlag.flag.unused;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["zeroFlag"] = (unsigned char)Response->occCmd.output.occupancyFlag.zeroFlag;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyVsChannel"] = (unsigned char)Response->occCmd.output.occupancyVsChannel;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyVsTimeOfDay"] = (unsigned char)Response->occCmd.output.occupancyVsTimeOfDay;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["spectrogram"] = (unsigned char)Response->occCmd.output.spectrogram;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["timegram"] = (unsigned char)Response->occCmd.output.timegram;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["saveIntermediateData"] = Response->occCmd.saveIntermediateData;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["storageTime"] = Response->occCmd.storageTime;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["thresholdMethod"] = Response->occCmd.thresholdMethod;
		jsonObj["SValidateOccupancyResp"]["occCmd"]["useSecondaryThreshold"] = Response->occCmd.useSecondaryThreshold;
		jsonObj["SValidateOccupancyResp"]["status"] = Response->status;
	}
	break;
	default: // not OCCUPANCY_RESPONSE
		//tempstr.Format(_T("unexpected occupancy message %u"),
			//respType);
		logMIAer.warn("unexpected occupancy message: " + std::to_string(respType));
		return;
	}
	streamBuffer.push_back(jsonObj.dump());
}
void processOccupancyDFResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);

	switch (respType)
	{
	case ECSMSDllMsgType::OCCDF_STATE_RESPONSE:
		{
			SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
			jsonObj["SStateResp"]["completionTime"] = Response->completionTime;
			jsonObj["SStateResp"]["state"] = Response->state;
		}
		break;
	case ECSMSDllMsgType::OCCDF_SOLICIT_STATE_RESPONSE:
		{
			SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
			jsonObj["SStateResp"]["completionTime"] = Response->completionTime;
			jsonObj["SStateResp"]["state"] = Response->state;
		}
		break;
	case ECSMSDllMsgType::OCCDF_FREQ_VS_CHANNEL:
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
		jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = Response->frequencies->internal;
		
		jsonObj["SFrequencyVsChannelResp"]["hostName"] = Response->hostName;
		jsonObj["SFrequencyVsChannelResp"]["numBands"] = Response->numBands;
		jsonObj["SFrequencyVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = Response->occPrimaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = Response->occSecondaryThreshold;
		jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = Response->saveIntermediateData;
		jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = Response->selectedAntenna;
		jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = Response->useSecondaryThreshold;
	}
	break;
	case ECSMSDllMsgType::OCCDF_SCANDF_VS_CHANNEL:
	{
		SEquipCtrlMsg::SScanDfVsChannelResp* Response = (SEquipCtrlMsg::SScanDfVsChannelResp*)data;
		jsonObj["SScanDfVsChannelResp"]["aveFldStr"] = Response->aveFldStr;
		jsonObj["SScanDfVsChannelResp"]["aveRange"] = Response->aveRange;
		jsonObj["SScanDfVsChannelResp"]["numAzimuths"] = Response->numAzimuths;
		jsonObj["SScanDfVsChannelResp"]["numChannels"] = Response->numChannels;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["firstChannel"] = Response->occHdr.firstChannel;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = Response->occHdr.gpsResponse.dateTime;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = Response->occHdr.gpsResponse.latitude;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = Response->occHdr.gpsResponse.longitude;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)Response->occHdr.gpsResponse.status.accuracy;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)Response->occHdr.gpsResponse.status.antenna;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.batVolt;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)Response->occHdr.gpsResponse.status.lockHist;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)Response->occHdr.gpsResponse.status.mode;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)Response->occHdr.gpsResponse.status.noGps;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)Response->occHdr.gpsResponse.status.notTested;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)Response->occHdr.gpsResponse.status.numSats;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)Response->occHdr.gpsResponse.status.nvRam;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)Response->occHdr.gpsResponse.status.oscVolt;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)Response->occHdr.gpsResponse.status.pllSynth;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)Response->occHdr.gpsResponse.status.receiver;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)Response->occHdr.gpsResponse.status.satLock;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr1;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)Response->occHdr.gpsResponse.status.timErr2;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)Response->occHdr.gpsResponse.status.timSrce;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)Response->occHdr.gpsResponse.status.tracking;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["numChannels"] = Response->occHdr.numChannels;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["numTimeOfDays"] = Response->occHdr.numTimeOfDays;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["numTotalChannels"] = Response->occHdr.numTotalChannels;
		jsonObj["SScanDfVsChannelResp"]["occHdr"]["status"] = Response->occHdr.status;
		jsonObj["SScanDfVsChannelResp"]["scanDfData"] = Response->scanDfData;
	}
	break;
	case ECSMSDllMsgType::OCCDF_STATUS:
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
		jsonObj["SEquipTaskStatusResp"]["dateTime"] = Response->dateTime;
		jsonObj["SEquipTaskStatusResp"]["key"] = Response->key;
		jsonObj["SEquipTaskStatusResp"]["status"] = Response->status;
		jsonObj["SEquipTaskStatusResp"]["taskId"] = Response->taskId;
	}
	break;
	default: // not OCCUPANCYDF_RESPONSE
		//tempstr.Format(_T("unexpected occupancyDF message %u"),
			//respType);
        logMIAer.warn("Unexpected occupancyDF message: " + std::to_string(respType));
		return;
	}

	streamBuffer.push_back(jsonObj.dump());

}
void ProcessRealTimeData(_In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);
	switch (respType)
	{
	case ECSMSDllMsgType::RT_SPECTRUM_START:
	{
		const SSmsRealtimeMsg::SStartV2* pData = (SSmsRealtimeMsg::SStartV2*)data;
		jsonObj["SStartV2"]["band"]["chanSize"]["internal"] = pData->band->chanSize.internal;
		jsonObj["SStartV2"]["band"]["firstChanFreq"]["internal"] = pData->band->firstChanFreq.internal;
		jsonObj["SStartV2"]["band"]["numChan"] = pData->band->numChan;
		jsonObj["SStartV2"]["numBands"] = pData->numBands;
		jsonObj["SStartV2"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_STOP:
	{
		const SSmsRealtimeMsg::SStop* pData = (const SSmsRealtimeMsg::SStop*)data;
		jsonObj["SStop"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_V1RESPONSE:
	{
		const SSmsRealtimeMsg::SSpectrum* pData = (SSmsRealtimeMsg::SSpectrum*)data;
		jsonObj["SSpectrum"]["bandIndex"] = pData->bandIndex;
		jsonObj["SSpectrum"]["chanData"] = pData->chanData;
		jsonObj["SSpectrum"]["chanSize"] = pData->chanSize;
		jsonObj["SSpectrum"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SSpectrum"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SSpectrum"]["numChan"] = pData->numChan;
		jsonObj["SSpectrum"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_V2RESPONSE:
	{
		const SSmsRealtimeMsg::SSpectrumV2* pData = (SSmsRealtimeMsg::SSpectrumV2*)data;
		jsonObj["SSpectrumV2"]["bandIndex"] = pData->bandIndex;
		jsonObj["SSpectrumV2"]["chanData"] = pData->chanData;
		jsonObj["SSpectrumV2"]["chanSize"]["internal"] = pData->chanSize.internal;
		jsonObj["SSpectrumV2"]["firstChanFreq"]["internal"] = pData->firstChanFreq.internal;
		jsonObj["SSpectrumV2"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SSpectrumV2"]["numChan"] = pData->numChan;
		jsonObj["SSpectrumV2"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_SPECTRUM_RESPONSE:
	{
		const SSmsRealtimeMsg::SSpectrumV3* pData = (SSmsRealtimeMsg::SSpectrumV3*)data;
		jsonObj["SSpectrumV3"]["bandIndex"] = pData->bandIndex;
		jsonObj["SSpectrumV3"]["chanData"] = pData->chanData;
		jsonObj["SSpectrumV3"]["chanSize"]["internal"] = pData->chanSize.internal;
		jsonObj["SSpectrumV3"]["firstChanFreq"]["internal"] = pData->firstChanFreq.internal;
		jsonObj["SSpectrumV3"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SSpectrumV3"]["numChan"] = pData->numChan;
		jsonObj["SSpectrumV3"]["taskId"] = pData->taskId;
		jsonObj["SSpectrumV3"]["efield"] = pData->efield;
		jsonObj["SSpectrumV3"]["zeroVal"] = pData->zeroVal;
		break;

	}
	case ECSMSDllMsgType::RT_DF_START:
	{
		const SSmsRealtimeMsg::SStartV2* pData = (SSmsRealtimeMsg::SStartV2*)data;
		jsonObj["SStartV2"]["band"]["chanSize"]["internal"] = pData->band->chanSize.internal;
		jsonObj["SStartV2"]["band"]["firstChanFreq"]["internal"] = pData->band->firstChanFreq.internal;
		jsonObj["SStartV2"]["band"]["numChan"] = pData->band->numChan;
		jsonObj["SStartV2"]["numBands"] = pData->numBands;
		jsonObj["SStartV2"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_STARTV1:
	{
		const SSmsRealtimeMsg::SStart* pData = (SSmsRealtimeMsg::SStart*)data;
		jsonObj["SStart"]["chanSize"] = pData->chanSize;
		jsonObj["SStart"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SStart"]["MAX_OCCBANDS"] = pData->MAX_OCCBANDS;
		jsonObj["SStart"]["numBands"] = pData->numBands;
		jsonObj["SStart"]["numChan"] = pData->numChan;
		jsonObj["SStart"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_STOP:
	{
		const SSmsRealtimeMsg::SStop* pData = (SSmsRealtimeMsg::SStop*)data;
		jsonObj["SStop"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_DATAV1:
	{
		const SSmsRealtimeMsg::SDfData* pData = (SSmsRealtimeMsg::SDfData*)data;
		jsonObj["SDfData"]["bandIndex"] = pData->bandIndex;
		jsonObj["SDfData"]["chanData"]["azimData"] = pData->chanData->azimData;
		jsonObj["SDfData"]["chanData"]["specData"] = pData->chanData->specData;
		jsonObj["SDfData"]["chanSize"] = pData->chanSize;
		jsonObj["SDfData"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SDfData"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SDfData"]["numChan"] = pData->numChan;
		jsonObj["SDfData"]["taskId"] = pData->taskId;
		break;
	}
	case ECSMSDllMsgType::RT_DF_DATAV2:
	{
		const SSmsRealtimeMsg::SDfDataV2* pData = (SSmsRealtimeMsg::SDfDataV2*)data;
		jsonObj["SDfDataV2"]["bandIndex"] = pData->bandIndex;
		jsonObj["SDfDataV2"]["chanData"]["azimData"] = pData->chanData->azimData;
		jsonObj["SDfDataV2"]["chanData"]["specData"] = pData->chanData->specData;
		jsonObj["SDfDataV2"]["chanSize"] = pData->chanSize;
		jsonObj["SDfDataV2"]["firstChanFreq"] = pData->firstChanFreq;
		jsonObj["SDfDataV2"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SDfDataV2"]["numChan"] = pData->numChan;
		jsonObj["SDfDataV2"]["taskId"] = pData->taskId;
		jsonObj["SDfDataV2"]["horizPol"] = pData->horizPol;
		break;
	}
	case ECSMSDllMsgType::RT_DF_DATA:
	{
		const SSmsRealtimeMsg::SDfDataV3* pData = (SSmsRealtimeMsg::SDfDataV3*)data;
		jsonObj["SDfDataV3"]["bandIndex"] = pData->bandIndex;
		jsonObj["SDfDataV3"]["chanData"]["azimData"] = pData->chanData->azimData;
		jsonObj["SDfDataV3"]["chanData"]["specData"] = pData->chanData->specData;
		jsonObj["SDfDataV3"]["chanSize"]["internal"] = pData->chanSize.internal;
		jsonObj["SDfDataV3"]["firstChanFreq"]["internal"] = pData->firstChanFreq.internal;
		jsonObj["SDfDataV3"]["noiseFloor"] = pData->noiseFloor;
		jsonObj["SDfDataV3"]["numChan"] = pData->numChan;
		jsonObj["SDfDataV3"]["taskId"] = pData->taskId;
		jsonObj["SDfDataV3"]["horizPol"] = pData->horizPol;
		break;
	}
	case ECSMSDllMsgType::RT_IQ_DATA:
	{
		SSmsRealtimeMsg::SIqDataV4* iqData = (SSmsRealtimeMsg::SIqDataV4*)data; // v5 should be sent to rds app
		jsonObj["SIqDataV4"]["actualBW"]["internal"] = iqData->actualBW.internal;
		jsonObj["SIqDataV4"]["actualSampleRate"] = iqData->actualSampleRate;
		jsonObj["SIqDataV4"]["dataType"] = iqData->dataType;
		jsonObj["SIqDataV4"]["ddcChannel"] = iqData->ddcChannel;
		jsonObj["SIqDataV4"]["EOS"] = iqData->EOS;
		jsonObj["SIqDataV4"]["freq"]["internal"] = iqData->freq.internal;
		jsonObj["SIqDataV4"]["inputPort"] = iqData->inputPort;
		jsonObj["SIqDataV4"]["MAX_SAMPLES"] = iqData->MAX_SAMPLES;
		jsonObj["SIqDataV4"]["numSamples"] = iqData->numSamples;
		jsonObj["SIqDataV4"]["rxAtten"] = iqData->rxAtten;
		jsonObj["SIqDataV4"]["sampleOffset"] = iqData->sampleOffset;
		jsonObj["SIqDataV4"]["scaleFactor"] = iqData->scaleFactor;
		jsonObj["SIqDataV4"]["seqNumber"] = iqData->seqNumber;
		jsonObj["SIqDataV4"]["SIq"]["samplesFloat32"]["im"] = iqData->SIq.samplesFloat32->im;
		jsonObj["SIqDataV4"]["SIq"]["samplesFloat32"]["re"] = iqData->SIq.samplesFloat32->re;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt16"]["im"] = iqData->SIq.samplesInt16->im;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt16"]["re"] = iqData->SIq.samplesInt16->re;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt32"]["im"] = iqData->SIq.samplesInt32->im;
		jsonObj["SIqDataV4"]["SIq"]["samplesInt32"]["re"] = iqData->SIq.samplesInt32->re;
		jsonObj["SIqDataV4"]["streamID"] = iqData->streamID;
		jsonObj["SIqDataV4"]["streamStartTime"]["timestamp"] = iqData->streamStartTime.timestamp;

		break;
	}
	default:
	{
		logMIAer.warn("Unexpected response type in ProcessRealTimeData: " + std::to_string(respType));
		return;
	}
	}
	streamBuffer.push_back(jsonObj.dump());
}
void OnData(_In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ unsigned long destAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	switch (respType)
	{
		case ECSMSDllMsgType::GET_BIST:
		case ECSMSDllMsgType::GET_BIST_RESULT:
		case ECSMSDllMsgType::GET_DIAGNOSTICS:
			processBITEResponse(respType, data);
			break;
		case ECSMSDllMsgType::GET_ANT_LIST_INFO:
			ProcessAntListResponse(data);
			break;
		case ECSMSDllMsgType::OCC_MSGLEN_DIST_RESPONSE:
		case ECSMSDllMsgType::OCC_FREQ_VS_CHANNEL:
		case ECSMSDllMsgType::OCC_CHANNEL_RESULT:
		case ECSMSDllMsgType::OCC_STATUS:
		case ECSMSDllMsgType::OCC_STATE_RESPONSE:
		case ECSMSDllMsgType::OCC_SOLICIT_STATE_RESPONSE:
		case ECSMSDllMsgType::OCC_SPECTRUM_RESPONSE:
		case ECSMSDllMsgType::OCC_TIMEOFDAY_RESULT:
		case ECSMSDllMsgType::OCC_EFLD_CHANNEL_RESULT:
		case ECSMSDllMsgType::OCC_MSGLEN_CHANNEL_RESULT:
		case ECSMSDllMsgType::OCC_EFLD_TIMEOFDAY_RESULT:
			processOccupancyResponse(respType, data);
			break;
		case ECSMSDllMsgType::OCCDF_FREQ_VS_CHANNEL:
		case ECSMSDllMsgType::OCCDF_SCANDF_VS_CHANNEL:
		case ECSMSDllMsgType::OCCDF_STATUS:
		case ECSMSDllMsgType::OCCDF_STATE_RESPONSE:
		case ECSMSDllMsgType::OCCDF_SOLICIT_STATE_RESPONSE:
			processOccupancyDFResponse(respType, data);
			break;
		case ECSMSDllMsgType::AVD_FREQ_VS_CHANNEL:
		case ECSMSDllMsgType::AVD_OCC_CHANNEL_RESULT:
		case ECSMSDllMsgType::AVD_FREQ_MEAS:
		case ECSMSDllMsgType::AVD_BW_MEAS:
		case ECSMSDllMsgType::AVD_SOLICIT_STATE_RESPONSE:
		case ECSMSDllMsgType::AVD_STATE_RESPONSE:
		case ECSMSDllMsgType::AVD_STATUS:
			processAutoViolateResponse(respType, data);
			break;
		case ECSMSDllMsgType::GET_MEAS:
		case ECSMSDllMsgType::VALIDATE_MEAS:
			processMeasResponse(respType, sourceAddr, data);
			break;
		case ECSMSDllMsgType::SET_PAN_PARAMS:
		case ECSMSDllMsgType::SET_AUDIO_PARAMS:
		case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
			processDemodCtrlResponse(respType, data);
			break;
		case ECSMSDllMsgType::GET_PAN:
			processPanResponse(data);
			break;
		case ECSMSDllMsgType::GET_DM:

			//m_taskType = ECSMSDllMsgType::GET_DM; // TODO should be in DM_STATUS reponse instead
			break;
		default:
			logMIAer.warn("Unexpected response type: " + std::to_string(respType));
			break;
	}
	return;
}

/*
* Felipe Machado - 23/08/2024
* Data callback for Scorpio API
*/
void OnDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ unsigned long desstAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	OnData(respType, sourceAddr, desstAddr, data);
	//streamBuffer.push_back(strData);
	logMIAer.info("OnData received with type " + respType);
	logMIAer.info("OnData received destination address " + desstAddr);
	logMIAer.info("OnData received server ID " + serverId);
}

/*
* Felipe Machado - 23/08/2024
* Error callback for Scorpio API
*/
void OnErrorFunc(_In_  unsigned long serverId, _In_ const std::wstring& errorMsg)
{
	std::string str(errorMsg.begin(), errorMsg.end());
	logMIAer.error(str);
	std::string strData = "{\"serverId\":"+ std::to_string(serverId) + ", \"errorMsg\":\"" + str + "\"}";
	errorBuffer.push_back(strData);
	logMIAer.info("OnErrorFunc received error message: " + str);
	logMIAer.info("OnErrorFunc received server ID: " + serverId);

}

/*
* Felipe Machado - 02/09/2024
* Realtime callback for Scorpio API
*/
void OnRealTimeDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data)
{
	ProcessRealTimeData(respType, data);
	logMIAer.info("OnRealtimeData received with type " + respType);
	logMIAer.info("OnRealtimeData received server ID " + serverId);
}

//
// Signal handler function
//
void signalHandler(int signal) {

	if (signal == SIGINT)
	{
		running.store(false);
		interruptionCode.store(mcs::Code::CTRL_C_INTERRUPT);
		logMIAer.warn("Received interruption signal (ctrl+C)");
	}
	else if (signal == SIGTERM)
	{
		running.store(false);
		interruptionCode.store(mcs::Code::KILL_INTERRUPT);
		logMIAer.warn("Received termination signal (kill)");
	}
	else 	{
		std::string message = "Received unknown signal. #LttOS: " + std::to_string(signal);
		logMIAer.warn(message);
	}
}

//
// Register the signal handlers
//
void registerSignalHandlers() {
	std::signal(SIGINT, signalHandler);  // Handles Ctrl+C
	std::signal(SIGTERM, signalHandler); // Handles kill command
}

//
// Function to create simulated data
// Data is a random number of packages, from 1 to 5.
// Each package has 4096 bytes of data, which is 1024 floats of 32 bits
// First 12 points are a sequency of 0, 1, 0, -1, repeated 3 times
// Following 500 points describes a ramp from -100 to -20
// Finally 512 points describes cosine cycles with the same amplitude. The number of cycles varies with the package number.

void generateSimData(void) {
	
	float data[1024];
	int j = 1;
	std::string trace;
	
	for (j = 1; rand() % 6; j++) {
		for (int i = 0; i < 12; i = i + 4) {
			data[0+i] = (float)(0.0);
			data[1+i] = (float)(1.0);
			data[2+i] = (float)(0.0);
			data[3+i] = (float)(-1.0);
		}
		for (int i = 12; i < 512; i++) {
			data[i] = (float)((40.0 * (((float)(i) / 250) - 1)) - 60.0);
		}
		for (int i = 512; i < 1024; i++) {
			data[i] = (float)((60.0 * cos((i * 0.0184)*2*j)) - 80.0);
		}
		trace = std::string(reinterpret_cast<char*>(data), sizeof(data));
		streamBuffer.push_back(trace);
	}
	
	// logger.info("Simulated data generated " + std::to_string(j) + " traces with 1024 float points.");
	if (sizeof(trace) > 48) {
		std::string message = "Sample points: [";
		// add the first 12 points as uint8 translation from the streamBuffer
		for (int i = 0; i < 48; i++) {
			message += std::to_string((unsigned char)trace[i]) + ", ";
		}
		message += "...]";
		logMIAer.info(message);
	}
	//
}

//
// Function to handle command connections
//
void handleCommandConnection(SOCKET clientSocket, std::string name) {
	clientSocketCommand = clientSocket;

	char buffer[1024];

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead > 0) {
			std::string command(buffer, bytesRead);
			{
				std::lock_guard<std::mutex> lock(MCCommandMutex);
				commandQueue.push_back(command);
			}
			
			std::string ack = mcs::Form::ACK + 
				std::to_string(commandQueueCounter) +
				mcs::Form::SEP;
			commandQueueCounter++;

			iResult = send(clientSocket, ack.c_str(), (int)(ack.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " ACK send failed. EC:" + std::to_string(WSAGetLastError()));
				logMIAer.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				// test if the connection is still alive
				iResult = send(clientSocket,
					mcs::Form::PING,
					static_cast<int>(strlen(mcs::Form::PING)),
					0);
				if (iResult == SOCKET_ERROR) {
					logMIAer.warn(name + " PING send failed. EC:" + std::to_string(WSAGetLastError()));
					logMIAer.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
					return;
				}

				logMIAer.info(name + " waiting for commands from client...");
				checkPeriod = config["service"]["command"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["command"]["sleep_ms"].get<int>()));
		}
		
	}
}

//
// Function to handle streaming data
//
void handleStreamConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!streamBuffer.empty()) {
			std::string data = streamBuffer.back() + mcs::Form::BLOCK_END;
			streamBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				if (config["service"]["simulated"].get<bool>()) {
					generateSimData();
					logMIAer.info(name + " generated sim data. Nothing from the station.");
				}
				else {
					logMIAer.info(name + " waiting for data from station to send...");
				}
				checkPeriod = config["service"]["stream"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["stream"]["sleep_ms"].get<int>()));
		}
	}
}

//
// Function to handle error data
//
void handleErrorConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!errorBuffer.empty()) {
			std::string data = errorBuffer.back() + mcs::Form::BLOCK_END;
			errorBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		} else {
			if (checkPeriod == 0) {
				logMIAer.info(name + " waiting for data from station to send...");
				checkPeriod = config["service"]["error"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["error"]["sleep_ms"].get<int>()));
		}
	}
}

//
// Function to handle real time data
//
void handleRealTimeConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!realtimeBuffer.empty()) {
			std::string data = realtimeBuffer.back() + mcs::Form::BLOCK_END;
			realtimeBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				logMIAer.info(name + " waiting for data from station to send...");
				checkPeriod = config["service"]["realtime"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["realtime"]["sleep_ms"].get<int>()));
		}
	}
}

//
// Function to listen on a specific port
//
void socketHandle(	std::string name,
					int ServiceCode,
					int port,
					int timeout,
					void (*connectionHandler)(SOCKET, std::string)) {

	SOCKET listenSocket = INVALID_SOCKET;
	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::string portStr = std::to_string(port);
	int iResult = getaddrinfo(NULL, portStr.c_str(), &hints, &result);
	if (iResult != 0) {
		logMIAer.error(name + " socket getaddrinfo failed. EC:" + std::to_string(iResult));
		running.store(false);
		interruptionCode.store(ServiceCode);
		WSACleanup();
		return;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		logMIAer.error(name + " socket creation failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	iResult = setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	if (iResult == SOCKET_ERROR) {
		logMIAer.error(name + " socket setsockopt timeout failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	iResult = ::bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		logMIAer.error(name + " socket bind failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		logMIAer.error(name + " socket listen failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	// create variable to store the client address
	struct sockaddr_in clientAddr;

	SOCKET clientSocket = INVALID_SOCKET;
	while (running.load()) {
		// call the connection handler 
		logMIAer.info(name + " listening on port " + std::to_string(port));

		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, NULL);
		if (clientSocket == INVALID_SOCKET) {
			logMIAer.warn(name + " failed in accept operation with " + std::string(inet_ntoa(clientAddr.sin_addr)) + ". EC:" + std::to_string(WSAGetLastError()));
		}
		else {
			logMIAer.info(name + " accepted connection from " + std::string(inet_ntoa(clientAddr.sin_addr)));

			connectionHandler(clientSocket, name);
		}
	}

	closesocket(listenSocket);
	WSACleanup();
	logMIAer.info(name + " stopped listening on port " + std::to_string(port));
}

//
// Create a connection object to the station and connect to it.
//
void StationConnect(void) {
	// Create a local copy of APIserverId. This is necessary because TCI methods update the APIserverId value to the next available ID.
	// unsigned long NextServerId = APIserverId;

	// Hostname as simple string, extracted from th JSON configuration file
	std::string hostNameStr = config["station"]["address"].get<std::string>();
	station.hostName = stringToWString(hostNameStr);

	// Port as simple string, extracted from the JSON configuration file where it is defined as a number
	std::string portStr = std::to_string(config["station"]["port"].get<int>());
	station.port = stringToWString(portStr);

	// Timeout as unsigned long, extracted from the JSON configuration file in second and converted to miliseconds
	station.sendTimeout = (unsigned long)(config["station"]["timeout_s"].get<int>())*1000;

	// Error code using API ERetCode enum
	ERetCode errCode;

	// Create the connection object
	errCode = ScorpioAPICreate(
					APIserverId,
					station,
					OnErrorFunc,
					OnDataFunc,
					OnRealTimeDataFunc);

	// Error message string to be used in the logger
	std::string message;

	// Handle the error code from object creation
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Object associated with station not created: " + ERetCodeToString(errCode);
		logMIAer.error(message);
		//running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
		return;
	}
	else
	{
		logMIAer.info("Object creation successful");
	}

	// NextServerId = APIserverId;

	// Once the object was successfully created, test connection to the station

	errCode = RequestCapabilities(APIserverId, StationCapabilities);

	// Handle the error code from station connection
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Failed to connect to " + hostNameStr + ": " + ERetCodeToString(errCode);
		logMIAer.error(message);
//		running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
		return;
	}
	else
	{
		message = "Connected to station " + hostNameStr;
		logMIAer.info(message);
	}
}

//
// Disconnect station and socket clients
//
void StationDisconnect(void)
{

	ERetCode errCode = Disconnect(APIserverId);
	logMIAer.warn("Disconnecting station returned:" + ERetCodeToString(errCode));
	
	/* DLL function not returning API_SUCCESS - Need to investigate
	if (errCode != ERetCode::API_SUCCESS)
	{
		logger.error("Error disconnecting from station " + ERetCodeToString(errCode));
		running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
	}
	else
	{
		logger.info("Disconnected from station");
	}
	*/
}

int main() {

	registerSignalHandlers();

	// Read configuration from JSON file
	std::ifstream config_file("config.json");
	/*
	  * Felipe Machado - 29/08/2024
	  * checks if configuration file exists, if does not exist, a new file is generated with default values 
	*/
	if (config_file.fail()) {
		newDefaultConfigFile();
		std::ifstream config_file("config.json");
		config = json::parse(config_file);
	}
	else {
		config = json::parse(config_file);
	}	

	logMIAer.start("MIAerConn", config["log"]["console"]["enable"].get<bool>(), config["log"]["file"]["enable"].get<bool>(), config["log"]["console"]["level"].get<std::string>(), config["log"]["file"]["path"].get<std::string>(), config["log"]["file"]["level"].get<std::string>());
	StationConnect();

	// Start thread for the command channel socket service. This thread will listen for incoming commands and place then in the command queue
	std::thread commandThread(socketHandle,
		"Control service",
		mcs::Code::COMMAND_ERROR,
		config["service"]["command"]["port"].get<int>(),
		config["service"]["command"]["timeout_s"].get<int>(),
		handleCommandConnection);

	// Start thread for the stream channel socket service. This thread will listen for incoming connections and stream data back to the client
	std::thread streamThread(socketHandle,
		"Stream service",
		mcs::Code::STREAM_ERROR,
		config["service"]["stream"]["port"].get<int>(),
		config["service"]["stream"]["timeout_s"].get<int>(),
		handleStreamConnection);

	/*
	  * Felipe Machado - 02/09/2024
	*/
	// Start thread for the stream channel socket service. This thread will stream error back to the client
	std::thread errorThread(socketHandle,
		"Error service",
		mcs::Code::STREAM_ERROR,
		config["service"]["error"]["port"].get<int>(),
		config["service"]["error"]["timeout_s"].get<int>(),
		handleErrorConnection);

	/*
	  * Felipe Machado - 02/09/2024
	*/
	// Start thread for the stream channel socket service. This thread will stream real time data to the client
	std::thread realtimeThread(socketHandle,
		"Real time service",
		mcs::Code::STREAM_ERROR,
		config["service"]["realtime"]["port"].get<int>(),
		config["service"]["realtime"]["timeout_s"].get<int>(),
		handleRealTimeConnection);
		
	// Main loop to process commands received
	while (running.load()) {

		if (!commandQueue.empty()) {
			{
				std::lock_guard<std::mutex> lock(MCCommandMutex);
				std::string command = commandQueue.back();
				commandQueue.pop_back();
			}

			logMIAer.info("Processing command: " + command);
			
			/*
			* Felipe Machado - 23/08/2024
			* Exemplo pacote socket
			* {commandCode: 1, commandStruct: struct, taskType: , reqID: ,  }
			*/ 
			unsigned long requestID = 0;
			using json = nlohmann::json;
			json jsonObj = json::parse(command);

			int cmd = jsonObj["commandCode"].get<int>();

			switch (cmd)
			{
				case ECSMSDllMsgType::GET_OCCUPANCY:
					logMIAer.logCommandExec(RequestOccupancy(APIserverId, jsonToSOccupReqData(jsonObj["occupancyParams"]), &requestID), "RequestOccupancy");
					break;
				case ECSMSDllMsgType::GET_OCCUPANCYDF:
					logMIAer.logCommandExec(RequestOccupancyDF(APIserverId, jsonToSOccDFReqData(jsonObj["occupancyParams"]), &requestID), "RequestOccupancyDF");
					break;
				case ECSMSDllMsgType::GET_AVD:
					logMIAer.logCommandExec(RequestAVD(APIserverId, jsonToSAVDReqData(jsonObj["acdParams"]), &requestID), "RequestAVD");
					break;
				case ECSMSDllMsgType::GET_MEAS:
					logMIAer.logCommandExec(RequestMeasurement(APIserverId, jsonToSMeasReqData(jsonObj["measParams"]), &requestID), "RequestMeasurement");
					break;
				case ECSMSDllMsgType::GET_TASK_STATUS:
					logMIAer.logCommandExec(RequestTaskStatus(APIserverId, jsonObj["reqId"].get<unsigned long>()), "RequestTaskStatus");
					break;
				case ECSMSDllMsgType::GET_TASK_STATE:
					logMIAer.logCommandExec(RequestTaskState(APIserverId, (ECSMSDllMsgType)jsonObj["taskType"].get<unsigned long>(), jsonObj["reqId"].get<unsigned long>()), "RequestTaskState");
					break;
				case ECSMSDllMsgType::TASK_SUSPEND:
					logMIAer.logCommandExec(SuspendTask(APIserverId, (ECSMSDllMsgType)jsonObj["taskType"].get<unsigned long>(), jsonObj["reqId"].get<unsigned long>()), "SuspendTask");
					break;
				case ECSMSDllMsgType::TASK_RESUME:
					logMIAer.logCommandExec(ResumeTask(APIserverId, (ECSMSDllMsgType)jsonObj["taskType"].get<unsigned long>(), jsonObj["reqId"].get<unsigned long>()), "ResumeTask");
					break;
				case ECSMSDllMsgType::TASK_TERMINATE:
					logMIAer.logCommandExec(TerminateTask(APIserverId, jsonObj["reqId"].get<unsigned long>()), "TerminateTask");
					break;
				case ECSMSDllMsgType::GET_BIST:
					logMIAer.logCommandExec(RequestBist(APIserverId, (EBistScope)jsonObj["scope"].get<int>(), &requestID), "RequestBist");
					break;
				case ECSMSDllMsgType::SET_AUDIO_PARAMS:
					logMIAer.logCommandExec(SetAudio(APIserverId, jsonToSAudioParams(jsonObj["audioParams"]), &requestID), "SetAudio");
					break;
				case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
					logMIAer.logCommandExec(FreeAudio(APIserverId, jsonObj["channel"].get<unsigned long>(), &requestID), "FreeAudio");
					break;
				case ECSMSDllMsgType::SET_PAN_PARAMS:
					logMIAer.logCommandExec(SetPanParams(APIserverId, jsonToSPanParams(jsonObj["panParams"]), &requestID), "SetPanParams");
					break;
				case ECSMSDllMsgType::GET_PAN:
					logMIAer.logCommandExec(RequestPan(APIserverId, jsonToSGetPanParams(jsonObj["panParams"]), &requestID), "RequestPan");
					break;
				default:
					logMIAer.error("command not recognized");
					break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	logMIAer.info("Service will shutdown...");

	// Join threads before exiting
	if (commandThread.joinable()) {
		commandThread.join();
	}
	if (streamThread.joinable()) {
		streamThread.join();
	}
	if (errorThread.joinable()) {
		errorThread.join();
	}
	if (realtimeThread.joinable()) {
		realtimeThread.join();
	}

	// Close the connection
	StationDisconnect();
	logMIAer.~MIAerLog();

	return static_cast<int>(interruptionCode.load());
}

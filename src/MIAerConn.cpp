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

// Vector used for the data stream output
std::vector<std::string> streamBuffer;

// Vector used for the data error output
std::vector<std::string> errorBuffer;

// Vector used for the real time output
std::vector<std::string> realtimeBuffer;

// Mutex to protect the command queue
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
	CString respMsg;
	static bool first = true;

	if (respType == ECSMSDllMsgType::GET_BIST ||
		respType == ECSMSDllMsgType::GET_BIST_RESULT)
	{
		SEquipCtrlMsg::SGetBistResp* BITEResponse = (SEquipCtrlMsg::SGetBistResp*)respdata;
		
	}
	else if (respType == ECSMSDllMsgType::GET_DIAGNOSTICS)
	{
		SEquipCtrlMsg::SGetBistResp* BITEResponse = (SEquipCtrlMsg::SGetBistResp*)respdata;
		
	}
	else
	{
	}

}
void ProcessAntListResponse(_In_ SEquipCtrlMsg::UBody* data)
{
	SEquipCtrlMsg::SAntInfoListResp* antListResponse = (SEquipCtrlMsg::SAntInfoListResp*)data;
}
void processAutoViolateResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	CString tempstr;
	SEquipCtrlMsg::SOccupancyHeader* pOccHdr = nullptr;

	if (respType == ECSMSDllMsgType::AVD_STATE_RESPONSE ||
		respType == ECSMSDllMsgType::AVD_SOLICIT_STATE_RESPONSE)
	{
		CString stateType;
		if (respType == ECSMSDllMsgType::AVD_STATE_RESPONSE)
			stateType = _T("(Unsolicited)");
		else
			stateType = _T("(Solicited)");
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		
	}
	else if (respType == ECSMSDllMsgType::AVD_FREQ_VS_CHANNEL)
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
	}
	else if (respType == ECSMSDllMsgType::AVD_FREQ_MEAS)
	{
		SEquipCtrlMsg::SAvdMeasureResult* Response = (SEquipCtrlMsg::SAvdMeasureResult*)data;
	}
	else if (respType == ECSMSDllMsgType::AVD_BW_MEAS)
	{
		SEquipCtrlMsg::SAvdMeasureResult* Response = (SEquipCtrlMsg::SAvdMeasureResult*)data;
	}
	else if (respType == ECSMSDllMsgType::AVD_STATUS)
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
	}
	else if (respType == ECSMSDllMsgType::AVD_OCC_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
	}
	else	// not AUTOVIOLATE_RESPONSE
	{
		tempstr.Format(_T("unexpected AVD message %u"),
			respType);
	}

	if (pOccHdr)
	{
		if (pOccHdr->status == ErrorCodes::SUCCESS ||
			pOccHdr->status == ErrorCodes::NOERROR_SIMULATE ||
			pOccHdr->status == 10000)
		{
			COleDateTime temp;
			temp.m_dt = pOccHdr->gpsResponse.dateTime;
			CString avdStr;
			avdStr.Format(_T("AVD %u "), respType);
			tempstr.Format(_T("  %lu %lu %lu %lu %ld"),
				pOccHdr->numTotalChannels, pOccHdr->firstChannel,
				pOccHdr->numChannels, pOccHdr->numTimeOfDays,
				pOccHdr->status);
			tempstr = avdStr + temp.Format() + tempstr;
		}
		else
		{
			tempstr.Format(_T("AVD response status %ld"),
				pOccHdr->status);
		}
	}

}
void processMeasResponse(_In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	CString tempstr;

	if (respType == ECSMSDllMsgType::VALIDATE_MEAS)
	{
		SEquipCtrlMsg::SValidateMeasurementResp* DwellResponse = (SEquipCtrlMsg::SValidateMeasurementResp*)data;
		
	}
	else if (respType == ECSMSDllMsgType::GET_MEAS)
	{
		SEquipCtrlMsg::SGetMeasResp* MeasResponse = (SEquipCtrlMsg::SGetMeasResp*)data;
	}
}
void processDemodCtrlResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	CString tempstr;

	if (respType == ECSMSDllMsgType::SET_RCVR)
	{
		SEquipCtrlMsg::SGenericResp* RcvrResponse = (SEquipCtrlMsg::SGenericResp*)data;

		tempstr.Format(_T("Receiver Control Status: %ld"), RcvrResponse->status);
	}
	else if (respType == ECSMSDllMsgType::SET_PAN_PARAMS)
	{
		SEquipCtrlMsg::SGenericResp* PanParaResponse = (SEquipCtrlMsg::SGenericResp*)data;

		tempstr.Format(_T("Pan Para Status: %ld"), PanParaResponse->status);
	}
	else if (respType == ECSMSDllMsgType::SET_AUDIO_PARAMS)
	{
		SEquipCtrlMsg::SAudioParamsResp* AudioResponse = (SEquipCtrlMsg::SAudioParamsResp*)data;

		tempstr.Format(_T("Audio Params Status: %ld channel: %lu"), AudioResponse->status, AudioResponse->channel);
	}
	else if (respType == ECSMSDllMsgType::FREE_AUDIO_CHANNEL)
	{
		SEquipCtrlMsg::SGenericResp* FreeAudioResponse = (SEquipCtrlMsg::SGenericResp*)data;

		tempstr.Format(_T("Free Audio Channel Status: %ld"), FreeAudioResponse->status);
	}
	else
	{
		//tempstr.Format(_T("unexpected Demod Control message subType %lu"),
		//	m_Response.hdr.msgSubType);
	}
}
void processPanResponse(_In_ SEquipCtrlMsg::UBody* data)
{
	//error codes:
	// 0 = no error
	// 1 = server timeout
	// 2 = Carrier signal not detected

	SEquipCtrlMsg::SGetPanResp* PanResponse = (SEquipCtrlMsg::SGetPanResp*)data;
	if (PanResponse->status == ErrorCodes::SUCCESS ||
		PanResponse->status == ErrorCodes::NOERROR_SIMULATE)
	{
		std::string strPanResp = "{\"PanResponse\":{}}";
		streamBuffer.push_back(strPanResp);
	}
	else
	{
		std::string strPanResp = "{\"PanResponse\":{\"status\": \"error\"}}";
		streamBuffer.push_back(strPanResp);
		CString tempstr;
		tempstr.Format(_T("pan response status %ld"),
			PanResponse->status);

	}
}
void processOccupancyResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	CString tempstr;

	if (respType == ECSMSDllMsgType::OCC_MSGLEN_DIST_RESPONSE)
	{
		SEquipCtrlMsg::SMsgLengthDistributionResp* Response = (SEquipCtrlMsg::SMsgLengthDistributionResp*)data;
	}
	else if (respType == ECSMSDllMsgType::OCC_SPECTRUM_RESPONSE)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
		
	}
	else if (respType == ECSMSDllMsgType::OCC_STATE_RESPONSE ||
		respType == ECSMSDllMsgType::OCC_SOLICIT_STATE_RESPONSE)
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		
	}
	else if (respType == ECSMSDllMsgType::OCC_FREQ_VS_CHANNEL)
	{
		SEquipCtrlMsg::SFrequencyVsChannelResp* Response = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
	}
	else if (respType == ECSMSDllMsgType::OCC_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
	}
	else if (respType == ECSMSDllMsgType::OCC_EFLD_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
	}
	else if (respType == ECSMSDllMsgType::OCC_TIMEOFDAY_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
	}
	else if (respType == ECSMSDllMsgType::OCC_MSGLEN_CHANNEL_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
	}
	else if (respType == ECSMSDllMsgType::OCC_EFLD_TIMEOFDAY_RESULT)
	{
		SEquipCtrlMsg::SOccResult* Response = (SEquipCtrlMsg::SOccResult*)data;
	}
	else if (respType == ECSMSDllMsgType::OCC_STATUS)
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
	}
	else if (respType == ECSMSDllMsgType::VALIDATE_OCCUPANCY)
	{
		SEquipCtrlMsg::SValidateOccupancyResp* Response = (SEquipCtrlMsg::SValidateOccupancyResp*)data;
	}
	else	// not OCCUPANCY_RESPONSE
	{
		tempstr.Format(_T("unexpected occupancy message %u"),
			respType);
	}
}
void processOccupancyDFResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	using json = nlohmann::json;
	json jsonObj;
	jsonObj["respType"] = std::to_string(respType);

	if (respType == ECSMSDllMsgType::OCCDF_STATE_RESPONSE ||
		respType == ECSMSDllMsgType::OCCDF_SOLICIT_STATE_RESPONSE)
	{
		SEquipCtrlMsg::SStateResp* Response = (SEquipCtrlMsg::SStateResp*)data;
		jsonObj["SStateResp"]["completionTime"] = Response->completionTime;
		jsonObj["SStateResp"]["state"] = Response->state;
	}
	else if (respType == ECSMSDllMsgType::OCCDF_FREQ_VS_CHANNEL)
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
	else if (respType == ECSMSDllMsgType::OCCDF_SCANDF_VS_CHANNEL)
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
	else if (respType == ECSMSDllMsgType::OCCDF_STATUS)
	{
		SEquipCtrlMsg::SEquipTaskStatusResp* Response = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
		jsonObj["SEquipTaskStatusResp"]["dateTime"] = Response->dateTime;
		jsonObj["SEquipTaskStatusResp"]["key"] = Response->key;
		jsonObj["SEquipTaskStatusResp"]["status"] = Response->status;
		jsonObj["SEquipTaskStatusResp"]["taskId"] = Response->taskId;
	}
	else	// not OCCUPANCYDF_RESPONSE
	{
		//tempstr.Format(_T("unexpected occupancyDF message %u"),
		//	respType);
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
	std::string strData = std::string(reinterpret_cast<char*>(data), sizeof(data));
	std::string strJson = "{\"serverId\":" + std::to_string(serverId) + ", \"respType\":\"" + std::to_string(respType) + "\", \"data\":\""+strData+"\"}";
	realtimeBuffer.push_back(strData);
	logMIAer.info("OnData received with type " + respType);
	logMIAer.info("OnData received server ID " + serverId);
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
				std::to_string(commandQueue.size()) +
				mcs::Form::SEP;
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
// ! Need to fix the mutex lock, moving it to functions which manipulate the commandQueue outside the scope of the main loop
			std::lock_guard<std::mutex> lock(MCCommandMutex);
			std::string command = commandQueue.back();
			commandQueue.pop_back();
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

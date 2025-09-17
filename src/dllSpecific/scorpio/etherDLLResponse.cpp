
/**
* @file etherDLLResponse.cpp
*
* @brief Code file for functions that convert Scorpio DLL responses to JSON
* * Include callback functions used by the Scorpio API
* * and additional function for converting DLL data structures to JSON
* * some convertions will require additional data processing
* * which will be handled by functions in dllSpecific/scorpio/etherDLLDataProcess.hpp
*
* * @author fslobao
* * @date 2025-09-17
* * @version 1.0
*
* * @note Requires C++11 or later
* * @note Uses nlohmann/json library for JSON handling
*
* * * Dependencies:
* * - nlohmann/json.hpp
**/

// ----------------------------------------------------------------------
#pragma once

// Include core EtherDLL libraries
#include "EtherDLLConstants.hpp"
#include "EtherDLLUtils.hpp"

// Include DLL specific libraries
#include "etherDLLResponse.hpp"
#include "etherDLLDataProcess.hpp"
#include "etherDLLCodes.hpp"

// Include provided DLL libraries
#include "ScorpioAPITypes.h"
#include "EquipCtrlMsg.h"
#include "SmsRealtimeMsg.h"

// Include general C++ libraries
#include <string>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <limits>
#include <locale>
#include <codecvt>

// Include project libraries
#include <nlohmann/json.hpp>

// For convenience
using json = nlohmann::json;


// ----------------------------------------------------------------------
/*
    Global variables related to the API
*/

CLoopbackCapture loopbackCapture;



//
// Convert response of BIT command in JSON
//
json processBITEResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* respdata)
{
	json jsonObj;

	jsonObj["respType"] = int(respType);

	switch (respType) 
    {
        case ECSMSDllMsgType::GET_BIST: // BIST_RESPONSE, GET_BIST_RESPONSE, GET_DIAGNOSTICS_RESPONSE, GET_BIST_RESULT_RESPONSE
		case ECSMSDllMsgType::GET_BIST_RESULT:
        case ECSMSDllMsgType::GET_DIAGNOSTICS:
            {
                SEquipCtrlMsg::SGetBistResp* BITEResponse = (SEquipCtrlMsg::SGetBistResp*)respdata;
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

                jsonObj["BIST"]["last"] = bool(BITEResponse->last);
				jsonObj["BIST"]["result"] = BISTResultToString(BITEResponse->result);
                std::string text = converter.to_bytes(BITEResponse->text, BITEResponse->text + BITEResponse->textLen);
				jsonObj["BIST"]["text"] = text;
                // jsonObj["BIST"]["textLen"] = BITEResponse->textLen; // Not necessary to include textLen in the JSON
            }
            break;

        default:
            jsonObj["BIST"]["Error"] = std::string("Unexpected processBITEResponse type ") + ECSMSDllMsgTypeToString(respType);
            break;
    }
	
	return jsonObj;
}

//
// Convert response of type GET_ANT_LIST_INFO in JSON
//
json ProcessAntListResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	SEquipCtrlMsg::SAntInfoListResp* antListResponse = (SEquipCtrlMsg::SAntInfoListResp*)data;
	json jsonObj;

    jsonObj["Type"] = static_cast<int>(respType);

    jsonObj["Equipment"]["antenna"]["numAntennas"] = antListResponse->numAnt;
    for (size_t i = 0; i < antListResponse->numAnt; ++i) {
        jsonObj["Equipment"]["antenna"][i]["Name"] = antListResponse->ant[i].antName;
        jsonObj["Equipment"]["antenna"][i]["type"] = eAntToString(antListResponse->ant[i].ant);
        jsonObj["Equipment"]["antenna"][i]["lowFrequency"] = antListResponse->ant[i].lowFreq.internal;
        jsonObj["Equipment"]["antenna"][i]["highFrequency"] = antListResponse->ant[i].highFreq.internal;
	    jsonObj["Equipment"]["antenna"][i]["azimuthStart"] = antListResponse->ant[i].azimuthStart;
	    jsonObj["Equipment"]["antenna"][i]["azimuthStep"] = antListResponse->ant[i].azimuthStep;
	    jsonObj["Equipment"]["antenna"][i]["azimuthStop"] = antListResponse->ant[i].azimuthStop;
	    jsonObj["Equipment"]["antenna"][i]["azimuthTolerance"] = antListResponse->ant[i].azimuthTolerance;
	    jsonObj["Equipment"]["antenna"][i]["aziOrientation"] = antListResponse->ant[i].aziOrientation;
	    jsonObj["Equipment"]["antenna"][i]["elevationStart"] = antListResponse->ant[i].elevationStart;
        jsonObj["Equipment"]["antenna"][i]["elevationStep"] = antListResponse->ant[i].elevationStep;
	    jsonObj["Equipment"]["antenna"][i]["elevationStop"] = antListResponse->ant[i].elevationStop;
	    jsonObj["Equipment"]["antenna"][i]["elevationTolerance"] = antListResponse->ant[i].elevationTolerance;
		jsonObj["Equipment"]["antenna"][i]["polarization"] = eAntPolToString(antListResponse->ant[i].polarization);
	    jsonObj["Equipment"]["antenna"][i]["rotatorId"] = antListResponse->ant[i].rotatorId;
    }
	return jsonObj;
}

//
// Convert response of types AVD_FREQ_VS_CHANNEL, AVD_OCC_CHANNEL_RESULT, AVD_FREQ_MEAS, AVD_BW_MEAS, AVD_SOLICIT_STATE_RESPONSE, AVD_STATE_RESPONSE and AVD_STATUS  in JSON
//
json processAutoViolateResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	SEquipCtrlMsg::SOccupancyHeader* pOccHdr = nullptr;
	json jsonObj;

    jsonObj["respType"] = static_cast<int>(respType);

	switch (respType)
    {
        case ECSMSDllMsgType::AVD_STATE_RESPONSE:
		case ECSMSDllMsgType::AVD_SOLICIT_STATE_RESPONSE:
            {
                SEquipCtrlMsg::SStateResp* AVDResponse = (SEquipCtrlMsg::SStateResp*)data;

                jsonObj["SStateResp"]["completionTime"] = double(AVDResponse->completionTime);
				jsonObj["SStateResp"]["state"] = eStateRespToString(AVDResponse->state);
            }
            break;

        case ECSMSDllMsgType::AVD_FREQ_VS_CHANNEL:
            {
                SEquipCtrlMsg::SFrequencyVsChannelResp* AVDResponse = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
                jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = AVDResponse->frequencies->internal;
                jsonObj["SFrequencyVsChannelResp"]["hostName"] = AVDResponse->hostName;
                jsonObj["SFrequencyVsChannelResp"]["numBands"] = AVDResponse->numBands;
                jsonObj["SFrequencyVsChannelResp"]["numChannels"] = AVDResponse->numChannels;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = AVDResponse->occHdr.firstChannel;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["dateTime"] = AVDResponse->occHdr.gpsResponse.dateTime;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["latitude"] = AVDResponse->occHdr.gpsResponse.latitude;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["longitude"] = AVDResponse->occHdr.gpsResponse.longitude;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.accuracy;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.antenna;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.batVolt;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.lockHist;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.mode;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.noGps;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.notTested;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.numSats;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.nvRam;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.oscVolt;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.pllSynth;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.receiver;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.satLock;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr1;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr2;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timSrce;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.tracking;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = AVDResponse->occHdr.numChannels;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = AVDResponse->occHdr.numTimeOfDays;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = AVDResponse->occHdr.numTotalChannels;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = AVDResponse->occHdr.status;
                jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = AVDResponse->occPrimaryThreshold;
                jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = AVDResponse->occSecondaryThreshold;
                jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = AVDResponse->saveIntermediateData;
                jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = AVDResponse->selectedAntenna;
                jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = AVDResponse->useSecondaryThreshold;
            }
            break;

        case ECSMSDllMsgType::AVD_FREQ_MEAS:
            {
                SEquipCtrlMsg::SAvdMeasureResult* AVDResponse = (SEquipCtrlMsg::SAvdMeasureResult*)data;
// TODO #16: Check the origin of the loop total. Why 1000?
                for (size_t i = 0; i < 1000; ++i) {
                    jsonObj["SAvdMeasureResult"]["measData"][i]["result"] = AVDResponse->measData[i].result;
                    jsonObj["SAvdMeasureResult"]["measData"][i]["stdDev"] = AVDResponse->measData[i].stdDev;
                }
                jsonObj["SAvdMeasureResult"]["occHdr"]["firstChannel"] = AVDResponse->occHdr.firstChannel;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["dateTime"] = AVDResponse->occHdr.gpsResponse.dateTime;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["latitude"] = AVDResponse->occHdr.gpsResponse.latitude;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["longitude"] = AVDResponse->occHdr.gpsResponse.longitude;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.accuracy;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.antenna;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.batVolt;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.lockHist;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.mode;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.noGps;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.notTested;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.numSats;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.nvRam;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.oscVolt;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.pllSynth;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.receiver;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.satLock;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr1;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr2;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timSrce;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.tracking;
                jsonObj["SAvdMeasureResult"]["occHdr"]["numChannels"] = AVDResponse->occHdr.numChannels;
                jsonObj["SAvdMeasureResult"]["occHdr"]["numTimeOfDays"] = AVDResponse->occHdr.numTimeOfDays;
                jsonObj["SAvdMeasureResult"]["occHdr"]["numTotalChannels"] = AVDResponse->occHdr.numTotalChannels;
                jsonObj["SAvdMeasureResult"]["occHdr"]["status"] = AVDResponse->occHdr.status;
            }
            break;

        case ECSMSDllMsgType::AVD_BW_MEAS:
            {
                SEquipCtrlMsg::SAvdMeasureResult* AVDResponse = (SEquipCtrlMsg::SAvdMeasureResult*)data;
                for (size_t i = 0; i < 1000; ++i) {
                    jsonObj["SAvdMeasureResult"]["measData"][i]["result"] = AVDResponse->measData[i].result;
                    jsonObj["SAvdMeasureResult"]["measData"][i]["stdDev"] = AVDResponse->measData[i].stdDev;
                }
                jsonObj["SAvdMeasureResult"]["occHdr"]["firstChannel"] = AVDResponse->occHdr.firstChannel;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["dateTime"] = AVDResponse->occHdr.gpsResponse.dateTime;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["latitude"] = AVDResponse->occHdr.gpsResponse.latitude;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["longitude"] = AVDResponse->occHdr.gpsResponse.longitude;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.accuracy;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.antenna;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.batVolt;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.lockHist;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.mode;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.noGps;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.notTested;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.numSats;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.nvRam;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.oscVolt;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.pllSynth;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.receiver;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.satLock;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr1;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr2;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timSrce;
                jsonObj["SAvdMeasureResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.tracking;
                jsonObj["SAvdMeasureResult"]["occHdr"]["numChannels"] = AVDResponse->occHdr.numChannels;
                jsonObj["SAvdMeasureResult"]["occHdr"]["numTimeOfDays"] = AVDResponse->occHdr.numTimeOfDays;
                jsonObj["SAvdMeasureResult"]["occHdr"]["numTotalChannels"] = AVDResponse->occHdr.numTotalChannels;
                jsonObj["SAvdMeasureResult"]["occHdr"]["status"] = AVDResponse->occHdr.status;
            }
            break;

        case ECSMSDllMsgType::AVD_STATUS:
            {
                SEquipCtrlMsg::SEquipTaskStatusResp* AVDResponse = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
                jsonObj["SEquipTaskStatusResp"]["dateTime"] = AVDResponse->dateTime;
                jsonObj["SEquipTaskStatusResp"]["key"] = AVDResponse->key;
                jsonObj["SEquipTaskStatusResp"]["status"] = AVDResponse->status;
                jsonObj["SEquipTaskStatusResp"]["taskId"] = AVDResponse->taskId;
            }
            break;

        case ECSMSDllMsgType::AVD_OCC_CHANNEL_RESULT:
            {
                SEquipCtrlMsg::SOccResult* AVDResponse = (SEquipCtrlMsg::SOccResult*)data;
                jsonObj["SOccResult"]["occHdr"]["firstChannel"] = AVDResponse->occHdr.firstChannel;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["dateTime"] = AVDResponse->occHdr.gpsResponse.dateTime;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["latitude"] = AVDResponse->occHdr.gpsResponse.latitude;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["longitude"] = AVDResponse->occHdr.gpsResponse.longitude;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["accuracy"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.accuracy;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["antenna"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.antenna;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["batVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.batVolt;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["lockHist"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.lockHist;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["mode"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.mode;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["noGps"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.noGps;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["notTested"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.notTested;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["numSats"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.numSats;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["nvRam"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.nvRam;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["oscVolt"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.oscVolt;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["pllSynth"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.pllSynth;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["receiver"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.receiver;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["satLock"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.satLock;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr1"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr1;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timErr2"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timErr2;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["timSrce"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.timSrce;
                jsonObj["SOccResult"]["occHdr"]["gpsResponse"]["status"]["tracking"] = (unsigned long)AVDResponse->occHdr.gpsResponse.status.tracking;
                jsonObj["SOccResult"]["occHdr"]["numChannels"] = AVDResponse->occHdr.numChannels;
                jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = AVDResponse->occHdr.numTimeOfDays;
                jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = AVDResponse->occHdr.numTotalChannels;
                jsonObj["SOccResult"]["occHdr"]["status"] = AVDResponse->occHdr.status;
                jsonObj["SOccResult"]["resultData"]["avg"] = AVDResponse->resultData->avg;
                jsonObj["SOccResult"]["resultData"]["avg"] = AVDResponse->resultData->max;
            }
            break;

        default:
			jsonObj["Error"] = std::string("Unexpected processAutoViolateResponse type ") + ECSMSDllMsgTypeToString(respType);
            break;
    }
	
	return jsonObj;
}

//
// Convert response of types GET_MEAS and VALIDATE_MEAS in JSON
//
json processMeasResponse(_In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	json jsonObj;

	jsonObj["respType"] = static_cast<int>(respType);

	switch (respType)
    {
        case ECSMSDllMsgType::VALIDATE_MEAS:
            {
                SEquipCtrlMsg::SValidateMeasurementResp* MeasResponse = (SEquipCtrlMsg::SValidateMeasurementResp*)data;
                jsonObj["SValidateMeasurementResp"]["bwDwellTime"] = MeasResponse->bwDwellTime;
                jsonObj["SValidateMeasurementResp"]["dfDwellTime"] = MeasResponse->dfDwellTime;
                jsonObj["SValidateMeasurementResp"]["fieldStrengthDwellTime"] = MeasResponse->fieldStrengthDwellTime;
                jsonObj["SValidateMeasurementResp"]["freqDwellTime"] = MeasResponse->freqDwellTime;
                jsonObj["SValidateMeasurementResp"]["modulationDwellTime"] = MeasResponse->modulationDwellTime;
                jsonObj["SValidateMeasurementResp"]["status"] = MeasResponse->status;
                jsonObj["SValidateMeasurementResp"]["totalTime"] = MeasResponse->totalTime;
            }
            break;
	    case ECSMSDllMsgType::GET_MEAS:
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
            break;
        default:
			jsonObj["Error"] = std::string("Unexpected processMeasResponse type ") + ECSMSDllMsgTypeToString(respType);
            break;
    }

	return jsonObj;
}

//
// Convert response of types SET_PAN_PARAMS, SET_AUDIO_PARAMS and FREE_AUDIO_CHANNEL in JSON
//
json processDemodCtrlResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	json jsonObj;

	jsonObj["respType"] = static_cast<int>(respType);

	switch (respType)
    {
        case ECSMSDllMsgType::SET_RCVR:
            {
                SEquipCtrlMsg::SGenericResp* RcvrResponse = (SEquipCtrlMsg::SGenericResp*)data;
                jsonObj["SGenericResp"]["status"] = RcvrResponse->status;
            }
            break;

	    case ECSMSDllMsgType::SET_PAN_PARAMS:
            {
                SEquipCtrlMsg::SGenericResp* PanParaResponse = (SEquipCtrlMsg::SGenericResp*)data;
                jsonObj["SGenericResp"]["status"] = PanParaResponse->status;
            }
            break;

	    case ECSMSDllMsgType::SET_AUDIO_PARAMS:
            {
                SEquipCtrlMsg::SAudioParamsResp* AudioResponse = (SEquipCtrlMsg::SAudioParamsResp*)data;
                jsonObj["SAudioParamsResp"]["channel"] = AudioResponse->channel;
                jsonObj["SAudioParamsResp"]["ipAddress"] = AudioResponse->ipAddress;
                jsonObj["SAudioParamsResp"]["status"] = AudioResponse->status;
            }
            break;

	    case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
            {
                SEquipCtrlMsg::SGenericResp* FreeAudioResponse = (SEquipCtrlMsg::SGenericResp*)data;
                jsonObj["SGenericResp"]["status"] = FreeAudioResponse->status;
            }
            break;

        default:
			jsonObj["Error"] = std::string("Unexpected processDemodCtrlResponse type ") + ECSMSDllMsgTypeToString(respType);
            break;
    }
	
	return jsonObj;
}

//
// Convert response of type GET_PAN in JSON
//
json processPanResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	json jsonObj;

	jsonObj["respType"] = static_cast<int>(respType);

	SEquipCtrlMsg::SGetPanResp* PanResponse = (SEquipCtrlMsg::SGetPanResp*)data;

    jsonObj["measure"]["status"] = PanResponse->status;
    jsonObj["measure"]["dateTime"] = COleTimeToIsoStr(PanResponse->dateTime);
    jsonObj["measure"]["powerDbm"] = PanResponse->powerDbm;
    jsonObj["setting"]["attenuation"] = PanResponse->rcvrAtten;

    auto spectrumInfo = calculateSpectrumInfo(PanResponse);
    size_t sweepByteLen = static_cast<size_t>(PanResponse->numBins) * sizeof(float);

    jsonObj["spectrum"]["numBins"] = PanResponse->numBins;
    jsonObj["spectrum"]["startFrequency"] = spectrumInfo.startFrequency;
	jsonObj["spectrum"]["stopFrequency"] = spectrumInfo.stopFrequency;
	jsonObj["spectrum"]["frequencyUnit"] = "MHz";
	jsonObj["spectrum"]["binSize"] = spectrumInfo.binSize;
	jsonObj["spectrum"]["binSizeUnit"] = "Hz";
    jsonObj["spectrum"]["sweepData"] = base64_encode(
        parsedBinData(PanResponse->binData, PanResponse->numBins),
        static_cast<unsigned int>(sweepByteLen)
    );
    jsonObj["spectrum"]["conversionFactorForFS"] = PanResponse->conversionFactorForFS;

	jsonObj["demod"]["nActiveAudioChannels"] = PanResponse->nActiveAudioChannels;
    jsonObj["demod"]["audioPower"]["active"] = PanResponse->audioPower->active;
    for (size_t i = 0; i < PanResponse->nActiveAudioChannels; ++i) {
        jsonObj["demod"]["audioPower"][i]["powerdBm"] = PanResponse->audioPower[i].powerdBm;
    }

	return jsonObj;

}

//
// Convert response of types OCC_MSGLEN_DIST_RESPONSE, OCC_FREQ_VS_CHANNEL, OCC_CHANNEL_RESULT, OCC_STATUS, OCC_STATE_RESPONSE, OCC_SOLICIT_STATE_RESPONSE, OCC_SPECTRUM_RESPONSE, OCC_TIMEOFDAY_RESULT, OCC_EFLD_CHANNEL_RESULT, OCC_MSGLEN_CHANNEL_RESULT, OCC_EFLD_TIMEOFDAY_RESULT in JSON
//
json processOccupancyResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	json jsonObj;

	jsonObj["respType"] = static_cast<int>(respType);

    switch (respType)
    {
        case ECSMSDllMsgType::OCC_MSGLEN_DIST_RESPONSE:
            {
                SEquipCtrlMsg::SMsgLengthDistributionResp* OCCResponse = (SEquipCtrlMsg::SMsgLengthDistributionResp*)data;

                jsonObj["SMsgLengthDistributionResp"]["histData"]["channel"] = OCCResponse->histData->channel;
                jsonObj["SMsgLengthDistributionResp"]["histData"]["length"] = OCCResponse->histData->length;
                jsonObj["SMsgLengthDistributionResp"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
            }
            break;

        case ECSMSDllMsgType::OCC_SPECTRUM_RESPONSE:
            {
                SEquipCtrlMsg::SOccResult* OCCResponse = (SEquipCtrlMsg::SOccResult*)data;

                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
                jsonObj["SOccResult"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)OCCResponse->occHdr.numChannels;
                jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)OCCResponse->occHdr.numTimeOfDays;
                jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)OCCResponse->occHdr.numTotalChannels;
                jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)OCCResponse->occHdr.status;
                jsonObj["SOccResult"]["resultData"] = (unsigned long)OCCResponse->resultData;
            }
            break;

        case ECSMSDllMsgType::OCC_STATE_RESPONSE: /* Fallback to apply the same processing to both cases */
        case ECSMSDllMsgType::OCC_SOLICIT_STATE_RESPONSE:
            {
                SEquipCtrlMsg::SStateResp* OCCResponse = (SEquipCtrlMsg::SStateResp*)data;
                jsonObj["SStateResp"]["completionTime"] = (unsigned long)OCCResponse->completionTime;
                jsonObj["SStateResp"]["state"] = eStateRespToString(OCCResponse->state);
            }
            break;

        case ECSMSDllMsgType::OCC_FREQ_VS_CHANNEL:
            {
                SEquipCtrlMsg::SFrequencyVsChannelResp* OCCResponse = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;

                jsonObj["SFrequencyVsChannelResp"]["frequencies"]["internal"] = OCCResponse->frequencies->internal;
                jsonObj["SFrequencyVsChannelResp"]["hostName"] = OCCResponse->hostName;
                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
                jsonObj["SFrequencyVsChannelResp"]["numBands"] = OCCResponse->numBands;
                jsonObj["SFrequencyVsChannelResp"]["numChannels"] = OCCResponse->numChannels;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numChannels"] = OCCResponse->occHdr.numChannels;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTimeOfDays"] = OCCResponse->occHdr.numTimeOfDays;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["numTotalChannels"] = OCCResponse->occHdr.numTotalChannels;
                jsonObj["SFrequencyVsChannelResp"]["occHdr"]["status"] = OCCResponse->occHdr.status;
                jsonObj["SFrequencyVsChannelResp"]["occPrimaryThreshold"] = OCCResponse->occPrimaryThreshold;
                jsonObj["SFrequencyVsChannelResp"]["occSecondaryThreshold"] = OCCResponse->occSecondaryThreshold;
                jsonObj["SFrequencyVsChannelResp"]["saveIntermediateData"] = OCCResponse->saveIntermediateData;
                jsonObj["SFrequencyVsChannelResp"]["selectedAntenna"] = OCCResponse->selectedAntenna;
                jsonObj["SFrequencyVsChannelResp"]["useSecondaryThreshold"] = OCCResponse->useSecondaryThreshold;
            }
            break;

        case ECSMSDllMsgType::OCC_CHANNEL_RESULT:
            {
                SEquipCtrlMsg::SOccResult* OCCResponse = (SEquipCtrlMsg::SOccResult*)data;

                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
                jsonObj["SOccResult"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)OCCResponse->occHdr.numChannels;
                jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)OCCResponse->occHdr.numTimeOfDays;
                jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)OCCResponse->occHdr.numTotalChannels;
                jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)OCCResponse->occHdr.status;
                jsonObj["SOccResult"]["resultData"] = (unsigned long)OCCResponse->resultData;
            }
            break;

        case ECSMSDllMsgType::OCC_EFLD_CHANNEL_RESULT:
            {
                SEquipCtrlMsg::SOccResult* OCCResponse = (SEquipCtrlMsg::SOccResult*)data;

                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
                jsonObj["SOccResult"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)OCCResponse->occHdr.numChannels;
                jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)OCCResponse->occHdr.numTimeOfDays;
                jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)OCCResponse->occHdr.numTotalChannels;
                jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)OCCResponse->occHdr.status;
                jsonObj["SOccResult"]["resultData"] = (unsigned long)OCCResponse->resultData;
            }
            break;

        case ECSMSDllMsgType::OCC_TIMEOFDAY_RESULT:
            {
                SEquipCtrlMsg::SOccResult* OCCResponse = (SEquipCtrlMsg::SOccResult*)data;

                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
                jsonObj["SOccResult"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)OCCResponse->occHdr.numChannels;
                jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)OCCResponse->occHdr.numTimeOfDays;
                jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)OCCResponse->occHdr.numTotalChannels;
                jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)OCCResponse->occHdr.status;
                jsonObj["SOccResult"]["resultData"] = (unsigned long)OCCResponse->resultData;
            }
            break;

        case ECSMSDllMsgType::OCC_MSGLEN_CHANNEL_RESULT:
            {
                SEquipCtrlMsg::SOccResult* OCCResponse = (SEquipCtrlMsg::SOccResult*)data;

                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
                jsonObj["SOccResult"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)OCCResponse->occHdr.numChannels;
                jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)OCCResponse->occHdr.numTimeOfDays;
                jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)OCCResponse->occHdr.numTotalChannels;
                jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)OCCResponse->occHdr.status;
                jsonObj["SOccResult"]["resultData"] = (unsigned long)OCCResponse->resultData;
            }
            break;

        case ECSMSDllMsgType::OCC_EFLD_TIMEOFDAY_RESULT:
            {
                SEquipCtrlMsg::SOccResult* OCCResponse = (SEquipCtrlMsg::SOccResult*)data;

                jsonObj["site"] = ProcessGpsData(&(OCCResponse->occHdr.gpsResponse));
                jsonObj["SOccResult"]["occHdr"]["firstChannel"] = OCCResponse->occHdr.firstChannel;
                jsonObj["SOccResult"]["occHdr"]["numChannels"] = (unsigned long)OCCResponse->occHdr.numChannels;
                jsonObj["SOccResult"]["occHdr"]["numTimeOfDays"] = (unsigned long)OCCResponse->occHdr.numTimeOfDays;
                jsonObj["SOccResult"]["occHdr"]["numTotalChannels"] = (unsigned long)OCCResponse->occHdr.numTotalChannels;
                jsonObj["SOccResult"]["occHdr"]["status"] = (unsigned long)OCCResponse->occHdr.status;
                jsonObj["SOccResult"]["resultData"] = (unsigned long)OCCResponse->resultData;
            }
            break;

        case ECSMSDllMsgType::OCC_STATUS:
            {
                SEquipCtrlMsg::SEquipTaskStatusResp* OCCResponse = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
                jsonObj["SEquipTaskStatusResp"]["dateTime"] = OCCResponse->dateTime;
                jsonObj["SEquipTaskStatusResp"]["key"] = OCCResponse->key;
                jsonObj["SEquipTaskStatusResp"]["status"] = OCCResponse->status;
                jsonObj["SEquipTaskStatusResp"]["taskId"] = OCCResponse->taskId;
            }
            break;

        case ECSMSDllMsgType::VALIDATE_OCCUPANCY:
            {
                SEquipCtrlMsg::SValidateOccupancyResp* OCCResponse = (SEquipCtrlMsg::SValidateOccupancyResp*)data;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["ant"] = OCCResponse->occCmd.ant;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["channelBandwidth"]["internal"] = OCCResponse->occCmd.band[0].channelBandwidth.internal;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["exclude"] = OCCResponse->occCmd.band[0].exclude;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["highFrequency"]["internal"] = OCCResponse->occCmd.band[0].highFrequency.internal;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["lowFrequency"]["internal"] = OCCResponse->occCmd.band[0].lowFrequency.internal;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["horizPol"] = (unsigned long)OCCResponse->occCmd.band[0].sType.signalType.horizPol;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["narrow"] = (unsigned long)OCCResponse->occCmd.band[0].sType.signalType.narrow;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["unused0"] = (unsigned long)OCCResponse->occCmd.band[0].sType.signalType.unused0;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["band"]["sType"]["signalType"]["unused1"] = (unsigned long)OCCResponse->occCmd.band[0].sType.signalType.unused1;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["confidenceLevel"] = OCCResponse->occCmd.confidenceLevel;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["desiredAccuracy"] = OCCResponse->occCmd.desiredAccuracy;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["durationMethod"] = OCCResponse->occCmd.durationMethod;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["measurementTime"] = OCCResponse->occCmd.measurementTime;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["numBands"] = OCCResponse->occCmd.numBands;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["occPrimaryThreshold"] = OCCResponse->occCmd.occPrimaryThreshold;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["occSecondaryThreshold"] = OCCResponse->occCmd.occSecondaryThreshold;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["occupancyMinGap"] = OCCResponse->occCmd.occupancyMinGap;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["eFieldVsChannel"] = OCCResponse->occCmd.output.eFieldVsChannel;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["msglengthDistribution"] = OCCResponse->occCmd.output.msglengthDistribution;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["msglengthVsChannel"] = OCCResponse->occCmd.output.msglengthVsChannel;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["adjustAgcThres"] = (unsigned char)OCCResponse->occCmd.output.occupancyFlag.flag.adjustAgcThres;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["eFieldVsTOD"] = (unsigned char)OCCResponse->occCmd.output.occupancyFlag.flag.eFieldVsTOD;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["gainMode"] = (unsigned char)OCCResponse->occCmd.output.occupancyFlag.flag.gainMode;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["spurTest"] = (unsigned char)OCCResponse->occCmd.output.occupancyFlag.flag.spurTest;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["flag"]["unused"] = (unsigned char)OCCResponse->occCmd.output.occupancyFlag.flag.unused;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyFlag"]["zeroFlag"] = (unsigned char)OCCResponse->occCmd.output.occupancyFlag.zeroFlag;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyVsChannel"] = (unsigned char)OCCResponse->occCmd.output.occupancyVsChannel;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["occupancyVsTimeOfDay"] = (unsigned char)OCCResponse->occCmd.output.occupancyVsTimeOfDay;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["spectrogram"] = (unsigned char)OCCResponse->occCmd.output.spectrogram;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["output"]["timegram"] = (unsigned char)OCCResponse->occCmd.output.timegram;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["saveIntermediateData"] = OCCResponse->occCmd.saveIntermediateData;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["storageTime"] = OCCResponse->occCmd.storageTime;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["thresholdMethod"] = OCCResponse->occCmd.thresholdMethod;
                jsonObj["SValidateOccupancyResp"]["occCmd"]["useSecondaryThreshold"] = OCCResponse->occCmd.useSecondaryThreshold;
                jsonObj["SValidateOccupancyResp"]["status"] = OCCResponse->status;
            }
            break;

        default:
			jsonObj["error"] = std::string("Unexpected processOccupancyResponse type ") + ECSMSDllMsgTypeToString(respType);
            break;
    }
	return jsonObj;
}

//
// Convert response of types OCCDF_FREQ_VS_CHANNEL, OCCDF_SCANDF_VS_CHANNEL, OCCDF_STATUS, OCCDF_STATE_RESPONSE and OCCDF_SOLICIT_STATE_RESPONSE in JSON
//
json processOccupancyDFResponse(_In_ ECSMSDllMsgType respType, _In_ SEquipCtrlMsg::UBody* data)
{
	json jsonObj;

	jsonObj["respType"] = static_cast<int>(respType);

    switch (respType)
    {
        case ECSMSDllMsgType::OCCDF_STATE_RESPONSE: /* Fall through to apply the same processing to both cases */
        case ECSMSDllMsgType::OCCDF_SOLICIT_STATE_RESPONSE:
            {
                //SEquipCtrlMsg::SStateResp* OCCDFResponse = (SEquipCtrlMsg::SStateResp*)data;
			    SEquipCtrlMsg::SStateResp* OCCDFResponse = reinterpret_cast<SEquipCtrlMsg::SStateResp*>(data);

                jsonObj["SStateResp"]["completionTime"] = OCCDFResponse->completionTime;
				jsonObj["SStateResp"]["state"] = eStateRespToString(OCCDFResponse->state);
            }
            break;

	    case ECSMSDllMsgType::OCCDF_FREQ_VS_CHANNEL:
            {
                //SEquipCtrlMsg::SFrequencyVsChannelResp* OCCDFResponse = (SEquipCtrlMsg::SFrequencyVsChannelResp*)data;
                SEquipCtrlMsg::SFrequencyVsChannelResp* OCCDFResponse = reinterpret_cast<SEquipCtrlMsg::SFrequencyVsChannelResp*>(data);

                std::vector<float> freqVsChanData(OCCDFResponse->occHdr.numTotalChannels);
                float maxValue = std::numeric_limits<float>::max();
				float minValue = -maxValue;

                int j = (int)OCCDFResponse->occHdr.firstChannel;
                for (int i = 0; i < int(OCCDFResponse->occHdr.numChannels); i++, j++)
				{
                    freqVsChanData[j] = static_cast<float>(Units::Frequency(OCCDFResponse->frequencies[i]).Hz<double>() / mcs::MHZ_MULTIPLIER);
                    if (freqVsChanData[j] < minValue) minValue = freqVsChanData[j];
                    if (freqVsChanData[j] > maxValue) maxValue = freqVsChanData[j];
				}

                jsonObj["site"] = ProcessGpsData(&(OCCDFResponse->occHdr.gpsResponse));

                jsonObj["equipment"]["hostName"] = OCCDFResponse->hostName;
                jsonObj["equipment"]["selectedAntenna"] = eAntToString(OCCDFResponse->selectedAntenna);

                jsonObj["channel"]["Occupancy"]["status"] = eErrorCodeToString(OCCDFResponse->occHdr.status);
				jsonObj["channel"]["Occupancy"]["firstChannel"] = OCCDFResponse->occHdr.firstChannel;
                jsonObj["channel"]["Occupancy"]["lastChannel"] = j;
				jsonObj["channel"]["Occupancy"]["numChannels"] = OCCDFResponse->occHdr.numChannels;
                jsonObj["channel"]["Occupancy"]["numTotalChannels"] = OCCDFResponse->occHdr.numTotalChannels;
				jsonObj["channel"]["Occupancy"]["numTimeOfDays"] = OCCDFResponse->occHdr.numTimeOfDays;
				jsonObj["channel"]["Occupancy"]["numBands"] = OCCDFResponse->numBands;
				jsonObj["channel"]["Occupancy"]["maxValue"] = maxValue;
				jsonObj["channel"]["Occupancy"]["minValue"] = minValue;
                jsonObj["channel"]["Occupancy"]["Unit"] =  "MHz";
				jsonObj["channel"]["Occupancy"]["frequencyData"] = freqVsChanData;

                jsonObj["settings"]["primaryThreshold"]["absolute"] = OCCDFResponse->occPrimaryThreshold[0];
                jsonObj["settings"]["primaryThreshold"]["aboveNoise"] = OCCDFResponse->occPrimaryThreshold[1];
                jsonObj["settings"]["secondaryThreshold"]["absolute"] = OCCDFResponse->occSecondaryThreshold[0];
                jsonObj["settings"]["secondaryThreshold"]["aboveNoise"] = OCCDFResponse->occSecondaryThreshold[1];
                jsonObj["settings"]["saveIntermediateData"] = bool(OCCDFResponse->saveIntermediateData);
                jsonObj["settings"]["useSecondaryThreshold"] = bool(OCCDFResponse->useSecondaryThreshold);
            }
            break;

        case ECSMSDllMsgType::OCCDF_SCANDF_VS_CHANNEL:
            {
                // SEquipCtrlMsg::SScanDfVsChannelResp* OCCDFResponse = (SEquipCtrlMsg::SScanDfVsChannelResp*)data;
			    SEquipCtrlMsg::SScanDfVsChannelResp* OCCDFResponse = reinterpret_cast<SEquipCtrlMsg::SScanDfVsChannelResp*>(data);

                std::vector<unsigned long> chanData(OCCDFResponse->occHdr.numTotalChannels);

                std::copy(OCCDFResponse->scanDfData,
                    OCCDFResponse->scanDfData + OCCDFResponse->occHdr.numChannels,
                    chanData.begin() + OCCDFResponse->occHdr.firstChannel);

                jsonObj["site"] = ProcessGpsData(&(OCCDFResponse->occHdr.gpsResponse));

                jsonObj["channel"]["Occupancy"]["data"] = chanData;
                
                jsonObj["channel"]["Occupancy"]["status"] = eErrorCodeToString(OCCDFResponse->occHdr.status); 
                jsonObj["channel"]["Occupancy"]["firstChannel"] = OCCDFResponse->occHdr.firstChannel;
                jsonObj["channel"]["Occupancy"]["numChannels"] = OCCDFResponse->occHdr.numChannels;
                jsonObj["channel"]["Occupancy"]["numTotalChannels"] = OCCDFResponse->occHdr.numTotalChannels;
                jsonObj["channel"]["Occupancy"]["numTimeOfDays"] = OCCDFResponse->occHdr.numTimeOfDays;
                jsonObj["channel"]["Occupancy"]["aveFldStr"] = OCCDFResponse->aveFldStr;
                jsonObj["channel"]["Occupancy"]["aveRange"] = OCCDFResponse->aveRange;
                jsonObj["channel"]["Occupancy"]["numAzimuths"] = OCCDFResponse->numAzimuths;
                // jsonObj["channel"]["Occupancy"]["numChannels"] = OCCDFResponse->numChannels;
                jsonObj["channel"]["Occupancy"]["azimuthData"] = chanData;
            }
            break;

        case ECSMSDllMsgType::OCCDF_STATUS:
            {
                // SEquipCtrlMsg::SEquipTaskStatusResp* OCCDFResponse = (SEquipCtrlMsg::SEquipTaskStatusResp*)data;
			    SEquipCtrlMsg::SEquipTaskStatusResp* OCCDFResponse = reinterpret_cast<SEquipCtrlMsg::SEquipTaskStatusResp*>(data);

                jsonObj["SEquipTaskStatusResp"]["dateTime"] = OCCDFResponse->dateTime;
                jsonObj["SEquipTaskStatusResp"]["key"] = OCCDFResponse->key;
				jsonObj["SEquipTaskStatusResp"]["status"] = eStatusToString(OCCDFResponse->status);
                jsonObj["SEquipTaskStatusResp"]["taskId"] = OCCDFResponse->taskId;
            }
            break;

        default:
            jsonObj["error"] = std::string("Unexpected processOccupancyDFResponse type ") + ECSMSDllMsgTypeToString(respType);
            break;
    }
	return jsonObj;
}

//
// Convert response returned by callback OnRealTimeDataFunc in JSON
//
json ProcessRealTimeData(_In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data)
{
	json jsonObj;

	jsonObj["respType"] = static_cast<int>(respType);

	switch (respType)
	{
        case ECSMSDllMsgType::RT_SPECTRUM_START:
            {
                const SSmsRealtimeMsg::SStartV2* RTResponse = (SSmsRealtimeMsg::SStartV2*)data;
                unsigned int i;

                jsonObj["SStart"]["taskId"] = RTResponse->taskId;
                jsonObj["SStart"]["numBands"] = RTResponse->numBands;		
                for (i = 0; i < RTResponse->numBands; i++)
                {
                    json bandObj;
                    bandObj["binSize"] = RTResponse->band[i].chanSize.internal;
                    bandObj["startFreq"] = RTResponse->band[i].firstChanFreq.internal;
                    bandObj["numChan"] = RTResponse->band[i].numChan;
                    jsonObj["SStart"]["band"].push_back(bandObj);
                }
            }
            break;

        case ECSMSDllMsgType::RT_SPECTRUM_STOP:
            {
                const SSmsRealtimeMsg::SStop* RTResponse = (const SSmsRealtimeMsg::SStop*)data;
                jsonObj["SStop"]["taskId"] = RTResponse->taskId;
            }
            break;

        case ECSMSDllMsgType::RT_SPECTRUM_V1RESPONSE:
            {
                const SSmsRealtimeMsg::SSpectrum* RTResponse = (SSmsRealtimeMsg::SSpectrum*)data;

                jsonObj["Spectrum"]["taskId"] = RTResponse->taskId; 
                jsonObj["Spectrum"]["bandIndex"] = RTResponse->bandIndex;
                jsonObj["Spectrum"]["startFreq"] = RTResponse->firstChanFreq;
                jsonObj["Spectrum"]["binSize"] = RTResponse->chanSize;
                jsonObj["Spectrum"]["numChan"] = RTResponse->numChan;
                jsonObj["Spectrum"]["noiseFloor"] = RTResponse->noiseFloor;
                jsonObj["Spectrum"]["levelData"] = std::string(reinterpret_cast<const char*>(RTResponse->chanData), RTResponse->numChan);
            }
            break;

        case ECSMSDllMsgType::RT_SPECTRUM_V2RESPONSE:
            {
                const SSmsRealtimeMsg::SSpectrumV2* RTResponse = (SSmsRealtimeMsg::SSpectrumV2*)data;

                jsonObj["Spectrum"]["taskId"] = RTResponse->taskId;
                jsonObj["Spectrum"]["bandIndex"] = RTResponse->bandIndex;
                jsonObj["Spectrum"]["startFreq"]["internal"] = RTResponse->firstChanFreq.internal;
                jsonObj["Spectrum"]["binSize"]["internal"] = RTResponse->chanSize.internal;
                jsonObj["Spectrum"]["numChan"] = RTResponse->numChan;
                jsonObj["Spectrum"]["noiseFloor"] = RTResponse->noiseFloor;		
                jsonObj["Spectrum"]["levelData"] = std::string(reinterpret_cast<const char*>(RTResponse->chanData), RTResponse->numChan);
            }
            break;
        case ECSMSDllMsgType::RT_SPECTRUM_RESPONSE:
            {
                const SSmsRealtimeMsg::SSpectrumV3* RTResponse = (SSmsRealtimeMsg::SSpectrumV3*)data;

                jsonObj["Spectrum"]["taskId"] = RTResponse->taskId; 
                jsonObj["Spectrum"]["bandIndex"] = RTResponse->bandIndex;
                jsonObj["Spectrum"]["startFreq"]["internal"] = RTResponse->firstChanFreq.internal;
                jsonObj["Spectrum"]["binSize"]["internal"] = RTResponse->chanSize.internal;
                jsonObj["Spectrum"]["numChan"] = RTResponse->numChan;
                jsonObj["Spectrum"]["noiseFloor"] = RTResponse->noiseFloor;
                jsonObj["Spectrum"]["efield"] = RTResponse->efield;
                jsonObj["Spectrum"]["zeroVal"] = RTResponse->zeroVal;
                jsonObj["Spectrum"]["levelData"] = std::string(reinterpret_cast<const char*>(RTResponse->chanData), RTResponse->numChan);
            }
            break;

        case ECSMSDllMsgType::RT_DF_START:
            {
                const SSmsRealtimeMsg::SStartV2* RTResponse = (SSmsRealtimeMsg::SStartV2*)data;
                unsigned int i;
                
                jsonObj["RTDFStart"]["taskId"] = RTResponse->taskId;
                jsonObj["RTDFStart"]["numBands"] = RTResponse->numBands;

                for (i = 0; i < RTResponse->numBands; i++)
                {
                    json bandObj;
                    bandObj["binSize"] = RTResponse->band[i].chanSize.internal;
                    bandObj["startFreq"] = RTResponse->band[i].firstChanFreq.internal;
                    bandObj["numChan"] = RTResponse->band[i].numChan;
                    jsonObj["RTDFStart"]["band"].push_back(bandObj);
                }
            }
            break;

        case ECSMSDllMsgType::RT_DF_STARTV1:
            {
                const SSmsRealtimeMsg::SStart* RTResponse = (SSmsRealtimeMsg::SStart*)data;
                unsigned int i;

                jsonObj["RTDFStart"]["taskId"] = RTResponse->taskId;
                jsonObj["RTDFStart"]["numBands"] = RTResponse->numBands;
                jsonObj["RTDFStart"]["MAX_OCCBANDS"] = RTResponse->MAX_OCCBANDS;
                for (i = 0; i < RTResponse->numBands; i++)
                {
                    json bandObj;
                    bandObj["binSize"]["internal"] = RTResponse->chanSize[i];
                    bandObj["startFreq"]["internal"] = RTResponse->firstChanFreq[i];
                    bandObj["numChan"] = RTResponse->numChan[i];
                    jsonObj["RTDFStart"]["band"].push_back(bandObj);
                }		
            }
            break;

        case ECSMSDllMsgType::RT_DF_STOP:
            {
                const SSmsRealtimeMsg::SStop* RTResponse = (SSmsRealtimeMsg::SStop*)data;
                jsonObj["SStop"]["taskId"] = RTResponse->taskId;
            }
            break;

        case ECSMSDllMsgType::RT_DF_DATAV1:
            {
                const SSmsRealtimeMsg::SDfData* RTResponse = (SSmsRealtimeMsg::SDfData*)data;
                jsonObj["SDfData"]["bandIndex"] = RTResponse->bandIndex;
                jsonObj["SDfData"]["chanData"]["azimData"] = RTResponse->chanData->azimData;
                jsonObj["SDfData"]["chanData"]["specData"] = RTResponse->chanData->specData;
                jsonObj["SDfData"]["chanSize"] = RTResponse->chanSize;
                jsonObj["SDfData"]["firstChanFreq"] = RTResponse->firstChanFreq;
                jsonObj["SDfData"]["noiseFloor"] = RTResponse->noiseFloor;
                jsonObj["SDfData"]["numChan"] = RTResponse->numChan;
                jsonObj["SDfData"]["taskId"] = RTResponse->taskId;
            }
            break;

        case ECSMSDllMsgType::RT_DF_DATAV2:
            {
                const SSmsRealtimeMsg::SDfDataV2* RTResponse = (SSmsRealtimeMsg::SDfDataV2*)data;
                jsonObj["SDfDataV2"]["bandIndex"] = RTResponse->bandIndex;
                jsonObj["SDfDataV2"]["chanData"]["azimData"] = RTResponse->chanData->azimData;
                jsonObj["SDfDataV2"]["chanData"]["specData"] = RTResponse->chanData->specData;
                jsonObj["SDfDataV2"]["chanSize"] = RTResponse->chanSize;
                jsonObj["SDfDataV2"]["firstChanFreq"] = RTResponse->firstChanFreq;
                jsonObj["SDfDataV2"]["noiseFloor"] = RTResponse->noiseFloor;
                jsonObj["SDfDataV2"]["numChan"] = RTResponse->numChan;
                jsonObj["SDfDataV2"]["taskId"] = RTResponse->taskId;
                jsonObj["SDfDataV2"]["horizPol"] = RTResponse->horizPol;
            }
            break;

        case ECSMSDllMsgType::RT_DF_DATA:
            {
                const SSmsRealtimeMsg::SDfDataV3* RTResponse = (SSmsRealtimeMsg::SDfDataV3*)data;
                jsonObj["SDfDataV3"]["bandIndex"] = RTResponse->bandIndex;
                jsonObj["SDfDataV3"]["chanData"]["azimData"] = RTResponse->chanData->azimData;
                jsonObj["SDfDataV3"]["chanData"]["specData"] = RTResponse->chanData->specData;
                jsonObj["SDfDataV3"]["chanSize"]["internal"] = RTResponse->chanSize.internal;
                jsonObj["SDfDataV3"]["firstChanFreq"]["internal"] = RTResponse->firstChanFreq.internal;
                jsonObj["SDfDataV3"]["noiseFloor"] = RTResponse->noiseFloor;
                jsonObj["SDfDataV3"]["numChan"] = RTResponse->numChan;
                jsonObj["SDfDataV3"]["taskId"] = RTResponse->taskId;
                jsonObj["SDfDataV3"]["horizPol"] = RTResponse->horizPol;
            }
            break;

        case ECSMSDllMsgType::RT_IQ_DATA:
            {
                SSmsRealtimeMsg::SIqDataV4* RTResponse = (SSmsRealtimeMsg::SIqDataV4*)data; // v5 should be sent to rds app
                jsonObj["SIqDataV4"]["actualBW"]["internal"] = RTResponse->actualBW.internal;
                jsonObj["SIqDataV4"]["actualSampleRate"] = RTResponse->actualSampleRate;
                jsonObj["SIqDataV4"]["dataType"] = RTResponse->dataType;
                jsonObj["SIqDataV4"]["ddcChannel"] = RTResponse->ddcChannel;
                jsonObj["SIqDataV4"]["EOS"] = RTResponse->EOS;
                jsonObj["SIqDataV4"]["freq"]["internal"] = RTResponse->freq.internal;
                jsonObj["SIqDataV4"]["inputPort"] = RTResponse->inputPort;
                jsonObj["SIqDataV4"]["MAX_SAMPLES"] = RTResponse->MAX_SAMPLES;
                jsonObj["SIqDataV4"]["numSamples"] = RTResponse->numSamples;
                jsonObj["SIqDataV4"]["rxAtten"] = RTResponse->rxAtten;
                jsonObj["SIqDataV4"]["sampleOffset"] = RTResponse->sampleOffset;
                jsonObj["SIqDataV4"]["scaleFactor"] = RTResponse->scaleFactor;
                jsonObj["SIqDataV4"]["seqNumber"] = RTResponse->seqNumber;
                jsonObj["SIqDataV4"]["SIq"]["samplesFloat32"]["im"] = RTResponse->SIq.samplesFloat32->im;
                jsonObj["SIqDataV4"]["SIq"]["samplesFloat32"]["re"] = RTResponse->SIq.samplesFloat32->re;
                jsonObj["SIqDataV4"]["SIq"]["samplesInt16"]["im"] = RTResponse->SIq.samplesInt16->im;
                jsonObj["SIqDataV4"]["SIq"]["samplesInt16"]["re"] = RTResponse->SIq.samplesInt16->re;
                jsonObj["SIqDataV4"]["SIq"]["samplesInt32"]["im"] = RTResponse->SIq.samplesInt32->im;
                jsonObj["SIqDataV4"]["SIq"]["samplesInt32"]["re"] = RTResponse->SIq.samplesInt32->re;
                jsonObj["SIqDataV4"]["streamID"] = RTResponse->streamID;
                jsonObj["SIqDataV4"]["streamStartTime"]["timestamp"] = RTResponse->streamStartTime.timestamp;
            }
            break;

        default:
            jsonObj["error"] = std::string("Unexpected ProcessRealTimeData type ") + ECSMSDllMsgTypeToString(respType);
            break;
	}
	return jsonObj;
}

//
// Convert response returned by callback OnGpsDataFunc in JSON
//
json ProcessGpsData(SEquipCtrlMsg::SGpsResponse* gpsResponse)
{
    json jsonObj;

    jsonObj["dateTime"] = double(gpsResponse->dateTime);
    jsonObj["latitude"] = double(gpsResponse->latitude);
    jsonObj["longitude"] = double(gpsResponse->longitude);
    jsonObj["status"]["accuracy"] = int(gpsResponse->status.accuracy);
    jsonObj["status"]["antenna"] = int(gpsResponse->status.antenna);
    jsonObj["status"]["batVolt"] = int(gpsResponse->status.batVolt);
    jsonObj["status"]["lockHist"] = int(gpsResponse->status.lockHist);
    jsonObj["status"]["mode"] = int(gpsResponse->status.mode);
    jsonObj["status"]["noGps"] = int(gpsResponse->status.noGps);
    jsonObj["status"]["notTested"] = int(gpsResponse->status.notTested);
    jsonObj["status"]["numSats"] = int(gpsResponse->status.numSats);
    jsonObj["status"]["nvRam"] = int(gpsResponse->status.nvRam);
    jsonObj["status"]["oscVolt"] = int(gpsResponse->status.oscVolt);
    jsonObj["status"]["pllSynth"] = int(gpsResponse->status.pllSynth);
    jsonObj["status"]["receiver"] = int(gpsResponse->status.receiver);
    jsonObj["status"]["satLock"] = int(gpsResponse->status.satLock);
    jsonObj["status"]["timErr1"] = int(gpsResponse->status.timErr1);
    jsonObj["status"]["timErr2"] = int(gpsResponse->status.timErr2);
    jsonObj["status"]["timSrce"] = int(gpsResponse->status.timSrce);
    jsonObj["status"]["tracking"] = int(gpsResponse->status.tracking);

	return jsonObj;
}


// ----------------------------------------------------------------------
/*
    Data callback for Scorpio API
*/
void OnDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ unsigned long desstAddr, _In_ SEquipCtrlMsg::UBody* data)
{
    std::string response;

    switch (respType)
    {
    case ECSMSDllMsgType::GET_BIST:
    case ECSMSDllMsgType::GET_BIST_RESULT:
    case ECSMSDllMsgType::GET_DIAGNOSTICS:
        streamBuffer.push_back(processBITEResponse(respType, data));
        break;
    case ECSMSDllMsgType::GET_ANT_LIST_INFO:
        streamBuffer.push_back(ProcessAntListResponse(respType, data));
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
        streamBuffer.push_back(processOccupancyResponse(respType, data));
        break;
    case ECSMSDllMsgType::OCCDF_FREQ_VS_CHANNEL:
    case ECSMSDllMsgType::OCCDF_SCANDF_VS_CHANNEL:
    case ECSMSDllMsgType::OCCDF_STATUS:
    case ECSMSDllMsgType::OCCDF_STATE_RESPONSE:
    case ECSMSDllMsgType::OCCDF_SOLICIT_STATE_RESPONSE:
        streamBuffer.push_back(processOccupancyDFResponse(respType, data));
        break;
    case ECSMSDllMsgType::AVD_FREQ_VS_CHANNEL:
    case ECSMSDllMsgType::AVD_OCC_CHANNEL_RESULT:
    case ECSMSDllMsgType::AVD_FREQ_MEAS:
    case ECSMSDllMsgType::AVD_BW_MEAS:
    case ECSMSDllMsgType::AVD_SOLICIT_STATE_RESPONSE:
    case ECSMSDllMsgType::AVD_STATE_RESPONSE:
    case ECSMSDllMsgType::AVD_STATUS:
        streamBuffer.push_back(processAutoViolateResponse(respType, data));
        break;
    case ECSMSDllMsgType::GET_MEAS:
    case ECSMSDllMsgType::VALIDATE_MEAS:
        streamBuffer.push_back(processMeasResponse(respType, sourceAddr, data));
        break;
    case ECSMSDllMsgType::SET_PAN_PARAMS:
    case ECSMSDllMsgType::SET_AUDIO_PARAMS:
    case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
        streamBuffer.push_back(processDemodCtrlResponse(respType, data));
        break;
    case ECSMSDllMsgType::GET_PAN:
        response = processPanResponse(respType, data);
        logEtherDLL.info("GET_PAN response: " + response);
        streamBuffer.push_back(response);
        //streamBuffer.push_back(processPanResponse(respType, data));
        break;
    case ECSMSDllMsgType::GET_DM:

        //m_taskType = ECSMSDllMsgType::GET_DM; // TODO should be in DM_STATUS reponse instead
        break;
    default:
        break;
    }

    log.info("OnData received with type " + respType);
    logEtherDLL.info("OnData received destination address " + desstAddr);
    logEtherDLL.info("OnData received server ID " + serverId);
}

// ----------------------------------------------------------------------
/*
    Error callback for Scorpio API
*/
void OnErrorFunc(_In_  unsigned long serverId, _In_ const std::wstring& errorMsg)
{
    std::string str(errorMsg.begin(), errorMsg.end());
    logEtherDLL.error(str);
    std::string strData = "{\"serverId\":" + std::to_string(serverId) + ", \"errorMsg\":\"" + str + "\"}";
    errorBuffer.push_back(strData);
    logEtherDLL.info("OnErrorFunc received error message: " + str);
    logEtherDLL.info("OnErrorFunc received server ID: " + serverId);

}

// ----------------------------------------------------------------------
/*
    Realtime callback for Scorpio API
*/
void OnRealTimeDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data)
{
    streamBuffer.push_back(ProcessRealTimeData(respType, data));
    logEtherDLL.info("OnRealtimeData received with type " + respType);
    logEtherDLL.info("OnRealtimeData received server ID " + serverId);
}
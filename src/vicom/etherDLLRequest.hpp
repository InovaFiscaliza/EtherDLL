/**
 * @file etherDLLRequest.hpp
 * @brief JSON to Vicom API data structure conversion and request processing
 * 
 * This header file contains function declarations for converting JSON objects
 * to Vicom API data structures and for processing these requests.
 * 
 * @author fslobao
 * @date 2025-09-10
 * @version 1.1
 * 
 * @note Requires C++17 or later
 * @note Uses nlohmann/json library for JSON parsing
 * 
 * Dependencies:
 * - nlohmann/json.hpp
 * - ViCom*.h
 * 
 **/
 // ----------------------------------------------------------------------
#pragma once

// Include DLL specific libraries
#include "etherDLLInit.hpp"
#include "etherDLLDataProcess.hpp"
#include "etherDLLValidation.hpp"

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
extern MessageQueue response;

// ----------------------------------------------------------------------
/**
 * @brief Call the appropriate DLL function based on the request in JSON format
 *
 * Include the identification of the function based on request type
 * conversion from JSON to the appropriate struct for each function call.
 * 
 * @param DLLConn: Connection data for the Vicom DLL
 * @param request: JSON object containing the parameters
 * @param msgType: Message type to be validated
 * @return void
 * @throws NO EXCEPTION HANDLING
**/
void DLLFunctionCall(DLLConnectionData& DLLConn, json request, unsigned long msgType)
{
	const std::string logSource = "Vicom::DLLFunctionCall";
	loggerPtr->debug("Processing request: {}", request.dump());
	
	unsigned long requestID = request.value(TaskKeys::QueueId::VALUE, TaskKeys::QueueId::INIT_VALUE);
	json reqArguments = request.value(TaskKeys::Arguments::VALUE, json::object());
	json responseJson;

	switch (msgType) {
		case VicomTask::POWER_SCAN_CODE:
		{
			if (DLLConn.pInterface == nullptr) {
				loggerPtr->error("Vicom interface not connected.");
				responseJson["error"] = "Vicom interface not connected.";
				response.push(responseJson, logSource);
				return;
			}

			CViComError err;
			SSweepSettings sweepSettings;

			// Populate sweepSettings from JSON, with defaults from DefaultDLLParam
			using SweepConf = DefaultDLLParam::SweepSettings;
			sweepSettings.dwFrontEndSelectionMask = reqArguments.value(SweepConf::FRONT_END_MASK, SweepConf::FRONT_END_MASK_V);
			sweepSettings.dStartFrequencyInHz = reqArguments.value(SweepConf::START_FREQ_HZ, SweepConf::START_FREQ_HZ_V);
			sweepSettings.dStopFrequencyInHz = reqArguments.value(SweepConf::STOP_FREQ_HZ, SweepConf::STOP_FREQ_HZ_V);
			sweepSettings.bRequestRawData = reqArguments.value(SweepConf::REQ_RAW_DATA, SweepConf::REQ_RAW_DATA_V);

			// These settings are taken from the TSMWPorwerScan example and can be exposed in the JSON API later
			sweepSettings.sSpectrumSettings.fMaxReportingRateInHz = 10.0f;
			sweepSettings.sSpectrumSettings.fMaxDeviceMeasRateInHz = 10.0f;
			sweepSettings.sSpectrumSettings.eWindowType = SSpectrumSettings::RFPOWERSCAN_WINDOWTYPE_HANNING;
			sweepSettings.sSpectrumSettings.eFFTSize = SSpectrumSettings::RFPOWERSCAN_FFTSIZE_1024;
			sweepSettings.sSpectrumSettings.bAutoBandwidth = TRUE;
			sweepSettings.sSpectrumSettings.dwBandwidthInHz = (DWORD)20e6;
			sweepSettings.sSpectrumSettings.bLevelThreshold = FALSE;
			sweepSettings.sSpectrumSettings.fThresholdInDbm = -100.0f;
			sweepSettings.sSpectrumSettings.bPreamplifier = TRUE;
			sweepSettings.sSpectrumSettings.bAutoAttenuation = TRUE;
			sweepSettings.sSpectrumSettings.bAttenuationInDb = 0;
			sweepSettings.sMeasurementTime.dwMeasTimeInNs = 1000000;
			sweepSettings.sMeasurementTime.eDetectorType = SMeasurementTime::RFPOWERSCAN_DETECTOR_TYPE_RMS;
			sweepSettings.sFrequencyDetector.dwCountOfLines = 890;
			sweepSettings.sFrequencyDetector.eDetectorType = SFrequencyDetector::RFPOWERSCAN_FREQDET_TYPE_RMS;
			sweepSettings.sTimeDetector.eDetectorType = STimeDetector::RFPOWERSCAN_TIMEDET_TYPE_RMS;
			sweepSettings.sTimeDetector.eDetectorIntervalType = STimeDetector::RFPOWERSCAN_TIMEDET_INTERVAL_TIMERANGE;
			sweepSettings.sTimeDetector.dwTimeParameterInMs = 1000;
			sweepSettings.sMarker.bUseMarker = 0;
			sweepSettings.sMarker.bReturnsPowerValues = 0;


			if (!DLLConn.pInterface->SetSweepSettings(err, sweepSettings)) {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Error configuring SweepSettings: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
				response.push(responseJson, logSource);
				return;
			}
			
			CViComBasicInterface& basicIF = DLLConn.pInterface->GetBasicInterface();
			basicIF.StartMeasurement();

			// Get result with a 5-second timeout
			const SMeasResult* pResult = DLLConn.pInterface->GetResult(err, 5000); 
			if (pResult) {
				responseJson = processPowerScanResult(pResult);
				loggerPtr->info("Power scan successful.");
			}
			else {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Failed to get power scan result: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
			}

			basicIF.StopMeasurement();
			basicIF.HasMeasurementStopped();
			
			response.push(responseJson, logSource);
			break;
		}
		default:
		{
			loggerPtr->error("Unknown message type: {}", msgType);
			responseJson["error"] = "Unknown message type";
			response.push(responseJson, logSource);
			break;
		}
	}

	std::string reqName = request.value(TaskKeys::CommandName::VALUE, TaskKeys::CommandName::INIT_VALUE);
	loggerPtr->debug("Request [{}] processed", reqName);
}


// ----------------------------------------------------------------------
/** @brief Process messages from the request queue and call the appropriate DLL function
 *
 * This function will lock the thread. Must be run in a separate thread.
 * Messages are expected to be in JSON format and end with the defined message end sequence.
 *
 * @param DLLConn: Connection data for the Vicom DLL
 * @param request: Thread-safe message queue for incoming requests
 * @param response: Thread-safe message queue for outgoing responses
 * @param interruptionCode: Signal for service interruption
 * @throws NO EXCEPTION HANDLING
*/
void processRequestQueue(DLLConnectionData& DLLConn, MessageQueue& request, MessageQueue& response, edll::INT_CODE& interruptionCode)
{
	const std::string funcName = "processRequestQueue";

	while (interruptionCode == edll::Code::RUNNING)
	{
		json oneRequest = request.waitAndPop(interruptionCode, funcName);

		unsigned long cmd = oneRequest.value(TaskKeys::CommandCode::VALUE, TaskKeys::CommandCode::INIT_VALUE);

		if (!validRequest(oneRequest, cmd, response)) {
			continue;
		}

		DLLFunctionCall(DLLConn, oneRequest, cmd);
	}
}

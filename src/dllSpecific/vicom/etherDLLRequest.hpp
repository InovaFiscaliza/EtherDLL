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

			// Populate sweepSettings from JSON
			using SweepConf = DefaultDLLParam::SweepSettings;

			// Helper Lambda for Missing Parameter Check
			auto checkParam = [&](const char* key) -> bool {
				if (!reqArguments.contains(key)) {
					std::string errMsg = std::string("Missing required parameter: ") + key;
					loggerPtr->error(errMsg);
					responseJson["error"] = errMsg;
					response.push(responseJson, logSource);
					return false;
				}
				return true;
			};

			// --- Basic Sweep Settings ---
			if (!checkParam(SweepConf::FRONT_END_MASK)) return;
			sweepSettings.dwFrontEndSelectionMask = reqArguments[SweepConf::FRONT_END_MASK].get<unsigned long>();

			if (!checkParam(SweepConf::START_FREQ_HZ)) return;
			sweepSettings.dStartFrequencyInHz = reqArguments[SweepConf::START_FREQ_HZ].get<double>();

			if (!checkParam(SweepConf::STOP_FREQ_HZ)) return;
			sweepSettings.dStopFrequencyInHz = reqArguments[SweepConf::STOP_FREQ_HZ].get<double>();

			if (!checkParam(SweepConf::REQ_RAW_DATA)) return;
			sweepSettings.bRequestRawData = reqArguments[SweepConf::REQ_RAW_DATA].get<bool>() ? TRUE : FALSE;


			// --- Spectrum Settings ---
			if (!checkParam(SweepConf::MAX_REPORTING_RATE)) return;
			sweepSettings.sSpectrumSettings.fMaxReportingRateInHz = reqArguments[SweepConf::MAX_REPORTING_RATE].get<float>();

			if (!checkParam(SweepConf::MAX_DEVICE_MEAS_RATE)) return;
			sweepSettings.sSpectrumSettings.fMaxDeviceMeasRateInHz = reqArguments[SweepConf::MAX_DEVICE_MEAS_RATE].get<float>();

			if (!checkParam(SweepConf::WINDOW_TYPE)) return;
			sweepSettings.sSpectrumSettings.eWindowType = (SSpectrumSettings::etWindowType)reqArguments[SweepConf::WINDOW_TYPE].get<int>();

			if (!checkParam(SweepConf::FFT_SIZE)) return;
			sweepSettings.sSpectrumSettings.eFFTSize = (SSpectrumSettings::etFFTSize)reqArguments[SweepConf::FFT_SIZE].get<int>();

			if (!checkParam(SweepConf::AUTO_BANDWIDTH)) return;
			sweepSettings.sSpectrumSettings.bAutoBandwidth = reqArguments[SweepConf::AUTO_BANDWIDTH].get<bool>() ? TRUE : FALSE;

			if (!checkParam(SweepConf::BANDWIDTH_HZ)) return;
			sweepSettings.sSpectrumSettings.dwBandwidthInHz = reqArguments[SweepConf::BANDWIDTH_HZ].get<DWORD>();

			if (!checkParam(SweepConf::LEVEL_THRESHOLD)) return;
			sweepSettings.sSpectrumSettings.bLevelThreshold = reqArguments[SweepConf::LEVEL_THRESHOLD].get<bool>() ? TRUE : FALSE;

			if (!checkParam(SweepConf::THRESHOLD_DBM)) return;
			sweepSettings.sSpectrumSettings.fThresholdInDbm = reqArguments[SweepConf::THRESHOLD_DBM].get<float>();

			if (!checkParam(SweepConf::PREAMPLIFIER)) return;
			sweepSettings.sSpectrumSettings.bPreamplifier = reqArguments[SweepConf::PREAMPLIFIER].get<bool>() ? TRUE : FALSE;

			if (!checkParam(SweepConf::AUTO_ATTENUATION)) return;
			sweepSettings.sSpectrumSettings.bAutoAttenuation = reqArguments[SweepConf::AUTO_ATTENUATION].get<bool>() ? TRUE : FALSE;

			if (!checkParam(SweepConf::ATTENUATION_DB)) return;
			sweepSettings.sSpectrumSettings.bAttenuationInDb = (BYTE)reqArguments[SweepConf::ATTENUATION_DB].get<int>();

			
			// --- Measurement Time Settings ---
			if (!checkParam(SweepConf::MEAS_TIME_NS)) return;
			sweepSettings.sMeasurementTime.dwMeasTimeInNs = reqArguments[SweepConf::MEAS_TIME_NS].get<DWORD>();

			if (!checkParam(SweepConf::MEAS_DETECTOR_TYPE)) return;
			sweepSettings.sMeasurementTime.eDetectorType = (SMeasurementTime::etDetectorType)reqArguments[SweepConf::MEAS_DETECTOR_TYPE].get<int>();


			// --- Frequency Detector Settings ---
			if (!checkParam(SweepConf::FREQ_DETECTOR_LINES)) return;
			sweepSettings.sFrequencyDetector.dwCountOfLines = reqArguments[SweepConf::FREQ_DETECTOR_LINES].get<DWORD>();

			if (!checkParam(SweepConf::FREQ_DETECTOR_TYPE)) return;
			sweepSettings.sFrequencyDetector.eDetectorType = (SFrequencyDetector::etFrequencyDetectorType)reqArguments[SweepConf::FREQ_DETECTOR_TYPE].get<int>();


			// --- Time Detector Settings ---
			if (!checkParam(SweepConf::TIME_DETECTOR_TYPE)) return;
			sweepSettings.sTimeDetector.eDetectorType = (STimeDetector::etTimeDetectorType)reqArguments[SweepConf::TIME_DETECTOR_TYPE].get<int>();

			if (!checkParam(SweepConf::TIME_DETECTOR_INTERVAL_TYPE)) return;
			sweepSettings.sTimeDetector.eDetectorIntervalType = (STimeDetector::etTimeDetectorIntervalType)reqArguments[SweepConf::TIME_DETECTOR_INTERVAL_TYPE].get<int>();

			if (!checkParam(SweepConf::TIME_PARAMETER_MS)) return;
			sweepSettings.sTimeDetector.dwTimeParameterInMs = reqArguments[SweepConf::TIME_PARAMETER_MS].get<DWORD>();


			// --- Marker Settings ---
			if (!checkParam(SweepConf::USE_MARKER)) return;
			sweepSettings.sMarker.bUseMarker = reqArguments[SweepConf::USE_MARKER].get<bool>() ? 1 : 0;

			if (!checkParam(SweepConf::RETURN_POWER_VALUES)) return;
			sweepSettings.sMarker.bReturnsPowerValues = reqArguments[SweepConf::RETURN_POWER_VALUES].get<bool>() ? 1 : 0;


			// Apply Settings
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

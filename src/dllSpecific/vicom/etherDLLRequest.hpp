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
#include "ViComGpsInterface.h"
#include "ViComGpsInterfaceData.h"

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
using namespace RohdeSchwarz::ViCom;
using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;
using namespace RohdeSchwarz::ViCom::GPS;

// Global variables
extern spdlog::logger* loggerPtr;
extern MessageQueue response;

// ----------------------------------------------------------------------
/**
 * @brief Get parameter value from JSON with default fallback
 *
 * @param reqArguments: JSON object containing the parameters
 * @param key: Parameter key to look for
 * @param defaultValue: Default value if key not found
 * @return T: Value from JSON or default
 * @throws NO EXCEPTION HANDLING
**/
template<typename T>
T getParamValue(const json& reqArguments, const char* key, T defaultValue) {
    if (reqArguments.contains(key)) {
        return reqArguments[key].get<T>();
    }
    loggerPtr->debug("Using default value for parameter: {}", key);
    return defaultValue;
}

// ----------------------------------------------------------------------
/**
 * @brief Load sweep settings from JSON arguments with default values
 *
 * @param reqArguments: JSON object containing the parameters
 * @return RohdeSchwarz::ViCom::RFPOWERSCAN::SSweepSettings: Populated sweep settings structure
 * @throws NO EXCEPTION HANDLING
**/
RohdeSchwarz::ViCom::RFPOWERSCAN::SSweepSettings loadDefaultParams(const json& reqArguments)
{
	using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;
	SSweepSettings sweepSettings;

	sweepSettings.dwFrontEndSelectionMask = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::FRONT_END_MASK, 1UL);
	sweepSettings.dStartFrequencyInHz = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::START_FREQ_HZ, 90000000.0);
	sweepSettings.dStopFrequencyInHz = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::STOP_FREQ_HZ, 107000000.0);
	sweepSettings.bRequestRawData = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::REQ_RAW_DATA, false) ? TRUE : FALSE;
	sweepSettings.sSpectrumSettings.fMaxReportingRateInHz = static_cast<float>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::MAX_REPORTING_RATE, 10.0));
	sweepSettings.sSpectrumSettings.fMaxDeviceMeasRateInHz = static_cast<float>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::MAX_DEVICE_MEAS_RATE, 30.0));
	sweepSettings.sSpectrumSettings.eWindowType = static_cast<SSpectrumSettings::etWindowType>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::WINDOW_TYPE, 1));
	sweepSettings.sSpectrumSettings.eFFTSize = static_cast<SSpectrumSettings::etFFTSize>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::FFT_SIZE, 1024));
	sweepSettings.sSpectrumSettings.bAutoBandwidth = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::AUTO_BANDWIDTH, true) ? TRUE : FALSE;
	sweepSettings.sSpectrumSettings.dwBandwidthInHz = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::BANDWIDTH_HZ, 20000000UL);
	sweepSettings.sSpectrumSettings.bLevelThreshold = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::LEVEL_THRESHOLD, false) ? TRUE : FALSE;
	sweepSettings.sSpectrumSettings.fThresholdInDbm = static_cast<float>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::THRESHOLD_DBM, -100.0));
	sweepSettings.sSpectrumSettings.bPreamplifier = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::PREAMPLIFIER, true) ? TRUE : FALSE;
	sweepSettings.sSpectrumSettings.bAutoAttenuation = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::AUTO_ATTENUATION, true) ? TRUE : FALSE;
	sweepSettings.sSpectrumSettings.bAttenuationInDb = static_cast<BYTE>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::ATTENUATION_DB, 0));
	sweepSettings.sMeasurementTime.dwMeasTimeInNs = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::MEAS_TIME_NS, 1000000UL);
	sweepSettings.sMeasurementTime.eDetectorType = static_cast<SMeasurementTime::etDetectorType>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::MEAS_DETECTOR_TYPE, 1));
	sweepSettings.sFrequencyDetector.dwCountOfLines = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::FREQ_DETECTOR_LINES, 1024UL);
	sweepSettings.sFrequencyDetector.eDetectorType = static_cast<SFrequencyDetector::etFrequencyDetectorType>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::FREQ_DETECTOR_TYPE, 1));
	sweepSettings.sTimeDetector.eDetectorType = static_cast<STimeDetector::etTimeDetectorType>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::TIME_DETECTOR_TYPE, 1));
	sweepSettings.sTimeDetector.eDetectorIntervalType = static_cast<STimeDetector::etTimeDetectorIntervalType>(getParamValue(reqArguments, DefaultDLLParam::SweepSettings::TIME_DETECTOR_INTERVAL_TYPE, 1));
	sweepSettings.sTimeDetector.dwTimeParameterInMs = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::TIME_PARAMETER_MS, 1000UL);
	sweepSettings.sMarker.bUseMarker = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::USE_MARKER, false) ? 1 : 0;
	sweepSettings.sMarker.bReturnsPowerValues = getParamValue(reqArguments, DefaultDLLParam::SweepSettings::RETURN_POWER_VALUES, false) ? 1 : 0;

	return sweepSettings;
}

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
void DLLFunctionCall(DLLConnectionData& DLLConn, json request, unsigned long msgType, std::atomic<bool>& refMeasurementInProgress, const std::atomic<edll::INT_CODE>& interruptionCode)
{
	const std::string logSource = "Vicom::DLLFunctionCall";
	loggerPtr->debug("Processing request: {}", request.dump());
	
	unsigned long requestID = request.value(TaskKeys::QueueId::VALUE, TaskKeys::QueueId::INIT_VALUE);
	json reqArguments = request.value(TaskKeys::Arguments::VALUE, json::object());
	json responseJson;

	switch (msgType) {
		case VicomTask::POWER_SCAN_CONFIG_CODE:
		{
			if (DLLConn.ps_pInterface == nullptr) {
				loggerPtr->error("Vicom interface not connected.");
				responseJson["error"] = "Vicom interface not connected.";
				response.push(responseJson, logSource);
				return;
			}

			RohdeSchwarz::ViCom::CViComError err;
			RohdeSchwarz::ViCom::RFPOWERSCAN::SSweepSettings sweepSettings = loadDefaultParams(reqArguments);

			if (!DLLConn.ps_pInterface->SetSweepSettings(err, sweepSettings)) {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Error configuring SweepSettings: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
				response.push(responseJson, logSource);
				return;
			}

			DLLConn.sweepSettings = sweepSettings;
			DLLConn.isConfigured = true;

			responseJson["status"] = "configured";
			loggerPtr->info("Power scan configured successfully.");
			response.push(responseJson, logSource);
			break;
		}

		case VicomTask::POWER_SCAN_MEASURE_CODE:
		{
			if (DLLConn.ps_pInterface == nullptr) {
				loggerPtr->error("Vicom interface not connected.");
				responseJson["error"] = "Vicom interface not connected.";
				response.push(responseJson, logSource);
				return;
			}

			if (!DLLConn.isConfigured) {
				loggerPtr->error("Power scan not configured. Call powerScanConfig first.");
				responseJson["error"] = "Power scan not configured. Call powerScanConfig first.";
				response.push(responseJson, logSource);
				return;
			}

			RohdeSchwarz::ViCom::CViComError err;
			RohdeSchwarz::ViCom::RFPOWERSCAN::SSweepSettings sweepSettings = DLLConn.sweepSettings;

			if (!DLLConn.ps_pInterface->SetSweepSettings(err, sweepSettings)) {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Error configuring SweepSettings: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
				response.push(responseJson, logSource);
				return;
			}
			
			RohdeSchwarz::ViCom::CViComBasicInterface& basicIF = DLLConn.ps_pInterface->GetBasicInterface();
			basicIF.StartMeasurement();
			refMeasurementInProgress.store(true, std::memory_order_release);

			while (refMeasurementInProgress.load(std::memory_order_acquire) && interruptionCode == edll::Code::RUNNING) {
				int retrieveTimeMs = static_cast<int>(1000 / sweepSettings.sSpectrumSettings.fMaxReportingRateInHz);
				const RohdeSchwarz::ViCom::RFPOWERSCAN::SMeasResult* pResult = DLLConn.ps_pInterface->GetResult(err, 5000);
				if (pResult) {
					responseJson = processPowerScanResult(&sweepSettings, pResult);
					loggerPtr->info("Power scan successful.");
				}
				else {
					CStringA ansiErrorString(err.GetErrorString());
					loggerPtr->error("Failed to get power scan result: {}", ansiErrorString.GetString());
					responseJson["error"] = vicomErrorToJson(err);
				}
				response.push(responseJson, logSource);
				
				auto startTime = std::chrono::steady_clock::now();
				while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() < retrieveTimeMs
					&& interruptionCode == edll::Code::RUNNING
					&& refMeasurementInProgress.load(std::memory_order_acquire))
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
			}

			basicIF.StopMeasurement();
			basicIF.HasMeasurementStopped();
			
			break;
		}

		case VicomTask::GPS_GET_LOCATION_CODE:
		{
			if (DLLConn.gps_pInterface == nullptr) {
				loggerPtr->error("Vicom GPS interface not connected.");
				responseJson["error"] = "Vicom GPS interface not connected.";
				response.push(responseJson, logSource);
				return;
			}

			RohdeSchwarz::ViCom::CViComError err;
			RohdeSchwarz::ViCom::GPS::SGPSDeviceSettings gpsSettings; // Use default settings

			if (!DLLConn.gps_pInterface->SetGPSDeviceSettings(err, gpsSettings)) {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Error configuring GPS Settings: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
				response.push(responseJson, logSource);
				return;
			}

			RohdeSchwarz::ViCom::CViComBasicInterface& basicIF = DLLConn.gps_pInterface->GetBasicInterface();

			if (!basicIF.StartMeasurement(err)) {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Error starting GPS measurement: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
				response.push(responseJson, logSource);
				return;
			}

			// Get result with a 5-second timeout
			loggerPtr->info("Waiting for GPS result...");
			const RohdeSchwarz::ViCom::GPS::SMeasResult* pResult = DLLConn.gps_pInterface->GetResult(err, 5000);
			if (pResult) {
				responseJson = processGPSResult(pResult);
				loggerPtr->info("GPS location retrieval successful.");
			}
			else {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Failed to get GPS location result: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
			}

			loggerPtr->debug("Stopping GPS measurement...");
			basicIF.StopMeasurement();
			loggerPtr->debug("GPS measurement stop command sent.");
			
			response.push(responseJson, logSource);
			break;
		}

		case VicomTask::IDN_CODE:
		{
			responseJson["model"] = DLLConn.receiverModel;
			responseJson["serial"] = DLLConn.serialNumber;
			responseJson["sw_version"] = DLLConn.softwareVersion;
			responseJson["hw_version"] = DLLConn.hardwareVersion;

			std::string idnResponse = "Rohde&Schwarz," + DLLConn.receiverModel + "," + DLLConn.serialNumber + "," + DLLConn.softwareVersion;
			responseJson["message"] = idnResponse;
			
			loggerPtr->info("IDN query successful: {}", idnResponse);
			response.push(responseJson, logSource);
			break;
		}

		case VicomTask::STOP_MEASUMENT_CODE:
		{
			refMeasurementInProgress.store(false, std::memory_order_release);
			loggerPtr->info("Stop measurement command received.");
			responseJson["message"] = "Stop measurement command processed.";
			response.push(responseJson, logSource);
			break;
		}

		case VicomTask::GET_SETTINGS_CODE:
		{
			if (DLLConn.ps_pInterface == nullptr) {
				loggerPtr->error("Vicom interface not connected.");
				responseJson["error"] = "Vicom interface not connected.";
				response.push(responseJson, logSource);
				return;
			}

			RohdeSchwarz::ViCom::CViComError err;
			const RohdeSchwarz::ViCom::RFPOWERSCAN::SSettings* pSettings = DLLConn.ps_pInterface->GetSettings(err);

			if (pSettings) {
				responseJson = processSettingsResult(pSettings);
				loggerPtr->info("GetSettings successful.");
			}
			else {
				CStringA ansiErrorString(err.GetErrorString());
				loggerPtr->error("Failed to get settings: {}", ansiErrorString.GetString());
				responseJson["error"] = vicomErrorToJson(err);
			}
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
 * @param refMeasurementInProgress: (bool&) Reference to a flag indicating if a measurement is in progress
 * @throws NO EXCEPTION HANDLING
*/
void processRequestQueue(
    DLLConnectionData& DLLConn,
    MessageQueue& request,
    MessageQueue& response,
    const std::atomic<edll::INT_CODE>& interruptionCode,
    std::atomic<bool>& refMeasurementInProgress
)
{
    const std::string funcName = "processRequestQueue";

    while (interruptionCode == edll::Code::RUNNING)
    {
        json oneRequest = request.waitAndPop(interruptionCode, funcName);

		if (interruptionCode != edll::Code::RUNNING) {
			break;
		}

        unsigned long cmd = oneRequest.value(TaskKeys::CommandCode::VALUE, TaskKeys::CommandCode::INIT_VALUE);

        if (!validRequest(oneRequest, cmd, response)) {
            continue;
        }

        DLLFunctionCall(DLLConn, oneRequest, cmd, refMeasurementInProgress, interruptionCode);
    }
}

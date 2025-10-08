/**
 * @file etherDLLValidation.hpp
 * @brief Validation functions for JSON objects in Scorpio API requests
 *
 * This header file contains function declarations for validating JSON objects
 * used in various Scorpio API requests within the DLL interface.
 *
 * @author fslobao
 * @date 2025-10-04
 * @version 1.0
 *
 * @note Requires C++14 or later
 * @note Uses nlohmann/json library for JSON parsing
 *
 * Dependencies:
 * - nlohmann/json.hpp
 *
 **/
 // ----------------------------------------------------------------------
#pragma once

// Include provided DLL libraries

// Include DLL specific libraries

// Include core EtherDLL libraries
#include "EtherDLLConfig.hpp"
#include "EtherDLLUtils.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <string>

// For convenience
using json = nlohmann::json;

// Global variables
extern spdlog::logger* loggerPtr;

// Validation Constants
const double MIN_FREQ = 20e6;      // Minimum frequency in Hz
const double MAX_FREQ = 3e9;       // Maximum frequency in Hz
const double MIN_BANDWIDTH = 500;  // Minimum bandwidth in Hz
const double MAX_BANDWIDTH = 80e6; // Maximum bandwidth in Hz
const int MIN_DURATION = 1;        // Minimum duration in seconds
const int MAX_DURATION = 3600;     // Maximum duration in seconds
const int MIN_ANT = 1;             // Minimum antenna number
const int MAX_ANT = 16;            // Maximum antenna number
const int MIN_DF_CONFIDENCE = 1;      // Minimum DF confidence level
const int MAX_DF_CONFIDENCE = 360;    // Maximum DF confidence level
const int MIN_AZIMUTHS = 1;        // Minimum number of azimuth
const int MAX_AZIMUTHS = 720;      // Maximum number of azimuth
const int MIN_RECORD_HOLDOFF = 0;  // Minimum record holdoff time
const int MAX_RECORD_HOLDOFF = 1024; // Maximum record holdoff time
const int MIN_SCAN_DF_THRESHOLD = 0; // Minimum scan DF threshold
const int MAX_SCAN_DF_THRESHOLD = 256; // Maximum scan DF threshold
const int MIN_NUM_BANDS = 1;       // Minimum number of bands
const int MAX_NUM_BANDS = 255;   // Maximum number of bands
const int MIN_STORAGE_TIME = 1;    // Minimum storage time in ms
const int MAX_STORAGE_TIME = 3600000; // Maximum storage time in ms
const int MIN_MEASUREMENT_TIME = 100; // Minimum measurement time in ms
const int MAX_MEASUREMENT_TIME = 3600000; // Maximum measurement time in ms
const int MIN_BFO = 0;             // Minimum BFO value
const int MAX_BFO = 1024;          // Maximum BFO value
const int MIN_DET_MODE = 0;        // Minimum detection mode
const int MAX_DET_MODE = 256;      // Maximum detection mode
const int MIN_RCVD_ATTEN = 0;      // Minimum receiver attenuation in dB
const int MAX_RCVD_ATTEN = 255;     // Maximum receiver attenuation in dB
const int MIN_AGC_TIME = 0;        // Minimum AGC time in ms
const int MAX_AGC_TIME = 3600000;  // Maximum AGC time in


// ----------------------------------------------------------------------
/**
  * @brief Validate AVD request JSON object
  *
  * @param jsonObj: JSON object containing the specific arguments for AVD request
  * @param validator: JsonValidator instance to accumulate validation results
  * @return void
  * @throws NO EXCEPTION HANDLING
  **/
void validateAVDRequest(const json& request, JsonValidator& validator) {

    validator
		.requireRange(request, "freq", MIN_FREQ, MAX_FREQ)
		.requireRange(request, "bandwidth", MIN_BANDWIDTH, MAX_BANDWIDTH)
		.requireRange(request, "duration", MIN_DURATION, MAX_DURATION)
        .requireType(request, "modulation", "string");
}

// ----------------------------------------------------------------------
/**
  * @brief Validate Measurement request JSON object
  * 
  * @param jsonObj: JSON object containing the specific arguments for Measurement request
  * @param validator: JsonValidator instance to accumulate validation results
  * @return void
  * @throws NO EXCEPTION HANDLING
 **/
void validateMeasurementRequest(const json& request, JsonValidator& validator) {
    validator
        .requireRange(request, "ant", MIN_ANT, MAX_ANT)
        .requireRange(request, "freq", MIN_FREQ, MAX_FREQ)
        .requireRange(request, "bandwidth", MIN_BANDWIDTH, MAX_BANDWIDTH);
}

// ----------------------------------------------------------------------
/**
  * @brief Validate occupancy request JSON object
  *
  * @param jsonObj: JSON object containing the specific arguments for occupancy request
  * @param validator: JsonValidator instance to accumulate validation results
  * @return void
  * @throws NO EXCEPTION HANDLING
 **/
void validateOccupancyRequest(const json& request, JsonValidator& validator) {

    validator
		.requireRange(request, "ant", MIN_ANT, MAX_ANT)
		.requireRange(request, "numBands", MIN_NUM_BANDS, MAX_NUM_BANDS)
		.requireRange(request, "storageTime", MIN_STORAGE_TIME, MAX_STORAGE_TIME)
		.requireRange(request, "measurementTime", MIN_MEASUREMENT_TIME, MAX_MEASUREMENT_TIME)
		.requireRange(request, "numAzimuths", MIN_AZIMUTHS, MAX_AZIMUTHS)
        .requireRange(request, "confidence", MIN_DF_CONFIDENCE, MAX_DF_CONFIDENCE)
        .requireRange(request, "recordHoldoff", MIN_RECORD_HOLDOFF, MAX_RECORD_HOLDOFF)
        .requireRange(request, "scanDfThreshold", MIN_SCAN_DF_THRESHOLD, MAX_SCAN_DF_THRESHOLD)
		.requireType(request, "recordAudioDf", VALID_TYPE_BOOLEAN)
        .validateObjectItems(request, "band", [](const json& bandItem, JsonValidator& v, size_t index) {
        v.requireRange(bandItem, "channelBandwidth", MIN_BANDWIDTH, MAX_BANDWIDTH)
            .requireType(bandItem, "exclude", VALID_TYPE_BOOLEAN)
			.requireRange(bandItem, "stopFrequency", MIN_FREQ, MAX_FREQ)
			.requireRange(bandItem, "startFrequency", MIN_FREQ, MAX_FREQ)
            .custom(bandItem,
                "startFrequency",
                [&bandItem](const json& lf) {
                    if (bandItem.contains("stopFrequency")) {
                        return lf.get<double>() < bandItem["stopFrequency"].get<double>();
                    }
                    return true;
                        },
                "startFrequency must be less than stopFrequency");
            }
        );
}

// ----------------------------------------------------------------------
/**
 * @brief Validate OccupancyDF request JSON object
 * @param request: JSON object containing the specific arguments for OccupancyDF request
 * @param validator: JsonValidator instance to accumulate validation results
 * @return void
 * @throws NO EXCEPTION HANDLING
**/
void validateOccupancyDFRequest(const json& request, JsonValidator& validator) {

    validator
        .validateObjectItems(request, "rcvrCtrl", [](const json& rcvrItem, JsonValidator& v, size_t index) {
           v.requireRange(rcvrItem, "freq", MIN_FREQ, MAX_FREQ)
            .requireRange(rcvrItem, "bandwidth", MIN_BANDWIDTH, MAX_BANDWIDTH)
            .requireRange(rcvrItem, "bfo", MIN_BFO, MAX_BFO)
            .requireRange(rcvrItem, "detMode", MIN_DET_MODE, MAX_DET_MODE)
			.requireRange(rcvrItem, "agcTime", MIN_AGC_TIME, MAX_AGC_TIME);
        });
}

// ----------------------------------------------------------------------
/**
 * @brief Validate panParams JSON object
 * @param request: JSON object containing the specific arguments for panParams
 * @param validator: JsonValidator instance to accumulate validation results
 * @return void
 * @throws NO EXCEPTION HANDLING
**/
void validateGetPan(const json& request, JsonValidator& validator) {

    validator
        .requireRange(request, "centerFrequency", MIN_FREQ, MAX_FREQ)
        .requireRange(request, "span", MIN_BANDWIDTH, MAX_BANDWIDTH)
        .requireRange(request, "rcvrAtten", MIN_RCVD_ATTEN, MAX_RCVD_ATTEN);
}

// ----------------------------------------------------------------------
/**
* @brief Validate audioParams JSON object
* @param request: JSON object containing the specific arguments for audioParams
* @param validator: JsonValidator instance to accumulate validation results
* @return void
* @throws NO EXCEPTION HANDLING
*/
void validateAudioParams(const json& request, JsonValidator& validator) {

    validator
		.requireRange(request, "freq", MIN_FREQ, MAX_FREQ)
        .optionalType(request, "channel", VALID_TYPE_NUMBER)
		.requireRange(request, "bandwidth", MIN_BANDWIDTH, MAX_BANDWIDTH)
        .optionalType(request, "bfo", VALID_TYPE_NUMBER)
        .optionalType(request, "anyChannel", VALID_TYPE_BOOLEAN)
        .optionalType(request, "detMode", VALID_TYPE_NUMBER)
        .optionalType(request, "doModRec", VALID_TYPE_BOOLEAN)
        .optionalType(request, "doRDS", VALID_TYPE_BOOLEAN)
        .optionalType(request, "streamID", VALID_TYPE_NUMBER)
		.requireArray(request, "test_array", VALID_TYPE_NUMBER, 2)
        .requireArray(request, "test_string_array", VALID_TYPE_STRING, 2)
        .requireType(request, "test_float", VALID_TYPE_NUMBER)
        .requireType(request, "test_string", VALID_TYPE_STRING);
}

// ----------------------------------------------------------------------
/**
* @brief Build response JSON object for invalid requests as response to client
* @param jsonObj: JSON object containing the request parameters
* @param errorMsg: Error message to be included in the response
* @return nlohmann::json: Response JSON object
*/
nlohmann::json buildErrorResponse(const nlohmann::json& jsonObj, const std::string& errorMsg) {
    nlohmann::json response = jsonObj;

	using TaskKeys = edll::DefaultConfig::Service::TaskKeys;

	response[TaskKeys::CommandCode::VALUE] = TaskKeys::CommandCode::INIT_VALUE;
	response[TaskKeys::CommandName::VALUE] = TaskKeys::CommandName::INIT_VALUE;
	response[TaskKeys::Arguments::VALUE] = json::object();
	response[TaskKeys::Message::VALUE] = errorMsg;

    return response;
}

// ----------------------------------------------------------------------
/**
* @brief Test JSON object contains the required information is present
*
* @param jsonObj: JSON object containing the parameters
* @param msgType: Message type to be validated (see ECSMSDllMsgType enum)
* @return nlohmann::json:
* @throws NO EXCEPTION HANDLING
**/
bool validRequest(json request, unsigned long msgType, MessageQueue& response) {

	const std::string logSource = "EtherDLLValidation::validRequest";
    
    JsonValidator validator;

	// Validate common fields
	using TaskKeys = edll::DefaultConfig::Service::TaskKeys;

    validator
        .requireType(request, TaskKeys::CommandCode::VALUE, VALID_TYPE_NUMBER)
	    .requireType(request, TaskKeys::CommandName::VALUE, VALID_TYPE_STRING)
		.requireType(request, TaskKeys::Arguments::VALUE, VALID_TYPE_OBJECT);

    switch (msgType) {          
        case ECSMSDllMsgType::GET_OCCUPANCYDF:
            validateOccupancyDFRequest(request[TaskKeys::Arguments::VALUE], validator);
            // Fall through intended
        case ECSMSDllMsgType::GET_OCCUPANCY:
            validateOccupancyRequest(request[TaskKeys::Arguments::VALUE], validator);
			break;
        case ECSMSDllMsgType::GET_AVD:
			validateAVDRequest(request[TaskKeys::Arguments::VALUE], validator);
			break;
        case ECSMSDllMsgType::GET_MEAS:
			validateMeasurementRequest(request[TaskKeys::Arguments::VALUE], validator);
			break;
        case ECSMSDllMsgType::GET_TASK_STATUS:
            return true;
        case ECSMSDllMsgType::GET_TASK_STATE:
            return true;
        case ECSMSDllMsgType::TASK_SUSPEND:
            return true;
        case ECSMSDllMsgType::TASK_RESUME:
            return true;
        case ECSMSDllMsgType::TASK_TERMINATE:
            return true;
        case ECSMSDllMsgType::GET_BIST:
            return true;
        case ECSMSDllMsgType::SET_AUDIO_PARAMS:
			validateAudioParams(request[TaskKeys::Arguments::VALUE], validator);
			break;
        case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
            return true;
        case ECSMSDllMsgType::SET_PAN_PARAMS:
            return true;
        case ECSMSDllMsgType::GET_PAN:
			validateGetPan(request[TaskKeys::Arguments::VALUE], validator);
		    break;
        default: {
            loggerPtr->error("Unknown message type for validation: " + std::to_string((unsigned long)msgType));
            return false;
        }
    }

    if (!validator.isValid()) {
		std::string message = "Request validation failed: " + validator.getErrorString();
        loggerPtr->error(message);
		response.push(buildErrorResponse(request, message), logSource);
        return false;
	}
	loggerPtr->debug("Request validation passed");
	return true;
}
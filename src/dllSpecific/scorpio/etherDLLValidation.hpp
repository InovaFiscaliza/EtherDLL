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
#include "EtherDLLConstants.hpp"
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
        .requireType(request, "freq", "number")
        .requireType(request, "bandwidth", "number")
        .requireType(request, "duration", "integer")
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
        .requireNumberRange(request, "ant", 1, 16)
        .requireNumberRange(request, "freq", 20e6, 3e9)
        .requireNumberRange(request, "bandwidth", 500, 80e6);
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
        .requireNumberRange(request, "ant", 1, 16)
		.requireNumberRange(request, "numBands", 1, 1e256)
		.requireNumberRange(request, "storageTime", 1, 3600000)
		.requireNumberRange(request, "measurementTime", 100, 3600000)
		.requireNumberRange(request, "numAzimuths", 1, 720)
        .requireNumberRange(request, "confidence", 1, 360)
        .requireNumberRange(request, "recordHoldoff", 0, 1024)
        .requireNumberRange(request, "scanDfThreshold", 0, 256)
		.requireType(request, "recordAudioDf", "bool")
        .validateObjectItems(request, "band", [](const json& bandItem, JsonValidator& v, size_t index) {
        v.requireNumberRange(bandItem, "channelBandwidth", 500, 80e6)
            .requireType(bandItem, "exclude", "bool")
            .requireNumberRange(bandItem, "highFrequency", 20e6, 3e9)
            .requireNumberRange(bandItem, "lowFrequency", 20e6, 3e9)
            .custom(bandItem, "lowFrequency", [&bandItem](const json& lf) {
            if (bandItem.contains("highFrequency")) {
                return lf.get<double>() < bandItem["highFrequency"].get<double>();
            }
            return true;
                }, "lowFrequency must be less than highFrequency");
            });
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
           v.requireNumberRange(rcvrItem, "freq", 20e6, 3e9)
            .requireNumberRange(rcvrItem, "bandwidth", 500, 80e6)
            .requireNumberRange(rcvrItem, "bfo", 0, 1024)
            .requireNumberRange(rcvrItem, "detMode", 0, 256)
			.requireNumberRange(rcvrItem, "agcTime", 0, 3600000);
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
        .requireNumberRange(request, "freq", 20e6, 3e9)
        .requireNumberRange(request, "bandwidth", 5e2, 80e6)
        .requireNumberRange(request, "rcvrAtten", 0, 255);
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

    if (!request.contains("audioParams")) {
        validator.addError("audioParams field is required");
    }

    const auto& audioParams = request["audioParams"];
    validator
        .optionalType(audioParams, "freq", "number")
        .optionalType(audioParams, "channel", "integer")
        .optionalType(audioParams, "bandwidth", "number")
        .optionalType(audioParams, "bfo", "number")
        .optionalType(audioParams, "anyChannel", "boolean")
        .optionalType(audioParams, "detMode", "integer")
        .optionalType(audioParams, "doModRec", "boolean")
        .optionalType(audioParams, "doRDS", "boolean")
        .optionalType(audioParams, "streamID", "integer");
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

    JsonValidator validator;

	// Validate common fields
	using TaskKeys = edll::DefaultConfig::Service::TaskKeys;

    validator
        .requireType(request, TaskKeys::CommandCode::VALUE, "number")
        .requireType(request, TaskKeys::CommandName::VALUE, "string")
		.requireType(request, TaskKeys::Arguments::VALUE, "object");

    switch (msgType) {          
        case ECSMSDllMsgType::GET_OCCUPANCY:
            validateOccupancyRequest(request[TaskKeys::Arguments::VALUE], validator);
			// Fall through intended
        case ECSMSDllMsgType::GET_OCCUPANCYDF:
            validateOccupancyDFRequest(request[TaskKeys::Arguments::VALUE], validator);
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
			validateAudioParams(request, validator);
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
		response.push(buildErrorResponse(request, message));
        return false;
	}
	loggerPtr->debug("Request validation passed");
	return true;
}
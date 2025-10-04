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
  * @brief Validate occupancy request JSON object
  *
  * @param jsonObj: JSON object containing the parameters
  * @param validator: JsonValidator instance to accumulate validation results
  * @return void
  * @throws NO EXCEPTION HANDLING
 **/
void validateOccupancyRequest(const json& request, JsonValidator& validator) {

    if (!request.contains("audioParams")) {
        validator.addError("audioParams field is required");
    }

    validator
        .requireArray(request, "band", 1)
        .optionalType(request, "ant", "integer")
        .optionalType(request, "measurementTime", "integer")
        .optionalType(request, "storageTime", "integer")
        .validateArrayItems(request, "band", [](const json& bandItem, JsonValidator& v, size_t index) {
        v.requireType(bandItem, "lowFrequency", "number")
            .requireType(bandItem, "highFrequency", "number")
            .optionalType(bandItem, "channelBandwidth", "number")
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
* @brief Validate audioParams JSON object
* @param request: JSON object containing the request parameters
* @return std::string: Empty if valid, error message if invalid
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
* @brief Build response JSON object for invalid requests
* @param jsonObj: JSON object containing the request parameters
* @param errorMsg: Error message to be included in the response
* @return nlohmann::json: Response JSON object
*/
nlohmann::json buildErrorResponse(const nlohmann::json& jsonObj, const std::string& errorMsg) {
    nlohmann::json response = jsonObj;

	using msg = edll::DefaultConfig::Service::Queue;
	response[msg::CommandCode::VALUE] = msg::CommandCode::INIT_VALUE;
	response[msg::CommandName::VALUE] = msg::CommandName::INIT_VALUE;
	response[msg::Arguments::VALUE] = json::object();
	response[msg::Message::VALUE] = errorMsg;

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
    validator
        .requireField(request, "commandCode")
        .requireType(request, "commandCode", "integer")
        .requireField(request, "commandName")
        .requireType(request, "commandName", "string")
        .requireField(request, "arguments")
		.requireType(request, "arguments", "object");

    switch (msgType) {
        case ECSMSDllMsgType::GET_OCCUPANCYDF:
            // fallthrough intended to also validate GET_OCCUPANCY fields
        case ECSMSDllMsgType::GET_OCCUPANCY:
            validateOccupancyRequest(request, validator);
		    break;
        case ECSMSDllMsgType::GET_AVD:
            return true;
        case ECSMSDllMsgType::GET_MEAS:
            return true;
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
            return true;
        case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
            return true;
        case ECSMSDllMsgType::SET_PAN_PARAMS:
            return true;
        case ECSMSDllMsgType::GET_PAN:
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
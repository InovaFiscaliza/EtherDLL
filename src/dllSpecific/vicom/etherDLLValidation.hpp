/**
 * @file etherDLLValidation.hpp
 * @brief Validation functions for JSON objects in Vicom API requests
 *
 * This header file contains function declarations for validating JSON objects
 * used in various Vicom API requests within the DLL interface.
 *
 * @author fslobao
 * @date 2025-12-03
 * @version 1.1
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

// Include DLL specific libraries
#include "etherDLLCodes.hpp"

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

// ----------------------------------------------------------------------
/**
  * @brief Validate Power Scan request JSON object
  *
  * @param request: JSON object containing the specific arguments for the request
  * @param validator: JsonValidator instance to accumulate validation results
  * @return void
  * @throws NO EXCEPTION HANDLING
  **/
void validatePowerScanRequest(const json& request, JsonValidator& validator) {
    using SweepConf = DefaultDLLParam::SweepSettings;

    validator
        .optionalType(request, SweepConf::START_FREQ_HZ, VALID_TYPE_NUMBER)
        .optionalType(request, SweepConf::STOP_FREQ_HZ, VALID_TYPE_NUMBER)
        .custom(request, SweepConf::START_FREQ_HZ, [&request](const json& sf) {
            if (request.contains(SweepConf::STOP_FREQ_HZ)) {
                return sf.get<double>() < request[SweepConf::STOP_FREQ_HZ].get<double>();
            }
            return true;
        }, "start_freq_hz must be less than stop_freq_hz");
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
	response[TaskKeys::CommandName::VALUE] = "error_response";
	response[TaskKeys::Arguments::VALUE] = json::object();
	response[TaskKeys::Message::VALUE] = errorMsg;

    return response;
}

// ----------------------------------------------------------------------
/**
* @brief Test JSON object contains the required information is present
*
* @param request: JSON object containing the parameters
* @param msgType: Message type to be validated
* @param response: Message queue to push error responses to
* @return bool: True if the request is valid, false otherwise.
* @throws NO EXCEPTION HANDLING
**/
bool validRequest(json request, unsigned long msgType, MessageQueue& response) {

	const std::string logSource = "Vicom::validRequest";
    
    JsonValidator validator;

	// Validate common fields
	using TaskKeys = edll::DefaultConfig::Service::TaskKeys;

    validator
        .requireType(request, TaskKeys::CommandCode::VALUE, VALID_TYPE_NUMBER)
	    .requireType(request, TaskKeys::CommandName::VALUE, VALID_TYPE_STRING)
		.requireType(request, TaskKeys::Arguments::VALUE, VALID_TYPE_OBJECT);

    // Stop validation if common fields are invalid
    if (!validator.isValid()) {
        std::string message = "Request validation failed on common fields: " + validator.getErrorString();
        loggerPtr->error(message);
		response.push(buildErrorResponse(request, message), logSource);
        return false;
    }

    switch (msgType) {          
        case VicomTask::GPS_GET_LOCATION_CODE:
            // No specific arguments to validate
            break;
        case VicomTask::IDN_CODE:
            // No specific arguments to validate
            break;
        case VicomTask::GET_SETTINGS_CODE:
            // No specific arguments to validate
            break;
        case VicomTask::POWER_SCAN_CONFIG_CODE:
            validatePowerScanRequest(request[TaskKeys::Arguments::VALUE], validator);
			break;
        case VicomTask::POWER_SCAN_MEASURE_CODE:
            // No specific arguments to validate
            break;
        default: {
            loggerPtr->error("Unknown message type for validation: {}", msgType);
			response.push(buildErrorResponse(request, "Unknown command code."), logSource);
            return false;
        }
    }

    if (!validator.isValid()) {
		std::string message = "Request validation failed for command '" + request.value(TaskKeys::CommandName::VALUE, "") + "': " + validator.getErrorString();
        loggerPtr->error(message);
		response.push(buildErrorResponse(request, message), logSource);
        return false;
	}

	loggerPtr->debug("Request validation passed for command '{}'", request.value(TaskKeys::CommandName::VALUE, ""));
	return true;
}
/**
 * @file etherDLLValidation.hpp
 * @brief Validation functions for ViCom API request parameters
 * 
 * This header file contains function declarations for validating JSON objects
 * used in various ViCom API requests within the DLL interface.
 * 
 * @author mlnunes@anatel.gov.br
 * @date 2026-3-19
 * @version 1.3
 * 
 * @note Requires C++14 or later
 * @note Uses nlohmann/json library for JSON parsing
 * 
 * Dependencies:
 * - etherDLLCodes.hpp
 * - EtherDLLConfig.hpp
 * - EtherDLLUtils.hpp
 * 
 * @ingroup vicom
 */
 // ----------------------------------------------------------------------
#pragma once

// Include DLL specific libraries
#include "etherDLLInit.hpp"
#include "etherDLLCodes.hpp"

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

// Global variables
extern spdlog::logger* loggerPtr;

// ----------------------------------------------------------------------
// Validate Power Scan request parameters
// @brief Checks if Power Scan request JSON contains valid parameters
// @param request JSON object containing the request arguments
// @param validator JsonValidator instance to accumulate validation results
// @note Validates frequency range (start < stop)
// @ingroup vicom_validation
// ----------------------------------------------------------------------
void validatePowerScanRequest(const json& request, JsonValidator& validator) {
    using SweepConf = DefaultDLLParam::SweepSettings;

    const char* startFreqKey = findPresentKeyName(request, SweepConf::START_FREQ_HZ);
    const char* stopFreqKey = findPresentKeyName(request, SweepConf::STOP_FREQ_HZ);

    if (startFreqKey != nullptr) {
        validator.optionalType(request, startFreqKey, VALID_TYPE_NUMBER);
    }

    if (stopFreqKey != nullptr) {
        validator.optionalType(request, stopFreqKey, VALID_TYPE_NUMBER);
    }

    if (startFreqKey != nullptr) {
        validator.custom(request, startFreqKey, [&request](const json& sf) {
            const auto* stopFreq = findValueByKey(request, SweepConf::STOP_FREQ_HZ);
            if (stopFreq != nullptr) {
                return sf.get<double>() < stopFreq->get<double>();
            }
            return true;
        }, "start frequency must be less than stop frequency");
    }
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
// Validate request message structure
// @brief Verifies that request contains required fields based on message type
// @param request JSON object containing the request parameters
// @param msgType Message type code to determine validation rules
// @param response Message queue to push error responses if validation fails
// @return true if request is valid, false otherwise
// @note Uses specific validators based on msgType (Power Scan, GPS, etc.)
// @ingroup vicom_validation
// ----------------------------------------------------------------------
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
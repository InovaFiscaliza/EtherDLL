/**
 * @file etherDLLDataProcess.hpp
 *
 * @brief Data processing statements for EtherDLL service
 *
 * This source file contains the implementation of data processing functions
 * used in the EtherDLL service when processing received Vicom data
 * before sending it to the client.
 *
 * * @author fslobao
 * * @date 2025-09-16
 * * @version 1.1
 *
 * * @note Requires C++17 or later
 * * @note Uses nlohmann/json library for JSON handling
 *
 * * * Dependencies:
 *
**/
// ----------------------------------------------------------------------
#pragma once

// Include provided DLL libraries
#include "stdafx.h"
#include "ViComRFPowerScanInterface.h"
#include "ViComRFPowerScanInterfaceData.h"
#include "ViComBasicInterface.h"
#include "ViComBasicInterfaceData.h"

// Include core EtherDLL libraries
#include "EtherDLLConfig.hpp"
#include "EtherDLLUtils.hpp"

// Include project libraries
#include <nlohmann/json.hpp>

// Include general C++ libraries
#include <string>
#include <vector>
#include <cstdint>

// For convenience
using json = nlohmann::json;
using namespace RohdeSchwarz::ViCom;
using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;

// ----------------------------------------------------------------------
/** @brief Process the power scan result and convert it to a JSON object
 *
 * @param pResult: Pointer to the SMeasResult structure containing the measurement result.
 * @return json: A JSON object containing the processed scan data.
 * @throws NO EXCEPTION HANDLING
**/
json processPowerScanResult(const SSweepSettings* sweepSettings, const SMeasResult* pResult)
{
    json resultJson;

    if (!pResult || !pResult->pSpectrumResult) {
        resultJson["error"] = "Invalid or null result pointer.";
        return resultJson;
    }

    const SMeasResult::SSpectrumResult* spectrumResult = pResult->pSpectrumResult;
	auto startFreq = sweepSettings->dStartFrequencyInHz;
	auto stopFreq = sweepSettings->dStopFrequencyInHz;
    
    resultJson["count"] = spectrumResult->dwCount;
	resultJson["startFrequency"] = startFreq;
	resultJson["stopFrequency"] = stopFreq;

    // Convert the float array of spectrum values to a Base64 encoded string
    if (spectrumResult->dwCount > 0 && spectrumResult->pfSpectrumValuesInDBm)
    {
        const unsigned char* dataPtr = reinterpret_cast<const unsigned char*>(spectrumResult->pfSpectrumValuesInDBm);
        size_t dataSize = spectrumResult->dwCount * sizeof(float);
        resultJson["spectrum_dbm_base64"] = base64Encode(dataPtr, static_cast<unsigned int>(dataSize));
    }
    else
    {
        resultJson["spectrum_dbm_base64"] = "";
    }

    return resultJson;
}

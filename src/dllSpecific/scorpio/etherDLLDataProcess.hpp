/**
 * @file etherDLLDataProcess.hpp
 *
 * @brief Data processing statements for EtherDLL service
 *
 * This source file contains the implementation of data processing functions
 * used in the EtherDLL service when processing received ScorpioDLL data
 * before sending it to the client.
 *
 * * @author fslobao
 * * @date 2025-09-16
 * * @version 1.0
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
#include "StdAfx.h"
#include "EquipCtrlMsg.h"

// Include DLL specific libraries
#include "etherDLLResponse.hpp"
#include "etherDLLData.hpp"

// Include core EtherDLL libraries
#include "EtherDLLConfig.hpp"

// Include project libraries

// Include general C++ libraries
#include <string>
#include <vector>
#include <cstdint>
#include <limits>

// For convenience

// ----------------------------------------------------------------------
/** @brief Expand scan data from compressed format
 *
 * Expands the compressed data in the input[ninput] array into the
 * the array output[noutput]
 * if successful, returns the actual number of elements in the output array
 * if unsuccessful, returns -1  (output array overrun)
 * input and output arrays can not be the same
 *
 * @param ninput: Number of elements in the input array
 * @param input: Pointer to the input array (compressed data)
 * @param noutput: Number of elements in the output array
 * @param output: Pointer to the output array (expanded data)
 * @return int: Actual number of elements in the output array if successful, -1 if unsuccessful (output array overrun)
 * @throws NO EXCEPTION HANDLING
 *
 * @note input and output arrays cannot be the same
**/
int ScanDataExpand(int ninput, int* input, int noutput, int* output)
{
    int* outend = output + noutput;
    int i;
    int* in = input;
    int* out = output;

    for (i = 0; i < ninput; i++, in++)
    {
        if (*in > 0)	// copy this element to output
        {
            if (out >= outend) return (-1);	// output overrun
            *out++ = *in;
        }
        else	// insert required number of zeroes into output
        {
            int zerocount = -(*in);
            if (out + zerocount > outend) return (-1);	// output overrun
            for (int* j = out; j < out + zerocount; j++) *j = 0;
            out += zerocount;
        }
    }
    return (out - output);
}

// ----------------------------------------------------------------------
/** @brief Convert 8-bit binary data to float32 with offset
 *
 * Convert bindata (uint8 array starting at pointer with numBins elements)
 * into floats32 vector with offset of 192.0f
 * Float vector is returned as uint8 vector with size numBins * 4 (32bits)
 * Use of char* to return the vector is to simplify later serialization
 *
 * @param binData: Pointer to the input binary data array (uint8)
 * @param numBins: Number of elements in the input binary data array
 * @return const unsigned char*: Pointer to the output data array (uint8) containing float32 values
 * @throws NO EXCEPTION HANDLING
**/
const unsigned char* parsedBinData(const unsigned char* binData, unsigned short numBins, float offset)
{
    static_assert(sizeof(float) == 4, "Expected float32");

    // TODO REMOVE THUS CONSTANT
    const size_t nRequested = static_cast<size_t>(numBins);

    // Buffer persistente por thread para evitar retorno de ponteiro pendente
    thread_local static std::vector<unsigned char> parsedData;
    parsedData.resize(nRequested * sizeof(float));

    float* outFloats = reinterpret_cast<float*>(parsedData.data());

    std::transform(binData, binData + nRequested, outFloats,
        [offset](unsigned char val) { return static_cast<float>(val) - offset; });

    return parsedData.data();
}


// ----------------------------------------------------------------------
/** @brief Convert SSmsRealtimeMsg::SDfDataV3::SChanData data into multiple
 *  float32 vectors with scalling and offset applied.
 *  Vectors returned are: azimuth, confidence, spectrum, dfSpectrum
 *     struct SChanData
		    {
			    unsigned short	chan;		// Channel number (zero-based)
			    unsigned short	azimData;	// Channel lob data (1/100 degree; 0 to 35999)
			    unsigned short	conf;		// DF confidence (1/10th %, 0 - 1000)
			    unsigned char	specData;	// Channel spectrum data (dBm + 200)
			    unsigned char	dfSpecData;	// DF antenna spectrum data (dBm + 200)
		    }
 * Float vectors are returned as uint8 vector with size numBins * 4 (32bits)
 *
 * @param binData: Pointer to the input binary data array (uint8)
 * @param numBins: Number of elements in the input binary data array
 * @return const unsigned char*: Pointer to the output data array (uint8) containing float32 values
 * @throws NO EXCEPTION HANDLING
**/
DFDataRawResult parsedDFData(const SSmsRealtimeMsg::SDfDataV3::SChanData* chanData,
    unsigned short numElements)
{
    static_assert(sizeof(float) == 4, "Expected float32");

    const size_t nRequested = static_cast<size_t>(numElements);
    const size_t byteSize = nRequested * sizeof(float);

    DFDataRawResult result(byteSize);

    float* azimFloats = reinterpret_cast<float*>(result.azimuth.data());
    float* confFloats = reinterpret_cast<float*>(result.confidence.data());
    float* specFloats = reinterpret_cast<float*>(result.spectrum.data());
    float* dfSpecFloats = reinterpret_cast<float*>(result.dfSpectrum.data());

    std::transform(chanData, chanData + nRequested, azimFloats,
        [](const auto& ch) { return static_cast<float>(ch.azimData) / AZIMUTH_SCALE; });
    
    std::transform(chanData, chanData + nRequested, confFloats,
        [](const auto& ch) { return static_cast<float>(ch.conf) / CONFIDENCE_SCALE; });
    
    std::transform(chanData, chanData + nRequested, specFloats,
        [](const auto& ch) { return static_cast<float>(ch.specData) - OCC_BYTE_POWER_OFFSET; });
    
    std::transform(chanData, chanData + nRequested, dfSpecFloats,
        [](const auto& ch) { return static_cast<float>(ch.dfSpecData) - OCC_BYTE_POWER_OFFSET; });

    return result;
}


// ----------------------------------------------------------------------
/** @brief Calculate spectrum information from pan response
 *
 * Calculate start frequency, stop frequency and bin size from the pan response
 * Central frequency is converted from internal units to MHz
 * Bin size is converted from internal units to Hz
 * Start and stop frequencies are calculated based on central frequency and half span
 * Half span is calculated as (bin size * floor(numBins / 2)) / 1,000,000 to convert to MHz
 *
 * @param panResponse: Pointer to the SGetPanResp structure containing pan response data
 * @return FrequencyInfo: Structure containing start frequency, stop frequency and bin size
 * @throws NO EXCEPTION HANDLING
**/
FrequencyRange panFrequencyInfo(const SEquipCtrlMsg::SGetPanResp* panResponse)
{
    double centralFrequency = Units::Frequency(panResponse->freq).Hz<double>();

    double binSize = Units::Frequency(panResponse->binSize).Hz<double>();

    double halfSpan = binSize * double(floor(panResponse->numBins / 2.0));

    // Calculate start and stop frequencies
    FrequencyRange freqInfo{};

    freqInfo.startFrequency = centralFrequency - halfSpan;
    freqInfo.stopFrequency = centralFrequency + halfSpan;
    freqInfo.binSize = binSize;

    return freqInfo;
}

// ----------------------------------------------------------------------
/** @brief Preprocessor for filtering and transforming messages.
 *  Message processor state might be affected by received messages.
 *
 * Provides preprocessing logic for messages before sending to client.
 * Returns an optional to indicate whether the message should be sent.
**/
class MessagePreprocessor {
private:
    const json* configPtr;
    spdlog::logger* loggerPtr;

public:
    // ------------------------------------------------------------------
    /** @brief Default constructor - initializes with null pointers
    **/
    MessagePreprocessor()
        : configPtr(nullptr), loggerPtr(nullptr) {
    }

    // ------------------------------------------------------------------
    /** @brief Construct preprocessor with configuration
        * @param config: JSON configuration object
        * @param logger: Logger reference for debug output
        **/
    MessagePreprocessor(const json& config, spdlog::logger& logger)
        : configPtr(&config), loggerPtr(&logger) {
    }

    // ------------------------------------------------------------------
    /** @brief Initialize or update preprocessor configuration
        * @param config: JSON configuration object
        * @param logger: Logger reference for debug output
        **/
    void initialize(const json& config, spdlog::logger& logger) {
        configPtr = &config;
        loggerPtr = &logger;
    }

    // ------------------------------------------------------------------
    /** @brief Check if preprocessor is properly initialized
        * @return bool: True if initialized, false otherwise
        **/
    bool isInitialized() const {
        return configPtr != nullptr && loggerPtr != nullptr;
    }

    // ------------------------------------------------------------------
    /** @brief Process data before sending to DLL or to client
        *
        * Applies filtering, transformation, and validation to outgoing messages.
        * Returns empty optional if message should be suppressed.
        *
        * @param data: JSON object to preprocess
        * @return std::optional<json>: Processed message, or std::nullopt to skip sending
        * @throws NO EXCEPTION HANDLING
        **/
    std::optional<json> process(json data) {
        // Ensure preprocessor is initialized
        if (!isInitialized()) {
            // Cannot log without logger - return nullopt to suppress message
            return std::nullopt;
        }

        // Filter: Skip empty or invalid responses
        if (data.is_null() || data.empty()) {
            loggerPtr->debug("ResponsePreprocessor: Skipping empty response");
            return std::nullopt;
        }

        using TaskKeys = edll::DefaultConfig::Service::TaskKeys;

        unsigned long msgType = data.value(TaskKeys::CommandCode::VALUE, TaskKeys::CommandCode::INIT_VALUE);
        switch (msgType) {
		    case PP_CODE::RESET:
                
                break;
			case PP_CODE::STOP:
                break;
			case PP_CODE::BIST:
                break;
            case PP_CODE::SET_SCAN:
                break;
            case PP_CODE::GET_IDN:
                break;
            case PP_CODE::GET_STATUS:
                break;
            case PP_CODE::GET_ERROR:
                break;
            case PP_CODE::GET_BIST:
                break;
            case PP_CODE::GET_GPS:
                break;
            case PP_CODE::GET_SCAN:
				break;
            default:
                break;
        }


        // Transform: Remove internal-only fields before sending
        removeInternalFields(data);

        return data;
    }

private:
    // ------------------------------------------------------------------
    /** @brief Remove fields intended for internal use only
        * @param response: JSON object to modify
        **/
    void removeInternalFields(json& response) {
        // Remove internal tracking fields not meant for client
        static const std::vector<std::string> internalFields = {
            "_internal", "_suppress", "_debug"
        };

        for (const auto& field : internalFields) {
            if (response.contains(field)) {
                response.erase(field);
            }
        }
    }

    // ------------------------------------------------------------------
    /** @brief Validate response has minimum required structure
        * @param response: JSON object to validate
        * @return bool: True if valid, false otherwise
        **/
    bool validateResponse(const json& response) {
        // Basic validation - customize based on protocol requirements
        return response.is_object();
    }
};
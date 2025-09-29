/**
* @file etherDLLDataProcess.cpp
*
* @brief Data processing functions for EtherDLL service
* 
* This source file contains the implementation of data processing functions
* used in the EtherDLL service when processing received ScorpioDLL data
* before sending it to the client.
*
* * @author fslobao
* * @date 2025-09-16
* * @version 1.0
*
* * @note Requires C++11 or later
* * @note Uses nlohmann/json library for JSON handling
*
* * * Dependencies:
* *     - EquipCtrlMsg.h
* *	    - etherDLLConstants.hpp
* *     - etherDLLResponse.hpp
**/

// ----------------------------------------------------------------------
// Include provided DLL libraries
#include "EquipCtrlMsg.h"

// Include DLL specific libraries
#include "etherDLLDataProcess.hpp"
#include "etherDLLResponse.hpp"

// Include core EtherDLL libraries
#include "EtherDLLConstants.hpp"

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
static const unsigned char* parsedBinData(const unsigned char* binData, unsigned short numBins)
{
    static_assert(sizeof(float) == 4, "Esperado float32");

	// TODO REMOVE THUS CONSTANT
    const size_t nRequested = static_cast<size_t>(numBins);

    // Buffer persistente por thread para evitar retorno de ponteiro pendente
    thread_local static std::vector<unsigned char> parsedData;
    parsedData.resize(nRequested * sizeof(float));

    float* outFloats = reinterpret_cast<float*>(parsedData.data());

    for (size_t i = 0; i < nRequested; ++i) {
        outFloats[i] = static_cast<float>(binData[i]) - BYTE_POWER_OFFSET;
    }

    return parsedData.data();
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
 * @return SpectrumInfo: Structure containing start frequency, stop frequency and bin size
 * @throws NO EXCEPTION HANDLING
**/
SpectrumInfo calculateSpectrumInfo(const SEquipCtrlMsg::SGetPanResp* panResponse)
{
    // Convert central frequency from internal units to MHz
    double centralFrequency = double(panResponse->freq.internal) / (FREQ_FACTOR * edll::MHZ_MULTIPLIER);

    // Convert bin size from internal units to Hz
    double binSize = double(panResponse->binSize.internal) / FREQ_FACTOR;

    // Calculate half span in MHz  
    double halfSpan = (binSize * double(floor(panResponse->numBins / double(2.0)))) / double(1000000.0);

    // Calculate start and stop frequencies
    double startFrequency = centralFrequency - halfSpan;
    double stopFrequency = centralFrequency + halfSpan;

    return { startFrequency, stopFrequency, binSize };
}

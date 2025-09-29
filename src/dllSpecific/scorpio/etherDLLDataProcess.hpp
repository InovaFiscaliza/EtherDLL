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
 * * @note Requires C++11 or later
 * * @note Uses nlohmann/json library for JSON handling
 *
 * * * Dependencies:
 *
**/
// ----------------------------------------------------------------------
#pragma once

// Include provided DLL libraries

// Include DLL specific libraries

// Include core EtherDLL libraries

// Include project libraries

// Include general C++ libraries
#include <string>

// For convenience

// Data structures
struct SpectrumInfo {
    double startFrequency;  // MHz
    double stopFrequency;   // MHz  
    double binSize;         // Hz
};

// Constants
constexpr double FREQ_FACTOR = 1920.0;
constexpr float BYTE_POWER_OFFSET = 192.0;

// Function prototypes
int ScanDataExpand(int ninput, int* input, int noutput, int* output);
static const unsigned char* parsedBinData(const unsigned char* binData, unsigned short numBins);
SpectrumInfo calculateSpectrumInfo(const SEquipCtrlMsg::SGetPanResp* panResponse);

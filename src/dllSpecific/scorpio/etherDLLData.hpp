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

// Include core EtherDLL libraries
#include "EtherDLLConfig.hpp"
#include "EtherDLLUtils.hpp"

// Include project libraries

// Include general C++ libraries
#include <string>
#include <vector>
#include <cstdint>
#include <limits>

// For convenience


// Constants
constexpr double FREQ_FACTOR = 1920.0;
constexpr float PAN_BYTE_POWER_OFFSET = 192.0;
constexpr float OCC_BYTE_POWER_OFFSET = 200.0;


// Data structures
struct SpectrumInfo {
    double startFrequency;  // MHz
    double stopFrequency;   // MHz  
    double binSize;         // Hz
};

struct SiteInfo {
    Normal latitude;    // Average coordinate in decimal degrees
    Normal longitude;   // Average coordinate in decimal degrees
    NonNormal satelliteCount{/*sampleSize=*/0,
                            /*histogramBins=*/8,
                            /*histogramMin=*/0.0,
                            /*histogramMax=*/7.0 };
};



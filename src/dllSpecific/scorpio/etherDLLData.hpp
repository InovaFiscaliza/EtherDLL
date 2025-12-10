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
constexpr float PAN_BYTE_POWER_OFFSET = 192.0f;
constexpr float OCC_BYTE_POWER_OFFSET = 200.0f;
constexpr float CONFIDENCE_SCALE = 10.0f; // Confidence is given in tenths of percent (0 - 1000)
constexpr float AZIMUTH_SCALE = 100.0f;   // Azimuth is given in hundredths of degree (0 - 35999)


// Data structures

// ------------------------------------------------------
/** @brief Structure to hold site GPS information
 *
 * latitude and longitude with measurement count, average and standard deviation,
 * Satellite measurement count and histogram up to 8 bins (0 to 7 satellites)
 * Timestamps for the first and last measurments.
 *
**/
struct Site {
    Normal latitude;    // Average coordinate in decimal degrees
    Normal longitude;   // Average coordinate in decimal degrees
	NonNormal satelliteCount; // Number of satellites used in measurements
    std::string firstUpdateTime; // datetime in ISO 8601 format "YYYY-MM-DDThh:mm:ss.ssssZ"
    std::string lastUpdateTime; // datetime in ISO 8601 format "YYYY-MM-DDThh:mm:ss.ssssZ"
};


struct Equipment {
    std::string lastUpdateTime; // datetime in ISO 8601 format "YYYY-MM-DDThh:mm:ss.ssssZ"
    std::string model;
    std::string serialNumber;
    std::string firmwareVersion;
    std::string hardwareVersion;
    std::string softwareVersion;
    std::string calibrationDate; // date in ISO 8601 format "YYYY-MM-DD"
};


struct FrequencyRange {
    double startFrequency;      // in Hz
    double stopFrequency;       // in Hz
    double binSize;             // in Hz
};

struct Configuration {
    FrequencyRange frequencyInfo;
    float receiverAttenuation; // in dB
    float referenceLevel;      // in dBm
    float preselectorAttenuation; // in dB
    std::string lastUpdateTime; // datetime in ISO 8601 format "YYYY-MM-DDThh:mm:ss.ssssZ"
};

struct Spectrum {
    unsigned long taskId;
    FrequencyRange frequencyInfo;
    std::vector<float> trace;      // Power Amplitude Normalized data in dBm
	std::string unit;               // e.g., "dBm"; "dBuV/m", "%", "degrees"
    std::string firstUpdateTime;    // datetime in ISO 8601 format "YYYY-MM-DDThh:mm:ss.ssssZ"
    std::string lastUpdateTime;     // datetime in ISO 8601 format "YYYY-MM-DDThh:mm:ss.ssssZ"
};

// Define the result structure (add to etherDLLData.hpp or at the top of this file)
struct DFDataRawResult {
    std::vector<unsigned char> azimuth;      // numBins * sizeof(float) bytes
    std::vector<unsigned char> confidence;
    std::vector<unsigned char> spectrum;
    std::vector<unsigned char> dfSpectrum;

    // Default constructor
    DFDataRawResult() = default;

    // Constructor with size and optional default value
    explicit DFDataRawResult(size_t size, unsigned char defaultValue = 0)
        : azimuth(size, defaultValue)
        , confidence(size, defaultValue)
        , spectrum(size, defaultValue)
        , dfSpectrum(size, defaultValue)
    {}
};






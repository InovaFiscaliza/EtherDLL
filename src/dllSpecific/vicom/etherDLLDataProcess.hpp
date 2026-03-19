/**
 * @file etherDLLDataProcess.hpp
 * @brief Data processing functions for ViCom API results
 * 
 * This header file contains functions to convert ViCom API measurement results
 * into JSON format for transmission to clients:
 * - Power Scan result processing
 * - GPS result processing
 * - Settings query result processing
 * 
 * @author fslobao
 * @date 2025-09-16
 * @version 1.2
 * 
 * @note Requires C++17 or later
 * @note Uses nlohmann/json library for JSON handling
 * 
 * Dependencies:
 * - ViComRFPowerScanInterface.h
 * - ViComGpsInterface.h
 * - EtherDLLConfig.hpp
 * - EtherDLLUtils.hpp
 * 
 * @ingroup vicom
 */

// etherDLLDataProcess.hpp
#pragma once

// Include provided DLL libraries
#include "stdafx.h"
#include "ViComRFPowerScanInterface.h"
#include "ViComRFPowerScanInterfaceData.h"
#include "ViComGpsInterface.h"
#include "ViComGpsInterfaceData.h"
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

// ----------------------------------------------------------------------
// For convenience
// ----------------------------------------------------------------------
using json = nlohmann::json;
using namespace RohdeSchwarz::ViCom;
using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;
using namespace RohdeSchwarz::ViCom::GPS;

// ----------------------------------------------------------------------
// Process Power Scan measurement result
// @brief Converts Power Scan measurement data to JSON format
// @param sweepSettings Pointer to sweep settings used in measurement
// @param pResult Pointer to the measurement result structure
// @return JSON object containing count, frequencies, and spectrum data
// @note Spectrum values are encoded as Base64 string
// @ingroup vicom_processing
// ----------------------------------------------------------------------
inline json processPowerScanResult(const SSweepSettings* sweepSettings, const RFPOWERSCAN::SMeasResult* pResult)
{
   using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;

   json resultJson;

   if (!pResult || !pResult->pSpectrumResult) {
      resultJson["error"] = "Invalid or null result pointer.";
      return resultJson;
   }

   const RFPOWERSCAN::SMeasResult::SSpectrumResult* spectrumResult = pResult->pSpectrumResult;
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

// ----------------------------------------------------------------------
// Process GPS measurement result
// @brief Converts GPS location data to JSON format
// @param pResult Pointer to the GPS measurement result
// @return JSON object containing latitude, longitude, and altitude
// @ingroup vicom_processing
// ----------------------------------------------------------------------
inline json processGPSResult(const RohdeSchwarz::ViCom::GPS::SMeasResult* pResult)
{
   using namespace RohdeSchwarz::ViCom::GPS;
   
   json resultJson;
   
   if (!pResult) {
      resultJson["error"] = "Invalid or null GPS result pointer.";
      return resultJson;
   }
   resultJson["latitude"] = pResult->sPosition.dLatitude;
   resultJson["longitude"] = pResult->sPosition.dLongitude;
   resultJson["altitude"] = pResult->sPosition.dAltitude;
   return resultJson;
}

// ----------------------------------------------------------------------
// Process device settings query result
// @brief Converts device settings to JSON format for client response
// @param pSettings Pointer to the settings structure from ViCom API
// @return JSON object containing sweep settings, spectrum settings, etc.
// @ingroup vicom_processing
// ----------------------------------------------------------------------
inline json processSettingsResult(const RFPOWERSCAN::SSettings* pSettings)
{
   using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;

   json resultJson;

   if (!pSettings) {
      resultJson["error"] = "Invalid or null settings pointer.";
      return resultJson;
   }

   resultJson["receiver_index"] = pSettings->dwReceiverIndex;
   resultJson["result_buffer_depth"] = pSettings->ResultBufferDepth.dwValue;

   const SSweepSettings& sweep = pSettings->SweepSettings;
   json sweepJson;

   sweepJson["front_end_selection_mask"] = sweep.dwFrontEndSelectionMask;
   sweepJson["start_frequency"] = sweep.dStartFrequencyInHz;
   sweepJson["stop_frequency"] = sweep.dStopFrequencyInHz;
   sweepJson["require_raw_data"] = sweep.bRequestRawData == TRUE;

   json spectrumJson;
   spectrumJson["max_reporting_rate"] = sweep.sSpectrumSettings.fMaxReportingRateInHz;
   spectrumJson["max_device_measurement_rate"] = sweep.sSpectrumSettings.fMaxDeviceMeasRateInHz;
   spectrumJson["window_type"] = sweep.sSpectrumSettings.eWindowType;
   spectrumJson["fft_size"] = sweep.sSpectrumSettings.eFFTSize;
   spectrumJson["auto_bandwidth"] = sweep.sSpectrumSettings.bAutoBandwidth == TRUE;
   spectrumJson["bandwidth_hz"] = sweep.sSpectrumSettings.dwBandwidthInHz;
   spectrumJson["level_threshold"] = sweep.sSpectrumSettings.bLevelThreshold == TRUE;
   spectrumJson["threshold_dbm"] = sweep.sSpectrumSettings.fThresholdInDbm;
   spectrumJson["preamplifier"] = sweep.sSpectrumSettings.bPreamplifier == TRUE;
   spectrumJson["auto_attenuation"] = sweep.sSpectrumSettings.bAutoAttenuation == TRUE;
   spectrumJson["attenuation_db"] = sweep.sSpectrumSettings.bAttenuationInDb;
   sweepJson["spectrum"] = spectrumJson;

   json measTimeJson;
   measTimeJson["measurement_time_ns"] = sweep.sMeasurementTime.dwMeasTimeInNs;
   measTimeJson["detector_type"] = sweep.sMeasurementTime.eDetectorType;
   sweepJson["measurement_time"] = measTimeJson;

   json freqDetJson;
   freqDetJson["count_of_lines"] = sweep.sFrequencyDetector.dwCountOfLines;
   freqDetJson["detector_type"] = sweep.sFrequencyDetector.eDetectorType;
   sweepJson["frequency_detector"] = freqDetJson;

   json timeDetJson;
   timeDetJson["detector_type"] = sweep.sTimeDetector.eDetectorType;
   timeDetJson["detector_interval_type"] = sweep.sTimeDetector.eDetectorIntervalType;
   timeDetJson["time_parameter_ms"] = sweep.sTimeDetector.dwTimeParameterInMs;
   sweepJson["time_detector"] = timeDetJson;

   json markerJson;
   markerJson["use_marker"] = sweep.sMarker.bUseMarker == 1;
   markerJson["return_power_values"] = sweep.sMarker.bReturnsPowerValues == 1;
   sweepJson["marker"] = markerJson;

   resultJson["sweep_settings"] = sweepJson;

   return resultJson;
}
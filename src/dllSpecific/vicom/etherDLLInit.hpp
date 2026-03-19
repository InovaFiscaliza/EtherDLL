/**
 * @file etherDLLInit.hpp
 * @brief Initialization and connection functions for ViCom DLL interface
 * 
 * This header file contains the declarations for:
 * - ViCom connection management (VicomConnection struct)
 * - Default configuration parameters (DefaultDLLParam struct)
 * - Connection and disconnection functions (connectAPI, disconnectAPI)
 * - Configuration file management (newDefaultConfigFile, validDLLConfigParams)
 * 
 * @author mlnunes@anatel.gov.br
 * @date 2026-3-19
 * @version 1.3
 * 
 * @note Requires C++17 or later
 * @note Uses nlohmann/json library for JSON handling
 * @note Uses ViCom API headers from Rohde & Schwarz
 * 
 * Dependencies:
 * - ViComRFPowerScanInterface.h
 * - ViComGpsInterface.h
 * - etherDLLCodes.hpp
 * - etherDLLResponse.hpp
 * 
 * @ingroup vicom
 */

// etherDLLInit.hpp
#pragma once

// Include provided DLL libraries
#include "stdafx.h"
#include <atlstr.h>
#include "ViComRFPowerScanInterface.h"
#include "ViComGpsInterface.h"
#include "ViComRFPowerScanInterfaceData.h"
#include "ViComBasicInterface.h"
#include "ViComBasicInterfaceData.h"
#include "ViComLoader.h"
#include "ViComError.h"

// Include DLL specific libraries
#include "etherDLLCodes.hpp"
#include "etherDLLResponse.hpp"

// Include core EtherDLL libraries
#include "EtherDLLUtils.hpp"
#include "EtherDLLLog.hpp"
#include "EtherDLLConfig.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <filesystem>
#include <string>
#include <fstream>
#include <stdexcept>
#include <memory>

// ----------------------------------------------------------------------
// For convenience
// ----------------------------------------------------------------------
using json = nlohmann::json;
using namespace RohdeSchwarz::ViCom;
using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;
using namespace RohdeSchwarz::ViCom::GPS;

// ----------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------
extern spdlog::logger* loggerPtr;

// ----------------------------------------------------------------------
// Structure to hold ViCom connection objects
// This structure will hold the loader and interface pointers for the ViCom API
// @brief Manages Power Scan and GPS interfaces for the TSMW receiver
// @ingroup vicom_connection
// ----------------------------------------------------------------------
struct VicomConnection {
   std::unique_ptr<CViComLoader<CViComRFPowerScanInterface>> ps_loader;
   CViComRFPowerScanInterface* ps_pInterface;
   std::unique_ptr<CViComLoader<CViComGpsInterface>> gps_loader;
   CViComGpsInterface* gps_pInterface;
   std::string receiverModel;
   std::string serialNumber;
   std::string softwareVersion;
   std::string hardwareVersion;
   RohdeSchwarz::ViCom::RFPOWERSCAN::SSweepSettings sweepSettings;
   bool isConfigured = false;
};

// ----------------------------------------------------------------------
// Alias to be used by core EtherDLL code to pass connection parameters
// Name must not be changed
// May be defined as an alias to any DLL specific structure or to other data type
// May also be defined as a structure containing multiple parameters
// A default value must be defined as DEFAULT_DLL_CONNECTION_DATA for initialization purposes
// ----------------------------------------------------------------------
using DLLConnectionData = VicomConnection;

inline DLLConnectionData DEFAULT_DLL_CONNECTION_DATA{};

// ----------------------------------------------------------------------
// Define structure for default configuration parameters related to Vicom API
// Json like structure to centralize string parameters used to configure default parameters when using Vicom API.
// This avoid the need of users sending all parameters every time a command is issued.
// ----------------------------------------------------------------------
struct DefaultDLLParam {
   static constexpr const char* KEY = "dll_default";

   struct Station {
      static constexpr const char* KEY = "station";

      struct Address {
         static constexpr const char* KEY = "address";
         static constexpr const char* VALUE = "192.168.0.2";
      };
      struct ReceiverType {
         static constexpr const char* KEY = "receiver_type";
         static constexpr const char* VALUE = "TSMW";
      };
      struct ReceiverModel {
         static constexpr const char* KEY = "receiver_model";
         static constexpr const char* VALUE = "";
      };
      struct ReceiveSoftwareVersion {
         static constexpr const char* KEY = "receiver_software_version";
         static constexpr const char* VALUE = "";
      };
      struct ReceiverHardwareVersion {
         static constexpr const char* KEY = "receiver_hardware_version";
         static constexpr const char* VALUE = "";
      };
   };

   struct SweepSettings {
      static constexpr const char* KEY = "sweep_settings";
      
      static constexpr const char* FRONT_END_MASK = "frontEndMask";
      static constexpr const char* START_FREQ_HZ = "startFrequency";
      static constexpr const char* STOP_FREQ_HZ = "stopFrequency";
      static constexpr const char* REQ_RAW_DATA = "requireRawData";
      static constexpr const char* MAX_REPORTING_RATE = "maxReportingRate";
      static constexpr const char* MAX_DEVICE_MEAS_RATE = "maxDeviceMeasurementRate";
      static constexpr const char* WINDOW_TYPE = "windowType";
      static constexpr const char* FFT_SIZE = "fftSize";
      static constexpr const char* AUTO_BANDWIDTH = "autoBandwidth";
      static constexpr const char* BANDWIDTH_HZ = "bandwidth";
      static constexpr const char* LEVEL_THRESHOLD = "levelThreshold";
      static constexpr const char* THRESHOLD_DBM = "threshold";
      static constexpr const char* PREAMPLIFIER = "preamplifier";
      static constexpr const char* AUTO_ATTENUATION = "autoAttenuation";
      static constexpr const char* ATTENUATION_DB = "attenuation";
      static constexpr const char* MEAS_TIME_NS = "measurementTime";
      static constexpr const char* MEAS_DETECTOR_TYPE = "measurementDetectorType";
      static constexpr const char* FREQ_DETECTOR_LINES = "numBins";
      static constexpr const char* FREQ_DETECTOR_TYPE = "frequencyDetectorType";
      static constexpr const char* TIME_DETECTOR_TYPE = "timeDetectorType";
      static constexpr const char* TIME_DETECTOR_INTERVAL_TYPE = "timeDetectorIntervalType";
      static constexpr const char* TIME_PARAMETER_MS = "timeParameter";
      static constexpr const char* USE_MARKER = "useMarker";
      static constexpr const char* RETURN_POWER_VALUES = "returnPowerValues";
      static constexpr const char* SINGLE_SWEEP = "singleSweepMode";
   };
};

// ----------------------------------------------------------------------
// Build a default dll parameters JSON object from the DefaultDLLParam struct
// @brief Creates default configuration values for DLL parameters
// @param default_param JSON object to be populated (default: empty object)
// @return JSON object containing the default parameters
// @ingroup vicom_config
// ----------------------------------------------------------------------
inline json buildDLLDefaultParamJson(json default_param = json::object()) {

   default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::Address::KEY] = DefaultDLLParam::Station::Address::VALUE;
   default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::ReceiverType::KEY] = DefaultDLLParam::Station::ReceiverType::VALUE;
   default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::ReceiverModel::KEY] = DefaultDLLParam::Station::ReceiverModel::VALUE;
   default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::ReceiveSoftwareVersion::KEY] = DefaultDLLParam::Station::ReceiveSoftwareVersion::VALUE;
   default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::ReceiverHardwareVersion::KEY] = DefaultDLLParam::Station::ReceiverHardwareVersion::VALUE;

   return default_param;
}

// ----------------------------------------------------------------------
// Save default configuration file
// @brief Creates a new configuration file with default values
// @param filename Name of the configuration file to be created
// @throws std::runtime_error if file cannot be created or written
// @note Does not overwrite existing files or create folder structure
// @ingroup vicom_config
// ----------------------------------------------------------------------
inline void newDefaultConfigFile(const std::string& filename)
{

   json default_config = buildCoreDefaultConfigJson();
   default_config = buildDLLDefaultParamJson(default_config);

   std::filesystem::path currentPath = std::filesystem::current_path();
   std::filesystem::path fullPath = currentPath / filename;

   std::ofstream file(fullPath);

   try {
      std::ofstream file(filename);

      // Test if file is open
      if (!file.is_open()) {
         throw std::runtime_error("Failed to open/create file: " + fullPath.generic_string());
      }

      // Set exceptions to be thrown on failure
      file.exceptions(std::ios::failbit | std::ios::badbit);

      file << default_config.dump(4);
      file.close();
   }
   catch (const std::ios_base::failure& e) {
      throw std::runtime_error("I/O error when attempting to use file: " + fullPath.generic_string() + " - " + std::string(e.what()));
   }
   catch (const std::exception& e) {
      throw std::runtime_error("Failed to create configuration file: " + fullPath.generic_string() + " - " + std::string(e.what()));
   }
}

// ----------------------------------------------------------------------
// Test DLL configuration parameters
// @brief Validates the DLL configuration section in the JSON config
// @param config JSON object containing the configuration parameters
// @return true if configuration is valid, false otherwise
// @ingroup vicom_config
// ----------------------------------------------------------------------
inline bool validDLLConfigParams(const nlohmann::json& config)
{
   bool validConfig = true;
   using station_conf = DefaultDLLParam::Station;

   if (!config.contains(DefaultDLLParam::KEY) || !config[DefaultDLLParam::KEY].is_object()) {
      loggerPtr->error("No DLL configuration section found");
      return false;
   }

   const auto& dll_config = config[DefaultDLLParam::KEY];

   if (!dll_config.contains(station_conf::KEY) || !dll_config[station_conf::KEY].is_object()) {
      loggerPtr->error("No station configuration section found");
      return false;
   }

   const auto& station_config = dll_config[station_conf::KEY];

   if (station_config.contains(station_conf::Address::KEY)) {
      if (!station_config[station_conf::Address::KEY].is_string() || station_config[station_conf::Address::KEY].get<std::string>().empty()) {
         loggerPtr->error("Station address in configuration is invalid or empty");
         validConfig = false;
      }
   } else {
      loggerPtr->error("No station address found in configuration");
      validConfig = false;
   }

   return validConfig;
}

// ----------------------------------------------------------------------
// Create connection to ViCom DLL
// @brief Establishes connection to TSMW receiver via ViCom API
// @param stationConnData Connection data structure to be populated
// @param config JSON object containing the configuration parameters
// @return true if connection is successful, false otherwise
// @note Supports Power Scan and GPS interfaces
// @note If demo mode is enabled in config, skips actual connection
// @ingroup vicom_connection
// ----------------------------------------------------------------------
inline bool connectAPI(DLLConnectionData& stationConnData, const nlohmann::json& config)
{
   // Check if running in demo mode and skip connection if so
   if (config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::DemoMode::KEY].get<bool>()) {
      loggerPtr->warn("Starting EtherDLL service in DEMO mode. No connection to station will be attempted.");
      stationConnData.ps_loader = nullptr;
      stationConnData.ps_pInterface = nullptr;
      stationConnData.gps_loader = nullptr;
      stationConnData.gps_pInterface = nullptr;
      return true;
   }

   try
   {
      CViComError err;
      using station_conf = DefaultDLLParam::Station;

      json station_config = config[DefaultDLLParam::KEY][station_conf::KEY].get<json>();
      
      std::string ipAddressStr = station_config[station_conf::Address::KEY].get<std::string>();

      // For now, receiver type is hardcoded. This could be read from config.
      Receiver::Type receptor = Receiver::TSMW;

      // Power Scan Interface
      stationConnData.ps_loader = std::make_unique<CViComLoader<CViComRFPowerScanInterface>>(receptor);

      if (!stationConnData.ps_loader->Connect(err, ipAddressStr.c_str()))
      {
         CStringA ansiErrorString(err.GetErrorString());
         loggerPtr->error("No receiver found for Power Scan. Error: {}", ansiErrorString.GetString());
         stationConnData.ps_pInterface = nullptr;
         stationConnData.ps_loader.reset();
         return false;
      }
      
      loggerPtr->info("TSMW for Power Scan connected at address {}.", ipAddressStr);
      stationConnData.ps_pInterface = stationConnData.ps_loader->GetInterface(err);

      if (stationConnData.ps_pInterface == NULL)
      {
         CStringA ansiErrorString(err.GetErrorString());
         loggerPtr->error("Failed to get Power Scan interface. Error: {}", ansiErrorString.GetString());
         stationConnData.ps_pInterface = nullptr;
         stationConnData.ps_loader.reset();
         return false;
      }

      // Retrieve TSMW device information
      const SConnectedReceiverTable* pTable = stationConnData.ps_pInterface->GetBasicInterface().GetConnectedReceivers(err);
      if (pTable && pTable->dwCountOfReceivers > 0) {
         const SConnectedReceiverTable::SReceiver& receiver = pTable->Receivers[0];

         auto verToStr = [](DWORD ver) {
            return std::to_string((ver >> 24) & 0xFF) + "." +
               std::to_string((ver >> 16) & 0xFF) + "." +
               std::to_string((ver >> 8) & 0xFF) + "." +
               std::to_string(ver & 0xFF);
         };

         auto typeToStr = [](Receiver::Type type) {
            switch (type) {
            case Receiver::TSMU: return "TSMU";
            case Receiver::TSMU_H: return "TSMU_H";
            case Receiver::TSMQ: return "TSMQ";
            case Receiver::TSML_CW: return "TSML_CW";
            case Receiver::TSML_W: return "TSML_W";
            case Receiver::TSML_G: return "TSML_G";
            case Receiver::TSML_C: return "TSML_C";
            case Receiver::TSML_E: return "TSML_E";
            case Receiver::TSML_GW: return "TSML_GW";
            case Receiver::TSMW: return "TSMW";
            case Receiver::TSME: return "TSME";
            default: return "UNKNOWN";
            }
         };

         stationConnData.receiverModel = typeToStr(receiver.eReceiver);
         stationConnData.serialNumber = std::to_string(receiver.dwSerialNumber);
         stationConnData.softwareVersion = verToStr(receiver.dwSoftwareVersion);
         stationConnData.hardwareVersion = verToStr(receiver.dwHardwareVersion);
         loggerPtr->info("TSMW Device Info - Model: {}, Serial: {}, SW Version: {}, HW Version: {}",
            stationConnData.receiverModel, stationConnData.serialNumber, stationConnData.softwareVersion, stationConnData.hardwareVersion);
      }
      else {
         loggerPtr->warn("Failed to retrieve TSMW device information.");
      }

      // GPS Interface
      stationConnData.gps_loader = std::make_unique<CViComLoader<CViComGpsInterface>>(receptor);
      if (!stationConnData.gps_loader->Connect(err, ipAddressStr.c_str()))
      {
         CStringA ansiErrorString(err.GetErrorString());
         loggerPtr->error("No receiver found for GPS. Error: {}", ansiErrorString.GetString());
         stationConnData.ps_pInterface = nullptr;
         stationConnData.ps_loader.reset();
         stationConnData.gps_pInterface = nullptr;
         stationConnData.gps_loader.reset();
         return false;
      }

      loggerPtr->info("TSMW for GPS connected at address {}.", ipAddressStr);
      stationConnData.gps_pInterface = stationConnData.gps_loader->GetInterface(err);

      if (stationConnData.gps_pInterface == NULL)
      {
         CStringA ansiErrorString(err.GetErrorString());
         loggerPtr->error("Failed to get GPS interface. Error: {}", ansiErrorString.GetString());
         stationConnData.ps_pInterface = nullptr;
         stationConnData.ps_loader.reset();
         stationConnData.gps_pInterface = nullptr;
         stationConnData.gps_loader.reset();
         return false;
      }
   }
   catch (const CViComError& err)
   {
      CStringA ansiErrorString(err.GetErrorString());
      loggerPtr->error("CViComError ({}) \"{}\"", err.GetErrorCode(), ansiErrorString.GetString());
      stationConnData.ps_pInterface = nullptr;
      stationConnData.ps_loader.reset();
      stationConnData.gps_pInterface = nullptr;
      stationConnData.gps_loader.reset();
      return false;
   }
   catch (const std::exception& e)
   {
      loggerPtr->error("An exception occurred during connectAPI: {}", e.what());
      stationConnData.ps_pInterface = nullptr;
      stationConnData.ps_loader.reset();
      stationConnData.gps_pInterface = nullptr;
      stationConnData.gps_loader.reset();
      return false;
   }

   return true;
}

// ----------------------------------------------------------------------
// Disconnect from ViCom DLL
// @brief Closes connections to Power Scan and GPS interfaces
// @param stationConnData Connection data structure containing active connections
// @return true if disconnection is successful, false otherwise
// @ingroup vicom_connection
// ----------------------------------------------------------------------
inline bool disconnectAPI(DLLConnectionData& stationConnData)
{
   if (stationConnData.ps_loader)
   {
      try
      {
         CViComError err;
         stationConnData.ps_loader->Disconnect(err);
         loggerPtr->info("Disconnected from Power Scan station.");
      }
      catch (const CViComError& err)
      {
         CStringA ansiErrorString(err.GetErrorString());
         loggerPtr->error("CViComError on disconnect ({}) \"{}\"", err.GetErrorCode(), ansiErrorString.GetString());
      }
      catch (const std::exception& e)
      {
         loggerPtr->error("An exception occurred during disconnectAPI for Power Scan: {}", e.what());
         return false;
      }
      stationConnData.ps_pInterface = nullptr;
   }
   if (stationConnData.gps_loader)
   {
      try
      {
         CViComError err;
         stationConnData.gps_loader->Disconnect(err);
         loggerPtr->info("Disconnected from GPS station.");
      }
      catch (const CViComError& err)
      {
         CStringA ansiErrorString(err.GetErrorString());
         loggerPtr->error("CViComError on disconnect ({}) \"{}\"", err.GetErrorCode(), ansiErrorString.GetString());
      }
      catch (const std::exception& e)
      {
         loggerPtr->error("An exception occurred during disconnectAPI for GPS: {}", e.what());
         return false;
      }
      stationConnData.gps_pInterface = nullptr;
   }
   return true;
}
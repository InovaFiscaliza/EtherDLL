/**
* @file etherDLLInit.hpp
*
* @brief Heder file for configuration file creation with default values
* 
* This header file contains the implementation for creating a new configuration file
* It also hold specific DLL linking instructions maintaing EtherDLL base configurations clean of specifi DLL dependencies.
*
*
* * @author fslobao
* * @date 2025-09-10
* * @version 1.0
*
* * @note Requires C++17 or later
* * @note Uses nlohmann/json library for JSON handling
*
**/


// ----------------------------------------------------------------------
/**
 * Conditional library linking for ViComAPI based on build configuration
**/
//#ifdef _WIN32
//	#pragma comment (lib, "ViComRFPOWERSCANw.lib") //RELEASE/DEBUG 32Bits
//#else
//	#ifdef NDEBUG
//		#pragma comment (lib, "ViComRFPOWERSCANw.lib") //RELEASE 64Bits
//	#else
//		#pragma comment (lib, "ViComRFPOWERSCANw_d.lib") //DEBUG 64Bits
//	#endif
//#endif

// ----------------------------------------------------------------------
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

// For convenience
using json = nlohmann::json;
using namespace RohdeSchwarz::ViCom;
using namespace RohdeSchwarz::ViCom::RFPOWERSCAN;
using namespace RohdeSchwarz::ViCom::GPS;


// Global variables
extern spdlog::logger* loggerPtr;

// ----------------------------------------------------------------------
/** @brief Define a structure to hold Vicom connection objects
 *
 * This structure will hold the loader and interface pointers for the Vicom API
**/
struct VicomConnection {
	std::unique_ptr<CViComLoader<CViComRFPowerScanInterface>> ps_loader;
	CViComRFPowerScanInterface* ps_pInterface;
	std::unique_ptr<CViComLoader<CViComGpsInterface>> gps_loader;
	CViComGpsInterface* gps_pInterface;
};

// ----------------------------------------------------------------------
/** @brief Alias to be used by core EtherDLL code to pass connection parameters
 * 
 * \! Name must not be changed \n
 *    May be defined as an alias to any DLL specific structure or to other data type
 *    May also be defined as a structure containing multiple parameters
 *    A default value must be defined as DEFAULT_DLL_CONNECTION_DATA for initialization purposes
**/
using DLLConnectionData = VicomConnection;

inline DLLConnectionData DEFAULT_DLL_CONNECTION_DATA{};

// ----------------------------------------------------------------------
/** @brief Define structure for default configuration parameters reated to Vicom API
 *
 * Json like structure to centralize string parameters used to configure default parameters when using Vicom API.
 * This avoid the need of users sending all parameters every time a command is issued.
**/
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
	};
};

// ----------------------------------------------------------------------
/** @brief Build a default dll parameters JSON object from the DefaultDLLParam struct
 *
 * @param  nlohmann::json: JSON object to be populated with default parameters, default is empty object
 * @return nlohmann::json: JSON object containing the default parameters
 * @throws NO EXCEPTION HANDLING
**/
json buildDLLDefaultParamJson(json default_param = json::object()) {

	default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::Address::KEY] = DefaultDLLParam::Station::Address::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::ReceiverType::KEY] = DefaultDLLParam::Station::ReceiverType::VALUE;

	return default_param;
}

// ----------------------------------------------------------------------
/**
 * @brief Save default configuration file
 *
 * Create a new configuration file with default values, saving it to the specified filename.
 * It does not overwrite existing files or create the folder structure.
 *
 * @param filename: Name of the configuration file to be created
 * @return void
 * @throws std::runtime_error if file cannot be created or written
 **/
void newDefaultConfigFile(const std::string& filename)
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
/** @brief Test DLL configuration parameters
 * 
 * @param config: JSON object containing the configuration parameters
 * @return bool: True if configuration is valid, false otherwise
 * @throws NO EXCEPTION HANDLING
**/
bool validDLLConfigParams(const nlohmann::json& config)
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
/** @brief Create a connection object to the DLL and test it.
*
* @param stationConnData: DLLConnectionData structure to be populated with connection parameters
* @param config: JSON object containing the configuration parameters
* @return bool: True if connection is successful, false otherwise
* @throws NO EXCEPTION HANDLING
*/
bool connectAPI(DLLConnectionData& stationConnData, const nlohmann::json& config)
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
			return false;
		}
		
		loggerPtr->info("TSMW for Power Scan connected at address {}.", ipAddressStr);
		stationConnData.ps_pInterface = stationConnData.ps_loader->GetInterface(err);

		if (stationConnData.ps_pInterface == NULL)
		{
			CStringA ansiErrorString(err.GetErrorString());
			loggerPtr->error("Failed to get Power Scan interface. Error: {}", ansiErrorString.GetString());
			return false;
		}

		// GPS Interface
		stationConnData.gps_loader = std::make_unique<CViComLoader<CViComGpsInterface>>(receptor);
		if (!stationConnData.gps_loader->Connect(err, ipAddressStr.c_str()))
		{
			CStringA ansiErrorString(err.GetErrorString());
			loggerPtr->error("No receiver found for GPS. Error: {}", ansiErrorString.GetString());
			return false;
		}

		loggerPtr->info("TSMW for GPS connected at address {}.", ipAddressStr);
		stationConnData.gps_pInterface = stationConnData.gps_loader->GetInterface(err);

		if (stationConnData.gps_pInterface == NULL)
		{
			CStringA ansiErrorString(err.GetErrorString());
			loggerPtr->error("Failed to get GPS interface. Error: {}", ansiErrorString.GetString());
			return false;
		}
	}
	catch (const CViComError& err)
	{
		CStringA ansiErrorString(err.GetErrorString());
		loggerPtr->error("CViComError ({}) \"{}\"", err.GetErrorCode(), ansiErrorString.GetString());
		return false;
	}
	catch (const std::exception& e)
	{
		loggerPtr->error("An exception occurred during connectAPI: {}", e.what());
		return false;
	}

	return true;
}

// ----------------------------------------------------------------------
/** @brief Disconnect station and socket clients
*
* @param stationConnData: DLLConnectionData structure containing connection parameters
* @return bool: True if disconnection is successful, false otherwise
* @throws NO EXCEPTION HANDLING
*/
bool disconnectAPI(DLLConnectionData& stationConnData)
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
	}
	return true;
}

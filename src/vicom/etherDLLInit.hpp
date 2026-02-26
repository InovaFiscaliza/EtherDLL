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

// Global variables
extern spdlog::logger* loggerPtr;

// ----------------------------------------------------------------------
/** @brief Define a structure to hold Vicom connection objects
 *
 * This structure will hold the loader and interface pointers for the Vicom API
**/
struct VicomConnection {
	std::unique_ptr<CViComLoader<CViComRFPowerScanInterface>> loader;
	CViComRFPowerScanInterface* pInterface;
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
		
		static constexpr const char* FRONT_END_MASK = "front_end_mask";
		static constexpr unsigned long FRONT_END_MASK_V = 1;

		static constexpr const char* START_FREQ_HZ = "start_freq_hz";
		static constexpr double START_FREQ_HZ_V = 87.8e6;

		static constexpr const char* STOP_FREQ_HZ = "stop_freq_hz";
		static constexpr double STOP_FREQ_HZ_V = 107.8e6;

		static constexpr const char* REQ_RAW_DATA = "req_raw_data";
		static constexpr bool REQ_RAW_DATA_V = false;
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
	
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::SweepSettings::KEY][DefaultDLLParam::SweepSettings::FRONT_END_MASK] = DefaultDLLParam::SweepSettings::FRONT_END_MASK_V;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::SweepSettings::KEY][DefaultDLLParam::SweepSettings::START_FREQ_HZ] = DefaultDLLParam::SweepSettings::START_FREQ_HZ_V;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::SweepSettings::KEY][DefaultDLLParam::SweepSettings::STOP_FREQ_HZ] = DefaultDLLParam::SweepSettings::STOP_FREQ_HZ_V;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::SweepSettings::KEY][DefaultDLLParam::SweepSettings::REQ_RAW_DATA] = DefaultDLLParam::SweepSettings::REQ_RAW_DATA_V;

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
		stationConnData.loader = nullptr;
		stationConnData.pInterface = nullptr;
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

		stationConnData.loader = std::make_unique<CViComLoader<CViComRFPowerScanInterface>>(receptor);

		if (!stationConnData.loader->Connect(err, ipAddressStr.c_str()))
		{
			CStringA ansiErrorString(err.GetErrorString());
			loggerPtr->error("No receiver found. Error: {}", ansiErrorString.GetString());
			return false;
		}
		
		loggerPtr->info("TSMW connected at address {}.", ipAddressStr);
		stationConnData.pInterface = stationConnData.loader->GetInterface(err);

		if (stationConnData.pInterface == NULL)
		{
			CStringA ansiErrorString(err.GetErrorString());
			loggerPtr->error("Failed to get interface. Error: {}", ansiErrorString.GetString());
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
	if (stationConnData.loader)
	{
		try
		{
			CViComError err;
			stationConnData.loader->Disconnect(err);
			loggerPtr->info("Disconnected from station.");
		}
		catch (const CViComError& err)
		{
			CStringA ansiErrorString(err.GetErrorString());
			loggerPtr->error("CViComError on disconnect ({}) \"{}\"", err.GetErrorCode(), ansiErrorString.GetString());
		}
		catch (const std::exception& e)
		{
			loggerPtr->error("An exception occurred during disconnectAPI: {}", e.what());
			return false;
		}
	}
	return true;
}

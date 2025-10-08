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
 * Conditional library linking for ScorpioAPI based on build configuration
**/
#ifdef _X86_
	#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE/DEBUG 32Bits
#else
	#ifdef NDEBUG
		#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE 64Bits
	#else
		#pragma comment (lib, "ScorpioAPIDlld.lib") //DEBUG 64Bits
	#endif
#endif

// ----------------------------------------------------------------------
#pragma once

// Include provided DLL libraries
#include "StdAfx.h"
#include <ScorpioAPITypes.h>
#include <ScorpioAPIDll.h>
#include "MoreEquipCtrlMsg.h"
#include "OccupSpectConnect.h"

// Include DLL specific libraries
#include "etherDLLCodes.hpp"
#include "etherDLLResponse.hpp"

// Include core EtherDLL libraries
#include "EtherDLLUtils.hpp"
#include "EtherDLLLog.hpp"
#include "EtherDLLConfig.hpp"
#include "EtherDLLCodes.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <filesystem>
#include <string>
#include <fstream>
#include <stdexcept>

// For convenience
using json = nlohmann::json;

// Global variables
extern spdlog::logger* loggerPtr;

// 
// ----------------------------------------------------------------------
// 
/** @brief Alias to be used by core EtherDLL code to pass connection parameters
 * 
 * \! Name must not be changed \n
 *    May be defined as an alias to any DLL specific structure or to other data type
 *    May also be defined as a structure containing multiple parameters
 *    A default value must be defined as DEFAULT_DLL_CONNECTION_DATA for initialization purposes
**/
using DLLConnectionData = unsigned long;

const DLLConnectionData DEFAULT_DLL_CONNECTION_DATA = 0;


// ----------------------------------------------------------------------
/** @brief Define structure for default configuration parameters reated to Scorpio API
 *
 * Json like structure to centralize string parameters used to configure default parameters when using Scorpio API.
 * This avoid the need of users sending all parameters every time a command is issued.
**/
struct DefaultDLLParam {
	static constexpr const char* KEY = "dll_default";

	struct Station {
		static constexpr const char* KEY = "station";

		struct Address {
			static constexpr const char* KEY = "address";
			static constexpr const char* VALUE = "172.24.3.15";
		};
		struct Port {
			static constexpr const char* KEY = "port";
			static constexpr int VALUE = 3303;
		};
		struct Timeout {
			static constexpr const char* KEY = "timeoutS";
			static constexpr int VALUE = 10;
		};
	};

	struct OCCRequest {
		static constexpr const char* KEY = "OCCRequest";

		struct durationMethod {
			static constexpr const char* KEY = "durationMethod";
			static constexpr const char* VALUE = "Fixed";
		};
		struct thresholdMethod {
			static constexpr const char* KEY = "thresholdMethod";
			static constexpr const char* VALUE = "Noise Riding";
		};
		struct storageTime {
			static constexpr const char* KEY = "storageTime";
			static constexpr unsigned long VALUE = 1; // hours
		};
		struct measurementTime {
			static constexpr const char* KEY = "measurementTime";
			static constexpr unsigned long VALUE = 1; // seconds
		};
		struct confidenceLevel {
			static constexpr const char* KEY = "confidenceLevel";
			static constexpr unsigned char VALUE = 90; // percent
		};
		struct desiredAccuracy {
			static constexpr const char* KEY = "desiredAccuracy";
			static constexpr unsigned char VALUE = 90; // percent
		};
		struct occPrimaryThreshold {
			static constexpr const char* KEY = "occPrimaryThreshold";
			static constexpr short VALUE = 10; // dBuV/m
		};
		struct occupancyMinGap {
			static constexpr const char* KEY = "occupancyMinGap";
			static constexpr unsigned long VALUE = 1; // seconds
		};
		struct ant {
			static constexpr const char* KEY = "ant";
			static constexpr unsigned long VALUE = 1; // Antenna 1
		};
		struct occflags {
			static constexpr const char* KEY = "occflags";

			struct occupancyVsChannel {
				static constexpr const char* KEY = "occupancyVsChannel";
				static constexpr bool VALUE = true;
			};
			struct eFieldVsChannel {
				static constexpr const char* KEY = "eFieldVsChannel";
				static constexpr bool VALUE = false;
			};
			struct occupancyVsTimeOfDay {
				static constexpr const char* KEY = "occupancyVsTimeOfDay";
				static constexpr bool VALUE = false;
			};
			struct msglengthVsChannel {
				static constexpr const char* KEY = "msglengthVsChannel";
				static constexpr bool VALUE = false;
			};
			struct msglengthDistribution {
				static constexpr const char* KEY = "msglengthDistribution";
				static constexpr bool VALUE = false;
			};
			struct spectrogram {
				static constexpr const char* KEY = "spectrogram";
				static constexpr bool VALUE = false;
			};
			struct timegram {
				static constexpr const char* KEY = "timegram";
				static constexpr bool VALUE = false;
			};
		};
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
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::Port::KEY] = DefaultDLLParam::Station::Port::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::Station::KEY][DefaultDLLParam::Station::Timeout::KEY] = DefaultDLLParam::Station::Timeout::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::durationMethod::KEY] = DefaultDLLParam::OCCRequest::durationMethod::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::thresholdMethod::KEY] = DefaultDLLParam::OCCRequest::thresholdMethod::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::storageTime::KEY] = DefaultDLLParam::OCCRequest::storageTime::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::measurementTime::KEY] = DefaultDLLParam::OCCRequest::measurementTime::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::confidenceLevel::KEY] = DefaultDLLParam::OCCRequest::confidenceLevel::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::desiredAccuracy::KEY] = DefaultDLLParam::OCCRequest::desiredAccuracy::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occPrimaryThreshold::KEY] = DefaultDLLParam::OCCRequest::occPrimaryThreshold::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occupancyMinGap::KEY] = DefaultDLLParam::OCCRequest::occupancyMinGap::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::ant::KEY] = DefaultDLLParam::OCCRequest::ant::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occflags::KEY][DefaultDLLParam::OCCRequest::occflags::occupancyVsChannel::KEY] = DefaultDLLParam::OCCRequest::occflags::occupancyVsChannel::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occflags::KEY][DefaultDLLParam::OCCRequest::occflags::eFieldVsChannel::KEY] = DefaultDLLParam::OCCRequest::occflags::eFieldVsChannel::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occflags::KEY][DefaultDLLParam::OCCRequest::occflags::occupancyVsTimeOfDay::KEY] = DefaultDLLParam::OCCRequest::occflags::occupancyVsTimeOfDay::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occflags::KEY][DefaultDLLParam::OCCRequest::occflags::msglengthVsChannel::KEY] = DefaultDLLParam::OCCRequest::occflags::msglengthVsChannel::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occflags::KEY][DefaultDLLParam::OCCRequest::occflags::msglengthDistribution::KEY] = DefaultDLLParam::OCCRequest::occflags::msglengthDistribution::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occflags::KEY][DefaultDLLParam::OCCRequest::occflags::spectrogram::KEY] = DefaultDLLParam::OCCRequest::occflags::spectrogram::VALUE;
	default_param[DefaultDLLParam::KEY][DefaultDLLParam::OCCRequest::KEY][DefaultDLLParam::OCCRequest::occflags::KEY][DefaultDLLParam::OCCRequest::occflags::timegram::KEY] = DefaultDLLParam::OCCRequest::occflags::timegram::VALUE;

	return default_param;
}

// ----------------------------------------------------------------------
/**
 * @brief Save default configuration file
 *
 * Create a new configuration file with default values, saving it to the specified filename.
 * It does not overwrite existing files or create the folder structure.
 * Default configuration is included from a separate file to keep the code clean.
 * File has the JSON content defined within a raw string literal. R"(...)".
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

	if (config.contains(DefaultDLLParam::KEY) == false
		|| config[DefaultDLLParam::KEY].is_object() == false) {
		loggerPtr->error("No DLL configuration section found");
		return false;
	}

	if (config[DefaultDLLParam::KEY].contains(station_conf::KEY) == false
		|| config[DefaultDLLParam::KEY][station_conf::KEY].is_object() == false) {
		loggerPtr->error("No station configuration section found");
		return false;
	}

	json station_config = config[DefaultDLLParam::KEY][station_conf::KEY].get<json>();

	// Check station configuration parameters
	if (station_config.contains(station_conf::Address::KEY)) {
		if (station_config[station_conf::Address::KEY].is_string()) {
			if (station_config[station_conf::Address::KEY].get<std::string>().empty()) {
				loggerPtr->error("Station address in configuration is empty");
				validConfig = false;
			}
		}
		else {
			loggerPtr->error("Invalid type for station address in configuration");
			validConfig = false;
		}
	}
	else {
		loggerPtr->error("No station address found in configuration");
		validConfig = false;
	}

	if (station_config.contains(station_conf::Port::KEY)) {
		if (station_config[station_conf::Port::KEY].is_number_integer()) {
			int port = station_config[station_conf::Port::KEY].get<int>();
			if (port < 1 || port > 65535) {
				loggerPtr->error("Station port in configuration is out of valid range (1-65535)");
				validConfig = false;
			}
		}
		else {
			loggerPtr->error("Invalid type for station port in configuration");
			validConfig = false;
		}
	}
	else {
		loggerPtr->error("No station port found in configuration");
		validConfig = false;
	}

	if (station_config.contains(station_conf::Timeout::KEY)) {
		if (station_config[station_conf::Timeout::KEY].is_number_integer()) {
			int timeout = station_config[station_conf::Timeout::KEY].get<int>();
			if (timeout < 1) {
				loggerPtr->error("Station timeout in configuration must be a positive integer");
				validConfig = false;
			}
		}
		else {
			loggerPtr->error("Invalid type for station timeout in configuration");
			validConfig = false;
		}
	}
	else {
		loggerPtr->error("No station timeout found in configuration");
		validConfig = false;
	}

	return validConfig;
}

// ----------------------------------------------------------------------
/** @brief Create a connection object to the DLL and test it.
*
* Alias DLLConnectionData should be used to pass the connection data structure
* This is specific to the DLL and should not directly accessed by core EtherDLL code.
* but rather passed through the alias to other DLL specific functions, as required.
*
* @param config: JSON object containing the configuration parameters
* @param station: DLLConnectionData structure to be populated with connection parameters
* @param logger: spdlog logger object for logging messages
* @return bool: True if connection is successful, false otherwise
* @throws NO EXCEPTION HANDLING
*/
bool connectAPI(DLLConnectionData& stationConnID, const nlohmann::json& config)
{
	std::string message;
	SScorpioAPIClient station;

	using station_conf = DefaultDLLParam::Station;
	json station_config = config[DefaultDLLParam::KEY][station_conf::KEY].get<json>();
	
	// Check if running in demo mode and skip connection if so
	if (config[edll::DefaultConfig::Service::KEY][edll::DefaultConfig::Service::DemoMode::KEY].get<bool>()) {
		loggerPtr->warn("Starting EtherDLL service in DEMO mode. No connection to station will be attempted.");
		stationConnID = DEFAULT_DLL_CONNECTION_DATA;
		return true;
	}

	// Prepare station data structure from the config data
	std::string hostNameStr = station_config[station_conf::Address::KEY].get<std::string>();
	station.hostName = stringToWString(hostNameStr);

	std::string portStr = std::to_string(station_config[station_conf::Port::KEY].get<int>());
	station.port = stringToWString(portStr);

	station.sendTimeout = (unsigned long)(station_config[station_conf::Timeout::KEY].get<int>() * 1000);

	ERetCode errCode;

	// Create the connection object
	errCode = ScorpioAPICreate(
		stationConnID,
		station,
		OnErrorFunc,
		OnDataFunc,
		OnRealTimeDataFunc);

	// Handle the error code from object creation
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Object associated with the API was not created: " + ERetCodeToString(errCode);
		loggerPtr->error(message);
		return false;
	}

	//Test connection to the station
	SCapabilities StationCapabilities;

	errCode = RequestCapabilities(stationConnID, StationCapabilities);

	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Failed to connect to " + hostNameStr + " [" + portStr + "]. Erro " + ERetCodeToString(errCode);
		loggerPtr->error(message);
		return false;
	}
	
	message = "Connected to station " + hostNameStr + " [" + portStr + "]";
	loggerPtr->info(message);

	return true;
}

// ----------------------------------------------------------------------
/** @brief Disconnect station and socket clients
*
* @param stationConnID: DLLConnectionData structure containing connection parameters
* @return bool: True if disconnection is successful, false otherwise
* @throws NO EXCEPTION HANDLING
*/
bool disconnectAPI(DLLConnectionData& stationConnID)
{

	ERetCode errCode = Disconnect(stationConnID);
	loggerPtr->warn("Disconnecting station returned:" + ERetCodeToString(errCode));

	// TODO: DLL function not returning API_SUCCESS - Need to investigate
	if (errCode != ERetCode::API_SUCCESS)
	{
		loggerPtr->error("Error disconnecting from station " + ERetCodeToString(errCode));
		return false;
	}

	loggerPtr->info("Disconnected from station");
	return true;
}
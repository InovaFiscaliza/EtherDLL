/**
* @file etherDLLconfig.cpp
* 
* @brief Configuration file creation with default values
* * This source file contains the implementation for creating a new configuration file
* * with default values, saving it to a specified filename.
* 
* * @author fslobao
* * @date 2025-09-10
* * @version 1.0
* 
* * @note Requires C++11 or later
* * @note Uses nlohmann/json library for JSON handling
* 
* * * Dependencies:
* * - nlohmann/json.hpp
* * - scorpioDLLConfig.h
* 
**/


// ----------------------------------------------------------------------
#pragma once

// Include core EtherDLL libraries
#include "EtherDLLUtils.hpp"

// Include DLL specific libraries
#include "etherDLLConfig.hpp"
#include "etherDLLCodes.hpp"
#include "etherDLLResponse.hpp"

// Include provided DLL libraries
#include <ScorpioAPITypes.h>
#include <ScorpioAPIDll.h>

// Include general C++ libraries
#include <string>
#include <fstream>
#include <stdexcept>

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// For convenience
using json = nlohmann::json;


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

    std::string default_config =
		#include "etherDLLConfig.json.inc"
        ;
    try {
        std::ofstream file(filename);

        // Verificação se arquivo está ser aberto
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir/criar o arquivo: " + filename);
        }

        // Habilitar exceções para operações futuras ofstream
        file.exceptions(std::ios::failbit | std::ios::badbit);

        file << default_config;
        file.close();
    }    
    catch (const std::ios_base::failure& e) {
        throw std::runtime_error("Erro de I/O no arquivo: " + filename + " - " + e.what());
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Falha ao criar arquivo de configuração: " + std::string(e.what()));
    }
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
bool connectAPI(DLLConnectionData& stationConnID, const nlohmann::json& config, spdlog::logger& logger)
{
	std::string message;
	SScorpioAPIClient station;

	// Prepare station data structure from the config data
	std::string hostNameStr = config["station"]["address"].get<std::string>();
	station.hostName = stringToWString(hostNameStr);

	std::string portStr = std::to_string(config["station"]["port"].get<int>());
	station.port = stringToWString(portStr);

	station.sendTimeout = (unsigned long)(config["station"]["timeout_s"].get<int>()) * 1000;

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
		logger.error(message);
		return false;
	}

	logger.info("Object creation successful");

	// Test connection to the station
	SCapabilities StationCapabilities;

	errCode = RequestCapabilities(stationConnID, StationCapabilities);

	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Failed to connect to " + hostNameStr + " [" + portStr + "]. Erro " + ERetCodeToString(errCode);
		logger.error(message);
		return false;
	}

	message = "Connected to station " + hostNameStr + " [" + portStr + "]";
	logger.info(message);
	return true;
}

// ----------------------------------------------------------------------
/** @brief Disconnect station and socket clients
* 
* @param stationConnID: DLLConnectionData structure containing connection parameters
* @param logger: spdlog logger object for logging messages
* @return bool: True if disconnection is successful, false otherwise
* @throws NO EXCEPTION HANDLING
*/
bool disconnectAPI(DLLConnectionData& stationConnID, spdlog::logger& logger)
{

	ERetCode errCode = Disconnect(stationConnID);
	logger.warn("Disconnecting station returned:" + ERetCodeToString(errCode));

	// TODO: DLL function not returning API_SUCCESS - Need to investigate
	if (errCode != ERetCode::API_SUCCESS)
	{
		logger.error("Error disconnecting from station " + ERetCodeToString(errCode));
		return false;
	}
	
	logger.info("Disconnected from station");
	return true;
}

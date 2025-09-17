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
#include "etherDLLConfig.hpp"

#include <string>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <ScorpioAPIDll.h>

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
void newDefaultConfigFile(const std::string& filename) {


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
/*
	Create a connection object to the DLL and connect to it.
*/
void connectAPI(const nlohmann::json& config, StationInfo& station, Logger& logger) {
	// Create a local copy of APIserverId. This is necessary because TCI methods update the APIserverId value to the next available ID.
	// unsigned long NextServerId = APIserverId;

	// Hostname as simple string, extracted from th JSON configuration file
	std::string hostNameStr = config["station"]["address"].get<std::string>();
	station.hostName = stringToWString(hostNameStr);

	// Port as simple string, extracted from the JSON configuration file where it is defined as a number
	std::string portStr = std::to_string(config["station"]["port"].get<int>());
	station.port = stringToWString(portStr);

	// Timeout as unsigned long, extracted from the JSON configuration file in second and converted to miliseconds
	station.sendTimeout = (unsigned long)(config["station"]["timeout_s"].get<int>()) * 1000;

	// Error code using API ERetCode enum
	ERetCode errCode;

	// Create the connection object
	errCode = ScorpioAPICreate(
		APIserverId,
		station,
		OnErrorFunc,
		OnDataFunc,
		OnRealTimeDataFunc);

	// Error message string to be used in the logger
	std::string message;

	// Handle the error code from object creation
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Object associated with the API was not created: " + ERetCodeToString(errCode);
		logEtherDLL.error(message);
		//running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
		return;
	}
	else
	{
		logEtherDLL.info("Object creation successful");
	}

	// NextServerId = APIserverId;

	// Once the object was successfully created, test connection to the station

	errCode = RequestCapabilities(APIserverId, StationCapabilities);

	// Handle the error code from station connection
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Failed to connect to " + hostNameStr + ": " + ERetCodeToString(errCode);
		logEtherDLL.error(message);
		//		running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
		return;
	}
	else
	{
		message = "Connected to station " + hostNameStr;
		logEtherDLL.info(message);
	}
}
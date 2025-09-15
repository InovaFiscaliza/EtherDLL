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
void newDefaultConfigFile(const std::string& filename) {

	std::string default_config = R"(
{
  "log": {
    "console": {
      "enable": true,
      "level": "trace"
    },
    "file": {
      "enable": true,
      "level": "trace",
      "path": "log.txt"
    }
  },
  "station": {
    "address": "172.24.3.15",
    "port": 3303,
    "timeout_s": 10
  },
  "service": {
    "command": {
      "port": 30000,
      "timeout_s": 10000,
      "sleep_ms": 100,
      "check_period": 10
    },
    "stream": {
      "port": 30001,
      "timeout_s": 10000,
      "sleep_ms": 500,
      "check_period": 200
    },
    "error": {
      "port": 30002,
      "timeout_s": 10000,
      "sleep_ms": 500,
      "check_period": 200
    },
    "realtime": {
      "port": 30003,
      "timeout_s": 10000,
      "sleep_ms": 500,
      "check_period": 200
    },
    "audio": {
      "port": 30004,
      "timeout_s": 60000
    },
    "simulated": true
  },
  "default": {
    "OCCRequest": {
      "durationMethod": "Fixed",
      "thresholdMethod": "Noise Riding",
      "storageTime": 1,
      "measurementTime": 1,
      "confidenceLevel": 90,
      "desiredAccuracy": 90,
      "occPrimaryThreshold": 10,
      "occupancyMinGap": 1,
      "occflags": {
        "occupancyVsChannel": true,
        "eFieldVsChannel": false,
        "occupancyVsTimeOfDay": false,
        "msglengthVsChannel": false,
        "msglengthDistribution": false,
        "spectrogram": false,
        "timegram": false
      },
      "ant": 1
    }
  }
})";

    try {
        std::ofstream file(filename);
        
		// Verificação se arquivo pode ser aberto
        if (!file.is_open()) {
            throw std::runtime_error("Não foi possível abrir o arquivo: " + filename);
        }
        
		// Habilitar exceções para operações futuras ofstream
        file.exceptions(std::ios::failbit | std::ios::badbit);
        
        file << default_config;
        
    } catch (const std::ios_base::failure& e) {
        throw std::runtime_error("Erro de I/O no arquivo: " + filename + " - " + e.what());
    } catch (const std::exception& e) {
        throw std::runtime_error("Falha ao criar arquivo de configuração: " + std::string(e.what()));
    }
}

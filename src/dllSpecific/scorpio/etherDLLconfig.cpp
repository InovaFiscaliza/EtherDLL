/**
* @file scorpioDLLconfig.cpp
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


#include <nlohmann/json.hpp>
#include "scorpioDLLConfig.h"

using json = nlohmann::json;

// ----------------------------------------------------------------------
/**
 * @brief Save default configuration file
 *
 * Create a new configuration file with default values, saving it to the specified filename.
 *
 * @param filename: Name of the configuration file to be created
 * @return void
 * @throws `runtime_error` if file cannot be created or written
 **/
void newDefaultConfigFile(const std::string& filename) {
	// Create a JSON object with default values

	json jsonObj;

	jsonObj["log"]["console"]["enable"] = true;
	jsonObj["log"]["console"]["level"] = "trace";

	jsonObj["log"]["file"]["enable"] = true;
	jsonObj["log"]["file"]["level"] = "trace";
	jsonObj["log"]["file"]["path"] = "log.txt";

	jsonObj["station"]["address"] = "166.139.112.178";
	jsonObj["station"]["port"] = 3303;
	jsonObj["station"]["timeout_s"] = 10;

	jsonObj["service"]["command"]["port"] = 3000;
	jsonObj["service"]["command"]["timeout_s"] = 10000;
	jsonObj["service"]["command"]["sleep_ms"] = 100;
	jsonObj["service"]["command"]["check_period"] = 10;

	jsonObj["service"]["stream"]["port"] = 3001;
	jsonObj["service"]["stream"]["timeout_s"] = 10000;
	jsonObj["service"]["stream"]["sleep_ms"] = 500;
	jsonObj["service"]["stream"]["check_period"] = 200;

	jsonObj["service"]["error"]["port"] = 3002;
	jsonObj["service"]["error"]["timeout_s"] = 10000;
	jsonObj["service"]["error"]["sleep_ms"] = 500;
	jsonObj["service"]["error"]["check_period"] = 200;

	jsonObj["service"]["realtime"]["port"] = 3003;
	jsonObj["service"]["realtime"]["timeout_s"] = 10000;
	jsonObj["service"]["realtime"]["sleep_ms"] = 500;
	jsonObj["service"]["realtime"]["check_period"] = 200;

	jsonObj["service"]["audio"]["port"] = 3004;
	jsonObj["service"]["audio"]["timeout_s"] = 60000;

	jsonObj["service"]["simulated"] = true;

	try {
		std::ofstream file(filename);

		// Verificar se o arquivo foi aberto com sucesso
		if (!file.is_open()) {
			throw std::runtime_error("Não foi possível abrir o arquivo: " + filename);
		}

		// Escrever JSON com formatação
		file << jsonObj.dump(4); // 4 espaços de indentação

		// Verificar se a escrita foi bem-sucedida
		if (file.fail()) {
			throw std::runtime_error("Erro ao escrever no arquivo: " + filename);
		}

		// Forçar flush e fechar explicitamente
		file.flush();
		file.close();

		if (file.fail()) {
			throw std::runtime_error("Erro ao finalizar escrita do arquivo: " + filename);
		}

	}
	catch (const std::exception& e) {
		// Log do erro ou re-throw dependendo da necessidade
		throw std::runtime_error("Falha ao criar arquivo de configuração: " + std::string(e.what()));
	}
}

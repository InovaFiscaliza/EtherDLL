/**
 * @file EtherDLL.cpp
 * @brief Main source file for EtherDLL service
 **
 * * @author fslobao
 * * @date 2025-09-12
 * * @version 1.0
 *
 * * @note Designed for Windows OS using VSStudio and MSVC compiler for x86 architecture
 * * @note Requires C++11 or later
 * * @note Uses nlohmann/json library for JSON handling
 * * @note Uses spdlog library for logging
 *
 * * * Dependencies:
 * * - string
 * * - externalCodes.h
**/

// ----------------------------------------------------------------------
// Include the standard C++ headers
#include <mutex>
#include <future>
#include <atomic>
#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include <csignal>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Include additional libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general EtherDLL headers
#include "EtherDLLLog.hpp"
#include "EtherDLLAudio.hpp"
#include "EtherDLLUtils.hpp"
#include "EtherDLLConstants.hpp"
#include "EtherDLLClient.hpp"

// Include to DLL specific headers
#include "etherDLLCodes.hpp"
#include "etherDLLConfig.hpp"
#include "etherDLLRequest.hpp"
#include "etherDLLResponse.hpp"
#include "filesystem"

 // Libs for socket
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// For convenience
using json = nlohmann::json;


// ----------------------------------------------------------------------
/*
	Global variables related to the application
*/
// Logger object
spdlog::logger log;

// Code to represent the cause for not running
edll::INT_CODE interruptionCode = edll::Code::RUNNING;

// ----------------------------------------------------------------------
/*
	Handle interruptio signals 'ctrl+C' and 'kill'
*/
static void signalHandler(int signal) {

	if (signal == SIGINT)
	{
		interruptionCode = edll::Code::CTRL_C_INTERRUPT;
		log.critical("Received interruption signal (ctrl+C)");
	}
	else if (signal == SIGTERM)
	{
		interruptionCode = edll::Code::KILL_INTERRUPT;
		log.critical("Received termination signal (kill)");
	}
	else 	{
		std::string message = "Received unknown signal. #LttOS: " + std::to_string(signal);
		log.warn(message);
	}
}

// ----------------------------------------------------------------------
/*
	Register the signal callback handlers
*/
static void registerSignalHandlers() {
	std::signal(SIGINT, signalHandler);  // Handles Ctrl+C
	std::signal(SIGTERM, signalHandler); // Handles kill command
}


// ----------------------------------------------------------------------
/**
 * @brief Read the configuration file and return a JSON object
 *
 * @param config: JSON object containing configuration
 * @return nlohmann::json: JSON object containing the configuration
 * @throws std::invalid_argument if the file cannot be read or parsed
 * @throws std::runtime_error if the file does not exist and cannot be created
**/
static json readConfigFile(std::string fileName) {

	json config;

	try {
		std::ifstream configFile(fileName);

		if (!configFile.is_open()) {
			std::cout << "Configuration file not found, creating default: " + fileName << std::endl;
			newDefaultConfigFile(fileName);
		}

		configFile.open(fileName);
		config = json::parse(configFile);
		configFile.close();
	}
	catch (const json::parse_error& e) {
		throw std::invalid_argument("JSON parsing error: " + std::string(e.what()));
	}
	catch (const std::exception& e) {
		throw std::invalid_argument("Error reading configuration file: " + std::string(e.what()));
	}

	return config;
}


// ----------------------------------------------------------------------
/**
 * @brief Print help message to the std::cout
 * 
 * @param None
 * @return None
 * @throws None
**/
static void print_help() {
	std::cout << std::endl;
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	std::cout << "EtherDLL Version " + std::string(edll::VERSION) << std::endl;
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	std::cout << "Usage: EtherDLL.exe [options] [config_file.json]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -h, --help       Show this help message and exit" << std::endl;
	std::cout << "  -f, --file	     JSON configuration file to use instead of default" << std::endl;
	std::cout << std::endl;
	std::cout << " * If argument is provided, looks for the default filename 'EtherDLLConfig.json'" << std::endl;
	std::cout << " * If the configuration file does not exist, a default one will be created." << std::endl;
	std::cout << std::endl;
}


// ----------------------------------------------------------------------
/**
 * @brief Handle input arguments and return the configuration file name
 * 
 * @param argc: Number of arguments received from command line
 * @param argv: Array of argument strings received from command line
 * @return std::string: Configuration file name
 * @throws std::invalid_argument if the arguments are invalid
**/
static  std::string handleInputArguments(int argc, char* argv[]) {
	// test if the application was called with any command line arguments
	if (argc > 1) {
		std::string arg1 = argv[1];

		//Make extended format initial characters similar to short format, e.g. change "--help" to "-help"
		char switchChar = '-';
		for (size_t i = 0; i < 2; i++) {
			if (arg1[i] == '-') continue;
			arg1[i] = switchChar;
		}

		switch (switchChar) {
			case 'h':
				// intended fall through
			case 'H': {
				print_help();
				if (argc != 2) {
					throw std::invalid_argument("Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 2)");
				}
				exit(0);
			}
			case 'f':
				// intended fall through
			case 'F': {
				if (argc != 3) {
					print_help();
					throw std::invalid_argument("Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 2)");
				}
	
				std::string fileName = std::string(argv[2]);
				return fileName;
			}
			default: {
				print_help();
				throw std::invalid_argument("Unknown argument: " + arg1);
			}
		}
	}
	// No arguments provided, use default configuration file name
	return std::string(edll::DEFAULT_CONFIG_FILENAME);
}


// ----------------------------------------------------------------------
/*
* @brief Main function
* 
* Initialize the application, read configuration,
* Connect to the DLL API and start communication
* Open socket ports and wait for requests
* 
* * @param argc: Number of arguments received from command line
* * @param argv: Array of argument strings received from command line
* * @return int: Exit code
*/
int main(int argc, char* argv[]) {

	registerSignalHandlers();

	std::string configFileName = handleInputArguments(argc, argv);

	json config = readConfigFile(configFileName);

	auto logPtr = initializeLog(config["log"].get<json>());
    if (logPtr) {
        log = *logPtr;
    }

	// Initialize DLL connection
	DLLConnectionData DLLConnID = DEFAULT_DLL_CONNECTION_DATA;
	if (!connectAPI(DLLConnID, config, log)) {
		interruptionCode = edll::Code::STATION_ERROR;
		return static_cast<int>(interruptionCode);
	}

	messageQueue request;
	messageQueue response;
	while (interruptionCode == edll::Code::RUNNING)
	{
		SOCKET clientSocket = establishClientCommunication(config, interruptionCode, log);

		// Start threads for client communication
		auto receiveFuture = std::async(std::launch::async, [&]() {
			clientRequestToDLL(clientSocket, config, request, interruptionCode, log);
			return true; 
			});

		// Start thread for sending responses to client
		auto sendFuture = std::async(std::launch::async, [&]() {
			DLLResponseToClient(clientSocket, config, response, interruptionCode, log);
			return true;
			});

		// Hold the main loop to process messages between client and DLL
		bool activeComm = true;
		bool activeProces = true;
		while (interruptionCode == edll::Code::RUNNING && activeProces && activeComm)
		{
			activeProces = processMessages(stationConnID, config, log);

			activeComm = !(receiveFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready &&
				sendFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready);

			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"].value("sleep_ms", edll::DEFAULT_SLEEP_MS)));
		}

		closesocket(clientSocket);
		if (!activeComm) log.info("Client disconnected.");
	}
	// Close the connection
	if (!disconnectAPI(DLLConnID, log)) {
		log.error("Failed to disconnect from station.");
	}
	log.info("Service stopped.");
	log.flush();

	return static_cast<int>(interruptionCode);
}

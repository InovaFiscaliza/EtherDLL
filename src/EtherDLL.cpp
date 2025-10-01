/**
 * @file EtherDLL.cpp
 * @brief Main source file for EtherDLL service
 **
 * * @author fslobao
 * * @date 2025-09-12
 * * @version 1.0
 *
 * * @note Designed for Windows OS using VSStudio and MSVC compiler for x86 architecture
 * * @note Requires C++17 or later
 * * @note Uses nlohmann/json library for JSON handling
 * * @note Uses spdlog library for logging
 *
 * * * Dependencies:
 * * - string
 * * - externalCodes.h
**/

// ----------------------------------------------------------------------
// Include to DLL specific headers
#include "etherDLLCodes.hpp"
#include "etherDLLInit.hpp"
#include "etherDLLRequest.hpp"
#include "etherDLLResponse.hpp"

// Include core EtherDLL headers
#include "EtherDLLLog.hpp"
#include "EtherDLLUtils.hpp"
#include "EtherDLLConstants.hpp"
#include "EtherDLLClient.hpp"

// Include additional libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include the standard C++ headers
#include <mutex>
#include <future>
#include <atomic>
#include <string>
#include <csignal>
#include <fstream>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

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
// Code to represent the cause for not running
edll::INT_CODE interruptionCode = edll::Code::RUNNING;

// Message queues
MessageQueue request;
MessageQueue response;

// Logger pointer
spdlog::logger* loggerPtr = nullptr;

// ----------------------------------------------------------------------
/*
	Handle interruptio signals 'ctrl+C' and 'kill'
*/
static void signalHandler(int signal) {

	if (signal == SIGINT)
	{
		interruptionCode = edll::Code::CTRL_C_INTERRUPT;
		loggerPtr->critical("Received interrupt signal (Ctrl+C)");
	}
	else if (signal == SIGTERM)
	{
		interruptionCode = edll::Code::KILL_INTERRUPT;
		loggerPtr->critical("Received termination signal (kill)");
	}
	else 	{
		std::string message = "Received unknown signal. #LttOS: " + std::to_string(signal);
		loggerPtr->warn(message);
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
	std::cout << " * If argument is provided, EtherDLL try to read the file 'EtherDLLConfig.json'" << std::endl;
	std::cout << " * If the configuration file does not exist, a default one will be created." << std::endl;
	std::cout << std::endl;
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
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
					throw std::invalid_argument("Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 1)");
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
	return std::string(edll::DefaultConfig::DEFAULT_CONFIG_FILENAME);
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

	std::string configFileName = handleInputArguments(argc, argv);

	json config = readConfigFile(configFileName);

	//TODO: validate log parameteres and use get method
	std::string log_name = config[edll::DefaultConfig::Log::KEY].value(edll::DefaultConfig::Log::Name::KEY, edll::DefaultConfig::Log::Name::VALUE);

	auto logger_ptr = std::make_shared<spdlog::logger>(log_name);
	loggerPtr = logger_ptr.get();

	initializeLog(config, *logger_ptr);

	registerSignalHandlers();

	// Initialize DLL connection
	DLLConnectionData DLLConnID = DEFAULT_DLL_CONNECTION_DATA;
	if (!connectAPI(DLLConnID, config, *logger_ptr)) {
		interruptionCode = edll::Code::STATION_ERROR;
		logger_ptr->error("Exiting since no station was available.");
		return static_cast<int>(interruptionCode);
	}

	// Add these at the top with other global variables:
	std::mutex threadCompletionMutex;
	std::condition_variable threadCompletionCV;
	std::atomic<bool> anyThreadCompleted = false;

	while (interruptionCode == edll::Code::RUNNING)
	{
		// Inicialise ClientConn object to wait for a client connection
		ClientConn clientConn(config, interruptionCode, *logger_ptr);

		// Reset completion flag
		anyThreadCompleted = false;

		// Lambda to signal completion
		auto signalCompletion = [&threadCompletionMutex, &anyThreadCompleted, &threadCompletionCV]() {
				{
					std::lock_guard<std::mutex> lock(threadCompletionMutex);
					anyThreadCompleted = true;
				}
			threadCompletionCV.notify_one();
			};

		// Start threads with completion signaling
		auto requestComFuture = std::async(std::launch::async, [&]() {
			clientConn.clientRequestToDLL(request);
			signalCompletion();
			return true;
			});

		auto requestProcFuture = std::async(std::launch::async, [&]() {
			processRequestQueue(DLLConnID, request, interruptionCode, *logger_ptr);
			signalCompletion();
			return true;
			});

		auto responseConFuture = std::async(std::launch::async, [&]() {
			clientConn.DLLResponseToClient(response);
			signalCompletion();
			return true;
			});

		{
			std::unique_lock<std::mutex> lock(threadCompletionMutex);
			threadCompletionCV.wait(lock, [&]() {
				return anyThreadCompleted.load() || interruptionCode != edll::Code::RUNNING;
				});
		}

		logger_ptr->info("Service interrupted");

		// Clean up
		if (!clientConn.isConnected()) {
			logger_ptr->info("Client disconnected.");
		}

		clientConn.closeConnection();
	}

	// Close the connection
	if (!disconnectAPI(DLLConnID, *logger_ptr)) {
		logger_ptr->error("Failed to disconnect from station.");
	}
	logger_ptr->info("Service stopped.");
	logger_ptr->flush();

	return static_cast<int>(interruptionCode);
}

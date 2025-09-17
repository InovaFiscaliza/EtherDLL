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
#include <thread>
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
#include <EtherDLLLog.hpp>
#include <EtherDLLAudio.hpp>
#include <EtherDLLUtils.hpp>
#include <EtherDLLConstants.hpp>

// Include to DLL specific headers
#include <etherDLLCodes.hpp>
#include <etherDLLConfig.hpp>
#include <etherDLLRequest.hpp>
#include <etherDLLResponse.hpp>
#include <filesystem>

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

// Atomic flag to signal application error
std::atomic<bool> running = true; 

// Code to represent the cause for not running
std::atomic<int> interruptionCode = edll::Code::RUNNING;

// Vector used for the command queue
std::vector<std::string> requestQueue;
std::mutex requestQueueMutex;

// Vector used for the data stream output
std::vector<std::string> responseQueue;
std::mutex responseQueueMutex;


// ----------------------------------------------------------------------
/*
	Handle interruptio signals 'ctrl+C' and 'kill'
*/
static void signalHandler(int signal) {

	if (signal == SIGINT)
	{
		running = false;
		interruptionCode = edll::Code::CTRL_C_INTERRUPT;
		log.critical("Received interruption signal (ctrl+C)");
	}
	else if (signal == SIGTERM)
	{
		running = false;
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

			// Reopen the newly created file
			configFile.open(fileName);
			if (!configFile.is_open()) {
				throw std::runtime_error("Failed to create or open configuration file: " + fileName);
			}
		}

		config = json::parse(configFile);
		configFile.close(); // Explicitly close the file
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

		//if used "--" change to "-"
		if (arg1.length() > 2) {
			arg1 = arg1.substr(1);
		}

		switch (arg1[2]) {
			case '-h':
				// intended fall through
			case '-H': {
				print_help();
				exit(0);
			}
			case '-f':
				// intended fall through
			case '-F':
				if (argc != 3) {
					print_help();
					throw std::invalid_argument("Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 2)");
				}
				else
				{
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

	// DLLConnectionData must be defined in specific DLL file etherDLLConfig.hpp
	// It is an alias to a structure or data type used to pass connection parameters to the DLL API functions
	DLLConnectionData station;

	if ( !connectAPI(station, config, log) ) {
		log.error("Failed to connect to station. Exiting...");
		return static_cast<int>(edll::Code::STATION_ERROR);
	}
		

	// Start thread for the command channel socket service. This thread will listen for incoming commands and place then in the command queue
	std::thread commandThread(socketHandle,
		"Control service",
		edll::Code::COMMAND_ERROR,
		config["service"]["command"]["port"].get<int>(),
		config["service"]["command"]["timeout_s"].get<int>(),
		handleCommandConnection,
		log);

	// Start thread for the stream channel socket service. This thread will listen for incoming connections and stream data back to the client
	std::thread streamThread(socketHandle,
		"Stream service",
		edll::Code::STREAM_ERROR,
		config["service"]["stream"]["port"].get<int>(),
		config["service"]["stream"]["timeout_s"].get<int>(),
		handleStreamConnection);

	// Start thread for the stream channel socket service. This thread will stream error back to the client
	std::thread errorThread(socketHandle,
		"Error service",
		edll::Code::STREAM_ERROR,
		config["service"]["error"]["port"].get<int>(),
		config["service"]["error"]["timeout_s"].get<int>(),
		handleErrorConnection);

	// Start thread for the stream channel socket service. This thread will stream real time data to the client
	std::thread realtimeThread(socketHandle,
		"Real time service",
		edll::Code::STREAM_ERROR,
		config["service"]["realtime"]["port"].get<int>(),
		config["service"]["realtime"]["timeout_s"].get<int>(),
		handleRealTimeConnection);

	// Start thread for the audio socket service.
	std::thread audioThread(socketHandle,
		"Audio service",
		edll::Code::STREAM_ERROR,
		config["service"]["audio"]["port"].get<int>(),
		config["service"]["audio"]["timeout_s"].get<int>(),
		handleAudioConnection);
		
	// Main loop to process commands received
	while (running.load()) {

		if (!requestQueue.empty()) {
			std::string command;
			{
				std::lock_guard<std::mutex> lock(requestQueueMutex);
				command = requestQueue.back();
				requestQueue.pop_back();
			}
			log.info("Processing command: " + command);
			
			unsigned long requestID = 0;
			json jsonObj = json::parse(command);

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	log.info("Service will shutdown...");

	// Join threads before exiting
	if (commandThread.joinable()) {
		commandThread.join();
	}
	if (streamThread.joinable()) {
		streamThread.join();
	}
	if (errorThread.joinable()) {
		errorThread.join();
	}
	if (realtimeThread.joinable()) {
		realtimeThread.join();
	}
	if (audioThread.joinable()) {
		audioThread.join();
	}

	// Close the connection
	if (!disconnectAPI(station, log)) {
		log.error("Failed to disconnect from station.");
	}
	log.info("Service stopped.");
	log.flush();

	return static_cast<int>(interruptionCode);
}

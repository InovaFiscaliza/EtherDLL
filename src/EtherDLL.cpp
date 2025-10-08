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
#include "EtherDLLConfig.hpp"
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
		std::cout << "JSON parsing error: " + std::string(e.what());
		throw std::invalid_argument("Invalid JSON format in configuration file: " + fileName);
	}
	catch (const std::exception& e) {
		std::cout << "Error reading configuration file: " + std::string(e.what());
		throw std::invalid_argument("Error reading configuration file: " + fileName);
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
					std::cout << "Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 1)" << std::endl;
					throw std::invalid_argument("Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 1)");
				}
				exit(0);
			}
			case 'f':
				// intended fall through
			case 'F': {
				if (argc != 3) {
					print_help();
					std::cout << "Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 2)" << std::endl;
					throw std::invalid_argument("Too many arguments provided. (Provided " + std::to_string(argc) + " arguments, expected 2)");
				}
	
				std::string fileName = std::string(argv[2]);
				return fileName;
			}
			default: {
				print_help();
				std::cout << "Unknown argument: " + arg1 << std::endl;
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

	// Initialize Winsock
	WSADATA wsaData;
	int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaResult != 0) {
		std::cerr << "WSAStartup failed with error: " << wsaResult << std::endl;
		return static_cast<int>(edll::Code::SERVICE_ERROR);
	}

	std::string configFileName = handleInputArguments(argc, argv);

	json config = readConfigFile(configFileName);

	testLogConfig(config);

	std::string log_name = config[edll::DefaultConfig::Log::KEY].value(edll::DefaultConfig::Log::Name::KEY, edll::DefaultConfig::Log::Name::VALUE);
	auto logger_ptr = std::make_shared<spdlog::logger>(log_name);
	loggerPtr = logger_ptr.get();

	initializeLog(config, *logger_ptr);

	loggerPtr->flush();

	registerSignalHandlers();

	if (!validDLLConfigParams(config)) {
		logger_ptr->error("Exiting due to invalid DLL specific configuration parameters.");
		WSACleanup();
		return static_cast<int>(edll::Code::SERVICE_ERROR);
	}
	if (!validServiceParams(config)) {
		logger_ptr->error("Exiting due to invalid Service configuration parameters.");
		WSACleanup();
		return static_cast<int>(edll::Code::SERVICE_ERROR);
	}

	// Add these at the top with other global variables:
	std::mutex threadCompletionMutex;
	std::condition_variable threadCompletionCV;
	std::atomic<bool> anyThreadCompleted = false;


	DLLConnectionData DLLConnID = DEFAULT_DLL_CONNECTION_DATA;

	if (!connectAPI(DLLConnID, config)) {
		logger_ptr->error("Error establishing DLL connection.");
		interruptionCode = edll::Code::STATION_ERROR;
	}

	while (interruptionCode == edll::Code::RUNNING)
	{
		// Initialize ClientConn object to wait for a client connection
		ClientConn clientConn(config, interruptionCode, *logger_ptr);

		if (!clientConn.isConnected()) {
			if (interruptionCode == edll::Code::RUNNING) {
				logger_ptr->error("Error establishing client connection. Retrying in 5 seconds");
				std::this_thread::sleep_for(std::chrono::seconds(5));
				continue;
			}
			else {
				break;
			}
		}

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

		// Start threads for each parallel task
		auto requestComFuture = std::async(std::launch::async, [&]() {
			logger_ptr->debug("Starting thread to receive client request and populate request queue");
			clientConn.clientRequestToDLL(request, response);
			signalCompletion();
			logger_ptr->debug("Finished thread to receive client request and populate request queue");
			return true;
			});

		auto requestProcFuture = std::async(std::launch::async, [&]() {
			logger_ptr->debug("Starting thread that send requests from queue to DLL");
			processRequestQueue(DLLConnID, request, response, interruptionCode);
			signalCompletion();
			logger_ptr->debug("Finished thread that send requests from queue to DLL");
			return true;
			});

		auto responseComFuture = std::async(std::launch::async, [&]() {
			logger_ptr->debug("Starting thread that sends DLL response to client");
			clientConn.DLLResponseToClient(response);
			signalCompletion();
			logger_ptr->debug("Finished thread that sends DLL response to client");
			return true;
			});

		auto pingComFuture = std::async(std::launch::async, [&]() {
			logger_ptr->debug("Starting thread that send ping messages to test client connection");
			clientConn.pingClient(response);
			signalCompletion();
			logger_ptr->debug("Finished thread that send ping messages to test client connection");
			return true;
			});

		// Wait for any thread to complete or for an interruption signal
		{
			std::unique_lock<std::mutex> lock(threadCompletionMutex);
			threadCompletionCV.wait(lock, [&]() {
				return anyThreadCompleted.load() || interruptionCode != edll::Code::RUNNING;
				});
		}

		logger_ptr->info("Service interrupted");


		std::vector<std::future<bool>*> futures = {
			&requestComFuture, &requestProcFuture,
			&responseComFuture, &pingComFuture
		};

		// Check each future for exceptions
		for (auto* future : futures) {
			if (future->valid()) {
				try {
					if (future->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
						future->get(); // Consume any exceptions
					}
				}
				catch (const std::exception& e) {
					logger_ptr->warn("Thread exception during cleanup: {}", e.what());
				}
			}
		}

		// Close client connection if still open
		if (!clientConn.isConnected()) {
			logger_ptr->info("Client disconnected.");
		}
		else
		{
			clientConn.closeConnection();
		}
	}

	if (!disconnectAPI(DLLConnID, *logger_ptr)) {
		logger_ptr->error("Failed to disconnect from station.");
	}

	logger_ptr->info("Service stopped.");
	logger_ptr->flush();

	// Cleanup Winsock
	WSACleanup();

	return static_cast<int>(interruptionCode);
}

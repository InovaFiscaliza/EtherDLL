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
#include "etherDLLInit.hpp"
#include "etherDLLRequest.hpp"
#include "etherDLLDataProcess.hpp"

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
	Global variables
*/
// Code to represent the cause for not running
edll::INT_CODE interruptionCode = edll::Code::RUNNING;

// Message queues
MessageQueue request;
MessageQueue response;
MessagePreprocessor preprocessor;


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

	testLogConfig(config);

	std::string log_name = config[edll::DefaultConfig::Log::KEY].value(edll::DefaultConfig::Log::Name::KEY, edll::DefaultConfig::Log::Name::VALUE);
	auto logger_ptr = std::make_shared<spdlog::logger>(log_name);
	loggerPtr = logger_ptr.get();

	initializeLog(config, *logger_ptr);

	loggerPtr->flush();

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

	// Initialize Winsock
	WSADATA wsaData;
	int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaResult != 0) {
		std::cerr << "WSAStartup failed with error: " << wsaResult << std::endl;
		return static_cast<int>(edll::Code::SERVICE_ERROR);
	}

	// Add these at the top with other global variables:
	std::mutex threadCompletionMutex;
	std::condition_variable threadCompletionCV;
	std::atomic<bool> anyThreadCompleted = false;

	// Connect to the DLL API

	DLLConnectionData DLLConnID = DEFAULT_DLL_CONNECTION_DATA;

	if (!connectAPI(DLLConnID, config)) {
		logger_ptr->error("Error establishing DLL connection.");
		interruptionCode = edll::Code::STATION_ERROR;
	}

	// Initialize preprocessor
	preprocessor.initialize(config, *logger_ptr);


	// Main service loop
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
			processRequestQueue(DLLConnID, request, preprocessor, response, interruptionCode);
			signalCompletion();
			logger_ptr->debug("Finished thread that send requests from queue to DLL");
			return true;
			});

		auto responseComFuture = std::async(std::launch::async, [&]() {
			logger_ptr->debug("Starting thread that sends DLL response to client");
			clientConn.DLLResponseToClient(preprocessor, response);
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

	if (interruptionCode != edll::Code::STATION_ERROR) {
		if (!disconnectAPI(DLLConnID)) {
			logger_ptr->error("Failed to disconnect from station.");
		}
	}

	logger_ptr->info("Service stopped.");
	logger_ptr->flush();

	// Cleanup Winsock
	WSACleanup();

	return static_cast<int>(interruptionCode);
}

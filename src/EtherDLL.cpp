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
#include "dllSpecific/vicom/etherDLLInit.hpp"
#include "etherDLLRequest.hpp"

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
#include <windows.h>
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
std::atomic<edll::INT_CODE> interruptionCode = edll::Code::RUNNING;
static_assert(std::atomic<edll::INT_CODE>::is_always_lock_free,
	"Signal handlers require a lock-free interruption code");

// Logger pointer
spdlog::logger* loggerPtr = nullptr;

// ----------------------------------------------------------------------
/*
	Record process shutdown requests without performing cleanup in callback context.
*/
// Keep CRT signal handlers limited to a lock-free state update. Logging,
// queue operations, and condition-variable notifications belong to workers.
static void signalHandler(int signal) {
	if (signal == SIGINT) {
		interruptionCode.store(edll::Code::CTRL_C_INTERRUPT, std::memory_order_relaxed);
	}
	else if (signal == SIGTERM) {
		interruptionCode.store(edll::Code::KILL_INTERRUPT, std::memory_order_relaxed);
	}
}

// Windows console events are handled on a system-created thread. This callback
// only records the request; the main thread performs orderly session shutdown.
static BOOL WINAPI consoleControlHandler(DWORD controlType) {
	switch (controlType) {
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
		interruptionCode.store(edll::Code::CTRL_C_INTERRUPT, std::memory_order_relaxed);
		return TRUE;
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		interruptionCode.store(edll::Code::KILL_INTERRUPT, std::memory_order_relaxed);
		return TRUE;
	default:
		return FALSE;
	}
}

// ----------------------------------------------------------------------
/*
	Register minimal CRT and Windows console shutdown callbacks.
*/
static void registerSignalHandlers() {
	if (!SetConsoleCtrlHandler(consoleControlHandler, TRUE)) {
		loggerPtr->warn("Failed to register Windows console control handler. EC:{}", GetLastError());
	}
	if (std::signal(SIGINT, signalHandler) == SIG_ERR) {
		loggerPtr->warn("Failed to register SIGINT handler.");
	}
	if (std::signal(SIGTERM, signalHandler) == SIG_ERR) {
		loggerPtr->warn("Failed to register SIGTERM handler.");
	}
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


	//DLLConnectionData DLLConnID(DEFAULT_DLL_CONNECTION_DATA);
	DLLConnectionData DLLConnID{};

	if (!connectAPI(DLLConnID, config)) {
		logger_ptr->error("Error establishing DLL connection.");
		interruptionCode = edll::Code::STATION_ERROR;
	}

	while (interruptionCode == edll::Code::RUNNING)
	{
		ClientSession session;

		// Initialize ClientConn object to wait for a client connection
		ClientConn clientConn(config, interruptionCode, session.stopRequested, *logger_ptr);

		if (!clientConn.isConnected()) {
			if (interruptionCode == edll::Code::RUNNING) {
				logger_ptr->error("Error establishing client connection. Retrying in 5 seconds");
				const auto retryDeadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
				while (interruptionCode == edll::Code::RUNNING &&
					std::chrono::steady_clock::now() < retryDeadline) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				continue;
			}
			else {
				break;
			}
		}

		auto runSessionTask = [&](const std::string& taskName, auto task) {
			SessionWorkerCompletionGuard completionGuard(session);
			logger_ptr->debug("Starting {}", taskName);
			try {
				task();
			}
			catch (const std::exception& e) {
				logger_ptr->error("{} failed: {}", taskName, e.what());
			}
			catch (...) {
				logger_ptr->error("{} failed with an unknown exception", taskName);
			}
			logger_ptr->debug("Finished {}", taskName);
			return true;
		};

		// Start threads for each parallel task
		auto requestComFuture = std::async(std::launch::async, [&]() {
			return runSessionTask("client request receiver", [&]() {
				clientConn.clientRequestToDLL(session.request, session.response);
			});
		});

		auto requestProcFuture = std::async(std::launch::async, [&]() {
			return runSessionTask("request processor", [&]() {
				processRequestQueue(DLLConnID, session, interruptionCode);
			});
		});

		auto responseComFuture = std::async(std::launch::async, [&]() {
			return runSessionTask("client response sender", [&]() {
				clientConn.DLLResponseToClient(session.response);
			});
		});

		auto pingComFuture = std::async(std::launch::async, [&]() {
			return runSessionTask("client ping sender", [&]() {
				clientConn.pingClient(session.response);
			});
		});

		// Wait for any thread to complete or for an interruption signal
		{
			std::unique_lock<std::mutex> lock(session.completionMutex);
			while (!session.workerCompleted.load(std::memory_order_acquire) &&
				interruptionCode == edll::Code::RUNNING) {
				// The short timeout lets the main thread observe a process signal;
				// the signal handler does not touch session-owned synchronization.
				session.completionCondition.wait_for(lock, std::chrono::milliseconds(100));
			}
		}

		if (interruptionCode == edll::Code::RUNNING) {
			logger_ptr->info("Client session ended; cleaning up its workers.");
		}
		else {
			logger_ptr->info("Service shutdown requested; cleaning up client-session workers.");
		}

		// Stop all workers in this client session while keeping the service-level
		// interruption code unchanged for ordinary client disconnects.
		session.requestStop();
		clientConn.shutdownConnection();

		std::vector<std::future<bool>*> futures = {
			&requestComFuture, &requestProcFuture,
			&responseComFuture, &pingComFuture
		};

		// get() waits for every async worker and consumes any uncaught exception.
		for (auto* future : futures) {
			if (future->valid()) {
				try {
					future->get();
				}
				catch (const std::exception& e) {
					logger_ptr->warn("Thread exception during cleanup: {}", e.what());
				}
			}
		}

		// Close the socket only after no worker can still be using it.
		if (!clientConn.isConnected()) {
			logger_ptr->info("Client disconnected.");
		}
		else
		{
			clientConn.closeConnection();
		}
	}

	if (!disconnectAPI(DLLConnID)) {
		logger_ptr->error("Failed to disconnect from station.");
	}

	logger_ptr->info("Service stopped.");
	logger_ptr->flush();

	// Cleanup Winsock
	WSACleanup();

	return static_cast<int>(interruptionCode);
}

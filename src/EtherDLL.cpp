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
// JSON configuration object
json config;

// Logger object
spdlog::logger log;

// Atomic flag to signal application error
std::atomic<bool> running{ true }; 

// Code to represent the cause for not running
std::atomic<int> interruptionCode{ edll::Code::RUNNING };

// Vector used for the command queue
std::vector<std::string> commandQueue;

// Vector used for the data stream output
std::vector<std::string> streamBuffer;

// Vector used for the data error output
std::vector<std::string> errorBuffer;

// Vector used for the real time output
std::vector<std::string> realtimeBuffer;

// Mutex to protect the command queue
std::mutex MCCommandMutex;
std::mutex MCstreamMutex;
std::mutex MCStationMutex;


// ----------------------------------------------------------------------
/*
	Handle interruptio signals 'ctrl+C' and 'kill'
*/
void signalHandler(int signal) {

	if (signal == SIGINT)
	{
		running.store(false);
		interruptionCode.store(mcs::Code::CTRL_C_INTERRUPT);
		logEtherDLL.warn("Received interruption signal (ctrl+C)");
	}
	else if (signal == SIGTERM)
	{
		running.store(false);
		interruptionCode.store(mcs::Code::KILL_INTERRUPT);
		logEtherDLL.warn("Received termination signal (kill)");
	}
	else 	{
		std::string message = "Received unknown signal. #LttOS: " + std::to_string(signal);
		logEtherDLL.warn(message);
	}
}

// ----------------------------------------------------------------------
/*
	Register the signal callback handlers
*/
void registerSignalHandlers() {
	std::signal(SIGINT, signalHandler);  // Handles Ctrl+C
	std::signal(SIGTERM, signalHandler); // Handles kill command
}

// ----------------------------------------------------------------------
/*
	Receive commands from clients and push them to the command queue, to be sent to the DLL API.
*/
void handleCommandConnection(SOCKET clientSocket, std::string name) {
	char buffer[1024];

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead > 0) {
			std::string command(buffer, bytesRead);
			{
				std::lock_guard<std::mutex> lock(MCCommandMutex);
				commandQueue.push_back(command);
			}
			
			std::string ack = mcs::Form::ACK + 
				std::to_string(commandQueue.size()) +
				"}"+
				mcs::Form::MSG_END;
			iResult = send(clientSocket, ack.c_str(), (int)(ack.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logEtherDLL.warn(name + " ACK send failed. EC:" + std::to_string(WSAGetLastError()));
				logEtherDLL.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				// test if the connection is still alive
				iResult = send(clientSocket,
					mcs::Form::PING,
					static_cast<int>(strlen(mcs::Form::PING)),
					0);
				if (iResult == SOCKET_ERROR) {
					logEtherDLL.warn(name + " PING send failed. EC:" + std::to_string(WSAGetLastError()));
					logEtherDLL.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
					return;
				}

				logEtherDLL.info(name + " waiting for commands from client...");
				checkPeriod = config["service"]["command"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["command"]["sleep_ms"].get<int>()));
		}
		
	}
}

// ----------------------------------------------------------------------
/*
	Send data from the DLL API to the client, whenever there is data available in the streamBuffer.
*/
void handleStreamConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!streamBuffer.empty()) {
			std::string data = streamBuffer.back() + mcs::Form::MSG_END;
			streamBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logEtherDLL.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				logEtherDLL.info(name + " waiting for data from station to send...");
				checkPeriod = config["service"]["stream"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["stream"]["sleep_ms"].get<int>()));
		}
	}
}

// ----------------------------------------------------------------------
/*
	Send error data from the DLL API to the client, whenever data available in the errorBuffer.
*/
void handleErrorConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!errorBuffer.empty()) {
			std::string data = errorBuffer.back() + mcs::Form::MSG_END;
			errorBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logEtherDLL.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		} else {
			if (checkPeriod == 0) {
				logEtherDLL.info(name + " waiting for data from station to send...");
				checkPeriod = config["service"]["error"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["error"]["sleep_ms"].get<int>()));
		}
	}
}

// ----------------------------------------------------------------------
/*
	Send real-time data from the station to the client, whenever there is data available in the realtimeBuffer.
*/
void handleRealTimeConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!realtimeBuffer.empty()) {
			std::string data = realtimeBuffer.back() + mcs::Form::MSG_END;
			realtimeBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logEtherDLL.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				logEtherDLL.info(name + " waiting for data from station to send...");
				checkPeriod = config["service"]["realtime"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["realtime"]["sleep_ms"].get<int>()));
		}
	}
}

// ----------------------------------------------------------------------
/*
	Handle audio data
*/
void handleAudioConnection(SOCKET clientSocket, std::string name) {
	loopbackCapture.handleSocketConnection(clientSocket, name);
}

// ----------------------------------------------------------------------
/*
	Listen for incoming connections on a socket.
*/
void socketHandle(	std::string name,
					int ServiceCode,
					int port,
					int timeout,
					void (*connectionHandler)(SOCKET, std::string)) {

	SOCKET listenSocket = INVALID_SOCKET;
	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::string portStr = std::to_string(port);
	int iResult = getaddrinfo(NULL, portStr.c_str(), &hints, &result);
	if (iResult != 0) {
		logEtherDLL.error(name + " socket getaddrinfo failed. EC:" + std::to_string(iResult));
		running.store(false);
		interruptionCode.store(ServiceCode);
		WSACleanup();
		return;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		logEtherDLL.error(name + " socket creation failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	iResult = setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	if (iResult == SOCKET_ERROR) {
		logEtherDLL.error(name + " socket setsockopt timeout failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	iResult = ::bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		logEtherDLL.error(name + " socket bind failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		logEtherDLL.error(name + " socket listen failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	// create variable to store the client address
	struct sockaddr_in clientAddr;

	SOCKET clientSocket = INVALID_SOCKET;
	while (running.load()) {
		// call the connection handler 
		logEtherDLL.info(name + " listening on port " + std::to_string(port));

		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, NULL);
		if (clientSocket == INVALID_SOCKET) {
			logEtherDLL.warn(name + " failed in accept operation with " + std::string(inet_ntoa(clientAddr.sin_addr)) + ". EC:" + std::to_string(WSAGetLastError()));
		}
		else {
			logEtherDLL.info(name + " accepted connection from " + std::string(inet_ntoa(clientAddr.sin_addr)));

			connectionHandler(clientSocket, name);
		}
	}

	closesocket(listenSocket);
	WSACleanup();
	logEtherDLL.info(name + " stopped listening on port " + std::to_string(port));
}

// ----------------------------------------------------------------------
/*
	Disconnect station and socket clients
*/
void disconnectAPI(void)
{

	ERetCode errCode = Disconnect(APIserverId);
	logEtherDLL.warn("Disconnecting station returned:" + ERetCodeToString(errCode));
	
	/* DLL function not returning API_SUCCESS - Need to investigate
	if (errCode != ERetCode::API_SUCCESS)
	{
		logger.error("Error disconnecting from station " + ERetCodeToString(errCode));
		running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
	}
	else
	{
		logger.info("Disconnected from station");
	}
	*/
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
json readConfigFile(std::string fileName) {

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
void print_help() {
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
std::string handleInputArguments(int argc, char* argv[]) {
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

	connectAPI();

	// Start thread for the command channel socket service. This thread will listen for incoming commands and place then in the command queue
	std::thread commandThread(socketHandle,
		"Control service",
		mcs::Code::COMMAND_ERROR,
		config["service"]["command"]["port"].get<int>(),
		config["service"]["command"]["timeout_s"].get<int>(),
		handleCommandConnection);

	// Start thread for the stream channel socket service. This thread will listen for incoming connections and stream data back to the client
	std::thread streamThread(socketHandle,
		"Stream service",
		mcs::Code::STREAM_ERROR,
		config["service"]["stream"]["port"].get<int>(),
		config["service"]["stream"]["timeout_s"].get<int>(),
		handleStreamConnection);

	// Start thread for the stream channel socket service. This thread will stream error back to the client
	std::thread errorThread(socketHandle,
		"Error service",
		mcs::Code::STREAM_ERROR,
		config["service"]["error"]["port"].get<int>(),
		config["service"]["error"]["timeout_s"].get<int>(),
		handleErrorConnection);

	// Start thread for the stream channel socket service. This thread will stream real time data to the client
	std::thread realtimeThread(socketHandle,
		"Real time service",
		mcs::Code::STREAM_ERROR,
		config["service"]["realtime"]["port"].get<int>(),
		config["service"]["realtime"]["timeout_s"].get<int>(),
		handleRealTimeConnection);

	// Start thread for the audio socket service.
	std::thread audioThread(socketHandle,
		"Audio service",
		mcs::Code::STREAM_ERROR,
		config["service"]["audio"]["port"].get<int>(),
		config["service"]["audio"]["timeout_s"].get<int>(),
		handleAudioConnection);
		
	// Main loop to process commands received
	while (running.load()) {

		if (!commandQueue.empty()) {
			std::string command;
			{
				std::lock_guard<std::mutex> lock(MCCommandMutex);
				command = commandQueue.back();
				commandQueue.pop_back();
			}
			logEtherDLL.info("Processing command: " + command);
			
			unsigned long requestID = 0;
			json jsonObj = json::parse(command);


			std::tie(message, log_level)


		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	logEtherDLL.info("Service will shutdown...");

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
	disconnectAPI();
	logEtherDLL.~EtherDLLLog();

	return static_cast<int>(interruptionCode.load());
}

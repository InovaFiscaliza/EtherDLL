// MIAerConn.cpp : Defines the entry point for the application.
//

// Include the ScorpioAPI libraries 3h2cl5vTu8cg
#include <StdAfx.h>
#include <ScorpioAPIDll.h> 

// Include the standard C++ headers
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <csignal>
#include <chrono>
#include <fstream>

#pragma comment(lib, "Ws2_32.lib")

// Include the nlohmann JSON library
#include <nlohmann/json.hpp>

// Include the spdlog library
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// Include to solution specific libraries
#include <messages.h>
#include <MIAerConnCodes.hpp>

// For convenience
using json = nlohmann::json;

//
// Global variables related to the application
//

// JSON configuration object
json config;

// Logger object
spdlog::logger logger = spdlog::logger("MIAerConn");

// Atomic flag to signal application error
std::atomic<bool> running{ true }; // Atomic flag to control the thread loops

// Atomic flag to signal application error code
std::atomic<MCService::Code> interruptionCode{ MCService::Code::RUNNING }; // Code to represent the cause for not running

// Vector used for the command queue
std::vector<std::string> commandQueue;

// Vector used for the data stream output
std::vector<std::string> streamBuffer;

// Mutex to protect the command queue
std::mutex commandMutex;

//
// Global variables related to the API
//

// API server ID. This service is intended to be used to connect to a single station, always 0.
unsigned long APIserverId = 0;

// Station connection parameters
SScorpioAPIClient station;

// Station capabilities
SCapabilities StationCapabilities;

ErrorCB OnErrFunc;
DataCB OnDataFunc;
RealTimeDataCB OnRealtimeDataFunc;

// Return code for API functions
ERetCode errCode;

//
// Signal handler function
//
void signalHandler(int signal) {

	if (signal == SIGINT)
	{
		running.store(false);
		interruptionCode.store(MCService::Code::CTRL_C_INTERRUPT);
		logger.warn("Received interruption signal (ctrl+C)");
	}
	else if (signal == SIGTERM)
	{
		running.store(false);
		interruptionCode.store(MCService::Code::KILL_INTERRUPT);
		logger.warn("Received termination signal (kill)");
	}
	else 	{
		std::string message = "Received unknown signal. #LttOS: " + std::to_string(signal);
		logger.warn(message);
	}
}

//
// Register the signal handlers
//
void registerSignalHandlers() {
	std::signal(SIGINT, signalHandler);  // Handles Ctrl+C
	std::signal(SIGTERM, signalHandler); // Handles kill command
}


//
// Start the logger
//
void StartLogger(void) {

	bool console_sink_enabled = config["log"]["console"]["enable"].get<bool>();

	// Clear all the sinks from the logger
	logger.sinks().clear();

	if (console_sink_enabled) {
		// Create a logger sink to write to the console
		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		// Get the console log levels from the configuration file
		std::string console_log_level = config["log"]["console"]["level"].get<std::string>();

		// Set the log levels for the console and file sinks
		switch (console_log_level[0]) {
		case 't':
			console_sink->set_level(spdlog::level::trace);
			break;
		case 'd':
			console_sink->set_level(spdlog::level::debug);
			break;
		case 'i':
			console_sink->set_level(spdlog::level::info);
			break;
		case 'w':
			console_sink->set_level(spdlog::level::warn);
			break;
		case 'e':
			console_sink->set_level(spdlog::level::err);
			break;
		case 'c':
			console_sink->set_level(spdlog::level::critical);
			break;
		default:
			console_sink->set_level(spdlog::level::info);
			break;
		}

		// Add the console sink to the logger
		logger.sinks().push_back(console_sink);
	}

	bool file_sink_enabled = config["log"]["file"]["enable"].get<bool>();

	if (file_sink_enabled) {
		// Get the log filename from the configuration file
		std::string log_filename = std::string(config["log"]["file"]["path"].get<std::string>());

		// Create a logger sink that writes to a file
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename, true);

		// Get the file log levels from the configuration file
		std::string file_log_level = config["log"]["file"]["level"].get<std::string>();

		// Set the log levels for the console and file sinks
		switch (file_log_level[0]) {
		case 't':
			file_sink->set_level(spdlog::level::trace);
			break;
		case 'd':
			file_sink->set_level(spdlog::level::debug);
			break;
		case 'i':
			file_sink->set_level(spdlog::level::info);
			break;
		case 'w':
			file_sink->set_level(spdlog::level::warn);
			break;
		case 'e':
			file_sink->set_level(spdlog::level::err);
			break;
		case 'c':
			file_sink->set_level(spdlog::level::critical);
			break;
		default:
			file_sink->set_level(spdlog::level::info);
			break;
		}

		// Add the file sink to the logger
		logger.sinks().push_back(file_sink);
	}	

	logger.info("Starting...");
}


//
// Function to handle command connections
//
void handleCommandConnection(SOCKET clientSocket) {
	char buffer[1024];
	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead > 0) {
		std::string command(buffer, bytesRead);
		{
			std::lock_guard<std::mutex> lock(commandMutex);
			commandQueue.push_back(command);
		}
		std::string ack = "ACK";
		send(clientSocket, ack.c_str(), static_cast<int>(ack.length()), 0);
	}
	closesocket(clientSocket);
}

//
// Function to handle streaming data
//
void handleStreamConnection(SOCKET clientSocket) {

	/* TODO: Implement a control code to activate the output stream. May allow for multiple clients. 
	char buffer[1024];
	int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytesRead > 0) {
		std::string connectionCode(buffer, bytesRead);
		logger.info("Client connected with code: " + connectionCode);
	}
	*/

	while (running.load()) {
		if (!streamBuffer.empty()) {
			std::string data = streamBuffer.back();
			streamBuffer.pop_back();
			send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["stream_sleep"].get<int>()));

			// Simulation of streaming data - to be replaced with data from the station
			// random number of packages, from 0 to 4.
			// Each package has 4096 bytes of data, which is 1024 floats of 32 bits
			// The data is a binary sequency of about one cycle of a sine wave
			for (int j = 0; rand() % 5; j++) {
				float data[1024];
				for (int i = 0; i < 1024; i++) {
					data[i] = (float)((60.0 * sin(i * 0.006136)) - 80.0);
				}
				streamBuffer.push_back(std::string(reinterpret_cast<char*>(data), sizeof(data)));
			}
		}
	}

	closesocket(clientSocket);
}


//
// Function to listen on a specific port
//
void listenOnPort(std::string name,
	MCService::Code ServiceCode,
	int port,
	void (*connectionHandler)(SOCKET)) {

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
		logger.error("getaddrinfo failed: " + std::to_string(iResult));
		running.store(false);
		interruptionCode.store(ServiceCode);
		WSACleanup();
		return;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		logger.error("Error at socket(): " + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	using namespace std;

	iResult = ::bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		logger.error("bind failed: " + std::to_string(WSAGetLastError()));
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
		logger.error("Listen failed: " + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	logger.info(name + " is listening on port " + std::to_string(port));

	while (running.load()) {
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

		if (clientSocket != INVALID_SOCKET) {
			std::thread([connectionHandler, clientSocket]() {
				connectionHandler(clientSocket);
				}).detach();
		}
	}

	closesocket(listenSocket);
	logger.info(name + " stopped listening on port " + std::to_string(port));
}

//
// Convert a string to a wide string. Used in messages from the station
//
std::wstring stringToWString(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}


//
// Create a connection object to the station and connect to it.
//
void StationConnect(void) {
	// Create a local copy of APIserverId. This is necessary because TCI methods update the APIserverId value to the next available ID.
	// unsigned long NextServerId = APIserverId;

	// Hostname as simple string, extracted from th JSON configuration file
	std::string hostNameStr = config["station"]["address"].get<std::string>();
	station.hostName = stringToWString(hostNameStr);

	// Port as simple string, extracted from the JSON configuration file where it is defined as a number
	std::string portStr = std::to_string(config["station"]["port"].get<int>());
	station.port = stringToWString(portStr);

	// Timeout as unsigned long, extracted from the JSON configuration file
	station.sendTimeout = config["station"]["timeout"].get<unsigned long>();

	// Error code using API ERetCode enum
	ERetCode errCode;

	// Create the connection object
	errCode = ScorpioAPICreate(
					APIserverId,
					station,
					OnErrFunc,
					OnDataFunc,
					OnRealtimeDataFunc);

	// Error message string to be used in the logger
	std::string message;

	// Handle the error code from object creation
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Object associated with station not created: " + ERetCodeToString(errCode);
		logger.error(message);
		running.store(false);
		interruptionCode.store(MCService::Code::STATION_ERROR);
		return;
	}
	else
	{
		logger.info("Object creation successful");
	}

	// NextServerId = APIserverId;

	// Once the object was successfully created, test connection to the station
	errCode = RequestCapabilities(APIserverId, StationCapabilities);

	// Handle the error code from station connection
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Connection with " + hostNameStr + " not stablished: " + ERetCodeToString(errCode);
		logger.error(message);
//		running.store(false);
		interruptionCode.store(MCService::Code::STATION_ERROR);
		return;
	}
	else
	{
		message = "Connection with " + hostNameStr + " stablished";
		logger.info(message);
	}
}

//
// Disconnect station and socket clients
//
void StationDisconnect(void)
{
	ERetCode errCode;
	std::string message;

	// Create a local copy of APIserverId. This is necessary because TCI methods update the APIserverID value to the next available ID.
	unsigned long NextServerId = APIserverId;

	errCode = Disconnect(NextServerId);
	
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Error disconnecting from station " + ERetCodeToString(errCode);
		logger.error(message);
		running.store(false);
		interruptionCode.store(MCService::Code::STATION_ERROR);
	}
	else
	{
		logger.info("Disconnected from station");
	}

}

int main() {

	registerSignalHandlers();

	// Read configuration from JSON file
	std::ifstream config_file("config.json");
	config = json::parse(config_file);

	StartLogger();

	int CommandPort = config["service"]["command_port"].get<int>();
	int StreamPort = config["service"]["stream_port"].get<int>();
	int timeout = config["service"]["timeout"].get<int>();

	StationConnect();

	// Start thread for the command channel socket service. This thread will listen for incoming commands and place then in the command queue
	std::thread commandThread(listenOnPort,
		"Control service",
		MCService::Code::COMMAND_ERROR,
		CommandPort,
		handleCommandConnection);

	// Start thread for the stream channel socket service. This thread will listen for incoming connections and stream data back to the client
	std::thread streamThread(listenOnPort,
		"Stream service",
		MCService::Code::STREAM_ERROR,
		StreamPort,
		handleStreamConnection);

	// Main loop to process commands
	while (running.load()) {
		std::lock_guard<std::mutex> lock(commandMutex);

		if (!commandQueue.empty()) {
			std::string command = commandQueue.back();
			commandQueue.pop_back();
			logger.info("Processing command: " + command);
			// Process the command and possibly send responses back to clients
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	logger.info("Service will shutdown...");

	// Join threads before exiting
	if (commandThread.joinable()) {
		commandThread.join();
	}
	if (streamThread.joinable()) {
		streamThread.join();
	}

	// Close the connection
	StationDisconnect();

	// Final flush before the application exits, save log to file.
	logger.flush();

	return static_cast<int>(interruptionCode.load());
}

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

// Include the nlohmann JSON library
#include <nlohmann/json.hpp>

// Include to solution specific libraries
#include <ExternalCodes.h>
#include <MIAerConnCodes.hpp>
#include <MIAerConnUtils.h>
#include "MIAerLog.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

/*
* Felipe Machado - 29/08/2024
* In debug and release the name of ScorpioAPI is different, so checking is necessary to know which one to call

*/
#ifdef _X86_
	#pragma comment (lib, "ScorpioAPIDll.lib")
#else
	#ifdef NDEBUG
		#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE
	#else
		#pragma comment (lib, "ScorpioAPIDlld.lib") //DEBUG
	#endif
#endif

#pragma comment (lib, "AdvApi32.lib")

// For convenience
using json = nlohmann::json;

//
// Global variables related to the application
//

// JSON configuration object
json config;

// Atomic flag to signal application error
std::atomic<bool> running{ true }; 

// Code to represent the cause for not running
std::atomic<int> interruptionCode{ mcs::Code::RUNNING };

// Vector used for the command queue
std::vector<std::string> commandQueue;

// Vector used for the data stream output
std::vector<std::string> streamBuffer;

// Mutex to protect the command queue
std::mutex MCCommandMutex;
std::mutex MCstreamMutex;
std::mutex MCStationMutex;
//
// Global variables related to the API
//

// API server ID. This service is intended to be used to connect to a single station, always 0.
unsigned long APIserverId = 0;

// Station connection parameters
SScorpioAPIClient station;

// Station capabilities
SCapabilities StationCapabilities;

RealTimeDataCB OnRealtimeDataFunc;

//Socket connection for commands
SOCKET clientSocketCommand = NULL;

MIAerLog logMIAer;

/*
* Felipe Machado - 23/08/2024
* Data callback for Scorpio API
*/
void OnDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ unsigned long desstAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	std::string strData = std::string(reinterpret_cast<char*>(data), sizeof(data));
	streamBuffer.push_back(strData);
	logMIAer.info("OnData received with type " + respType);
	logMIAer.info("OnData received destination address " + desstAddr);
	logMIAer.info("OnData received server ID " + serverId);
}

/*
* Felipe Machado - 23/08/2024
* Error callback for Scorpio API
*/
void OnErrorFunc(_In_  unsigned long serverId, _In_ const std::wstring& errorMsg)
{
	std::string str(errorMsg.begin(), errorMsg.end());
	logMIAer.error(str);
	if (clientSocketCommand != NULL) {
		int iResult = send(clientSocketCommand,
			(str + "\n").c_str(),
			static_cast<int>(strlen(str.c_str()) + 1),
			0);
		if (iResult == SOCKET_ERROR) {
			logMIAer.warn("Failed to send socket. EC:" + std::to_string(WSAGetLastError()));
			logMIAer.info("Connection with address" + std::to_string(clientSocketCommand) + " lost.");
			return;
		}
	}
	else {
		logMIAer.warn("Command socket connection not found");
	}

}

//
// Signal handler function
//
void signalHandler(int signal) {

	if (signal == SIGINT)
	{
		running.store(false);
		interruptionCode.store(mcs::Code::CTRL_C_INTERRUPT);
		logMIAer.warn("Received interruption signal (ctrl+C)");
	}
	else if (signal == SIGTERM)
	{
		running.store(false);
		interruptionCode.store(mcs::Code::KILL_INTERRUPT);
		logMIAer.warn("Received termination signal (kill)");
	}
	else 	{
		std::string message = "Received unknown signal. #LttOS: " + std::to_string(signal);
		logMIAer.warn(message);
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
// Function to create simulated data
// Data is a random number of packages, from 1 to 5.
// Each package has 4096 bytes of data, which is 1024 floats of 32 bits
// First 12 points are a sequency of 0, 1, 0, -1, repeated 3 times
// Following 500 points describes a ramp from -100 to -20
// Finally 512 points describes cosine cycles with the same amplitude. The number of cycles varies with the package number.

void generateSimData(void) {
	
	float data[1024];
	int j = 1;
	std::string trace;
	
	for (j = 1; rand() % 6; j++) {
		for (int i = 0; i < 12; i = i + 4) {
			data[0+i] = (float)(0.0);
			data[1+i] = (float)(1.0);
			data[2+i] = (float)(0.0);
			data[3+i] = (float)(-1.0);
		}
		for (int i = 12; i < 512; i++) {
			data[i] = (float)((40.0 * (((float)(i) / 250) - 1)) - 60.0);
		}
		for (int i = 512; i < 1024; i++) {
			data[i] = (float)((60.0 * cos((i * 0.0184)*2*j)) - 80.0);
		}
		trace = std::string(reinterpret_cast<char*>(data), sizeof(data));
		streamBuffer.push_back(trace);
	}
	
	// logger.info("Simulated data generated " + std::to_string(j) + " traces with 1024 float points.");
	if (sizeof(trace) > 48) {
		std::string message = "Sample points: [";
		// add the first 12 points as uint8 translation from the streamBuffer
		for (int i = 0; i < 48; i++) {
			message += std::to_string((unsigned char)trace[i]) + ", ";
		}
		message += "...]";
		logMIAer.info(message);
	}
	//
}

//
// Function to handle command connections
//
void handleCommandConnection(SOCKET clientSocket, std::string name) {
	clientSocketCommand = clientSocket;

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
				mcs::Form::SEP;
			iResult = send(clientSocket, ack.c_str(), (int)(ack.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " ACK send failed. EC:" + std::to_string(WSAGetLastError()));
				logMIAer.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
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
					logMIAer.warn(name + " PING send failed. EC:" + std::to_string(WSAGetLastError()));
					logMIAer.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
					return;
				}

				logMIAer.info(name + " waiting for commands from client...");
				checkPeriod = config["service"]["command"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["command"]["sleep_ms"].get<int>()));
		}
		
	}
}

//
// Function to handle streaming data
//
void handleStreamConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!streamBuffer.empty()) {
			std::string data = streamBuffer.back() + mcs::Form::BLOCK_END;
			streamBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		} else {
			if (checkPeriod == 0) {
				if (config["service"]["simulated"].get<bool>()) {
					generateSimData();
					logMIAer.info(name + " generated sim data. Nothing from the station.");
				}
				else {
					logMIAer.info(name + " waiting for data from station to send...");
				}
				checkPeriod = config["service"]["stream"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["stream"]["sleep_ms"].get<int>()));
		}
	}
}


//
// Function to listen on a specific port
//
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
		logMIAer.error(name + " socket getaddrinfo failed. EC:" + std::to_string(iResult));
		running.store(false);
		interruptionCode.store(ServiceCode);
		WSACleanup();
		return;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		logMIAer.error(name + " socket creation failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	iResult = setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	if (iResult == SOCKET_ERROR) {
		logMIAer.error(name + " socket setsockopt timeout failed. EC:" + std::to_string(WSAGetLastError()));
		running.store(false);
		interruptionCode.store(ServiceCode);
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	//using namespace std;

	iResult = ::bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		logMIAer.error(name + " socket bind failed. EC:" + std::to_string(WSAGetLastError()));
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
		logMIAer.error(name + " socket listen failed. EC:" + std::to_string(WSAGetLastError()));
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
		logMIAer.info(name + " listening on port " + std::to_string(port));

		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, NULL);
		if (clientSocket == INVALID_SOCKET) {
			logMIAer.warn(name + " failed in accept operation with " + std::string(inet_ntoa(clientAddr.sin_addr)) + ". EC:" + std::to_string(WSAGetLastError()));
		}
		else {
			logMIAer.info(name + " accepted connection from " + std::string(inet_ntoa(clientAddr.sin_addr)));

			connectionHandler(clientSocket, name);
		}
	}

	closesocket(listenSocket);
	WSACleanup();
	logMIAer.info(name + " stopped listening on port " + std::to_string(port));
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

	// Timeout as unsigned long, extracted from the JSON configuration file in second and converted to miliseconds
	station.sendTimeout = (unsigned long)(config["station"]["timeout_s"].get<int>())*1000;

	// Error code using API ERetCode enum
	ERetCode errCode;

	// Create the connection object
	errCode = ScorpioAPICreate(
					APIserverId,
					station,
					OnErrorFunc,
					OnDataFunc,
					OnRealtimeDataFunc);

	// Error message string to be used in the logger
	std::string message;

	// Handle the error code from object creation
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Object associated with station not created: " + ERetCodeToString(errCode);
		logMIAer.error(message);
		//running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
		return;
	}
	else
	{
		logMIAer.info("Object creation successful");
	}

	// NextServerId = APIserverId;

	// Once the object was successfully created, test connection to the station

	errCode = RequestCapabilities(APIserverId, StationCapabilities);

	// Handle the error code from station connection
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Failed to connect to " + hostNameStr + ": " + ERetCodeToString(errCode);
		logMIAer.error(message);
//		running.store(false);
		interruptionCode.store(mcs::Code::STATION_ERROR);
		return;
	}
	else
	{
		message = "Connected to station " + hostNameStr;
		logMIAer.info(message);
	}
}

//
// Disconnect station and socket clients
//
void StationDisconnect(void)
{

	ERetCode errCode = Disconnect(APIserverId);
	logMIAer.warn("Disconnecting station returned:" + ERetCodeToString(errCode));
	
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

int main() {

	registerSignalHandlers();

	// Read configuration from JSON file
	std::ifstream config_file("config.json");
	/*
	  * Felipe Machado - 29/08/2024
	  * checks if configuration file exists, if does not exist, a new file is generated with default values 
	*/
	if (config_file.fail()) {
		newDefaultConfigFile();
		std::ifstream config_file("config.json");
		config = json::parse(config_file);
	}
	else {
		config = json::parse(config_file);
	}	

	logMIAer.start("MIAerConn", config["log"]["console"]["enable"].get<bool>(), config["log"]["file"]["enable"].get<bool>(), config["log"]["console"]["level"].get<std::string>(), config["log"]["file"]["path"].get<std::string>(), config["log"]["file"]["level"].get<std::string>());
	StationConnect();

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
		
	// Main loop to process commands received
	while (running.load()) {

		if (!commandQueue.empty()) {
// ! Need to fix the mutex lock, moving it to functions which manipulate the commandQueue outside the scope of the main loop
			std::lock_guard<std::mutex> lock(MCCommandMutex);
			std::string command = commandQueue.back();
			commandQueue.pop_back();
			logMIAer.info("Processing command: " + command);
			
			/*
			* Felipe Machado - 23/08/2024
			* Exemplo pacote socket
			* cmd|param01|param02|...
			*/ 
			unsigned long requestID = 0;
			std::vector<std::string> params = split(command, "|");

			int cmd = atoi(params[0].c_str());

			switch (cmd)
			{
				case ECSMSDllMsgType::GET_OCCUPANCY:
					logMIAer.logCommandExec(RequestOccupancy(APIserverId, stringToSOccupReqData(params[1]), &requestID), "RequestOccupancy");
					break;
				case ECSMSDllMsgType::GET_OCCUPANCYDF:
					logMIAer.logCommandExec(RequestOccupancyDF(APIserverId, stringToSOccDFReqData(params[1]), &requestID), "RequestOccupancyDF");
					break;
				case ECSMSDllMsgType::GET_AVD:
					logMIAer.logCommandExec(RequestAVD(APIserverId, stringToSAVDReqData(params[1]), &requestID), "RequestAVD");
					break;
				case ECSMSDllMsgType::GET_MEAS:
					logMIAer.logCommandExec(RequestMeasurement(APIserverId, stringToSMeasReqData(params[1]), &requestID), "RequestMeasurement");
					break;
				case ECSMSDllMsgType::GET_TASK_STATUS:
					logMIAer.logCommandExec(RequestTaskStatus(APIserverId, stringToUnsignedLong(params[1])), "RequestTaskStatus");
					break;
				case ECSMSDllMsgType::GET_TASK_STATE:
					logMIAer.logCommandExec(RequestTaskState(APIserverId, (ECSMSDllMsgType)stringToUnsignedLong(params[1]), stringToUnsignedLong(params[2])), "RequestTaskState");
					break;
				case ECSMSDllMsgType::TASK_SUSPEND:
					logMIAer.logCommandExec(SuspendTask(APIserverId, (ECSMSDllMsgType)stringToUnsignedLong(params[1]), stringToUnsignedLong(params[2])), "SuspendTask");
					break;
				case ECSMSDllMsgType::TASK_RESUME:
					logMIAer.logCommandExec(ResumeTask(APIserverId, (ECSMSDllMsgType)stringToUnsignedLong(params[1]), stringToUnsignedLong(params[2])), "ResumeTask");
					break;
				case ECSMSDllMsgType::TASK_TERMINATE:
					logMIAer.logCommandExec(TerminateTask(APIserverId, stringToUnsignedLong(params[1])), "TerminateTask");
					break;
				case ECSMSDllMsgType::GET_BIST:
					logMIAer.logCommandExec(RequestBist(APIserverId, (EBistScope)atoi(params[1].c_str()), &requestID), "RequestBist");
					break;
				case ECSMSDllMsgType::SET_AUDIO_PARAMS:
					logMIAer.logCommandExec(SetAudio(APIserverId, stringToSAudioParams(params[1]), &requestID), "SetAudio");
					break;
				case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
					logMIAer.logCommandExec(FreeAudio(APIserverId, stringToUnsignedLong(params[1]), &requestID), "FreeAudio");
					break;
				case ECSMSDllMsgType::SET_PAN_PARAMS:
					logMIAer.logCommandExec(SetPanParams(APIserverId, stringToSPanParams(params[1]), &requestID), "SetPanParams");
					break;
				case ECSMSDllMsgType::GET_PAN:
					logMIAer.logCommandExec(RequestPan(APIserverId, stringToSGetPanParams(params[1]), &requestID), "RequestPan");
					break;
				default:
					logMIAer.error("command not recognized");
					break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	logMIAer.info("Service will shutdown...");

	// Join threads before exiting
	if (commandThread.joinable()) {
		commandThread.join();
	}
	if (streamThread.joinable()) {
		streamThread.join();
	}

	// Close the connection
	StationDisconnect();
	logMIAer.~MIAerLog();

	return static_cast<int>(interruptionCode.load());
}

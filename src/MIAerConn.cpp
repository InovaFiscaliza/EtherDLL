// MIAerConn.cpp : Defines the entry point for the application.
//

// Include the ScorpioAPI libraries 3h2cl5vTu8cg
#include <StdAfx.h>

// Include the standard C++ headers
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <chrono>
#include <csignal>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Include the nlohmann JSON library
#include <nlohmann/json.hpp>

// Include to solution specific libraries
#include <MIAerLog.h>
#include <ExternalCodes.h>
#include <MIAerConnAudio.h>
#include <MIAerConnUtils.h>
#include <MIAerConnCodes.hpp>
#include <MIAerConnProcessApiResponse.h>

 // Libs for socket
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

/*
* Felipe Machado - 29/08/2024
* In debug and release the name of ScorpioAPI is different, so checking is necessary to know which one to call

*/
#ifdef _X86_
	#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE/DEBUG 32Bits
#else
	#ifdef NDEBUG
		#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE 64Bits
	#else
		#pragma comment (lib, "ScorpioAPIDlld.lib") //DEBUG 64Bits
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

// Vector used for the data error output
std::vector<std::string> errorBuffer;

// Vector used for the real time output
std::vector<std::string> realtimeBuffer;

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

MIAerLog logMIAer;

CLoopbackCapture loopbackCapture;

/*
* Felipe Machado - 23/08/2024
* Data callback for Scorpio API
*/
void OnDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ unsigned long sourceAddr, _In_ unsigned long desstAddr, _In_ SEquipCtrlMsg::UBody* data)
{
	switch (respType)
	{
		case ECSMSDllMsgType::GET_BIST:
		case ECSMSDllMsgType::GET_BIST_RESULT:
		case ECSMSDllMsgType::GET_DIAGNOSTICS:
			streamBuffer.push_back(processBITEResponse(respType, data));
			break;
		case ECSMSDllMsgType::GET_ANT_LIST_INFO:
			streamBuffer.push_back(ProcessAntListResponse(data));
			break;
		case ECSMSDllMsgType::OCC_MSGLEN_DIST_RESPONSE:
		case ECSMSDllMsgType::OCC_FREQ_VS_CHANNEL:
		case ECSMSDllMsgType::OCC_CHANNEL_RESULT:
		case ECSMSDllMsgType::OCC_STATUS:
		case ECSMSDllMsgType::OCC_STATE_RESPONSE:
		case ECSMSDllMsgType::OCC_SOLICIT_STATE_RESPONSE:
		case ECSMSDllMsgType::OCC_SPECTRUM_RESPONSE:
		case ECSMSDllMsgType::OCC_TIMEOFDAY_RESULT:
		case ECSMSDllMsgType::OCC_EFLD_CHANNEL_RESULT:
		case ECSMSDllMsgType::OCC_MSGLEN_CHANNEL_RESULT:
		case ECSMSDllMsgType::OCC_EFLD_TIMEOFDAY_RESULT:
			streamBuffer.push_back(processOccupancyResponse(respType, data));
			break;
		case ECSMSDllMsgType::OCCDF_FREQ_VS_CHANNEL:
		case ECSMSDllMsgType::OCCDF_SCANDF_VS_CHANNEL:
		case ECSMSDllMsgType::OCCDF_STATUS:
		case ECSMSDllMsgType::OCCDF_STATE_RESPONSE:
		case ECSMSDllMsgType::OCCDF_SOLICIT_STATE_RESPONSE:
			streamBuffer.push_back(processOccupancyDFResponse(respType, data));
			break;
		case ECSMSDllMsgType::AVD_FREQ_VS_CHANNEL:
		case ECSMSDllMsgType::AVD_OCC_CHANNEL_RESULT:
		case ECSMSDllMsgType::AVD_FREQ_MEAS:
		case ECSMSDllMsgType::AVD_BW_MEAS:
		case ECSMSDllMsgType::AVD_SOLICIT_STATE_RESPONSE:
		case ECSMSDllMsgType::AVD_STATE_RESPONSE:
		case ECSMSDllMsgType::AVD_STATUS:
			streamBuffer.push_back(processAutoViolateResponse(respType, data));
			break;
		case ECSMSDllMsgType::GET_MEAS:
		case ECSMSDllMsgType::VALIDATE_MEAS:
			streamBuffer.push_back(processMeasResponse(respType, sourceAddr, data));
			break;
		case ECSMSDllMsgType::SET_PAN_PARAMS:
		case ECSMSDllMsgType::SET_AUDIO_PARAMS:
		case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
			streamBuffer.push_back(processDemodCtrlResponse(respType, data));
			break;
		case ECSMSDllMsgType::GET_PAN:
			streamBuffer.push_back(processPanResponse(data));
			break;
		case ECSMSDllMsgType::GET_DM:

			//m_taskType = ECSMSDllMsgType::GET_DM; // TODO should be in DM_STATUS reponse instead
			break;
		default:
			break;
	}

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
	std::string strData = "{\"serverId\":"+ std::to_string(serverId) + ", \"errorMsg\":\"" + str + "\"}";
	errorBuffer.push_back(strData);
	logMIAer.info("OnErrorFunc received error message: " + str);
	logMIAer.info("OnErrorFunc received server ID: " + serverId);

}

/*
* Felipe Machado - 02/09/2024
* Realtime callback for Scorpio API
*/
void OnRealTimeDataFunc(_In_  unsigned long serverId, _In_ ECSMSDllMsgType respType, _In_ SSmsRealtimeMsg::UBody* data)
{
	streamBuffer.push_back(ProcessRealTimeData(respType, data));
	logMIAer.info("OnRealtimeData received with type " + respType);
	logMIAer.info("OnRealtimeData received server ID " + serverId);
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
// Function to handle command connections
//
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
			std::string data = streamBuffer.back() + mcs::Form::MSG_END;
			streamBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				logMIAer.info(name + " waiting for data from station to send...");
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
// Function to handle error data
//
void handleErrorConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!errorBuffer.empty()) {
			std::string data = errorBuffer.back() + mcs::Form::MSG_END;
			errorBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		} else {
			if (checkPeriod == 0) {
				logMIAer.info(name + " waiting for data from station to send...");
				checkPeriod = config["service"]["error"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["error"]["sleep_ms"].get<int>()));
		}
	}
}

//
// Function to handle real time data
//
void handleRealTimeConnection(SOCKET clientSocket, std::string name) {

	int checkPeriod = 0;
	int iResult = 0;
	while (running.load()) {
		if (!realtimeBuffer.empty()) {
			std::string data = realtimeBuffer.back() + mcs::Form::MSG_END;
			realtimeBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				logMIAer.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				logMIAer.info(name + " waiting for data from station to send...");
				checkPeriod = config["service"]["realtime"]["check_period"].get<int>();
			}
			else {
				checkPeriod--;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config["service"]["realtime"]["sleep_ms"].get<int>()));
		}
	}
}

//
// Function to handle audio data
//
void handleAudioConnection(SOCKET clientSocket, std::string name) {
	loopbackCapture.handleSocketConnection(clientSocket, name);
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
					OnRealTimeDataFunc);

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

	/*
	  * Felipe Machado - 02/09/2024
	*/
	// Start thread for the stream channel socket service. This thread will stream error back to the client
	std::thread errorThread(socketHandle,
		"Error service",
		mcs::Code::STREAM_ERROR,
		config["service"]["error"]["port"].get<int>(),
		config["service"]["error"]["timeout_s"].get<int>(),
		handleErrorConnection);

	/*
	  * Felipe Machado - 02/09/2024
	*/
	// Start thread for the stream channel socket service. This thread will stream real time data to the client
	std::thread realtimeThread(socketHandle,
		"Real time service",
		mcs::Code::STREAM_ERROR,
		config["service"]["realtime"]["port"].get<int>(),
		config["service"]["realtime"]["timeout_s"].get<int>(),
		handleRealTimeConnection);

	/*
	  * Felipe Machado - 04/10/2024
	*/
	// Start thread for the audio socket service. This thread will stream real time data to the client
	std::thread audioThread(socketHandle,
		"Audio service",
		mcs::Code::STREAM_ERROR,
		5000,
		6000,
		handleAudioConnection);
		
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
			* {commandCode: 1, commandStruct: struct, taskType: , reqID: ,  }
			*/ 
			unsigned long requestID = 0;
			using json = nlohmann::json;
			json jsonObj = json::parse(command);

			int cmd = jsonObj["commandCode"].get<int>();

			switch (cmd)
			{
				case ECSMSDllMsgType::GET_OCCUPANCY:
					logMIAer.logCommandExec(RequestOccupancy(APIserverId, jsonToSOccupReqData(jsonObj["occupancyParams"]), &requestID), "RequestOccupancy");
					break;
				case ECSMSDllMsgType::GET_OCCUPANCYDF:
					logMIAer.logCommandExec(RequestOccupancyDF(APIserverId, jsonToSOccDFReqData(jsonObj["occupancyParams"]), &requestID), "RequestOccupancyDF");
					break;
				case ECSMSDllMsgType::GET_AVD:
					logMIAer.logCommandExec(RequestAVD(APIserverId, jsonToSAVDReqData(jsonObj["acdParams"]), &requestID), "RequestAVD");
					break;
				case ECSMSDllMsgType::GET_MEAS:
					logMIAer.logCommandExec(RequestMeasurement(APIserverId, jsonToSMeasReqData(jsonObj["measParams"]), &requestID), "RequestMeasurement");
					break;
				case ECSMSDllMsgType::GET_TASK_STATUS:
					logMIAer.logCommandExec(RequestTaskStatus(APIserverId, jsonObj["reqId"].get<unsigned long>()), "RequestTaskStatus");
					break;
				case ECSMSDllMsgType::GET_TASK_STATE:
					logMIAer.logCommandExec(RequestTaskState(APIserverId, (ECSMSDllMsgType)jsonObj["taskType"].get<unsigned long>(), jsonObj["reqId"].get<unsigned long>()), "RequestTaskState");
					break;
				case ECSMSDllMsgType::TASK_SUSPEND:
					logMIAer.logCommandExec(SuspendTask(APIserverId, (ECSMSDllMsgType)jsonObj["taskType"].get<unsigned long>(), jsonObj["reqId"].get<unsigned long>()), "SuspendTask");
					break;
				case ECSMSDllMsgType::TASK_RESUME:
					logMIAer.logCommandExec(ResumeTask(APIserverId, (ECSMSDllMsgType)jsonObj["taskType"].get<unsigned long>(), jsonObj["reqId"].get<unsigned long>()), "ResumeTask");
					break;
				case ECSMSDllMsgType::TASK_TERMINATE:
					logMIAer.logCommandExec(TerminateTask(APIserverId, jsonObj["reqId"].get<unsigned long>()), "TerminateTask");
					break;
				case ECSMSDllMsgType::GET_BIST:
					logMIAer.logCommandExec(RequestBist(APIserverId, (EBistScope)jsonObj["scope"].get<int>(), &requestID), "RequestBist");
					break;
				case ECSMSDllMsgType::SET_AUDIO_PARAMS:
				{
					ERetCode ret = SetAudio(APIserverId, jsonToSAudioParams(jsonObj["audioParams"]), &requestID);
					logMIAer.logCommandExec(ret, "SetAudio");
					if (ret == ERetCode::API_SUCCESS) {
						DWORD processId = wcstoul(L"123", nullptr, 0);
						HRESULT hr = loopbackCapture.StartCaptureAsync(processId, false, L"saida.wav");
						if (FAILED(hr))
						{
							logMIAer.error("Failed to start audio capture");
						}
						logMIAer.info("Capturing 20 seconds of audio.");
						Sleep(20000);

						loopbackCapture.StopCaptureAsync();
						logMIAer.info("Finished audio capture.");
					}
					break;
				}
				case ECSMSDllMsgType::FREE_AUDIO_CHANNEL:
					logMIAer.logCommandExec(FreeAudio(APIserverId, jsonObj["channel"].get<unsigned long>(), &requestID), "FreeAudio");
					break;
				case ECSMSDllMsgType::SET_PAN_PARAMS:
					logMIAer.logCommandExec(SetPanParams(APIserverId, jsonToSPanParams(jsonObj["panParams"]), &requestID), "SetPanParams");
					break;
				case ECSMSDllMsgType::GET_PAN:
					logMIAer.logCommandExec(RequestPan(APIserverId, jsonToSGetPanParams(jsonObj["panParams"]), &requestID), "RequestPan");
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
	StationDisconnect();
	logMIAer.~MIAerLog();

	return static_cast<int>(interruptionCode.load());
}

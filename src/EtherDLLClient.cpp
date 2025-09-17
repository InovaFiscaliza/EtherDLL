/**
* @file etherDLLconfig.hpp
*
* @brief Heder file for configuration file creation with default values
*
* This header file contains the implementation for creating a new configuration file
* It also hold specific DLL linking instructions maintaing EtherDLL base configurations clean of specifi DLL dependencies.
*
*
* * @author fslobao
* * @date 2025-09-10
* * @version 1.0
*
* * @note Requires C++11 or later
* * @note Uses nlohmann/json library for JSON handling
*
**/


// ----------------------------------------------------------------------
// Include core EtherDLL libraries
#include "EtherDLLClient.hpp"

#include "EtherDLLConstants.hpp"

// Include DLL specific libraries

// Include provided DLL libraries


// Include general C++ libraries
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// For convenience
using json = nlohmann::json;


// ----------------------------------------------------------------------
/* Include the standard C++ headers
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
*/

// ----------------------------------------------------------------------
/*
	Receive commands from clients and push them to the command queue, to be sent to the DLL API.
*/
void clientRequestToDLL(SOCKET clientSocket, json config, std::atomic<edll::Code>& interruptionCode, spdlog::logger& log) {
	char buffer[edll::SOCKET_BUFFER_SIZE];

	int checkPeriod = 0;
	int iResult = 0;

	while (interruptionCode.load() == edll::Code::RUNNING) {
		int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead > 0) {
			std::string request(buffer, bytesRead);

			// test if string is a valid JSON
			if (!isValidJSON(request)) {
				log.warn("Received request not in JSON format: " + request);
				continue;
			}
			{

				std::lock_guard<std::mutex> lock(requestQueueMutex);

				

				requestQueue.push_back(request);
			}
			
			std::string ack = edll::Form::ACK + 
				std::to_string(requestQueue.size()) +
				"}"+
				edll::Form::MSG_END;
			iResult = send(clientSocket, ack.c_str(), (int)(ack.length()), 0);
			if (iResult == SOCKET_ERROR) {
				log.warn(name + " ACK send failed. EC:" + std::to_string(WSAGetLastError()));
				log.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				// test if the connection is still alive
				iResult = send(clientSocket,
					edll::Form::PING,
					static_cast<int>(strlen(edll::Form::PING)),
					0);
				if (iResult == SOCKET_ERROR) {
					log.warn(name + " PING send failed. EC:" + std::to_string(WSAGetLastError()));
					log.info(name + " connection with address" + std::to_string(clientSocket) + " lost.");
					return;
				}

				log.info(name + " waiting for commands from client...");
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
void DLLResponseToClient(SOCKET clientSocket, json config, std::atomic<edll::Code>& interruptionCode, spdlog::logger& log) {

	int checkPeriod = 0;
	int iResult = 0;
	while (interruptionCode.load() == edll::Code::RUNNING) {
		if (!streamBuffer.empty()) {
			std::string data = streamBuffer.back() + edll::Form::MSG_END;
			streamBuffer.pop_back();
			iResult = send(clientSocket, data.c_str(), static_cast<int>(data.length()), 0);
			if (iResult == SOCKET_ERROR) {
				log.warn(name + " data send failed. EC:" + std::to_string(WSAGetLastError()));
				return;
			}
		}
		else {
			if (checkPeriod == 0) {
				log.info(name + " waiting for data from station to send...");
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
/** @brief Wait and stablish connection to a single client.
 *
 * This function creates a listening socket on the specified port and waits for a client to connect.
 * It sets a timeout for receiving data on the socket.
 * If a client connects, it returns the client socket.
 * If an error occurs, it logs the error and sets the interruption code.
 *
 * @param port: Port number to listen for incoming connections
 * @param timeout: Timeout in milliseconds for receiving data on the socket
 * @param log: spdlog logger object for logging messages
 * @return SOCKET: Client socket if connection is successful, INVALID_SOCKET otherwise
 * @throws NO EXCEPTION HANDLING
 **
*/
SOCKET establishClientCommunication(int port, int timeout, std::atomic<edll::Code>& interruptionCode, spdlog::logger& log)
{
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
		log.error("Socket getaddrinfo failed. EC:" + std::to_string(iResult));
		interruptionCode = edll::Code::CLIENT_ERROR;
		WSACleanup();
		return;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		log.error("Socket creation failed. EC:" + std::to_string(WSAGetLastError()));
		interruptionCode = edll::Code::CLIENT_ERROR;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	iResult = setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	if (iResult == SOCKET_ERROR) {
		log.error("Socket setsockopt timeout failed. EC:" + std::to_string(WSAGetLastError()));
		interruptionCode = edll::Code::CLIENT_ERROR;
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	iResult = ::bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		log.error("Socket bind failed. EC:" + std::to_string(WSAGetLastError()));
		interruptionCode = edll::Code::CLIENT_ERROR;
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		log.error("Socket listen failed. EC:" + std::to_string(WSAGetLastError()));
		interruptionCode = edll::Code::CLIENT_ERROR;
		closesocket(listenSocket);
		WSACleanup();
		return;
	}

	// create variable to store the client address
	struct sockaddr_in clientAddr;

	SOCKET clientSocket = INVALID_SOCKET;
	while (interruptionCode.load() == edll::Code::RUNNING) {
		// call the connection handler 
		log.info("Waiting for client connections on port " + std::to_string(port));

		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, NULL);
		if (clientSocket == INVALID_SOCKET) {
			log.warn("Failed in accept operation with " + std::string(inet_ntoa(clientAddr.sin_addr)) + ".EC:" + std::to_string(WSAGetLastError()));
		}
		else {
			log.info("Accepted connection from " + std::string(inet_ntoa(clientAddr.sin_addr)));

			closesocket(listenSocket);
			WSACleanup();
			log.info("Stopped waiting for new connections on port " + std::to_string(port));

			return clientSocket;
		}
	}
}

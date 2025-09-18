/**
* @file EtherDLLClient.cpp
*
* @brief Functions to handle client connections and data transmission.
*
* Implements functions to establish a single client connections using socket
* No multi-client is supported.
* Fuctions will hold threads to handle communication with the client and dll
* thus they must be run in separate threads.
* It handles receiving commands from the client, pushing them to the requestQueue,
* It handles sending data from the DLL API to the client by reading from the responseQueue.
*
* * @author fslobao
* * @date 2025-09-18
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
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
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
/** @brief Send messages from client to the DLL, whenever there are messages available
 *
 * This function will lock the thread. Must be run in a separate thread.
 * Messages are expected to be in JSON format and end with the defined message end sequence.
 * Each complete message will be acknowledged with an ACK or NACK response.
 * If no data is received, a PING message will be sent periodically to check if the connection is still alive.
 *
 * @param clientSocket: Socket connected to the client
 * @param config: JSON object containing configuration
 * @param request: Thread-safe message queue containing messages to be sent to the DLL
 * @param interruptionCode: Signal interruption for service interruption
 * @param log: spdlog logger object for logging messages
 * @throws NO EXCEPTION HANDLING
*/
void clientRequestToDLL(SOCKET clientSocket, json config, messageQueue& request, edll::INT_CODE& interruptionCode, spdlog::logger& log)
{
	char buffer[edll::SOCKET_BUFFER_SIZE];
	std::string accumulatedData = "";

	int checkPeriod = 0;
	int iResult = 0;

	while (interruptionCode == edll::Code::RUNNING) {
		int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
		
		if (bytesRead > 0) {
			size_t pos = accumulatedData.find(edll::Form::MSG_END);

			if (pos != std::string::npos) { // Found the end of a message
				
				std::string completeMessage = accumulatedData.substr(0, pos);

				json jsonObj = json::parse(completeMessage, nullptr, false);

				if (jsonObj != json::value_t::discarded) {
					request.push(completeMessage);
					std::string messageID = jsonObj.value(edll::Form::MSG_ID, "");
					log.debug("Received message ID: " + messageID);
					std::string ack = edll::Form::ACK + messageID + edll::Form::MSG_END;
					iResult = send(clientSocket, ack.c_str(), ack.length(), 0);
				}
				else {
					log.warn("Received invalid JSON message: " + completeMessage);
					std::string nack = std::string(edll::Form::NACK) + edll::Form::MSG_END;
					iResult = send(clientSocket, nack.c_str(), nack.length(), 0);
				}

				// keep any extra data after the end of the message for the next iteration
				if (pos != accumulatedData.length() - strlen(edll::Form::MSG_END)) {	
					accumulatedData = accumulatedData.substr(pos + strlen(edll::Form::MSG_END), std::string::npos);
					log.debug("Keeping data for next iteration: " + accumulatedData);
				}
				else {
					accumulatedData.clear();
				}

				if (iResult == SOCKET_ERROR) {
					log.warn("Failed sending ACK/NACK message. EC:" + std::to_string(WSAGetLastError()));
					log.info("Connection with address" + std::to_string(clientSocket) + " lost.");
					return;
				}	
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
					log.warn("PING send failed. EC:" + std::to_string(WSAGetLastError()));
					log.info("Connection with address" + std::to_string(clientSocket) + " lost.");
					return;
				}

				log.info("Waiting for commands from client...");
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
/** @brief Send messages from the DLL API to the client, whenever there are messages available
 * 
 * This function will lock the thread. Must be run in a separate thread.
 * 
 * @param clientSocket: Socket connected to the client
 * @param config: JSON object containing configuration
 * @param response: Thread-safe message queue containing messages to be sent to the client
 * @param interruptionCode: Signal interruption for service interruption
 * @param log: spdlog logger object for logging messages
 * @throws NO EXCEPTION HANDLING
*/
void DLLResponseToClient(SOCKET clientSocket, json config, messageQueue& response, edll::INT_CODE& interruptionCode, spdlog::logger& log) {

	int checkPeriod = 0;
	int iResult = 0;
	while (interruptionCode.load() == edll::Code::RUNNING) {
		if (!response.queue.empty()) {
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
/** @brief Wait and establish connection to a single client.
 *
 * This function creates a listening socket on the specified port and waits for a client to connect.
 * If a client connects, it returns the client communication socket and close the listening socket.
 * It sets sockect configuration such as a timeout .
 * If an error occurs, it logs the error and sets the interruption code.
 *
 * @param config: JSON object containing configuration parameters
 * @param interruptionCode: Signal interruption for service interruption
 * @param log: spdlog logger object for logging messages
 * @return SOCKET: Client socket if connection is successful, INVALID_SOCKET otherwise
 * @throws NO EXCEPTION HANDLING
 **
*/
SOCKET establishClientCommunication(json config, edll::INT_CODE& interruptionCode, spdlog::logger& log)
{
	SOCKET listenSocket = INVALID_SOCKET;
	struct addrinfo* result = nullptr;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	std::string portStr = std::to_string(config["service"]["command"]["port"].get<int>());
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

	int timeout = config["service"]["command"]["timeout_s"].get<int>();
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

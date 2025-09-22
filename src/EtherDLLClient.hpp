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

#pragma once

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
#include <chrono>
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
/** @brief Thread-safe message queue object
* 
* Provides methods to handle messages from the queue in a thread-safe manner.
**/
class MessageQueue {
private:
	// Internal queue to hold messages
	std::queue<json> msgQueue;
	// Mutex to protect access to the queue
	mutable std::mutex mtx;
	// Total number of messages ever added to the queue. May return to zero if it overflows.
	unsigned long messageCount = 0;

public:
	/** @brief Push an item to the queue in a thread-safe manner
	 *
	 * @param item: Item to be pushed to the queue
	 * @return unsigned long: Message count for the item just added
	 * @throws NO EXCEPTION HANDLING
	**/
	unsigned long push(const json& item) {
		std::lock_guard<std::mutex> lock(mtx);
		msgQueue.push(item);
		return messageCount++;
	}

	/** @brief Pop an item from the front of the queue in a thread-safe manner
	 * Add message termination sequence to the message upon popping.
	 *
	 * @param None
	 * @return json: Item popped from the front of the queue, or empty string if the queue is empty
	 * @throws NO EXCEPTION HANDLING
	**/
	json pop() {
		std::lock_guard<std::mutex> lock(mtx);
		if (msgQueue.empty()) {
			return json{};
		}
		json item = msgQueue.front();
		msgQueue.pop();
		return item;
	}

	/** @brief Check if the queue is empty in a thread-safe manner
	 *
	 * @param None
	 * @return bool: True if the queue is empty, false otherwise
	 * @throws NO EXCEPTION HANDLING
	**/
	bool empty() const {
		std::lock_guard<std::mutex> lock(mtx);
		return msgQueue.empty();
	}

	/** @brief Get the size of the queue in a thread-safe manner
	 *
	 * @param None
	 * @return size_t: Number of items in the queue
	 * @throws NO EXCEPTION HANDLING
	**/
	size_t size() const {
		std::lock_guard<std::mutex> lock(mtx);
		return msgQueue.size();
	}

	/** @brief Get the total number of messages ever added to the queue
	 * Value may return to zero if it overflows UINT64 max value.
	 * @param None
	 * @return unsigned long: Total number of messages added to the queue
	 * @throws NO EXCEPTION HANDLING
	 **/
	unsigned long getMessageCount() const {
		std::lock_guard<std::mutex> lock(mtx);
		return messageCount;
	}
};


// ----------------------------------------------------------------------
/** @brief Socket client connection object
 *
 * This class will handle the creation of a listening socket, waiting for a client to connect,
 * It stores socket client and associated info.
 * Provides methods to process messages from the client and send responses back and connection info.
 * Close the connection when done.
 *
 * @param config: JSON object containing configuration parameters
 * @param interruptionCode: Signal interruption for service interruption
 * @param log: spdlog logger object for logging messages
 * @return ClientConn object if connection is successful
 * @throws NO EXCEPTION HANDLING
 **/
class ClientConn {
private:
	json config;
	edll::INT_CODE& interruptionCode;
	spdlog::logger& log;

	SOCKET clientSocket = INVALID_SOCKET;
	std::string clientIP;

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
	void establishConnection()
	{
		SOCKET listenSocket = INVALID_SOCKET;
		struct addrinfo* result = nullptr;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		std::string portStr = std::to_string(config["service"]["command"].value("port", edll::DEFAULT_SERVICE_PORT));
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

		int timeout = config["service"]["command"].value("timeout_s", edll::DEFAULT_TIMEOUT_S) * 1000; // milliseconds
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
		struct sockaddr_in clientAddr {};

		while (interruptionCode == edll::Code::RUNNING) {
			// call the connection handler 
			log.info("Waiting for client connections on port " + portStr);

			clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, NULL);

			// configure socket for non-blocking operations
			u_long mode = 1;
			ioctlsocket(clientSocket, FIONBIO, &mode);

			{
				struct sockaddr_in clientAddr {};
				int addrLen = sizeof(clientAddr);
				if (getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen) == 0) {
					clientIP = std::string(inet_ntoa(clientAddr.sin_addr));
					if (!clientIP.empty())
						log.debug("Processing message from client IP: " + clientIP);
				}
			}

			if (clientSocket == INVALID_SOCKET) {
				log.warn("Failed in accept operation with " + clientIP + ".EC:" + std::to_string(WSAGetLastError()));
			}
			else {
				log.info("Accepted connection from " + clientIP);

				closesocket(listenSocket);
				WSACleanup();
				log.info("Stopped waiting for new connections on port " + portStr);

				return;
			}
		}
	}

public:
	// ----------------------------------------------------------------------
	/** @brief Establish a connection with a client
	 *
	 * This function will block the thread until a client connects or an interruption signal is received.
	 * It will create a socket, bind it to the specified port and listen for incoming connections.
	 * Once a client connects, it will store the client socket and IP address.
	 *
	 * @throws NO EXCEPTION HANDLING
	**/
	ClientConn(json config,
		edll::INT_CODE& interruptionCode, spdlog::logger& log)
		: config(config),
		interruptionCode(interruptionCode), log(log), clientIP("")
	{
		establishConnection();
	}


	// ----------------------------------------------------------------------
	/** @brief Send messages from client to the DLL, whenever there are messages available
	 *
	 * This function will lock the thread. Must be run in a separate thread.
	 * Messages are expected to be in JSON format and end with the defined message end sequence.
	 * Each complete message will be acknowledged with an ACK or NACK response.
	 * ACK will contain the message ID if available, NACK will contain the length of the invalid message.
	 * If no data is received, a PING message will be sent periodically to check if the connection is still alive.
	 *
	 * @param clientSocket: Socket connected to the client
	 * @param config: JSON object containing configuration
	 * @param request: Thread-safe message queue containing messages to be sent to the DLL
	 * @param interruptionCode: Signal interruption for service interruption
	 * @param log: spdlog logger object for logging messages
	 * @throws NO EXCEPTION HANDLING
	*/
	void clientRequestToDLL(MessageQueue& request)
	{
		std::string msgJsonStartStr = "{\"";
		std::string msgJsonMidStr = "\":";

		std::string msgEndStr = config["service"]["msg_keys"].value("end", edll::DEFAULT_END_MSG);
		std::string ackStr = config["service"]["msg_keys"].value("ack", edll::DEFAULT_ACK_MSG);
		std::string nackStr = config["service"]["msg_keys"].value("nack", edll::DEFAULT_NACK_MSG);

		std::string idStr = config["service"]["msg_keys"].value("id", edll::DEFAULT_ID);

		ackStr = msgJsonStartStr + ackStr + msgJsonMidStr;
		nackStr = msgJsonStartStr + nackStr + msgJsonMidStr;
		std::string msgJsonEndStr = "}" + msgEndStr;

		char buffer[edll::SOCKET_BUFFER_SIZE];
		std::string accumulatedData = "";

		int bufferTTLInit = config["service"].value("buffer_ttl_period", edll::DEFAULT_BUFFER_TTL_PERIOD);
		int bufferTTL = bufferTTLInit;

		int iResult = 0;

		while (interruptionCode == edll::Code::RUNNING) {

			// read data from socket - blocking call
			int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

			if (bytesRead > 0) {

				accumulatedData.append(buffer, bytesRead);

				size_t pos = accumulatedData.find(msgEndStr);

				if (pos != std::string::npos) { // Found the end of a message

					std::string completeMessage = accumulatedData.substr(0, pos);

					json jsonObj = json::parse(completeMessage, nullptr, false);

					if (jsonObj != json::value_t::discarded) {

						// add client id and queue id to object
						jsonObj[edll::CLIENT_IP] = clientIP;
						if (!jsonObj.contains(idStr)) {
							jsonObj[idStr] = "No Client Message ID";
						}
						jsonObj[edll::RECEIVE_MSG_COUNT] = request.push(jsonObj);

						std::string messageID = jsonObj[idStr];
						log.debug("Received message ID: " + messageID);
						std::string ack = ackStr + messageID + msgEndStr;
						iResult = send(clientSocket, ack.c_str(), ack.length(), 0);
					}
					else {
						log.warn("Received invalid JSON message: " + completeMessage);
						std::string nack = nackStr + std::to_string(completeMessage.length()) + msgEndStr;
						iResult = send(clientSocket, nack.c_str(), nack.length(), 0);
					}

					// keep any extra data after the end of the message for the next iteration
					if (pos != accumulatedData.length() - msgEndStr.length()) {
						accumulatedData = accumulatedData.substr(pos + msgEndStr.length(), std::string::npos);
						log.debug("Keeping data for next iteration: " + accumulatedData);
						bufferTTL--;
					}
					else {
						accumulatedData.clear();
						bufferTTL = bufferTTLInit;
					}

					if (iResult == SOCKET_ERROR) {
						log.warn("Failed sending ACK/NACK message. EC:" + std::to_string(WSAGetLastError()));
						log.info("Connection with address" + std::to_string(clientSocket) + " lost.");
						return;
					}
				}
			}
			else {
				int error = WSAGetLastError();

				if (error == WSAEWOULDBLOCK) {
					// No data available to read
					std::this_thread::sleep_for(std::chrono::milliseconds(config["service"].value("sleep_ms", edll::DEFAULT_SLEEP_MS)));
				}
				else {
					// Unkwonn connection error
					log.error("Client connection error: " + std::to_string(error));
					return;
				}

				if (bufferTTL == 0) {
					if (accumulatedData.length() > 0) {
						log.debug("Buffer TTL expired. Clearing accumulated data: " + accumulatedData);
						accumulatedData.clear();
					}
					bufferTTL = bufferTTLInit;
				}
			}
		}
	}


	// ----------------------------------------------------------------------
	/** @brief Send messages from the DLL API to the client, whenever there are messages available
	 *
	 * This function will lock the thread. Must be run in a separate thread.
	 * Messages are expected to be in JSON format and end with the defined message end sequence.
	 * If no data is available to send, a PING message will be sent periodically to check if the connection is still alive.
	 * PING message will contain the current timestamp in milliseconds since epoch.
	 *
	 * @param clientSocket: Socket connected to the client
	 * @param config: JSON object containing configuration
	 * @param response: Thread-safe message queue containing messages to be sent to the client
	 * @param interruptionCode: Signal interruption for service interruption
	 * @param log: spdlog logger object for logging messages
	 * @throws NO EXCEPTION HANDLING
	*/
	void DLLResponseToClient(MessageQueue& response)
	{
		std::string msgJsonStartStr = "{\"";
		std::string msgJsonMidStr = "\":";

		std::string msgEndStr = config["service"]["msg_keys"].value("end", edll::DEFAULT_END_MSG);
		std::string pingStr = config["service"]["msg_keys"].value("ping", edll::DEFAULT_PING_MSG);
		bool pingEnable = config["service"].value("ping_enable", edll::DEFAULT_PING_STATE);

		pingStr = msgJsonStartStr + pingStr + msgJsonMidStr;

		std::string msgJsonEndStr = "}" + msgEndStr;

		int pingPeriodInit = config["service"].value("ping_period", edll::DEFAULT_PING_PERIOD);
		int pingPeriod = pingPeriodInit;

		int iResult = 0;

		while (interruptionCode == edll::Code::RUNNING) {
			if (!response.empty()) {
				std::string message = response.pop().dump() + msgEndStr;

				iResult = send(clientSocket, message.c_str(), static_cast<int>(message.length()), 0);
				if (iResult == SOCKET_ERROR) {
					log.warn("Data send failed. EC:" + std::to_string(WSAGetLastError()));
					return;
				}
			}
			else {
				if (pingPeriod == 0 && pingEnable) {
					// test if the connection is still alive
					auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::system_clock::now().time_since_epoch()).count();
					std::string ping = pingStr + std::to_string(now) + msgJsonEndStr;

					iResult = send(clientSocket, ping.c_str(), ping.length(), 0);

					if (iResult == SOCKET_ERROR) {
						log.warn("PING send failed. EC:" + std::to_string(WSAGetLastError()));
						log.info("Connection with address" + std::to_string(clientSocket) + " lost.");
						return;
					}

					log.info("Waiting for commands from client...");
					pingPeriod = pingPeriodInit;
				}
				else {
					pingPeriod--;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(config["service"].value("sleep_ms", edll::DEFAULT_SLEEP_MS)));
			}
		}
	}

	// ----------------------------------------------------------------------
	/** @brief Getter for client socket
	 * @param None
	 * @return SOCKET: Client socket if connection is established, INVALID_SOCKET otherwise
	 * @throws NO EXCEPTION HANDLING
	**/
	SOCKET getClientSocket() const {
		return clientSocket;
	}

	// ----------------------------------------------------------------------
	/** @brief Check if connection is established
	 * @param None
	 * @return bool: True if connection is established, false otherwise
	 * @throws NO EXCEPTION HANDLING
	**/
	bool isConnected() const {
		return clientSocket != INVALID_SOCKET;
	}

	// ----------------------------------------------------------------------
	/** @brief Getter for client IP address
	 * @param None
	 * @return std::string: Client IP address if connection is established, empty string otherwise
	 * @throws NO EXCEPTION HANDLING
	**/
	std::string getClientIP() const {
		return clientIP;
	}

	// ----------------------------------------------------------------------
	/** @brief Destructor to clean up socket
	 * @param None
	 * @return void
	 * @throws NO EXCEPTION HANDLING
	**/
	~ClientConn() {
		if (clientSocket != INVALID_SOCKET) {
			closesocket(clientSocket);
		}
	}
};
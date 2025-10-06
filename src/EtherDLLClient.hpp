/**
* @file etherDLLInit.hpp
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
* * @note Requires C++17 or later
* * @note Uses nlohmann/json library for JSON handling
*
**/

#pragma once

// Include DLL specific libraries

// Include core EtherDLL libraries
#include "EtherDLLConstants.hpp"
#include "EtherDLLLog.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <string>
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

// For convenience
using json = nlohmann::json;

using service = edll::DefaultConfig::Service;
using taskKeys = edll::DefaultConfig::Service::TaskKeys;

// Global variables
extern spdlog::logger* loggerPtr;


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
	// Condition variable to signal addition of messages
	std::condition_variable push_condition;
	// Flag to indicate that a message was pushed
	bool messagePushed = false;
	// Condition variable to signal removal of messages
	std::condition_variable pop_condition;



public:
	/** @brief Push an item to the queue in a thread-safe manner
	 * Signal any waiting threads that a new item is available.
	 * Upon and pushing, add to the item the queue ID
	 * Optionally add the queue ID to another key, 
	 * 
	 * @param item: Item to be pushed to the queue
	 * @param setClientKey: If true, also set the client ID key to the same value as the queue ID (default false)
	 * @param logSource: Message to log upon pushing the item
	 * @return unsigned long: Message count for the item just added
	 * @throws NO EXCEPTION HANDLING
	**/
	unsigned long push(json item, std::string logSource, bool setClientKey = false) {

		std::lock_guard<std::mutex> lock(mtx);
		item[taskKeys::QueueId::VALUE] = messageCount;
		if (setClientKey) {
			item[taskKeys::ClientId::VALUE] = messageCount;
		}
		msgQueue.push(item);
		messagePushed = true;

		push_condition.notify_all();
		loggerPtr->debug(logSource + " pushed item to queue. New size: {}", msgQueue.size());

		return messageCount++;
	}

	/** @brief Push an item to the queue in a thread-safe manner
	 * Signal any waiting threads that a new item is available.
	 * Upon and pushing, add to the item the queue ID
	 * Optionally add the queue ID to another key,
	 * After pushing, wait until the item is popped from the queue
	 *
	 * @param item: Item to be pushed to the queue
	 * @param setClientKey: If true, also set the client ID key to the same value as the queue ID (default false)
	 * @param logSource: Message to log upon pushing the item
	 * @return unsigned long: Message count for the item just added
	 * @throws NO EXCEPTION HANDLING
	**/
	unsigned long pushAndWait(json item, std::string logSource, const edll::INT_CODE& interruptionCode, bool setClientKey = false) {

		
		unsigned long msgCount = push(item, logSource, setClientKey);

		// Wait until the item is popped from the queue or an interruption is signaled

		{
			std::unique_lock<std::mutex> lock(mtx);
			pop_condition.wait(lock, [this, msgCount, &interruptionCode] {
				return messageCount != msgCount || interruptionCode != edll::Code::RUNNING;
				});
		}
		return msgCount;
	
	}

	/** @brief Pop an item from the front of the queue in a thread-safe manner
	 * Add message termination sequence to the message upon popping.
	 *
	 * @param logSource: Message to log upon popping the item
	 * @return json: Item popped from the front of the queue, or empty string if the queue is empty
	 * @throws NO EXCEPTION HANDLING
	**/
	json pop(std::string logSource) {
		std::lock_guard<std::mutex> lock(mtx);
		if (msgQueue.empty()) {
			return json{};
		}
		json item = msgQueue.front();
		msgQueue.pop();
		pop_condition.notify_all();

		loggerPtr->debug(logSource + " popped item from queue. New size: {}", msgQueue.size());
		return item;
	}

	/** @brief Wait for and pop an item from the front of the queue in a thread-safe manner
	* Blocks until an item is available or interruption is signaled.
	*
	* @param interruptionCode: Reference to interruption signal to check for shutdown
	* @param logSource: Message to log upon popping the item
	* @return json: Item popped from the front of the queue, or empty json if interrupted
	* @throws NO EXCEPTION HANDLING
	**/
	json waitAndPop(const edll::INT_CODE& interruptionCode, std::string logSource) {

		{
			std::unique_lock<std::mutex> lock(mtx);

			loggerPtr->debug(logSource + " waiting for item in message queue");

			// Wait until queue is not empty or we're interrupted
			push_condition.wait(lock, [this, &interruptionCode] {
				return !msgQueue.empty() || interruptionCode != edll::Code::RUNNING;
				});
		}

		// If wait was interrupted, return empty json
		if (interruptionCode != edll::Code::RUNNING) {
			return json(); 
		}

		return pop(logSource);
	}

	/** @brief Hold the thread execution waiting for any action in the queue, interruption or timeout
	 *
	 * @param interruptionCode: Reference to interruption signal to check for shutdown
	 * @param timeoutMs: Maximum time to wait in milliseconds (default 1000 ms)
	 * @return bool: True if the queue changed or , false otherwise
	 * @throws NO EXCEPTION HANDLING
	**/
	bool waitAction(const edll::INT_CODE& interruptionCode, std::string logSource, int timeoutMs = 1000) {

		std::unique_lock<std::mutex> lock(mtx);

		loggerPtr->debug(logSource + " waiting for item in message queue");

		// Wait until queue is not empty or we're interrupted
		bool notified = push_condition.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this, &interruptionCode] {
			return interruptionCode != edll::Code::RUNNING;
			});

		if (messagePushed) {
			messagePushed = false;
			return interruptionCode == edll::Code::RUNNING;
		}
		else {
			return false;
		}
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
 * @param logger: spdlog logger object for logging messages
 * @return ClientConn object if connection is successful
 * @throws NO EXCEPTION HANDLING
 **/
class ClientConn {
private:

	// Configuration parameters
	json config;
	edll::INT_CODE& interruptionCode;
	spdlog::logger& logger;

	// Client socket and info
	SOCKET clientSocket = INVALID_SOCKET;
	std::string clientIP;

	std::string msgJsonStartStr = std::string(edll::JSON_START);
	std::string msgJsonMidStr = std::string(edll::JSON_MID);


	json msgKeys = config[service::KEY][service::Msg::KEY].get<json>();

	std::string msgEndStr = msgKeys[service::Msg::End::KEY].get<std::string>();
	std::string msgJsonEndStr = std::string(edll::JSON_END) + msgEndStr;
	std::string ackStr = msgJsonStartStr + msgKeys[service::Msg::Ack::KEY].get<std::string>() + msgJsonMidStr;
	std::string nackStr = msgJsonStartStr + msgKeys[service::Msg::Nack::KEY].get<std::string>() + msgJsonMidStr;
	std::string pingStr = msgJsonStartStr + msgKeys[service::Msg::Ping::KEY].get<std::string>() + msgJsonMidStr;

	bool pingEnable = config[service::KEY][service::PingEnable::KEY].get<bool>();

	std::string idStr = config[service::KEY][taskKeys::KEY][taskKeys::ClientId::KEY].get<std::string>();

	std::chrono::steady_clock::time_point lastClientMsgTime = std::chrono::steady_clock::now();


	// ----------------------------------------------------------------------
	/** @brief Build message to signal to the client the service will shut down
	 *
	 * @param None
	 * @return json: Message indicating service shutdown
	 * @throws NO EXCEPTION HANDLING
	**/
	json buildServiceInterruptionMsg(const edll::INT_CODE& interruptionCode) {
		json msg;

		msg[taskKeys::CommandCode::VALUE] = taskKeys::CommandCode::INIT_VALUE;
		msg[taskKeys::CommandName::VALUE] = taskKeys::CommandName::INIT_VALUE;
		msg[taskKeys::Arguments::VALUE] = json::object();
		msg[taskKeys::Message::VALUE] = std::string("Service interruption. Code: ") + std::to_string(interruptionCode);

		return msg;
	}

	// ----------------------------------------------------------------------
	/** @brief Wait and establish connection to a single client.
	 *
	 * This function creates a listening socket on the specified port and waits for a client to connect.
	 * If a client connects, it returns the client communication socket and close the listening socket.
	 * It sets sockect configuration such as a timeout .
	 * If an error occurs, it logs the error and sets the interruption code.
	 *
	 * @param none
	 * @return void
	 * @throws NO EXCEPTION HANDLING
	**/
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

		json service_config = config[service::KEY].get<json>();

		std::string portStr = std::to_string(service_config[service::Port::KEY].get<int>());
		int iResult = getaddrinfo(NULL, portStr.c_str(), &hints, &result);
		if (iResult != 0) {
			loggerPtr->error("Socket getaddrinfo failed. EC:" + std::to_string(iResult));
			interruptionCode = edll::Code::CLIENT_ERROR;
			WSACleanup();
			return;
		}

		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET) {
			loggerPtr->error("Socket creation failed. EC:" + std::to_string(WSAGetLastError()));
			interruptionCode = edll::Code::CLIENT_ERROR;
			freeaddrinfo(result);
			WSACleanup();
			return;
		}

		int timeout = service_config[service::Timeout::KEY].get<int>() * 1000; // milliseconds
		iResult = setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
		if (iResult == SOCKET_ERROR) {
			loggerPtr->error("Socket setsockopt timeout failed. EC:" + std::to_string(WSAGetLastError()));
			interruptionCode = edll::Code::CLIENT_ERROR;
			freeaddrinfo(result);
			closesocket(listenSocket);
			WSACleanup();
			return;
		}

		iResult = ::bind(listenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
		if (iResult == SOCKET_ERROR) {
			loggerPtr->error("Socket bind failed. EC:" + std::to_string(WSAGetLastError()));
			interruptionCode = edll::Code::CLIENT_ERROR;
			freeaddrinfo(result);
			closesocket(listenSocket);
			WSACleanup();
			return;
		}

		freeaddrinfo(result);

		iResult = listen(listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			loggerPtr->error("Socket listen failed. EC:" + std::to_string(WSAGetLastError()));
			interruptionCode = edll::Code::CLIENT_ERROR;
			closesocket(listenSocket);
			WSACleanup();
			return;
		}

		// create variable to store the client address
		struct sockaddr_in clientAddr {};

		// call the connection handler 
		loggerPtr->info("Waiting for client connections on port " + portStr);

		// hold the excecution until a client connects
		clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, NULL);

		// configure socket for non-blocking operations
		// u_long mode = 1;
		// ioctlsocket(clientSocket, FIONBIO, &mode);

		int addrLen = sizeof(clientAddr);
		if (getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen) == 0) {
			clientIP = std::string(inet_ntoa(clientAddr.sin_addr));
			if (!clientIP.empty()) {
				loggerPtr->debug("Waiting message from client IP: " + clientIP);
			}
			else {
				clientIP = taskKeys::ClientIp::INIT_VALUE;
				loggerPtr->debug("IP address from connected client could not be determined.");
			}

		}

		if (clientSocket == INVALID_SOCKET) {
			loggerPtr->warn("Failed in accept operation with " + clientIP + ".EC:" + std::to_string(WSAGetLastError()));
		}
		else {
			loggerPtr->info("Accepted connection from " + clientIP);
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
		edll::INT_CODE& interruptionCode, spdlog::logger& logger)
		: config(config),
		interruptionCode(interruptionCode), logger(logger), clientIP("")
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
	 * If no message ID is provided by the client, a sequential number will be generated and returned in the ACK message.
	 * The client provided or generated message ID will be used to track responses from the DLL back to the client.
	 *
	 * @param request: Thread-safe message queue containing messages to be sent to the DLL
	 * @param response: Thread-safe message queue containing messages to be sent to the client
	 * @throws NO EXCEPTION HANDLING
	*/
	void clientRequestToDLL(MessageQueue& request, MessageQueue& response)
	{
		const std::string logSource = "ClientRequestToDLL";

		json serviceKeys = config[edll::DefaultConfig::Service::KEY].get<json>();

		int bufferSize = serviceKeys[service::BufferSize::KEY].get<int>();
		std::string buffer;
		buffer.resize(bufferSize + 1);
		std::string accumulatedData = "";

		int bufferTTLInit = serviceKeys[service::BufferTTL::KEY].get<int>();
		int bufferTTL = bufferTTLInit;

		int iResult = 0;

		loggerPtr->debug(logSource + " is waiting for messages from " + clientIP);

		while (interruptionCode == edll::Code::RUNNING) {

			// read data from socket - blocking call
			int bytesRead = recv(clientSocket, buffer.data(), static_cast<int>(bufferSize), 0);

			if (bytesRead > 0) {

				bool dataToProcess = true;

				json jsonObj = json::parse(buffer, nullptr, false);

				
				if (jsonObj == json::value_t::discarded) {
					accumulatedData.append(buffer.data(), bytesRead);

					jsonObj = json::parse(accumulatedData, nullptr, false);

					if (jsonObj == json::value_t::discarded) {
						loggerPtr->debug(logSource + " accumulated data. So far: " + accumulatedData);
						bufferTTL--;
						dataToProcess = false;

						json nackObj;
						nackObj[service::Msg::Nack::VALUE] = std::to_string(accumulatedData.length());
						response.push(nackObj, logSource, true);
					}
				}

				if (dataToProcess) {
					bufferTTL = bufferTTLInit;
					accumulatedData.clear();

					// add client id and queue id to object
					jsonObj[taskKeys::ClientIp::VALUE] = clientIP;

					if (jsonObj.contains(idStr)) {
						jsonObj[taskKeys::QueueId::VALUE] = request.push(jsonObj, logSource, false);
					}
					else
					{
						jsonObj[idStr] = request.push(jsonObj, logSource, true);

						// add the queue id to the message for debug/tracking purposes
						jsonObj[taskKeys::QueueId::VALUE] = jsonObj[idStr];
					}
						
					loggerPtr->debug(logSource + " received message from client: " + jsonObj.dump());

					json ackObj;
					ackObj[service::Msg::Ack::VALUE] = jsonObj[idStr];
					response.push(ackObj, logSource, true);
				}
			}
			else {
				int error = WSAGetLastError();

				if (bufferTTL == 0) {
					if (accumulatedData.length() > 0) {
						loggerPtr->debug(logSource + " Buffer TTL expired. Clearing accumulated data: " + accumulatedData);
						accumulatedData.clear();
					}
					bufferTTL = bufferTTLInit;
				}

				if (error == WSAETIMEDOUT) {
					if (interruptionCode != edll::Code::RUNNING) {
						response.push(buildServiceInterruptionMsg(interruptionCode), logSource, true);
					}
					continue;
				}
				
				if (error != WSAEWOULDBLOCK) {
					// Unknown connection error
					loggerPtr->error(logSource + " Client connection error: " + std::to_string(error));
					return;
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
	 * @param logger: spdlog logger object for logging messages
	 * @throws NO EXCEPTION HANDLING
	*/
	void DLLResponseToClient(MessageQueue& response)
	{
		const std::string logSource = "DLLResponseToClient";

		int iResult = 0;

		while (interruptionCode == edll::Code::RUNNING)
		{
			json oneResponse = response.waitAndPop(interruptionCode, logSource);

			std::string message = oneResponse.dump() + msgEndStr;

			iResult = send(clientSocket, message.c_str(), static_cast<int>(message.length()), 0);
			if (iResult == SOCKET_ERROR) {
				loggerPtr->warn(logSource + "data send failed. EC:" + std::to_string(WSAGetLastError()));
			}
			else {
				loggerPtr->debug(logSource + " sent message to client: " + message);

				// reset message timer to avoid unnecessary pings if communication is active
				lastClientMsgTime = std::chrono::steady_clock::now();
			}
		}
	}

	// ----------------------------------------------------------------------
	/** @brief Ping the client periodically to check if the connection is still alive
	 * 
	 * This function will lock the thread. Must be run in a separate thread.
	 * PING message will contain the current timestamp in milliseconds since epoch.
	 * If DemoMode is enabled in the configuration, it will also send demo data messages periodically.
	 * after the ping has been sent.
	 *
	 * @param None
	 * @return void
	 * @throws NO EXCEPTION HANDLING
	**/
	void pingClient(MessageQueue& response)
	{
		const std::string logSource = "pingClient";

		int pingPeriodMs = config[service::KEY][service::PingPeriod::KEY].get<double>() * 1000;
		int iResult = 0;

		while (interruptionCode == edll::Code::RUNNING) {

			bool timeout = response.waitAction(interruptionCode, logSource, pingPeriodMs);

			auto now = std::chrono::steady_clock::now();

			if (timeout) {
				std::chrono::milliseconds timeSinceLastMsg = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClientMsgTime);
				if (timeSinceLastMsg.count() > pingPeriodMs) {
					json pingObj;
					pingObj[service::Msg::Ping::VALUE] = now.time_since_epoch().count();
					response.pushAndWait(pingObj, logSource, interruptionCode, true);
				}
			} 
			else {
				if (config[service::KEY][service::DemoMode::KEY].get<bool>() && !timeout) {
					response.pushAndWait(buildDemoData(), logSource, interruptionCode, true);
				}
			}
			// sleep a 100 ms to avoid overload in case of error
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
	/** @brief Close the client connection
	 * @param None
	 * @return void
	 * @throws NO EXCEPTION HANDLING
	**/
	void closeConnection() {
		if (clientSocket != INVALID_SOCKET) {
			closesocket(clientSocket);
			clientSocket = INVALID_SOCKET;
			loggerPtr->info("Closed connection with client " + clientIP);
			clientIP = "";
		}
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
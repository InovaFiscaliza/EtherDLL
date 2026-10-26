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
#include "EtherDLLConfig.hpp"
#include "EtherDLLLog.hpp"
#include "EtherDLLUtils.hpp"

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
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <limits>
#include <utility>
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

namespace edll::detail {

// Send a complete byte range, continuing after short positive writes.
// The callbacks keep this helper testable without relying on OS scheduling.
template<typename SendFunction, typename LastErrorFunction>
bool sendAllBytes(const char* data, size_t length, SendFunction&& sendFunction,
	LastErrorFunction&& lastErrorFunction, size_t& bytesSent, int& errorCode) {
	bytesSent = 0;
	errorCode = 0;

	while (bytesSent < length) {
		const size_t remaining = length - bytesSent;
		const size_t maxChunk = static_cast<size_t>((std::numeric_limits<int>::max)());
		const int chunkLength = static_cast<int>(remaining < maxChunk ? remaining : maxChunk);
		const int result = sendFunction(data + bytesSent, chunkLength);

		if (result == SOCKET_ERROR) {
			errorCode = lastErrorFunction();
			if (errorCode == WSAEINTR) {
				continue;
			}
			return false;
		}
		if (result <= 0 || result > chunkLength) {
			errorCode = result == 0 ? WSAECONNRESET : WSAEINVAL;
			return false;
		}

		bytesSent += static_cast<size_t>(result);
	}

	return true;
}

} // namespace edll::detail


// ----------------------------------------------------------------------
/** @brief Thread-safe message queue object
* 
* Provides methods to handle messages from the queue in a thread-safe manner.
**/
class MessageQueue {
private:
	struct QueuedMessage {
		std::uint64_t sequence;
		json item;
	};

	// Internal queue to hold messages and their private, monotonic sequence.
	std::queue<QueuedMessage> msgQueue;
	// Mutex to protect access to the queue
	mutable std::mutex mtx;
	// Total number of messages ever added to the queue. May return to zero if it overflows.
	unsigned long messageCount = 0;
	// Sequence numbers are independent of the externally visible queue ID.
	std::uint64_t nextSequence = 0;
	std::uint64_t lastPoppedSequence = 0;
	// Condition variable to signal addition of messages
	std::condition_variable push_condition;
	// Flag to indicate that a message was pushed
	bool messagePushed = false;
	// Condition variable to signal removal of messages
	std::condition_variable pop_condition;

	// Enqueue a message and return both its public queue ID and private sequence.
	std::pair<unsigned long, std::uint64_t> pushInternal(
		json item, const std::string& logSource, bool setClientKey) {

		std::lock_guard<std::mutex> lock(mtx);
		const unsigned long queueId = messageCount++;
		const std::uint64_t sequence = ++nextSequence;

		item[taskKeys::QueueId::VALUE] = queueId;
		if (setClientKey) {
			item[taskKeys::ClientId::VALUE] = queueId;
		}
		msgQueue.push(QueuedMessage{ sequence, std::move(item) });
		messagePushed = true;

		push_condition.notify_all();
		loggerPtr->debug(logSource + " pushed item to queue. New size: {}", msgQueue.size());

		return { queueId, sequence };
	}

	// Remove the front item while the caller holds mtx.
	json popLocked(const std::string& logSource) {
		if (msgQueue.empty()) {
			return json{};
		}
		QueuedMessage queuedMessage = std::move(msgQueue.front());
		msgQueue.pop();
		lastPoppedSequence = queuedMessage.sequence;
		pop_condition.notify_all();

		loggerPtr->debug(logSource + " popped item from queue. New size: {}", msgQueue.size());
		return std::move(queuedMessage.item);
	}



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
		return pushInternal(std::move(item), logSource, setClientKey).first;
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
	unsigned long pushAndWait(json item, std::string logSource,
		const std::atomic<edll::INT_CODE>& interruptionCode, bool setClientKey = false,
		const std::atomic<bool>* sessionStop = nullptr) {

		const auto result = pushInternal(std::move(item), logSource, setClientKey);

		// Wait until this specific item is popped or an interruption is signaled.

		{
			std::unique_lock<std::mutex> lock(mtx);
			pop_condition.wait(lock, [this, sequence = result.second, &interruptionCode, sessionStop] {
				return lastPoppedSequence >= sequence || interruptionCode != edll::Code::RUNNING ||
					(sessionStop && sessionStop->load(std::memory_order_acquire));
				});
		}
		return result.first;
	
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
		return popLocked(logSource);
	}

	/** @brief Wait for and pop an item from the front of the queue in a thread-safe manner
	* Blocks until an item is available or interruption is signaled.
	*
	* @param interruptionCode: Reference to interruption signal to check for shutdown
	* @param logSource: Message to log upon popping the item
	* @return json: Item popped from the front of the queue, or empty json if interrupted
	* @throws NO EXCEPTION HANDLING
	**/
	json waitAndPop(const std::atomic<edll::INT_CODE>& interruptionCode, std::string logSource,
		const std::atomic<bool>* sessionStop = nullptr) {

		std::unique_lock<std::mutex> lock(mtx);

		loggerPtr->debug(logSource + " waiting for item in message queue");

		// Wait until queue is not empty or we're interrupted.
		push_condition.wait(lock, [this, &interruptionCode, sessionStop] {
			return !msgQueue.empty() || interruptionCode != edll::Code::RUNNING ||
				(sessionStop && sessionStop->load(std::memory_order_acquire));
			});

		// Preserve the existing policy: interruption takes precedence over queued items.
		if (interruptionCode != edll::Code::RUNNING ||
			(sessionStop && sessionStop->load(std::memory_order_acquire))) {
			return json{};
		}

		// Keep the lock from the availability check through removal so another
		// consumer cannot take the item between the wait and the pop.
		return popLocked(logSource);
	}

	/** @brief Hold the thread execution waiting for any action in the queue, interruption or timeout
	 *
	 * @param interruptionCode: Reference to interruption signal to check for shutdown
	 * @param timeoutMs: Maximum time to wait in milliseconds (default 1000 ms)
	 * @return bool: True if the queue changed or , false otherwise
	 * @throws NO EXCEPTION HANDLING
	**/
	bool waitAction(const std::atomic<edll::INT_CODE>& interruptionCode, std::string logSource,
		int timeoutMs = 1000, const std::atomic<bool>* sessionStop = nullptr) {

		std::unique_lock<std::mutex> lock(mtx);

		loggerPtr->debug(logSource + " waiting for item in message queue");

		// Wake promptly when a push occurs or the service is interrupted.
		push_condition.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this, &interruptionCode, sessionStop] {
			return messagePushed || interruptionCode != edll::Code::RUNNING ||
				(sessionStop && sessionStop->load(std::memory_order_acquire));
			});

		if (messagePushed) {
			messagePushed = false;
			return interruptionCode == edll::Code::RUNNING &&
				!(sessionStop && sessionStop->load(std::memory_order_acquire));
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

	/** @brief Notify any threads waiting for messages or for pop signaling
	 * Used to wake up threads when an interruption signal is received.
	 * @param None
	 * @return void
	 * @throws NO EXCEPTION HANDLING
	**/
	void notify() {
		push_condition.notify_all();
		pop_condition.notify_all();
	}

};

// Own all mutable resources and cancellation/completion state for one client.
struct ClientSession {
	MessageQueue request;
	MessageQueue response;
	std::atomic<bool> stopRequested{ false };
	std::atomic<bool> measurementInProgress{ false };
	std::atomic<bool> measurementStopRequested{ false };
	std::atomic<bool> measurementWorkerRunning{ false };
	std::mutex measurementWorkerMutex;
	std::thread measurementWorker;
	std::mutex completionMutex;
	std::condition_variable completionCondition;
	std::atomic<bool> workerCompleted{ false };

	void requestStop() noexcept {
		stopRequested.store(true, std::memory_order_release);
		measurementStopRequested.store(true, std::memory_order_release);
		request.notify();
		response.notify();
	}

	void requestMeasurementStop() noexcept {
		measurementStopRequested.store(true, std::memory_order_release);
	}

	bool startMeasurementWorker(std::function<void()> task) {
		std::lock_guard<std::mutex> lock(measurementWorkerMutex);
		if (measurementWorkerRunning.load(std::memory_order_acquire)) {
			return false;
		}
		if (measurementWorker.joinable()) {
			measurementWorker.join();
		}

		measurementStopRequested.store(false, std::memory_order_release);
		measurementWorkerRunning.store(true, std::memory_order_release);
		try {
			measurementWorker = std::thread([this, task = std::move(task)]() mutable {
				try {
					task();
				}
				catch (const std::exception& e) {
					try { loggerPtr->error("Measurement worker failed: {}", e.what()); }
					catch (...) {}
				}
				catch (...) {
					try { loggerPtr->error("Measurement worker failed with an unknown exception"); }
					catch (...) {}
				}
				measurementWorkerRunning.store(false, std::memory_order_release);
			});
		}
		catch (...) {
			measurementWorkerRunning.store(false, std::memory_order_release);
			throw;
		}
		return true;
	}

	void joinMeasurementWorker() {
		std::lock_guard<std::mutex> lock(measurementWorkerMutex);
		if (measurementWorker.joinable()) {
			measurementWorker.join();
		}
	}

	void markWorkerCompleted() noexcept {
		workerCompleted.store(true, std::memory_order_release);
		completionCondition.notify_all();
	}

	~ClientSession() {
		requestStop();
		try {
			joinMeasurementWorker();
		}
		catch (...) {
		}
	}
};

// Guarantees that every worker exit, including stack unwinding, requests
// session cancellation and wakes the session coordinator.
class SessionWorkerCompletionGuard {
private:
	ClientSession& session;

public:
	explicit SessionWorkerCompletionGuard(ClientSession& session) noexcept
		: session(session) {}

	~SessionWorkerCompletionGuard() noexcept {
		session.requestStop();
		session.markWorkerCompleted();
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
	std::atomic<edll::INT_CODE>& interruptionCode;
	std::atomic<bool>& sessionStop;
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
	json buildServiceInterruptionMsg(const std::atomic<edll::INT_CODE>& interruptionCode) {
		json msg;

		msg[taskKeys::CommandCode::VALUE] = taskKeys::CommandCode::INIT_VALUE;
		msg[taskKeys::CommandName::VALUE] = taskKeys::CommandName::INIT_VALUE;
		msg[taskKeys::Arguments::VALUE] = json::object();
		msg[taskKeys::Message::VALUE] = std::string("Service interruption. Code: ") + std::to_string(interruptionCode.load());

		return msg;
	}

	// Parse and dispatch one complete delimiter-terminated protocol frame.
	void processClientFrame(const std::string& frame, MessageQueue& request,
		MessageQueue& response, const std::string& logSource) {
		json jsonObj = json::parse(frame, nullptr, false);
		if (jsonObj.is_discarded() || !jsonObj.is_object()) {
			loggerPtr->warn(logSource + " received an invalid JSON frame of {} bytes", frame.size());
			queueFrameNack(frame.size(), response, logSource);
			return;
		}

		// Add client ID and queue ID to the request object.
		jsonObj[taskKeys::ClientIp::VALUE] = clientIP;
		if (jsonObj.contains(idStr)) {
			jsonObj[taskKeys::QueueId::VALUE] = request.push(jsonObj, logSource, false);
		}
		else {
			jsonObj[idStr] = request.push(jsonObj, logSource, true);
			jsonObj[taskKeys::QueueId::VALUE] = jsonObj[idStr];
		}

		loggerPtr->debug(logSource + " received message from client: " + jsonObj.dump());

		json ackObj;
		ackObj[service::Msg::Ack::VALUE] = jsonObj[idStr];
		response.push(ackObj, logSource, true);
	}

	void queueFrameNack(size_t frameSize, MessageQueue& response, const std::string& logSource) {
		json nackObj;
		nackObj[service::Msg::Nack::VALUE] = std::to_string(frameSize);
		response.push(nackObj, logSource, true);
	}

	// Return the longest suffix that may be the start of a delimiter split
	// across socket reads.
	size_t delimiterPrefixSuffixLength(const std::string& data) const {
		if (msgEndStr.size() <= 1) {
			return 0;
		}

		size_t candidateLength = data.size() < msgEndStr.size() - 1
			? data.size()
			: msgEndStr.size() - 1;
		while (candidateLength > 0) {
			if (data.compare(data.size() - candidateLength, candidateLength,
				msgEndStr, 0, candidateLength) == 0) {
				return candidateLength;
			}
			--candidateLength;
		}
		return 0;
	}

	void addDiscardedFrameBytes(size_t& totalBytes, size_t additionalBytes) const {
		const size_t maxSize = (std::numeric_limits<size_t>::max)();
		totalBytes = additionalBytes > maxSize - totalBytes
			? maxSize
			: totalBytes + additionalBytes;
	}

	void processAccumulatedFrames(std::string& accumulatedData, bool& discardingOversizedFrame,
		size_t& discardedFrameBytes, MessageQueue& request, MessageQueue& response,
		const std::string& logSource) {
		while (true) {
			if (discardingOversizedFrame) {
				const size_t delimiterIndex = accumulatedData.find(msgEndStr);
				if (delimiterIndex == std::string::npos) {
					const size_t suffixLength = delimiterPrefixSuffixLength(accumulatedData);
					addDiscardedFrameBytes(discardedFrameBytes,
						accumulatedData.size() - suffixLength);
					if (suffixLength < accumulatedData.size()) {
						accumulatedData.erase(0, accumulatedData.size() - suffixLength);
					}
					return;
				}

				addDiscardedFrameBytes(discardedFrameBytes, delimiterIndex);
				loggerPtr->warn(logSource + " rejected oversized frame of {} bytes", discardedFrameBytes);
				queueFrameNack(discardedFrameBytes, response, logSource);
				accumulatedData.erase(0, delimiterIndex + msgEndStr.size());
				discardingOversizedFrame = false;
				discardedFrameBytes = 0;
			}

			const size_t delimiterIndex = accumulatedData.find(msgEndStr);
			if (delimiterIndex != std::string::npos) {
				if (delimiterIndex > edll::MAX_FRAME_SIZE_BYTES) {
					loggerPtr->warn(logSource + " rejected oversized frame of {} bytes", delimiterIndex);
					queueFrameNack(delimiterIndex, response, logSource);
				}
				else {
					const std::string frame = accumulatedData.substr(0, delimiterIndex);
					processClientFrame(frame, request, response, logSource);
				}

				accumulatedData.erase(0, delimiterIndex + msgEndStr.size());
				continue;
			}

			const size_t suffixLength = delimiterPrefixSuffixLength(accumulatedData);
			const size_t frameBytes = accumulatedData.size() - suffixLength;
			if (frameBytes > edll::MAX_FRAME_SIZE_BYTES) {
				loggerPtr->warn(logSource + " rejected oversized frame exceeding {} bytes",
					edll::MAX_FRAME_SIZE_BYTES);
				discardingOversizedFrame = true;
				discardedFrameBytes = frameBytes;
				if (suffixLength < accumulatedData.size()) {
					accumulatedData.erase(0, accumulatedData.size() - suffixLength);
				}
			}
			return;
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

		// Non-blocking accept wait loop
		fd_set readfds;
		struct timeval tv;
		while (interruptionCode == edll::Code::RUNNING) {
			FD_ZERO(&readfds);
			FD_SET(listenSocket, &readfds);
			tv.tv_sec = 0;
			tv.tv_usec = 500000; // 500ms timeout

			iResult = select(static_cast<int>(listenSocket + 1), &readfds, NULL, NULL, &tv);

			if (iResult > 0) {
				// hold the excecution until a client connects
				clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, NULL);
				break;
			}
			else if (iResult == SOCKET_ERROR) {
				loggerPtr->error("Socket select failed. EC:" + std::to_string(WSAGetLastError()));
				break;
			}
			// if iResult == 0, it timed out, so just loop and check interruptionCode
		}

		closesocket(listenSocket);

		if (clientSocket == INVALID_SOCKET) {
			if (interruptionCode == edll::Code::RUNNING) {
				loggerPtr->warn("Failed in accept operation or select timeout. EC:" + std::to_string(WSAGetLastError()));
			}
			return;
		}

		// configure socket for non-blocking operations
		// u_long mode = 1;
		// ioctlsocket(clientSocket, FIONBIO, &mode);

		int addrLen = sizeof(clientAddr);
		if (getpeername(clientSocket, (struct sockaddr*)&clientAddr, &addrLen) == 0) {
			char ipStringBuffer[INET_ADDRSTRLEN];
			if (inet_ntop(AF_INET, &clientAddr.sin_addr, ipStringBuffer, INET_ADDRSTRLEN) != nullptr) {
				clientIP = std::string(ipStringBuffer);
				loggerPtr->debug("Waiting message from client IP: " + clientIP);
			} else {
				clientIP = taskKeys::ClientIp::INIT_VALUE;
				loggerPtr->debug("Error converting IP address using inet_ntop. EC:" + std::to_string(WSAGetLastError()));
			}

		}

		loggerPtr->info("Accepted connection from " + clientIP);
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
		std::atomic<edll::INT_CODE>& interruptionCode,
		std::atomic<bool>& sessionStop, spdlog::logger& logger)
		: config(config),
		interruptionCode(interruptionCode), sessionStop(sessionStop), logger(logger), clientIP("")
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
		buffer.resize(bufferSize);
		std::string accumulatedData = "";
		bool discardingOversizedFrame = false;
		size_t discardedFrameBytes = 0;

		int iResult = 0;

		loggerPtr->debug(logSource + " is waiting for messages from " + clientIP);

		while (interruptionCode == edll::Code::RUNNING &&
			!sessionStop.load(std::memory_order_acquire)) {

			fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(clientSocket, &readfds);
			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 500000; // 500ms timeout

			iResult = select(static_cast<int>(clientSocket + 1), &readfds, NULL, NULL, &tv);

			if (iResult < 0) {
				loggerPtr->error(logSource + " select failed. EC:" + std::to_string(WSAGetLastError()));
				break;
			}
			if (iResult == 0) {
				loggerPtr->debug(logSource + " select timeout, no data available");
				continue;
			}

			// read data from socket - blocking call (but select told us data is ready)
			int bytesRead = recv(clientSocket, buffer.data(), static_cast<int>(bufferSize), 0);

			if (bytesRead > 0) {
				accumulatedData.append(buffer.data(), static_cast<size_t>(bytesRead));
				processAccumulatedFrames(accumulatedData, discardingOversizedFrame, discardedFrameBytes,
					request, response, logSource);
			}
			else if (bytesRead == 0) {
				// Connection closed by client
				loggerPtr->info(logSource + " Client disconnected.");
				return;
			}
			else {
				int error = WSAGetLastError();
				if (error != WSAEWOULDBLOCK && error != WSAETIMEDOUT) {
					loggerPtr->error(logSource + " recv failed. EC:" + std::to_string(error));
					return;
				}
			}
		}

		if (interruptionCode != edll::Code::RUNNING) {
			response.push(buildServiceInterruptionMsg(interruptionCode), logSource, true);
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

		while (interruptionCode == edll::Code::RUNNING &&
			!sessionStop.load(std::memory_order_acquire))
		{
			loggerPtr->debug("DLLResponseToClient: waiting for response");
			json oneResponse = response.waitAndPop(interruptionCode, logSource, &sessionStop);
			loggerPtr->debug("DLLResponseToClient: got response");

			if (interruptionCode != edll::Code::RUNNING ||
				sessionStop.load(std::memory_order_acquire)) {
				loggerPtr->debug("DLLResponseToClient: cancellation detected, breaking");
				break;
			}

			loggerPtr->debug("DLLResponseToClient: preparing message");
			std::string message = oneResponse.dump() + msgEndStr;
			loggerPtr->debug("DLLResponseToClient: sending message: {}", message);

			size_t bytesSent = 0;
			int errorCode = 0;
			const bool sentCompletely = edll::detail::sendAllBytes(
				message.data(), message.size(),
				[this](const char* data, int length) {
					return send(clientSocket, data, length, 0);
				},
				[] { return WSAGetLastError(); },
				bytesSent, errorCode);
			if (!sentCompletely) {
				loggerPtr->error(logSource + " failed after sending {}/{} bytes. EC:{}",
					bytesSent, message.size(), errorCode);
				// A partial frame cannot be repaired by sending the next queued
				// message on the same stream. End this sender and its session.
				return;
			}

			loggerPtr->debug(logSource + " sent message to client: " + message);

			// Reset the idle timer only after the complete frame was sent.
			lastClientMsgTime = std::chrono::steady_clock::now();
		}
		loggerPtr->debug("DLLResponseToClient: exiting loop");
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

		int pingPeriodMs = config[service::KEY][service::PingPeriod::KEY].get<int>() * 1000;

		while (interruptionCode == edll::Code::RUNNING &&
			!sessionStop.load(std::memory_order_acquire)) {

			const bool queueChanged = response.waitAction(interruptionCode, logSource,
				pingPeriodMs, &sessionStop);
			if (interruptionCode != edll::Code::RUNNING ||
				sessionStop.load(std::memory_order_acquire)) {
				break;
			}

			// A queue change means normal traffic is active; only generate
			// periodic output after the wait expires without a new push.
			if (!queueChanged) {
				if (config[service::KEY][service::DemoMode::KEY].get<bool>()) {
					response.pushAndWait(buildDemoData(), logSource, interruptionCode, true, &sessionStop);
				}
				else if (pingEnable) {
					auto now = std::chrono::steady_clock::now();
					std::chrono::milliseconds timeSinceLastMsg = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClientMsgTime);
					if (timeSinceLastMsg.count() > pingPeriodMs) {
						json pingObj;
						pingObj[service::Msg::Ping::VALUE] = now.time_since_epoch().count();
						response.pushAndWait(pingObj, logSource, interruptionCode, true, &sessionStop);
					}
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

	// Wake any socket operation blocked in this client session. The socket is
	// closed by the owner only after all session workers have joined.
	void shutdownConnection() {
		if (clientSocket != INVALID_SOCKET) {
			if (shutdown(clientSocket, SD_BOTH) == SOCKET_ERROR) {
				const int error = WSAGetLastError();
				if (error != WSAENOTCONN && error != WSAEINVAL) {
					loggerPtr->debug("Socket shutdown returned EC:{}", error);
				}
			}
		}
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

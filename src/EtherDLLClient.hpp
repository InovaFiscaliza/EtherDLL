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
* Provides methods to push and pop messages from the queue in a thread-safe manner.
**/
struct messageQueue {

	// vector containing messages in the queue
	std::queue<std::string> msgQueue;
	// mutex for thread safety
	mutable std::mutex mtx;

	/** @brief Push an item to the queue in a thread-safe manner
	 *
	 * @param item: Item to be pushed to the queue
	 * @return void
	 * @throws NO EXCEPTION HANDLING
	**/
	void push(std::string& item) {
		std::lock_guard<std::mutex> lock(mtx);
		msgQueue.push(item)
	}

	/** @brief Pop an item from the front of the queue in a thread-safe manner
	 * Add message termination sequence to the message upon popping.
	 * 
	 * @param None
	 * @return std::string: Item popped from the front of the queue, or empty string if the queue is empty
	 * @throws NO EXCEPTION HANDLING
	 **/
	std::string pop(void) {
		std::lock_guard<std::mutex> lock(mtx); 
		if (msgQueue.empty()) return "";
		std::string item = msgQueue.front() + edll::Form::MSG_END;
		msgQueue.pop();
		return item;
	}
};

// Function prototypes
void clientRequestToDLL(SOCKET clientSocket, json config, messageQueue& request, edll::INT_CODE& interruptionCode, spdlog::logger& log);
void DLLResponseToClient(SOCKET clientSocket, json config, messageQueue& response, edll::INT_CODE& interruptionCode, spdlog::logger& log);
SOCKET establishClientCommunication(json config, edll::INT_CODE& interruptionCode, spdlog::logger& log);

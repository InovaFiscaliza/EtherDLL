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

// Function prototypes
void clientRequestToDLL(SOCKET clientSocket, std::atomic<edll::Code>& interruptionCode, spdlog::logger& log);
void DLLResponseToClient(SOCKET clientSocket, std::atomic<edll::Code>& interruptionCode, spdlog::logger& log);
SOCKET establishClientCommunication(int port, int timeout, std::atomic<edll::Code>& interruptionCode, spdlog::logger& log);

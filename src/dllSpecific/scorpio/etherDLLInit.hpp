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
* * @note Requires C++11 or later
* * @note Uses nlohmann/json library for JSON handling
*
**/


// ----------------------------------------------------------------------
/**
 * Conditional library linking for ScorpioAPI based on build configuration
**/
#ifdef _X86_
	#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE/DEBUG 32Bits
#else
	#ifdef NDEBUG
		#pragma comment (lib, "ScorpioAPIDll.lib") //RELEASE 64Bits
	#else
		#pragma comment (lib, "ScorpioAPIDlld.lib") //DEBUG 64Bits
	#endif
#endif

// ----------------------------------------------------------------------
#pragma once

// Include provided DLL libraries
#include <ScorpioAPITypes.h>

// Include DLL specific libraries

// Include core EtherDLL libraries

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <string>

// For convenience
using json = nlohmann::json;

// ----------------------------------------------------------------------
// 
/** @brief Alias to be used by core EtherDLL code to pass connection parameters
 * 
 * \! Name must not be changed \n
 *    May be defined as an alias to any DLL specific structure or to other data type
 *    May also be defined as a structure containing multiple parameters
 *    A default value must be defined as DEFAULT_DLL_CONNECTION_DATA for initialization purposes
**/
using DLLConnectionData = unsigned long;

const DLLConnectionData DEFAULT_DLL_CONNECTION_DATA = 0;

// Function prototypes
void newDefaultConfigFile(const std::string& filename);
bool connectAPI(DLLConnectionData& stationConnID, const nlohmann::json& config, spdlog::logger& logger);
bool disconnectAPI(DLLConnectionData& stationConnID, spdlog::logger& logger);

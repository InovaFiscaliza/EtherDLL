// MIAerConn.cpp : Defines the entry point for the application.
//

// Include the standard C++ headers
#include <iostream>
#include <fstream>

// Include the nlohmann JSON library
#include <nlohmann/json.hpp>

// Include the spdlog library
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// Include the EasySocket library
#include <masesk/EasySocket.hpp>

// Include the ScorpioAPI libraries
#include <StdAfx.h>
#include <ScorpioAPIDll.h>

// Include to solution specific libraries
#include <messages.h>

// For convenience handling JSON data
using json = nlohmann::json;

// Global variables related to the application
json config;
spdlog::logger logger = spdlog::logger("MIAerConn");

// Global variables related to the API
unsigned long APIserverId = 0; // Declare APIserverId. This service is intended to be used to connect to a single station, always 0.
SScorpioAPIClient params; // Declare and initialize params
ErrorCB OnErrFunc;
DataCB OnDataFunc;
RealTimeDataCB OnRealtimeDataFunc;
ERetCode errCode;

//
// FUNCTION: stringToWString(const std::string& str)
//
// PURPOSE: Convert a string to a wide string.
//
//
std::wstring stringToWString(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}


//
//  FUNCTION: StationConnect(void)
//
//  PURPOSE: Create a connection object to the station and connect to it.
//
//
void StationConnect(bool* service_error)
{
	// Error code using API ERetCode enum
	ERetCode errCode;

	// Error message string to be used in the logger
	std::string message;

	// Create a local copy of APIserverId. This is necessary because TCI methods update the APIserverId value to the next available ID.
	unsigned long NextServerId = APIserverId;

	// Hostname as simple string, extracted from th JSON configuration file
	std::string hostNameStr = config["station address"].get<std::string>();
	params.hostName = stringToWString(hostNameStr);

	// Port as simple string, extracted from the JSON configuration file
	std::string portStr = config["station port"].get<std::string>();
	params.port = stringToWString(portStr);

	// Timeout as unsigned long, extracted from the JSON configuration file
	params.sendTimeout = config["station timeout"].get<unsigned long>();

	// Create the connection object
	errCode = ScorpioAPICreate(
		NextServerId,
		params,
		OnErrFunc,
		OnDataFunc,
		OnRealtimeDataFunc
	);

	// Handle the error code from object creation
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Object associated with station not created: " + ERetCodeToString(errCode);
		logger.error(message);
		*service_error = true;
	}
	else
	{
		logger.info("Object creation successful");
	}

	NextServerId = APIserverId;

	// Actually connect to the station
	errCode = Connect(
		NextServerId,
		params,
		OnErrFunc,
		OnDataFunc,
		OnRealtimeDataFunc
	);

	// Handle the error code from station connection
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Connection with " + hostNameStr + " not stablished: " + ERetCodeToString(errCode);
		logger.error(message);
		*service_error = true;
	}
	else
	{
		message = "Connection with " + hostNameStr + " stablished";
		logger.info(message);
	}
}

//
//  FUNCTION: DisconnectAll(void)
//
//  PURPOSE: Disconnect station and socket clients
//
//
void DisconnectAll(bool* service_error)
{
	ERetCode errCode;
	std::string message;

	// Create a local copy of APIserverId. This is necessary because TCI methods update the APIserverID value to the next available ID.
	unsigned long NextServerId = APIserverId;

	errCode = Disconnect(NextServerId);
	
	if (errCode != ERetCode::API_SUCCESS)
	{
		message = "Error disconnecting from station " + ERetCodeToString(errCode);
		logger.error(message);
		*service_error = true;
	}
	else
	{
		logger.info("Disconnected from station");
	}

}

int main() {

	bool service_error = false;
	bool system_kill_signal = true; // ! Implement function to handle system and users kill signal

	// Read configuration from JSON file
	std::ifstream input_file("config.json");
	input_file >> config;

	// Create a logger sink to write to the console
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);

	// Get the log filename from the configuration file
	std::string log_filename = std::string(config["log file"].get<std::string>());

	// Create a logger sink that writes to a file
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename, true);
	file_sink->set_level(spdlog::level::trace);

	// Create a logger object with the console and file sinks
	logger.sinks().clear();
	logger.sinks().push_back(console_sink);
	logger.sinks().push_back(file_sink);
	//logger.set_level(spdlog::level::debug);

	logger.info("Application started");
	// logger.warn("");
	// logger.error("");

	StationConnect(&service_error);

	// while not service_error and not system kill signal
	while (!service_error && !system_kill_signal) {
		// wait for interrupt signal from console or system kill
		// if signal received, set service_error to true
	}

	if (system_kill_signal) {
		logger.warn("Kill signal received. Stopping service.");
	}
	else {
		logger.error("Stopping service due to the reported error");
	}
	// Close the connection
	DisconnectAll(&service_error);

	// Final flush before the application exits, save log to file.
	file_sink->flush();

	if (service_error) {
			return 1003;
		}
		else {
			return 0;
		}
}

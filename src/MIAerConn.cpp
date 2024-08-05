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
bool ErrorStatus = false; // Declare ErrorStatus
json config;
spdlog::logger logger = spdlog::logger("MIAerConn");

// Global variables related to the API
unsigned long serverId; // Declare serverId
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
//  FUNCTION: StartConnection(void)
//
//  PURPOSE: Create a connection object.
//
//
ERetCode StationConnect(void)
{

	// Set connection parameters from the configuration file
	std::string hostNameStr = config["station address"].get<std::string>();
	params.hostName = stringToWString(hostNameStr);

	std::string portStr = config["station port"].get<std::string>();
	params.port = stringToWString(portStr);

	params.sendTimeout = config["station timeout"].get<unsigned long>();

	// Create the connection object
	return ScorpioAPICreate(
		serverId,
		params,
		OnErrFunc,
		OnDataFunc,
		OnRealtimeDataFunc
	);
}

int main() {

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
	logger.sinks().push_back(console_sink);
	logger.sinks().push_back(file_sink);

	//logger.set_level(spdlog::level::debug);

	logger.info("Application started");
	// logger.warn("");
	// logger.error("");

    errCode = StationConnect();
    if (errCode != ERetCode::API_SUCCESS)
    {
		logger.error("Connection Error: %s",ERetCodeToString(errCode));
		ErrorStatus = true;
	}
	else
	{
		logger.info("Connection successful");
    } 

// Final flush before the application exits, save log to file.
	file_sink->flush();

	if (ErrorStatus) {
			return 1;
		}
		else {
			return 0;
		}
}

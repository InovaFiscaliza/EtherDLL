#ifndef EtherDLLLog_H
#define EtherDLLLog_H

#include "stdafx.h"
#include <ExternalCodes.h>
// Include the spdlog library
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

extern std::vector<std::string> errorBuffer;

#pragma once

class EtherDLLLog
{
	public:
		void start(std::string name, bool consoleEnable, bool fileEnable, std::string consoleLevel, std::string logFilename, std::string fileLevel);
		void logCommandExec(ERetCode errCode, std::string command);
		void info(std::string str);
		void warn(std::string str);
		void error(std::string str);
		~EtherDLLLog();

	private:
		std::string logName;
		//spdlog::logger logger;
};

#endif
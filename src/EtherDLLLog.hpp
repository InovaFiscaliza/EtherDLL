/**
 * @file EtherDLLLog.cpp
 * @brief
 *
 * This header file contains function declarations for converting JSON objects
 * to various Scorpio API data structures used in the DLL interface.
 *
 * @author fslobao
 * @date 2025-09-10
 * @version 1.0
 *
 * @note Requires C++14 or later
 * @note Uses nlohmann/json library for JSON parsing
 *
 * Dependencies:
 * - nlohmann/json.hpp
 * - spdlog/spdlog.h
 *
 **/
// ----------------------------------------------------------------------
#pragma once

// Include core EtherDLL libraries
#include "EtherDLLConfig.hpp"

// Include project libraries
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Include general C++ libraries
#include <string>

// For convenience
using json = nlohmann::json;

// ----------------------------------------------------------------------
/**
 * @brief Initialize the logger based on the provided JSON configuration.
 *
 * @param config: JSON object containing valid logging configuration
 * @return std::shared_ptr<spdlog::logger>: Shared pointer to the initialized logger
 * @throws std::invalid_argument if the configuration is invalid
**/
void initializeLog(json config, spdlog::logger& logger) {
    json log_config = config["log"].get<json>();

    logger.sinks().clear();

    // Variable to track the most verbose level needed
    spdlog::level::level_enum most_verbose_level = spdlog::level::off;

    // add the console sink to the logger
    if (log_config.contains("console")) {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        char console_level = log_config["console"]["level"].get<std::string>()[0];
        spdlog::level::level_enum sink_level;
        switch (console_level) {
        case 't':
            sink_level = spdlog::level::trace;
            break;
        case 'd':
            sink_level = spdlog::level::debug;
            break;
        case 'i':
            sink_level = spdlog::level::info;
            break;
        case 'w':
            sink_level = spdlog::level::warn;
            break;
        case 'e':
            sink_level = spdlog::level::err;
            break;
        case 'c':
            sink_level = spdlog::level::critical;
            break;
        default:
            throw std::invalid_argument("Invalid console log level. Use trace, debug, info, warning, error or critical.");
            break;
        }
        console_sink->set_level(sink_level);

        // Track the most verbose level
        if (sink_level < most_verbose_level) {
            most_verbose_level = sink_level;
        }

        logger.sinks().push_back(console_sink);
    }

    // Add the file sink to the logger
    if (log_config.contains("file")) {
        std::string filename = log_config["file"]["filename"].get<std::string>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);

        char fileLevel = log_config["file"]["level"].get<std::string>()[0];
        spdlog::level::level_enum sink_level;
        switch (fileLevel) {
        case 't':
            sink_level = spdlog::level::trace;
            break;
        case 'd':
            sink_level = spdlog::level::debug;
            break;
        case 'i':
            sink_level = spdlog::level::info;
            break;
        case 'w':
            sink_level = spdlog::level::warn;
            break;
        case 'e':
            sink_level = spdlog::level::err;
            break;
        case 'c':
            sink_level = spdlog::level::critical;
            break;
        default:
            sink_level = spdlog::level::info;
            break;
        }
        file_sink->set_level(sink_level);

        // Track the most verbose level
        if (sink_level < most_verbose_level) {
            most_verbose_level = sink_level;
        }

        logger.sinks().push_back(file_sink);
    }

    // Set the global log level
    logger.set_level(most_verbose_level);

    // Set flush level - flush on every error or higher level message
    logger.flush_on(spdlog::level::info);

    logger.info("Starting " + log_config["name"].get<std::string>() + "...");

    logger.flush();
}
/**
 * @file EtherDLLUtils.hpp
 * @brief Declarations of utility functions for EtherDLL base program
 *
 * @author fslobao
 * @date 2025-09-12
 * @version 1.0
 *
 * @note Requires C++14 or later
 *
 * Special Dependencies:
 * * - none
 **/

 // ----------------------------------------------------------------------
#pragma once

// Include to DLL specific headers

// Include core EtherDLL libraries

// Include project libraries

// Include general C++ libraries
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// Constants
static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

// Definitions
typedef unsigned char BYTE;

// Function prototypes
std::vector<std::string> split(std::string s, std::string delimiter);
std::wstring stringToWString(const std::string& str);
unsigned long stringToUnsignedLong(std::string str);
bool stringToBool(std::string str);
std::string COleTimeToIsoStr(double oleTime);
std::string base64_encode(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode(std::string const&);
std::string wchartToUtf8String(const wchar_t* wstr, size_t len);

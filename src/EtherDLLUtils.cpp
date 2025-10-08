/**
 * @file EtherDLLUtils.hpp
 * @brief General utility functions used in EtherDLL
 *
 * @author fslobao
 * @date 2025-09-12
 * @version 1.0
 *
 * @note Requires C++14 or later
 * @note Uses nlohmann/json library for JSON parsing
 *
 * Special Dependencies:
 * - nlohmann/json.hpp
 *
 **/

 // ----------------------------------------------------------------------
// Include to DLL specific headers

// Include core EtherDLL libraries
#include "EtherDLLUtils.hpp"

// Include project libraries
#include <nlohmann/json.hpp>

// Include general C++ libraries
#include <ctime>
#include <chrono>
#include <fmt/chrono.h>
#include <cmath>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

// For convenience
using json = nlohmann::json;


// ----------------------------------------------------------------------
/**
 * @brief Split a string into a vector of strings using a specified delimiter
 *
 * @param s: Input string to be split
 * @param delimiter: Delimiter string used to split the input string
 * @return std::vector<std::string>: Vector containing the split substrings
 * @throws NO 
**/
std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

// ----------------------------------------------------------------------
/**
 * @brief Convert std::string to std::wstring
 *
 * @param str: Input string to be converted
 * @return std::wstring: Converted wide string
 * @throws NO EXCEPTION HANDLING
 **/
std::wstring stringToWString(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}

// ----------------------------------------------------------------------
/**
 * @brief Convert std::string to unsigned long
 * 
 * @param str: Input string to be converted
 * @return unsigned long: Converted unsigned long value
 * @throws NO EXCEPTION HANDLING
**/
unsigned long stringToUnsignedLong(std::string str) {
	return strtoul(str.c_str(), NULL, 0);
}

// ----------------------------------------------------------------------
/**
 * @brief Convert std::string to boolean
 * 
 * @param str: Input string to be converted
 * @return bool: Converted boolean value
 * @throws NO EXCEPTION HANDLING
**/
bool stringToBool(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}

// ----------------------------------------------------------------------
/** @brief Convert COleTime to ISO formated string
 *
 * COLeTime is a double representing the number of days since midnight, December 30, 1899.
 * Output is a string in ISO 8601 format: "YYYY-MM-DDTHH:MM:SS.ssssssZ"
 * Time is represented down to microseconds (0.000001 seconds)
 *
 * @param oleTime: COleTime value to be converted
 * @return std::string: ISO 8601 formatted string
 * @throws NO EXCEPTION HANDLING
**/
std::string COleTimeToIsoStr(double oleTime) {
	const double OLE_TIME_EPOCH_DIFF = 25569.0;
	const int SECONDS_PER_DAY = 86400;

	double unixTimeSeconds = (oleTime - OLE_TIME_EPOCH_DIFF) * SECONDS_PER_DAY;

	// Separar parte inteira e fracionária para preservar precisão
	std::time_t seconds = static_cast<std::time_t>(unixTimeSeconds);
	double fractionalPart = unixTimeSeconds - seconds;

	// Converter para chrono::time_point com precisão de microssegundos
	auto tp = std::chrono::system_clock::from_time_t(seconds);
	auto microseconds = std::chrono::microseconds(
		static_cast<long long>(fractionalPart * 1000000)
	);
	tp += microseconds;

	return fmt::format("{:%Y-%m-%dT%H:%M:%S}.{:06d}Z",
		fmt::gmtime(std::chrono::system_clock::to_time_t(tp)),
		microseconds.count() % 1000000);
}


// ----------------------------------------------------------------------
/** @brief Base64 encoding function
 *
 * Functions to encode binary data to Base64 string and decode Base64 string to binary data.
 * Uses standard Base64 character set.
 * Based on public domain code by Jouni Malinen
 *
 * @param buf: Pointer to the input binary data
 * @param bufLen: Length of the input binary data
 * @return std::string: Base64 encoded string
 *
 * @param encoded_string: Input Base64 encoded string
 * @return std::vector<BYTE>: Decoded binary data
 * @throws NO EXCEPTION HANDLING
**/
std::string base64Encode(BYTE const* buf, unsigned int bufLen) {
    std::string ret;
    int i = 0;
    int j = 0;
    BYTE char_array_3[3];
    BYTE char_array_4[4];

    while (bufLen--) {
        char_array_3[i++] = *(buf++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += BASE64_CHARS[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += BASE64_CHARS[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

// ----------------------------------------------------------------------
/** @brief convert wchar_t* to UTF-8 std::string
 *
 * Convert wide character string (wchar_t*) to UTF-8 encoded std::string
 * Uses Windows API WideCharToMultiByte for conversion
 * Necessary for proper handling of Unicode characters in Windows environment
 * when using C++17 standard
 *
 * @param wstr: Pointer to the input wide character string
 * @param len: Length of the input wide character string
 * @return std::string: Converted UTF-8 encoded string
 * @throws NO EXCEPTION HANDLING
**/
std::string wchartToUtf8String(const wchar_t* wstr, size_t len)
{
    if (!wstr || len == 0) return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(len), nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) return std::string();

    std::string outStr(size_needed, 0);

    WideCharToMultiByte(CP_UTF8, 0, wstr, static_cast<int>(len), &outStr[0], size_needed, nullptr, nullptr);

    return outStr;
}

// ------------------------------------------------------
/** @brief Build demo data JSON object
 *
 * @param  none
 * @return JSON object containing demo data
 * @throws NO EXCEPTION HANDLING
 **/
json buildDemoData() {

	json demoData = {
		{"CODE", 55},
		{"SID", 1},
		{"demod", {
			{"audioPower", {
				{"active", false}
				}
			},
			{"nActiveAudioChannels", 0}
			}
		},
		{"measure", {
			{"dateTime", "2025-10-07T08:28:09.575224Z"},
			{"powerDbm", 125},
			{"status", 0}
			}
		},
		{"setting", {
			{"attenuation", 56}
			}
		},
		{"spectrum", {
			{"binSize", 1250.0},
			{"binSizeUnit", "Hz"},
			{"conversionFactorForFS", 0.0},
			{"frequencyUnit", "MHz"},
			{"numBins", 801},
			{"startFrequency", 100.8},
			{"stopFrequency", 101.8},
			{"sweepData", "AACYwgAAlsIAAKbCAACUwgAAjsIAAJzCAACSwgAAiMIAAJTCAACOwgAAhsIAAKDCAACMwgAAjsIAAKLCAACSwgAAkMIAAIrCAACcwgAAgMIAAHjCAACKwgAAeMIAAHDCAAB0wgAAcMIAAGzCAABswgAAdMIAAHjCAABcwgAAYMIAAHzCAABkwgAAXMIAAGzCAABowgAAUMIAAFzCAAB4wgAAVMIAAFzCAACAwgAAXMIAAHDCAACCwgAAhMIAAHzCAABwwgAAgMIAAGTCAABwwgAAhMIAAGDCAABgwgAAgsIAAHzCAAB8wgAAeMIAAHTCAACCwgAAbMIAAFjCAABkwgAAcMIAAFzCAABUwgAAZMIAAHjCAABUwgAAXMIAAFzCAABcwgAAZMIAAFzCAABwwgAAdMIAAILCAACCwgAAaMIAAGTCAACOwgAAYMIAAFTCAABswgAAbMIAAGjCAABgwgAAYMIAAGzCAAB4wgAAWMIAAFTCAABswgAAdMIAAHTCAABOwgAAVMIAAFDCAABkwgAAbMIAAFDCAABMwgAAUMIAAFjCAABOwgAAbMIAAGDCAABcwgAAcMIAAHjCAABcwgAATMIAAETCAABEwgAARMIAAETCAABIwgAASMIAAEjCAABEwgAAQMIAAEjCAABQwgAAWMIAAGTCAABgwgAATMIAAEDCAABAwgAASMIAAEjCAABQwgAAYMIAAHDCAACQwgAAcMIAAGzCAABswgAAcMIAAHDCAABwwgAAdMIAAHDCAABswgAAaMIAAGzCAAB0wgAAdMIAAHDCAAB4wgAAhMIAAIjCAACOwgAAksIAAJTCAACOwgAAjMIAAI7CAACIwgAAhMIAAILCAACEwgAAisIAAJDCAACSwgAAksIAAJbCAACawgAAmsIAAJrCAACcwgAAmsIAAJbCAACawgAAosIAAJ7CAACcwgAAnMIAAKDCAACqwgAApsIAAKLCAACmwgAAxMIAALLCAAC0wgAAtMIAALDCAACqwgAArMIAALTCAAC0wgAArsIAAKrCAACwwgAAtsIAALbCAAC2wgAAtMIAALrCAADKwgAA1MIAAMrCAADAwgAAuMIAALrCAADGwgAA3MIAAMzCAADAwgAAvsIAAMjCAADswgAA0sIAAMTCAADEwgAAxsIAAMjCAADKwgAAxsIAAMrCAADSwgAA2MIAANjCAADYwgAA6sIAANLCAADWwgAA3sIAANTCAADGwgAAwsIAAMrCAADgwgAA2sIAANbCAADSwgAAzsIAANDCAADMwgAAxMIAAMbCAADQwgAAzMIAAMjCAADGwgAAzsIAANrCAADKwgAAxsIAAMTCAADGwgAAyMIAAMrCAADKwgAAzsIAANzCAADUwgAAzsIAAMDCAADCwgAA0MIAAODCAADmwgAA3sIAANjCAADUwgAA1sIAANrCAADYwgAA1sIAANbCAADSwgAA3sIAAMrCAADAwgAAvsIAALzCAAC4wgAAuMIAAMDCAADIwgAAysIAAMzCAADEwgAAwsIAAMzCAADgwgAAzMIAAM7CAADKwgAAysIAAMzCAADEwgAAusIAALrCAADGwgAA1sIAAMzCAADEwgAAxsIAAMzCAADOwgAA1MIAAM7CAADCwgAAwsIAAMzCAADOwgAA0MIAANLCAADIwgAAysIAAMjCAADMwgAA1MIAAMrCAADOwgAAzsIAAMjCAADIwgAAysIAAMbCAADEwgAAxMIAAMTCAADGwgAAyMIAAMzCAADIwgAAxMIAAMjCAADOwgAAzMIAAMzCAADUwgAAzsIAAMrCAADEwgAAysIAANjCAADswgAA0sIAAMzCAADIwgAA0sIAANzCAADWwgAA4MIAANzCAADcwgAAzsIAAMrCAADSwgAAysIAAMTCAADGwgAA3sIAAM7CAADIwgAAysIAAOLCAADOwgAAxsIAAM7CAADOwgAA0MIAANbCAADSwgAA0MIAAMzCAADawgAA7MIAANjCAADCwgAAuMIAALLCAAC2wgAAwsIAANDCAADCwgAAvMIAAL7CAADAwgAAwMIAAL7CAACwgAAwsIAAMrCAADcwgAA8MIAAOTCAADKwgAAwsIAAMTCAADOwgAA0MIAAOrCAACqwgAAjsIAAILCAACEwgAAlMIAALbCAADewgAA5MIAANbCAADEwgAAzsIAAM7CAADEwgAAzMIAANTCAADQwgAA0MIAAPTCAADcwgAA1sIAANbCAADUwgAA0sIAANLCAADSwgAA9sIAAMrCAADCwgAAyMIAAODCAADSwgAAwMIAALzCAADAwgAAyMIAAMjCAADGwgAAysIAANLCAADcwgAA3MIAAOTCAADcwgAAysIAAMbCAADKwgAAysIAAMTCAADEwgAAysIAANjCAADkwgAA6sIAANzCAADYwgAA1sIAAOLCAADWwgAAyMIAAMzCAADUwgAAxsIAAMTCAADSwgAA2sIAAMjCAACwgAAvMIAAMjCAADUwgAAysIAAMjCAADEwgAAxMIAAMrCAADIwgAAxMIAAMTCAADGwgAAzMIAANDCAADOwgAA2MIAAObCAADMwgAAzsIAANjCAADKwgAAzsIAAMzCAADEwgAAvMIAAL7CAADCwgAAxMIAAMTCAADCwgAAxMIAAMjCAADKwgAAxMIAAMDCAACwgAAxMIAAMjCAADAwgAAvMIAAL7CAADGwgAA0sIAANzCAADawgAA4MIAAMzCAADEwgAAxsIAAMrCAADQwgAA2sIAAODCAADewgAA2sIAANDCAADKwgAAxsIAAMzCAADSwgAA7MIAANbCAADKwgAAxMIAAMTCAADKwgAA1MIAANLCAADYwgAA3sIAAADAADYwgAAyMIAAMTCAADOwgAA6MIAANTCAADWwgAA5sIAAMjCAADCwgAAzsIAANrCAADQwgAA0sIAAM7CAADIwgAAyMIAAMzCAADQwgAA1MIAANrCAADewgAA4sIAANbCAADOwgAA0sIAANzCAADSwgAAzsIAANrCAADQwgAAxMIAAMbCAADOwgAA0MIAANLCAADSwgAA4MIAAN7CAADywgAAzMIAAMjCAADQwgAA7sIAAMrCAADGwgAAyMIAAMTCAADCwgAAyMIAAM7CAADKwgAAzMIAANDCAADSwgAAxsIAAMDCAADCwgAAysIAAMjCAADGwgAAxsIAAMLCAADIwgAAyMIAAMbCAADIwgAAzMIAAMzCAADMwgAA3MIAANrCAADawgAAzsIAAMbCAADMwgAA1MIAANLCAADUwgAA0sIAAMzCAADIwgAAyMIAAMzCAADUwgAA0sIAAN7CAADgwgAA2MIAANTCAADQwgAA2MIAAM7CAADOwgAA0MIAANLCAADCwgAAwMIAAMjCAADAwgAAuMIAALbCAAC8wgAA0MIAANbCAADOwgAAysIAAMLCAADAwgAAwsIAAMrCAAC8wgAAuMIAALbCAACwwgAArsIAALLCAADewgAAtMIAAK7CAACowgAAqMIAAKzCAAC0wgAAwMIAALLCAACuwgAAqsIAAKLCAACewgAAoMIAAKLCAACwwgAAnMIAAJbCAACcwgAAqMIAAKDCAACcwgAAnMIAAJ7CAACiwgAAusIAAKrCAACiwgAAnMIAAJzCAACwwgAArMIAAKrCAAC6wgAAusIAAKzCAACuwgAAqsIAAKLCAACmwgAAqsIAAJzCAACawgAAqsIAALbCAACywgAAvMIAAKzCAACkwgAAosIAAKbCAACowgAAsMIAAK7CAACkwgAAoMIAAKTCAAC0wgAArMIAAKjCAACiwgAAmsIAAJjCAACqwgAArsIAAKzCAACowgAAosIAAJ7CAACiwgAApsIAAKTCAACuwgAAssIAAKLCAACgwgAApsIAAK7CAAC2wgAAssIAAKjCAACkwgAApsIAALDCAAC0wgAAtsIAALjCAAC8wgAAuMIAALrCAADCwgAAusIAALLCAACswgAAqsIAAKrCAACswgAAqsIAAKbCAACkwgAApMIAAKjCAACswgAAqsIAAKbCAACmwgAAqMIAAKrCAACswgAArMIAAKrCAACqwgAArMIAALDCAACywgAAtsIAALTCAAC0wgAAtMIAALbCAAC0wgAAtsIAALzCAAC4wgAAtMIAALbCAAC4wgAAtMIAALLCAACywgAAtMIAALTCAAC4wgAAusIAALjCAAC2wgAAtsIAALjCAADAwgAAxsIAAL7CAAC6wgAAvMIAALrCAAC4wgAAusIAAMDC"}
			}
		}
	};

	return demoData;
}
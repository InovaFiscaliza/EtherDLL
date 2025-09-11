#include "EtherDLLUtils.h"
#include <nlohmann/json.hpp>

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

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
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Include general C++ libraries
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

// Constants

// Base64 characters
static const std::string BASE64_CHARS =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

constexpr std::size_t HASH_STRING = 479440892;   // djb2 hash for string "string"
constexpr std::size_t HASH_NUMBER = 284762254;   // djb2 hash for string "number"
constexpr std::size_t HASH_BOOLEAN = 2257391749;  // djb2 hash for string "boolean"
constexpr std::size_t HASH_ARRAY = 695403079;    // djb2 hash for string "array"
constexpr std::size_t HASH_OBJECT = 301260540;  // djb2 hash for string "object"

constexpr const char* VALID_TYPE_STRING = "string";
constexpr const char* VALID_TYPE_NUMBER = "number";
constexpr const char* VALID_TYPE_BOOLEAN = "boolean";
constexpr const char* VALID_TYPE_ARRAY = "array";
constexpr const char* VALID_TYPE_OBJECT = "object";

// For convenience
using json = nlohmann::json;

// Definitions
typedef unsigned char BYTE;

// Global variables
extern spdlog::logger* loggerPtr;

// ----------------------------------------------------------------------

// Compute djb2 hash of string
constexpr std::size_t stringToHash(const char* str, std::size_t hash = 5381) {
    return (*str == 0) ? hash : stringToHash(str + 1, ((hash << 5) + hash) + *str);
}

// Class to validate JSON objects against expected schema
class JsonValidator {
public:
    struct ValidationError {
        std::string path;
        std::string message;

        std::string toString() const {
            return path + ": " + message;
        }
    };

private:
    std::vector<ValidationError> errors;
    std::string currentPath;

    // ----------------------------------------------------------------------
	/** @brief Push a new segment to the current JSON path
     * @param segment The JSON path segment to add
	 * @return void
	 * @throws NO EXCEPTION HANDLING
    **/
    void pushPath(const std::string& segment) {
        if (currentPath.empty()) {
            currentPath = segment;
        }
        else {
            currentPath += "." + segment;
        }
    }

    // ----------------------------------------------------------------------
    /** @brief Pop the last segment from the current JSON path
	 * @param None
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void popPath() {
        auto pos = currentPath.find_last_of('.');
        if (pos != std::string::npos) {
            currentPath = currentPath.substr(0, pos);
        }
        else {
            currentPath.clear();
        }
    }

public:

    // ----------------------------------------------------------------------
    /** @brief Add a validation error with the current JSON path
	 * @param message The error message to add
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void addError(const std::string& message) {
        errors.push_back({ currentPath, message });
    }

    // ----------------------------------------------------------------------
    /** @brief Clear previous validation results
	 * @param None
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void reset() {
        errors.clear();
        currentPath.clear();
    }

    // ----------------------------------------------------------------------
    /** @brief Get validation error vector
     * @param None
     * @return const std::vector<ValidationError>&
     * @throws NO EXCEPTION HANDLING
    **/
    const std::vector<ValidationError>& getErrors() const {
        return errors;
    }

    // ----------------------------------------------------------------------
    /** @brief Check if validation passed
     * @param None
     * @return bool
     * @throws NO EXCEPTION HANDLING
    **/
    bool isValid() const {
        return errors.empty();
    }

    // ----------------------------------------------------------------------
    /** @brief Get formatted error string
     * @param None
     * @return std::string
     * @throws NO EXCEPTION HANDLING
    **/
    std::string getErrorString() const {
        if (errors.empty()) return "";

        std::stringstream ss;
        for (size_t i = 0; i < errors.size(); ++i) {
            if (i > 0) ss << "; ";
            ss << errors[i].toString();
        }
        return ss.str();
    }

    // ----------------------------------------------------------------------
	/** @brief Test if a required field exists in the JSON object
	 * Add a validation error if the field is missing
	 * @param obj The JSON object to validate
	 * @param fieldName The name of the required field
     * @return JsonValidator&
     * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& requireField(const json& obj, const std::string& fieldName) {
        pushPath(fieldName);
        if (!obj.contains(fieldName)) {
            addError ("Field required");
        }
        popPath();
        return *this;
    }

    // ----------------------------------------------------------------------
	/** @brief Test if a field is of the expected type
     * Add a validation error if the field is of the wrong type
     * @param obj The JSON object to validate
     * @param fieldName The name of the field to check
     * @param typeName The expected type of the field
     * @return JsonValidator&
     * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& requireType(const json& obj, const std::string& fieldName, const std::string& typeName) {

		int numErrors = static_cast<int>(errors.size());
		*this = requireField(obj, fieldName);

		if (static_cast<int>(errors.size()) > numErrors) {
			// Field is missing, no need to check type
			return *this;
		}

        pushPath(fieldName);
        
        bool valid = false;
        const auto& field = obj[fieldName];

		size_t hash = stringToHash(typeName.c_str());

		loggerPtr->debug("Validation of field '{}'. Expected <{}>. received <{}>, hash {}", fieldName, typeName, obj[fieldName].type_name(), hash);

        switch (hash) {
            case HASH_STRING: {
                valid = field.is_string();
                break;
            }
            case HASH_NUMBER: {
                valid = field.is_number();
                break;
            }
            case HASH_BOOLEAN: {
                valid = field.is_boolean();
                break;
            }
            case HASH_ARRAY: {
                valid = field.is_array();
                break;
            }
            case HASH_OBJECT: {
                valid = field.is_object();
                break;
            }
            default: {
                valid = false;
                break;
            }
        }

        if (!valid) {
            addError("Type required: '" + typeName + "'");
        }

        popPath();
        return *this;
    }

    
    template<typename T>
    // ----------------------------------------------------------------------
	/** @brief Test if a required numeric field is within a specified range
     * Range limits are included (>= min and <= max)
     * @tparam T The numeric type
     * @param obj The JSON object to validate
     * @param fieldName The name of the field to check
     * @param typeName The expected type of the field
     * @param minValue The minimum allowed value
     * @param maxValue The maximum allowed value
     * @return JsonValidator&
     * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& requireRange(const json& obj, const std::string& fieldName,
        T minValue, T maxValue) {

        int numErrors = static_cast<int>(errors.size());
		*this = requireType(obj, fieldName, VALID_TYPE_NUMBER);

		if (static_cast<int>(errors.size()) > numErrors) {
			return *this;
		}

        pushPath(fieldName);

        T value = obj[fieldName].get<T>();
        if (value < minValue || value > maxValue) {
            addError("Number must be between " + std::to_string(minValue) +
                " and " + std::to_string(maxValue));
        }

        popPath();
        return *this;
    }

    // ----------------------------------------------------------------------
    /** @brief Test if an optional integer field is within a specified range
     * @param obj The JSON object to validate
     * @param fieldName The name of the field to check
     * @param minValue The minimum allowed value
     * @param maxValue The maximum allowed value
     * @return JsonValidator&
	 * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& optionalRange(const json& obj, const std::string& fieldName,
        int minValue, int maxValue) {
        if (obj.contains(fieldName) && !obj[fieldName].is_null()) {
            return requireRange(obj, fieldName, minValue, maxValue);
        }
		return *this;
    }

	// ----------------------------------------------------------------------
    /** @brief Test if a required array field exists and meets minimum size
     * @param obj The JSON object to validate
     * @param fieldName The name of the array field to check
     * @param minItems The minimum number of items required in the array
     * @return JsonValidator&
	 * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& requireArray(const json& obj, const std::string& fieldName, const std::string& typeName, size_t minItems = 1) {

        int numErrors = static_cast<int>(errors.size());
        *this = requireField(obj, fieldName);
        if (static_cast<int>(errors.size()) > numErrors) {
            return *this;
        }

        pushPath(fieldName);

        if (obj[fieldName].size() < minItems) {
            addError("Array must have at least " + std::to_string(minItems) + " items");
			popPath();
			return *this;
        }

		if (obj[fieldName][0].type_name() != typeName) {
			addError("Array items must be of type '" + typeName + "'");
		}
        popPath();
        return *this;
    }

    // ----------------------------------------------------------------------
    /** @brief Validate each item in an array using a custom validator function
     * @param obj The JSON object to validate
     * @param fieldName The name of the array field to check
     * @param itemValidator A function that takes a JSON item, a JsonValidator reference, and the item index
     *                      to perform custom validation on each array item
     * @return JsonValidator&
     * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& validateObjectItems(const json& obj, const std::string& fieldName,
        std::function<void(const json&, JsonValidator&, size_t)> itemValidator) {

        int numErrors = static_cast<int>(errors.size());
		*this = requireField(obj, fieldName);
        if (static_cast<int>(errors.size()) > numErrors) {
            return *this;
        }

        if (obj.contains(fieldName) && obj[fieldName].is_array()) {
            const auto& arr = obj[fieldName];
            for (size_t i = 0; i < arr.size(); ++i) {
                pushPath(fieldName + "[" + std::to_string(i) + "]");
                itemValidator(arr[i], *this, i);
                popPath();
            }
        }
        return *this;
    }

	// ----------------------------------------------------------------------
    /** @brief Test if an optional field is of the expected type
     * Add a validation error if the field is of the wrong type
     * @param obj The JSON object to validate
     * @param fieldName The name of the field to check
     * @param typeName The expected type of the field
     * @return JsonValidator&
	 * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& optionalType(const json& obj, const std::string& fieldName, const std::string& typeName) {
        if (obj.contains(fieldName) && !obj[fieldName].is_null()) {
            requireType(obj, fieldName, typeName);
        }
        return *this;
    }

	// ----------------------------------------------------------------------
    /** @brief Apply a custom validation function to a field if it exists
     * @param obj The JSON object to validate
     * @param fieldName The name of the field to check
     * @param validator A function that takes a JSON value and returns true if valid, false otherwise
	 * @param errorMessage The error message to add if validation fails
	 * @return JsonValidator&
	 * @throws NO EXCEPTION HANDLING
    **/
    JsonValidator& custom(const json& obj, const std::string& fieldName,
        std::function<bool(const json&)> validator,
        const std::string& errorMessage) {
        pushPath(fieldName);
        if (obj.contains(fieldName)) {
            if (!validator(obj[fieldName])) {
                addError(errorMessage);
            }
        }
        popPath();
        return *this;
    }
};

// ----------------------------------------------------------------------
// Function prototypes
std::vector<std::string> split(std::string s, std::string delimiter);
std::wstring stringToWString(const std::string& str);
unsigned long stringToUnsignedLong(std::string str);
bool stringToBool(std::string str);
std::string COleTimeToIsoStr(double oleTime);
std::string base64Encode(BYTE const* buf, unsigned int bufLen);
std::string wchartToUtf8String(const wchar_t* wstr, size_t len);
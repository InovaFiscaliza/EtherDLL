/** @file EtherDLLUtils.hpp
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
#include <cmath>

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
/** @brief Compute djb2 hash of string
 * @param str The input string
 * @param hash The initial hash value (default is 5381)
 * @return The computed hash value
 * @throws NO EXCEPTION HANDLING
**/
constexpr std::size_t stringToHash(const char* str, std::size_t hash = 5381) {
    return (*str == 0) ? hash : stringToHash(str + 1, ((hash << 5) + hash) + *str);
}

// ----------------------------------------------------------------------
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

/** Class to store and perform online computation of basic descriptive indexes for a normally distributed variable
  * Algorithm description according to:
  *   @ARTICLE{Welford62noteon,
  *    author = {Author(s) B. P. Welford and B. P. Welford},
  *    title = {Note on a method for calculating corrected sums of squares and products},
  *    journal = {Technometrics},
  *    year = {1962},
  *    pages = {419--420}
  *  }
 **/
class Normal {
private:
    // data fields
    double mean_value;      // mean_value = ((count_elements*mean_value)+ X )/(count_elements+1);
    double std_value;       // std_value = ( n-2 / n-1 ) std_value {n-1}+{1\over n}(X_n-\bar X_{n-1})^2.
    size_t count_elements;              // count_elements = count_elements + 1;
    double sum;             // to reduce computational effort and rounding error on average computation
    double sum_squares;     // to reduce computational effort and reduce error on standard deviation computation

    // ---------------------------------------------------------------
    /** @brief Validate arithmetic calculations for mean and standard deviation
     * Ensures count_elements is positive (prevents division by zero), validates that computed
     * values are finite (not NaN or Inf), and ensures sum_squares is non-negative
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void validateArguments(double newMean, double newStd, size_t newCount, double newSum, double newSumSquares) {
        // Validation: Ensure count is positive to avoid division by zero
        if (newCount < 0) {
			throw std::invalid_argument("Count must be positive. Error in validateArguments");
        }
        else if (newCount == 0) {
            if (newMean != 0.0 || newStd != 0.0 || newSum != 0.0 || newSumSquares != 0.0) {
                throw std::invalid_argument("All values must be zero when count is 0. Error in validateArguments");
            }
            else {
                return; // All values are valid for count 0
            }
        }
        else if (newCount == 1) {
            if (newStd != 0.0) {
                throw std::invalid_argument("Standard deviation must be zero when count is 1. Error in validateArguments");
            }
            if (newSum != newMean) {
                throw std::invalid_argument("Sum must equal mean when count is 1. Error in validateArguments");
            }
            if (newSumSquares != 0.0) {
                throw std::invalid_argument("Sum of squares must be zero when count is 1. Error in validateArguments");
            }
		}

        // Validation: Check for NaN or Inf in mean_value
        if (!std::isfinite(newMean)) {
            throw std::invalid_argument("Mean must be finite. Error in validateArguments");
        }

        // Validation: sum_squares should never be negative (mathematical property)
        if (newSumSquares < 0.0) {
            throw std::invalid_argument("Sum of squares must be non-negative. Error in validateArguments");
        }

        // Validation: Check for NaN or Inf in std_value
        if (!std::isfinite(newStd)) {
            throw std::invalid_argument("Standard deviation must be finite. Error in validateArguments");
        }


		// Considering that newCount is greater than 1 and all values so far are valid, test the arithmetic relationships
        if (newSum / newCount != newMean) {
            throw std::invalid_argument("Sum divided by count must equal mean. Error in validateArguments");
		}

        double computedStd = sqrt(newSumSquares / static_cast<double>(newCount));
        if (std::abs(computedStd - newStd) > 1e-6) {
            throw std::invalid_argument("Standard deviation does not match computed value. Error in validateArguments");
        }
    }

public:
    // Default constructor
    Normal() : mean_value(0.0), std_value(0.0), count_elements(0), sum(0.0), sum_squares(0.0) {}


    // ---------------------------------------------------------------
    /** @brief Add element to the normal distribution statistics
     * @param new_element The new value to add
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void add_element(double new_element) {
        double previous_mean_value;
        double delta;

        if (count_elements > 1) {
            previous_mean_value = mean_value;

            sum = sum + new_element;
            count_elements++;
            mean_value = sum / (double)count_elements;

            sum_squares = sum_squares + ((new_element - previous_mean_value) * (new_element - mean_value));

            std_value = sqrt(sum_squares / (double)count_elements);
        }
        else {
            if (count_elements < 1) {
                mean_value = new_element;
                count_elements = 1;
                sum = new_element;
            }
            else {
                count_elements = 2;
                mean_value = (mean_value + new_element) / (double)count_elements;
                sum = sum + new_element;
                delta = new_element - mean_value;
                sum_squares = delta * delta;
            }
        }
    }

    // ---------------------------------------------------------------
    /** @brief Get the mean value
     * @param None
     * @return double The current mean value
     * @throws NO EXCEPTION HANDLING
    **/
    double mean() const {
        return mean_value;
    }

    // ---------------------------------------------------------------
    /** @brief Get the standard deviation value
     * @param None
     * @return double The current standard deviation
     * @throws NO EXCEPTION HANDLING
    **/
    double std() const {
        return std_value;
    }

    // ---------------------------------------------------------------
    /** @brief Get the count_elements of elements
     * @param None
     * @return int The number of elements added
     * @throws NO EXCEPTION HANDLING
    **/
    size_t count() const {
        return count_elements;
    }

    // ---------------------------------------------------------------
    /** @brief Reset all values to initial state
     * @param None
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void reset() {
        mean_value = 0.0;
        std_value = 0.0;
        count_elements = 0;
        sum = 0.0;
        sum_squares = 0.0;
    }

    // ---------------------------------------------------------------
    /** @brief Set all values manually
     * @param newMean The mean value to set
     * @param newStd The standard deviation to set
     * @param newCount The count to set
     * @param newSum The sum to set
     * @param newSumSquares The sum of squares to set
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void set(double newMean, size_t newCount, double newSum, double newStd = 0.0, double newSumSquares = 0.0) {

		validateArguments(newMean, newStd, newCount, newSum, newSumSquares);

        mean_value = newMean;
        std_value = newStd;
        count_elements = newCount;
        sum = newSum;
        sum_squares = newSumSquares;
    }
};

/** Class to store and perform online computation of basic descriptive indexes for a variable that do not follow a normal probability distribution
 *  May work with windowed data, histogram computation, categories or only min/max tracking
**/
class NonNormal {
private:
    // basic fields
	double maximum_value;           // maximum value observed
    double minimum_value;           // minimum value observed
	size_t count_elements;          // total number of elements observed

	// windowed data fields
	bool has_window;                // Flag to indicate if windowed data is enabled
	size_t window_size;             // Maximum size of the circular buffer
	std::vector<double> window;     // Circular buffer for windowed data
    size_t write_index;             // Current write position in circular buffer
    size_t current_size;            // Number of valid elements in buffer

    // histogram fields
	bool has_histogram;             // Flag to indicate if histogram is enabled
	std::vector<size_t> histogram;  // Histogram bin counts
	size_t num_bins;                // Number of histogram bins
	double hist_min;                // Minimum value for histogram range
    double hist_max;                // Maximum value for histogram range
	double bin_width;               // Width of each histogram bin

	// category fields
	bool has_categories;            // Flag to indicate if categories are enabled
    json categories;                // JSON object to hold category information

    // ----------------------------------------------------------------------
    /** @brief Calculate the bin index for a given value
     * Uses discrete approximation to the closest bin
     * @param value The value to find the bin for
     * @return size_t The bin index (clamped to valid range)
     * @throws NO EXCEPTION HANDLING
    **/
    size_t getBinIndex(double value) const {
        if (num_bins == 0 || bin_width <= 0.0) {
            return 0;
        }

        // Calculate bin index with rounding to nearest bin
        double normalized = (value - hist_min) / bin_width;
        int bin = static_cast<int>(std::round(normalized));

        // Clamp to valid range [0, num_bins - 1]
        if (bin < 0) {
            return 0;
        }
        if (static_cast<size_t>(bin) >= num_bins) {
            return num_bins - 1;
        }
        return static_cast<size_t>(bin);
    }

public:
    // ----------------------------------------------------------------------
    /** @brief Default constructor with default sample size of 1000 and no histogram
     * @throws NO EXCEPTION HANDLING
    **/
    NonNormal() :   maximum_value(std::numeric_limits<double>::lowest()),
                    minimum_value(std::numeric_limits<double>::max()),
                    count_elements(0),

		            has_window(false),
                    window_size(0),
                    window(),
                    write_index(0),
                    current_size(0),

		            has_histogram(false),
                    hist_min(0.0),
                    hist_max(0.0),
                    num_bins(0),
                    bin_width(0.0),
                    histogram(),

                    has_categories(false),
		            categories()
                    {}


    // ----------------------------------------------------------------------
    /** @brief Constructor for windowed data tracking
     * @param window_size The maximum number of elements to store in the circular buffer
     * @throws std::invalid_argument if window_size is 0
    **/
    static NonNormal new_window(size_t window_size) {
        if (window_size == 0) {
            throw std::invalid_argument("Window size must be greater than 0 for windowed mode");
        }
        NonNormal instance;
        instance.has_window = true;
        instance.window_size = window_size;
        instance.window.resize(window_size, 0.0);

        return instance;
    }

    // ----------------------------------------------------------------------
    /** @brief Constructor for histogram tracking
     * @param histogram_bins The number of bins for the histogram
     * @param histogram_min The minimum value for histogram range
     * @param histogram_max The maximum value for histogram range
     * @throws std::invalid_argument if histogram parameters are invalid
    **/
    static NonNormal new_histogram(size_t histogram_bins, double histogram_min, double histogram_max) {
        if (histogram_bins == 0) {
            throw std::invalid_argument("Number of histogram bins must be greater than 0");
        }
        if (histogram_max <= histogram_min) {
            throw std::invalid_argument("Histogram max must be greater than min");
        }
        NonNormal instance;
        instance.has_histogram = true;
        instance.num_bins = histogram_bins;
        instance.hist_min = histogram_min;
        instance.hist_max = histogram_max;
        instance.bin_width = (histogram_max - histogram_min) / static_cast<double>(histogram_bins);
        instance.histogram.resize(histogram_bins, 0);
        return instance;
    }

    // ----------------------------------------------------------------------
    /** @brief Static factory for categorical data tracking 
     * @return NonNormal A new instance with categorical tracking enabled
     * @throws NO EXCEPTION HANDLING
    **/
    static NonNormal new_categorical() {
        NonNormal instance;
        instance.has_categories = true;
        return instance;
    }

    // ----------------------------------------------------------------------
    /** @brief Enable histogram data tracking on this instance
     * @param histogram_bins The number of bins for the histogram
     * @param histogram_min The minimum value for histogram range
     * @param histogram_max The maximum value for histogram range
     * @return NonNormal& Reference to this instance for method chaining
     * @throws NO EXCEPTION HANDLING
    **/
    NonNormal& enable_histogram(size_t histogram_bins, double histogram_min, double histogram_max) {
        has_histogram = true;
        num_bins = histogram_bins;
        hist_min = histogram_min;
        hist_max = histogram_max;
        bin_width = (histogram_max - histogram_min) / static_cast<double>(histogram_bins);
        histogram.resize(histogram_bins, 0);
        return *this;
    }

    // ----------------------------------------------------------------------
    /** @brief Enable windowed data tracking on this instance
     * @param window_size The maximum number of elements to store in the circular buffer
     * @return NonNormal& Reference to this instance for method chaining
     * @throws NO EXCEPTION HANDLING
     */
    NonNormal& enable_window(size_t window_size) {
        has_window = true;
        this->window_size = window_size;
        window.resize(window_size, 0.0);
        return *this;
    }

    // ----------------------------------------------------------------------
    /** @brief Enable categorical data tracking on this instance
     * @return NonNormal& Reference to this instance for method chaining
     * @throws NO EXCEPTION HANDLING
    **/
    NonNormal& enable_categorical() {
        has_categories = true;
        return *this;
    }


    // ----------------------------------------------------------------------
    /** @brief Add element to the distribution statistics
     * Overwrites oldest element if maximum size is reached
     * Updates histogram if enabled
     * @param new_element The new value to add
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void add_element(double new_element) {
        double removedValue = 0.0;
        bool elementRemoved = false;

        // Handle has_window data
        if (has_window) {
            // Check if we're overwriting an existing element
            if (current_size >= window_size) {
                removedValue = window[write_index];
                elementRemoved = true;
            }

            // Write new element at current position
            window[write_index] = new_element;
            
            // Advance write index (circular)
            write_index = (write_index + 1) % window_size;
            
            // Update current size (cap at window_size)
            if (current_size < window_size) {
                current_size++;
            }

            // Recalculate min/max if removed element was an extreme value
            if (elementRemoved) {
                if (removedValue >= maximum_value || removedValue <= minimum_value) {
                    auto [minIt, maxIt] = std::minmax_element(window.begin(), window.begin() + current_size);
                    minimum_value = *minIt;
                    maximum_value = *maxIt;
                }
            }
        }

        // Update histogram if enabled
        if (has_histogram) {
            size_t binIndex = getBinIndex(new_element);
            histogram[binIndex]++;
        }

        // Update categories if enabled
        if (has_categories) {
            std::string key = std::to_string(new_element);
            if (categories.contains(key)) {
                categories[key] = categories[key].get<size_t>() + 1;
            }
            else {
                categories[key] = 1;
            }
		}

        // Update min/max values
        if (new_element > maximum_value) {
            maximum_value = new_element;
        }

        if (new_element < minimum_value) {
            minimum_value = new_element;
        }

        // update count_elements
        count_elements++;
    }

    // ----------------------------------------------------------------------
    /** @brief Get the maximum value
     * @return double The current maximum value
     * @throws NO EXCEPTION HANDLING
    **/
    double max() const {
        return maximum_value;
    }

    // ----------------------------------------------------------------------
    /** @brief Get the minimum value
     * @return double The current minimum value
     * @throws NO EXCEPTION HANDLING
    **/
    double min() const {
        return minimum_value;
    }

    // ----------------------------------------------------------------------
    /** @brief Get the total count of elements added
     * @return size_t The number of elements added
	 * @throws NO EXCEPTION HANDLING
    **/
    size_t count() const {
        return count_elements;
    }

    // ----------------------------------------------------------------------
    /** @brief Get the current count of elements in the window
     * @return size_t The number of elements in the buffer
     * @throws NO EXCEPTION HANDLING
    **/
    size_t used() const {
        return current_size;
    }

    // ----------------------------------------------------------------------
    /** @brief Get the maximum capacity
     * @return size_t The maximum number of elements allowed
     * @throws NO EXCEPTION HANDLING
    **/
    size_t available() const {
        return window_size - current_size;
    }

    // ----------------------------------------------------------------------
	/** @brief Get the available window data vector
	 * @return const std::vector<double>& Reference to the window data ordered from newest to oldest
	 * @throws runtime_error if windowed data is not enabled
    **/
    const std::vector<double>& window_data() {
		// create a copy from the initial segment of the circular buffer
        if (!has_window) throw std::runtime_error("Window data not enabled");

		// create a vector with the same size as current_size
		std::vector<double> available_data(current_size);

        size_t index = write_index;
        for(int i = 0; i < current_size; i++) {
            available_data[i] = window[index];
			index--;
            if (index == static_cast<size_t>(-1)) {
                index = window_size - 1;
			}
		}
		return available_data;
    }

    // ----------------------------------------------------------------------
    /** @brief Get the histogram vector
     * @return const std::vector<size_t>& Reference to the histogram data
     * @throws runtime_error if histogram is not enabled
    **/
    const std::vector<size_t>& histogram_data() const {
        if (!has_histogram) {
            throw std::runtime_error("Histogram is not enabled");
        }
        return histogram;
    }

    // ----------------------------------------------------------------------
    /** @brief Get the number of histogram bins
     * @return size_t The number of bins (0 if histogram disabled)
	 * @throws runtime_error if histogram is not enabled
    **/
    size_t n_bins() const {
        if (!has_histogram) {
            throw std::runtime_error("Histogram is not enabled");
		}
        return num_bins;
    }


    // ----------------------------------------------------------------------
    /** @brief Get the histogram maximum count
     * @return size_t The highest count in any histogram bin
     * @throws runtime_error if histogram is not enabled
    **/
    size_t histogram_max() const {
        if (!has_histogram) {
            throw std::runtime_error("Histogram is not enabled");
        }
        // Find the maximum count in the histogram
        auto max_it = std::max_element(histogram.begin(), histogram.end());
        return *max_it;
    }


    // ----------------------------------------------------------------------
	/** @brief Get the mode from the histogram data
	 * @return double The center value of the bin with the highest count
     * @throws runtime_error if histogram is not enabled
    **/
    double mode() const {
        double mode_value = NAN;

		// preferable method using categorical data if available
        if (has_categories) {
            size_t max_count = 0;
            std::string mode_key;
            for (auto& [key, value] : categories.items()) {
                size_t count = value.get<size_t>();
                if (count > max_count) {
                    max_count = count;
                    mode_key = key;
                }
            }
            if (!mode_key.empty()) {
                return std::stod(mode_key);
            }
            else {
				throw std::runtime_error("No categories available to determine mode.");
            }
		}

        if (has_histogram) {
            auto max_it = std::max_element(histogram.begin(), histogram.end());
            size_t max_bin = std::distance(histogram.begin(), max_it);

            // Calculate the center value of the bin
            return hist_min + (max_bin * bin_width);
        }

		throw std::runtime_error("Categories or histogram must be enabled to determine mode.");

    }

    // ----------------------------------------------------------------------
	/** @brief Get the categories JSON object
     * @return json The categories JSON object
     * @throws NO EXCEPTION HANDLING
    **/
    json get_categories() const {
        if (!has_categories) {
            throw std::runtime_error("Categories are not enabled");
        }
        return categories;
    }


    // ----------------------------------------------------------------------
	/** @brief Get the count of unique categories
	 * @return size_t The number of unique categories
     * @throws NO EXCEPTION HANDLING
    **/
    size_t unique_categories() const {
        if (!has_categories) {
            throw std::runtime_error("Categories are not enabled");
        }
        return categories.size();
    }

    // ----------------------------------------------------------------------
	/** @brief Get the number of occurrences of a specific category
	 * @return size_t The count of the specified category
     * @throws NO EXCEPTION HANDLING
    **/
    size_t category_count(const double& category) const {
        if (!has_categories) {
            throw std::runtime_error("Categories are not enabled");
        }
        if (categories.contains(std::to_string(category))) {
            return categories[std::to_string(category)].get<size_t>();
        }
        return 0;
	}
        

    // ----------------------------------------------------------------------
	/** @brief Reset windowed data, histogram, min/max, and count
     * @return void
     * @throws NO EXCEPTION HANDLING
    **/
    void reset() {
        // Reset min/max
        minimum_value = std::numeric_limits<double>::max();
        maximum_value = std::numeric_limits<double>::lowest();

        // Reset count
        count_elements = 0;

        // Reset windowed data
        if (has_window) {
            std::fill(window.begin(), window.end(), 0.0);
            write_index = 0;
            current_size = 0;
        }

        // Reset histogram
        if (has_histogram) {
            std::fill(histogram.begin(), histogram.end(), 0);
        }

		// Reset categories
        if (has_categories) {
            categories.clear();
		}

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
static  std::string handleInputArguments(int argc, char* argv[]);
json buildDemoData();
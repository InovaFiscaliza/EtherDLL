/**
 * @file etherDLLData.hpp
 *
 * @brief Data structures and processing definitions for EtherDLL spectrum measurement storage
 *
 * This header defines the data architecture for storing spectrum measurements
 * with configurable trace types (max, min, last, average) and time-series
 * capabilities for up to 60 seconds of 1-second aggregated data.
 *
 * Thread-safety: All structures containing mutable data include mutex protection.
 * Access methods are marked with thread-safety guarantees.
 *
 * @author fslobao
 * @date 2026-02-05
 * @version 1.0
 *
 * @note Requires C++17 or later
 * @note Uses nlohmann/json library for JSON handling
 * @note Uses spdlog for logging
 *
 * Dependencies:
 * - StdAfx.h (DLL provided)
 * - EquipCtrlMsg.h (DLL provided)
 * - etherDLLResponse.hpp (DLL specific)
 * - EtherDLLConfig.hpp (Core)
 * - EtherDLLUtils.hpp (Core - Normal and NonNormal classes)
**/
// ----------------------------------------------------------------------
#pragma once

// Include provided DLL libraries
#include "StdAfx.h"
#include "EquipCtrlMsg.h"

// Include DLL specific libraries
#include "etherDLLResponse.hpp"

// Include core EtherDLL libraries
#include "EtherDLLConfig.hpp"
#include "EtherDLLUtils.hpp"

// Include general C++ libraries
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <cstdint>
#include <limits>
#include <chrono>
#include <optional>
#include <mutex>
#include <shared_mutex>
#include <iomanip>
#include <sstream>

// ----------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------

/** @brief Power offset for PAN (Panoramic) spectrum byte conversion */
constexpr float PAN_BYTE_POWER_OFFSET = 192.0f;

/** @brief Power offset for OCC (Occupancy) spectrum byte conversion */
constexpr float OCC_BYTE_POWER_OFFSET = 200.0f;

/** @brief Scaling factor for confidence values (tenths of percent: 0-1000) */
constexpr float CONFIDENCE_SCALE = 10.0f;

/** @brief Scaling factor for azimuth values (hundredths of degree: 0-35999) */
constexpr float AZIMUTH_SCALE = 100.0f;

/** @brief Maximum number of spectrum trace records to keep in history */
constexpr size_t MAX_TRACE_HISTORY = 60;


// ----------------------------------------------------------------------
// Data Structures
// ----------------------------------------------------------------------

/**
 * @struct DFDataRawResult
 * @brief Container for raw Direction Finding (DF) binary data from hardware
 *
 * Stores unprocessed binary data as received from the Scorpio DLL before
 * conversion to float/engineering units. All vectors should have the same
 * size corresponding to the number of frequency bins.
 *
 * @threadsafety Not thread-safe - external synchronization required if shared
 */
struct DFDataRawResult {
    /** @brief Azimuth data: numBins * sizeof(uint16_t) bytes */
    std::vector<unsigned char> azimuth;

    /** @brief Confidence level data: numBins bytes */
    std::vector<unsigned char> confidence;

    /** @brief Spectrum power data: numBins bytes */
    std::vector<unsigned char> spectrum;

    /** @brief DF-specific spectrum data: numBins bytes */
    std::vector<unsigned char> dfSpectrum;

    /**
     * @brief Default constructor - creates empty vectors
     */
    DFDataRawResult() = default;

    /**
     * @brief Constructor with size initialization
     *
     * @param size Number of bytes to allocate for each vector
     * @param defaultValue Initial value for all bytes (default: 0)
     */
    explicit DFDataRawResult(size_t size, unsigned char defaultValue = 0)
        : azimuth(size, defaultValue)
        , confidence(size, defaultValue)
        , spectrum(size, defaultValue)
        , dfSpectrum(size, defaultValue)
    {
    }
};

/**
 * @struct Site
 * @brief GPS location information with statistical tracking
 *
 * Stores latitude/longitude coordinates with measurement statistics
 * (mean, standard deviation, count) and satellite information.
 * Uses Normal class for coordinate statistics and NonNormal for satellite counts.
 *
 * @threadsafety Read/write operations require external synchronization
 */
struct Site {
    /** @brief Latitude statistics in decimal degrees (mean, stddev, count) */
    Normal latitude;

    /** @brief Longitude statistics in decimal degrees (mean, stddev, count) */
    Normal longitude;

    /** @brief Satellite count histogram (0-7+ satellites) */
    NonNormal satelliteCount;

    /** @brief Timestamp of first GPS measurement received */
    std::chrono::system_clock::time_point firstUpdateTime;

    /** @brief Timestamp of most recent GPS measurement */
    std::chrono::system_clock::time_point lastUpdateTime;

    /**
     * @brief Get first update time as ISO 8601 string
     * @return std::string Formatted timestamp (e.g., "2025-09-16T14:32:05.123Z")
     * @threadsafety Thread-safe
     */
    std::string getFirstUpdateTimeISO() const {
        return TimeStampHelper::toISO8601(firstUpdateTime);
    }

    /**
     * @brief Get last update time as ISO 8601 string
     * @return std::string Formatted timestamp
     * @threadsafety Thread-safe
     */
    std::string getLastUpdateTimeISO() const {
        return TimeStampHelper::toISO8601(lastUpdateTime);
    }
};

/**
 * @struct Equipment
 * @brief Hardware equipment identification and version information
 *
 * Stores static equipment metadata including model, serial number,
 * firmware/hardware/software versions, and calibration date.
 *
 * @threadsafety Read/write operations require external synchronization
 */
struct Equipment {
    /** @brief Timestamp of last equipment info update */
    std::chrono::system_clock::time_point lastUpdateTime;

    /** @brief Equipment model identifier */
    std::string model;

    /** @brief Equipment serial number */
    std::string serialNumber;

    /** @brief Firmware version string */
    std::string firmwareVersion;

    /** @brief Hardware version string */
    std::string hardwareVersion;

    /** @brief Software version string */
    std::string softwareVersion;

    /** @brief Calibration date in ISO 8601 format (YYYY-MM-DD) */
    std::string calibrationDate;

    /**
     * @brief Get last update time as ISO 8601 string
     * @return std::string Formatted timestamp
     * @threadsafety Thread-safe
     */
    std::string getLastUpdateTimeISO() const {
        return TimeStampHelper::toISO8601(lastUpdateTime);
    }
};

/**
 * @struct FrequencyRange
 * @brief Defines frequency span and bin resolution for spectrum measurements
 *
 * Specifies the frequency range [startFrequency, stopFrequency] and bin size
 * for spectral data. Provides validation helper to calculate expected bin count.
 *
 * @threadsafety Immutable after initialization - thread-safe for reads
 */
struct FrequencyRange {
    /** @brief Start frequency in Hz */
    double startFrequency;

    /** @brief Stop frequency in Hz */
    double stopFrequency;

    /** @brief Frequency resolution per bin in Hz */
    double binSize;

    /**
     * @brief Calculate the number of frequency bins
     *
     * @return size_t Number of bins covering the frequency range
     *
     * @threadsafety Thread-safe (const method)
     * @throws NO EXCEPTION HANDLING
     */
    size_t getBinCount() const {
        return static_cast<size_t>((stopFrequency - startFrequency) / binSize) + 1;
    }
};

/**
 * @struct Configuration
 * @brief Spectrum analyzer configuration parameters
 *
 * Contains measurement settings including frequency range, gain/attenuation,
 * reference level, and trace options. This configuration applies to all
 * subsequent spectrum measurements until changed.
 *
 * @threadsafety Read/write operations require external synchronization
 */
struct Configuration {
    /** @brief Frequency span and resolution settings */
    FrequencyRange frequencyInfo;

    /** @brief Receiver gain in dB (negative values for attenuation) */
    float receiverGain;

    /** @brief Reference level in dBm */
    float referenceLevel;

    /** @brief Preselector gain in dB (negative values for attenuation) */
    float preselectorGain;

    /** @brief Enable maximum trace computation and storage */
    bool enableMax;

    /** @brief Enable minimum trace computation and storage */
    bool enableMin;

    /** @brief Enable last trace storage */
    bool enableLast;

    /** @brief Enable average trace computation and storage */
    bool enableAverage;

    /** @brief Timestamp of last configuration change */
    std::chrono::system_clock::time_point lastUpdateTime;

    /**
     * @brief Default constructor - disables all trace types
     */
    Configuration()
        : receiverGain(0.0f)
        , referenceLevel(0.0f)
        , preselectorGain(0.0f)
        , enableMax(false)
        , enableMin(false)
        , enableLast(false)
        , enableAverage(false)
    {
    }

    /**
     * @brief Get last update time as ISO 8601 string
     * @return std::string Formatted timestamp
     * @threadsafety Thread-safe
     */
    std::string getLastUpdateTimeISO() const {
        return TimeStampHelper::toISO8601(lastUpdateTime);
    }
};

/**
 * @struct SpectrumTrace
 * @brief Single spectrum measurement with optional trace types
 *
 * Contains one second of aggregated spectrum data with optional max, min,
 * last, and average traces. Each enabled trace vector must have size equal
 * to the number of frequency bins defined in Configuration::frequencyInfo.
 *
 * The sumTrace and traceCount fields support online average computation:
 * average[i] = sumTrace[i] / traceCount
 *
 * @threadsafety Not thread-safe - external synchronization required
**/
struct SpectrumTrace {
    
    std::chrono::system_clock::time_point timestamp;    /** @brief Timestamp when this trace was recorded */
    std::optional<std::vector<float>> maxTrace;         /** @brief Maximum value per bin (optional) */
    std::optional<std::vector<float>> minTrace;         /** @brief Minimum value per bin (optional) */    
    std::optional<std::vector<float>> lastTrace;        /** @brief Last measured value per bin (optional) */
    std::optional<std::vector<double>> sumTrace;        /** @brief Accumulator for average computation (optional, uses double for precision) */
    uint32_t traceCount;                                /** @brief Number of measurements aggregated into this trace */

    /**
     * @brief Default constructor - initializes count to zero
     */
    SpectrumTrace() : traceCount(0) {}

    /**
     * @brief Factory method to create a pre-initialized trace
     *
     * Initializes trace vectors with appropriate sentinel values:
     * - maxTrace: lowest float (for max aggregation)
     * - minTrace: highest float (for min aggregation)
     * - sumTrace/lastTrace: zero
     *
     * @param binCount Number of frequency bins
     * @param enableMax Enable and initialize maxTrace
     * @param enableMin Enable and initialize minTrace
     * @param enableLast Enable and initialize lastTrace
     * @param enableAverage Enable and initialize sumTrace
     * @return SpectrumTrace Pre-allocated and initialized trace
     *
     * @threadsafety Thread-safe (creates new object)
     * @throws NO EXCEPTION HANDLING
    **/
    static SpectrumTrace createPreallocated(
        size_t binCount,
        bool enableMax,
        bool enableMin,
        bool enableLast,
        bool enableAverage)
    {
        SpectrumTrace trace;
        trace.timestamp = std::chrono::system_clock::now();
        trace.traceCount = 0;

        if (enableMax) {
            trace.maxTrace.emplace(binCount, std::numeric_limits<float>::lowest());
        }
        if (enableMin) {
            trace.minTrace.emplace(binCount, std::numeric_limits<float>::max());
        }
        if (enableLast) {
            trace.lastTrace.emplace(binCount, 0.0f);
        }
        if (enableAverage) {
            trace.sumTrace.emplace(binCount, 0.0);
        }

        return trace;  // RVO/NRVO - no copy, direct move
    }

    /**
     * @brief Get timestamp as ISO 8601 string
     * @return std::string Formatted timestamp
     * @threadsafety Thread-safe
     */
    std::string getTimestampISO() const {
        return TimeStampHelper::toISO8601(timestamp);
    }

    /**
     * @brief Validate that all enabled traces have the expected size
     *
     * Checks that each optional trace (if present) has exactly expectedBinCount elements.
     *
     * @param expectedBinCount The required size for all trace vectors
     * @return bool True if all enabled traces are valid, false otherwise
     *
     * @threadsafety Thread-safe (const method)
     * @throws NO EXCEPTION HANDLING
     */
    bool validate(size_t expectedBinCount) const {
        if (maxTrace.has_value() && maxTrace->size() != expectedBinCount) {
            return false;
        }
        if (minTrace.has_value() && minTrace->size() != expectedBinCount) {
            return false;
        }
        if (lastTrace.has_value() && lastTrace->size() != expectedBinCount) {
            return false;
        }
        if (sumTrace.has_value() && sumTrace->size() != expectedBinCount) {
            return false;
        }
        return true;
    }

    /**
     * @brief Calculate average trace from accumulated sum
     *
     * Computes average[i] = sumTrace[i] / traceCount for all bins.
     *
     * @return std::vector<float> The computed average trace
     *
     * @threadsafety Thread-safe (const method)
     * @throws std::runtime_error if sumTrace is not present or traceCount is zero
     */
    std::vector<float> getAverageTrace() const {
        if (!sumTrace.has_value()) {
            throw std::runtime_error("Sum trace not available for average calculation");
        }
        if (traceCount == 0) {
            throw std::runtime_error("Cannot compute average with zero trace count");
        }

        std::vector<float> avgTrace;
        avgTrace.reserve(sumTrace->size());

        for (const auto& sum : *sumTrace) {
            avgTrace.push_back(static_cast<float>(sum / static_cast<double>(traceCount)));
        }

        return avgTrace;
    }
};

/**
 * @struct SpectrumData
 * @brief Time-series collection of spectrum measurements with thread-safety
 *
 * Stores up to MAX_TRACE_HISTORY (60) seconds of spectrum data in a deque
 * for efficient FIFO operations. Includes automatic removal of oldest traces
 * when capacity is reached.
 *
 * Thread-safety: All public methods use shared_mutex for concurrent reads
 * and exclusive writes. Multiple threads can read simultaneously, but writes
 * are exclusive.
 *
 * @threadsafety Thread-safe (internal mutex protection)
 */
struct SpectrumData {
    MeasurementUnit unit;              ///< Measurement unit (enum, 1 byte)
    FrequencyRange frequencyInfo;      ///< Frequency range and bin size
    Configuration configuration;       ///< Configuration settings
    std::deque<SpectrumTrace> traces;  ///< Time-series data

    /**
     * @brief Default constructor
     */
    SpectrumData() : unit(MeasurementUnit::UNKNOWN) {}

    /**
     * @brief Parameterized constructor with enum unit
     *
     * @param config Configuration settings
     * @param freqInfo Frequency range
     * @param measurementUnit Unit enum (default: dBm)
     * @param capacity Trace capacity (default: MAX_TRACE_HISTORY)
     */
    SpectrumData(
        const Configuration& config,
        const FrequencyRange& freqInfo,
        MeasurementUnit measurementUnit = MeasurementUnit::DBM,
        size_t capacity = MAX_TRACE_HISTORY)
        : unit(measurementUnit)
        , frequencyInfo(freqInfo)
        , configuration(config)
    {
        size_t binCount = frequencyInfo.getBinCount();

        for (size_t i = 0; i < capacity; ++i) {
            traces.push_back(
                SpectrumTrace::createPreallocated(
                    binCount,
                    config.enableMax,
                    config.enableMin,
                    config.enableLast,
                    config.enableAverage
                )
            );
        }
        traces.clear();

        loggerPtr->info("Initialized SpectrumData: {} traces, {} bins, unit={}",
            capacity, binCount, UnitHelper::toString(unit));
    }

    /**
     * @brief Get unit as string
     */
    std::string getUnitString() const {
        return UnitHelper::toString(unit);
    }

    /**
     * @brief Pre-allocate capacity for expected trace count
     *
     * Allocates internal blocks to minimize reallocation during runtime.
     * Call once after configuration is received from client.
     *
     * @param expectedCapacity Number of traces to pre-allocate (typically 60)
     * @param binCount Number of bins per trace
     *
     * @threadsafety Thread-safe (exclusive lock)
     */
     /**
      * @brief Pre-allocate capacity for expected trace count
      *
      * Allocates internal deque blocks and pre-initializes trace vectors with
      * appropriate sentinel values for aggregation operations.
      *
      * @param capacity Number of traces to pre-allocate (typically 60)
      * @param binCount Number of bins per trace
      *
      * @threadsafety Thread-safe (exclusive lock)
      */
    void preallocate(size_t capacity, size_t binCount) {
        std::unique_lock<std::shared_mutex> lock(dataMutex);

        // Reserve space and initialize with proper sentinel values
        for (size_t i = 0; i < capacity; ++i) {
            traces.push_back(
                SpectrumTrace::createPreallocated(
                    binCount,
                    configuration.enableMax,
                    configuration.enableMin,
                    configuration.enableLast,
                    configuration.enableAverage
                )
            );
        }

        // Clear elements but keep allocated memory
        traces.clear();

        loggerPtr->info("Pre-allocated {} traces with {} bins each (Max={}, Min={}, Last={}, Avg={})",
            capacity, binCount,
            configuration.enableMax, configuration.enableMin,
            configuration.enableLast, configuration.enableAverage);
    }

    /**
     * @brief Add a new trace, removing oldest if at capacity
     *
     * Validates the trace before insertion. If the deque is at MAX_TRACE_HISTORY,
     * removes the oldest trace before adding the new one.
     *
     * @param trace The spectrum trace to add (moved, not copied)
     * @return bool True if added successfully, false if validation failed
     *
     * @threadsafety Thread-safe (exclusive lock)
     * @throws NO EXCEPTION HANDLING
     */
    bool addTrace(SpectrumTrace&& trace) {
        std::unique_lock<std::shared_mutex> lock(dataMutex);

        if (!trace.validate(frequencyInfo.getBinCount())) {
            return false;
        }

        if (traces.size() >= MAX_TRACE_HISTORY) {
            traces.pop_front();  // Remove oldest
        }
        traces.push_back(std::move(trace));
        return true;
    }

    /**
     * @brief Get the number of stored traces
     *
     * @return size_t Current number of traces in the deque
     *
     * @threadsafety Thread-safe (shared lock)
     * @throws NO EXCEPTION HANDLING
     */
    size_t getTraceCount() const {
        std::shared_lock<std::shared_mutex> lock(dataMutex);
        return traces.size();
    }

    /**
     * @brief Clear all stored traces
     *
     * @threadsafety Thread-safe (exclusive lock)
     * @throws NO EXCEPTION HANDLING
     */
    void clear() {
        std::unique_lock<std::shared_mutex> lock(dataMutex);
        traces.clear();
    }

    /**
     * @brief Get a copy of the current configuration
     *
     * @return Configuration Copy of current settings
     *
     * @threadsafety Thread-safe (shared lock)
     * @throws NO EXCEPTION HANDLING
     */
    Configuration getConfiguration() const {
        std::shared_lock<std::shared_mutex> lock(dataMutex);
        return configuration;
    }

    /**
     * @brief Update configuration settings
     *
     * @param newConfig The new configuration to apply
     *
     * @threadsafety Thread-safe (exclusive lock)
     * @throws NO EXCEPTION HANDLING
     */
    void setConfiguration(const Configuration& newConfig) {
        std::unique_lock<std::shared_mutex> lock(dataMutex);
        configuration = newConfig;
    }

    /**
     * @brief Get a copy of all traces (for serialization/export)
     *
     * @return std::deque<SpectrumTrace> Copy of all stored traces
     *
     * @threadsafety Thread-safe (shared lock)
     * @throws NO EXCEPTION HANDLING
     */
    std::deque<SpectrumTrace> getAllTraces() const {
        std::shared_lock<std::shared_mutex> lock(dataMutex);
        return traces;
    }

private:
    /** @brief Mutex for thread-safe access to traces and configuration */
    mutable std::shared_mutex dataMutex;
};

/**
 * @struct BandKey
 * @brief Lightweight key for band identification with numeric unit
 *
 * Uses enum for unit instead of string for:
 * - Faster comparison (1 byte integer vs string)
 * - Type safety (compile-time validation)
 * - Smaller memory footprint (24 bytes → 17 bytes)
 */
struct BandKey {
    double startFrequency;       ///< Start frequency in Hz (8 bytes)
    double stopFrequency;        ///< Stop frequency in Hz (8 bytes)
    MeasurementUnit unit;        ///< Measurement unit (1 byte)

    /**
     * @brief Default constructor
     */
    BandKey()
        : startFrequency(0.0)
        , stopFrequency(0.0)
        , unit(MeasurementUnit::UNKNOWN)
    {
    }

    /**
     * @brief Parameterized constructor
     *
     * @param start Start frequency in Hz
     * @param stop Stop frequency in Hz
     * @param measurementUnit Unit enum value
     */
    BandKey(double start, double stop, MeasurementUnit measurementUnit)
        : startFrequency(start)
        , stopFrequency(stop)
        , unit(measurementUnit)
    {
    }

    /**
     * @brief Equality comparison for unordered_map
     *
     * @param other BandKey to compare
     * @return bool True if all fields match
     */
    bool operator==(const BandKey& other) const {
        return startFrequency == other.startFrequency &&
            stopFrequency == other.stopFrequency &&
            unit == other.unit; 
    }

    /**
     * @brief Get human-readable string representation
     *
     * @return std::string Format: "100.0-200.0 MHz (dBm)"
     */
    std::string toString() const {
        double startMHz = startFrequency / edll::MHZ_MULTIPLIER;
        double stopMHz = stopFrequency / edll::MHZ_MULTIPLIER;
        return std::to_string(startMHz) + "-" + std::to_string(stopMHz) +
            " MHz (" + UnitHelper::toString(unit) + ")";
    }

    /**
     * @brief Export to JSON for serialization
     *
     * @return json JSON object with frequency range and unit string
     */
    json toJson() const {
        return {
            {"startFrequency", startFrequency},
            {"stopFrequency", stopFrequency},
            {"unit", UnitHelper::toString(unit)}
        };
    }

    /**
     * @brief Import from JSON
     *
     * @param j JSON object with frequency/unit fields
     * @return BandKey Parsed band key
     */
    static BandKey fromJson(const json& j) {
        double start = j.value("startFrequency", 0.0);
        double stop = j.value("stopFrequency", 0.0);
        std::string unitStr = j.value("unit", "unknown");

        return BandKey(start, stop, UnitHelper::fromString(unitStr));
    }
};

/**
 * @struct BandKeyHash
 * @brief Optimized hash function for enum-based BandKey
 */
struct BandKeyHash {
    size_t operator()(const BandKey& key) const {
        size_t h1 = std::hash<double>{}(key.startFrequency);
        size_t h2 = std::hash<double>{}(key.stopFrequency);
        size_t h3 = static_cast<size_t>(key.unit); 

        // Boost hash_combine
        h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        h1 ^= h3 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        return h1;
    }
};

/**
 * @struct MultiBandSpectrumData
 * @brief Multi-band storage with enum-based units
 */
struct MultiBandSpectrumData {
    std::unordered_map<BandKey, SpectrumData, BandKeyHash> bands;
    Configuration globalConfiguration;
    Equipment equipment;
    Site site;

    MultiBandSpectrumData() = default;

    /**
     * @brief Add band with enum unit (zero redundancy)
     */
    bool addBand(SpectrumData&& spectrumData) {
        std::unique_lock<std::shared_mutex> lock(globalMutex);

        BandKey key(
            spectrumData.frequencyInfo.startFrequency,
            spectrumData.frequencyInfo.stopFrequency,
            spectrumData.unit 
        );

        bands.emplace(key, std::move(spectrumData));
        loggerPtr->info("Added band: {}", key.toString());
        return true;
    }

    /**
     * @brief Get band by frequency and enum unit (fast lookup)
     */
    SpectrumData* getBand(double start, double stop, MeasurementUnit unit) {
        BandKey key(start, stop, unit);
        auto it = bands.find(key);  // O(1) with enum comparison
        return (it != bands.end()) ? &(it->second) : nullptr;
    }

    /**
     * @brief Get band by frequency and string unit (JSON interface)
     */
    SpectrumData* getBand(double start, double stop, const std::string& unitStr) {
        return getBand(start, stop, UnitHelper::fromString(unitStr));
    }

    // ... rest of methods unchanged ...

private:
    mutable std::shared_mutex globalMutex;
};
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include <memory>

namespace wreckingball {

/**
 * @brief Severity levels for error logging
 */
enum class LogLevel {
    Info,      // Informational message
    Warning,   // Warning that doesn't prevent execution
    Error,     // Error that may affect gameplay
    Critical   // Critical error requiring immediate abort
};

/**
 * @brief Error and event logging system
 *
 * Logs messages to both stderr and a timestamped log file.
 * Thread-safe for multi-threaded logging if needed.
 * Log file created in data/logs/ directory with format: error-YYYYMMDD-HHMMSS-mmm.log
 */
class ErrorLogger {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to the singleton ErrorLogger instance
     */
    static ErrorLogger& GetInstance();

    /**
     * @brief Initialize logger and create log file
     * @param log_directory Directory to store log files (default: data/logs/)
     * @return true on success, false if log file cannot be created
     */
    bool Initialize(const std::string& log_directory = "data/logs/");

    /**
     * @brief Shutdown logger and close log file
     */
    void Shutdown();

    /**
     * @brief Log a message with specified severity
     * @param level Severity level
     * @param message Message to log
     */
    void Log(LogLevel level, const std::string& message);

    /**
     * @brief Log informational message
     * @param message Message to log
     */
    void LogInfo(const std::string& message);

    /**
     * @brief Log warning message
     * @param message Message to log
     */
    void LogWarning(const std::string& message);

    /**
     * @brief Log error message
     * @param message Message to log
     */
    void LogError(const std::string& message);

    /**
     * @brief Log critical error and prepare for shutdown
     * @param message Message to log
     */
    void LogCritical(const std::string& message);

    /**
     * @brief Check if logger is initialized
     * @return true if logger is ready
     */
    bool IsInitialized() const { return is_initialized_; }

    /**
     * @brief Get log file path
     * @return Path to current log file
     */
    const std::string& GetLogFilePath() const { return log_file_path_; }

    // Delete copy constructor and assignment operator (singleton)
    ErrorLogger(const ErrorLogger&) = delete;
    ErrorLogger& operator=(const ErrorLogger&) = delete;

private:
    ErrorLogger() = default;
    ~ErrorLogger();

    /**
     * @brief Generate timestamped log filename
     * @return Filename in format: error-YYYYMMDD-HHMMSS-mmm.log
     */
    std::string GenerateLogFileName() const;

    /**
     * @brief Get current timestamp string
     * @return Formatted timestamp: [YYYY-MM-DD HH:MM:SS.mmm]
     */
    std::string GetTimestamp() const;

    /**
     * @brief Convert log level to string
     * @param level Log level
     * @return String representation (INFO, WARNING, ERROR, CRITICAL)
     */
    const char* LogLevelToString(LogLevel level) const;

    bool is_initialized_ = false;
    std::string log_file_path_;
    std::ofstream log_file_;
    std::mutex log_mutex_;  // Thread safety for logging
};

} // namespace wreckingball

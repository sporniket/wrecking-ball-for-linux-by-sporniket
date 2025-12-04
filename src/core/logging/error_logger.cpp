// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "error_logger.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <ctime>

namespace wreckingball {

ErrorLogger& ErrorLogger::GetInstance() {
    static ErrorLogger instance;
    return instance;
}

ErrorLogger::~ErrorLogger() {
    Shutdown();
}

bool ErrorLogger::Initialize(const std::string& log_directory) {
    std::lock_guard<std::mutex> lock(log_mutex_);

    if (is_initialized_) {
        return true;  // Already initialized
    }

    // Create log directory if it doesn't exist
    try {
        std::filesystem::create_directories(log_directory);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "[ERROR] Failed to create log directory: " << e.what() << std::endl;
        return false;
    }

    // Generate log file name with timestamp
    std::string log_filename = GenerateLogFileName();
    log_file_path_ = log_directory + "/" + log_filename;

    // Open log file
    log_file_.open(log_file_path_, std::ios::out | std::ios::app);
    if (!log_file_.is_open()) {
        std::cerr << "[ERROR] Failed to open log file: " << log_file_path_ << std::endl;
        return false;
    }

    is_initialized_ = true;

    // Write header to log file
    log_file_ << "========================================" << std::endl;
    log_file_ << "Sporniket's Wrecking Ball - Error Log" << std::endl;
    log_file_ << "Log started: " << GetTimestamp() << std::endl;
    log_file_ << "========================================" << std::endl;
    log_file_.flush();

    return true;
}

void ErrorLogger::Shutdown() {
    std::lock_guard<std::mutex> lock(log_mutex_);

    if (!is_initialized_) {
        return;
    }

    if (log_file_.is_open()) {
        log_file_ << "========================================" << std::endl;
        log_file_ << "Log ended: " << GetTimestamp() << std::endl;
        log_file_ << "========================================" << std::endl;
        log_file_.close();
    }

    is_initialized_ = false;
}

void ErrorLogger::Log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(log_mutex_);

    if (!is_initialized_) {
        // Log to stderr only if not initialized
        std::cerr << "[" << LogLevelToString(level) << "] " << message << std::endl;
        return;
    }

    std::string timestamp = GetTimestamp();
    std::string level_str = LogLevelToString(level);

    // Format: [TIMESTAMP] [LEVEL] Message
    std::string formatted_message = timestamp + " [" + level_str + "] " + message;

    // Write to stderr
    std::cerr << formatted_message << std::endl;

    // Write to log file
    if (log_file_.is_open()) {
        log_file_ << formatted_message << std::endl;
        log_file_.flush();  // Ensure immediate write
    }
}

void ErrorLogger::LogInfo(const std::string& message) {
    Log(LogLevel::Info, message);
}

void ErrorLogger::LogWarning(const std::string& message) {
    Log(LogLevel::Warning, message);
}

void ErrorLogger::LogError(const std::string& message) {
    Log(LogLevel::Error, message);
}

void ErrorLogger::LogCritical(const std::string& message) {
    Log(LogLevel::Critical, message);
}

std::string ErrorLogger::GenerateLogFileName() const {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &now_time_t);
#else
    localtime_r(&now_time_t, &tm_buf);
#endif

    std::ostringstream oss;
    oss << "error-"
        << std::setfill('0')
        << std::setw(4) << (tm_buf.tm_year + 1900)
        << std::setw(2) << (tm_buf.tm_mon + 1)
        << std::setw(2) << tm_buf.tm_mday
        << "-"
        << std::setw(2) << tm_buf.tm_hour
        << std::setw(2) << tm_buf.tm_min
        << std::setw(2) << tm_buf.tm_sec
        << "-"
        << std::setw(3) << now_ms.count()
        << ".log";

    return oss.str();
}

std::string ErrorLogger::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &now_time_t);
#else
    localtime_r(&now_time_t, &tm_buf);
#endif

    std::ostringstream oss;
    oss << "["
        << std::setfill('0')
        << std::setw(4) << (tm_buf.tm_year + 1900) << "-"
        << std::setw(2) << (tm_buf.tm_mon + 1) << "-"
        << std::setw(2) << tm_buf.tm_mday << " "
        << std::setw(2) << tm_buf.tm_hour << ":"
        << std::setw(2) << tm_buf.tm_min << ":"
        << std::setw(2) << tm_buf.tm_sec << "."
        << std::setw(3) << now_ms.count()
        << "]";

    return oss.str();
}

const char* ErrorLogger::LogLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARNING";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

} // namespace wreckingball

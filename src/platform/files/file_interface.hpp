// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace wreckingball {

/**
 * @brief Platform-agnostic file I/O interface
 *
 * Abstracts file operations to enable future Android porting
 * where file access may use different APIs (Android asset manager).
 */
class IFileIO {
public:
    virtual ~IFileIO() = default;

    /**
     * @brief Read entire file contents as string
     * @param path File path
     * @return File contents, or empty string on error
     */
    virtual std::string ReadFile(const std::filesystem::path& path) = 0;

    /**
     * @brief Write string contents to file
     * @param path File path
     * @param contents Content to write
     * @return true on success, false on failure
     */
    virtual bool WriteFile(const std::filesystem::path& path, const std::string& contents) = 0;

    /**
     * @brief Check if file exists
     * @param path File path
     * @return true if file exists
     */
    virtual bool FileExists(const std::filesystem::path& path) = 0;

    /**
     * @brief Check if directory exists
     * @param path Directory path
     * @return true if directory exists
     */
    virtual bool DirectoryExists(const std::filesystem::path& path) = 0;

    /**
     * @brief Create directory (and parent directories if needed)
     * @param path Directory path
     * @return true on success, false on failure
     */
    virtual bool CreateDirectory(const std::filesystem::path& path) = 0;

    /**
     * @brief List files in directory matching pattern
     * @param directory Directory to search
     * @param extension File extension filter (e.g., ".md"), or empty for all files
     * @return Vector of file paths
     */
    virtual std::vector<std::filesystem::path> ListFiles(
        const std::filesystem::path& directory,
        const std::string& extension = ""
    ) = 0;

    /**
     * @brief Get user data directory path (for settings, progress, custom levels)
     * @return Path to user data directory (typically data/ in project root)
     */
    virtual std::filesystem::path GetUserDataDirectory() = 0;

    /**
     * @brief Get assets directory path (for levels, fonts, audio)
     * @return Path to assets directory (typically assets/ in project root)
     */
    virtual std::filesystem::path GetAssetsDirectory() = 0;
};

} // namespace wreckingball

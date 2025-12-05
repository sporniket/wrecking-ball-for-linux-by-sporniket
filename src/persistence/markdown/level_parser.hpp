// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../core/entities/level.hpp"
#include <string>
#include <optional>
#include <vector>

namespace wreckingball {

/**
 * @brief Parser for custom Markdown level files
 *
 * Level files use YAML front matter for metadata and fenced code blocks
 * for brick layout encoding.
 *
 * Format:
 * ---
 * id: 1
 * name: "Level Name"
 * type: Classical|ReachForStars|GetTheKey
 * difficulty: Easy|Medium|Hard
 * ---
 *
 * ```brick-layout
 * R---- R---- R---- R----
 * S-- K---- E----------
 * ```
 *
 * Brick codes: R=Regular, S=Star, K=Key, E=Exit, .=Empty
 * Dashes indicate width (R---- = 5-unit wide Regular brick)
 */
class LevelParser {
public:
    LevelParser();
    ~LevelParser();

    /**
     * @brief Load a level from a Markdown file
     * @param file_path Path to the .md level file
     * @return Loaded level, or std::nullopt if parsing failed
     */
    std::optional<Level> LoadLevel(const std::string& file_path);

    /**
     * @brief Save a level to a Markdown file
     * @param level Level to save
     * @param file_path Path to write the .md file
     * @return true if save succeeded, false otherwise
     */
    bool SaveLevel(const Level& level, const std::string& file_path);

    /**
     * @brief Validate level structure and rules
     * @param level Level to validate
     * @return true if level is valid, false otherwise
     */
    bool ValidateLevel(const Level& level);

    /**
     * @brief Get all available level files in a directory
     * @param directory_path Path to search for .md level files
     * @return List of level file paths
     */
    std::vector<std::string> GetAvailableLevels(const std::string& directory_path) const;

    /**
     * @brief Get last error message (if parsing failed)
     * @return Error description
     */
    std::string GetLastError() const { return last_error_; }

private:
    /**
     * @brief Parse YAML front matter
     * @param content File content
     * @return true if parsed successfully
     */
    bool ParseFrontMatter(const std::string& content, Level& level);

    /**
     * @brief Extract brick layout from fenced code block
     * @param content File content
     * @return Brick layout lines, or empty if not found
     */
    std::vector<std::string> ExtractBrickLayout(const std::string& content);

    /**
     * @brief Parse brick encoding from layout lines
     * @param lines Layout lines
     * @param level Level to populate with bricks
     * @return true if parsed successfully
     */
    bool ParseBrickEncoding(const std::vector<std::string>& lines, Level& level);

    /**
     * @brief Parse a single line of brick codes
     * @param line Layout line (e.g., "R---- S-- K---")
     * @param y_position Vertical position in grid
     * @param level Level to add bricks to
     * @return true if parsed successfully
     */
    bool ParseBrickLine(const std::string& line, int y_position, Level& level);

    /**
     * @brief Calculate brick width by counting dashes
     * @param encoded_brick String like "R----" or "S--"
     * @return Width in units
     */
    int CalculateBrickWidth(const std::string& encoded_brick) const;

    /**
     * @brief Parse brick type from code character
     * @param code Character code ('R', 'S', 'K', 'E', '.')
     * @return BrickType, or std::nullopt if invalid
     */
    std::optional<BrickType> ParseBrickType(char code) const;

    /**
     * @brief Convert BrickType to character code
     * @param type Brick type
     * @return Character code
     */
    char BrickTypeToCode(BrickType type) const;

    /**
     * @brief Validate level dimensions (38 units wide, 23 units tall)
     * @param level Level to validate
     * @return true if dimensions valid
     */
    bool ValidateDimensions(const Level& level) const;

    /**
     * @brief Validate level type requirements
     * - ReachForStars must have ≥1 star brick
     * - GetTheKey must have ≥1 key brick and ≥1 exit brick
     * @param level Level to validate
     * @return true if requirements met
     */
    bool ValidateLevelTypeRequirements(const Level& level);

    mutable std::string last_error_;
};

} // namespace wreckingball

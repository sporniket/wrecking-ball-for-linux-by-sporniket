// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "level_parser.hpp"
#include "../../core/logging/error_logger.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <cctype>

namespace wreckingball {

LevelParser::LevelParser() : last_error_("") {}

LevelParser::~LevelParser() {}

std::optional<Level> LevelParser::LoadLevel(const std::string& file_path) {
    last_error_.clear();

    // Read file content
    std::ifstream file(file_path);
    if (!file.is_open()) {
        last_error_ = "Failed to open file: " + file_path;
        ErrorLogger::GetInstance().LogError(last_error_);
        return std::nullopt;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    // Create level object
    Level level;

    // Parse YAML front matter
    if (!ParseFrontMatter(content, level)) {
        ErrorLogger::GetInstance().LogError("Failed to parse front matter: " + file_path);
        return std::nullopt;
    }

    // Extract brick layout
    std::vector<std::string> layout_lines = ExtractBrickLayout(content);
    if (layout_lines.empty()) {
        last_error_ = "No brick layout found in file";
        ErrorLogger::GetInstance().LogError(last_error_);
        return std::nullopt;
    }

    // Parse brick encoding
    if (!ParseBrickEncoding(layout_lines, level)) {
        ErrorLogger::GetInstance().LogError("Failed to parse brick encoding: " + file_path);
        return std::nullopt;
    }

    // Validate level
    if (!ValidateLevel(level)) {
        ErrorLogger::GetInstance().LogError("Level validation failed: " + file_path + " - " + last_error_);
        return std::nullopt;
    }

    ErrorLogger::GetInstance().LogInfo("Loaded level: " + level.name + " (ID: " + std::to_string(level.level_id) + ")");
    return level;
}

bool LevelParser::SaveLevel(const Level& level, const std::string& file_path) {
    std::ofstream file(file_path);
    if (!file.is_open()) {
        last_error_ = "Failed to open file for writing: " + file_path;
        ErrorLogger::GetInstance().LogError(last_error_);
        return false;
    }

    // Write YAML front matter
    file << "---\n";
    file << "id: " << level.level_id << "\n";
    file << "name: \"" << level.name << "\"\n";

    // Write level type
    file << "type: ";
    switch (level.level_type) {
        case LevelType::Classical: file << "Classical\n"; break;
        case LevelType::ReachForStars: file << "ReachForStars\n"; break;
        case LevelType::GetTheKey: file << "GetTheKey\n"; break;
    }

    file << "difficulty: \"" << level.difficulty_str << "\"\n";

    if (!level.author.empty()) {
        file << "author: \"" << level.author << "\"\n";
    }
    if (!level.description.empty()) {
        file << "description: \"" << level.description << "\"\n";
    }
    if (!level.version.empty()) {
        file << "version: \"" << level.version << "\"\n";
    }
    if (!level.tags.empty()) {
        file << "tags: \"" << level.tags << "\"\n";
    }

    file << "---\n\n";

    // Write brick layout
    file << "```brick-layout\n";

    // Build grid representation (38 units wide x 23 units tall)
    constexpr int GRID_WIDTH = 38;
    constexpr int GRID_HEIGHT = 23;

    // Create empty grid
    std::vector<std::vector<char>> grid(GRID_HEIGHT, std::vector<char>(GRID_WIDTH, '.'));

    // Place bricks in grid
    for (const auto& brick : level.bricks) {
        if (brick.is_destroyed) continue;  // Don't save destroyed bricks

        char code = BrickTypeToCode(brick.type);
        int start_x = static_cast<int>(brick.position_x);
        int y = static_cast<int>(brick.position_y);

        if (y >= 0 && y < GRID_HEIGHT) {
            for (int i = 0; i < brick.width && (start_x + i) < GRID_WIDTH; ++i) {
                if (start_x + i >= 0) {
                    grid[y][start_x + i] = code;
                }
            }
        }
    }

    // Write grid to file with run-length encoding
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        std::string line;
        int x = 0;

        while (x < GRID_WIDTH) {
            char current = grid[y][x];

            if (current == '.') {
                // Skip empty space
                x++;
                continue;
            }

            // Count consecutive same characters
            int count = 1;
            while (x + count < GRID_WIDTH && grid[y][x + count] == current) {
                count++;
            }

            // Write brick code with dashes for width
            line += current;
            for (int i = 1; i < count; ++i) {
                line += '-';
            }
            line += ' ';

            x += count;
        }

        // Only write non-empty lines
        if (!line.empty()) {
            // Trim trailing space
            if (line.back() == ' ') {
                line.pop_back();
            }
            file << line << "\n";
        }
    }

    file << "```\n";
    file.close();

    ErrorLogger::GetInstance().LogInfo("Saved level: " + level.name + " to " + file_path);
    return true;
}

bool LevelParser::ParseFrontMatter(const std::string& content, Level& level) {
    // Find YAML front matter delimiters
    size_t start = content.find("---");
    if (start == std::string::npos) {
        last_error_ = "No YAML front matter found";
        return false;
    }

    size_t end = content.find("---", start + 3);
    if (end == std::string::npos) {
        last_error_ = "Incomplete YAML front matter";
        return false;
    }

    std::string yaml = content.substr(start + 3, end - start - 3);
    std::istringstream stream(yaml);
    std::string line;

    while (std::getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        // Parse key-value pairs
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;

        std::string key = line.substr(0, colon);
        std::string value = line.substr(colon + 1);

        // Trim value
        value.erase(0, value.find_first_not_of(" \t\""));
        value.erase(value.find_last_not_of(" \t\"") + 1);

        // Set level properties
        if (key == "id" || key == "level_id") {
            level.level_id = std::stoi(value);
        } else if (key == "name") {
            level.name = value;
        } else if (key == "type") {
            if (value == "Classical") {
                level.level_type = LevelType::Classical;
            } else if (value == "ReachForStars") {
                level.level_type = LevelType::ReachForStars;
            } else if (value == "GetTheKey") {
                level.level_type = LevelType::GetTheKey;
            } else {
                last_error_ = "Invalid level type: " + value;
                return false;
            }
        } else if (key == "difficulty") {
            level.difficulty_str = value;
        } else if (key == "author") {
            level.author = value;
        } else if (key == "description") {
            level.description = value;
        } else if (key == "version") {
            level.version = value;
        } else if (key == "tags") {
            level.tags = value;
        }
    }

    return true;
}

std::vector<std::string> LevelParser::ExtractBrickLayout(const std::string& content) {
    std::vector<std::string> lines;

    // Find brick-layout fenced code block
    size_t start = content.find("```brick-layout");
    if (start == std::string::npos) {
        return lines;
    }

    size_t content_start = content.find('\n', start) + 1;
    size_t end = content.find("```", content_start);
    if (end == std::string::npos) {
        return lines;
    }

    std::string layout = content.substr(content_start, end - content_start);
    std::istringstream stream(layout);
    std::string line;

    while (std::getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    return lines;
}

bool LevelParser::ParseBrickEncoding(const std::vector<std::string>& lines, Level& level) {
    level.bricks.clear();

    for (size_t y = 0; y < lines.size(); ++y) {
        if (!ParseBrickLine(lines[y], static_cast<int>(y), level)) {
            return false;
        }
    }

    return true;
}

bool LevelParser::ParseBrickLine(const std::string& line, int y_position, Level& level) {
    int x_position = 0;
    size_t i = 0;

    while (i < line.length()) {
        char c = line[i];

        // Skip whitespace
        if (std::isspace(c)) {
            i++;
            continue;
        }

        // Parse brick type
        auto brick_type_opt = ParseBrickType(c);
        if (!brick_type_opt) {
            last_error_ = "Invalid brick code: " + std::string(1, c);
            return false;
        }

        BrickType brick_type = *brick_type_opt;

        // Count dashes to determine width
        int width = 1;
        i++;
        while (i < line.length() && line[i] == '-') {
            width++;
            i++;
        }

        // Skip empty spaces (dots represent empty areas, not bricks)
        if (brick_type == BrickType::Empty) {
            x_position += width;
            continue;
        }

        // Create brick
        Brick brick;
        brick.type = brick_type;
        // Offset position to account for borders: x+1 (left border), y+2 (top border + HUD)
        brick.position_x = x_position + 1;
        brick.position_y = y_position + 2;
        brick.width = width;
        brick.hits_remaining = 1;  // Default 1 hit
        brick.is_destroyed = false;
        brick.is_active = (brick_type != BrickType::Exit);  // Exits start inactive

        // Set color based on brick type (EGA palette indices)
        switch (brick_type) {
            case BrickType::Regular:
                brick.color_index = 4;  // Red
                break;
            case BrickType::Star:
                brick.color_index = 14;  // Yellow
                break;
            case BrickType::Key:
                brick.color_index = 11;  // Light Cyan
                break;
            case BrickType::Exit:
                brick.color_index = 10;  // Light Green
                break;
            default:
                brick.color_index = 7;  // Light Gray
                break;
        }

        level.bricks.push_back(brick);

        x_position += width;
    }

    return true;
}

int LevelParser::CalculateBrickWidth(const std::string& encoded_brick) const {
    if (encoded_brick.empty()) return 0;

    int width = 1;  // First character is the brick code
    for (size_t i = 1; i < encoded_brick.length(); ++i) {
        if (encoded_brick[i] == '-') {
            width++;
        } else {
            break;
        }
    }

    return width;
}

std::optional<BrickType> LevelParser::ParseBrickType(char code) const {
    switch (code) {
        case 'R': return BrickType::Regular;
        case 'S': return BrickType::Star;
        case 'K': return BrickType::Key;
        case 'E': return BrickType::Exit;
        case '.': return BrickType::Empty;
        default: return std::nullopt;
    }
}

char LevelParser::BrickTypeToCode(BrickType type) const {
    switch (type) {
        case BrickType::Regular: return 'R';
        case BrickType::Star: return 'S';
        case BrickType::Key: return 'K';
        case BrickType::Exit: return 'E';
        case BrickType::Empty: return '.';
        default: return '?';
    }
}

bool LevelParser::ValidateLevel(const Level& level) {
    // Validate dimensions
    if (!ValidateDimensions(level)) {
        return false;
    }

    // Validate level type requirements
    if (!ValidateLevelTypeRequirements(level)) {
        return false;
    }

    return true;
}

bool LevelParser::ValidateDimensions(const Level& level) const {
    // Playfield dimensions accounting for borders:
    // Screen: 40 units wide × 25 units tall
    // Left border at x=0, right border at x=39
    // Top border + HUD at y=0-1
    // Brick area: x=1 to x=38, y=2 to y=24
    constexpr int MIN_BRICK_X = 1;      // After left border
    constexpr int MAX_BRICK_X_END = 39;  // Before right border
    constexpr int MIN_BRICK_Y = 2;      // After top border + HUD
    constexpr int MAX_BRICK_Y = 24;     // Bottom of playfield

    for (const auto& brick : level.bricks) {
        // Check minimum bounds
        if (brick.position_x < MIN_BRICK_X) {
            last_error_ = "Brick position_x (" + std::to_string(brick.position_x) +
                         ") is before playfield start (min:" + std::to_string(MIN_BRICK_X) + ")";
            return false;
        }
        if (brick.position_y < MIN_BRICK_Y) {
            last_error_ = "Brick position_y (" + std::to_string(brick.position_y) +
                         ") is before playfield start (min:" + std::to_string(MIN_BRICK_Y) + ")";
            return false;
        }

        // Check maximum bounds
        if (brick.position_x + brick.width > MAX_BRICK_X_END) {
            last_error_ = "Brick extends beyond playfield (x:" + std::to_string(brick.position_x) +
                         " + width:" + std::to_string(brick.width) + " = " +
                         std::to_string(brick.position_x + brick.width) +
                         " > max:" + std::to_string(MAX_BRICK_X_END) + ")";
            return false;
        }
        if (brick.position_y > MAX_BRICK_Y) {
            last_error_ = "Brick position_y (" + std::to_string(brick.position_y) +
                         ") exceeds playfield height (max:" + std::to_string(MAX_BRICK_Y) + ")";
            return false;
        }
    }

    return true;
}

bool LevelParser::ValidateLevelTypeRequirements(const Level& level) {
    int star_count = 0;
    int key_count = 0;
    int exit_count = 0;

    for (const auto& brick : level.bricks) {
        if (brick.type == BrickType::Star) star_count++;
        if (brick.type == BrickType::Key) key_count++;
        if (brick.type == BrickType::Exit) exit_count++;
    }

    switch (level.level_type) {
        case LevelType::Classical:
            // No special requirements
            return true;

        case LevelType::ReachForStars:
            if (star_count < 1) {
                last_error_ = "ReachForStars level must have at least 1 star brick";
                return false;
            }
            return true;

        case LevelType::GetTheKey:
            if (key_count < 1) {
                last_error_ = "GetTheKey level must have at least 1 key brick";
                return false;
            }
            if (exit_count < 1) {
                last_error_ = "GetTheKey level must have at least 1 exit brick";
                return false;
            }
            return true;

        default:
            return false;
    }
}

std::vector<std::string> LevelParser::GetAvailableLevels(const std::string& directory_path) const {
    std::vector<std::string> level_files;

    try {
        if (!std::filesystem::exists(directory_path)) {
            ErrorLogger::GetInstance().LogWarning("Level directory does not exist: " + directory_path);
            return level_files;
        }

        for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".md") {
                level_files.push_back(entry.path().string());
            }
        }

        // Sort by filename
        std::sort(level_files.begin(), level_files.end());

    } catch (const std::exception& e) {
        ErrorLogger::GetInstance().LogError("Failed to scan level directory: " + std::string(e.what()));
    }

    return level_files;
}

} // namespace wreckingball

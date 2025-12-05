// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "brick.hpp"
#include "level_type.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace wreckingball {

/**
 * @brief Represents a playable stage with brick layout and completion rules
 *
 * Levels can be Classical (break all bricks), ReachForStars (break all stars),
 * or GetTheKey (collect keys, then break exit).
 */
struct Level {
    uint16_t level_id;           // Unique level identifier (1-9999)
    std::string name;            // Human-readable level name (max 64 chars)
    LevelType level_type;        // Level completion type
    std::string author;          // Level creator or "Built-in"
    std::vector<Brick> bricks;   // Collection of bricks in level (max 874)
    bool is_locked;              // Whether level is unlocked for play
    bool is_custom;              // Whether level is user-created
    uint8_t difficulty;          // Difficulty rating (1-5)
    std::string difficulty_str;  // Difficulty as string ("Easy", "Medium", "Hard")
    std::string description;     // Level description (optional)
    std::string version;         // Level version string (optional)
    std::string tags;            // Comma-separated tags (optional)

    /**
     * @brief Default constructor
     */
    Level()
        : level_id(0), name("Untitled"), level_type(LevelType::Classical),
          author("Built-in"), is_locked(true), is_custom(false), difficulty(1),
          difficulty_str("Easy"), description(""), version("1.0"), tags("") {}

    /**
     * @brief Validate level has required brick types for its level_type
     * @return true if level is valid
     */
    bool IsValid() const {
        if (bricks.empty()) {
            return false;  // Cannot have empty level
        }

        // Count brick types
        int star_count = 0;
        int key_count = 0;
        int exit_count = 0;

        for (const auto& brick : bricks) {
            if (!brick.IsValid()) {
                return false;  // All bricks must be valid
            }
            if (brick.type == BrickType::Star) star_count++;
            if (brick.type == BrickType::Key) key_count++;
            if (brick.type == BrickType::Exit) exit_count++;
        }

        // Validate level type requirements
        switch (level_type) {
            case LevelType::Classical:
                // No specific requirements, just needs bricks
                return true;

            case LevelType::ReachForStars:
                // Must have at least one star brick
                return star_count > 0;

            case LevelType::GetTheKey:
                // Must have at least one key and one exit
                return key_count > 0 && exit_count > 0;
        }

        return false;
    }

    /**
     * @brief Count bricks of a specific type
     * @param type Brick type to count
     * @return Number of bricks of that type
     */
    int CountBricksOfType(BrickType type) const {
        int count = 0;
        for (const auto& brick : bricks) {
            if (brick.type == type && !brick.is_destroyed) {
                count++;
            }
        }
        return count;
    }

    /**
     * @brief Count total non-empty, non-destroyed bricks
     * @return Number of active bricks remaining
     */
    int CountRemainingBricks() const {
        int count = 0;
        for (const auto& brick : bricks) {
            if (brick.type != BrickType::Empty && !brick.is_destroyed) {
                count++;
            }
        }
        return count;
    }
};

} // namespace wreckingball

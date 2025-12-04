// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "level_type.hpp"
#include <cstdint>
#include <set>
#include <string>

namespace wreckingball {

/**
 * @brief Represents persistent player data across game sessions
 *
 * Tracks unlocked levels, completion counts, statistics, and achievements.
 * Serialized to INI file for persistence.
 */
struct PlayerProgress {
    std::set<uint16_t> unlocked_levels;        // Set of unlocked level IDs
    uint16_t completed_levels_classical;       // Count of Classical levels completed
    uint16_t completed_levels_star;            // Count of ReachForStars levels completed
    uint16_t completed_levels_key;             // Count of GetTheKey levels completed
    uint32_t highest_score;                    // All-time highest score
    uint64_t total_bricks_destroyed;           // Lifetime brick destruction count
    uint32_t total_stars_collected;            // Lifetime star brick count
    uint32_t total_keys_collected;             // Lifetime key brick count
    std::set<std::string> earned_achievements; // Set of earned achievement IDs

    /**
     * @brief Default constructor
     */
    PlayerProgress()
        : completed_levels_classical(0), completed_levels_star(0),
          completed_levels_key(0), highest_score(0),
          total_bricks_destroyed(0), total_stars_collected(0),
          total_keys_collected(0) {
        // Level 1 is always unlocked
        unlocked_levels.insert(1);
    }

    /**
     * @brief Unlock a level by ID
     * @param level_id Level to unlock
     */
    void UnlockLevel(uint16_t level_id) {
        unlocked_levels.insert(level_id);
    }

    /**
     * @brief Check if level is unlocked
     * @param level_id Level to check
     * @return true if level is unlocked
     */
    bool IsLevelUnlocked(uint16_t level_id) const {
        return unlocked_levels.find(level_id) != unlocked_levels.end();
    }

    /**
     * @brief Record level completion
     * @param level_type Type of level completed
     */
    void RecordLevelCompletion(LevelType level_type) {
        switch (level_type) {
            case LevelType::Classical:
                completed_levels_classical++;
                break;
            case LevelType::ReachForStars:
                completed_levels_star++;
                break;
            case LevelType::GetTheKey:
                completed_levels_key++;
                break;
        }
    }

    /**
     * @brief Update highest score if new score is higher
     * @param score Score to compare
     * @return true if new high score set
     */
    bool UpdateHighScore(uint32_t score) {
        if (score > highest_score) {
            highest_score = score;
            return true;
        }
        return false;
    }

    /**
     * @brief Add statistics for bricks destroyed
     * @param brick_count Number of bricks destroyed
     * @param star_count Number of star bricks destroyed
     * @param key_count Number of key bricks destroyed
     */
    void AddStats(uint32_t brick_count, uint32_t star_count, uint32_t key_count) {
        total_bricks_destroyed += brick_count;
        total_stars_collected += star_count;
        total_keys_collected += key_count;
    }

    /**
     * @brief Earn an achievement
     * @param achievement_id Achievement ID to earn
     * @return true if achievement was newly earned (not already earned)
     */
    bool EarnAchievement(const std::string& achievement_id) {
        return earned_achievements.insert(achievement_id).second;
    }

    /**
     * @brief Check if achievement is earned
     * @param achievement_id Achievement ID to check
     * @return true if achievement is earned
     */
    bool HasAchievement(const std::string& achievement_id) const {
        return earned_achievements.find(achievement_id) != earned_achievements.end();
    }

    /**
     * @brief Get total completed levels across all types
     * @return Total completed level count
     */
    uint32_t GetTotalCompletedLevels() const {
        return static_cast<uint32_t>(completed_levels_classical) +
               static_cast<uint32_t>(completed_levels_star) +
               static_cast<uint32_t>(completed_levels_key);
    }
};

} // namespace wreckingball

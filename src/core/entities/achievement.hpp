// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace wreckingball {

// Forward declaration to avoid circular dependency
struct PlayerProgress;

/**
 * @brief Function type for achievement unlock criteria
 *
 * Takes PlayerProgress and returns true if achievement should be earned
 */
using UnlockCriteriaFunc = std::function<bool(const PlayerProgress&)>;

/**
 * @brief Represents a trophy/badge with unlock criteria
 *
 * Achievements are checked against PlayerProgress to determine if earned.
 * Achievement definitions are hardcoded in game logic.
 */
struct Achievement {
    std::string achievement_id;           // Unique achievement identifier (max 32 chars, snake_case)
    std::string name;                     // Display name (max 64 chars)
    std::string description;              // How to unlock (max 128 chars)
    bool is_earned;                       // Whether player has earned it
    UnlockCriteriaFunc unlock_criteria;   // Function to check if earned

    /**
     * @brief Default constructor
     */
    Achievement()
        : achievement_id(""), name(""), description(""),
          is_earned(false), unlock_criteria(nullptr) {}

    /**
     * @brief Constructor with all fields
     * @param id Achievement ID (snake_case)
     * @param display_name Display name
     * @param desc Description of how to unlock
     * @param criteria Function to check unlock condition
     */
    Achievement(const std::string& id, const std::string& display_name,
                const std::string& desc, UnlockCriteriaFunc criteria)
        : achievement_id(id), name(display_name), description(desc),
          is_earned(false), unlock_criteria(criteria) {}

    /**
     * @brief Check if achievement should be earned based on player progress
     * @param progress Player's current progress
     * @return true if unlock criteria met
     */
    bool CheckUnlock(const PlayerProgress& progress) const {
        if (!unlock_criteria) return false;
        return unlock_criteria(progress);
    }

    /**
     * @brief Validate achievement data
     * @return true if achievement is valid
     */
    bool IsValid() const {
        return !achievement_id.empty() &&
               achievement_id.length() <= 32 &&
               !name.empty() &&
               name.length() <= 64 &&
               description.length() <= 128 &&
               unlock_criteria != nullptr;
    }
};

} // namespace wreckingball

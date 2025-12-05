// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../entities/level.hpp"
#include "../entities/brick.hpp"
#include <string>

namespace wreckingball {

/**
 * @brief Checks level completion for all three level types
 *
 * Completion criteria:
 * - Classical: All bricks destroyed
 * - ReachForStars: All star bricks destroyed
 * - GetTheKey: All key bricks destroyed AND at least one exit brick destroyed
 */
class LevelCompletionChecker {
public:
    LevelCompletionChecker();
    ~LevelCompletionChecker();

    /**
     * @brief Check if level is complete
     * @param level Level to check
     * @return true if level completion criteria met
     */
    bool IsLevelComplete(const Level& level);

    /**
     * @brief Update exit brick activation for GetTheKey levels
     * Call this after destroying any key brick
     * @param level Level to update
     */
    void UpdateExitBrickActivation(Level& level);

    /**
     * @brief Get completion progress string for HUD display
     * @param level Level to check
     * @return Progress string (e.g., "3/5 stars", "All keys collected")
     */
    std::string GetCompletionProgress(const Level& level);

private:
    /**
     * @brief Check Classical level completion (all bricks destroyed)
     * @param level Level to check
     * @return true if complete
     */
    bool IsClassicalLevelComplete(const Level& level);

    /**
     * @brief Check ReachForStars level completion (all stars destroyed)
     * @param level Level to check
     * @return true if complete
     */
    bool IsStarLevelComplete(const Level& level);

    /**
     * @brief Check GetTheKey level completion (all keys + any exit destroyed)
     * @param level Level to check
     * @return true if complete
     */
    bool IsKeyLevelComplete(const Level& level);

    /**
     * @brief Check if all key bricks are destroyed
     * @param level Level to check
     * @return true if all keys destroyed
     */
    bool AreAllKeysDestroyed(const Level& level);

    /**
     * @brief Check if any active exit brick is destroyed
     * @param level Level to check
     * @return true if at least one active exit destroyed
     */
    bool IsAnyActiveExitDestroyed(const Level& level);

    /**
     * @brief Count total bricks (excluding Empty type)
     * @param level Level to check
     * @return Total brick count
     */
    int CountTotalBricks(const Level& level);

    /**
     * @brief Count destroyed bricks
     * @param level Level to check
     * @return Destroyed brick count
     */
    int CountDestroyedBricks(const Level& level);

    /**
     * @brief Count star bricks
     * @param level Level to check
     * @return Star brick count
     */
    int CountStarBricks(const Level& level);

    /**
     * @brief Count destroyed star bricks
     * @param level Level to check
     * @return Destroyed star brick count
     */
    int CountDestroyedStars(const Level& level);

    /**
     * @brief Count key bricks
     * @param level Level to check
     * @return Key brick count
     */
    int CountKeyBricks(const Level& level);

    /**
     * @brief Count destroyed key bricks
     * @param level Level to check
     * @return Destroyed key brick count
     */
    int CountDestroyedKeys(const Level& level);
};

} // namespace wreckingball

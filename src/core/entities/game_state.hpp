// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "ball.hpp"
#include "game_mode.hpp"
#include "level.hpp"
#include "paddle.hpp"
#include <cstdint>
#include <vector>

namespace wreckingball {

/**
 * @brief Represents the current gameplay session state
 *
 * Manages active level, score, lives, balls, paddle, and completion tracking.
 * Separate instances for Casual vs Arcade modes.
 */
struct GameState {
    Level* current_level;        // Pointer to active level (non-null during gameplay)
    GameMode game_mode;          // Casual or Arcade mode
    uint32_t score;              // Current score (0 to 4,294,967,295)
    uint8_t lives_remaining;     // Remaining lives (Arcade: 3-7, Casual: 255=infinite)
    std::vector<Ball> balls;     // Active balls in play
    Paddle paddle;               // Player paddle
    uint8_t keys_collected;      // Key bricks destroyed (GetTheKey levels)
    uint8_t keys_total;          // Total key bricks in level (cached)
    uint8_t stars_collected;     // Star bricks destroyed (ReachForStars levels)
    uint8_t stars_total;         // Total star bricks in level (cached)
    bool is_paused;              // Whether game is paused
    bool level_completed;        // Whether current level is complete

    /**
     * @brief Default constructor
     */
    GameState()
        : current_level(nullptr), game_mode(GameMode::Casual),
          score(0), lives_remaining(255),  // Casual: infinite lives
          keys_collected(0), keys_total(0),
          stars_collected(0), stars_total(0),
          is_paused(false), level_completed(false) {
        // Initialize with one ball
        balls.push_back(Ball());
    }

    /**
     * @brief Initialize game state for a new level
     * @param level Pointer to level to load
     * @param mode Game mode (Casual or Arcade)
     * @param initial_lives Starting lives for Arcade mode (3, 5, or 7)
     */
    void StartLevel(Level* level, GameMode mode, uint8_t initial_lives = 3) {
        current_level = level;
        game_mode = mode;
        score = 0;
        lives_remaining = (mode == GameMode::Casual) ? 255 : initial_lives;
        is_paused = false;
        level_completed = false;

        // Cache level totals
        if (level) {
            keys_total = level->CountBricksOfType(BrickType::Key);
            stars_total = level->CountBricksOfType(BrickType::Star);
        } else {
            keys_total = 0;
            stars_total = 0;
        }
        keys_collected = 0;
        stars_collected = 0;

        // Reset paddle and ball
        paddle = Paddle();
        balls.clear();
        Ball initial_ball;
        initial_ball.AttachToPaddle(paddle.position_x, paddle.position_y);
        balls.push_back(initial_ball);
    }

    /**
     * @brief Check if level completion condition is met
     * @return true if level is complete
     */
    bool CheckLevelComplete() {
        if (!current_level) return false;

        switch (current_level->level_type) {
            case LevelType::Classical:
                // All non-empty bricks destroyed
                return current_level->CountRemainingBricks() == 0;

            case LevelType::ReachForStars:
                // All star bricks destroyed
                return stars_collected >= stars_total;

            case LevelType::GetTheKey:
                // All keys collected and exit brick destroyed
                if (keys_collected >= keys_total) {
                    // Check if exit brick is destroyed
                    return current_level->CountBricksOfType(BrickType::Exit) == 0;
                }
                return false;
        }

        return false;
    }

    /**
     * @brief Check if game is over (Arcade mode: lives = 0)
     * @return true if game over
     */
    bool IsGameOver() const {
        return game_mode == GameMode::Arcade && lives_remaining == 0;
    }
};

} // namespace wreckingball

// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../entities/game_state.hpp"
#include "../entities/level.hpp"
#include "../entities/game_mode.hpp"
#include "../entities/game_screen.hpp"
#include "../physics/physics_engine.hpp"
#include "../level/completion_checker.hpp"
#include <memory>
#include <string>

namespace wreckingball {

/**
 * @brief Manages overall game state and orchestrates gameplay systems
 *
 * Responsibilities:
 * - Load and manage current level
 * - Update game state (paddle, balls, physics)
 * - Handle collisions and scoring
 * - Detect level completion
 * - Manage ball loss and respawn (Casual mode)
 * - Handle pause/resume
 * - Screen transitions
 */
class GameStateManager {
public:
    GameStateManager();
    ~GameStateManager();

    /**
     * @brief Start a level
     * @param level Level to start
     * @param mode Game mode (Casual or Arcade)
     * @return true if level started successfully
     */
    bool StartLevel(const Level& level, GameMode mode);

    /**
     * @brief Update game state (physics, collisions, completion check)
     * @param delta_time Time step in seconds
     */
    void Update(double delta_time);

    /**
     * @brief Update paddle position based on input
     * @param paddle_x Target paddle X position
     * @param screen_width Screen width for boundary checking
     */
    void UpdatePaddlePosition(double paddle_x, int screen_width);

    /**
     * @brief Launch ball from paddle
     */
    void LaunchBall();

    /**
     * @brief Handle brick destruction (update score, check for special bricks)
     * @param brick Brick that was destroyed
     */
    void OnBrickDestroyed(const Brick& brick);

    /**
     * @brief Handle ball loss (respawn or lose life)
     */
    void OnBallLost();

    /**
     * @brief Handle level completion
     */
    void OnLevelComplete();

    /**
     * @brief Toggle pause state
     */
    void TogglePause();

    /**
     * @brief Set current screen
     * @param screen Screen to switch to
     */
    void SetScreen(GameScreen screen);

    /**
     * @brief Get current screen
     * @return Current screen
     */
    GameScreen GetCurrentScreen() const { return current_screen_; }

    /**
     * @brief Get current game state
     * @return Game state reference
     */
    const GameState& GetGameState() const { return game_state_; }

    /**
     * @brief Get mutable game state (for rendering)
     * @return Game state reference
     */
    GameState& GetGameState() { return game_state_; }

    /**
     * @brief Check if game is paused
     * @return true if paused
     */
    bool IsPaused() const { return is_paused_; }

    /**
     * @brief Get completion progress string
     * @return Progress string for HUD
     */
    std::string GetCompletionProgress();

private:
    /**
     * @brief Initialize paddle for current level
     */
    void InitializePaddle();

    /**
     * @brief Initialize ball attached to paddle
     */
    void InitializeBall();

    /**
     * @brief Respawn ball on paddle (Casual mode)
     */
    void RespawnBall();

    /**
     * @brief Update score based on brick type
     * @param brick Brick that was destroyed
     */
    void UpdateScore(const Brick& brick);

    /**
     * @brief Check all balls for loss
     */
    void CheckBallsLost();

    /**
     * @brief Check level completion
     */
    void CheckLevelCompletion();

    GameState game_state_;
    PhysicsEngine physics_engine_;
    LevelCompletionChecker completion_checker_;

    GameScreen current_screen_;
    bool is_paused_;

    // Game constants (in unit coordinates)
    static constexpr double BALL_SPEED = 18.75;  // Units per second (150 pixels/s ÷ 8)
    static constexpr double PADDLE_SPEED = 25.0;  // Units per second (200 pixels/s ÷ 8)
    static constexpr int GRID_WIDTH_UNITS = 40;
    static constexpr int GRID_HEIGHT_UNITS = 25;
};

} // namespace wreckingball

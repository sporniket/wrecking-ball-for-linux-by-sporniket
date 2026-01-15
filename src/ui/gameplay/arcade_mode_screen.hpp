// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include "../../platform/input/input_interface.hpp"
#include "../../core/game/game_state_manager.hpp"
#include "gameplay_screen.hpp"
#include <memory>
#include <string>

namespace wreckingball {

/**
 * @brief Arcade mode gameplay screen
 *
 * Extends gameplay with limited lives (3/5/7), life deduction on ball loss,
 * game over screen, and sequential level progression.
 */
class ArcadeModeScreen {
public:
    /**
     * @brief Constructor
     * @param renderer Renderer for drawing
     * @param input_handler Input handler for controls
     * @param game_manager Game state manager
     */
    ArcadeModeScreen(IRenderer* renderer, IInputHandler* input_handler, GameStateManager* game_manager);

    /**
     * @brief Destructor
     */
    ~ArcadeModeScreen();

    // Prevent copying
    ArcadeModeScreen(const ArcadeModeScreen&) = delete;
    ArcadeModeScreen& operator=(const ArcadeModeScreen&) = delete;

    /**
     * @brief Initialize arcade mode
     * @param initial_level_id Starting level ID (default: 1)
     * @param lives Number of lives (3, 5, or 7)
     * @return true if initialization successful
     */
    bool Initialize(int initial_level_id = 1, uint8_t lives = 3);

    /**
     * @brief Update arcade mode logic
     * @param delta_time Time elapsed since last frame (seconds)
     */
    void Update(double delta_time);

    /**
     * @brief Render arcade mode screen
     */
    void Render();

    /**
     * @brief Handle input
     */
    void HandleInput();

    /**
     * @brief Check if screen should exit to menu
     * @return true if user requested back to menu
     */
    bool ShouldExit() const { return should_exit_; }

    /**
     * @brief Check if in life selection mode
     * @return true if showing life selection screen
     */
    bool IsInLifeSelection() const { return in_life_selection_; }

    /**
     * @brief Get highest level reached in current arcade session
     * @return Highest level number reached
     */
    int GetHighestLevelReached() const { return highest_level_reached_; }

    /**
     * @brief Show life selection screen (3/5/7 options)
     */
    void ShowLifeSelection();

    /**
     * @brief Reset arcade mode state (for starting fresh)
     * Resets level progress, scores, and returns to life selection
     */
    void Reset();

    /**
     * @brief Handle level completion (progress to next level)
     */
    void OnLevelComplete();

private:
    /**
     * @brief Render lives indicator in HUD
     */
    void RenderLivesDisplay();

    /**
     * @brief Render life selection screen
     */
    void RenderLifeSelection();

    /**
     * @brief Handle input during life selection
     */
    void HandleLifeSelectionInput();

    /**
     * @brief Load level by ID
     * @param level_id Level ID to load
     * @return true if level loaded successfully
     */
    bool LoadLevel(int level_id);

    IRenderer* renderer_;
    IInputHandler* input_handler_;
    GameStateManager* game_manager_;
    std::unique_ptr<GameplayScreen> gameplay_screen_;

    bool should_exit_;
    bool in_life_selection_;
    int selected_lives_option_;  // 0=3 lives, 1=5 lives, 2=7 lives
    int current_level_id_;
    int highest_level_reached_;
    uint32_t final_score_;

    // Input debouncing for life selection
    double last_navigation_time_;
    static constexpr double NAVIGATION_DEBOUNCE_DELAY = 0.3;  // 0.3 seconds
};

} // namespace wreckingball

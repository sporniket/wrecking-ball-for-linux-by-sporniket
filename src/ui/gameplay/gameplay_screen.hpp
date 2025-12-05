// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include "../../platform/input/input_interface.hpp"
#include "../../core/game/game_state_manager.hpp"
#include <memory>

namespace wreckingball {

/**
 * @brief Main gameplay screen
 *
 * Renders the play field, bricks, ball, paddle, and HUD.
 * Handles input during gameplay and pause menu.
 */
class GameplayScreen {
public:
    /**
     * @brief Constructor
     * @param renderer Renderer for drawing
     * @param input_handler Input handler for controls
     * @param game_manager Game state manager
     */
    GameplayScreen(IRenderer* renderer, IInputHandler* input_handler, GameStateManager* game_manager);

    /**
     * @brief Destructor
     */
    ~GameplayScreen();

    // Prevent copying
    GameplayScreen(const GameplayScreen&) = delete;
    GameplayScreen& operator=(const GameplayScreen&) = delete;

    /**
     * @brief Update gameplay logic
     * @param delta_time Time elapsed since last frame (seconds)
     */
    void Update(double delta_time);

    /**
     * @brief Render gameplay screen
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

private:
    /**
     * @brief Render play field borders
     */
    void RenderPlayField();

    /**
     * @brief Render all bricks
     */
    void RenderBricks();

    /**
     * @brief Render ball
     */
    void RenderBall();

    /**
     * @brief Render paddle
     */
    void RenderPaddle();

    /**
     * @brief Render HUD (score, level info, progress)
     */
    void RenderHUD();

    /**
     * @brief Render pause menu overlay
     */
    void RenderPauseMenu();

    /**
     * @brief Get color for brick rendering
     * @param brick Brick to get color for
     * @return Brick color
     */
    Color GetBrickColor(const Brick& brick) const;

    IRenderer* renderer_;
    IInputHandler* input_handler_;
    GameStateManager* game_manager_;
    bool should_exit_;
    bool is_paused_;
};

} // namespace wreckingball

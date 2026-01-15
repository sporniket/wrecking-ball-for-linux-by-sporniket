// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include "../../platform/input/input_interface.hpp"
#include "../../core/game/game_state_manager.hpp"
#include "gameplay_screen.hpp"
#include <memory>

namespace wreckingball {

/**
 * @brief Casual mode gameplay screen
 *
 * Wraps GameplayScreen with casual mode behavior:
 * - Infinite lives (ball respawns immediately on loss)
 * - Single level play
 * - No progression or game over
 */
class CasualModeScreen {
public:
    /**
     * @brief Constructor
     * @param renderer Renderer for drawing
     * @param input_handler Input handler for controls
     * @param game_manager Game state manager
     */
    CasualModeScreen(IRenderer* renderer, IInputHandler* input_handler, GameStateManager* game_manager);

    /**
     * @brief Destructor
     */
    ~CasualModeScreen();

    // Prevent copying
    CasualModeScreen(const CasualModeScreen&) = delete;
    CasualModeScreen& operator=(const CasualModeScreen&) = delete;

    /**
     * @brief Initialize casual mode with a level
     * @param level Level to play
     * @return true if initialization successful
     */
    bool Initialize(const Level& level);

    /**
     * @brief Update casual mode logic
     * @param delta_time Time elapsed since last frame (seconds)
     */
    void Update(double delta_time);

    /**
     * @brief Render casual mode screen
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
    IRenderer* renderer_;
    IInputHandler* input_handler_;
    GameStateManager* game_manager_;
    std::unique_ptr<GameplayScreen> gameplay_screen_;

    bool should_exit_;
};

} // namespace wreckingball

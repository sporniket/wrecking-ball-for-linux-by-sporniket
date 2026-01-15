// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include "../../platform/input/input_interface.hpp"
#include <cstdint>

namespace wreckingball {

/**
 * @brief Game Over screen for Arcade mode
 *
 * Displays final score, highest level reached, and allows returning to main menu.
 */
class GameOverScreen {
public:
    /**
     * @brief Constructor
     * @param renderer Renderer for drawing
     * @param input_handler Input handler for controls
     */
    GameOverScreen(IRenderer* renderer, IInputHandler* input_handler);

    /**
     * @brief Destructor
     */
    ~GameOverScreen();

    // Prevent copying
    GameOverScreen(const GameOverScreen&) = delete;
    GameOverScreen& operator=(const GameOverScreen&) = delete;

    /**
     * @brief Set game over stats to display
     * @param final_score Final score achieved
     * @param highest_level Highest level reached
     */
    void SetStats(uint32_t final_score, int highest_level);

    /**
     * @brief Render game over screen
     */
    void Render();

    /**
     * @brief Handle input (wait for confirmation to return to menu)
     */
    void HandleInput();

    /**
     * @brief Check if user wants to return to menu
     * @return true if user confirmed to exit
     */
    bool ShouldExit() const { return should_exit_; }

private:
    /**
     * @brief Render final score display
     */
    void RenderScore();

    /**
     * @brief Render highest level reached display
     */
    void RenderHighestLevel();

    /**
     * @brief Render instructions ("Press SPACE to continue")
     */
    void RenderInstructions();

    IRenderer* renderer_;
    IInputHandler* input_handler_;
    uint32_t final_score_;
    int highest_level_;
    bool should_exit_;
};

} // namespace wreckingball

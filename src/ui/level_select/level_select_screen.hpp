// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include "../../platform/input/input_interface.hpp"

namespace wreckingball {

/**
 * @brief Level selection screen (placeholder for Phase 6)
 *
 * Will display available levels with locked/unlocked states.
 * Currently shows placeholder text and back-to-menu option.
 */
class LevelSelectScreen {
public:
    /**
     * @brief Constructor
     * @param renderer Renderer for drawing
     * @param input_handler Input handler for controls
     */
    LevelSelectScreen(IRenderer* renderer, IInputHandler* input_handler);

    /**
     * @brief Destructor
     */
    ~LevelSelectScreen();

    // Prevent copying
    LevelSelectScreen(const LevelSelectScreen&) = delete;
    LevelSelectScreen& operator=(const LevelSelectScreen&) = delete;

    /**
     * @brief Update screen logic
     * @param delta_time Time elapsed since last frame (seconds)
     */
    void Update(double delta_time);

    /**
     * @brief Render screen
     */
    void Render();

    /**
     * @brief Handle input
     */
    void HandleInput();

    /**
     * @brief Check if should return to main menu
     * @return true if user wants to go back
     */
    bool ShouldReturnToMenu() const { return should_return_to_menu_; }

private:
    IRenderer* renderer_;
    IInputHandler* input_handler_;
    bool should_return_to_menu_;
    bool was_back_pressed_;
};

} // namespace wreckingball

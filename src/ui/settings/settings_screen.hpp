// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include "../../platform/input/input_interface.hpp"

namespace wreckingball {

/**
 * @brief Settings screen (placeholder for Phase 8)
 *
 * Will display configurable settings (audio, controls, display).
 * Currently shows placeholder text and back-to-menu option.
 */
class SettingsScreen {
public:
    /**
     * @brief Constructor
     * @param renderer Renderer for drawing
     * @param input_handler Input handler for controls
     */
    SettingsScreen(IRenderer* renderer, IInputHandler* input_handler);

    /**
     * @brief Destructor
     */
    ~SettingsScreen();

    // Prevent copying
    SettingsScreen(const SettingsScreen&) = delete;
    SettingsScreen& operator=(const SettingsScreen&) = delete;

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

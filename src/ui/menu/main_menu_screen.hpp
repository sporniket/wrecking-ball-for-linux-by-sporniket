// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include "../../platform/input/input_interface.hpp"
#include "../../core/entities/game_screen.hpp"
#include <vector>
#include <string>
#include <functional>

namespace wreckingball {

/**
 * @brief Menu option definition
 */
struct MenuOption {
    std::string label;
    GameScreen target_screen;
    std::function<void()> action;  // Optional custom action (e.g., exit game)
};

/**
 * @brief Main menu screen
 *
 * Central hub for navigation to all game screens.
 * Provides options for:
 * - Play (Casual/Arcade)
 * - Level Selection
 * - Achievements
 * - Settings
 * - Level Editor
 * - My Other Games
 * - Exit
 */
class MainMenuScreen {
public:
    /**
     * @brief Constructor
     * @param renderer Renderer for drawing
     * @param input_handler Input handler for controls
     */
    MainMenuScreen(IRenderer* renderer, IInputHandler* input_handler);

    /**
     * @brief Destructor
     */
    ~MainMenuScreen();

    // Prevent copying
    MainMenuScreen(const MainMenuScreen&) = delete;
    MainMenuScreen& operator=(const MainMenuScreen&) = delete;

    /**
     * @brief Update menu logic (e.g., animations, timers)
     * @param delta_time Time elapsed since last frame (seconds)
     */
    void Update(double delta_time);

    /**
     * @brief Render main menu screen
     */
    void Render();

    /**
     * @brief Handle input
     */
    void HandleInput();

    /**
     * @brief Check if a screen transition was requested
     * @return true if user selected a menu option
     */
    bool HasScreenTransition() const { return has_screen_transition_; }

    /**
     * @brief Get target screen for transition
     * @return GameScreen to transition to
     */
    GameScreen GetTargetScreen() const { return target_screen_; }

    /**
     * @brief Reset screen transition state
     */
    void ResetTransition() { has_screen_transition_ = false; }

    /**
     * @brief Check if game should exit
     * @return true if user selected Exit option
     */
    bool ShouldExit() const { return should_exit_; }

private:
    /**
     * @brief Render menu title
     */
    void RenderTitle();

    /**
     * @brief Render menu options with selection highlight
     */
    void RenderMenuOptions();

    /**
     * @brief Handle menu option selection
     */
    void OnOptionSelected();

    /**
     * @brief Navigate menu selection up
     */
    void NavigateUp();

    /**
     * @brief Navigate menu selection down
     */
    void NavigateDown();

    IRenderer* renderer_;
    IInputHandler* input_handler_;

    std::vector<MenuOption> menu_options_;
    int selected_option_;
    bool has_screen_transition_;
    GameScreen target_screen_;
    bool should_exit_;

    // Input debouncing
    bool was_up_pressed_;
    bool was_down_pressed_;
    bool was_select_pressed_;
};

} // namespace wreckingball

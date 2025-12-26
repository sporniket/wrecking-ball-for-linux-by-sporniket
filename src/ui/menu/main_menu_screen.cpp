// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "main_menu_screen.hpp"
#include "../../core/constants.hpp"
#include "../../core/logging/error_logger.hpp"

namespace wreckingball {

MainMenuScreen::MainMenuScreen(IRenderer* renderer, IInputHandler* input_handler)
    : renderer_(renderer), input_handler_(input_handler),
      selected_option_(0), has_screen_transition_(false),
      target_screen_(GameScreen::MainMenu), should_exit_(false),
      was_up_pressed_(false), was_down_pressed_(false), was_select_pressed_(false) {

    // T106: Initialize menu options
    menu_options_ = {
        {"Play (Casual)", GameScreen::GameplayCasual, nullptr},
        {"Play (Arcade)", GameScreen::GameplayArcade, nullptr},
        {"Level Selection", GameScreen::LevelSelect, nullptr},
        {"Achievements", GameScreen::Trophies, nullptr},
        {"Settings", GameScreen::Settings, nullptr},
        {"Level Editor", GameScreen::LevelEditor, nullptr},
        {"My Other Games", GameScreen::OtherGames, nullptr},
        {"Exit", GameScreen::MainMenu, [this]() { should_exit_ = true; }}
    };

    ErrorLogger::GetInstance().LogInfo("MainMenuScreen initialized with " +
                                      std::to_string(menu_options_.size()) + " options");
}

MainMenuScreen::~MainMenuScreen() {
    ErrorLogger::GetInstance().LogInfo("MainMenuScreen destroyed");
}

void MainMenuScreen::Update([[maybe_unused]] double delta_time) {
    // T106: Future animations or timers can be added here
}

void MainMenuScreen::Render() {
    // T106: Render main menu
    RenderTitle();
    RenderMenuOptions();
}

void MainMenuScreen::RenderTitle() {
    // T106: Render game title at top of screen
    const char* title = "SPORNIKET'S WRECKING BALL";
    const int title_x = SCREEN_WIDTH / 2 - 120;  // Approximate centering
    const int title_y = 10;  // Move higher to make room for menu

    // Draw title in white
    renderer_->DrawText(title, title_x, title_y, COLOR_TEXT_PRIMARY);

    // Draw subtitle/version
    const char* subtitle = "v0.2.0 - Main Menu";
    const int subtitle_x = SCREEN_WIDTH / 2 - 80;
    const int subtitle_y = 22;  // Adjusted for higher title
    renderer_->DrawText(subtitle, subtitle_x, subtitle_y, COLOR_TEXT_SECONDARY);
}

void MainMenuScreen::RenderMenuOptions() {
    // T107: Render menu options with selection highlight
    const int start_y = 45;  // Start higher on screen
    const int option_spacing = 14;  // Slightly tighter spacing
    const int option_x = 80;

    for (size_t i = 0; i < menu_options_.size(); ++i) {
        const int y = start_y + static_cast<int>(i) * option_spacing;
        const MenuOption& option = menu_options_[i];

        // T107: Highlight selected option
        if (static_cast<int>(i) == selected_option_) {
            // Draw selection indicator
            renderer_->DrawText(">", option_x - 20, y, COLOR_14);  // Yellow
            // Draw selected option in yellow
            renderer_->DrawText(option.label.c_str(), option_x, y, COLOR_14);
        } else {
            // Draw unselected option in white
            renderer_->DrawText(option.label.c_str(), option_x, y, COLOR_TEXT_PRIMARY);
        }
    }

    // Draw instructions at bottom of screen
    const char* instructions = "UP/DOWN: Navigate  ENTER/SPACE: Select  ESC: Exit";
    renderer_->DrawText(instructions, 10, SCREEN_HEIGHT - 10, COLOR_TEXT_SECONDARY);
}

void MainMenuScreen::HandleInput() {
    // T108: Handle keyboard/mouse input with debouncing
    if (!input_handler_) {
        return;
    }

    const auto& input_state = input_handler_->GetInputState();

    // Navigation up (paddle_movement < 0) - W/Up arrow
    if (input_state.paddle_movement < -0.5f) {
        if (!was_up_pressed_) {
            NavigateUp();
            was_up_pressed_ = true;
        }
    } else {
        was_up_pressed_ = false;
    }

    // Navigation down (paddle_movement > 0) - S/Down arrow
    if (input_state.paddle_movement > 0.5f) {
        if (!was_down_pressed_) {
            NavigateDown();
            was_down_pressed_ = true;
        }
    } else {
        was_down_pressed_ = false;
    }

    // Selection with confirm button (Enter/Space)
    if (input_state.confirm) {
        if (!was_select_pressed_) {
            OnOptionSelected();
            was_select_pressed_ = true;
        }
    } else {
        was_select_pressed_ = false;
    }

    // ESC to exit
    if (input_state.back) {
        should_exit_ = true;
    }
}

void MainMenuScreen::OnOptionSelected() {
    // T109: Handle menu option selection
    if (selected_option_ < 0 || selected_option_ >= static_cast<int>(menu_options_.size())) {
        return;
    }

    const MenuOption& option = menu_options_[selected_option_];

    ErrorLogger::GetInstance().LogInfo("Menu option selected: " + option.label);

    // Execute custom action if defined (e.g., Exit)
    if (option.action) {
        option.action();
    } else {
        // Transition to target screen
        target_screen_ = option.target_screen;
        has_screen_transition_ = true;
    }
}

void MainMenuScreen::NavigateUp() {
    selected_option_--;
    if (selected_option_ < 0) {
        selected_option_ = static_cast<int>(menu_options_.size()) - 1;  // Wrap to bottom
    }
}

void MainMenuScreen::NavigateDown() {
    selected_option_++;
    if (selected_option_ >= static_cast<int>(menu_options_.size())) {
        selected_option_ = 0;  // Wrap to top
    }
}

} // namespace wreckingball

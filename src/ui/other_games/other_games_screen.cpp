// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "other_games_screen.hpp"
#include "../../core/constants.hpp"
#include "../../core/logging/error_logger.hpp"

namespace wreckingball {

OtherGamesScreen::OtherGamesScreen(IRenderer* renderer, IInputHandler* input_handler)
    : renderer_(renderer), input_handler_(input_handler),
      should_return_to_menu_(false), was_back_pressed_(false) {
    ErrorLogger::GetInstance().LogInfo("OtherGamesScreen initialized (placeholder)");
}

OtherGamesScreen::~OtherGamesScreen() {
    ErrorLogger::GetInstance().LogInfo("OtherGamesScreen destroyed");
}

void OtherGamesScreen::Update([[maybe_unused]] double delta_time) {
    // Placeholder - no logic yet
}

void OtherGamesScreen::Render() {
    // T119: Placeholder rendering
    const char* title = "MY OTHER GAMES";
    const char* subtitle = "(Coming in Phase 10 - US8)";
    const char* back_msg = "Press ESC to return to main menu";

    renderer_->DrawText(title, SCREEN_WIDTH / 2 - 70, 60, COLOR_TEXT_PRIMARY);
    renderer_->DrawText(subtitle, SCREEN_WIDTH / 2 - 100, 80, COLOR_TEXT_SECONDARY);
    renderer_->DrawText(back_msg, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 40, COLOR_14);
}

void OtherGamesScreen::HandleInput() {
    if (!input_handler_) {
        return;
    }

    const auto& input_state = input_handler_->GetInputState();

    // ESC to return to menu (with debouncing)
    if (input_state.back) {
        if (!was_back_pressed_) {
            should_return_to_menu_ = true;
            was_back_pressed_ = true;
        }
    } else {
        was_back_pressed_ = false;
    }
}

} // namespace wreckingball

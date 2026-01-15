// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "casual_mode_screen.hpp"
#include "../../core/entities/game_mode.hpp"
#include "../../core/logging/error_logger.hpp"

namespace wreckingball {

CasualModeScreen::CasualModeScreen(IRenderer* renderer, IInputHandler* input_handler, GameStateManager* game_manager)
    : renderer_(renderer), input_handler_(input_handler), game_manager_(game_manager),
      gameplay_screen_(nullptr), should_exit_(false) {

    // Create wrapped gameplay screen
    gameplay_screen_ = std::make_unique<GameplayScreen>(renderer, input_handler, game_manager);

    ErrorLogger::GetInstance().LogInfo("CasualModeScreen created");
}

CasualModeScreen::~CasualModeScreen() = default;

bool CasualModeScreen::Initialize(const Level& level) {
    // Start level in Casual mode
    if (!game_manager_->StartLevel(level, GameMode::Casual)) {
        ErrorLogger::GetInstance().LogError("Failed to start casual level");
        return false;
    }

    ErrorLogger::GetInstance().LogInfo("Casual mode initialized: " + level.name +
        " (ID: " + std::to_string(level.level_id) + ")");

    return true;
}

void CasualModeScreen::Update(double delta_time) {
    // Forward update to wrapped gameplay screen
    if (gameplay_screen_) {
        gameplay_screen_->Update(delta_time);
    }
}

void CasualModeScreen::Render() {
    // Forward rendering to wrapped gameplay screen
    if (gameplay_screen_) {
        gameplay_screen_->Render();
    }
}

void CasualModeScreen::HandleInput() {
    // Forward input to wrapped gameplay screen
    if (gameplay_screen_) {
        gameplay_screen_->HandleInput();

        // Check if gameplay screen wants to exit
        if (gameplay_screen_->ShouldExit()) {
            should_exit_ = true;
        }
    }
}

} // namespace wreckingball

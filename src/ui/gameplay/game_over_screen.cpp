// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "game_over_screen.hpp"
#include "../../core/logging/error_logger.hpp"
#include <SDL2/SDL.h>
#include <sstream>
#include <iomanip>

namespace wreckingball {

GameOverScreen::GameOverScreen(IRenderer* renderer, IInputHandler* input_handler)
    : renderer_(renderer), input_handler_(input_handler),
      final_score_(0), highest_level_(1), should_exit_(false) {
}

GameOverScreen::~GameOverScreen() = default;

void GameOverScreen::SetStats(uint32_t final_score, int highest_level) {
    final_score_ = final_score;
    highest_level_ = highest_level;

    ErrorLogger::GetInstance().LogInfo("Game Over - Score: " +
        std::to_string(final_score) + ", Highest Level: " +
        std::to_string(highest_level));
}

void GameOverScreen::Render() {
    if (!renderer_) {
        return;
    }

    // Draw background (dark overlay)
    Color bg_color = {20, 20, 30, 255};
    SDL_Rect screen = {0, 0, 320, 200};
    renderer_->DrawFilledRect(screen, bg_color);

    // Draw "GAME OVER" title box
    Color title_bg = {170, 0, 0, 255};  // Red
    SDL_Rect title_box = {60, 30, 200, 35};
    renderer_->DrawFilledRect(title_box, title_bg);

    // Draw title border
    Color border_color = {255, 255, 255, 255};  // White
    SDL_Rect title_border_top = {60, 30, 200, 2};
    SDL_Rect title_border_bottom = {60, 63, 200, 2};
    SDL_Rect title_border_left = {60, 30, 2, 35};
    SDL_Rect title_border_right = {258, 30, 2, 35};
    renderer_->DrawFilledRect(title_border_top, border_color);
    renderer_->DrawFilledRect(title_border_bottom, border_color);
    renderer_->DrawFilledRect(title_border_left, border_color);
    renderer_->DrawFilledRect(title_border_right, border_color);

    // Render "GAME OVER" text
    renderer_->DrawText("GAME OVER", 110, 42, COLOR_TEXT_PRIMARY);

    // Render stats
    RenderScore();
    RenderHighestLevel();

    // Render instructions
    RenderInstructions();
}

void GameOverScreen::HandleInput() {
    if (!input_handler_) {
        return;
    }

    const InputState& input = input_handler_->GetInputState();

    // Any action returns to menu
    if (input.launch_ball || input.back || input.pause_toggle) {
        should_exit_ = true;
        ErrorLogger::GetInstance().LogInfo("Returning to main menu from game over screen");
    }
}

void GameOverScreen::RenderScore() {
    if (!renderer_) {
        return;
    }

    // Draw score box
    Color score_bg = {40, 40, 60, 255};
    SDL_Rect score_box = {70, 80, 180, 30};
    renderer_->DrawFilledRect(score_box, score_bg);

    // Draw score border
    Color border_color = {255, 255, 85, 255};  // Yellow
    SDL_Rect border_top = {70, 80, 180, 2};
    SDL_Rect border_bottom = {70, 108, 180, 2};
    SDL_Rect border_left = {70, 80, 2, 30};
    SDL_Rect border_right = {248, 80, 2, 30};
    renderer_->DrawFilledRect(border_top, border_color);
    renderer_->DrawFilledRect(border_bottom, border_color);
    renderer_->DrawFilledRect(border_left, border_color);
    renderer_->DrawFilledRect(border_right, border_color);

    // Render "FINAL SCORE:" label
    renderer_->DrawText("FINAL SCORE:", 85, 85, COLOR_TEXT_SECONDARY);

    // Render score value
    std::string score_text = std::to_string(final_score_);
    renderer_->DrawText(score_text, 130, 95, COLOR_TEXT_PRIMARY);
}

void GameOverScreen::RenderHighestLevel() {
    if (!renderer_) {
        return;
    }

    // Draw level box
    Color level_bg = {40, 40, 60, 255};
    SDL_Rect level_box = {70, 120, 180, 30};
    renderer_->DrawFilledRect(level_box, level_bg);

    // Draw level border
    Color border_color = {85, 255, 255, 255};  // Cyan
    SDL_Rect border_top = {70, 120, 180, 2};
    SDL_Rect border_bottom = {70, 148, 180, 2};
    SDL_Rect border_left = {70, 120, 2, 30};
    SDL_Rect border_right = {248, 120, 2, 30};
    renderer_->DrawFilledRect(border_top, border_color);
    renderer_->DrawFilledRect(border_bottom, border_color);
    renderer_->DrawFilledRect(border_left, border_color);
    renderer_->DrawFilledRect(border_right, border_color);

    // Render "HIGHEST LEVEL:" label
    renderer_->DrawText("HIGHEST LEVEL:", 80, 125, COLOR_TEXT_SECONDARY);

    // Render level number
    std::string level_text = std::to_string(highest_level_);
    renderer_->DrawText(level_text, 140, 135, COLOR_TEXT_PRIMARY);
}

void GameOverScreen::RenderInstructions() {
    if (!renderer_) {
        return;
    }

    // Draw instruction box
    Color instruction_bg = {40, 40, 60, 255};
    SDL_Rect instruction_box = {40, 165, 240, 25};
    renderer_->DrawFilledRect(instruction_box, instruction_bg);

    // Draw flashing indicator (simple blink effect)
    static int frame_counter = 0;
    frame_counter++;

    if ((frame_counter / 30) % 2 == 0) {  // Blink every 30 frames
        Color indicator_color = {255, 255, 255, 255};  // White
        SDL_Rect indicator = {155, 173, 10, 10};
        renderer_->DrawFilledRect(indicator, indicator_color);
    }

    // Render "Press SPACE to continue"
    renderer_->DrawText("Press SPACE to continue", 70, 172, COLOR_TEXT_SECONDARY);
}

} // namespace wreckingball

// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "gameplay_screen.hpp"
#include "../../core/entities/brick.hpp"
#include "../../core/entities/ball.hpp"
#include "../../core/entities/paddle.hpp"
#include "../../core/entities/level.hpp"
#include <SDL2/SDL.h>

namespace wreckingball {

GameplayScreen::GameplayScreen(IRenderer* renderer, IInputHandler* input_handler, GameStateManager* game_manager)
    : renderer_(renderer), input_handler_(input_handler), game_manager_(game_manager),
      should_exit_(false), is_paused_(false) {
}

GameplayScreen::~GameplayScreen() = default;

void GameplayScreen::Update(double delta_time) {
    if (is_paused_) {
        return;  // Don't update game logic when paused
    }

    // Update game state
    if (game_manager_) {
        game_manager_->Update(delta_time);
    }
}

void GameplayScreen::HandleInput() {
    if (!input_handler_ || !game_manager_) {
        return;
    }

    const InputState& input = input_handler_->GetInputState();

    // Handle pause toggle
    if (input.pause_toggle) {
        is_paused_ = !is_paused_;
    }

    // Handle back button (exit to menu)
    if (input.back) {
        should_exit_ = true;
    }

    if (is_paused_) {
        return;  // Don't process game input when paused
    }

    // Handle paddle movement
    const GameState& game_state = game_manager_->GetGameState();
    if (input.paddle_movement != 0.0f) {
        // Paddle moves in unit coordinates (40 units wide playfield)
        // Paddle speed in units/second
        double paddle_speed = 25.0;  // units per second
        double new_x = game_state.paddle.position_x + (input.paddle_movement * paddle_speed * 0.016);  // Approx 60fps
        game_manager_->UpdatePaddlePosition(new_x, 40);  // 40 = grid width in units
    }

    // Handle ball launch
    if (input.launch_ball) {
        game_manager_->LaunchBall();
    }
}

void GameplayScreen::Render() {
    if (!renderer_) {
        return;
    }

    // Render play field
    RenderPlayField();

    // Render game objects
    RenderBricks();
    RenderBall();
    RenderPaddle();

    // Render HUD
    RenderHUD();

    // Render pause menu if paused
    if (is_paused_) {
        RenderPauseMenu();
    }
}

void GameplayScreen::RenderPlayField() {
    // Draw play field borders (1 unit thick borders on left, right, top)
    // Convert unit coordinates to pixels (8 pixels per unit)
    constexpr int PIXELS_PER_UNIT = 8;
    constexpr int BORDER_THICKNESS = PIXELS_PER_UNIT;  // 1 unit = 8 pixels
    constexpr int PLAYFIELD_TOP = PIXELS_PER_UNIT;      // y=8 pixels
    constexpr int SCREEN_WIDTH = 320;
    constexpr int SCREEN_HEIGHT = 200;

    Color border_color = {255, 255, 255, 255};  // White

    // Left border: x=0, starts at y=8
    SDL_Rect left_border = {0, PLAYFIELD_TOP, BORDER_THICKNESS, SCREEN_HEIGHT - PLAYFIELD_TOP};
    renderer_->DrawFilledRect(left_border, border_color);

    // Right border: x=312 (320-8), starts at y=8
    SDL_Rect right_border = {SCREEN_WIDTH - BORDER_THICKNESS, PLAYFIELD_TOP, BORDER_THICKNESS, SCREEN_HEIGHT - PLAYFIELD_TOP};
    renderer_->DrawFilledRect(right_border, border_color);

    // Top border: y=8, full width
    SDL_Rect top_border = {0, PLAYFIELD_TOP, SCREEN_WIDTH, BORDER_THICKNESS};
    renderer_->DrawFilledRect(top_border, border_color);

    // No bottom border (ball can go out)
}

void GameplayScreen::RenderBricks() {
    if (!game_manager_) {
        return;
    }

    const GameState& game_state = game_manager_->GetGameState();
    if (!game_state.current_level) {
        return;
    }

    // Render each brick using the renderer
    for (const auto& brick : game_state.current_level->bricks) {
        if (!brick.is_destroyed) {
            renderer_->DrawBrick(brick);
        }
    }
}

void GameplayScreen::RenderBall() {
    if (!game_manager_) {
        return;
    }

    const GameState& game_state = game_manager_->GetGameState();

    // Render all balls (including inactive ones attached to paddle)
    Color ball_color = {255, 255, 255, 255};  // White
    for (const auto& ball : game_state.balls) {
        if (!ball.is_lost) {
            renderer_->DrawBall(ball, ball_color);
        }
    }
}

void GameplayScreen::RenderPaddle() {
    if (!game_manager_) {
        return;
    }

    const GameState& game_state = game_manager_->GetGameState();

    Color paddle_color = {200, 200, 200, 255};  // Light gray
    renderer_->DrawPaddle(game_state.paddle, paddle_color);
}

void GameplayScreen::RenderHUD() {
    if (!game_manager_ || !renderer_) {
        return;
    }

    const GameState& game_state = game_manager_->GetGameState();

    // HUD will be at top of screen
    // For now, this is a placeholder - text rendering will be implemented later
    // TODO: Render score, level name, completion progress

    // Draw HUD background bar
    Color hud_bg = {20, 20, 30, 200};  // Dark semi-transparent
    SDL_Rect hud_rect = {0, 0, 320, 10};
    renderer_->DrawFilledRect(hud_rect, hud_bg);

    (void)game_state;  // Unused for now until text rendering is implemented
}

void GameplayScreen::RenderPauseMenu() {
    if (!renderer_) {
        return;
    }

    // Draw semi-transparent overlay
    Color overlay = {0, 0, 0, 180};
    SDL_Rect screen = {0, 0, 320, 200};
    renderer_->DrawFilledRect(screen, overlay);

    // Draw pause menu box
    Color menu_bg = {40, 40, 60, 255};
    SDL_Rect menu_box = {80, 70, 160, 60};
    renderer_->DrawFilledRect(menu_box, menu_bg);

    // TODO: Render "PAUSED" text and menu options
    // This will be implemented when text rendering is available
}

Color GameplayScreen::GetBrickColor(const Brick& brick) const {
    // Simple color mapping based on brick type
    // This matches the palette used in SDLRenderer
    switch (brick.type) {
        case BrickType::Regular:
            return {170, 0, 0, 255};  // Red
        case BrickType::Star:
            return {255, 255, 85, 255};  // Yellow
        case BrickType::Key:
            return {85, 255, 255, 255};  // Cyan
        case BrickType::Exit:
            if (brick.is_active) {
                return {85, 255, 85, 255};  // Green (active)
            } else {
                return {42, 127, 42, 255};  // Dark green (inactive)
            }
        case BrickType::Empty:
        default:
            return {0, 0, 0, 0};  // Transparent
    }
}

} // namespace wreckingball

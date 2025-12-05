// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "game_state_manager.hpp"
#include "../logging/error_logger.hpp"
#include <algorithm>

namespace wreckingball {

GameStateManager::GameStateManager()
    : current_screen_(GameScreen::MainMenu),
      is_paused_(false) {
    ErrorLogger::GetInstance().LogInfo("GameStateManager initialized");
}

GameStateManager::~GameStateManager() {}

bool GameStateManager::StartLevel(const Level& level, GameMode mode) {
    ErrorLogger::GetInstance().LogInfo("Starting level: " + level.name + " (" +
        std::to_string(level.level_id) + ")");

    // Store level pointer - NOTE: Level must outlive GameState
    // In production, we'd store a copy or shared_ptr
    game_state_.current_level = const_cast<Level*>(&level);
    game_state_.game_mode = mode;
    game_state_.score = 0;

    // Initialize lives based on mode
    if (mode == GameMode::Arcade) {
        game_state_.lives_remaining = 3;  // Default arcade lives
    } else {
        game_state_.lives_remaining = -1;  // Infinite lives in Casual mode
    }

    // Initialize paddle and ball
    InitializePaddle();
    InitializeBall();

    // Set screen to gameplay
    current_screen_ = GameScreen::GameplayCasual;  // TODO: Set based on mode

    return true;
}

void GameStateManager::Update(double delta_time) {
    if (is_paused_) {
        return;  // Don't update when paused
    }

    // Don't update gameplay if level is complete or game is over
    if (current_screen_ == GameScreen::LevelComplete || current_screen_ == GameScreen::GameOver) {
        return;
    }

    // Update ball physics
    physics_engine_.UpdateBalls(game_state_.balls, delta_time);

    // Check wall collisions
    for (auto& ball : game_state_.balls) {
        physics_engine_.CheckWallCollisions(ball, GRID_WIDTH_UNITS, GRID_HEIGHT_UNITS);
    }

    // Check paddle collisions
    for (auto& ball : game_state_.balls) {
        if (physics_engine_.CheckPaddleCollision(ball, game_state_.paddle, BALL_SPEED)) {
            // TODO: Play paddle bounce sound
        }
    }

    // Check brick collisions
    for (auto& ball : game_state_.balls) {
        Brick* hit_brick = physics_engine_.CheckBrickCollision(
            ball,
            game_state_.current_level->bricks
        );

        if (hit_brick) {
            OnBrickDestroyed(*hit_brick);

            // Update exit activation for GetTheKey levels
            completion_checker_.UpdateExitBrickActivation(*game_state_.current_level);
        }
    }

    // Check for lost balls
    CheckBallsLost();

    // Check for level completion
    CheckLevelCompletion();
}

void GameStateManager::UpdatePaddlePosition(double paddle_x, int screen_width) {
    (void)screen_width;  // Not used, we clamp to playfield borders instead

    // Clamp paddle to playfield bounds (between left border at x=1 and right border at x=39)
    double half_width = game_state_.paddle.width / 2.0;
    double min_x = 1.0 + half_width;   // Left edge at x=1 (after left border)
    double max_x = 39.0 - half_width;  // Right edge at x=39 (before right border)

    game_state_.paddle.position_x = std::clamp(paddle_x, min_x, max_x);

    // Update attached balls to follow paddle
    for (auto& ball : game_state_.balls) {
        if (!ball.is_active) {
            ball.position_x = game_state_.paddle.position_x;
        }
    }
}

void GameStateManager::LaunchBall() {
    for (auto& ball : game_state_.balls) {
        if (!ball.is_active) {
            ball.is_active = true;

            // Launch at slight upward angle
            double launch_angle = -75.0 * (3.14159265358979323846 / 180.0);  // -75 degrees
            ball.velocity_x = BALL_SPEED * std::cos(launch_angle + 1.57079632679);  // Add 90° for upward
            ball.velocity_y = BALL_SPEED * std::sin(launch_angle + 1.57079632679);

            ErrorLogger::GetInstance().LogInfo("Ball launched");
            break;  // Only launch one ball
        }
    }
}

void GameStateManager::OnBrickDestroyed(const Brick& brick) {
    // Update score
    UpdateScore(brick);

    // TODO: Play brick destruction sound based on brick type

    ErrorLogger::GetInstance().LogInfo("Brick destroyed, score: " +
        std::to_string(game_state_.score));
}

void GameStateManager::OnBallLost() {
    if (game_state_.game_mode == GameMode::Casual) {
        // Casual mode: respawn ball immediately
        RespawnBall();
        ErrorLogger::GetInstance().LogInfo("Ball respawned (Casual mode)");
    } else {
        // Arcade mode: lose a life
        game_state_.lives_remaining--;
        ErrorLogger::GetInstance().LogInfo("Ball lost, lives remaining: " +
            std::to_string(game_state_.lives_remaining));

        if (game_state_.lives_remaining <= 0) {
            // Game over
            current_screen_ = GameScreen::GameOver;
            ErrorLogger::GetInstance().LogInfo("Game Over");
        } else {
            // Respawn ball
            RespawnBall();
        }
    }
}

void GameStateManager::OnLevelComplete() {
    ErrorLogger::GetInstance().LogInfo("Level complete: " +
        game_state_.current_level->name);

    // TODO: Show level complete screen
    // TODO: Save progress
    // TODO: Unlock next level

    current_screen_ = GameScreen::LevelComplete;
}

void GameStateManager::TogglePause() {
    is_paused_ = !is_paused_;
    ErrorLogger::GetInstance().LogInfo(is_paused_ ? "Game paused" : "Game resumed");
}

void GameStateManager::SetScreen(GameScreen screen) {
    current_screen_ = screen;
    ErrorLogger::GetInstance().LogInfo("Screen changed");
}

std::string GameStateManager::GetCompletionProgress() {
    return completion_checker_.GetCompletionProgress(*game_state_.current_level);
}

void GameStateManager::InitializePaddle() {
    // Position in unit coordinates (40 units wide × 25 units tall)
    game_state_.paddle.position_x = 20.0;  // Center of 40-unit wide field
    game_state_.paddle.position_y = 23.75; // Near bottom (25 units tall)
    game_state_.paddle.width = 2;          // 2 units wide (16 pixels)
    game_state_.paddle.height = 0.5;       // 0.5 units tall (4 pixels)
    game_state_.paddle.velocity_x = 0.0;
}

void GameStateManager::InitializeBall() {
    game_state_.balls.clear();

    Ball ball;
    // Position in unit coordinates, attached above paddle
    ball.position_x = game_state_.paddle.position_x;
    ball.position_y = game_state_.paddle.position_y - game_state_.paddle.height / 2.0 - 0.375;
    ball.velocity_x = 0.0;
    ball.velocity_y = 0.0;
    ball.radius = 0.25;  // 0.25 units (2 pixels)
    ball.is_active = false;
    ball.is_lost = false;

    game_state_.balls.push_back(ball);
}

void GameStateManager::RespawnBall() {
    // Clear lost balls
    game_state_.balls.erase(
        std::remove_if(game_state_.balls.begin(), game_state_.balls.end(),
            [](const Ball& b) { return b.is_lost; }),
        game_state_.balls.end()
    );

    // If no balls left, create new one
    if (game_state_.balls.empty()) {
        InitializeBall();
    }
}

void GameStateManager::UpdateScore(const Brick& brick) {
    // Base score by brick type
    int points = 0;

    switch (brick.type) {
        case BrickType::Regular:
            points = 10;
            break;
        case BrickType::Star:
            points = 50;  // Bonus for stars
            break;
        case BrickType::Key:
            points = 100;  // High value for keys
            break;
        case BrickType::Exit:
            points = 200;  // Highest value for exit
            break;
        default:
            points = 10;
            break;
    }

    game_state_.score += points;
}

void GameStateManager::CheckBallsLost() {
    for (auto& ball : game_state_.balls) {
        if (!ball.is_lost && ball.is_active) {
            if (physics_engine_.IsBallLost(ball, game_state_.paddle.position_y)) {
                ball.is_lost = true;
                OnBallLost();
            }
        }
    }
}

void GameStateManager::CheckLevelCompletion() {
    if (completion_checker_.IsLevelComplete(*game_state_.current_level)) {
        OnLevelComplete();
    }
}

} // namespace wreckingball

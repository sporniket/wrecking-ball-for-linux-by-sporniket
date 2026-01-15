// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "arcade_mode_screen.hpp"
#include "../../core/entities/game_mode.hpp"
#include "../../persistence/markdown/level_parser.hpp"
#include "../../core/logging/error_logger.hpp"
#include <SDL2/SDL.h>
#include <sstream>

namespace wreckingball {

ArcadeModeScreen::ArcadeModeScreen(IRenderer* renderer, IInputHandler* input_handler, GameStateManager* game_manager)
    : renderer_(renderer), input_handler_(input_handler), game_manager_(game_manager),
      gameplay_screen_(nullptr), should_exit_(false), in_life_selection_(true),
      selected_lives_option_(0), current_level_id_(1), highest_level_reached_(1), final_score_(0),
      last_navigation_time_(0.0) {

    // Create wrapped gameplay screen
    gameplay_screen_ = std::make_unique<GameplayScreen>(renderer, input_handler, game_manager);
}

ArcadeModeScreen::~ArcadeModeScreen() = default;

bool ArcadeModeScreen::Initialize(int initial_level_id, uint8_t lives) {
    current_level_id_ = initial_level_id;
    highest_level_reached_ = initial_level_id;
    in_life_selection_ = false;

    // Load the initial level
    if (!LoadLevel(current_level_id_)) {
        ErrorLogger::GetInstance().LogError("Failed to load initial level: " +
            std::to_string(current_level_id_));
        return false;
    }

    // Get the loaded level and start it in Arcade mode
    GameState& state = game_manager_->GetGameState();
    if (state.current_level) {
        // Start level in Arcade mode with specified lives
        if (!game_manager_->StartLevel(*state.current_level, GameMode::Arcade)) {
            ErrorLogger::GetInstance().LogError("Failed to start arcade level");
            return false;
        }

        // Set lives after starting the level
        game_manager_->GetGameState().lives_remaining = lives;

        ErrorLogger::GetInstance().LogInfo("Arcade mode initialized: Level " +
            std::to_string(current_level_id_) + ", Lives: " + std::to_string(lives) +
            ", Mode: Arcade");
    }

    return true;
}

void ArcadeModeScreen::Update(double delta_time) {
    if (in_life_selection_) {
        return;  // Don't update gameplay during life selection
    }

    // Check for game over
    const GameState& state = game_manager_->GetGameState();
    if (state.IsGameOver()) {
        // Store final stats
        final_score_ = state.score;
        should_exit_ = true;  // Will be handled by Application to show GameOverScreen
        ErrorLogger::GetInstance().LogInfo("Game Over - Score: " + std::to_string(final_score_) +
            ", Highest Level: " + std::to_string(highest_level_reached_));
        return;
    }

    // Check for level completion
    if (state.level_completed && game_manager_->GetCurrentScreen() == GameScreen::LevelComplete) {
        OnLevelComplete();
        return;
    }

    // Update wrapped gameplay screen
    if (gameplay_screen_) {
        gameplay_screen_->Update(delta_time);
    }
}

void ArcadeModeScreen::Render() {
    if (in_life_selection_) {
        RenderLifeSelection();
        return;
    }

    // Render wrapped gameplay screen
    if (gameplay_screen_) {
        gameplay_screen_->Render();
    }

    // Render additional arcade mode UI (lives display)
    RenderLivesDisplay();
}

void ArcadeModeScreen::HandleInput() {
    if (in_life_selection_) {
        HandleLifeSelectionInput();
        return;
    }

    // Forward input to wrapped gameplay screen
    if (gameplay_screen_) {
        gameplay_screen_->HandleInput();

        // Check if gameplay screen wants to exit
        if (gameplay_screen_->ShouldExit()) {
            should_exit_ = true;
        }
    }
}

void ArcadeModeScreen::ShowLifeSelection() {
    in_life_selection_ = true;
    selected_lives_option_ = 0;  // Default to 3 lives
}

void ArcadeModeScreen::Reset() {
    // Reset all state to initial values for a fresh arcade session
    should_exit_ = false;
    in_life_selection_ = true;
    selected_lives_option_ = 0;
    current_level_id_ = 1;
    highest_level_reached_ = 1;
    final_score_ = 0;
    last_navigation_time_ = 0.0;

    ErrorLogger::GetInstance().LogInfo("Arcade mode reset to initial state");
}

void ArcadeModeScreen::OnLevelComplete() {
    // Progress to next level
    current_level_id_++;

    // Update highest level reached
    if (current_level_id_ > highest_level_reached_) {
        highest_level_reached_ = current_level_id_;
    }

    ErrorLogger::GetInstance().LogInfo("Level complete, progressing to level " +
        std::to_string(current_level_id_));

    // Try to load next level
    if (LoadLevel(current_level_id_)) {
        // Start next level, preserving lives and score
        const GameState& old_state = game_manager_->GetGameState();
        uint8_t lives = old_state.lives_remaining;
        uint32_t score = old_state.score;

        game_manager_->StartLevel(*old_state.current_level, GameMode::Arcade);
        game_manager_->GetGameState().lives_remaining = lives;
        game_manager_->GetGameState().score = score;
    } else {
        // No more levels - game complete!
        ErrorLogger::GetInstance().LogInfo("All levels complete!");
        final_score_ = game_manager_->GetGameState().score;
        should_exit_ = true;
    }
}

void ArcadeModeScreen::RenderLivesDisplay() {
    if (!renderer_) {
        return;
    }

    const GameState& state = game_manager_->GetGameState();

    // Render lives indicator in top-right corner of HUD
    // HUD is at y=0-10 pixels
    // Draw life icons at x=280+ (leaving space for score on left)

    Color life_color = {255, 85, 85, 255};  // Light red
    int lives = static_cast<int>(state.lives_remaining);

    for (int i = 0; i < lives && i < 7; ++i) {
        // Draw small rectangles for each life (2x4 pixels each, 4 pixels apart)
        SDL_Rect life_icon = {280 + (i * 6), 3, 2, 4};
        renderer_->DrawFilledRect(life_icon, life_color);
    }

    // Render "LIVES:" label before the life icons
    renderer_->DrawText("LIVES:", 240, 3, COLOR_TEXT_SECONDARY);
}

void ArcadeModeScreen::RenderLifeSelection() {
    if (!renderer_) {
        return;
    }

    // Draw background
    Color bg_color = {20, 20, 30, 255};
    SDL_Rect screen = {0, 0, 320, 200};
    renderer_->DrawFilledRect(screen, bg_color);

    // Draw title box
    Color title_bg = {40, 40, 60, 255};
    SDL_Rect title_box = {60, 40, 200, 30};
    renderer_->DrawFilledRect(title_box, title_bg);

    // Render "SELECT LIVES" text
    renderer_->DrawText("SELECT LIVES", 100, 50, COLOR_TEXT_PRIMARY);

    // Draw option boxes (3, 5, 7 lives)
    const int option_y = 90;
    const int option_spacing = 50;
    const int option_width = 40;
    const int option_height = 40;

    Color selected_color = {85, 170, 255, 255};  // Light blue
    Color unselected_color = {60, 60, 80, 255};  // Dark gray

    // Draw three option boxes
    for (int i = 0; i < 3; ++i) {
        int x = 70 + (i * option_spacing);
        Color box_color = (i == selected_lives_option_) ? selected_color : unselected_color;
        SDL_Rect option_box = {x, option_y, option_width, option_height};
        renderer_->DrawFilledRect(option_box, box_color);

        // Draw life count indicators (small rectangles)
        int life_count = (i == 0) ? 3 : (i == 1) ? 5 : 7;
        Color life_icon_color = {255, 255, 255, 255};  // White

        for (int j = 0; j < life_count; ++j) {
            int icon_x = x + 8 + ((j % 3) * 8);
            int icon_y = option_y + 10 + ((j / 3) * 8);
            SDL_Rect life_icon = {icon_x, icon_y, 4, 6};
            renderer_->DrawFilledRect(life_icon, life_icon_color);
        }

        // Draw number label below the option box
        std::string label = std::to_string(life_count);
        Color label_color = (i == selected_lives_option_) ? COLOR_TEXT_PRIMARY : COLOR_TEXT_SECONDARY;
        renderer_->DrawText(label, x + 16, option_y + option_height + 5, label_color);
    }

    // Draw instruction box at bottom
    Color instruction_bg = {40, 40, 60, 255};
    SDL_Rect instruction_box = {40, 155, 240, 25};
    renderer_->DrawFilledRect(instruction_box, instruction_bg);

    // Render instructions
    renderer_->DrawText("LEFT/RIGHT: Select", 52, 160, COLOR_TEXT_SECONDARY);
    renderer_->DrawText("SPACE: Start", 95, 170, COLOR_TEXT_SECONDARY);
}

void ArcadeModeScreen::HandleLifeSelectionInput() {
    if (!input_handler_) {
        return;
    }

    const InputState& input = input_handler_->GetInputState();

    // Handle back button (return to menu)
    if (input.back) {
        should_exit_ = true;
        return;
    }

    // Get current time in seconds
    double current_time = SDL_GetTicks64() / 1000.0;

    // Handle left/right navigation with debouncing
    bool navigation_input = false;
    if (input.paddle_movement < -0.1f) {
        // Left - with debouncing
        if (current_time - last_navigation_time_ >= NAVIGATION_DEBOUNCE_DELAY) {
            selected_lives_option_ = (selected_lives_option_ - 1 + 3) % 3;
            last_navigation_time_ = current_time;
            navigation_input = true;
        }
    } else if (input.paddle_movement > 0.1f) {
        // Right - with debouncing
        if (current_time - last_navigation_time_ >= NAVIGATION_DEBOUNCE_DELAY) {
            selected_lives_option_ = (selected_lives_option_ + 1) % 3;
            last_navigation_time_ = current_time;
            navigation_input = true;
        }
    }

    // Reset debounce timer if no navigation input
    if (!navigation_input && std::abs(input.paddle_movement) < 0.1f) {
        last_navigation_time_ = 0.0;  // Reset when no input
    }

    // Handle selection (spacebar or enter to confirm)
    if (input.launch_ball) {
        // Start game with selected lives
        uint8_t lives = (selected_lives_option_ == 0) ? 3 :
                       (selected_lives_option_ == 1) ? 5 : 7;

        ErrorLogger::GetInstance().LogInfo("Starting arcade mode with " +
            std::to_string(lives) + " lives");

        Initialize(1, lives);
    }
}

bool ArcadeModeScreen::LoadLevel(int level_id) {
    // Construct level file path: assets/levels/XXX.md (e.g., 001.md, 012.md)
    std::stringstream ss;
    ss << "assets/levels/";

    // Format with leading zeros: 001.md, 002.md, etc.
    if (level_id < 10) {
        ss << "00" << level_id;
    } else if (level_id < 100) {
        ss << "0" << level_id;
    } else {
        ss << level_id;
    }
    ss << ".md";

    std::string level_path = ss.str();

    // Load the level
    LevelParser parser;
    auto level = parser.LoadLevel(level_path);

    if (!level.has_value()) {
        ErrorLogger::GetInstance().LogError("Failed to load level file: " + level_path);
        return false;
    }

    // Set the level_id from the filename (since it's no longer in the file)
    level->level_id = level_id;

    // Store loaded level persistently
    // NOTE: Using static to ensure level lifetime persists
    static Level loaded_levels[10];  // Support up to 10 levels
    if (level_id > 0 && level_id <= 10) {
        loaded_levels[level_id - 1] = level.value();
        game_manager_->GetGameState().current_level = &loaded_levels[level_id - 1];

        ErrorLogger::GetInstance().LogInfo("Loaded level: " + level->name +
            " (ID: " + std::to_string(level_id) + ") from " + level_path);
        return true;
    }

    ErrorLogger::GetInstance().LogError("Invalid level ID: " + std::to_string(level_id));
    return false;
}

} // namespace wreckingball

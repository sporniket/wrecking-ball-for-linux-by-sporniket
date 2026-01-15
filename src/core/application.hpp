// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "constants.hpp"
#include "entities/level.hpp"
#include "entities/game_screen.hpp"
#include <cstdint>
#include <memory>

// Forward declarations to avoid SDL2 dependency in header
struct SDL_Window;
struct SDL_Renderer;

namespace wreckingball {

// Forward declarations for game systems
class SDLRenderer;
class SDLInputHandler;
class SDLAudioPlayer;
class LevelParser;
class PhysicsEngine;
class LevelCompletionChecker;
class GameStateManager;
class GameplayScreen;

// Forward declarations for screen classes (Phase 4)
class MainMenuScreen;
class LevelSelectScreen;
class TrophiesScreen;
class SettingsScreen;
class LevelEditorScreen;
class OtherGamesScreen;

// Forward declarations for Phase 5 screens
class CasualModeScreen;
class ArcadeModeScreen;
class GameOverScreen;

/**
 * @brief Main application class managing game lifecycle
 *
 * Handles SDL2 initialization, main game loop with fixed timestep,
 * and cleanup. Uses accumulator pattern for consistent physics at 60 FPS.
 */
class Application {
public:
    Application();
    ~Application();

    /**
     * @brief Initialize SDL2, ErrorLogger, and game systems
     * @return true on success, false on failure
     */
    bool Initialize();

    /**
     * @brief Run the main game loop
     * Implements fixed timestep (60 FPS) with accumulator pattern
     */
    void Run();

    /**
     * @brief Shutdown and cleanup all systems
     */
    void Shutdown();

    /**
     * @brief Check if application is running
     * @return true if game loop is active
     */
    bool IsRunning() const { return is_running_; }

    /**
     * @brief Request application exit
     */
    void RequestExit() { is_running_ = false; }

    // Delete copy constructor and assignment operator
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

private:
    /**
     * @brief Process input events (SDL2 events, input handler)
     * @param delta_time Time elapsed since last update (in seconds)
     */
    void ProcessInput(double delta_time);

    /**
     * @brief Update game state with fixed timestep
     * @param delta_time Fixed timestep delta (1/60 second)
     */
    void Update(double delta_time);

    /**
     * @brief Render current frame
     * @param interpolation Interpolation factor for smooth rendering (0.0-1.0)
     */
    void Render(double interpolation);

    /**
     * @brief Initialize SDL2 subsystems
     * @return true on success, false on failure
     */
    bool InitializeSDL();

    /**
     * @brief Create game window and renderer
     * @return true on success, false on failure
     */
    bool CreateWindow();

    /**
     * @brief Initialize game systems (physics, audio, input, etc.)
     * @return true on success, false on failure
     */
    bool InitializeGameSystems();

    /**
     * @brief Initialize screen instances (Phase 4 - T121)
     * @return true on success, false on failure
     */
    bool InitializeScreens();

    /**
     * @brief Switch to a different screen (Phase 4 - T121)
     * @param screen Target screen to switch to
     */
    void SetCurrentScreen(GameScreen screen);

    bool is_running_;
    bool is_initialized_;
    GameScreen current_screen_;

    // SDL2 components
    SDL_Window* window_;

    // Timing
    uint64_t performance_frequency_;
    double fixed_timestep_;  // 1/60 second for 60 FPS

    // Game systems (Phase 3 integration)
    std::unique_ptr<SDLRenderer> sdl_renderer_;
    std::unique_ptr<SDLInputHandler> input_handler_;
    std::unique_ptr<SDLAudioPlayer> audio_player_;
    std::unique_ptr<LevelParser> level_parser_;
    std::unique_ptr<PhysicsEngine> physics_engine_;
    std::unique_ptr<LevelCompletionChecker> completion_checker_;
    std::unique_ptr<GameStateManager> game_manager_;

    // Screen instances (Phase 4)
    std::unique_ptr<MainMenuScreen> main_menu_screen_;
    std::unique_ptr<LevelSelectScreen> level_select_screen_;
    std::unique_ptr<TrophiesScreen> trophies_screen_;
    std::unique_ptr<SettingsScreen> settings_screen_;
    std::unique_ptr<LevelEditorScreen> level_editor_screen_;
    std::unique_ptr<OtherGamesScreen> other_games_screen_;

    // Phase 5 screens (Casual and Arcade Mode)
    std::unique_ptr<CasualModeScreen> casual_mode_screen_;
    std::unique_ptr<ArcadeModeScreen> arcade_mode_screen_;
    std::unique_ptr<GameOverScreen> game_over_screen_;

    // Current level (must persist throughout gameplay)
    Level current_level_;
};

} // namespace wreckingball

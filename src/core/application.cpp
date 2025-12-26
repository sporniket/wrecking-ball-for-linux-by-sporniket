// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "application.hpp"
#include "logging/error_logger.hpp"
#include "../sdl/renderer/sdl_renderer.hpp"
#include "../sdl/input/sdl_input_handler.hpp"
#include "../sdl/audio/sdl_audio_player.hpp"
#include "../persistence/markdown/level_parser.hpp"
#include "physics/physics_engine.hpp"
#include "level/completion_checker.hpp"
#include "game/game_state_manager.hpp"
#include "../ui/gameplay/gameplay_screen.hpp"
#include "../ui/menu/main_menu_screen.hpp"
#include "../ui/level_select/level_select_screen.hpp"
#include "../ui/trophies/trophies_screen.hpp"
#include "../ui/settings/settings_screen.hpp"
#include "../ui/level_editor/level_editor_screen.hpp"
#include "../ui/other_games/other_games_screen.hpp"
#include <SDL2/SDL.h>
#include <iostream>

namespace wreckingball {

Application::Application()
    : is_running_(false), is_initialized_(false),
      current_screen_(GameScreen::MainMenu),
      window_(nullptr),
      performance_frequency_(0), fixed_timestep_(FIXED_TIMESTEP) {
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    // Initialize ErrorLogger first
    ErrorLogger& logger = ErrorLogger::GetInstance();
    if (!logger.Initialize()) {
        std::cerr << "[ERROR] Failed to initialize ErrorLogger" << std::endl;
        return false;
    }

    logger.LogInfo("Sporniket's Wrecking Ball v0.1.0 starting...");
    logger.LogInfo("Copyright (c) 2025 David SPORN");
    logger.LogInfo("Licensed under AGPL-3.0-or-later");

    // Initialize SDL2
    if (!InitializeSDL()) {
        logger.LogCritical("Failed to initialize SDL2");
        return false;
    }

    // Create window and renderer
    if (!CreateWindow()) {
        logger.LogCritical("Failed to create window");
        return false;
    }

    // Initialize game systems
    if (!InitializeGameSystems()) {
        logger.LogCritical("Failed to initialize game systems");
        return false;
    }

    // Initialize screens (Phase 4 - T121)
    if (!InitializeScreens()) {
        logger.LogCritical("Failed to initialize screens");
        return false;
    }

    // Get SDL performance counter frequency for timing
    performance_frequency_ = SDL_GetPerformanceFrequency();

    is_initialized_ = true;
    logger.LogInfo("Application initialized successfully");

    return true;
}

void Application::Run() {
    if (!is_initialized_) {
        ErrorLogger::GetInstance().LogError("Cannot run: Application not initialized");
        return;
    }

    ErrorLogger::GetInstance().LogInfo("Starting main game loop (60 FPS fixed timestep)");

    is_running_ = true;
    double accumulator = 0.0;
    uint64_t previous_time = SDL_GetPerformanceCounter();

    // Main game loop with fixed timestep and accumulator
    while (is_running_) {
        // Calculate frame time
        uint64_t current_time = SDL_GetPerformanceCounter();
        double frame_time = static_cast<double>(current_time - previous_time) /
                           static_cast<double>(performance_frequency_);
        previous_time = current_time;

        // Cap frame time to avoid spiral of death (if game lags too much)
        if (frame_time > 0.25) {
            frame_time = 0.25;  // Max 250ms (4 FPS minimum)
        }

        // Accumulate frame time
        accumulator += frame_time;

        // Process input once per frame
        ProcessInput(frame_time);

        // Fixed timestep physics updates
        while (accumulator >= fixed_timestep_) {
            Update(fixed_timestep_);
            accumulator -= fixed_timestep_;
        }

        // Render with interpolation for smooth visuals
        double interpolation = accumulator / fixed_timestep_;
        Render(interpolation);
    }

    ErrorLogger::GetInstance().LogInfo("Game loop ended");
}

void Application::Shutdown() {
    if (!is_initialized_) {
        return;
    }

    ErrorLogger::GetInstance().LogInfo("Shutting down application...");

    // Cleanup screens (Phase 4)
    main_menu_screen_.reset();
    level_select_screen_.reset();
    trophies_screen_.reset();
    settings_screen_.reset();
    level_editor_screen_.reset();
    other_games_screen_.reset();

    // Cleanup game systems (unique_ptrs handle their own cleanup)
    sdl_renderer_.reset();
    gameplay_screen_.reset();
    game_manager_.reset();
    completion_checker_.reset();
    physics_engine_.reset();
    level_parser_.reset();
    audio_player_.reset();
    input_handler_.reset();

    // Cleanup SDL2 window
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    // Shutdown SDL2 subsystems
    SDL_Quit();

    ErrorLogger::GetInstance().LogInfo("Application shutdown complete");
    ErrorLogger::GetInstance().Shutdown();

    is_initialized_ = false;
}

void Application::ProcessInput([[maybe_unused]] double delta_time) {
    // T104/T121: Poll input through SDLInputHandler
    if (input_handler_) {
        input_handler_->PollInput();

        // Check for quit request
        if (input_handler_->IsQuitRequested()) {
            ErrorLogger::GetInstance().LogInfo("Quit event received");
            RequestExit();
            return;
        }

        // T121: Handle input based on current screen
        switch (current_screen_) {
            case GameScreen::MainMenu:
                if (main_menu_screen_) {
                    main_menu_screen_->HandleInput();
                    if (main_menu_screen_->ShouldExit()) {
                        ErrorLogger::GetInstance().LogInfo("Exit requested from main menu");
                        RequestExit();
                    } else if (main_menu_screen_->HasScreenTransition()) {
                        GameScreen target = main_menu_screen_->GetTargetScreen();
                        main_menu_screen_->ResetTransition();
                        SetCurrentScreen(target);
                    }
                }
                break;

            case GameScreen::LevelSelect:
                if (level_select_screen_) {
                    level_select_screen_->HandleInput();
                    if (level_select_screen_->ShouldReturnToMenu()) {
                        SetCurrentScreen(GameScreen::MainMenu);
                    }
                }
                break;

            case GameScreen::Trophies:
                if (trophies_screen_) {
                    trophies_screen_->HandleInput();
                    if (trophies_screen_->ShouldReturnToMenu()) {
                        SetCurrentScreen(GameScreen::MainMenu);
                    }
                }
                break;

            case GameScreen::Settings:
                if (settings_screen_) {
                    settings_screen_->HandleInput();
                    if (settings_screen_->ShouldReturnToMenu()) {
                        SetCurrentScreen(GameScreen::MainMenu);
                    }
                }
                break;

            case GameScreen::LevelEditor:
                if (level_editor_screen_) {
                    level_editor_screen_->HandleInput();
                    if (level_editor_screen_->ShouldReturnToMenu()) {
                        SetCurrentScreen(GameScreen::MainMenu);
                    }
                }
                break;

            case GameScreen::OtherGames:
                if (other_games_screen_) {
                    other_games_screen_->HandleInput();
                    if (other_games_screen_->ShouldReturnToMenu()) {
                        SetCurrentScreen(GameScreen::MainMenu);
                    }
                }
                break;

            case GameScreen::GameplayCasual:
            case GameScreen::GameplayArcade:
                if (gameplay_screen_) {
                    gameplay_screen_->HandleInput();
                    if (gameplay_screen_->ShouldExit()) {
                        SetCurrentScreen(GameScreen::MainMenu);
                    }
                }
                break;

            default:
                break;
        }
    }
}

void Application::Update([[maybe_unused]] double delta_time) {
    // T104/T121: Update based on current screen
    switch (current_screen_) {
        case GameScreen::MainMenu:
            if (main_menu_screen_) {
                main_menu_screen_->Update(delta_time);
            }
            break;

        case GameScreen::LevelSelect:
            if (level_select_screen_) {
                level_select_screen_->Update(delta_time);
            }
            break;

        case GameScreen::Trophies:
            if (trophies_screen_) {
                trophies_screen_->Update(delta_time);
            }
            break;

        case GameScreen::Settings:
            if (settings_screen_) {
                settings_screen_->Update(delta_time);
            }
            break;

        case GameScreen::LevelEditor:
            if (level_editor_screen_) {
                level_editor_screen_->Update(delta_time);
            }
            break;

        case GameScreen::OtherGames:
            if (other_games_screen_) {
                other_games_screen_->Update(delta_time);
            }
            break;

        case GameScreen::GameplayCasual:
        case GameScreen::GameplayArcade:
            if (gameplay_screen_) {
                gameplay_screen_->Update(delta_time);
            }
            break;

        default:
            break;
    }
}

void Application::Render([[maybe_unused]] double interpolation) {
    // T104/T121: Render based on current screen
    if (sdl_renderer_) {
        sdl_renderer_->BeginFrame();

        switch (current_screen_) {
            case GameScreen::MainMenu:
                if (main_menu_screen_) {
                    main_menu_screen_->Render();
                }
                break;

            case GameScreen::LevelSelect:
                if (level_select_screen_) {
                    level_select_screen_->Render();
                }
                break;

            case GameScreen::Trophies:
                if (trophies_screen_) {
                    trophies_screen_->Render();
                }
                break;

            case GameScreen::Settings:
                if (settings_screen_) {
                    settings_screen_->Render();
                }
                break;

            case GameScreen::LevelEditor:
                if (level_editor_screen_) {
                    level_editor_screen_->Render();
                }
                break;

            case GameScreen::OtherGames:
                if (other_games_screen_) {
                    other_games_screen_->Render();
                }
                break;

            case GameScreen::GameplayCasual:
            case GameScreen::GameplayArcade:
                if (gameplay_screen_) {
                    gameplay_screen_->Render();
                }
                break;

            default:
                break;
        }

        sdl_renderer_->EndFrame();
    }
}

bool Application::InitializeSDL() {
    ErrorLogger& logger = ErrorLogger::GetInstance();

    // Initialize SDL2 subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        logger.LogError(std::string("SDL_Init failed: ") + SDL_GetError());
        return false;
    }

    logger.LogInfo("SDL2 initialized (VIDEO, AUDIO, GAMECONTROLLER)");
    return true;
}

bool Application::CreateWindow() {
    ErrorLogger& logger = ErrorLogger::GetInstance();

    // Create window (will scale to fill screen while maintaining aspect ratio)
    window_ = SDL_CreateWindow(
        "Sporniket's Wrecking Ball",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH * 3,   // Start at 3x scale (960x600)
        SCREEN_HEIGHT * 3,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window_) {
        logger.LogError(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
        return false;
    }

    logger.LogInfo("Window created: 960x600 (3x scale of 320x200)");

    return true;
}

bool Application::InitializeGameSystems() {
    ErrorLogger& logger = ErrorLogger::GetInstance();

    // T101: Create platform implementations
    try {
        sdl_renderer_ = std::make_unique<SDLRenderer>(window_);
        logger.LogInfo("SDLRenderer created");
    } catch (const std::exception& e) {
        logger.LogError(std::string("Failed to create SDLRenderer: ") + e.what());
        return false;
    }

    input_handler_ = std::make_unique<SDLInputHandler>();
    logger.LogInfo("SDLInputHandler created");

    audio_player_ = std::make_unique<SDLAudioPlayer>();
    if (!audio_player_->Initialize()) {
        logger.LogWarning("Audio initialization failed - running in silent mode");
    } else {
        logger.LogInfo("SDLAudioPlayer initialized");
    }

    // Create game systems
    level_parser_ = std::make_unique<LevelParser>();
    logger.LogInfo("LevelParser created");

    physics_engine_ = std::make_unique<PhysicsEngine>();
    logger.LogInfo("PhysicsEngine created");

    completion_checker_ = std::make_unique<LevelCompletionChecker>();
    logger.LogInfo("LevelCompletionChecker created");

    // GameStateManager creates its own PhysicsEngine and LevelCompletionChecker
    game_manager_ = std::make_unique<GameStateManager>();
    logger.LogInfo("GameStateManager created");

    // T102: Create GameplayScreen
    gameplay_screen_ = std::make_unique<GameplayScreen>(
        sdl_renderer_.get(),
        input_handler_.get(),
        game_manager_.get()
    );
    logger.LogInfo("GameplayScreen created");

    // T103: Load default level (level 1) on game start
    auto level_opt = level_parser_->LoadLevel("assets/levels/001-classical-easy.md");
    if (!level_opt.has_value()) {
        logger.LogError("Failed to load default level: assets/levels/001-classical-easy.md");
        return false;
    }

    // Store level persistently to avoid dangling pointer
    current_level_ = std::move(*level_opt);

    // Log brick count for debugging
    int total_bricks = 0;
    for (const auto& brick : current_level_.bricks) {
        if (brick.type != BrickType::Empty) {
            total_bricks++;
        }
    }
    logger.LogInfo("Level has " + std::to_string(current_level_.bricks.size()) +
                   " brick entries (" + std::to_string(total_bricks) + " non-empty)");

    if (!game_manager_->StartLevel(current_level_, GameMode::Casual)) {
        logger.LogError("Failed to start level");
        return false;
    }

    logger.LogInfo("Default level loaded: " + current_level_.name);
    logger.LogInfo("Game systems initialized successfully");
    return true;
}

bool Application::InitializeScreens() {
    // T121: Initialize all screen instances
    ErrorLogger& logger = ErrorLogger::GetInstance();

    try {
        // Main menu screen (Phase 4)
        main_menu_screen_ = std::make_unique<MainMenuScreen>(
            sdl_renderer_.get(),
            input_handler_.get()
        );
        logger.LogInfo("MainMenuScreen created");

        // Placeholder screens (Phase 4)
        level_select_screen_ = std::make_unique<LevelSelectScreen>(
            sdl_renderer_.get(),
            input_handler_.get()
        );
        logger.LogInfo("LevelSelectScreen created");

        trophies_screen_ = std::make_unique<TrophiesScreen>(
            sdl_renderer_.get(),
            input_handler_.get()
        );
        logger.LogInfo("TrophiesScreen created");

        settings_screen_ = std::make_unique<SettingsScreen>(
            sdl_renderer_.get(),
            input_handler_.get()
        );
        logger.LogInfo("SettingsScreen created");

        level_editor_screen_ = std::make_unique<LevelEditorScreen>(
            sdl_renderer_.get(),
            input_handler_.get()
        );
        logger.LogInfo("LevelEditorScreen created");

        other_games_screen_ = std::make_unique<OtherGamesScreen>(
            sdl_renderer_.get(),
            input_handler_.get()
        );
        logger.LogInfo("OtherGamesScreen created");

        logger.LogInfo("All screens initialized successfully");
        return true;
    } catch (const std::exception& e) {
        logger.LogError(std::string("Failed to initialize screens: ") + e.what());
        return false;
    }
}

void Application::SetCurrentScreen(GameScreen screen) {
    // T121: Switch to different screen
    ErrorLogger& logger = ErrorLogger::GetInstance();

    logger.LogInfo("Switching to screen: " + std::to_string(static_cast<int>(screen)));

    // Handle special transitions
    if (screen == GameScreen::GameplayCasual || screen == GameScreen::GameplayArcade) {
        // T120: Load gameplay screen (will be enhanced in later phases)
        // For now, load the default level
        if (game_manager_ && !current_level_.name.empty()) {
            GameMode mode = (screen == GameScreen::GameplayArcade) ? GameMode::Arcade : GameMode::Casual;
            if (!game_manager_->StartLevel(current_level_, mode)) {
                logger.LogError("Failed to start level for gameplay");
                current_screen_ = GameScreen::MainMenu;
                return;
            }
        }
    }

    current_screen_ = screen;
}

} // namespace wreckingball

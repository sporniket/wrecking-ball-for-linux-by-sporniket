// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "application.hpp"
#include "logging/error_logger.hpp"
#include <SDL2/SDL.h>
#include <iostream>

namespace wreckingball {

Application::Application()
    : is_running_(false), is_initialized_(false),
      window_(nullptr), renderer_(nullptr),
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

    // Cleanup SDL2 renderer and window
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

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
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                ErrorLogger::GetInstance().LogInfo("Quit event received");
                RequestExit();
                break;

            case SDL_KEYDOWN:
                // Handle keyboard input
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    ErrorLogger::GetInstance().LogInfo("Escape key pressed - exiting");
                    RequestExit();
                }
                break;

            default:
                break;
        }
    }

    // TODO: Call IInputHandler::PollInput() when implemented
}

void Application::Update([[maybe_unused]] double delta_time) {
    // TODO: Update game state with fixed timestep
    // - Update paddle position
    // - Update ball physics
    // - Check collisions
    // - Update game logic
}

void Application::Render([[maybe_unused]] double interpolation) {
    // Clear screen with background color
    SDL_SetRenderDrawColor(renderer_,
                          COLOR_BACKGROUND.r,
                          COLOR_BACKGROUND.g,
                          COLOR_BACKGROUND.b,
                          COLOR_BACKGROUND.a);
    SDL_RenderClear(renderer_);

    // TODO: Render game objects with interpolation
    // - Draw bricks
    // - Draw paddle
    // - Draw ball(s)
    // - Draw HUD

    // Present frame
    SDL_RenderPresent(renderer_);
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

    // Create renderer with VSync enabled
    renderer_ = SDL_CreateRenderer(
        window_,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer_) {
        logger.LogError(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        return false;
    }

    // Set logical size for retro resolution (320x200)
    // SDL will handle scaling with letterboxing
    SDL_RenderSetLogicalSize(renderer_, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Use nearest-neighbor filtering for pixel-perfect scaling
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    // Enable integer scaling for crisp pixels (if supported)
    SDL_RenderSetIntegerScale(renderer_, SDL_TRUE);

    logger.LogInfo("Window created: 960x600 (3x scale of 320x200)");
    logger.LogInfo("Renderer created with VSync and integer scaling");

    return true;
}

bool Application::InitializeGameSystems() {
    ErrorLogger& logger = ErrorLogger::GetInstance();

    // TODO: Initialize game systems
    // - Create platform implementations (SDL input handler, SDL renderer, SDL audio player, SDL file IO)
    // - Initialize settings manager
    // - Initialize progress manager
    // - Initialize achievement tracker
    // - Load initial game state

    logger.LogInfo("Game systems initialized (placeholder)");
    return true;
}

} // namespace wreckingball

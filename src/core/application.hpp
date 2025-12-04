// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "constants.hpp"
#include <cstdint>

// Forward declarations to avoid SDL2 dependency in header
struct SDL_Window;
struct SDL_Renderer;

namespace wreckingball {

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

    bool is_running_;
    bool is_initialized_;

    // SDL2 components
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    // Timing
    uint64_t performance_frequency_;
    double fixed_timestep_;  // 1/60 second for 60 FPS
};

} // namespace wreckingball

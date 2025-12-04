// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "input_method.hpp"
#include <cstdint>
#include <SDL2/SDL_scancode.h>

namespace wreckingball {

/**
 * @brief Represents player preferences and configuration
 *
 * Settings for audio, display, input, and key bindings.
 * Serialized to INI file for persistence.
 */
struct GameSettings {
    // Audio settings
    uint8_t master_volume;        // Master audio volume (0-100)
    uint8_t music_volume;         // Music volume (0-100)
    uint8_t sfx_volume;           // Sound effects volume (0-100)

    // Display settings
    bool fullscreen;              // Fullscreen or windowed mode
    bool vsync_enabled;           // Enable VSync for 60Hz
    bool integer_scaling;         // Integer-only scaling for pixel-perfect

    // Input settings
    InputMethod input_method;     // Primary input device (Mouse, Keyboard, Controller)
    float mouse_sensitivity;      // Mouse movement sensitivity (0.5-2.0)
    float controller_deadzone;    // Analog stick deadzone (0.1-0.5)

    // Key bindings
    SDL_Scancode key_paddle_left;  // Key for paddle left movement
    SDL_Scancode key_paddle_right; // Key for paddle right movement
    SDL_Scancode key_launch;       // Key to launch ball
    SDL_Scancode key_pause;        // Key to pause game

    /**
     * @brief Default constructor with sensible defaults
     */
    GameSettings()
        : master_volume(80), music_volume(70), sfx_volume(90),
          fullscreen(false), vsync_enabled(true), integer_scaling(true),
          input_method(InputMethod::Mouse),
          mouse_sensitivity(1.0f), controller_deadzone(0.2f),
          key_paddle_left(SDL_SCANCODE_LEFT),
          key_paddle_right(SDL_SCANCODE_RIGHT),
          key_launch(SDL_SCANCODE_SPACE),
          key_pause(SDL_SCANCODE_ESCAPE) {}

    /**
     * @brief Validate all settings are within acceptable ranges
     * @return true if settings are valid
     */
    bool IsValid() const {
        return master_volume <= 100 &&
               music_volume <= 100 &&
               sfx_volume <= 100 &&
               mouse_sensitivity >= 0.5f && mouse_sensitivity <= 2.0f &&
               controller_deadzone >= 0.1f && controller_deadzone <= 0.5f;
    }

    /**
     * @brief Set volume level with clamping
     * @param volume_ptr Pointer to volume field to set
     * @param value New volume value
     */
    void SetVolume(uint8_t* volume_ptr, int value) {
        if (value < 0) value = 0;
        if (value > 100) value = 100;
        *volume_ptr = static_cast<uint8_t>(value);
    }

    /**
     * @brief Check if key binding conflicts exist
     * @return true if there are conflicting key bindings
     */
    bool HasKeyConflicts() const {
        // Check if any two keys are the same (excluding pause)
        return (key_paddle_left == key_paddle_right) ||
               (key_paddle_left == key_launch) ||
               (key_paddle_right == key_launch);
    }
};

} // namespace wreckingball

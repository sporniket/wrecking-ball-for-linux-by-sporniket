// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../core/entities/input_method.hpp"
#include <cstdint>

// Forward declaration to avoid SDL2 dependency in interface
struct SDL_Point;

namespace wreckingball {

/**
 * @brief Input state for a single frame
 */
struct InputState {
    float paddle_movement;     // -1.0 (left) to +1.0 (right), 0.0 = no movement
    bool launch_ball;          // true if launch button pressed this frame
    bool pause_toggle;         // true if pause button pressed this frame
    bool confirm;              // true if confirm button pressed (menu navigation)
    bool back;                 // true if back button pressed (menu navigation)
    SDL_Point mouse_position;  // Current mouse position (for menus)
};

// Forward declaration
struct GameSettings;

/**
 * @brief Platform-agnostic input handling interface
 *
 * Abstracts input operations to enable future Android porting.
 * SDL2 implementation will be in src/sdl/input/sdl_input_handler.hpp
 */
class IInputHandler {
public:
    virtual ~IInputHandler() = default;

    /**
     * @brief Poll input devices and update input state
     * Must be called once per frame at start of game loop
     */
    virtual void PollInput() = 0;

    /**
     * @brief Get current input state
     * @return Current frame's input state
     */
    virtual const InputState& GetInputState() const = 0;

    /**
     * @brief Set input method (mouse, keyboard, controller)
     * @param method Input method to use
     */
    virtual void SetInputMethod(InputMethod method) = 0;

    /**
     * @brief Get current input method
     * @return Active input method
     */
    virtual InputMethod GetInputMethod() const = 0;

    /**
     * @brief Update key bindings from settings
     * @param settings Game settings with key bindings
     */
    virtual void UpdateKeyBindings(const GameSettings& settings) = 0;

    /**
     * @brief Check if quit event occurred (window close, Alt+F4, etc.)
     * @return true if user requested quit
     */
    virtual bool IsQuitRequested() const = 0;
};

} // namespace wreckingball

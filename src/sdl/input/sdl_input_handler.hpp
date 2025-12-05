// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/input/input_interface.hpp"
#include <SDL2/SDL.h>

namespace wreckingball {

/**
 * @brief SDL2 implementation of IInputHandler
 *
 * Handles keyboard, mouse, and gamepad input for paddle control and menu navigation.
 */
class SDLInputHandler : public IInputHandler {
public:
    /**
     * @brief Constructor
     */
    SDLInputHandler();

    /**
     * @brief Destructor
     */
    ~SDLInputHandler() override;

    // Prevent copying
    SDLInputHandler(const SDLInputHandler&) = delete;
    SDLInputHandler& operator=(const SDLInputHandler&) = delete;

    // IInputHandler interface
    void PollInput() override;
    const InputState& GetInputState() const override;
    void SetInputMethod(InputMethod method) override;
    InputMethod GetInputMethod() const override;
    void UpdateKeyBindings(const GameSettings& settings) override;
    bool IsQuitRequested() const override;

private:
    /**
     * @brief Process keyboard input
     */
    void ProcessKeyboard();

    /**
     * @brief Process mouse input
     */
    void ProcessMouse();

    /**
     * @brief Process gamepad input
     */
    void ProcessGamepad();

    InputState input_state_;
    InputMethod input_method_;
    bool quit_requested_;

    // Key bindings (default values, can be customized via settings)
    SDL_Scancode key_move_left_;
    SDL_Scancode key_move_right_;
    SDL_Scancode key_launch_;
    SDL_Scancode key_pause_;
    SDL_Scancode key_confirm_;
    SDL_Scancode key_back_;

    // Previous frame state for edge detection
    bool prev_launch_key_;
    bool prev_pause_key_;
    bool prev_confirm_key_;
    bool prev_back_key_;
    bool prev_mouse_left_;
};

} // namespace wreckingball

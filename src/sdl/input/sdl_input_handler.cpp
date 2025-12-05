// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "sdl_input_handler.hpp"
#include "../../core/entities/game_settings.hpp"
#include <cstring>

namespace wreckingball {

SDLInputHandler::SDLInputHandler()
    : input_method_(InputMethod::Keyboard),
      quit_requested_(false),
      key_move_left_(SDL_SCANCODE_LEFT),
      key_move_right_(SDL_SCANCODE_RIGHT),
      key_launch_(SDL_SCANCODE_SPACE),
      key_pause_(SDL_SCANCODE_ESCAPE),
      key_confirm_(SDL_SCANCODE_RETURN),
      key_back_(SDL_SCANCODE_ESCAPE),
      prev_launch_key_(false),
      prev_pause_key_(false),
      prev_confirm_key_(false),
      prev_back_key_(false),
      prev_mouse_left_(false) {

    // Initialize input state to neutral
    std::memset(&input_state_, 0, sizeof(InputState));
    input_state_.paddle_movement = 0.0f;
}

SDLInputHandler::~SDLInputHandler() = default;

void SDLInputHandler::PollInput() {
    // Reset frame-based input flags
    input_state_.launch_ball = false;
    input_state_.pause_toggle = false;
    input_state_.confirm = false;
    input_state_.back = false;
    input_state_.paddle_movement = 0.0f;

    // Process SDL events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit_requested_ = true;
        }
    }

    // Process input based on active input method
    switch (input_method_) {
        case InputMethod::Keyboard:
            ProcessKeyboard();
            break;

        case InputMethod::Mouse:
            ProcessMouse();
            break;

        case InputMethod::Controller:
            ProcessGamepad();
            break;
    }
}

void SDLInputHandler::ProcessKeyboard() {
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

    // Paddle movement (continuous)
    if (keyboard_state[key_move_left_]) {
        input_state_.paddle_movement = -1.0f;  // Move left
    } else if (keyboard_state[key_move_right_]) {
        input_state_.paddle_movement = 1.0f;   // Move right
    }

    // Launch ball (edge-triggered)
    bool launch_key = keyboard_state[key_launch_];
    if (launch_key && !prev_launch_key_) {
        input_state_.launch_ball = true;
    }
    prev_launch_key_ = launch_key;

    // Pause toggle (edge-triggered)
    bool pause_key = keyboard_state[key_pause_];
    if (pause_key && !prev_pause_key_) {
        input_state_.pause_toggle = true;
    }
    prev_pause_key_ = pause_key;

    // Confirm button (edge-triggered)
    bool confirm_key = keyboard_state[key_confirm_];
    if (confirm_key && !prev_confirm_key_) {
        input_state_.confirm = true;
    }
    prev_confirm_key_ = confirm_key;

    // Back button (edge-triggered)
    bool back_key = keyboard_state[key_back_];
    if (back_key && !prev_back_key_) {
        input_state_.back = true;
    }
    prev_back_key_ = back_key;
}

void SDLInputHandler::ProcessMouse() {
    int mouse_x, mouse_y;
    Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    // Update mouse position
    input_state_.mouse_position.x = mouse_x;
    input_state_.mouse_position.y = mouse_y;

    // Mouse X movement for paddle control
    static int prev_mouse_x = mouse_x;
    int delta_x = mouse_x - prev_mouse_x;
    prev_mouse_x = mouse_x;

    if (delta_x < 0) {
        input_state_.paddle_movement = -1.0f;  // Move left
    } else if (delta_x > 0) {
        input_state_.paddle_movement = 1.0f;   // Move right
    }

    // Left mouse button for launch/confirm
    bool mouse_left = (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    if (mouse_left && !prev_mouse_left_) {
        input_state_.launch_ball = true;
        input_state_.confirm = true;
    }
    prev_mouse_left_ = mouse_left;

    // Right mouse button for back
    if (mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        input_state_.back = true;
    }

    // ESC key for pause/back (always available)
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);
    bool pause_key = keyboard_state[SDL_SCANCODE_ESCAPE];
    if (pause_key && !prev_pause_key_) {
        input_state_.pause_toggle = true;
        input_state_.back = true;
    }
    prev_pause_key_ = pause_key;
}

void SDLInputHandler::ProcessGamepad() {
    // TODO: Implement gamepad support (SDL_GameController API)
    // For now, fall back to keyboard
    ProcessKeyboard();
}

const InputState& SDLInputHandler::GetInputState() const {
    return input_state_;
}

void SDLInputHandler::SetInputMethod(InputMethod method) {
    input_method_ = method;
}

InputMethod SDLInputHandler::GetInputMethod() const {
    return input_method_;
}

void SDLInputHandler::UpdateKeyBindings(const GameSettings& settings) {
    // Update key bindings from settings
    // For now, use default bindings
    // TODO: Add customizable key bindings to GameSettings
    (void)settings;  // Unused for now
}

bool SDLInputHandler::IsQuitRequested() const {
    return quit_requested_;
}

} // namespace wreckingball

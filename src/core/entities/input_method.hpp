// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace wreckingball {

/**
 * @brief Input method types
 */
enum class InputMethod : uint8_t {
    Mouse = 0,      // Mouse control (default)
    Keyboard = 1,   // Keyboard arrow keys / WASD
    Controller = 2  // Game controller (SDL_GameController)
};

} // namespace wreckingball

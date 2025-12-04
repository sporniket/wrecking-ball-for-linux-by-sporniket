// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace wreckingball {

/**
 * @brief Brick type enumeration
 */
enum class BrickType : uint8_t {
    Empty = 0,      // No brick (empty space in grid)
    Regular = 1,    // Standard breakable brick (1-9 hits)
    Star = 2,       // Star-shaped brick (for "Reach for the Stars" levels)
    Key = 3,        // Key-shaped brick (for "Get the Key" levels)
    Exit = 4        // Exit brick (for "Get the Key" levels, activated after all keys collected)
};

} // namespace wreckingball

// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "brick_type.hpp"
#include <cstdint>

namespace wreckingball {

/**
 * @brief Represents a destructible block in the game
 *
 * Bricks have variable width (1-40 units) but fixed height (1 unit).
 * Each brick type has specific behavior for level completion.
 */
struct Brick {
    uint8_t position_x;      // X coordinate in grid units (0-39)
    uint8_t position_y;      // Y coordinate in grid units (0-24)
    uint8_t width;           // Width in grid units (1-40)
    uint8_t height;          // Height in grid units (always 1)
    BrickType type;          // Brick type (regular, star, key, exit)
    uint8_t hits_required;   // Number of hits needed to destroy (1-9)
    uint8_t hits_remaining;  // Current remaining hits (0-hits_required)
    uint8_t color_index;     // Index into 16-color palette (0-15)
    bool is_active;          // Whether brick can be destroyed (exit: false until keys collected)
    bool is_destroyed;       // Whether brick has been destroyed (hits_remaining = 0)

    /**
     * @brief Default constructor
     */
    Brick()
        : position_x(0), position_y(0), width(1), height(1),
          type(BrickType::Empty), hits_required(1), hits_remaining(1),
          color_index(0), is_active(true), is_destroyed(false) {}

    /**
     * @brief Check if brick is within valid grid bounds
     * @return true if brick position and size are valid
     */
    bool IsValid() const {
        return position_x + width <= 40 &&
               position_y >= 1 && position_y <= 24 &&
               width >= 1 && width <= 40 &&
               height == 1 &&
               hits_remaining <= hits_required &&
               color_index < 16;
    }

    /**
     * @brief Apply damage to brick (decrement hits_remaining)
     * @return true if brick was destroyed by this hit
     */
    bool Hit() {
        if (!is_active || is_destroyed || hits_remaining == 0) {
            return false;
        }

        hits_remaining--;
        if (hits_remaining == 0) {
            is_destroyed = true;
            return true;
        }
        return false;
    }
};

} // namespace wreckingball

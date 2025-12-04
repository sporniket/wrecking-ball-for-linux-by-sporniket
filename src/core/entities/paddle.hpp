// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace wreckingball {

/**
 * @brief Represents the player-controlled horizontal bar
 *
 * Paddle moves horizontally within screen bounds.
 * Position is stored in apparent pixel coordinates (320x200).
 */
struct Paddle {
    float position_x;   // Center X position in pixels (width/2 to 320 - width/2)
    float position_y;   // Y position in pixels (fixed at 180.0)
    float width;        // Paddle width in pixels (default: 32.0 = 4 units)
    float height;       // Paddle height in pixels (fixed: 8.0 = 1 unit)
    float velocity_x;   // Current horizontal velocity in pixels/second

    /**
     * @brief Default constructor
     */
    Paddle()
        : position_x(160.0f),  // Center of screen
          position_y(180.0f),  // Near bottom
          width(32.0f),        // 4 grid units
          height(8.0f),        // 1 grid unit
          velocity_x(0.0f) {}

    /**
     * @brief Check if paddle is within valid bounds
     * @return true if paddle position is valid
     */
    bool IsValid() const {
        float half_width = width / 2.0f;
        return position_x >= half_width &&
               position_x <= 320.0f - half_width &&
               width > 0.0f && height > 0.0f;
    }

    /**
     * @brief Get left edge X coordinate
     * @return Left edge position in pixels
     */
    float GetLeftEdge() const {
        return position_x - width / 2.0f;
    }

    /**
     * @brief Get right edge X coordinate
     * @return Right edge position in pixels
     */
    float GetRightEdge() const {
        return position_x + width / 2.0f;
    }

    /**
     * @brief Clamp paddle position to screen bounds
     */
    void ClampToScreen() {
        float half_width = width / 2.0f;
        if (position_x < half_width) {
            position_x = half_width;
            velocity_x = 0.0f;
        } else if (position_x > 320.0f - half_width) {
            position_x = 320.0f - half_width;
            velocity_x = 0.0f;
        }
    }
};

} // namespace wreckingball

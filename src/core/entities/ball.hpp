// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cmath>
#include <cstdint>

namespace wreckingball {

/**
 * @brief Represents a game ball with position and velocity
 *
 * Ball position and velocity use apparent pixel coordinates (320x200).
 * Physics system manages collisions and velocity updates.
 */
struct Ball {
    float position_x;   // X position in pixels (0.0-320.0)
    float position_y;   // Y position in pixels (0.0-200.0)
    float velocity_x;   // Horizontal velocity in pixels/second
    float velocity_y;   // Vertical velocity in pixels/second
    float radius;       // Ball radius in pixels (default: 3.0 = 4px diameter)
    bool is_active;     // true=moving in play, false=attached to paddle
    bool is_lost;       // true=fell below paddle (y > 200)

    /**
     * @brief Default constructor
     */
    Ball()
        : position_x(160.0f), position_y(168.0f),  // Start at paddle position
          velocity_x(0.0f), velocity_y(0.0f),
          radius(3.0f), is_active(false), is_lost(false) {}

    /**
     * @brief Check if ball is within valid screen bounds
     * @return true if ball position is valid
     */
    bool IsValid() const {
        return position_x >= 0.0f && position_x <= 320.0f &&
               position_y >= 0.0f && position_y <= 200.0f &&
               radius > 0.0f;
    }

    /**
     * @brief Get current speed (magnitude of velocity vector)
     * @return Speed in pixels per second
     */
    float GetSpeed() const {
        return std::sqrt(velocity_x * velocity_x + velocity_y * velocity_y);
    }

    /**
     * @brief Launch ball with specified velocity
     * @param vx Initial horizontal velocity
     * @param vy Initial vertical velocity
     */
    void Launch(float vx, float vy) {
        velocity_x = vx;
        velocity_y = vy;
        is_active = true;
        is_lost = false;
    }

    /**
     * @brief Attach ball to paddle (reset state)
     * @param paddle_x Paddle center X position
     * @param paddle_y Paddle Y position
     */
    void AttachToPaddle(float paddle_x, float paddle_y) {
        position_x = paddle_x;
        position_y = paddle_y - 12.0f;  // Position above paddle
        velocity_x = 0.0f;
        velocity_y = 0.0f;
        is_active = false;
        is_lost = false;
    }
};

} // namespace wreckingball

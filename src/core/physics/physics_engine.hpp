// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../entities/ball.hpp"
#include "../entities/paddle.hpp"
#include "../entities/brick.hpp"
#include "../entities/level.hpp"
#include <vector>

namespace wreckingball {

/**
 * @brief Physics engine for ball movement, collisions, and bounces
 *
 * Handles:
 * - Ball velocity updates
 * - Wall and ceiling collision detection
 * - Paddle-ball collision with angle-based bounces
 * - Brick-ball collision with variable-width brick support
 * - Ball loss detection
 * - Speed preservation (elastic collisions)
 */
class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    /**
     * @brief Update ball positions based on velocity
     * @param balls List of balls to update
     * @param delta_time Time step in seconds
     */
    void UpdateBalls(std::vector<Ball>& balls, double delta_time);

    /**
     * @brief Check and handle wall/ceiling collisions
     * @param ball Ball to check
     * @param screen_width Screen width in units
     * @param screen_height Screen height in units
     */
    void CheckWallCollisions(Ball& ball, int screen_width, int screen_height);

    /**
     * @brief Check and handle paddle-ball collision
     * @param ball Ball to check
     * @param paddle Paddle to check against
     * @param ball_speed Target ball speed after bounce
     * @return true if collision occurred
     */
    bool CheckPaddleCollision(Ball& ball, const Paddle& paddle, double ball_speed);

    /**
     * @brief Check and handle brick-ball collisions
     * @param ball Ball to check
     * @param bricks List of bricks to check against
     * @return Pointer to collided brick, or nullptr if no collision
     */
    Brick* CheckBrickCollision(Ball& ball, std::vector<Brick>& bricks);

    /**
     * @brief Check if ball is lost (below paddle threshold)
     * @param ball Ball to check
     * @param paddle_y Paddle Y position
     * @return true if ball is lost
     */
    bool IsBallLost(const Ball& ball, double paddle_y);

    /**
     * @brief Calculate paddle bounce angle and velocity
     * @param ball Ball that hit paddle
     * @param paddle Paddle that was hit
     * @param ball_speed Target ball speed after bounce
     */
    void CalculatePaddleBounce(Ball& ball, const Paddle& paddle, double ball_speed);

    /**
     * @brief Calculate impact position on paddle (-1.0 to 1.0)
     * @param ball_x Ball X position
     * @param paddle Paddle
     * @return Normalized impact position (-1 = left, 0 = center, 1 = right)
     */
    double CalculateImpactPosition(double ball_x, const Paddle& paddle);

private:
    /**
     * @brief Check AABB collision between ball and brick
     * @param ball Ball to check
     * @param brick Brick to check against
     * @return true if collision detected
     */
    bool CheckBallBrickCollision(const Ball& ball, const Brick& brick);

    /**
     * @brief Check AABB collision between ball and paddle
     * @param ball Ball to check
     * @param paddle Paddle to check against
     * @return true if collision detected
     */
    bool CheckBallPaddleCollision(const Ball& ball, const Paddle& paddle);

    /**
     * @brief Determine collision normal for brick bounce
     * @param ball Ball that collided
     * @param brick Brick that was hit
     * @return Collision normal (0 = horizontal, 1 = vertical)
     */
    int DetermineCollisionNormal(const Ball& ball, const Brick& brick);

    /**
     * @brief Normalize velocity to maintain constant speed
     * @param ball Ball to normalize
     * @param target_speed Target speed magnitude
     */
    void NormalizeVelocity(Ball& ball, double target_speed);

    // Physics constants
    static constexpr double MAX_BOUNCE_ANGLE = 75.0;  // Maximum bounce angle in degrees
    static constexpr double PI = 3.14159265358979323846;
};

} // namespace wreckingball

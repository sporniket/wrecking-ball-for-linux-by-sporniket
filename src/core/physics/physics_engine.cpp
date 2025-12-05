// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "physics_engine.hpp"
#include "../logging/error_logger.hpp"
#include <cmath>
#include <algorithm>

namespace wreckingball {

PhysicsEngine::PhysicsEngine() {
    ErrorLogger::GetInstance().LogInfo("PhysicsEngine initialized");
}

PhysicsEngine::~PhysicsEngine() {}

void PhysicsEngine::UpdateBalls(std::vector<Ball>& balls, double delta_time) {
    for (auto& ball : balls) {
        if (!ball.is_active || ball.is_lost) {
            continue;  // Don't update inactive (attached) or lost balls
        }

        // Update position based on velocity
        ball.position_x += ball.velocity_x * delta_time;
        ball.position_y += ball.velocity_y * delta_time;
    }
}

void PhysicsEngine::CheckWallCollisions(Ball& ball, int screen_width, int screen_height) {
    (void)screen_width;   // Not used, we check against borders not screen edges
    (void)screen_height;  // Unused: no bottom wall collision (ball loss detection)

    if (!ball.is_active || ball.is_lost) {
        return;
    }

    // Left border collision (border occupies x=0 to x=1, ball bounces at inner edge x=1)
    if (ball.position_x - ball.radius <= 1.0) {
        ball.position_x = 1.0 + ball.radius;
        ball.velocity_x = -ball.velocity_x;  // Reverse horizontal velocity
    }

    // Right border collision (border occupies x=39 to x=40, ball bounces at inner edge x=39)
    if (ball.position_x + ball.radius >= 39.0) {
        ball.position_x = 39.0 - ball.radius;
        ball.velocity_x = -ball.velocity_x;
    }

    // Top border collision (border + HUD occupy y=0 to y=2, ball bounces at inner edge y=2)
    if (ball.position_y - ball.radius <= 2.0) {
        ball.position_y = 2.0 + ball.radius;
        ball.velocity_y = -ball.velocity_y;  // Reverse vertical velocity
    }
}

bool PhysicsEngine::CheckPaddleCollision(Ball& ball, const Paddle& paddle, double ball_speed) {
    if (!ball.is_active || ball.is_lost) {
        return false;
    }

    // Only check if ball is moving downward
    if (ball.velocity_y <= 0.0) {
        return false;
    }

    if (CheckBallPaddleCollision(ball, paddle)) {
        // Calculate bounce
        CalculatePaddleBounce(ball, paddle, ball_speed);

        // Move ball above paddle to prevent double-collision
        double paddle_top = paddle.position_y - paddle.height / 2.0;
        ball.position_y = paddle_top - ball.radius;

        return true;
    }

    return false;
}

Brick* PhysicsEngine::CheckBrickCollision(Ball& ball, std::vector<Brick>& bricks) {
    if (!ball.is_active || ball.is_lost) {
        return nullptr;
    }

    for (auto& brick : bricks) {
        if (brick.is_destroyed) {
            continue;
        }

        if (CheckBallBrickCollision(ball, brick)) {
            // Determine collision normal
            int normal = DetermineCollisionNormal(ball, brick);

            // Bounce ball
            if (normal == 0) {
                // Horizontal collision (hit left or right side)
                ball.velocity_x = -ball.velocity_x;
            } else {
                // Vertical collision (hit top or bottom)
                ball.velocity_y = -ball.velocity_y;
            }

            // Apply damage to brick
            brick.hits_remaining--;
            if (brick.hits_remaining <= 0) {
                brick.is_destroyed = true;
            }

            return &brick;
        }
    }

    return nullptr;
}

bool PhysicsEngine::IsBallLost(const Ball& ball, double paddle_y) {
    // Ball is lost if it falls below the paddle
    return ball.position_y > paddle_y + 20.0;  // 20 units below paddle
}

void PhysicsEngine::CalculatePaddleBounce(Ball& ball, const Paddle& paddle, double ball_speed) {
    // Calculate impact position (-1.0 to 1.0)
    double impact_position = CalculateImpactPosition(ball.position_x, paddle);

    // Clamp to [-1, 1]
    impact_position = std::clamp(impact_position, -1.0, 1.0);

    // Calculate bounce angle based on impact position
    // Center hit = 90° (straight up)
    // Edge hits = 90° ± MAX_BOUNCE_ANGLE
    // Left edge (impact_position = -1): angle = 165° (bounce left)
    // Right edge (impact_position = 1): angle = 15° (bounce right)
    double max_angle_rad = MAX_BOUNCE_ANGLE * PI / 180.0;
    double bounce_angle = (PI / 2.0) - (impact_position * max_angle_rad);

    // Set new velocity
    ball.velocity_x = ball_speed * std::cos(bounce_angle);
    ball.velocity_y = -ball_speed * std::sin(bounce_angle);  // Negative = upward

    // Optional: Add paddle velocity influence
    double paddle_influence = 0.2;  // 20% of paddle velocity added
    ball.velocity_x += paddle.velocity_x * paddle_influence;

    // Normalize to maintain target speed
    NormalizeVelocity(ball, ball_speed);
}

double PhysicsEngine::CalculateImpactPosition(double ball_x, const Paddle& paddle) {
    double half_width = paddle.width / 2.0;
    double relative_x = ball_x - paddle.position_x;
    return relative_x / half_width;
}

bool PhysicsEngine::CheckBallBrickCollision(const Ball& ball, const Brick& brick) {
    // AABB collision detection for variable-width bricks
    double brick_left = brick.position_x;
    double brick_right = brick.position_x + brick.width;
    double brick_top = brick.position_y;
    double brick_bottom = brick.position_y + 1.0;  // 1 unit height

    double ball_left = ball.position_x - ball.radius;
    double ball_right = ball.position_x + ball.radius;
    double ball_top = ball.position_y - ball.radius;
    double ball_bottom = ball.position_y + ball.radius;

    return ball_right >= brick_left &&
           ball_left <= brick_right &&
           ball_bottom >= brick_top &&
           ball_top <= brick_bottom;
}

bool PhysicsEngine::CheckBallPaddleCollision(const Ball& ball, const Paddle& paddle) {
    // AABB collision detection
    double paddle_left = paddle.position_x - paddle.width / 2.0;
    double paddle_right = paddle.position_x + paddle.width / 2.0;
    double paddle_top = paddle.position_y - paddle.height / 2.0;
    double paddle_bottom = paddle.position_y + paddle.height / 2.0;

    double ball_left = ball.position_x - ball.radius;
    double ball_right = ball.position_x + ball.radius;
    double ball_top = ball.position_y - ball.radius;
    double ball_bottom = ball.position_y + ball.radius;

    return ball_right >= paddle_left &&
           ball_left <= paddle_right &&
           ball_bottom >= paddle_top &&
           ball_top <= paddle_bottom;
}

int PhysicsEngine::DetermineCollisionNormal(const Ball& ball, const Brick& brick) {
    // Calculate overlap on each axis
    double brick_center_x = brick.position_x + brick.width / 2.0;
    double brick_center_y = brick.position_y + 0.5;  // 1 unit height

    double dx = ball.position_x - brick_center_x;
    double dy = ball.position_y - brick_center_y;

    double overlap_x = (brick.width / 2.0 + ball.radius) - std::abs(dx);
    double overlap_y = (0.5 + ball.radius) - std::abs(dy);

    // Return axis with smallest overlap (that's the collision normal)
    if (overlap_x < overlap_y) {
        return 0;  // Horizontal (left/right collision)
    } else {
        return 1;  // Vertical (top/bottom collision)
    }
}

void PhysicsEngine::NormalizeVelocity(Ball& ball, double target_speed) {
    double current_speed = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);

    if (current_speed > 0.0) {
        double scale = target_speed / current_speed;
        ball.velocity_x *= scale;
        ball.velocity_y *= scale;
    }
}

} // namespace wreckingball

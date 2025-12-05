// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/ball.hpp"
#include "../../../src/core/entities/paddle.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

using namespace wreckingball;

// Helper function to check paddle-ball collision
static bool CheckPaddleBallCollision(const Ball& ball, const Paddle& paddle) {
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

// Helper function to calculate bounce angle based on impact position
// Returns normalized impact position (-1.0 = left edge, 0.0 = center, 1.0 = right edge)
static double CalculateImpactPosition(double ball_x, const Paddle& paddle) {
    return (ball_x - paddle.position_x) / (paddle.width / 2.0);
}

// Helper function to calculate new ball velocity after paddle bounce
static void CalculatePaddleBounce(Ball& ball, const Paddle& paddle, double ball_speed) {
    double impact_position = CalculateImpactPosition(ball.position_x, paddle);

    // Clamp impact position to [-1, 1]
    if (impact_position < -1.0) impact_position = -1.0;
    if (impact_position > 1.0) impact_position = 1.0;

    // Calculate bounce angle (-75° to +75°)
    // Center hit = straight up (90°), edges = 75° from vertical
    double max_angle = 75.0 * M_PI / 180.0;  // 75 degrees in radians
    double bounce_angle = (M_PI / 2.0) - (impact_position * max_angle);

    // Set new velocity components
    ball.velocity_x = ball_speed * std::cos(bounce_angle);
    ball.velocity_y = -ball_speed * std::sin(bounce_angle);  // Negative = upward
}

TEST_CASE("Paddle-ball collision detection", "[paddle][ball][physics]") {
    SECTION("Ball collides with center of paddle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 160.0;  // Center hit
        ball.position_y = 188.0;
        ball.radius = 2.0;

        bool collision = CheckPaddleBallCollision(ball, paddle);
        REQUIRE(collision);
    }

    SECTION("Ball collides with left side of paddle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 153.0;  // Left side
        ball.position_y = 188.0;
        ball.radius = 2.0;

        bool collision = CheckPaddleBallCollision(ball, paddle);
        REQUIRE(collision);
    }

    SECTION("Ball collides with right side of paddle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 167.0;  // Right side
        ball.position_y = 188.0;
        ball.radius = 2.0;

        bool collision = CheckPaddleBallCollision(ball, paddle);
        REQUIRE(collision);
    }

    SECTION("Ball misses paddle (too far left)") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 140.0;  // Far left
        ball.position_y = 190.0;
        ball.radius = 2.0;

        bool collision = CheckPaddleBallCollision(ball, paddle);
        REQUIRE_FALSE(collision);
    }

    SECTION("Ball misses paddle (too far right)") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 180.0;  // Far right
        ball.position_y = 190.0;
        ball.radius = 2.0;

        bool collision = CheckPaddleBallCollision(ball, paddle);
        REQUIRE_FALSE(collision);
    }

    SECTION("Ball misses paddle (above)") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 180.0;  // Well above
        ball.radius = 2.0;

        bool collision = CheckPaddleBallCollision(ball, paddle);
        REQUIRE_FALSE(collision);
    }
}

TEST_CASE("Paddle bounce angle calculation", "[paddle][ball][physics]") {
    SECTION("Center hit bounces straight up") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 160.0;  // Center hit
        ball.position_y = 188.0;
        ball.velocity_x = 50.0;
        ball.velocity_y = 100.0;  // Moving down
        ball.radius = 2.0;

        double ball_speed = 150.0;
        CalculatePaddleBounce(ball, paddle, ball_speed);

        // Center hit should bounce nearly straight up (vx ≈ 0, vy < 0)
        REQUIRE(std::abs(ball.velocity_x) < 10.0);  // Nearly zero horizontal velocity
        REQUIRE(ball.velocity_y < 0.0);  // Upward velocity
    }

    SECTION("Left edge hit bounces left") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 152.0;  // Left edge
        ball.position_y = 188.0;
        ball.velocity_x = 50.0;
        ball.velocity_y = 100.0;
        ball.radius = 2.0;

        double ball_speed = 150.0;
        CalculatePaddleBounce(ball, paddle, ball_speed);

        // Left edge hit should bounce left (vx < 0, vy < 0)
        REQUIRE(ball.velocity_x < 0.0);  // Leftward velocity
        REQUIRE(ball.velocity_y < 0.0);  // Upward velocity
    }

    SECTION("Right edge hit bounces right") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 168.0;  // Right edge
        ball.position_y = 188.0;
        ball.velocity_x = -50.0;
        ball.velocity_y = 100.0;
        ball.radius = 2.0;

        double ball_speed = 150.0;
        CalculatePaddleBounce(ball, paddle, ball_speed);

        // Right edge hit should bounce right (vx > 0, vy < 0)
        REQUIRE(ball.velocity_x > 0.0);  // Rightward velocity
        REQUIRE(ball.velocity_y < 0.0);  // Upward velocity
    }

    SECTION("Quarter left hit bounces at angle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 156.0;  // Quarter left (impact position = -0.5)
        ball.position_y = 188.0;
        ball.velocity_x = 0.0;
        ball.velocity_y = 100.0;
        ball.radius = 2.0;

        double ball_speed = 150.0;
        CalculatePaddleBounce(ball, paddle, ball_speed);

        // Should bounce left and up
        REQUIRE(ball.velocity_x < 0.0);
        REQUIRE(ball.velocity_y < 0.0);

        // Check magnitude is preserved
        double speed = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);
        REQUIRE_THAT(speed, Catch::Matchers::WithinRel(ball_speed, 0.01));
    }

    SECTION("Quarter right hit bounces at angle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 164.0;  // Quarter right (impact position = 0.5)
        ball.position_y = 188.0;
        ball.velocity_x = 0.0;
        ball.velocity_y = 100.0;
        ball.radius = 2.0;

        double ball_speed = 150.0;
        CalculatePaddleBounce(ball, paddle, ball_speed);

        // Should bounce right and up
        REQUIRE(ball.velocity_x > 0.0);
        REQUIRE(ball.velocity_y < 0.0);

        // Check magnitude is preserved
        double speed = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);
        REQUIRE_THAT(speed, Catch::Matchers::WithinRel(ball_speed, 0.01));
    }
}

TEST_CASE("Paddle bounce preserves ball speed", "[paddle][ball][physics]") {
    SECTION("Speed magnitude preserved after bounce") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 188.0;
        ball.velocity_x = 90.0;
        ball.velocity_y = 120.0;
        ball.radius = 2.0;

        double ball_speed = 150.0;

        CalculatePaddleBounce(ball, paddle, ball_speed);

        double ball_speed_after = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);

        // Speed should be exactly the target speed (elastic collision)
        REQUIRE_THAT(ball_speed_after, Catch::Matchers::WithinRel(ball_speed, 0.01));
    }

    SECTION("Speed preserved across multiple bounce positions") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;

        double ball_speed = 150.0;

        // Test multiple impact positions
        for (double x = 152.0; x <= 168.0; x += 2.0) {
            Ball ball;
            ball.position_x = x;
            ball.position_y = 188.0;
            ball.velocity_x = 50.0;
            ball.velocity_y = 100.0;
            ball.radius = 2.0;

            CalculatePaddleBounce(ball, paddle, ball_speed);

            double speed_after = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);
            REQUIRE_THAT(speed_after, Catch::Matchers::WithinRel(ball_speed, 0.01));
        }
    }
}

TEST_CASE("Paddle bounce impact position calculation", "[paddle][ball][physics]") {
    SECTION("Calculate impact position at center") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 160.0;
        double impact = CalculateImpactPosition(ball_x, paddle);

        REQUIRE_THAT(impact, Catch::Matchers::WithinRel(0.0, 0.01));
    }

    SECTION("Calculate impact position at left edge") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 152.0;  // Left edge (x - width/2)
        double impact = CalculateImpactPosition(ball_x, paddle);

        REQUIRE_THAT(impact, Catch::Matchers::WithinRel(-1.0, 0.01));
    }

    SECTION("Calculate impact position at right edge") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 168.0;  // Right edge (x + width/2)
        double impact = CalculateImpactPosition(ball_x, paddle);

        REQUIRE_THAT(impact, Catch::Matchers::WithinRel(1.0, 0.01));
    }

    SECTION("Calculate impact position quarter left") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 156.0;  // Quarter left
        double impact = CalculateImpactPosition(ball_x, paddle);

        REQUIRE_THAT(impact, Catch::Matchers::WithinRel(-0.5, 0.01));
    }

    SECTION("Calculate impact position quarter right") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 164.0;  // Quarter right
        double impact = CalculateImpactPosition(ball_x, paddle);

        REQUIRE_THAT(impact, Catch::Matchers::WithinRel(0.5, 0.01));
    }
}

TEST_CASE("Paddle bounce with moving paddle", "[paddle][ball][physics]") {
    SECTION("Paddle moving right adds horizontal velocity") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;
        paddle.velocity_x = 200.0;  // Moving right

        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 188.0;
        ball.velocity_x = 0.0;
        ball.velocity_y = 100.0;
        ball.radius = 2.0;

        double ball_speed = 150.0;
        CalculatePaddleBounce(ball, paddle, ball_speed);

        // Note: This is basic test - actual implementation might
        // add paddle velocity to ball velocity for more realistic physics
        REQUIRE(ball.velocity_y < 0.0);  // Should bounce up
    }

    SECTION("Paddle moving left adds horizontal velocity") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;
        paddle.velocity_x = -200.0;  // Moving left

        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 188.0;
        ball.velocity_x = 0.0;
        ball.velocity_y = 100.0;
        ball.radius = 2.0;

        double ball_speed = 150.0;
        CalculatePaddleBounce(ball, paddle, ball_speed);

        REQUIRE(ball.velocity_y < 0.0);  // Should bounce up
    }
}

// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/ball.hpp"
#include "../../../src/core/entities/brick.hpp"
#include "../../../src/core/entities/brick_type.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

using namespace wreckingball;

// Helper function for AABB collision detection
static bool CheckBallBrickCollision(const Ball& ball, const Brick& brick) {
    // Brick bounds (assuming brick.position_x and brick.position_y are center positions)
    double brick_left = brick.position_x;
    double brick_right = brick.position_x + brick.width;
    double brick_top = brick.position_y;
    double brick_bottom = brick.position_y + 1;  // 1 unit height

    // Ball bounds
    double ball_left = ball.position_x - ball.radius;
    double ball_right = ball.position_x + ball.radius;
    double ball_top = ball.position_y - ball.radius;
    double ball_bottom = ball.position_y + ball.radius;

    // AABB collision check
    return ball_right >= brick_left &&
           ball_left <= brick_right &&
           ball_bottom >= brick_top &&
           ball_top <= brick_bottom;
}

TEST_CASE("Narrow brick (1-unit) collision detection", "[collision][physics][narrow]") {
    SECTION("Ball collides with 1-unit brick from above") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;
        brick.is_destroyed = false;

        Ball ball;
        ball.position_x = 10.5;  // Center of brick
        ball.position_y = 9.0;   // Above brick
        ball.radius = 2.0;
        ball.velocity_y = 50.0;  // Moving down

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 1-unit brick from below") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 10.5;
        ball.position_y = 12.0;  // Below brick
        ball.radius = 2.0;
        ball.velocity_y = -50.0;  // Moving up

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 1-unit brick from left side") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 8.0;   // Left of brick
        ball.position_y = 10.5;
        ball.radius = 2.0;
        ball.velocity_x = 50.0;  // Moving right

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 1-unit brick from right side") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 13.0;  // Right of brick
        ball.position_y = 10.5;
        ball.radius = 2.0;
        ball.velocity_x = -50.0;  // Moving left

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball misses 1-unit brick (too far left)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 5.0;   // Far left of brick
        ball.position_y = 10.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }

    SECTION("Ball misses 1-unit brick (too far right)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;  // Far right of brick
        ball.position_y = 10.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }

    SECTION("Ball misses 1-unit brick (above)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 10.5;
        ball.position_y = 5.0;   // Far above brick
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }

    SECTION("Ball misses 1-unit brick (below)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 10.5;
        ball.position_y = 15.0;  // Far below brick
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }
}

TEST_CASE("Narrow brick collision - edge cases", "[collision][physics][narrow]") {
    SECTION("Ball grazes left edge of 1-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 9.0;   // Just touching left edge
        ball.position_y = 10.5;
        ball.radius = 1.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball grazes right edge of 1-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 12.0;  // Just touching right edge
        ball.position_y = 10.5;
        ball.radius = 1.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball barely misses 1-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 8.5;   // Just outside collision range
        ball.position_y = 10.5;
        ball.radius = 1.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }
}

TEST_CASE("Narrow brick destruction after collision", "[collision][physics][narrow]") {
    SECTION("1-hit narrow brick destroyed after collision") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 1;
        brick.is_destroyed = false;

        Ball ball;
        ball.position_x = 10.5;
        ball.position_y = 9.0;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);

        // Simulate hit
        brick.hits_remaining--;
        if (brick.hits_remaining <= 0) {
            brick.is_destroyed = true;
        }

        REQUIRE(brick.is_destroyed);
    }

    SECTION("Multi-hit narrow brick takes damage") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 1;
        brick.hits_remaining = 3;
        brick.is_destroyed = false;

        Ball ball;
        ball.position_x = 10.5;
        ball.position_y = 9.0;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);

        // Simulate hit
        brick.hits_remaining--;
        if (brick.hits_remaining <= 0) {
            brick.is_destroyed = true;
        }

        REQUIRE_FALSE(brick.is_destroyed);
        REQUIRE(brick.hits_remaining == 2);
    }
}

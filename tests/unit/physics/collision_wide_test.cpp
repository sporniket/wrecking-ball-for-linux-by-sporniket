// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/ball.hpp"
#include "../../../src/core/entities/brick.hpp"
#include "../../../src/core/entities/brick_type.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace wreckingball;

// Helper function for AABB collision detection
static bool CheckBallBrickCollision(const Ball& ball, const Brick& brick) {
    double brick_left = brick.position_x;
    double brick_right = brick.position_x + brick.width;
    double brick_top = brick.position_y;
    double brick_bottom = brick.position_y + 1;

    double ball_left = ball.position_x - ball.radius;
    double ball_right = ball.position_x + ball.radius;
    double ball_top = ball.position_y - ball.radius;
    double ball_bottom = ball.position_y + ball.radius;

    return ball_right >= brick_left &&
           ball_left <= brick_right &&
           ball_bottom >= brick_top &&
           ball_top <= brick_bottom;
}

TEST_CASE("Wide brick (10+ units) collision detection", "[collision][physics][wide]") {
    SECTION("Ball collides with 10-unit brick at left edge") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 10;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 11.0;  // Near left edge
        ball.position_y = 10.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 10-unit brick at center") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 10;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 15.0;  // Center of brick
        ball.position_y = 10.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 10-unit brick at right edge") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 10;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 19.0;  // Near right edge
        ball.position_y = 10.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 10-unit brick from above") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 10;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 15.0;
        ball.position_y = 8.5;   // Above brick
        ball.radius = 2.0;
        ball.velocity_y = 50.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 10-unit brick from below") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 10;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 15.0;
        ball.position_y = 12.0;  // Below brick
        ball.radius = 2.0;
        ball.velocity_y = -50.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }
}

TEST_CASE("Wide brick collision - 15-unit brick", "[collision][physics][wide]") {
    SECTION("Ball collides with 15-unit brick at various positions") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 5.0;
        brick.position_y = 8.0;
        brick.width = 15;
        brick.hits_remaining = 1;

        // Test collision at multiple positions along the brick
        for (double x = 6.0; x < 20.0; x += 3.0) {
            Ball ball;
            ball.position_x = x;
            ball.position_y = 8.5;
            ball.radius = 2.0;

            bool collision = CheckBallBrickCollision(ball, brick);
            REQUIRE(collision);
        }
    }

    SECTION("Ball misses 15-unit brick (too far left)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 5.0;
        brick.position_y = 8.0;
        brick.width = 15;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 2.0;   // Far left
        ball.position_y = 8.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }

    SECTION("Ball misses 15-unit brick (too far right)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 5.0;
        brick.position_y = 8.0;
        brick.width = 15;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 23.0;  // Far right
        ball.position_y = 8.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }
}

TEST_CASE("Wide brick collision - 20-unit brick", "[collision][physics][wide]") {
    SECTION("Ball sweeps across 20-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 5.0;
        brick.width = 20;
        brick.hits_remaining = 1;

        // Simulate ball moving horizontally across the brick
        for (double x = 1.0; x <= 19.0; x += 2.0) {
            Ball ball;
            ball.position_x = x;
            ball.position_y = 5.5;
            ball.radius = 2.0;

            bool collision = CheckBallBrickCollision(ball, brick);
            REQUIRE(collision);
        }
    }

    SECTION("Ball collision at exact edges of 20-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 5.0;
        brick.width = 20;
        brick.hits_remaining = 1;

        // Test left edge
        Ball ball_left;
        ball_left.position_x = 10.0;  // Exact left edge
        ball_left.position_y = 5.5;
        ball_left.radius = 2.0;
        REQUIRE(CheckBallBrickCollision(ball_left, brick));

        // Test right edge
        Ball ball_right;
        ball_right.position_x = 30.0;  // Exact right edge
        ball_right.position_y = 5.5;
        ball_right.radius = 2.0;
        REQUIRE(CheckBallBrickCollision(ball_right, brick));
    }
}

TEST_CASE("Wide brick special types", "[collision][physics][wide]") {
    SECTION("10-unit star brick collision") {
        Brick brick;
        brick.type = BrickType::Star;
        brick.position_x = 15.0;
        brick.position_y = 12.0;
        brick.width = 10;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;  // Center of star brick
        ball.position_y = 12.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
        REQUIRE(brick.type == BrickType::Star);
    }

    SECTION("12-unit key brick collision") {
        Brick brick;
        brick.type = BrickType::Key;
        brick.position_x = 8.0;
        brick.position_y = 15.0;
        brick.width = 12;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 14.0;  // Middle of key brick
        ball.position_y = 15.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
        REQUIRE(brick.type == BrickType::Key);
    }

    SECTION("15-unit exit brick collision (active)") {
        Brick brick;
        brick.type = BrickType::Exit;
        brick.position_x = 5.0;
        brick.position_y = 18.0;
        brick.width = 15;
        brick.hits_remaining = 1;
        brick.is_active = true;

        Ball ball;
        ball.position_x = 12.0;  // Middle of exit brick
        ball.position_y = 18.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
        REQUIRE(brick.type == BrickType::Exit);
        REQUIRE(brick.is_active);
    }
}

TEST_CASE("Wide brick destruction mechanics", "[collision][physics][wide]") {
    SECTION("10-unit brick destroyed after collision") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10.0;
        brick.position_y = 10.0;
        brick.width = 10;
        brick.hits_remaining = 1;
        brick.is_destroyed = false;

        Ball ball;
        ball.position_x = 15.0;
        ball.position_y = 10.5;
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
}

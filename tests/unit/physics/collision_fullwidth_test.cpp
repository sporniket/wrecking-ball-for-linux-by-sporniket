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

TEST_CASE("Full-width brick (40 units) collision detection", "[collision][physics][fullwidth]") {
    SECTION("Ball collides with 40-unit brick at far left") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 5.0;
        brick.width = 40;  // Full width (38 units play area + 1 unit borders)
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 2.0;   // Near left edge
        ball.position_y = 5.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 40-unit brick at center") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 5.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;  // Center of play area
        ball.position_y = 5.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball collides with 40-unit brick at far right") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 5.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 38.0;  // Near right edge
        ball.position_y = 5.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball sweeps entire width of 40-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        // Simulate ball moving across entire width
        for (double x = 1.0; x <= 39.0; x += 5.0) {
            Ball ball;
            ball.position_x = x;
            ball.position_y = 10.5;
            ball.radius = 2.0;

            bool collision = CheckBallBrickCollision(ball, brick);
            REQUIRE(collision);
        }
    }
}

TEST_CASE("Full-width brick collision from different angles", "[collision][physics][fullwidth]") {
    SECTION("Ball hits 40-unit brick from above") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 15.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;
        ball.position_y = 13.5;  // Above brick
        ball.radius = 2.0;
        ball.velocity_y = 100.0;  // Moving down

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball hits 40-unit brick from below") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 15.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;
        ball.position_y = 17.0;  // Below brick
        ball.radius = 2.0;
        ball.velocity_y = -100.0;  // Moving up

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball approaches 40-unit brick at angle") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 12.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 15.0;
        ball.position_y = 10.5;
        ball.radius = 2.0;
        ball.velocity_x = 80.0;   // Moving diagonally
        ball.velocity_y = 100.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }
}

TEST_CASE("Full-width brick edge cases", "[collision][physics][fullwidth]") {
    SECTION("Ball at exact left boundary of 40-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 8.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 0.0;   // Exact left edge
        ball.position_y = 8.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball at exact right boundary of 40-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 8.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 40.0;  // Exact right edge
        ball.position_y = 8.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball grazes top of 40-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;
        ball.position_y = 9.0;   // Just touching top
        ball.radius = 1.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }

    SECTION("Ball grazes bottom of 40-unit brick") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;
        ball.position_y = 12.0;  // Just touching bottom
        ball.radius = 1.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
    }
}

TEST_CASE("Full-width brick - no collision scenarios", "[collision][physics][fullwidth]") {
    SECTION("Ball above 40-unit brick (no collision)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;
        ball.position_y = 5.0;   // Well above brick
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }

    SECTION("Ball below 40-unit brick (no collision)") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 20.0;
        ball.position_y = 15.0;  // Well below brick
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE_FALSE(collision);
    }
}

TEST_CASE("Full-width brick special types", "[collision][physics][fullwidth]") {
    SECTION("40-unit star brick collision") {
        Brick brick;
        brick.type = BrickType::Star;
        brick.position_x = 0.0;
        brick.position_y = 8.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 25.0;
        ball.position_y = 8.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
        REQUIRE(brick.type == BrickType::Star);
    }

    SECTION("40-unit key brick collision") {
        Brick brick;
        brick.type = BrickType::Key;
        brick.position_x = 0.0;
        brick.position_y = 12.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        Ball ball;
        ball.position_x = 18.0;
        ball.position_y = 12.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
        REQUIRE(brick.type == BrickType::Key);
    }

    SECTION("40-unit exit brick collision (inactive - should not complete level)") {
        Brick brick;
        brick.type = BrickType::Exit;
        brick.position_x = 0.0;
        brick.position_y = 16.0;
        brick.width = 40;
        brick.hits_remaining = 1;
        brick.is_active = false;  // Not active yet

        Ball ball;
        ball.position_x = 30.0;
        ball.position_y = 16.5;
        ball.radius = 2.0;

        bool collision = CheckBallBrickCollision(ball, brick);
        REQUIRE(collision);
        REQUIRE(brick.type == BrickType::Exit);
        REQUIRE_FALSE(brick.is_active);  // Should not be active
    }
}

TEST_CASE("Full-width brick destruction mechanics", "[collision][physics][fullwidth]") {
    SECTION("40-unit brick destroyed after single hit") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 1;
        brick.is_destroyed = false;

        Ball ball;
        ball.position_x = 20.0;
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

    SECTION("40-unit multi-hit brick takes multiple hits") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 3;
        brick.is_destroyed = false;

        // First hit at left
        Ball ball1;
        ball1.position_x = 5.0;
        ball1.position_y = 10.5;
        ball1.radius = 2.0;
        REQUIRE(CheckBallBrickCollision(ball1, brick));
        brick.hits_remaining--;
        REQUIRE(brick.hits_remaining == 2);

        // Second hit at center
        Ball ball2;
        ball2.position_x = 20.0;
        ball2.position_y = 10.5;
        ball2.radius = 2.0;
        REQUIRE(CheckBallBrickCollision(ball2, brick));
        brick.hits_remaining--;
        REQUIRE(brick.hits_remaining == 1);

        // Third hit at right
        Ball ball3;
        ball3.position_x = 35.0;
        ball3.position_y = 10.5;
        ball3.radius = 2.0;
        REQUIRE(CheckBallBrickCollision(ball3, brick));
        brick.hits_remaining--;
        if (brick.hits_remaining <= 0) {
            brick.is_destroyed = true;
        }

        REQUIRE(brick.is_destroyed);
    }
}

TEST_CASE("Full-width brick performance considerations", "[collision][physics][fullwidth]") {
    SECTION("Multiple balls colliding with 40-unit brick simultaneously") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0.0;
        brick.position_y = 10.0;
        brick.width = 40;
        brick.hits_remaining = 1;

        // Create 5 balls at different positions
        Ball balls[5];
        for (int i = 0; i < 5; ++i) {
            balls[i].position_x = 8.0 * (i + 1);  // Spread across width
            balls[i].position_y = 10.5;
            balls[i].radius = 2.0;

            bool collision = CheckBallBrickCollision(balls[i], brick);
            REQUIRE(collision);
        }
    }
}

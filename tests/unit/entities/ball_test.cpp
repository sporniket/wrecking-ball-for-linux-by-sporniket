// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/ball.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>

using namespace wreckingball;

TEST_CASE("Ball entity construction", "[ball][entity]") {
    SECTION("Create ball with default properties") {
        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 180.0;
        ball.velocity_x = 100.0;
        ball.velocity_y = -150.0;
        ball.radius = 2.0;
        ball.is_active = false;
        ball.is_lost = false;

        REQUIRE_THAT(ball.position_x, Catch::Matchers::WithinRel(160.0, 0.01));
        REQUIRE_THAT(ball.position_y, Catch::Matchers::WithinRel(180.0, 0.01));
        REQUIRE_THAT(ball.velocity_x, Catch::Matchers::WithinRel(100.0, 0.01));
        REQUIRE_THAT(ball.velocity_y, Catch::Matchers::WithinRel(-150.0, 0.01));
        REQUIRE_THAT(ball.radius, Catch::Matchers::WithinRel(2.0, 0.01));
        REQUIRE_FALSE(ball.is_active);
        REQUIRE_FALSE(ball.is_lost);
    }

    SECTION("Create attached ball (starts on paddle)") {
        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 180.0;
        ball.velocity_x = 0.0;
        ball.velocity_y = 0.0;
        ball.is_active = true;

        REQUIRE(ball.is_active);
        REQUIRE_THAT(ball.velocity_x, Catch::Matchers::WithinRel(0.0, 0.01));
        REQUIRE_THAT(ball.velocity_y, Catch::Matchers::WithinRel(0.0, 0.01));
    }
}

TEST_CASE("Ball movement", "[ball][entity]") {
    SECTION("Ball moves based on velocity") {
        Ball ball;
        ball.position_x = 100.0;
        ball.position_y = 100.0;
        ball.velocity_x = 50.0;
        ball.velocity_y = -75.0;

        double dt = 1.0 / 60.0;  // 60 FPS fixed timestep

        // Update position
        ball.position_x += ball.velocity_x * dt;
        ball.position_y += ball.velocity_y * dt;

        REQUIRE_THAT(ball.position_x, Catch::Matchers::WithinRel(100.0 + 50.0 * dt, 0.01));
        REQUIRE_THAT(ball.position_y, Catch::Matchers::WithinRel(100.0 - 75.0 * dt, 0.01));
    }

    SECTION("Ball velocity magnitude remains constant after bounce") {
        Ball ball;
        ball.velocity_x = 100.0;
        ball.velocity_y = -150.0;

        double speed_before = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);

        // Simulate horizontal bounce (reverse vx)
        ball.velocity_x = -ball.velocity_x;

        double speed_after = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);

        REQUIRE_THAT(speed_before, Catch::Matchers::WithinRel(speed_after, 0.01));
    }
}

TEST_CASE("Ball attachment to paddle", "[ball][entity]") {
    SECTION("Attached ball stays with paddle") {
        Ball ball;
        ball.is_active = true;
        ball.position_x = 160.0;
        ball.position_y = 180.0;

        // Paddle moves
        double paddle_x = 170.0;
        ball.position_x = paddle_x;

        REQUIRE_THAT(ball.position_x, Catch::Matchers::WithinRel(170.0, 0.01));
        REQUIRE(ball.is_active);
    }

    SECTION("Ball launches from paddle") {
        Ball ball;
        ball.is_active = true;
        ball.velocity_x = 0.0;
        ball.velocity_y = 0.0;

        // Launch ball
        ball.is_active = false;
        ball.velocity_x = 100.0;
        ball.velocity_y = -150.0;

        REQUIRE_FALSE(ball.is_active);
        REQUIRE(ball.velocity_x != 0.0);
        REQUIRE(ball.velocity_y != 0.0);
    }
}

TEST_CASE("Ball lost detection", "[ball][entity]") {
    SECTION("Ball is lost when below screen") {
        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 210.0;  // Below paddle threshold
        ball.is_lost = false;

        // Check if ball is below paddle (y > 200)
        if (ball.position_y > 200.0) {
            ball.is_lost = true;
        }

        REQUIRE(ball.is_lost);
    }

    SECTION("Ball is not lost when above threshold") {
        Ball ball;
        ball.position_x = 160.0;
        ball.position_y = 100.0;
        ball.is_lost = false;

        if (ball.position_y > 200.0) {
            ball.is_lost = true;
        }

        REQUIRE_FALSE(ball.is_lost);
    }
}

TEST_CASE("Ball speed calculation", "[ball][entity]") {
    SECTION("Calculate speed from velocity components") {
        Ball ball;
        ball.velocity_x = 60.0;
        ball.velocity_y = -80.0;

        double speed = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);

        REQUIRE_THAT(speed, Catch::Matchers::WithinRel(100.0, 0.01));
    }

    SECTION("Normalize velocity to maintain constant speed") {
        Ball ball;
        ball.velocity_x = 90.0;
        ball.velocity_y = -120.0;

        double target_speed = 150.0;
        double current_speed = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);

        // Normalize and scale to target speed
        ball.velocity_x = (ball.velocity_x / current_speed) * target_speed;
        ball.velocity_y = (ball.velocity_y / current_speed) * target_speed;

        double new_speed = std::sqrt(ball.velocity_x * ball.velocity_x + ball.velocity_y * ball.velocity_y);

        REQUIRE_THAT(new_speed, Catch::Matchers::WithinRel(target_speed, 0.01));
    }
}

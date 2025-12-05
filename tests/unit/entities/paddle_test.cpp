// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/paddle.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace wreckingball;

TEST_CASE("Paddle entity construction", "[paddle][entity]") {
    SECTION("Create paddle with default properties") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.position_y = 190.0;
        paddle.width = 16;
        paddle.height = 4;
        paddle.velocity_x = 0.0;

        REQUIRE_THAT(paddle.position_x, Catch::Matchers::WithinRel(160.0, 0.01));
        REQUIRE_THAT(paddle.position_y, Catch::Matchers::WithinRel(190.0, 0.01));
        REQUIRE(paddle.width == 16);
        REQUIRE(paddle.height == 4);
        REQUIRE_THAT(paddle.velocity_x, Catch::Matchers::WithinRel(0.0, 0.01));
    }

    SECTION("Create paddle at custom position") {
        Paddle paddle;
        paddle.position_x = 200.0;
        paddle.position_y = 185.0;
        paddle.width = 20;
        paddle.height = 4;

        REQUIRE_THAT(paddle.position_x, Catch::Matchers::WithinRel(200.0, 0.01));
        REQUIRE_THAT(paddle.position_y, Catch::Matchers::WithinRel(185.0, 0.01));
        REQUIRE(paddle.width == 20);
    }
}

TEST_CASE("Paddle movement", "[paddle][entity]") {
    SECTION("Paddle moves left") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.velocity_x = -200.0;  // Moving left

        double dt = 1.0 / 60.0;  // 60 FPS fixed timestep
        paddle.position_x += paddle.velocity_x * dt;

        REQUIRE(paddle.position_x < 160.0);
        REQUIRE_THAT(paddle.position_x, Catch::Matchers::WithinRel(160.0 - 200.0 * dt, 0.01));
    }

    SECTION("Paddle moves right") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.velocity_x = 200.0;  // Moving right

        double dt = 1.0 / 60.0;
        paddle.position_x += paddle.velocity_x * dt;

        REQUIRE(paddle.position_x > 160.0);
        REQUIRE_THAT(paddle.position_x, Catch::Matchers::WithinRel(160.0 + 200.0 * dt, 0.01));
    }

    SECTION("Paddle stops when velocity is zero") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.velocity_x = 0.0;

        double dt = 1.0 / 60.0;
        double old_x = paddle.position_x;
        paddle.position_x += paddle.velocity_x * dt;

        REQUIRE_THAT(paddle.position_x, Catch::Matchers::WithinRel(old_x, 0.01));
    }
}

TEST_CASE("Paddle boundary constraints", "[paddle][entity]") {
    SECTION("Paddle constrained to left boundary") {
        Paddle paddle;
        paddle.position_x = 0.0;
        paddle.width = 16;
        paddle.velocity_x = -200.0;  // Trying to move left

        double dt = 1.0 / 60.0;
        paddle.position_x += paddle.velocity_x * dt;

        // Clamp to minimum position (half width from left edge)
        double min_x = paddle.width / 2.0;
        if (paddle.position_x < min_x) {
            paddle.position_x = min_x;
        }

        REQUIRE(paddle.position_x >= min_x);
    }

    SECTION("Paddle constrained to right boundary") {
        Paddle paddle;
        paddle.position_x = 320.0;
        paddle.width = 16;
        paddle.velocity_x = 200.0;  // Trying to move right

        double dt = 1.0 / 60.0;
        paddle.position_x += paddle.velocity_x * dt;

        // Clamp to maximum position (half width from right edge)
        double max_x = 320.0 - paddle.width / 2.0;
        if (paddle.position_x > max_x) {
            paddle.position_x = max_x;
        }

        REQUIRE(paddle.position_x <= max_x);
    }
}

TEST_CASE("Paddle collision bounds", "[paddle][entity]") {
    SECTION("Calculate paddle left edge") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double left_edge = paddle.position_x - paddle.width / 2.0;

        REQUIRE_THAT(left_edge, Catch::Matchers::WithinRel(152.0, 0.01));
    }

    SECTION("Calculate paddle right edge") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double right_edge = paddle.position_x + paddle.width / 2.0;

        REQUIRE_THAT(right_edge, Catch::Matchers::WithinRel(168.0, 0.01));
    }

    SECTION("Calculate paddle top edge") {
        Paddle paddle;
        paddle.position_y = 190.0;
        paddle.height = 4;

        double top_edge = paddle.position_y - paddle.height / 2.0;

        REQUIRE_THAT(top_edge, Catch::Matchers::WithinRel(188.0, 0.01));
    }
}

TEST_CASE("Paddle impact position for ball bounce", "[paddle][entity]") {
    SECTION("Ball hits center of paddle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 160.0;  // Center hit
        double impact_position = (ball_x - paddle.position_x) / (paddle.width / 2.0);

        REQUIRE_THAT(impact_position, Catch::Matchers::WithinRel(0.0, 0.01));
    }

    SECTION("Ball hits left edge of paddle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 152.0;  // Left edge
        double impact_position = (ball_x - paddle.position_x) / (paddle.width / 2.0);

        REQUIRE_THAT(impact_position, Catch::Matchers::WithinRel(-1.0, 0.01));
    }

    SECTION("Ball hits right edge of paddle") {
        Paddle paddle;
        paddle.position_x = 160.0;
        paddle.width = 16;

        double ball_x = 168.0;  // Right edge
        double impact_position = (ball_x - paddle.position_x) / (paddle.width / 2.0);

        REQUIRE_THAT(impact_position, Catch::Matchers::WithinRel(1.0, 0.01));
    }
}

TEST_CASE("Paddle width variations", "[paddle][entity]") {
    SECTION("Narrow paddle") {
        Paddle paddle;
        paddle.width = 12;
        REQUIRE(paddle.width == 12);
    }

    SECTION("Standard paddle") {
        Paddle paddle;
        paddle.width = 16;
        REQUIRE(paddle.width == 16);
    }

    SECTION("Wide paddle") {
        Paddle paddle;
        paddle.width = 24;
        REQUIRE(paddle.width == 24);
    }
}

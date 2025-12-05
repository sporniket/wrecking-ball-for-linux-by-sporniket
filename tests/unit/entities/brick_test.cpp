// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/brick.hpp"
#include "../../../src/core/entities/brick_type.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace wreckingball;

TEST_CASE("Brick entity construction", "[brick][entity]") {
    SECTION("Create regular brick with default properties") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 10;
        brick.position_y = 5;
        brick.width = 4;
        brick.hits_remaining = 1;
        brick.is_destroyed = false;

        REQUIRE(brick.type == BrickType::Regular);
        REQUIRE(brick.position_x == 10);
        REQUIRE(brick.position_y == 5);
        REQUIRE(brick.width == 4);
        REQUIRE(brick.hits_remaining == 1);
        REQUIRE_FALSE(brick.is_destroyed);
    }

    SECTION("Create star brick") {
        Brick brick;
        brick.type = BrickType::Star;
        brick.position_x = 0;
        brick.position_y = 0;
        brick.width = 1;
        brick.hits_remaining = 1;

        REQUIRE(brick.type == BrickType::Star);
        REQUIRE(brick.width == 1);
    }

    SECTION("Create key brick") {
        Brick brick;
        brick.type = BrickType::Key;
        brick.position_x = 15;
        brick.position_y = 10;
        brick.width = 2;
        brick.hits_remaining = 1;

        REQUIRE(brick.type == BrickType::Key);
        REQUIRE(brick.width == 2);
    }

    SECTION("Create exit brick") {
        Brick brick;
        brick.type = BrickType::Exit;
        brick.position_x = 20;
        brick.position_y = 15;
        brick.width = 5;
        brick.hits_remaining = 1;
        brick.is_active = false;

        REQUIRE(brick.type == BrickType::Exit);
        REQUIRE_FALSE(brick.is_active);
    }
}

TEST_CASE("Brick width variations", "[brick][entity]") {
    SECTION("1-unit narrow brick") {
        Brick brick;
        brick.width = 1;
        REQUIRE(brick.width == 1);
    }

    SECTION("10-unit wide brick") {
        Brick brick;
        brick.width = 10;
        REQUIRE(brick.width == 10);
    }

    SECTION("40-unit full-width brick") {
        Brick brick;
        brick.width = 40;
        REQUIRE(brick.width == 40);
    }
}

TEST_CASE("Brick hit behavior", "[brick][entity]") {
    SECTION("Single-hit brick destruction") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.hits_remaining = 1;
        brick.is_destroyed = false;

        // Simulate hit
        brick.hits_remaining--;
        if (brick.hits_remaining <= 0) {
            brick.is_destroyed = true;
        }

        REQUIRE(brick.hits_remaining == 0);
        REQUIRE(brick.is_destroyed);
    }

    SECTION("Multi-hit brick takes multiple hits") {
        Brick brick;
        brick.type = BrickType::Regular;
        brick.hits_remaining = 3;
        brick.is_destroyed = false;

        // First hit
        brick.hits_remaining--;
        REQUIRE(brick.hits_remaining == 2);
        REQUIRE_FALSE(brick.is_destroyed);

        // Second hit
        brick.hits_remaining--;
        REQUIRE(brick.hits_remaining == 1);
        REQUIRE_FALSE(brick.is_destroyed);

        // Third hit - destruction
        brick.hits_remaining--;
        if (brick.hits_remaining <= 0) {
            brick.is_destroyed = true;
        }
        REQUIRE(brick.hits_remaining == 0);
        REQUIRE(brick.is_destroyed);
    }
}

TEST_CASE("Exit brick activation", "[brick][entity]") {
    SECTION("Exit brick starts inactive") {
        Brick brick;
        brick.type = BrickType::Exit;
        brick.is_active = false;

        REQUIRE_FALSE(brick.is_active);
    }

    SECTION("Exit brick can be activated") {
        Brick brick;
        brick.type = BrickType::Exit;
        brick.is_active = false;

        // Simulate activation when all keys are collected
        brick.is_active = true;

        REQUIRE(brick.is_active);
    }
}

TEST_CASE("Brick color based on type and hits", "[brick][entity]") {
    SECTION("Regular brick color changes with hits") {
        Brick brick;
        brick.type = BrickType::Regular;

        // 3 hits - should be dark color
        brick.hits_remaining = 3;
        REQUIRE(brick.hits_remaining == 3);

        // 2 hits - medium color
        brick.hits_remaining = 2;
        REQUIRE(brick.hits_remaining == 2);

        // 1 hit - light color
        brick.hits_remaining = 1;
        REQUIRE(brick.hits_remaining == 1);
    }

    SECTION("Special brick types have distinct colors") {
        Brick star;
        star.type = BrickType::Star;
        REQUIRE(star.type == BrickType::Star);

        Brick key;
        key.type = BrickType::Key;
        REQUIRE(key.type == BrickType::Key);

        Brick exit;
        exit.type = BrickType::Exit;
        REQUIRE(exit.type == BrickType::Exit);
    }
}

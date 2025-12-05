// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/level.hpp"
#include "../../../src/core/entities/brick.hpp"
#include "../../../src/core/entities/level_type.hpp"
#include "../../../src/core/entities/brick_type.hpp"
#include <catch2/catch_test_macros.hpp>
#include <algorithm>

using namespace wreckingball;

TEST_CASE("Level entity construction", "[level][entity]") {
    SECTION("Create empty Classical level") {
        Level level;
        level.level_id = 1;
        level.name = "Test Level";
        level.level_type = LevelType::Classical;
        level.difficulty_str = "Easy";
        level.bricks.clear();

        REQUIRE(level.level_id == 1);
        REQUIRE(level.name == "Test Level");
        REQUIRE(level.level_type == LevelType::Classical);
        REQUIRE(level.difficulty_str == "Easy");
        REQUIRE(level.bricks.empty());
    }

    SECTION("Create ReachForStars level") {
        Level level;
        level.level_id = 2;
        level.name = "Star Collector";
        level.level_type = LevelType::ReachForStars;
        level.difficulty_str = "Medium";

        REQUIRE(level.level_type == LevelType::ReachForStars);
    }

    SECTION("Create GetTheKey level") {
        Level level;
        level.level_id = 3;
        level.name = "Key Master";
        level.level_type = LevelType::GetTheKey;
        level.difficulty_str = "Hard";

        REQUIRE(level.level_type == LevelType::GetTheKey);
    }
}

TEST_CASE("Level with bricks", "[level][entity]") {
    SECTION("Classical level with regular bricks") {
        Level level;
        level.level_id = 1;
        level.level_type = LevelType::Classical;

        // Add some regular bricks
        for (int i = 0; i < 10; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.position_x = i * 4;
            brick.position_y = 5;
            brick.width = 4;
            brick.hits_remaining = 1;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        REQUIRE(level.bricks.size() == 10);
        REQUIRE(level.level_type == LevelType::Classical);
    }

    SECTION("ReachForStars level with star bricks") {
        Level level;
        level.level_id = 2;
        level.level_type = LevelType::ReachForStars;

        // Add regular bricks
        for (int i = 0; i < 8; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.position_x = i * 4;
            brick.position_y = 5;
            brick.width = 4;
            brick.hits_remaining = 1;
            level.bricks.push_back(brick);
        }

        // Add star bricks (required for ReachForStars)
        Brick star;
        star.type = BrickType::Star;
        star.position_x = 32;
        star.position_y = 10;
        star.width = 2;
        star.hits_remaining = 1;
        level.bricks.push_back(star);

        REQUIRE(level.bricks.size() == 9);

        // Count star bricks
        int star_count = std::count_if(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) { return b.type == BrickType::Star; });
        REQUIRE(star_count >= 1);
    }

    SECTION("GetTheKey level with key and exit bricks") {
        Level level;
        level.level_id = 3;
        level.level_type = LevelType::GetTheKey;

        // Add regular bricks
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.position_x = i * 4;
            brick.position_y = 5;
            brick.width = 4;
            brick.hits_remaining = 1;
            level.bricks.push_back(brick);
        }

        // Add key brick (required)
        Brick key;
        key.type = BrickType::Key;
        key.position_x = 20;
        key.position_y = 10;
        key.width = 2;
        key.hits_remaining = 1;
        level.bricks.push_back(key);

        // Add exit brick (required, starts inactive)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.position_x = 30;
        exit.position_y = 15;
        exit.width = 3;
        exit.hits_remaining = 1;
        exit.is_active = false;
        level.bricks.push_back(exit);

        REQUIRE(level.bricks.size() == 7);

        // Count key and exit bricks
        int key_count = std::count_if(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) { return b.type == BrickType::Key; });
        int exit_count = std::count_if(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) { return b.type == BrickType::Exit; });

        REQUIRE(key_count >= 1);
        REQUIRE(exit_count >= 1);
    }
}

TEST_CASE("Level brick width variations", "[level][entity]") {
    SECTION("Level with 1-unit narrow bricks") {
        Level level;
        level.level_id = 1;
        level.level_type = LevelType::Classical;

        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0;
        brick.position_y = 5;
        brick.width = 1;
        brick.hits_remaining = 1;
        level.bricks.push_back(brick);

        REQUIRE(level.bricks[0].width == 1);
    }

    SECTION("Level with 10-unit wide bricks") {
        Level level;
        level.level_id = 1;
        level.level_type = LevelType::Classical;

        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0;
        brick.position_y = 5;
        brick.width = 10;
        brick.hits_remaining = 1;
        level.bricks.push_back(brick);

        REQUIRE(level.bricks[0].width == 10);
    }

    SECTION("Level with 40-unit full-width brick") {
        Level level;
        level.level_id = 1;
        level.level_type = LevelType::Classical;

        Brick brick;
        brick.type = BrickType::Regular;
        brick.position_x = 0;
        brick.position_y = 5;
        brick.width = 40;
        brick.hits_remaining = 1;
        level.bricks.push_back(brick);

        REQUIRE(level.bricks[0].width == 40);
    }
}

TEST_CASE("Level completion criteria", "[level][entity]") {
    SECTION("Classical level - all bricks destroyed") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add 3 bricks
        for (int i = 0; i < 3; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Destroy all bricks
        for (auto& brick : level.bricks) {
            brick.is_destroyed = true;
        }

        // Check completion
        bool all_destroyed = std::all_of(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) { return b.is_destroyed; });

        REQUIRE(all_destroyed);
    }

    SECTION("ReachForStars level - only stars destroyed") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add regular bricks
        for (int i = 0; i < 3; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Add star bricks
        for (int i = 0; i < 2; ++i) {
            Brick brick;
            brick.type = BrickType::Star;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Destroy only star bricks
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Star) {
                brick.is_destroyed = true;
            }
        }

        // Check if all stars are destroyed
        bool all_stars_destroyed = std::all_of(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) {
                return b.type != BrickType::Star || b.is_destroyed;
            });

        REQUIRE(all_stars_destroyed);
    }

    SECTION("GetTheKey level - all keys and one exit destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add regular bricks
        Brick regular;
        regular.type = BrickType::Regular;
        regular.is_destroyed = false;
        level.bricks.push_back(regular);

        // Add key brick
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Add exit brick
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = false;
        exit.is_active = false;
        level.bricks.push_back(exit);

        // Destroy key first
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Key) {
                brick.is_destroyed = true;
            }
        }

        // Check if all keys destroyed
        bool all_keys_destroyed = std::all_of(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) {
                return b.type != BrickType::Key || b.is_destroyed;
            });

        REQUIRE(all_keys_destroyed);

        // Activate exit bricks
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Exit) {
                brick.is_active = true;
            }
        }

        // Destroy one exit brick
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Exit && brick.is_active) {
                brick.is_destroyed = true;
                break;  // Only need to destroy one
            }
        }

        // Check completion
        bool any_exit_destroyed = std::any_of(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) {
                return b.type == BrickType::Exit && b.is_destroyed;
            });

        REQUIRE(all_keys_destroyed);
        REQUIRE(any_exit_destroyed);
    }
}

TEST_CASE("Level metadata", "[level][entity]") {
    SECTION("Level has author and description") {
        Level level;
        level.level_id = 1;
        level.name = "Test Level";
        level.author = "David SPORN";
        level.description = "A test level for unit testing";
        level.tags = "test, tutorial";

        REQUIRE(level.author == "David SPORN");
        REQUIRE(level.description == "A test level for unit testing");
        REQUIRE(level.tags == "test, tutorial");
    }

    SECTION("Level has version") {
        Level level;
        level.level_id = 1;
        level.version = "1.0";

        REQUIRE(level.version == "1.0");
    }
}

// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/level.hpp"
#include "../../../src/core/entities/brick.hpp"
#include "../../../src/core/entities/level_type.hpp"
#include "../../../src/core/entities/brick_type.hpp"
#include <catch2/catch_test_macros.hpp>
#include <algorithm>

using namespace wreckingball;

// Helper function to check Classical level completion
static bool IsClassicalLevelComplete(const Level& level) {
    return std::all_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) { return brick.is_destroyed; });
}

TEST_CASE("Classical level completion - all bricks destroyed", "[level][completion][classical]") {
    SECTION("Empty level is complete") {
        Level level;
        level.level_type = LevelType::Classical;
        level.bricks.clear();

        bool complete = IsClassicalLevelComplete(level);
        REQUIRE(complete);
    }

    SECTION("Level with all bricks destroyed is complete") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add 5 bricks, all destroyed
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = true;
            level.bricks.push_back(brick);
        }

        bool complete = IsClassicalLevelComplete(level);
        REQUIRE(complete);
    }

    SECTION("Level with one undestroyed brick is not complete") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add 5 bricks, all but one destroyed
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = (i < 4);  // Last one not destroyed
            level.bricks.push_back(brick);
        }

        bool complete = IsClassicalLevelComplete(level);
        REQUIRE_FALSE(complete);
    }

    SECTION("Level with no destroyed bricks is not complete") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add 10 bricks, none destroyed
        for (int i = 0; i < 10; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        bool complete = IsClassicalLevelComplete(level);
        REQUIRE_FALSE(complete);
    }
}

TEST_CASE("Classical level progression", "[level][completion][classical]") {
    SECTION("Level completes progressively as bricks are destroyed") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add 5 bricks
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Initially not complete
        REQUIRE_FALSE(IsClassicalLevelComplete(level));

        // Destroy bricks one by one
        for (size_t i = 0; i < level.bricks.size(); ++i) {
            level.bricks[i].is_destroyed = true;

            if (i < level.bricks.size() - 1) {
                // Still bricks remaining
                REQUIRE_FALSE(IsClassicalLevelComplete(level));
            } else {
                // All bricks destroyed
                REQUIRE(IsClassicalLevelComplete(level));
            }
        }
    }
}

TEST_CASE("Classical level with variable-width bricks", "[level][completion][classical]") {
    SECTION("Level with mixed width bricks") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add bricks of different widths
        Brick brick1;
        brick1.width = 1;
        brick1.is_destroyed = false;
        level.bricks.push_back(brick1);

        Brick brick2;
        brick2.width = 10;
        brick2.is_destroyed = false;
        level.bricks.push_back(brick2);

        Brick brick3;
        brick3.width = 40;
        brick3.is_destroyed = false;
        level.bricks.push_back(brick3);

        REQUIRE_FALSE(IsClassicalLevelComplete(level));

        // Destroy all bricks
        for (auto& brick : level.bricks) {
            brick.is_destroyed = true;
        }

        REQUIRE(IsClassicalLevelComplete(level));
    }
}

TEST_CASE("Classical level with multi-hit bricks", "[level][completion][classical]") {
    SECTION("Level not complete until multi-hit bricks fully destroyed") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add brick with 3 hits required
        Brick brick;
        brick.type = BrickType::Regular;
        brick.hits_remaining = 3;
        brick.is_destroyed = false;
        level.bricks.push_back(brick);

        REQUIRE_FALSE(IsClassicalLevelComplete(level));

        // Hit once
        level.bricks[0].hits_remaining = 2;
        REQUIRE_FALSE(IsClassicalLevelComplete(level));

        // Hit twice
        level.bricks[0].hits_remaining = 1;
        REQUIRE_FALSE(IsClassicalLevelComplete(level));

        // Final hit - destroy
        level.bricks[0].hits_remaining = 0;
        level.bricks[0].is_destroyed = true;
        REQUIRE(IsClassicalLevelComplete(level));
    }
}

TEST_CASE("Classical level completion count", "[level][completion][classical]") {
    SECTION("Calculate destruction progress") {
        Level level;
        level.level_type = LevelType::Classical;

        // Add 10 bricks
        for (int i = 0; i < 10; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Count destroyed bricks
        auto count_destroyed = [](const Level& lvl) {
            return std::count_if(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) { return b.is_destroyed; });
        };

        REQUIRE(count_destroyed(level) == 0);

        // Destroy 5 bricks
        for (int i = 0; i < 5; ++i) {
            level.bricks[i].is_destroyed = true;
        }

        REQUIRE(count_destroyed(level) == 5);
        REQUIRE_FALSE(IsClassicalLevelComplete(level));

        // Destroy remaining 5 bricks
        for (int i = 5; i < 10; ++i) {
            level.bricks[i].is_destroyed = true;
        }

        REQUIRE(count_destroyed(level) == 10);
        REQUIRE(IsClassicalLevelComplete(level));
    }
}

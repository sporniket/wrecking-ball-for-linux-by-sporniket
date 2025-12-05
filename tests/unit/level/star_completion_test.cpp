// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/level.hpp"
#include "../../../src/core/entities/brick.hpp"
#include "../../../src/core/entities/level_type.hpp"
#include "../../../src/core/entities/brick_type.hpp"
#include <catch2/catch_test_macros.hpp>
#include <algorithm>

using namespace wreckingball;

// Helper function to check ReachForStars level completion
// Level is complete when ALL star bricks are destroyed
static bool IsStarLevelComplete(const Level& level) {
    return std::all_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type != BrickType::Star || brick.is_destroyed;
        });
}

TEST_CASE("ReachForStars level completion - only stars matter", "[level][completion][star]") {
    SECTION("Level with all stars destroyed is complete") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add regular bricks (not destroyed)
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Add star bricks (destroyed)
        for (int i = 0; i < 3; ++i) {
            Brick brick;
            brick.type = BrickType::Star;
            brick.is_destroyed = true;
            level.bricks.push_back(brick);
        }

        bool complete = IsStarLevelComplete(level);
        REQUIRE(complete);  // Complete even though regular bricks remain
    }

    SECTION("Level with one undestroyed star is not complete") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add regular bricks (all destroyed - doesn't matter)
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = true;
            level.bricks.push_back(brick);
        }

        // Add star bricks (one not destroyed)
        Brick star1;
        star1.type = BrickType::Star;
        star1.is_destroyed = true;
        level.bricks.push_back(star1);

        Brick star2;
        star2.type = BrickType::Star;
        star2.is_destroyed = false;  // Still active
        level.bricks.push_back(star2);

        bool complete = IsStarLevelComplete(level);
        REQUIRE_FALSE(complete);
    }

    SECTION("Level with no stars is not valid but technically complete") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add only regular bricks (no stars - invalid level design)
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // No stars means completion check passes (vacuous truth)
        bool complete = IsStarLevelComplete(level);
        REQUIRE(complete);
    }
}

TEST_CASE("ReachForStars level progression", "[level][completion][star]") {
    SECTION("Level completes progressively as stars are destroyed") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add regular bricks
        for (int i = 0; i < 10; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Add 3 star bricks
        for (int i = 0; i < 3; ++i) {
            Brick brick;
            brick.type = BrickType::Star;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Initially not complete
        REQUIRE_FALSE(IsStarLevelComplete(level));

        // Destroy regular bricks - should not affect completion
        for (size_t i = 0; i < 10; ++i) {
            level.bricks[i].is_destroyed = true;
        }
        REQUIRE_FALSE(IsStarLevelComplete(level));

        // Destroy stars one by one
        for (size_t i = 10; i < 13; ++i) {
            level.bricks[i].is_destroyed = true;

            if (i < 12) {
                // Still stars remaining
                REQUIRE_FALSE(IsStarLevelComplete(level));
            } else {
                // All stars destroyed
                REQUIRE(IsStarLevelComplete(level));
            }
        }
    }
}

TEST_CASE("ReachForStars level with single star", "[level][completion][star]") {
    SECTION("Level with one star brick") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add many regular bricks
        for (int i = 0; i < 20; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Add single star brick
        Brick star;
        star.type = BrickType::Star;
        star.is_destroyed = false;
        level.bricks.push_back(star);

        REQUIRE_FALSE(IsStarLevelComplete(level));

        // Destroy only the star
        level.bricks.back().is_destroyed = true;

        REQUIRE(IsStarLevelComplete(level));
    }
}

TEST_CASE("ReachForStars level with multiple stars", "[level][completion][star]") {
    SECTION("Level with 5 stars scattered among regular bricks") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add bricks in mixed pattern
        for (int i = 0; i < 20; ++i) {
            Brick brick;
            if (i % 4 == 0) {
                brick.type = BrickType::Star;  // Every 4th brick is a star
            } else {
                brick.type = BrickType::Regular;
            }
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        REQUIRE_FALSE(IsStarLevelComplete(level));

        // Count stars
        int star_count = std::count_if(level.bricks.begin(), level.bricks.end(),
            [](const Brick& b) { return b.type == BrickType::Star; });
        REQUIRE(star_count == 5);

        // Destroy all stars
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Star) {
                brick.is_destroyed = true;
            }
        }

        REQUIRE(IsStarLevelComplete(level));
    }
}

TEST_CASE("ReachForStars level completion tracking", "[level][completion][star]") {
    SECTION("Calculate star destruction progress") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add 10 regular bricks and 5 star bricks
        for (int i = 0; i < 10; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Star;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Helper to count destroyed stars
        auto count_destroyed_stars = [](const Level& lvl) {
            return std::count_if(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) {
                    return b.type == BrickType::Star && b.is_destroyed;
                });
        };

        auto count_total_stars = [](const Level& lvl) {
            return std::count_if(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) { return b.type == BrickType::Star; });
        };

        REQUIRE(count_total_stars(level) == 5);
        REQUIRE(count_destroyed_stars(level) == 0);

        // Destroy 3 stars
        int destroyed = 0;
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Star && destroyed < 3) {
                brick.is_destroyed = true;
                destroyed++;
            }
        }

        REQUIRE(count_destroyed_stars(level) == 3);
        REQUIRE_FALSE(IsStarLevelComplete(level));  // 2 stars remain

        // Destroy remaining stars
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Star) {
                brick.is_destroyed = true;
            }
        }

        REQUIRE(count_destroyed_stars(level) == 5);
        REQUIRE(IsStarLevelComplete(level));
    }
}

TEST_CASE("ReachForStars level with variable-width star bricks", "[level][completion][star]") {
    SECTION("Wide star bricks behave same as narrow ones") {
        Level level;
        level.level_type = LevelType::ReachForStars;

        // Add star bricks of different widths
        Brick star1;
        star1.type = BrickType::Star;
        star1.width = 1;
        star1.is_destroyed = false;
        level.bricks.push_back(star1);

        Brick star2;
        star2.type = BrickType::Star;
        star2.width = 10;
        star2.is_destroyed = false;
        level.bricks.push_back(star2);

        Brick star3;
        star3.type = BrickType::Star;
        star3.width = 40;
        star3.is_destroyed = false;
        level.bricks.push_back(star3);

        REQUIRE_FALSE(IsStarLevelComplete(level));

        // Destroy all star bricks
        for (auto& brick : level.bricks) {
            brick.is_destroyed = true;
        }

        REQUIRE(IsStarLevelComplete(level));
    }
}

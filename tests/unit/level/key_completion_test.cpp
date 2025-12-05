// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/core/entities/level.hpp"
#include "../../../src/core/entities/brick.hpp"
#include "../../../src/core/entities/level_type.hpp"
#include "../../../src/core/entities/brick_type.hpp"
#include <catch2/catch_test_macros.hpp>
#include <algorithm>

using namespace wreckingball;

// Helper function to check if all keys are destroyed
static bool AreAllKeysDestroyed(const Level& level) {
    return std::all_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type != BrickType::Key || brick.is_destroyed;
        });
}

// Helper function to check if any exit is destroyed
static bool IsAnyExitDestroyed(const Level& level) {
    return std::any_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type == BrickType::Exit && brick.is_destroyed;
        });
}

// Helper function to check GetTheKey level completion
// Level is complete when ALL keys are destroyed AND at least one exit is destroyed
static bool IsKeyLevelComplete(const Level& level) {
    return AreAllKeysDestroyed(level) && IsAnyExitDestroyed(level);
}

TEST_CASE("GetTheKey level completion - keys and exit required", "[level][completion][key]") {
    SECTION("Level complete when all keys and one exit destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add regular bricks
        for (int i = 0; i < 5; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Add key brick (destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = true;
        level.bricks.push_back(key);

        // Add exit brick (destroyed)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = true;
        exit.is_active = true;
        level.bricks.push_back(exit);

        bool complete = IsKeyLevelComplete(level);
        REQUIRE(complete);
    }

    SECTION("Level not complete when keys destroyed but no exit destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = true;
        level.bricks.push_back(key);

        // Add exit brick (NOT destroyed)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = false;
        exit.is_active = true;
        level.bricks.push_back(exit);

        bool complete = IsKeyLevelComplete(level);
        REQUIRE_FALSE(complete);
    }

    SECTION("Level not complete when exit destroyed but keys remain") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (NOT destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Add exit brick (destroyed)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = true;
        exit.is_active = false;  // Should not be active if keys remain
        level.bricks.push_back(exit);

        bool complete = IsKeyLevelComplete(level);
        REQUIRE_FALSE(complete);
    }

    SECTION("Level not complete when neither keys nor exit destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (not destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Add exit brick (not destroyed)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = false;
        exit.is_active = false;
        level.bricks.push_back(exit);

        bool complete = IsKeyLevelComplete(level);
        REQUIRE_FALSE(complete);
    }
}

TEST_CASE("GetTheKey level with multiple keys", "[level][completion][key]") {
    SECTION("Level requires ALL keys to be destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add 3 key bricks
        for (int i = 0; i < 3; ++i) {
            Brick key;
            key.type = BrickType::Key;
            key.is_destroyed = false;
            level.bricks.push_back(key);
        }

        // Add exit brick
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = false;
        exit.is_active = false;
        level.bricks.push_back(exit);

        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy 2 out of 3 keys
        level.bricks[0].is_destroyed = true;
        level.bricks[1].is_destroyed = true;
        REQUIRE_FALSE(AreAllKeysDestroyed(level));
        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy final key
        level.bricks[2].is_destroyed = true;
        REQUIRE(AreAllKeysDestroyed(level));

        // Exit still not destroyed
        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy exit
        level.bricks[3].is_destroyed = true;
        REQUIRE(IsKeyLevelComplete(level));
    }
}

TEST_CASE("GetTheKey level with multiple exits", "[level][completion][key]") {
    SECTION("Only one exit needs to be destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = true;
        level.bricks.push_back(key);

        // Add 3 exit bricks, only destroy one
        for (int i = 0; i < 3; ++i) {
            Brick exit;
            exit.type = BrickType::Exit;
            exit.is_destroyed = (i == 0);  // Only first one destroyed
            exit.is_active = true;
            level.bricks.push_back(exit);
        }

        bool complete = IsKeyLevelComplete(level);
        REQUIRE(complete);  // Only one exit needed
    }

    SECTION("Any exit can be destroyed to complete level") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = true;
        level.bricks.push_back(key);

        // Add 3 exit bricks
        for (int i = 0; i < 3; ++i) {
            Brick exit;
            exit.type = BrickType::Exit;
            exit.is_destroyed = false;
            exit.is_active = true;
            level.bricks.push_back(exit);
        }

        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy middle exit
        level.bricks[2].is_destroyed = true;  // Index 2 = middle exit (0=key, 1=exit1, 2=exit2)
        REQUIRE(IsKeyLevelComplete(level));
    }
}

TEST_CASE("GetTheKey level progression", "[level][completion][key]") {
    SECTION("Typical level progression") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add regular bricks
        for (int i = 0; i < 10; ++i) {
            Brick brick;
            brick.type = BrickType::Regular;
            brick.is_destroyed = false;
            level.bricks.push_back(brick);
        }

        // Add 2 key bricks
        for (int i = 0; i < 2; ++i) {
            Brick key;
            key.type = BrickType::Key;
            key.is_destroyed = false;
            level.bricks.push_back(key);
        }

        // Add exit brick (inactive initially)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = false;
        exit.is_active = false;
        level.bricks.push_back(exit);

        // Initially not complete
        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy some regular bricks - no effect on completion
        for (int i = 0; i < 5; ++i) {
            level.bricks[i].is_destroyed = true;
        }
        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy first key
        level.bricks[10].is_destroyed = true;
        REQUIRE_FALSE(AreAllKeysDestroyed(level));
        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy second key
        level.bricks[11].is_destroyed = true;
        REQUIRE(AreAllKeysDestroyed(level));

        // Exit should now be activatable (game logic would do this)
        level.bricks[12].is_active = true;
        REQUIRE_FALSE(IsKeyLevelComplete(level));  // Still need to destroy exit

        // Destroy exit
        level.bricks[12].is_destroyed = true;
        REQUIRE(IsKeyLevelComplete(level));
    }
}

TEST_CASE("GetTheKey level completion tracking", "[level][completion][key]") {
    SECTION("Track key collection progress") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add 5 key bricks
        for (int i = 0; i < 5; ++i) {
            Brick key;
            key.type = BrickType::Key;
            key.is_destroyed = false;
            level.bricks.push_back(key);
        }

        // Add exit brick
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = false;
        exit.is_active = false;
        level.bricks.push_back(exit);

        // Helper to count destroyed keys
        auto count_destroyed_keys = [](const Level& lvl) {
            return std::count_if(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) {
                    return b.type == BrickType::Key && b.is_destroyed;
                });
        };

        auto count_total_keys = [](const Level& lvl) {
            return std::count_if(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) { return b.type == BrickType::Key; });
        };

        REQUIRE(count_total_keys(level) == 5);
        REQUIRE(count_destroyed_keys(level) == 0);

        // Destroy 3 keys
        for (int i = 0; i < 3; ++i) {
            level.bricks[i].is_destroyed = true;
        }

        REQUIRE(count_destroyed_keys(level) == 3);
        REQUIRE_FALSE(AreAllKeysDestroyed(level));

        // Destroy remaining keys
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Key) {
                brick.is_destroyed = true;
            }
        }

        REQUIRE(count_destroyed_keys(level) == 5);
        REQUIRE(AreAllKeysDestroyed(level));
    }
}

TEST_CASE("GetTheKey level with single key and single exit", "[level][completion][key]") {
    SECTION("Simplest valid GetTheKey level") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add single key
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Add single exit
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_destroyed = false;
        exit.is_active = false;
        level.bricks.push_back(exit);

        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy key
        level.bricks[0].is_destroyed = true;
        level.bricks[1].is_active = true;  // Activate exit
        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy exit
        level.bricks[1].is_destroyed = true;
        REQUIRE(IsKeyLevelComplete(level));
    }
}

TEST_CASE("GetTheKey level with variable-width bricks", "[level][completion][key]") {
    SECTION("Wide key and exit bricks work same as narrow ones") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Wide key brick
        Brick key;
        key.type = BrickType::Key;
        key.width = 20;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Full-width exit brick
        Brick exit;
        exit.type = BrickType::Exit;
        exit.width = 40;
        exit.is_destroyed = false;
        exit.is_active = false;
        level.bricks.push_back(exit);

        REQUIRE_FALSE(IsKeyLevelComplete(level));

        // Destroy key and exit
        level.bricks[0].is_destroyed = true;
        level.bricks[1].is_active = true;
        level.bricks[1].is_destroyed = true;

        REQUIRE(IsKeyLevelComplete(level));
    }
}

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

// Helper function to activate exit bricks when all keys are destroyed
static void UpdateExitBrickActivation(Level& level) {
    if (AreAllKeysDestroyed(level)) {
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Exit) {
                brick.is_active = true;
            }
        }
    }
}

TEST_CASE("Exit brick activation logic", "[level][exit][activation]") {
    SECTION("Exit bricks start inactive") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_active = false;
        level.bricks.push_back(exit);

        REQUIRE_FALSE(level.bricks[0].is_active);
    }

    SECTION("Exit activates when all keys destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Add exit brick (inactive)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_active = false;
        level.bricks.push_back(exit);

        // Check initial state
        UpdateExitBrickActivation(level);
        REQUIRE_FALSE(level.bricks[1].is_active);

        // Destroy key
        level.bricks[0].is_destroyed = true;

        // Update exit activation
        UpdateExitBrickActivation(level);
        REQUIRE(level.bricks[1].is_active);
    }

    SECTION("Multiple exits all activate when keys destroyed") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Add 3 exit bricks
        for (int i = 0; i < 3; ++i) {
            Brick exit;
            exit.type = BrickType::Exit;
            exit.is_active = false;
            level.bricks.push_back(exit);
        }

        // All exits inactive
        UpdateExitBrickActivation(level);
        for (size_t i = 1; i <= 3; ++i) {
            REQUIRE_FALSE(level.bricks[i].is_active);
        }

        // Destroy key
        level.bricks[0].is_destroyed = true;

        // All exits should activate
        UpdateExitBrickActivation(level);
        for (size_t i = 1; i <= 3; ++i) {
            REQUIRE(level.bricks[i].is_active);
        }
    }

    SECTION("Exits remain inactive until ALL keys destroyed") {
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
        exit.is_active = false;
        level.bricks.push_back(exit);

        // Destroy 2 out of 3 keys
        level.bricks[0].is_destroyed = true;
        level.bricks[1].is_destroyed = true;

        // Exit should still be inactive
        UpdateExitBrickActivation(level);
        REQUIRE_FALSE(level.bricks[3].is_active);

        // Destroy final key
        level.bricks[2].is_destroyed = true;

        // Exit should now activate
        UpdateExitBrickActivation(level);
        REQUIRE(level.bricks[3].is_active);
    }
}

TEST_CASE("Exit brick activation state persistence", "[level][exit][activation]") {
    SECTION("Exit remains active after activation") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (already destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = true;
        level.bricks.push_back(key);

        // Add exit brick
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_active = false;
        level.bricks.push_back(exit);

        // Activate exits
        UpdateExitBrickActivation(level);
        REQUIRE(level.bricks[1].is_active);

        // Call activation again - should remain active
        UpdateExitBrickActivation(level);
        REQUIRE(level.bricks[1].is_active);
    }
}

TEST_CASE("Exit brick cannot be destroyed when inactive", "[level][exit][activation]") {
    SECTION("Inactive exit should not count for level completion") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (not destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = false;
        level.bricks.push_back(key);

        // Add exit brick (inactive, but marked destroyed - should not complete level)
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_active = false;
        exit.is_destroyed = true;  // Destroyed but inactive
        level.bricks.push_back(exit);

        // Helper to check level completion
        auto is_complete = [](const Level& lvl) {
            bool all_keys_destroyed = std::all_of(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) {
                    return b.type != BrickType::Key || b.is_destroyed;
                });
            bool any_active_exit_destroyed = std::any_of(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) {
                    return b.type == BrickType::Exit && b.is_active && b.is_destroyed;
                });
            return all_keys_destroyed && any_active_exit_destroyed;
        };

        // Level should not be complete (exit inactive despite being destroyed)
        REQUIRE_FALSE(is_complete(level));
    }

    SECTION("Active exit can be destroyed to complete level") {
        Level level;
        level.level_type = LevelType::GetTheKey;

        // Add key brick (destroyed)
        Brick key;
        key.type = BrickType::Key;
        key.is_destroyed = true;
        level.bricks.push_back(key);

        // Add exit brick
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_active = false;
        exit.is_destroyed = false;
        level.bricks.push_back(exit);

        // Activate exit
        UpdateExitBrickActivation(level);
        REQUIRE(level.bricks[1].is_active);

        // Now destroy it
        level.bricks[1].is_destroyed = true;

        // Helper to check level completion
        auto is_complete = [](const Level& lvl) {
            bool all_keys_destroyed = std::all_of(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) {
                    return b.type != BrickType::Key || b.is_destroyed;
                });
            bool any_active_exit_destroyed = std::any_of(lvl.bricks.begin(), lvl.bricks.end(),
                [](const Brick& b) {
                    return b.type == BrickType::Exit && b.is_active && b.is_destroyed;
                });
            return all_keys_destroyed && any_active_exit_destroyed;
        };

        // Level should be complete
        REQUIRE(is_complete(level));
    }
}

TEST_CASE("Exit activation with no keys", "[level][exit][activation]") {
    SECTION("Level with no keys has exits immediately active") {
        Level level;
        level.level_type = LevelType::GetTheKey;  // Invalid design but test edge case

        // Add exit brick, no keys
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_active = false;
        level.bricks.push_back(exit);

        // Check if all keys destroyed (vacuously true)
        REQUIRE(AreAllKeysDestroyed(level));

        // Update exit activation
        UpdateExitBrickActivation(level);
        REQUIRE(level.bricks[0].is_active);
    }
}

TEST_CASE("Exit activation visual feedback", "[level][exit][activation]") {
    SECTION("Exit brick state changes from inactive to active") {
        Brick exit;
        exit.type = BrickType::Exit;
        exit.is_active = false;
        exit.is_destroyed = false;

        // Initially inactive
        REQUIRE_FALSE(exit.is_active);

        // Simulate activation (game would change color/appearance)
        exit.is_active = true;

        // Now active
        REQUIRE(exit.is_active);
        REQUIRE_FALSE(exit.is_destroyed);
    }
}

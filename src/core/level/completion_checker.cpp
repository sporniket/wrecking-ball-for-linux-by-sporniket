// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "completion_checker.hpp"
#include "../entities/level_type.hpp"
#include "../entities/brick_type.hpp"
#include "../logging/error_logger.hpp"
#include <algorithm>

namespace wreckingball {

LevelCompletionChecker::LevelCompletionChecker() {
    ErrorLogger::GetInstance().LogInfo("LevelCompletionChecker initialized");
}

LevelCompletionChecker::~LevelCompletionChecker() {}

bool LevelCompletionChecker::IsLevelComplete(const Level& level) {
    switch (level.level_type) {
        case LevelType::Classical:
            return IsClassicalLevelComplete(level);

        case LevelType::ReachForStars:
            return IsStarLevelComplete(level);

        case LevelType::GetTheKey:
            return IsKeyLevelComplete(level);

        default:
            ErrorLogger::GetInstance().LogError("Unknown level type in completion check");
            return false;
    }
}

void LevelCompletionChecker::UpdateExitBrickActivation(Level& level) {
    if (level.level_type != LevelType::GetTheKey) {
        return;  // Only applies to GetTheKey levels
    }

    // Check if all keys are destroyed
    if (AreAllKeysDestroyed(level)) {
        // Activate all exit bricks
        for (auto& brick : level.bricks) {
            if (brick.type == BrickType::Exit) {
                brick.is_active = true;
            }
        }
    }
}

std::string LevelCompletionChecker::GetCompletionProgress(const Level& level) {
    switch (level.level_type) {
        case LevelType::Classical: {
            int total = CountTotalBricks(level);
            int destroyed = CountDestroyedBricks(level);
            return std::to_string(destroyed) + "/" + std::to_string(total) + " bricks";
        }

        case LevelType::ReachForStars: {
            int total_stars = CountStarBricks(level);
            int destroyed_stars = CountDestroyedStars(level);
            return std::to_string(destroyed_stars) + "/" + std::to_string(total_stars) + " stars";
        }

        case LevelType::GetTheKey: {
            int total_keys = CountKeyBricks(level);
            int destroyed_keys = CountDestroyedKeys(level);

            if (destroyed_keys < total_keys) {
                return std::to_string(destroyed_keys) + "/" + std::to_string(total_keys) + " keys";
            } else {
                return "Find the exit!";
            }
        }

        default:
            return "Unknown";
    }
}

bool LevelCompletionChecker::IsClassicalLevelComplete(const Level& level) {
    // All bricks must be destroyed
    return std::all_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type == BrickType::Empty || brick.is_destroyed;
        });
}

bool LevelCompletionChecker::IsStarLevelComplete(const Level& level) {
    // All star bricks must be destroyed
    return std::all_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type != BrickType::Star || brick.is_destroyed;
        });
}

bool LevelCompletionChecker::IsKeyLevelComplete(const Level& level) {
    // All keys destroyed AND at least one active exit destroyed
    return AreAllKeysDestroyed(level) && IsAnyActiveExitDestroyed(level);
}

bool LevelCompletionChecker::AreAllKeysDestroyed(const Level& level) {
    return std::all_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type != BrickType::Key || brick.is_destroyed;
        });
}

bool LevelCompletionChecker::IsAnyActiveExitDestroyed(const Level& level) {
    return std::any_of(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type == BrickType::Exit &&
                   brick.is_active &&
                   brick.is_destroyed;
        });
}

int LevelCompletionChecker::CountTotalBricks(const Level& level) {
    return std::count_if(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type != BrickType::Empty;
        });
}

int LevelCompletionChecker::CountDestroyedBricks(const Level& level) {
    return std::count_if(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type != BrickType::Empty && brick.is_destroyed;
        });
}

int LevelCompletionChecker::CountStarBricks(const Level& level) {
    return std::count_if(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type == BrickType::Star;
        });
}

int LevelCompletionChecker::CountDestroyedStars(const Level& level) {
    return std::count_if(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type == BrickType::Star && brick.is_destroyed;
        });
}

int LevelCompletionChecker::CountKeyBricks(const Level& level) {
    return std::count_if(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type == BrickType::Key;
        });
}

int LevelCompletionChecker::CountDestroyedKeys(const Level& level) {
    return std::count_if(level.bricks.begin(), level.bricks.end(),
        [](const Brick& brick) {
            return brick.type == BrickType::Key && brick.is_destroyed;
        });
}

} // namespace wreckingball

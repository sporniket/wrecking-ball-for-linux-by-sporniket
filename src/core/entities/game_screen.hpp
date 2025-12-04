// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace wreckingball {

/**
 * @brief Game screen enumeration
 *
 * Represents different screens/states in the game
 */
enum class GameScreen : uint8_t {
    MainMenu,
    LevelSelect,
    GameplayCasual,
    GameplayArcade,
    Trophies,
    Settings,
    LevelEditor,
    OtherGames,
    Pause,
    LevelComplete,
    GameOver
};

} // namespace wreckingball

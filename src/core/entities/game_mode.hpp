// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace wreckingball {

/**
 * @brief Game mode enumeration
 */
enum class GameMode : uint8_t {
    Casual = 0,     // Unlimited balls, level selection
    Arcade = 1      // Limited lives (3/5/7), sequential levels from 1
};

} // namespace wreckingball

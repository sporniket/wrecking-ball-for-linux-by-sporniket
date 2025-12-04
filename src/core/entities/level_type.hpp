// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace wreckingball {

/**
 * @brief Level type enumeration
 *
 * Determines win condition for the level:
 * - Classical: Break all breakable bricks
 * - ReachForStars: Break all star-shaped bricks only
 * - GetTheKey: Break all key bricks, then break an exit brick
 */
enum class LevelType : uint8_t {
    Classical = 0,      // Break all breakable bricks
    ReachForStars = 1,  // Break all star bricks only (other bricks don't count)
    GetTheKey = 2       // Break all key bricks first, then break exit brick
};

} // namespace wreckingball

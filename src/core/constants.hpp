// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <cstdint>

namespace wreckingball {

// =============================================================================
// Screen and Grid Constants
// =============================================================================

/**
 * @brief Apparent screen resolution (retro aesthetic)
 * Physical window will be scaled up with nearest-neighbor filtering
 */
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 200;

/**
 * @brief Grid dimensions in units
 * The play field is divided into a grid of units for brick placement
 */
constexpr int GRID_WIDTH_UNITS = 40;
constexpr int GRID_HEIGHT_UNITS = 25;

/**
 * @brief Apparent pixels per grid unit
 * Each unit is 8 apparent pixels square (320/40 = 8, 200/25 = 8)
 */
constexpr int PIXELS_PER_UNIT = 8;

/**
 * @brief Play field dimensions (2-unit border around edge)
 * Left/right borders: 1 unit each
 * Top border: 1 unit
 * Bottom: 1 unit (ball death zone)
 */
constexpr int PLAYFIELD_WIDTH_UNITS = 38;
constexpr int PLAYFIELD_HEIGHT_UNITS = 23;
constexpr int PLAYFIELD_OFFSET_X_UNITS = 1;
constexpr int PLAYFIELD_OFFSET_Y_UNITS = 1;

/**
 * @brief Brick area (top portion of play field)
 * Typical brick area: rows 0-14 (15 rows)
 */
constexpr int BRICK_AREA_HEIGHT_UNITS = 15;

// =============================================================================
// Retro 16-Color Palette
// =============================================================================

/**
 * @brief 16-color palette for retro aesthetic
 * Colors specified as RGB hex values
 */
struct Color {
    uint8_t r, g, b, a;

    constexpr Color(uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ = 255)
        : r(r_), g(g_), b(b_), a(a_) {}
};

// Palette colors (from specification)
constexpr Color COLOR_0  = Color(0x11, 0x11, 0x22);  // #112 - Dark blue-gray
constexpr Color COLOR_1  = Color(0x44, 0x22, 0x33);  // #423 - Dark purple
constexpr Color COLOR_2  = Color(0x33, 0x33, 0x66);  // #336 - Dark blue
constexpr Color COLOR_3  = Color(0x55, 0x55, 0x55);  // #555 - Medium gray
constexpr Color COLOR_4  = Color(0x88, 0x44, 0x33);  // #843 - Brown
constexpr Color COLOR_5  = Color(0x33, 0x66, 0x22);  // #362 - Dark green
constexpr Color COLOR_6  = Color(0xcc, 0x44, 0x44);  // #c44 - Red
constexpr Color COLOR_7  = Color(0x77, 0x77, 0x66);  // #776 - Gray-green
constexpr Color COLOR_8  = Color(0x55, 0x77, 0xcc);  // #57c - Blue
constexpr Color COLOR_9  = Color(0xcc, 0x77, 0x33);  // #c73 - Orange
constexpr Color COLOR_10 = Color(0x88, 0x99, 0x99);  // #899 - Light gray
constexpr Color COLOR_11 = Color(0x66, 0xaa, 0x33);  // #6a3 - Green
constexpr Color COLOR_12 = Color(0xcc, 0xaa, 0x99);  // #ca9 - Beige
constexpr Color COLOR_13 = Color(0x66, 0xbb, 0xcc);  // #6bc - Cyan
constexpr Color COLOR_14 = Color(0xdd, 0xcc, 0x66);  // #dc6 - Yellow
constexpr Color COLOR_15 = Color(0xdd, 0xee, 0xdd);  // #ded - White

// Semantic color assignments
constexpr Color COLOR_BACKGROUND = COLOR_0;
constexpr Color COLOR_BORDER = COLOR_3;
constexpr Color COLOR_TEXT_PRIMARY = COLOR_15;
constexpr Color COLOR_TEXT_SECONDARY = COLOR_10;

// =============================================================================
// Physics Constants
// =============================================================================

/**
 * @brief Game runs at fixed 60 FPS
 */
constexpr int TARGET_FPS = 60;
constexpr double FIXED_TIMESTEP = 1.0 / TARGET_FPS;  // ~16.67ms per frame

/**
 * @brief Ball physics constants (in units per second)
 */
constexpr float BALL_INITIAL_SPEED = 12.0f;  // units/second
constexpr float BALL_MAX_SPEED = 20.0f;      // units/second
constexpr float BALL_RADIUS = 0.25f;         // Ball is 0.5 units diameter (4 pixels)
constexpr float BALL_SPEED_INCREMENT = 0.5f; // Speed increase per brick hit

/**
 * @brief Paddle physics constants
 */
constexpr float PADDLE_INITIAL_WIDTH = 4.0f;      // units (32 pixels)
constexpr float PADDLE_MIN_WIDTH = 2.0f;          // units (16 pixels)
constexpr float PADDLE_MAX_WIDTH = 6.0f;          // units (48 pixels)
constexpr float PADDLE_HEIGHT = 0.5f;             // units (4 pixels)
constexpr float PADDLE_SPEED = 16.0f;             // units/second
constexpr float PADDLE_Y_POSITION = 21.0f;        // units from top (near bottom)

/**
 * @brief Brick constants
 */
constexpr float BRICK_HEIGHT = 1.0f;              // All bricks are 1 unit tall
constexpr float BRICK_MIN_WIDTH = 1.0f;           // Minimum brick width
constexpr float BRICK_MAX_WIDTH = 40.0f;          // Maximum brick width (full screen)
constexpr int BRICK_HIT_SCORE = 10;               // Points per brick hit
constexpr int BRICK_DESTROY_SCORE = 50;           // Points for destroying brick

/**
 * @brief Ball collision response
 * Controls how paddle position affects ball bounce angle
 */
constexpr float PADDLE_BOUNCE_ANGLE_MAX = 60.0f;  // Max bounce angle in degrees
constexpr float WALL_BOUNCE_DAMPING = 0.98f;      // Slight speed loss on wall bounce

/**
 * @brief Lives and scoring
 */
constexpr int INITIAL_LIVES = 3;
constexpr int MAX_LIVES = 9;
constexpr int LIFE_LOST_PENALTY = 0;              // No score penalty for losing life
constexpr int LEVEL_COMPLETE_BONUS = 1000;

// =============================================================================
// Gameplay Constants
// =============================================================================

/**
 * @brief Level completion requirements
 */
constexpr int STARS_REQUIRED_PERCENTAGE = 80;     // Need 80% of stars for ReachForStars

/**
 * @brief Power-up durations (in seconds)
 */
constexpr float POWERUP_DURATION = 10.0f;

/**
 * @brief Menu and UI constants
 */
constexpr int MENU_ITEM_HEIGHT_UNITS = 2;         // Each menu item is 2 units tall
constexpr float MENU_FADE_DURATION = 0.3f;        // Screen transition duration

} // namespace wreckingball

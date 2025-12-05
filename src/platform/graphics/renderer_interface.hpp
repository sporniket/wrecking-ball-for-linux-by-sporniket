// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../core/constants.hpp"
#include <string>
#include <utility>
#include <cstdint>

// Forward declarations to avoid SDL2 dependency in interface
struct SDL_Rect;
struct SDL_Point;

namespace wreckingball {

// Forward declarations
struct Brick;
struct Ball;
struct Paddle;

// Color struct is defined in constants.hpp

/**
 * @brief Platform-agnostic rendering interface
 *
 * Abstracts rendering operations to enable future Android porting.
 * SDL2 implementation will be in src/sdl/renderer/sdl_renderer.hpp
 */
class IRenderer {
public:
    virtual ~IRenderer() = default;

    /**
     * @brief Begin frame rendering (clear screen)
     */
    virtual void BeginFrame() = 0;

    /**
     * @brief End frame rendering (present to screen)
     */
    virtual void EndFrame() = 0;

    /**
     * @brief Render filled rectangle
     * @param rect Rectangle bounds
     * @param color Fill color
     */
    virtual void DrawFilledRect(const SDL_Rect& rect, Color color) = 0;

    /**
     * @brief Render brick with appropriate color based on type and hits
     * @param brick Brick to render
     */
    virtual void DrawBrick(const Brick& brick) = 0;

    /**
     * @brief Render ball
     * @param ball Ball to render
     * @param color Ball color
     */
    virtual void DrawBall(const Ball& ball, Color color) = 0;

    /**
     * @brief Render paddle
     * @param paddle Paddle to render
     * @param color Paddle color
     */
    virtual void DrawPaddle(const Paddle& paddle, Color color) = 0;

    /**
     * @brief Render text (HUD, menus)
     * @param text Text string to render
     * @param x X coordinate (logical pixels)
     * @param y Y coordinate (logical pixels)
     * @param color Text color
     */
    virtual void DrawText(const std::string& text, int x, int y, Color color) = 0;

    /**
     * @brief Set logical rendering size (for retro 320x200 resolution)
     * @param width Logical width (typically 320)
     * @param height Logical height (typically 200)
     */
    virtual void SetLogicalSize(int width, int height) = 0;

    /**
     * @brief Get logical rendering size
     * @return Pair of (width, height)
     */
    virtual std::pair<int, int> GetLogicalSize() const = 0;

    /**
     * @brief Convert logical coordinates to screen coordinates
     * @param logical Logical coordinates
     * @return Screen coordinates
     */
    virtual SDL_Point LogicalToScreen(const SDL_Point& logical) const = 0;

    /**
     * @brief Convert screen coordinates to logical coordinates
     * @param screen Screen coordinates
     * @return Logical coordinates
     */
    virtual SDL_Point ScreenToLogical(const SDL_Point& screen) const = 0;
};

} // namespace wreckingball

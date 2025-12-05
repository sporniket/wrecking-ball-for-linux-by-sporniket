// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "../../platform/graphics/renderer_interface.hpp"
#include <SDL2/SDL.h>
#include <memory>

namespace wreckingball {

/**
 * @brief SDL2 implementation of IRenderer
 *
 * Provides retro 320x200 logical rendering with nearest-neighbor scaling
 * for crisp pixel-art aesthetic.
 */
class SDLRenderer : public IRenderer {
public:
    /**
     * @brief Constructor
     * @param window SDL window to render to
     */
    explicit SDLRenderer(SDL_Window* window);

    /**
     * @brief Destructor - cleans up SDL renderer
     */
    ~SDLRenderer() override;

    // Prevent copying
    SDLRenderer(const SDLRenderer&) = delete;
    SDLRenderer& operator=(const SDLRenderer&) = delete;

    // IRenderer interface
    void BeginFrame() override;
    void EndFrame() override;
    void DrawFilledRect(const SDL_Rect& rect, Color color) override;
    void DrawBrick(const Brick& brick) override;
    void DrawBall(const Ball& ball, Color color) override;
    void DrawPaddle(const Paddle& paddle, Color color) override;
    void DrawText(const std::string& text, int x, int y, Color color) override;
    void SetLogicalSize(int width, int height) override;
    std::pair<int, int> GetLogicalSize() const override;
    SDL_Point LogicalToScreen(const SDL_Point& logical) const override;
    SDL_Point ScreenToLogical(const SDL_Point& screen) const override;

    /**
     * @brief Get underlying SDL_Renderer
     * @return SDL renderer pointer
     */
    SDL_Renderer* GetSDLRenderer() const { return renderer_; }

private:
    /**
     * @brief Get color for brick based on type and hits
     * @param brick Brick to get color for
     * @return Brick color
     */
    Color GetBrickColor(const Brick& brick) const;

    /**
     * @brief Set SDL draw color from Color struct
     * @param color Color to set
     */
    void SetDrawColor(Color color);

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int logical_width_;
    int logical_height_;
};

} // namespace wreckingball

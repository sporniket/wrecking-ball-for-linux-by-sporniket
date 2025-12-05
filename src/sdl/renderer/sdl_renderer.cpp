// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "sdl_renderer.hpp"
#include "../../core/entities/brick.hpp"
#include "../../core/entities/ball.hpp"
#include "../../core/entities/paddle.hpp"
#include <stdexcept>
#include <cmath>

namespace wreckingball {

SDLRenderer::SDLRenderer(SDL_Window* window)
    : window_(window), renderer_(nullptr), logical_width_(320), logical_height_(200) {

    if (!window_) {
        throw std::runtime_error("SDL window is null");
    }

    // Create SDL renderer with hardware acceleration
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        throw std::runtime_error(std::string("Failed to create SDL renderer: ") + SDL_GetError());
    }

    // Enable nearest-neighbor scaling for retro pixel-art aesthetic (T051)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");  // 0 = nearest neighbor

    // Set logical rendering size for retro 320x200 resolution (T050)
    SDL_RenderSetLogicalSize(renderer_, logical_width_, logical_height_);

    // Enable alpha blending for transparency
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

SDLRenderer::~SDLRenderer() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
}

void SDLRenderer::BeginFrame() {
    // Clear screen to black
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}

void SDLRenderer::EndFrame() {
    SDL_RenderPresent(renderer_);
}

void SDLRenderer::SetDrawColor(Color color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

void SDLRenderer::DrawFilledRect(const SDL_Rect& rect, Color color) {
    SetDrawColor(color);
    SDL_RenderFillRect(renderer_, &rect);
}

Color SDLRenderer::GetBrickColor(const Brick& brick) const {
    // 16-color EGA-style palette
    static const Color palette[16] = {
        {0, 0, 0, 255},         // 0: Black
        {0, 0, 170, 255},       // 1: Blue
        {0, 170, 0, 255},       // 2: Green
        {0, 170, 170, 255},     // 3: Cyan
        {170, 0, 0, 255},       // 4: Red
        {170, 0, 170, 255},     // 5: Magenta
        {170, 85, 0, 255},      // 6: Brown
        {170, 170, 170, 255},   // 7: Light Gray
        {85, 85, 85, 255},      // 8: Dark Gray
        {85, 85, 255, 255},     // 9: Light Blue
        {85, 255, 85, 255},     // 10: Light Green
        {85, 255, 255, 255},    // 11: Light Cyan
        {255, 85, 85, 255},     // 12: Light Red
        {255, 85, 255, 255},    // 13: Light Magenta
        {255, 255, 85, 255},    // 14: Yellow
        {255, 255, 255, 255}    // 15: White
    };

    // Use brick's color_index (0-15) to select palette color
    uint8_t index = brick.color_index < 16 ? brick.color_index : 7;  // Default to light gray
    return palette[index];
}

void SDLRenderer::DrawBrick(const Brick& brick) {
    if (brick.is_destroyed) {
        return;  // Don't render destroyed bricks
    }

    // Get brick color based on type and hits
    Color brick_color = GetBrickColor(brick);

    // Dim inactive exit bricks (GetTheKey levels)
    if (brick.type == BrickType::Exit && !brick.is_active) {
        brick_color.r /= 2;
        brick_color.g /= 2;
        brick_color.b /= 2;
    }

    // Render brick as filled rectangle (convert units to pixels)
    constexpr int PIXELS_PER_UNIT = 8;
    SDL_Rect rect;
    rect.x = brick.position_x * PIXELS_PER_UNIT;
    rect.y = brick.position_y * PIXELS_PER_UNIT;
    rect.w = brick.width * PIXELS_PER_UNIT;
    rect.h = brick.height * PIXELS_PER_UNIT;  // Always 1 unit height

    DrawFilledRect(rect, brick_color);

    // Draw border for better visibility
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer_, &rect);
}

void SDLRenderer::DrawBall(const Ball& ball, Color color) {
    if (ball.is_lost) {
        return;  // Don't render lost balls
    }

    // Draw ball as filled circle (convert units to pixels)
    constexpr int PIXELS_PER_UNIT = 8;
    int center_x = static_cast<int>(ball.position_x * PIXELS_PER_UNIT);
    int center_y = static_cast<int>(ball.position_y * PIXELS_PER_UNIT);
    int radius = static_cast<int>(ball.radius * PIXELS_PER_UNIT);

    SetDrawColor(color);

    // Simple circle rasterization using midpoint circle algorithm
    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer_, center_x + x, center_y + y);
            }
        }
    }
}

void SDLRenderer::DrawPaddle(const Paddle& paddle, Color color) {
    // Calculate paddle bounds (paddle.position_x/y are center positions, convert units to pixels)
    constexpr int PIXELS_PER_UNIT = 8;
    SDL_Rect rect;
    rect.x = static_cast<int>((paddle.position_x - paddle.width / 2.0) * PIXELS_PER_UNIT);
    rect.y = static_cast<int>((paddle.position_y - paddle.height / 2.0) * PIXELS_PER_UNIT);
    rect.w = paddle.width * PIXELS_PER_UNIT;
    rect.h = paddle.height * PIXELS_PER_UNIT;

    DrawFilledRect(rect, color);

    // Draw border
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer_, &rect);
}

void SDLRenderer::DrawText(const std::string& text, int x, int y, Color color) {
    // TODO: Implement text rendering with bitmap font
    // For now, this is a stub that will be implemented when we add font support
    // Text rendering will use a simple 8x8 bitmap font for retro aesthetic
    (void)text;
    (void)x;
    (void)y;
    (void)color;
}

void SDLRenderer::SetLogicalSize(int width, int height) {
    logical_width_ = width;
    logical_height_ = height;
    SDL_RenderSetLogicalSize(renderer_, width, height);
}

std::pair<int, int> SDLRenderer::GetLogicalSize() const {
    return {logical_width_, logical_height_};
}

SDL_Point SDLRenderer::LogicalToScreen(const SDL_Point& logical) const {
    float scale_x, scale_y;
    SDL_RenderGetScale(renderer_, &scale_x, &scale_y);

    SDL_Point screen;
    screen.x = static_cast<int>(logical.x * scale_x);
    screen.y = static_cast<int>(logical.y * scale_y);
    return screen;
}

SDL_Point SDLRenderer::ScreenToLogical(const SDL_Point& screen) const {
    float scale_x, scale_y;
    SDL_RenderGetScale(renderer_, &scale_x, &scale_y);

    SDL_Point logical;
    logical.x = static_cast<int>(screen.x / scale_x);
    logical.y = static_cast<int>(screen.y / scale_y);
    return logical;
}

} // namespace wreckingball

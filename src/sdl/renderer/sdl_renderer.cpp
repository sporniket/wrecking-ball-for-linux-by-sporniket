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
    // Simple bitmap font text rendering
    int cursor_x = x;
    for (char ch : text) {
        DrawChar(ch, cursor_x, y, color);
        cursor_x += 6;  // Character width (5 pixels + 1 pixel spacing)
    }
}

void SDLRenderer::DrawChar(char ch, int x, int y, Color color) {
    const uint8_t* bitmap = GetCharBitmap(ch);
    if (!bitmap) {
        return;  // Unknown character, skip
    }

    SetDrawColor(color);

    // Render 5x7 character bitmap
    for (int row = 0; row < 7; ++row) {
        uint8_t row_data = bitmap[row];
        for (int col = 0; col < 5; ++col) {
            if (row_data & (1 << (4 - col))) {  // Check bit from left to right
                SDL_RenderDrawPoint(renderer_, x + col, y + row);
            }
        }
    }
}

const uint8_t* SDLRenderer::GetCharBitmap(char ch) const {
    // Simple 5x7 bitmap font (each byte represents one row, 5 bits used)
    // Font data for uppercase letters, digits, and basic punctuation

    static const uint8_t font_A[] = {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};  // A
    static const uint8_t font_B[] = {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E};  // B
    static const uint8_t font_C[] = {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E};  // C
    static const uint8_t font_D[] = {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E};  // D
    static const uint8_t font_E[] = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F};  // E
    static const uint8_t font_F[] = {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10};  // F
    static const uint8_t font_G[] = {0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F};  // G
    static const uint8_t font_H[] = {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};  // H
    static const uint8_t font_I[] = {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E};  // I
    static const uint8_t font_J[] = {0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C};  // J
    static const uint8_t font_K[] = {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11};  // K
    static const uint8_t font_L[] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F};  // L
    static const uint8_t font_M[] = {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11};  // M
    static const uint8_t font_N[] = {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11};  // N
    static const uint8_t font_O[] = {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};  // O
    static const uint8_t font_P[] = {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10};  // P
    static const uint8_t font_Q[] = {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D};  // Q
    static const uint8_t font_R[] = {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11};  // R
    static const uint8_t font_S[] = {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E};  // S
    static const uint8_t font_T[] = {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};  // T
    static const uint8_t font_U[] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};  // U
    static const uint8_t font_V[] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04};  // V
    static const uint8_t font_W[] = {0x11, 0x11, 0x11, 0x15, 0x15, 0x1B, 0x11};  // W
    static const uint8_t font_X[] = {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11};  // X
    static const uint8_t font_Y[] = {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04};  // Y
    static const uint8_t font_Z[] = {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F};  // Z

    static const uint8_t font_0[] = {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E};  // 0
    static const uint8_t font_1[] = {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E};  // 1
    static const uint8_t font_2[] = {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F};  // 2
    static const uint8_t font_3[] = {0x1F, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0E};  // 3
    static const uint8_t font_4[] = {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02};  // 4
    static const uint8_t font_5[] = {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E};  // 5
    static const uint8_t font_6[] = {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E};  // 6
    static const uint8_t font_7[] = {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08};  // 7
    static const uint8_t font_8[] = {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E};  // 8
    static const uint8_t font_9[] = {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C};  // 9

    static const uint8_t font_space[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // Space
    static const uint8_t font_colon[] = {0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 0x00};  // :
    static const uint8_t font_slash[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x00};  // /
    static const uint8_t font_dash[] = {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00};   // -
    static const uint8_t font_lparen[] = {0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02}; // (
    static const uint8_t font_rparen[] = {0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08}; // )
    static const uint8_t font_period[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00}; // .
    static const uint8_t font_gt[] = {0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08};     // >

    // Map characters to bitmaps
    if (ch >= 'A' && ch <= 'Z') {
        static const uint8_t* const uppercase[] = {
            font_A, font_B, font_C, font_D, font_E, font_F, font_G, font_H, font_I,
            font_J, font_K, font_L, font_M, font_N, font_O, font_P, font_Q, font_R,
            font_S, font_T, font_U, font_V, font_W, font_X, font_Y, font_Z
        };
        return uppercase[ch - 'A'];
    }

    // Lowercase letters use same as uppercase
    if (ch >= 'a' && ch <= 'z') {
        static const uint8_t* const uppercase[] = {
            font_A, font_B, font_C, font_D, font_E, font_F, font_G, font_H, font_I,
            font_J, font_K, font_L, font_M, font_N, font_O, font_P, font_Q, font_R,
            font_S, font_T, font_U, font_V, font_W, font_X, font_Y, font_Z
        };
        return uppercase[ch - 'a'];
    }

    if (ch >= '0' && ch <= '9') {
        static const uint8_t* const digits[] = {
            font_0, font_1, font_2, font_3, font_4, font_5, font_6, font_7, font_8, font_9
        };
        return digits[ch - '0'];
    }

    // Special characters
    switch (ch) {
        case ' ': return font_space;
        case ':': return font_colon;
        case '/': return font_slash;
        case '-': return font_dash;
        case '(': return font_lparen;
        case ')': return font_rparen;
        case '.': return font_period;
        case '>': return font_gt;
        default: return font_space;  // Unknown character = space
    }
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

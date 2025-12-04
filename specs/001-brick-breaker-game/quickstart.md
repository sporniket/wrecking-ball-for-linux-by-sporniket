# Developer Quickstart Guide

**Feature**: Sporniket's Wrecking Ball - Brick Breaker Game
**Date**: 2025-12-04
**Audience**: Developers contributing to the project

## Overview

This guide helps developers set up the development environment, understand the codebase structure, and start contributing to Sporniket's Wrecking Ball brick breaker game.

---

## Prerequisites

### Required Software

| Tool | Min Version | Purpose | Installation |
|------|-------------|---------|--------------|
| GCC or Clang | GCC 13+ or Clang 16+ | C++23 compiler | `sudo apt install g++-13` or `build-essential` |
| CMake | 3.25+ | Build system | `sudo apt install cmake` |
| SDL2 | 2.0.20+ | Graphics/input/audio | `sudo apt install libsdl2-dev` |
| SDL2_ttf | 2.20+ | Text rendering | `sudo apt install libsdl2-ttf-dev` |
| SDL2_mixer | 2.6+ | Audio playback | `sudo apt install libsdl2-mixer-dev` |
| Git | 2.x | Version control | `sudo apt install git` |
| inih | r56+ | INI parsing | Vendored or `sudo apt install libinih-dev` |
| Catch2 | 3.x | Unit testing | FetchContent or `sudo apt install catch2` |

### Recommended Tools

- **Clang-Format**: Code formatting (use `.clang-format` in repo root)
- **Clang-Tidy**: Static analysis
- **Valgrind**: Memory leak detection
- **GDB**: Debugging

---

## Quick Setup (5 Minutes)

```bash
# Clone repository
git clone <repository-url> wrecking-ball
cd wrecking-ball

# Install dependencies (Ubuntu/Debian)
sudo apt update
sudo apt install -y g++-13 cmake libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev catch2

# Build project
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -j$(nproc)

# Run game
./wrecking-ball

# Run tests
ctest --output-on-failure
```

---

## Project Structure

```text
wrecking-ball/
├── src/                      # Source code
│   ├── core/                 # Platform-agnostic game logic
│   │   ├── entities/         # Game entities (Ball, Brick, Level, etc.)
│   │   ├── physics/          # Collision detection, ball physics
│   │   ├── level/            # Level loading, completion logic
│   │   └── game/             # Game state management
│   ├── platform/             # Platform abstraction interfaces
│   ├── sdl/                  # SDL2 implementation of platform layer
│   ├── persistence/          # INI/Markdown parsing, validation
│   ├── ui/                   # Game screens (menu, gameplay, settings, etc.)
│   └── main.cpp              # Entry point
├── tests/                    # Test suite
│   ├── unit/                 # Unit tests (physics, parsing, entities)
│   ├── integration/          # Integration tests (game flow)
│   └── manual/               # Manual playtesting checklists
├── assets/                   # Game assets
│   ├── levels/               # Built-in level files (.md)
│   ├── fonts/                # TTF fonts
│   └── audio/                # Music and sound effects
├── data/                     # Runtime data (created at first run)
│   ├── settings/             # game-settings.ini
│   ├── progress/             # player-progress.ini
│   ├── custom_levels/        # User-created levels
│   └── logs/                 # Error logs
├── specs/                    # Design documents
│   └── 001-brick-breaker-game/
│       ├── spec.md           # Feature specification
│       ├── plan.md           # Implementation plan
│       ├── research.md       # Technology decisions
│       ├── data-model.md     # Entity definitions
│       ├── contracts/        # API contracts
│       └── quickstart.md     # This file
├── CMakeLists.txt            # Build configuration
├── LICENSE                   # AGPL-3.0 license
└── README.md                 # Project overview
```

---

## Architecture Overview

### Layer Separation

```text
┌───────────────────────────────────────┐
│      UI Layer (Game Screens)          │  ← User-facing screens
│  Menu, Gameplay, Settings, Editor     │
├───────────────────────────────────────┤
│      Core Game Logic                  │  ← Platform-agnostic
│  Physics, Level, Entities, State Mgmt │
├───────────────────────────────────────┤
│    Platform Abstraction Layer         │  ← Interfaces (IRenderer, IAudio, etc.)
│   IRenderer, IAudio, IInput, IFiles   │
├───────────────────────────────────────┤
│    Platform Implementation (SDL2)     │  ← Linux-specific (replaceable for Android)
│   SDL_Renderer, SDL_Mixer, SDL_Input  │
└───────────────────────────────────────┘
```

**Key Principle**: Core game logic NEVER directly calls SDL2 or Linux-specific APIs. Always go through platform abstraction interfaces.

### Key Design Patterns

- **Dependency Injection**: Systems receive interfaces, not concrete implementations
- **Entity-Component (lightweight)**: Entities have data, systems operate on entities
- **Observer Pattern**: Achievement tracker observes player progress changes
- **State Machine**: Game screens transition via `GameScreen` enum

---

## Building the Project

### CMake Configuration

```bash
# Debug build (recommended for development)
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-13 ..

# Release build (optimized for performance)
cmake -DCMAKE_BUILD_TYPE=Release ..

# With sanitizers (detect memory issues, UB)
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON ..
```

### Build Targets

```bash
# Build everything
cmake --build . -j$(nproc)

# Build only game executable
cmake --build . --target wrecking-ball

# Build tests
cmake --build . --target tests

# Run tests
ctest --output-on-failure

# Run specific test suite
./tests/unit/physics_tests
```

---

## Development Workflow

### 1. Pick a User Story

Refer to `specs/001-brick-breaker-game/spec.md` for prioritized user stories (P1-P8).

**Recommended implementation order**:
1. **P1**: Core gameplay with three level types ← Start here
2. **P2**: Main menu & navigation
3. **P3**: Arcade mode
4. **P4**: Level selection & progress tracking
5. **P5-P8**: Achievements, settings, level editor, other features

### 2. Understand the Data Model

Read `specs/001-brick-breaker-game/data-model.md` to understand:
- Entity fields and constraints
- Validation rules
- State transitions
- Serialization formats

### 3. Review API Contracts

Read `specs/001-brick-breaker-game/contracts/game-systems-api.md` for:
- System interfaces (e.g., `IPhysicsEngine`, `ILevelLoader`)
- Method signatures and contracts
- Performance requirements
- Error handling rules

### 4. Write Tests First (TDD Recommended)

```cpp
// tests/unit/physics/collision_test.cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include <catch2/catch_test_macros.hpp>
#include "core/physics/physics_engine.hpp"

TEST_CASE("Brick collision detection", "[physics][collision]") {
    PhysicsEngine engine;

    SECTION("Detects collision with 1-unit wide brick") {
        Brick brick{.position_x=10, .position_y=5, .width=1, .type=BrickType::Regular};
        Ball ball{.position_x=80.0f, .position_y=40.0f, .velocity_x=0, .velocity_y=100.0f};

        std::vector<Brick> bricks = {brick};
        Brick* hit = engine.CheckBrickCollision(ball, bricks);

        REQUIRE(hit != nullptr);
        REQUIRE(hit->position_x == 10);
    }

    SECTION("Detects collision with 10-unit wide brick") {
        Brick brick{.position_x=5, .position_y=5, .width=10, .type=BrickType::Regular};
        Ball ball{.position_x=100.0f, .position_y=40.0f, .velocity_x=0, .velocity_y=100.0f};

        std::vector<Brick> bricks = {brick};
        Brick* hit = engine.CheckBrickCollision(ball, bricks);

        REQUIRE(hit != nullptr);  // Ball hits wide brick
    }
}
```

### 5. Implement the Feature

```cpp
// src/core/physics/physics_engine.hpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once
#include "platform/input/physics_interface.hpp"
#include "core/entities/ball.hpp"
#include "core/entities/brick.hpp"
#include <vector>

class PhysicsEngine : public IPhysicsEngine {
public:
    Brick* CheckBrickCollision(Ball& ball, std::vector<Brick>& bricks) override;
    // ... other methods
};
```

```cpp
// src/core/physics/physics_engine.cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "physics_engine.hpp"
#include <cmath>

Brick* PhysicsEngine::CheckBrickCollision(Ball& ball, std::vector<Brick>& bricks) {
    for (auto& brick : bricks) {
        if (brick.is_destroyed) continue;

        // Convert brick position (grid units) to pixels
        float brick_x = brick.position_x * 8.0f;  // 8 pixels per unit
        float brick_y = brick.position_y * 8.0f;
        float brick_w = brick.width * 8.0f;
        float brick_h = 8.0f;  // Fixed 1 unit height

        // AABB collision detection
        if (ball.position_x + ball.radius >= brick_x &&
            ball.position_x - ball.radius <= brick_x + brick_w &&
            ball.position_y + ball.radius >= brick_y &&
            ball.position_y - ball.radius <= brick_y + brick_h) {

            // Collision detected! Update brick and ball
            brick.hits_remaining--;
            if (brick.hits_remaining == 0) {
                brick.is_destroyed = true;
            }

            // Bounce ball (reverse Y velocity for simplicity)
            ball.velocity_y = -ball.velocity_y;

            return &brick;
        }
    }
    return nullptr;  // No collision
}
```

### 6. Run Tests

```bash
# Run all tests
ctest --output-on-failure

# Run specific test
./tests/unit/physics_tests -c "Brick collision detection"

# Run with verbose output
./tests/unit/physics_tests -s
```

### 7. Manual Testing

```bash
# Build and run game
cmake --build . --target wrecking-ball
./wrecking-ball

# Test checklist (from tests/manual/)
# - Start level with variable-width bricks
# - Hit 1-unit, 5-unit, 10-unit, full-width bricks
# - Verify collision detection works for all widths
# - Check ball bounces correctly
```

---

## Code Style Guidelines

### Copyright Headers

Every source file MUST include:

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later
```

### Naming Conventions

```cpp
// Classes: PascalCase
class PhysicsEngine { };

// Interfaces: IPascalCase
class IPhysicsEngine { };

// Enums: PascalCase
enum class BrickType { Regular, Star, Key, Exit };

// Functions/Methods: PascalCase
void UpdateGameState();

// Variables: snake_case
int player_score = 0;
float ball_velocity_x = 100.0f;

// Constants: SCREAMING_SNAKE_CASE
constexpr int MAX_LIVES = 99;
constexpr float BALL_SPEED = 300.0f;

// Member variables: snake_case with m_ prefix (optional)
class Ball {
    float m_position_x;
    float m_position_y;
};
```

### Modern C++23 Features (Use These!)

```cpp
// Designated initializers
Brick brick{
    .position_x = 10,
    .position_y = 5,
    .width = 4,
    .type = BrickType::Regular
};

// std::optional for nullable values
std::optional<Brick> FindBrick(uint16_t id);

// std::expected for error handling (C++23)
std::expected<Level, std::string> LoadLevel(const std::string& path);

// Ranges and views
auto destroyed_bricks = bricks
    | std::views::filter([](const Brick& b) { return b.is_destroyed; });

// Concepts for constraints
template<typename T>
concept Collidable = requires(T obj) {
    { obj.GetBoundingBox() } -> std::convertible_to<SDL_Rect>;
};
```

---

## Debugging Tips

### GDB Quick Reference

```bash
# Run game in GDB
gdb ./wrecking-ball

# Set breakpoint
(gdb) break PhysicsEngine::CheckBrickCollision
(gdb) run

# Print variable
(gdb) print ball.position_x

# Step through code
(gdb) step   # Step into function
(gdb) next   # Step over function
(gdb) continue  # Continue execution

# Backtrace (call stack)
(gdb) backtrace
```

### Logging

```cpp
// Use stderr for error logging
#include <iostream>

std::cerr << "[ERROR] Failed to load level: " << filename << std::endl;

// Log to error file
void LogError(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();

    std::string log_file = "data/logs/error-" + std::to_string(ms) + ".log";
    std::ofstream log(log_file, std::ios::app);
    log << "[" << ms << "] " << message << std::endl;
}
```

### Valgrind (Memory Leaks)

```bash
# Check for memory leaks
valgrind --leak-check=full ./wrecking-ball

# Suppress SDL2 false positives (create valgrind.supp file)
valgrind --leak-check=full --suppressions=valgrind.supp ./wrecking-ball
```

---

## Common Tasks

### Adding a New Brick Type

1. Update `BrickType` enum in `src/core/entities/brick.hpp`
2. Add brick code to level file parser in `src/persistence/markdown/level_parser.cpp`
3. Update rendering in `src/sdl/renderer/sdl_renderer.cpp` (assign color)
4. Add tests in `tests/unit/level/level_parser_test.cpp`
5. Create sample level using new brick type in `assets/levels/`

### Adding a New Achievement

1. Define achievement in `src/core/game/achievements.cpp`:
   ```cpp
   Achievement{
       .achievement_id = "triple_bounce",
       .name = "Triple Threat",
       .description = "Hit 3 bricks with one ball bounce",
       .unlock_criteria = [](const PlayerProgress& p) {
           return p.max_bricks_per_bounce >= 3;
       }
   }
   ```
2. Update `PlayerProgress` entity to track new statistic if needed
3. Add test in `tests/unit/game/achievement_test.cpp`
4. Update trophies screen UI to display new achievement

### Creating a New Level

1. Create Markdown file in `assets/levels/` or `data/custom_levels/`
2. Follow format from `data-model.md` (front matter + fenced code block)
3. Use brick encoding: `.` (empty), `1-9` (regular), `*` (star), `k` (key), `x` (exit)
4. Use `-` to extend brick width (e.g., `3---` = 4-unit wide, 3 hits)
5. Test level in-game or with level parser unit test

Example level file:
```markdown
---
name: Star Collector
type: star
author: Developer
difficulty: 2
---

# Star Collector Level

Collect all the stars!

```brick-layout
......................................
..*-------*-------*-------*-------*-..
..1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-..
..*---*---*---*---*---*---*---*---*-..
..2-2-2-2-2-2-2-2-2-2-2-2-2-2-2-2-2-..
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
......................................
```
```

---

## Performance Optimization

### Profiling

```bash
# Compile with profiling
cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_PROFILING=ON ..
cmake --build .

# Run with profiler
gprof ./wrecking-ball > profile.txt

# Or use perf (Linux)
perf record ./wrecking-ball
perf report
```

### Common Bottlenecks

1. **Collision detection**: Use spatial partitioning (grid) if >100 bricks
2. **Rendering**: Batch draw calls, cache static elements (HUD, brick layout)
3. **File I/O**: Load levels asynchronously in background thread
4. **Memory allocations**: Pre-allocate vectors, use object pools for balls

### 60 FPS Target

```cpp
// Frame timing loop (see research.md for full implementation)
const double dt = 1.0 / 60.0;
double accumulator = 0.0;

while (running) {
    double frame_start = SDL_GetPerformanceCounter();

    // Fixed timestep update (game logic)
    accumulator += frame_time;
    while (accumulator >= dt) {
        game_state.Update(dt);  // Must complete in <16ms!
        accumulator -= dt;
    }

    // Variable rendering
    renderer.BeginFrame();
    // ... render scene ...
    renderer.EndFrame();

    frame_time = (SDL_GetPerformanceCounter() - frame_start) / frequency;
}
```

---

## Contributing Guidelines

1. **Read documentation** (`spec.md`, `plan.md`, `data-model.md`, `contracts/`)
2. **Follow user story priorities** (implement P1 before P5)
3. **Write tests first** (TDD recommended)
4. **Include copyright headers** in all source files
5. **Format code** with `clang-format` before committing
6. **Test manually** with checklist from `tests/manual/`
7. **Update documentation** if adding new systems or changing APIs
8. **Commit message format**: `<type>: <description>` (e.g., `feat: add star brick collision detection`)

### Pull Request Checklist

- [ ] All tests pass (`ctest`)
- [ ] Code formatted (`clang-format`)
- [ ] Copyright headers present
- [ ] Documentation updated (if API changed)
- [ ] Manual testing completed (relevant checklist)
- [ ] Performance verified (if gameplay code: maintains 60 FPS)
- [ ] No memory leaks (Valgrind clean)

---

## Troubleshooting

### Build Errors

**Error**: `SDL2 not found`
```bash
# Solution: Install SDL2 development packages
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

**Error**: `C++23 features not supported`
```bash
# Solution: Use newer compiler
sudo apt install g++-13
cmake -DCMAKE_CXX_COMPILER=g++-13 ..
```

### Runtime Errors

**Error**: `Failed to load level: <filename>`
- Check file exists in `assets/levels/` or `data/custom_levels/`
- Validate Markdown format (front matter, fenced code block)
- Check error log in `data/logs/error-<timestamp>.log`

**Error**: `Audio initialization failed`
- Non-fatal: game continues in silent mode
- Check audio device available: `aplay -l`
- Verify SDL2_mixer installed

### Performance Issues

**Game runs at <60 FPS**
- Profile with `gprof` or `perf`
- Check collision detection loop (optimize with spatial partitioning)
- Verify VSync enabled in settings
- Reduce brick count in level (test with simpler levels)

---

## Resources

- **SDL2 Documentation**: https://wiki.libsdl.org/
- **Catch2 Documentation**: https://github.com/catchorg/Catch2/tree/devel/docs
- **C++23 Reference**: https://en.cppreference.com/w/cpp/23
- **CMake Tutorial**: https://cmake.org/cmake/help/latest/guide/tutorial/

---

## Next Steps

1. **Set up development environment** (install dependencies)
2. **Build and run tests** (`cmake`, `ctest`)
3. **Read specification** (`spec.md`) to understand feature requirements
4. **Pick User Story P1** (Core Gameplay) and start implementation
5. **Join discussions** (if using GitHub Discussions / Discord / etc.)

Happy coding! 🎮🧱

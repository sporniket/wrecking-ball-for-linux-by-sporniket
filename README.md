# Sporniket's Wrecking Ball

**A brick breaker game for Linux with distinctive gameplay mechanics**

Copyright (c) 2025 David SPORN
Licensed under AGPL-3.0-or-later

---

## Overview

Sporniket's Wrecking Ball is a modern brick breaker game featuring:

- **Three Distinct Level Types**:
  - **Classical**: Break all breakable bricks
  - **Reach for the Stars**: Break all star-shaped bricks only
  - **Get the Key**: Collect keys to unlock exit bricks, then escape

- **Variable-Width Bricks**: Bricks range from 1 unit to full playfield width (40 units)
- **Retro Aesthetic**: 320x200 apparent resolution with 16-color palette
- **Two Game Modes**: Casual (unlimited lives) and Arcade (limited lives)
- **Level Editor**: Create and share custom levels
- **Cross-Platform Architecture**: Linux first, designed for future Android porting

---

## Build Requirements

### Compiler
- **GCC 13+** or **Clang 16+** (C++23 support required)

### Dependencies
- **CMake 3.25+**
- **SDL2 2.0.20+**
- **SDL2_ttf 2.20+**
- **SDL2_mixer 2.6+**

### Ubuntu/Debian Installation
```bash
sudo apt update
sudo apt install -y build-essential cmake g++-13 \
    libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

---

## Building from Source

```bash
# Clone repository
git clone <repository-url> wrecking-ball
cd wrecking-ball

# Create build directory
mkdir build && cd build

# Configure (Debug build)
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-13 ..

# Or configure (Release build)
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-13 ..

# Build
cmake --build . -j$(nproc)

# Run
./wrecking-ball
```

### Build Options

- **Debug build** (recommended for development):
  ```bash
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  ```

- **Release build** (optimized):
  ```bash
  cmake -DCMAKE_BUILD_TYPE=Release ..
  ```

- **With sanitizers** (detect memory issues):
  ```bash
  cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON ..
  ```

---

## Running Tests

```bash
# From build directory
ctest --output-on-failure

# Run specific test
./tests/unit/physics_tests

# With verbose output
./tests/unit/physics_tests -s
```

---

## Project Structure

```
wrecking-ball/
├── src/                    # Source code
│   ├── core/               # Platform-agnostic game logic
│   ├── platform/           # Abstraction interfaces
│   ├── sdl/                # SDL2 implementation
│   ├── persistence/        # File I/O (INI, Markdown)
│   └── ui/                 # Game screens
├── tests/                  # Test suite
│   ├── unit/               # Unit tests
│   ├── integration/        # Integration tests
│   └── manual/             # Playtesting checklists
├── assets/                 # Game assets
│   ├── levels/             # Level files (.md)
│   ├── fonts/              # TTF fonts
│   └── audio/              # Music and SFX
├── data/                   # Runtime data (created at first run)
│   ├── settings/           # Game settings
│   ├── progress/           # Player progress
│   ├── custom_levels/      # User-created levels
│   └── logs/               # Error logs
└── specs/                  # Design documents
    └── 001-brick-breaker-game/
        ├── spec.md         # Feature specification
        ├── plan.md         # Implementation plan
        ├── data-model.md   # Entity definitions
        ├── contracts/      # API contracts
        └── tasks.md        # Implementation tasks
```

---

## Development Status

**Current Phase**: Phase 1 - Setup (In Progress)

- [x] Project structure created
- [x] CMake build system configured
- [x] LICENSE and README created
- [ ] Third-party dependencies vendored
- [ ] Testing framework setup complete

**Next Phase**: Phase 2 - Foundational (Platform abstraction, core entities)

See `specs/001-brick-breaker-game/tasks.md` for detailed implementation plan.

---

## Gameplay

### Level Types

#### Classical Levels
Break all breakable bricks to complete the level. Traditional brick breaker gameplay.

#### Reach for the Stars Levels
Only star-shaped bricks count toward completion. Regular bricks can be ignored or used strategically for ball bounces.

#### Get the Key Levels
1. Break all key-shaped bricks to unlock the exit
2. Exit bricks become active (visually distinct)
3. Break any exit brick to complete the level

### Controls

- **Mouse**: Move paddle (default)
- **Keyboard**: Arrow keys or WASD
- **Game Controller**: Left stick or D-pad
- **Space/Click**: Launch ball
- **ESC**: Pause game

### Game Modes

- **Casual Mode**: Unlimited balls, level selection, relaxed gameplay
- **Arcade Mode**: Limited lives (3/5/7), sequential levels from 1, high score challenge

---

## Architecture Highlights

### Platform Abstraction
Core game logic is completely separated from platform-specific code through abstraction interfaces:
- `IRenderer` - Graphics rendering
- `IAudioPlayer` - Sound and music
- `IInputHandler` - Mouse/keyboard/controller input
- `IFileIO` - File system operations

This design enables future Android porting by swapping SDL2 implementation with Android NDK equivalents.

### Performance
- **60 FPS target** with fixed timestep game loop
- **<16ms input latency** (one frame at 60 FPS)
- Efficient collision detection for variable-width bricks
- Memory-optimized for future mobile deployment

### Security
- All file parsing hardened against malformed input
- Bounds checking on all loaded data
- Immediate abort on validation errors with diagnostic logging

---

## Contributing

1. Read design documents in `specs/001-brick-breaker-game/`
2. Follow copyright header requirements (see below)
3. Format code with `clang-format` before committing
4. Write tests for new features
5. Update documentation as needed

### Copyright Headers

Every source file MUST include:

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later
```

---

## License

This project is licensed under the **GNU Affero General Public License v3.0 or later** (AGPL-3.0-or-later).

See [LICENSE](LICENSE) for the full license text.

### Why AGPL?
The AGPL license ensures that if you modify this game and provide it as a network service, you must also provide the source code to users. This protects the copyleft nature of the project even in server/cloud scenarios.

---

## Third-Party Licenses

This project uses the following open-source libraries:

- **SDL2** - zlib License
- **SDL2_ttf** - zlib License
- **SDL2_mixer** - zlib License
- **inih** - BSD 3-Clause License
- **Catch2** - Boost Software License 1.0

See [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md) for complete license texts.

---

## Contact & Links

- **Developer**: David SPORN
- **License**: AGPL-3.0-or-later
- **Issue Tracker**: (To be added)
- **Documentation**: `specs/001-brick-breaker-game/`

---

## Acknowledgments

Special thanks to:
- The SDL2 development team for excellent cross-platform multimedia library
- Catch2 team for modern C++ testing framework
- All contributors to the open-source libraries used in this project

---

**Enjoy breaking bricks! 🎮🧱**

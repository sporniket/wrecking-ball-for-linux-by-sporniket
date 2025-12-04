# Implementation Plan: Sporniket's Wrecking Ball - Complete Game

**Branch**: `001-brick-breaker-game` | **Date**: 2025-12-04 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/001-brick-breaker-game/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/commands/plan.md` for the execution workflow.

## Summary

Implement a brick breaker game for Linux desktop with three distinct level types (Classical, "Reach for the Stars", "Get the Key") and variable-width bricks. The game will be built using modern C++23 with SDL2 for rendering, featuring retro/vintage visual style (320x200 apparent resolution, 16-color palette), INI-based persistence for settings and progress, and GitHub Flavored Markdown for level data. The architecture must support future Android porting through platform abstraction layers.

## Technical Context

**Language/Version**: C++23 (modern C++ features, requiring GCC 13+ or Clang 16+)
**Primary Dependencies**:
- SDL2 (graphics, input, window management)
- SDL2_ttf (text rendering for HUD and menus)
- SDL2_mixer (audio playback for music and sound effects)
- NEEDS CLARIFICATION: INI file parsing library (e.g., inih, boost::property_tree, or custom parser)
- NEEDS CLARIFICATION: Markdown parsing library for level files (e.g., cmark-gfm, md4c, or custom parser)
- NEEDS CLARIFICATION: Unit testing framework (e.g., Google Test, Catch2, doctest)

**Storage**: File-based persistence
- INI format for settings (game-settings.ini) and player progress (player-progress.ini)
- GitHub Flavored Markdown for level data files (one file per level, human-readable with fenced code blocks)
- Error logs: `error-<timestamp>.log` with timestamp in milliseconds
- All file I/O must be hardened against malformed data with bounds checking and immediate abort on errors

**Testing**: NEEDS CLARIFICATION (unit testing framework selection pending)
- Unit tests for game logic (collision detection, level completion detection, score calculation)
- Unit tests for file parsing (INI, Markdown level data) with malformed input handling
- Integration tests for game state management and input processing
- Manual playtesting checklists for each level type and user experience validation

**Target Platform**: Linux desktop (X11/Wayland), with architecture designed for future Android porting
- Display modes: windowed or fullscreen
- Input methods: mouse (default), keyboard, game controller (configurable in settings)

**Project Type**: Single native desktop application with platform abstraction layer

**Performance Goals**:
- Minimum 60 FPS during active gameplay
- Input latency below 16ms (one frame at 60 FPS)
- Asset scaling from 320x200 apparent resolution to actual screen size with no stuttering

**Visual Specifications**:
- Retro/vintage pixelized aesthetic
- Apparent screen resolution: 320x200 "pixels" (8x8 pixel units)
- Grid system: 40 units wide × 25 units high (each unit = 8 apparent pixels = 8x8 px block)
- Play field: 38 units wide × 23 units high, vertically centered at bottom
- HUD area: Top line of grid (40 units × 1 unit)
- 16-color palette only: #112, #423, #336, #555, #843, #362, #c44, #776, #57c, #c73, #899, #6a3, #ca9, #6bc, #dc6, #ded

**Constraints**:
- Security: All file parsing must validate bounds, lengths, and values; abort on malformed data with diagnostic logging
- Memory: Efficient enough for future mobile deployment (minimize allocations during gameplay)
- Offline-capable: No network dependencies, fully playable offline
- Portability: Platform-specific code isolated from game logic for Android porting

**Scale/Scope**:
- 10-30 built-in levels with mix of all three level types
- 8 main screens (Main Menu, Casual Game, Arcade Game, Level Selection, Trophies, Settings, Level Editor, My Other Games)
- Custom level support via level editor
- Variable-width bricks (1 to 40 units)
- 4+ brick types (regular, star, key, exit) with multiple durability levels

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

### I. Cross-Platform Foundation
**Status**: ✅ PASS

- Native Linux desktop application specified (C++23 with SDL2)
- Platform abstraction layer planned to isolate platform-specific code (SDL2 provides cross-platform windowing, input, and rendering)
- Architecture designed with Android porting in mind per requirements
- Game logic will be separated from platform I/O through abstraction layers

### II. Legal Compliance
**Status**: ✅ PASS

- All source files will include copyright notice: "Copyright (c) 2025 David SPORN"
- SPDX identifier will be included: "SPDX-License-Identifier: AGPL-3.0-or-later"
- LICENSE file with full AGPL-3.0 license text will be in repository root
- Third-party dependencies (SDL2, SDL2_ttf, SDL2_mixer, and selected parsing libraries) will be documented with license compatibility verification

### III. Brick Breaker Genre Fidelity
**Status**: ✅ PASS

- Paddle control via keyboard/mouse (and game controller) specified
- Ball physics with collision detection required
- Three distinct level types with brick layouts specified (Classical, "Reach for the Stars", "Get the Key")
- Variable-width bricks (1-40 units) with multiple brick types (regular, star, key, exit)
- Score tracking and progression system (unlocked levels, achievements, statistics)
- Multiple game modes (Casual with unlimited balls, Arcade with limited lives)

### IV. Performance & Responsiveness
**Status**: ✅ PASS

- 60 FPS minimum performance goal explicitly specified
- Input latency below 16ms explicitly required
- Smooth ball physics required (no stuttering or frame drops)
- Memory efficiency considered for future mobile deployment
- Asset scaling optimization required (320x200 apparent to actual screen size)

### V. Testing & Quality Assurance
**Status**: ⚠️ NEEDS CLARIFICATION (Framework selection pending)

- Unit test framework selection needed (Google Test, Catch2, or doctest)
- Test coverage planned for:
  - Game logic (collision detection, level completion for all three types, scoring)
  - File parsing (INI and Markdown with malformed input handling)
  - Integration tests for game state management and input processing
- Manual playtesting checklists planned for user experience validation
- Performance profiling required to verify 60 FPS and <16ms latency

**Overall Status**: ✅ CONDITIONALLY PASS - Proceed to Phase 0 research to resolve NEEDS CLARIFICATION items

## Project Structure

### Documentation (this feature)

```text
specs/[###-feature]/
├── plan.md              # This file (/speckit.plan command output)
├── research.md          # Phase 0 output (/speckit.plan command)
├── data-model.md        # Phase 1 output (/speckit.plan command)
├── quickstart.md        # Phase 1 output (/speckit.plan command)
├── contracts/           # Phase 1 output (/speckit.plan command)
└── tasks.md             # Phase 2 output (/speckit.tasks command - NOT created by /speckit.plan)
```

### Source Code (repository root)

```text
src/
├── core/                  # Platform-agnostic game logic
│   ├── entities/          # Game entities (Ball, Paddle, Brick, Level)
│   ├── physics/           # Collision detection, ball physics
│   ├── level/             # Level loading, completion detection, level types
│   └── game/              # Game state management, score, lives
├── platform/              # Platform abstraction layer
│   ├── input/             # Input handling (mouse, keyboard, controller)
│   ├── graphics/          # Rendering abstraction
│   ├── audio/             # Audio playback abstraction
│   └── files/             # File I/O abstraction
├── sdl/                   # SDL2 platform implementation
│   ├── window/            # SDL2 window and display management
│   ├── renderer/          # SDL2 rendering implementation
│   ├── input/             # SDL2 input implementation
│   └── audio/             # SDL2_mixer audio implementation
├── persistence/           # Data persistence layer
│   ├── ini/               # INI file parsing and writing
│   ├── markdown/          # Markdown level file parsing
│   └── validation/        # Input validation and bounds checking
├── ui/                    # User interface screens
│   ├── menu/              # Main menu screen
│   ├── gameplay/          # Gameplay screen (casual/arcade)
│   ├── level_select/      # Level selection screen
│   ├── trophies/          # Achievements/trophies screen
│   ├── settings/          # Settings/configuration screen
│   ├── level_editor/      # Level editor screen
│   └── other_games/       # "My Other Games" promotional screen
├── assets/                # Game assets (organized by type)
│   ├── levels/            # Built-in level files (.md)
│   ├── fonts/             # TTF fonts for text rendering
│   └── audio/             # Music and sound effects
└── main.cpp               # Application entry point

tests/
├── unit/                  # Unit tests for game logic and parsing
│   ├── physics/           # Collision detection tests
│   ├── level/             # Level completion logic tests
│   ├── persistence/       # File parsing tests with malformed input
│   └── entities/          # Entity behavior tests
├── integration/           # Integration tests for system interactions
│   ├── game_state/        # Game state management tests
│   └── input_processing/  # Input to game state tests
└── manual/                # Manual playtesting checklists
    ├── classical_levels.md
    ├── star_levels.md
    └── key_levels.md

data/                      # Runtime data directory (not in src/)
├── settings/              # User settings
│   └── game-settings.ini
├── progress/              # Player progress
│   └── player-progress.ini
├── custom_levels/         # User-created levels
└── logs/                  # Error logs (error-<timestamp>.log)

CMakeLists.txt             # CMake build configuration
LICENSE                    # AGPL-3.0 license text
README.md                  # Project documentation
THIRD-PARTY-NOTICES.md     # Third-party license information
```

**Structure Decision**: Single native desktop application with clear separation between platform-agnostic game logic (`src/core/`) and platform-specific implementation (`src/sdl/`, `src/platform/`). This enables future Android porting by replacing SDL2 implementation with Android-specific platform layer while reusing all core game logic. UI screens are modular to support incremental development according to user story priorities (P1-P8).

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

No constitutional violations requiring justification. All principles are satisfied or have minor clarifications that will be resolved during Phase 0 research.

---

## Post-Design Constitution Check

*Re-evaluated after Phase 1 design (research, data model, contracts) completed.*

### I. Cross-Platform Foundation
**Status**: ✅ PASS

- Platform abstraction layer fully defined via interfaces (IRenderer, IAudioPlayer, IInputHandler, IFileIO)
- Core game logic in `src/core/` isolated from platform-specific code in `src/sdl/`
- SDL2 implementation can be swapped for Android NDK implementation without touching core logic
- All game systems use interfaces, not concrete SDL2 types

### II. Legal Compliance
**Status**: ✅ PASS

- Copyright headers specified in quickstart guide and code examples
- All third-party dependencies verified for AGPL-3.0 compatibility (see research.md)
- LICENSE file planned in repository root
- THIRD-PARTY-NOTICES.md planned for dependency attribution

### III. Brick Breaker Genre Fidelity
**Status**: ✅ PASS

- All three level types fully specified (Classical, ReachForStars, GetTheKey)
- Variable-width brick system designed (1-40 units)
- Ball physics with collision detection defined in IPhysicsEngine contract
- Paddle control abstracted via IInputHandler for mouse/keyboard/controller
- Score tracking and progression in GameState and PlayerProgress entities

### IV. Performance & Responsiveness
**Status**: ✅ PASS

- 60 FPS target enforced in IRenderer contract and game loop design (fixed timestep)
- Input latency <16ms enforced in IInputHandler contract
- Performance requirements specified in API contracts (e.g., collision detection <1ms for 100 bricks)
- Memory efficiency considered: object pooling, pre-allocation strategies in quickstart guide

### V. Testing & Quality Assurance
**Status**: ✅ PASS (Clarifications Resolved)

- Testing framework selected: **Catch2 v3.x** (see research.md)
- Test structure defined in quickstart guide (unit, integration, manual)
- API contracts specify testability requirements for each system
- Performance profiling tools documented (gprof, perf, Valgrind)

**Overall Status**: ✅ FULLY COMPLIANT - Ready for Phase 2 (tasks generation)

---

## Phase 0-1 Artifacts Generated

### Phase 0: Research
- ✅ `research.md` - Technology decisions for INI parsing, Markdown parsing, testing framework, SDL2 best practices

### Phase 1: Design
- ✅ `data-model.md` - Full entity definitions with validation rules and state transitions
- ✅ `contracts/game-systems-api.md` - API contracts for 8 major subsystems (Physics, Level, GameState, Persistence, Input, Rendering, Audio, Achievements)
- ✅ `quickstart.md` - Developer onboarding guide with setup, architecture, code examples, debugging tips
- ✅ `CLAUDE.md` - Updated agent context with C++23 and file-based persistence

**Next Phase**: Run `/speckit.tasks` to generate prioritized implementation tasks from this plan.

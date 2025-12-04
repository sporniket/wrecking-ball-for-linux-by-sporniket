# Research & Technology Decisions

**Feature**: Sporniket's Wrecking Ball - Brick Breaker Game
**Date**: 2025-12-04
**Status**: Phase 0 - Research Complete

## Overview

This document resolves all "NEEDS CLARIFICATION" items identified in the Technical Context section of the implementation plan. Research focuses on selecting appropriate libraries for INI parsing, Markdown parsing, and unit testing that meet the project's requirements for C++23, security (malformed input handling), and AGPL-3.0 license compatibility.

---

## Decision 1: INI File Parsing Library

### Decision
Use **inih** (inih-r56 or later)

### Rationale
- **Lightweight**: Single-file header implementation (~500 lines), minimal dependencies
- **C++23 Compatible**: Pure C library with C++ wrapper, works with modern C++ compilers
- **Security**: Simple parser design makes it easy to add bounds checking and validation on top
- **License**: BSD 3-Clause license (AGPL-compatible, permissive)
- **Proven**: Widely used in embedded systems and games, stable API
- **Easy integration**: Can be vendored or linked, no complex build system requirements

### Alternatives Considered

| Alternative | Pros | Cons | Rejected Because |
|-------------|------|------|------------------|
| boost::property_tree | Full-featured, well-tested | Heavy dependency (entire Boost library), complex API | Overkill for simple INI files; adds significant binary size |
| Custom parser | Full control, no dependencies | More development time, potential for bugs | inih is proven, lightweight, and meets all needs without reinventing the wheel |
| iniparser (ndevilla) | Simple C library | Less maintained, fewer modern C++ features | inih has better modern C++ wrapper and more active development |

### Implementation Notes
- Use the C++ wrapper (`INIReader` class) for cleaner integration
- Add validation layer on top of inih to enforce:
  - Key/value length limits (e.g., max 256 characters per key, 1024 per value)
  - Numeric value bounds checking (e.g., volume 0-100, lives 1-99)
  - Whitelist of expected keys to reject unknown/malicious keys
- Implement immediate abort on parse errors with diagnostic logging to stderr and error log file

---

## Decision 2: Markdown Parsing Library for Level Files

### Decision
Use **custom lightweight parser** specifically tailored for level file format

### Rationale
- **Simplicity**: Level files have a well-defined, simple structure (metadata + fenced code block with brick encoding)
- **Security**: Full control over parsing logic allows precise bounds checking and validation
- **Performance**: No overhead from parsing full Markdown features (headers, lists, links, etc.) that aren't used
- **No dependencies**: Reduces external dependencies and potential license conflicts
- **Exact needs**: Only need to:
  - Parse front matter (level metadata: name, type, author)
  - Extract fenced code block contents (```text or ```brick-layout)
  - Parse brick encoding lines (character + dashes for width)

### Alternatives Considered

| Alternative | Pros | Cons | Rejected Because |
|-------------|------|------|------------------|
| cmark-gfm (GitHub's official GFM parser) | Full GFM support, well-tested | C library, complex for simple needs, overkill | Full Markdown parser is excessive; level files only use minimal subset |
| md4c | Fast, lightweight, single-file | Still parses full Markdown spec | Same as cmark-gfm - more complexity than needed |
| Boost.Spirit for custom parser | Powerful parser combinator library | Heavy Boost dependency, steep learning curve | Custom regex + string parsing is sufficient for simple line-based format |

### Implementation Notes
Custom parser will:
1. **Read file line-by-line** with size limit (e.g., max 10KB per level file, max 100 lines)
2. **Parse YAML-style front matter** (lines before `---` delimiter):
   - `name: Level Name` (max 64 chars)
   - `type: classical|star|key` (validate enum)
   - `author: Author Name` (max 64 chars)
3. **Extract fenced code block**:
   - Find ` ```text` or ` ```brick-layout` marker
   - Read lines until closing ` ``` `
   - Validate line count matches play field height (23 lines)
4. **Parse brick encoding per line**:
   - Each line = 38 units width (play field width)
   - Format: `<brick_code><dashes>` where brick_code is single char/digit
   - Brick codes: `.` (empty), `1-9` (regular brick with N hits), `*` (star), `k` (key), `x` (exit)
   - Dashes `-` extend brick width (e.g., `2---` = 4-unit wide brick needing 2 hits)
5. **Validation**:
   - Total width per line must equal 38 units
   - All brick codes must be in whitelist
   - Level type must have required bricks (star levels → at least one `*`, key levels → at least one `k` and one `x`)
6. **Error handling**: Abort immediately on any format violation, log specific error (line number, issue)

---

## Decision 3: Unit Testing Framework

### Decision
Use **Catch2 v3.x**

### Rationale
- **Modern C++ design**: Header-only option, works seamlessly with C++23
- **Expressive syntax**: BDD-style `TEST_CASE` and `SECTION` macros for readable tests
- **Single-header option**: Can be integrated as single `catch_amalgamated.hpp` for simple builds
- **License**: Boost Software License 1.0 (AGPL-compatible, permissive)
- **Active development**: Well-maintained, large community, excellent documentation
- **CMake integration**: First-class CMake support with `catch_discover_tests()`

### Alternatives Considered

| Alternative | Pros | Cons | Rejected Because |
|-------------|------|------|------------------|
| Google Test | Industry standard, mature | More verbose syntax, heavier setup | Catch2 is lighter and more modern C++-friendly |
| doctest | Fastest compile times, header-only | Smaller community than Catch2 | Catch2 has better documentation and ecosystem |
| Boost.Test | Part of Boost ecosystem | Requires Boost dependency | We're avoiding heavy Boost usage; Catch2 is standalone |

### Implementation Notes
- Use CMake integration: `find_package(Catch2 3 REQUIRED)` or FetchContent
- Organize tests by module:
  - `tests/unit/physics/collision_test.cpp` - collision detection for variable-width bricks
  - `tests/unit/level/completion_test.cpp` - level completion logic for all three types
  - `tests/unit/persistence/ini_parser_test.cpp` - INI parsing with malformed inputs
  - `tests/unit/persistence/level_parser_test.cpp` - Markdown level parsing with malformed inputs
- Use Catch2's `SECTION` for organizing test cases by scenario
- Example test structure:
  ```cpp
  TEST_CASE("Level completion detection - Reach for the Stars", "[level][stars]") {
      SECTION("Completes when all star bricks destroyed") {
          // Test star level completion logic
      }
      SECTION("Does not complete when regular bricks remain") {
          // Test that non-star bricks don't prevent completion
      }
  }
  ```

---

## Decision 4: SDL2 Best Practices for 60 FPS Performance

### Research Findings

#### Frame Timing Strategy
- **Fixed time step with accumulator** for physics updates
- **Variable rendering** for smooth visuals
- Implementation pattern:
  ```cpp
  const double dt = 1.0 / 60.0;  // 60 FPS target
  double accumulator = 0.0;

  while (running) {
      double frame_start = SDL_GetPerformanceCounter();

      // Accumulate frame time
      accumulator += frame_time;

      // Fixed time step physics updates
      while (accumulator >= dt) {
          update_game_state(dt);
          accumulator -= dt;
      }

      // Render with interpolation for smooth visuals
      render(accumulator / dt);

      // Cap frame rate if needed
      frame_time = (SDL_GetPerformanceCounter() - frame_start) / frequency;
  }
  ```

#### Rendering Optimization
- **Use SDL_Texture for all sprites**: Avoid SDL_Surface after initial loading
- **Batch rendering**: Group draw calls by texture to minimize state changes
- **Render to texture for static elements**: HUD, brick layouts can be pre-rendered
- **Double buffering**: Use `SDL_RENDERER_PRESENTVSYNC` for tear-free rendering at 60Hz

#### Input Handling
- **Poll events once per frame** at start of game loop
- **Use SDL_GetMouseState() and SDL_GetKeyboardState()** for responsive paddle control
- **Avoid blocking waits**: `SDL_PollEvent()` not `SDL_WaitEvent()`

#### Memory Management
- **Pre-allocate game objects**: Avoid allocations during gameplay loop
- **Object pooling for balls/particles**: Reuse objects rather than new/delete
- **Use std::vector::reserve()**: Pre-allocate capacity for collections

---

## Decision 5: Retro Graphics Rendering Strategy

### Research Findings

#### Scaling Strategy
- **Logical resolution**: 320x200 pixels (apparent screen)
- **Render target**: Use `SDL_RenderSetLogicalSize(renderer, 320, 200)`
  - SDL2 automatically scales to actual window size
  - Maintains aspect ratio with letterboxing
  - Hardware-accelerated scaling
- **Integer scaling option**: In settings, allow integer-only scaling (1x, 2x, 3x) for pixel-perfect rendering

#### Pixel Art Rendering
- **Disable texture filtering**: `SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")` for nearest-neighbor scaling
- **Grid alignment**: All sprites aligned to 8x8 pixel grid boundaries
- **Palette enforcement**: All colors must be from the 16-color palette; use lookup table or constexpr array

#### Font Rendering
- **SDL2_ttf with pixel fonts**: Use small TTF fonts (e.g., 8-point bitmap font) for retro aesthetic
- **Alternative**: Pre-render bitmap font atlas for faster text rendering
- **Text caching**: Render static text (labels, menu items) once to textures, reuse

---

## Decision 6: Game Controller Support Best Practices

### Research Findings

#### SDL2 Game Controller API
- **Use SDL_GameController API** not legacy SDL_Joystick
  - Provides standardized button mappings (A/B/X/Y, D-pad, triggers)
  - Works with Xbox, PlayStation, Switch controllers out of box
- **Controller database**: SDL2 includes `gamecontrollerdb.txt` for wide controller support

#### Integration Strategy
- **Hot-plug support**: Detect controllers added/removed during gameplay via `SDL_CONTROLLERDEVICEADDED`/`REMOVED` events
- **Configurable mappings**: Allow users to remap buttons in settings (stored in game-settings.ini)
- **Default mappings for paddle**:
  - Left stick/D-pad left-right → paddle movement
  - A button → launch ball / confirm menu
  - B button → pause / back
  - Start button → pause menu

---

## Summary of Decisions

| Component | Decision | License | Status |
|-----------|----------|---------|--------|
| INI Parsing | inih (r56+) | BSD 3-Clause | ✅ Selected |
| Markdown Parsing | Custom parser | N/A (own code) | ✅ Selected |
| Unit Testing | Catch2 v3.x | Boost License 1.0 | ✅ Selected |
| Frame Timing | Fixed time step + accumulator | N/A (pattern) | ✅ Documented |
| Rendering | SDL logical size + nearest-neighbor | N/A (SDL2 built-in) | ✅ Documented |
| Controllers | SDL_GameController API | N/A (SDL2 built-in) | ✅ Documented |

## License Compatibility Verification

All selected third-party components are compatible with AGPL-3.0-or-later:

- **SDL2**: zlib license (permissive, AGPL-compatible)
- **SDL2_ttf**: zlib license (permissive, AGPL-compatible)
- **SDL2_mixer**: zlib license (permissive, AGPL-compatible)
- **inih**: BSD 3-Clause (permissive, AGPL-compatible)
- **Catch2**: Boost License 1.0 (permissive, AGPL-compatible)

No GPL-incompatible licenses detected. All dependencies will be documented in `THIRD-PARTY-NOTICES.md` with full license texts and copyright notices.

---

## Next Steps

All "NEEDS CLARIFICATION" items from Technical Context are now resolved. Proceed to Phase 1 to:
1. Generate data model (`data-model.md`)
2. Define API contracts (`contracts/`)
3. Create quickstart guide (`quickstart.md`)
4. Update agent context with selected technologies

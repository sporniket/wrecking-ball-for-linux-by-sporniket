# Game Systems API Contracts

**Feature**: Sporniket's Wrecking Ball - Brick Breaker Game
**Date**: 2025-12-04
**Purpose**: Define internal API contracts between game systems

## Overview

This document defines the interfaces (contracts) between major game systems. These are C++ class interfaces that establish clear boundaries between subsystems, enabling modularity, testability, and future platform porting.

---

## 1. Physics System API

### IPhysicsEngine

**Purpose**: Handle collision detection and ball physics calculations.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

class IPhysicsEngine {
public:
    virtual ~IPhysicsEngine() = default;

    // Update ball positions and velocities based on dt
    virtual void UpdateBalls(std::vector<Ball>& balls, float dt) = 0;

    // Check collision between ball and paddle, update ball velocity
    // Returns: true if collision occurred
    virtual bool CheckPaddleCollision(Ball& ball, const Paddle& paddle) = 0;

    // Check collision between ball and brick, update ball velocity and brick state
    // Returns: pointer to collided brick, or nullptr if no collision
    virtual Brick* CheckBrickCollision(Ball& ball, std::vector<Brick>& bricks) = 0;

    // Check collision with screen boundaries (walls, ceiling)
    virtual void CheckWallCollisions(Ball& ball, const SDL_Rect& play_field) = 0;

    // Check if ball fell below paddle (lost)
    // Returns: true if ball is below paddle Y position
    virtual bool IsBallLost(const Ball& ball, float paddle_y) const = 0;

    // Calculate bounce angle based on paddle impact position
    // paddle_center_x: X position of paddle center
    // ball_x: X position of ball at impact
    // Returns: new velocity vector (normalized direction * speed)
    virtual std::pair<float, float> CalculatePaddleBounce(
        float paddle_center_x,
        float ball_x,
        float ball_speed
    ) const = 0;
};
```

**Contract Requirements**:
- All methods must be deterministic (same input → same output)
- `UpdateBalls()` must preserve ball speed magnitude (elastic collisions)
- Collision detection must work accurately for variable-width bricks (1-40 units)
- Performance: All collision checks must complete within 1ms for 100 bricks + 1 ball

---

## 2. Level System API

### ILevelLoader

**Purpose**: Load and validate level data from files.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

class ILevelLoader {
public:
    virtual ~ILevelLoader() = default;

    // Load level from Markdown file
    // Returns: Level object, or throws std::runtime_error on parse failure
    // Error handling: Logs to stderr and error-<timestamp>.log, then aborts
    virtual Level LoadLevel(const std::filesystem::path& level_file) = 0;

    // Validate level data (check brick types match level type, bounds, etc.)
    // Returns: true if valid, false otherwise (with error details in out_error)
    virtual bool ValidateLevel(const Level& level, std::string& out_error) const = 0;

    // Save level to Markdown file (for level editor)
    // Returns: true on success, false on failure
    virtual bool SaveLevel(const Level& level, const std::filesystem::path& output_file) = 0;

    // Get list of all available level files (built-in + custom)
    virtual std::vector<std::filesystem::path> GetAvailableLevels() const = 0;
};
```

**Contract Requirements**:
- `LoadLevel()` must validate file size (max 10KB) and line count (max 100 lines)
- Must enforce brick encoding validation (valid brick codes, correct total width per line)
- Level type validation: Classical has any bricks, ReachForStars has ≥1 star, GetTheKey has ≥1 key and ≥1 exit
- Parse errors must abort immediately with diagnostic logging (line number, specific issue)
- Custom levels stored in `data/custom_levels/`, built-in in `assets/levels/`

---

### ILevelCompletionChecker

**Purpose**: Determine when a level is completed based on level type.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

class ILevelCompletionChecker {
public:
    virtual ~ILevelCompletionChecker() = default;

    // Check if level completion condition is met
    // Returns: true if level is complete, false otherwise
    virtual bool IsLevelComplete(const Level& level, const GameState& state) const = 0;

    // Get completion progress (e.g., "5/10 star bricks destroyed")
    // Returns: progress string for display in HUD
    virtual std::string GetCompletionProgress(const Level& level, const GameState& state) const = 0;

    // Activate exit bricks when all keys collected (GetTheKey levels only)
    virtual void UpdateExitBrickActivation(Level& level, const GameState& state) = 0;
};
```

**Contract Requirements**:
- Classical levels: Complete when all breakable bricks (Regular, Star, Key, Exit) destroyed
- ReachForStars levels: Complete when all Star bricks destroyed (ignore others)
- GetTheKey levels: Complete when all Key bricks destroyed AND any Exit brick destroyed
- Exit brick activation logic: All Key bricks destroyed → set `is_active=true` on all Exit bricks
- Must be called every frame during gameplay to update exit brick states

---

## 3. Game State Management API

### IGameStateManager

**Purpose**: Manage game state transitions and gameplay flow.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

enum class GameScreen {
    MainMenu,
    LevelSelect,
    GameplayCasual,
    GameplayArcade,
    Trophies,
    Settings,
    LevelEditor,
    OtherGames,
    Pause,
    LevelComplete,
    GameOver
};

class IGameStateManager {
public:
    virtual ~IGameStateManager() = default;

    // Initialize game state for new level
    virtual void StartLevel(uint16_t level_id, GameMode mode) = 0;

    // Update game state each frame (dt = delta time in seconds)
    virtual void Update(float dt) = 0;

    // Handle ball lost event
    // Casual mode: respawn ball
    // Arcade mode: deduct life, respawn ball or trigger game over
    virtual void OnBallLost() = 0;

    // Handle brick destroyed event (update score, stats, check completion)
    virtual void OnBrickDestroyed(const Brick& brick) = 0;

    // Handle level completion (unlock next level, show completion screen)
    virtual void OnLevelComplete() = 0;

    // Pause/unpause gameplay
    virtual void TogglePause() = 0;

    // Get current game screen
    virtual GameScreen GetCurrentScreen() const = 0;

    // Transition to different screen
    virtual void SetScreen(GameScreen screen) = 0;

    // Get current game state (read-only access)
    virtual const GameState& GetState() const = 0;

    // Get mutable game state (for internal systems only)
    virtual GameState& GetMutableState() = 0;
};
```

**Contract Requirements**:
- `Update()` must call physics engine, level completion checker, and achievement tracker
- `OnBallLost()` in Arcade mode: decrement lives, check for game over
- `OnBrickDestroyed()` must update score based on brick type and hits required
- `OnLevelComplete()` must unlock next level, save progress, show completion screen
- State transitions must be atomic (no partial state updates)

---

## 4. Persistence System API

### ISettingsManager

**Purpose**: Load, save, and manage game settings.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

class ISettingsManager {
public:
    virtual ~ISettingsManager() = default;

    // Load settings from INI file
    // Returns: true on success, false on failure (uses defaults)
    // File: data/settings/game-settings.ini
    virtual bool LoadSettings() = 0;

    // Save current settings to INI file
    // Returns: true on success, false on failure
    virtual bool SaveSettings() const = 0;

    // Get current settings (read-only)
    virtual const GameSettings& GetSettings() const = 0;

    // Update settings (e.g., from settings screen UI)
    virtual void UpdateSettings(const GameSettings& new_settings) = 0;

    // Apply settings to game systems (e.g., update audio volumes, input method)
    virtual void ApplySettings() = 0;
};
```

**Contract Requirements**:
- `LoadSettings()` must validate all values (volume 0-100, valid enum values, etc.)
- If INI file doesn't exist, create with default values
- Malformed INI data → log error, use defaults, continue (don't abort for settings)
- `ApplySettings()` must immediately update audio volumes, input method, display mode

---

### IProgressManager

**Purpose**: Load, save, and manage player progress.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

class IProgressManager {
public:
    virtual ~IProgressManager() = default;

    // Load progress from INI file
    // Returns: true on success, false on failure (creates new progress)
    // File: data/progress/player-progress.ini
    virtual bool LoadProgress() = 0;

    // Save current progress to INI file
    // Returns: true on success, false on failure
    virtual bool SaveProgress() const = 0;

    // Get current progress (read-only)
    virtual const PlayerProgress& GetProgress() const = 0;

    // Unlock level (add to unlocked_levels set)
    virtual void UnlockLevel(uint16_t level_id) = 0;

    // Mark level as completed, update statistics
    virtual void MarkLevelComplete(uint16_t level_id, LevelType type, uint32_t score) = 0;

    // Update statistics (bricks destroyed, keys collected, etc.)
    virtual void UpdateStats(uint32_t bricks_destroyed, uint32_t stars, uint32_t keys) = 0;

    // Award achievement if earned
    virtual void CheckAndAwardAchievements() = 0;

    // Check if level is unlocked
    virtual bool IsLevelUnlocked(uint16_t level_id) const = 0;
};
```

**Contract Requirements**:
- `LoadProgress()` must validate all values (level IDs, counts, achievement IDs)
- Level 1 must always be unlocked (add to set if missing)
- Statistics must be monotonic (only increase, never decrease)
- `SaveProgress()` must be called after every level completion to persist state
- File corruption → log error, create new progress file with defaults

---

## 5. Input System API

### IInputHandler

**Purpose**: Abstract input handling for mouse, keyboard, and controller.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

struct InputState {
    float paddle_movement;     // -1.0 (left) to +1.0 (right), 0.0 = no movement
    bool launch_ball;          // true if launch button pressed this frame
    bool pause_toggle;         // true if pause button pressed this frame
    bool confirm;              // true if confirm button pressed (menu navigation)
    bool back;                 // true if back button pressed (menu navigation)
    SDL_Point mouse_position;  // Current mouse position (for menus)
};

class IInputHandler {
public:
    virtual ~IInputHandler() = default;

    // Poll input devices and update input state
    virtual void PollInput() = 0;

    // Get current input state
    virtual const InputState& GetInputState() const = 0;

    // Set input method (mouse, keyboard, controller)
    virtual void SetInputMethod(InputMethod method) = 0;

    // Get current input method
    virtual InputMethod GetInputMethod() const = 0;

    // Update key bindings from settings
    virtual void UpdateKeyBindings(const GameSettings& settings) = 0;

    // Check if quit event occurred (window close, Alt+F4, etc.)
    virtual bool IsQuitRequested() const = 0;
};
```

**Contract Requirements**:
- `PollInput()` must call `SDL_PollEvent()` to process all events
- Mouse input: track mouse X position, convert to paddle position
- Keyboard input: track left/right keys, convert to paddle_movement (-1.0 or +1.0)
- Controller input: read left stick X axis, apply deadzone, convert to paddle_movement
- Input latency must be <16ms (one frame at 60 FPS)
- Handle controller hot-plug events (add/remove during gameplay)

---

## 6. Rendering System API

### IRenderer

**Purpose**: Abstract rendering interface for platform portability.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

struct Color {
    uint8_t r, g, b, a;
};

class IRenderer {
public:
    virtual ~IRenderer() = default;

    // Begin frame rendering (clear screen)
    virtual void BeginFrame() = 0;

    // End frame rendering (present to screen)
    virtual void EndFrame() = 0;

    // Render filled rectangle
    virtual void DrawFilledRect(const SDL_Rect& rect, Color color) = 0;

    // Render brick (with appropriate color based on type and hits remaining)
    virtual void DrawBrick(const Brick& brick) = 0;

    // Render ball
    virtual void DrawBall(const Ball& ball, Color color) = 0;

    // Render paddle
    virtual void DrawPaddle(const Paddle& paddle, Color color) = 0;

    // Render text (HUD, menus)
    virtual void DrawText(const std::string& text, int x, int y, Color color) = 0;

    // Set logical rendering size (for retro 320x200 resolution)
    virtual void SetLogicalSize(int width, int height) = 0;

    // Get logical rendering size
    virtual std::pair<int, int> GetLogicalSize() const = 0;

    // Convert logical coordinates to screen coordinates (for mouse input)
    virtual SDL_Point LogicalToScreen(const SDL_Point& logical) const = 0;

    // Convert screen coordinates to logical coordinates
    virtual SDL_Point ScreenToLogical(const SDL_Point& screen) const = 0;
};
```

**Contract Requirements**:
- Must maintain 60 FPS during rendering of full screen (all bricks + ball + paddle + HUD)
- Logical size set to 320x200, scaled to actual window size
- Nearest-neighbor filtering for pixel-perfect scaling (no blur)
- All colors must use the 16-color palette defined in Visual Specifications
- Text rendering must use retro-style bitmap font (8-point or smaller)

---

## 7. Audio System API

### IAudioPlayer

**Purpose**: Abstract audio playback for music and sound effects.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

enum class SoundEffect {
    BrickHit,
    BrickDestroyed,
    PaddleBounce,
    WallBounce,
    BallLost,
    LevelComplete,
    MenuSelect,
    MenuConfirm,
    KeyCollected,
    ExitActivated
};

class IAudioPlayer {
public:
    virtual ~IAudioPlayer() = default;

    // Initialize audio system
    // Returns: true on success, false on failure (game continues silently)
    virtual bool Initialize() = 0;

    // Shutdown audio system
    virtual void Shutdown() = 0;

    // Play background music (loops)
    virtual void PlayMusic(const std::string& music_file) = 0;

    // Stop background music
    virtual void StopMusic() = 0;

    // Play sound effect (one-shot)
    virtual void PlaySoundEffect(SoundEffect effect) = 0;

    // Set master volume (0-100)
    virtual void SetMasterVolume(uint8_t volume) = 0;

    // Set music volume (0-100)
    virtual void SetMusicVolume(uint8_t volume) = 0;

    // Set sound effects volume (0-100)
    virtual void SetSFXVolume(uint8_t volume) = 0;

    // Check if audio device is available
    virtual bool IsAudioAvailable() const = 0;
};
```

**Contract Requirements**:
- If no audio device available, `Initialize()` returns false, game continues silently
- Volume changes must be applied immediately (audible within 100ms)
- Music must loop seamlessly (no gaps between loops)
- Sound effects must not block game logic (play asynchronously)
- Maximum 8 simultaneous sound effects (mix/limit to prevent audio glitches)

---

## 8. Achievement System API

### IAchievementTracker

**Purpose**: Track and award achievements based on player progress.

```cpp
// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

class IAchievementTracker {
public:
    virtual ~IAchievementTracker() = default;

    // Check all achievements and award newly earned ones
    // Returns: list of newly earned achievement IDs
    virtual std::vector<std::string> CheckAchievements(const PlayerProgress& progress) = 0;

    // Get all available achievements (for trophies screen)
    virtual std::vector<Achievement> GetAllAchievements() const = 0;

    // Get earned achievements
    virtual std::vector<Achievement> GetEarnedAchievements(const PlayerProgress& progress) const = 0;

    // Display achievement notification (UI overlay)
    virtual void ShowAchievementNotification(const Achievement& achievement) = 0;
};
```

**Contract Requirements**:
- `CheckAchievements()` called after level completion, stats update
- Achievement notification displayed for 3 seconds, then fades out
- Multiple achievements earned simultaneously → queue notifications, display sequentially
- Achievement criteria evaluated against `PlayerProgress` snapshot
- Example achievements:
  - "First Victory": Complete 1 level
  - "Star Collector": Complete 5 ReachForStars levels
  - "Key Master": Complete 10 GetTheKey levels
  - "High Scorer": Achieve score ≥ 100,000

---

## System Interaction Flow

```text
Main Game Loop (60 FPS):
┌─────────────────────────────────────────────┐
│ 1. IInputHandler::PollInput()              │
│    → Get InputState (paddle movement, etc.)│
└────────────────┬────────────────────────────┘
                 │
                 ↓
┌─────────────────────────────────────────────┐
│ 2. IGameStateManager::Update(dt)           │
│    ├─ Update paddle position from input    │
│    ├─ IPhysicsEngine::UpdateBalls(dt)      │
│    ├─ Check collisions (paddle, bricks)    │
│    ├─ ILevelCompletionChecker::Check()     │
│    └─ IAchievementTracker::CheckAchievements() │
└────────────────┬────────────────────────────┘
                 │
                 ↓
┌─────────────────────────────────────────────┐
│ 3. IRenderer::BeginFrame()                 │
│    ├─ Draw play field background           │
│    ├─ Draw all bricks (variable widths)    │
│    ├─ Draw paddle                           │
│    ├─ Draw balls                            │
│    ├─ Draw HUD (score, lives, progress)    │
│    └─ IRenderer::EndFrame()                │
└────────────────┬────────────────────────────┘
                 │
                 ↓
┌─────────────────────────────────────────────┐
│ 4. IAudioPlayer::PlaySoundEffect()        │
│    (on collision events)                    │
└─────────────────────────────────────────────┘
```

---

## Error Handling Contract

All systems must follow these error handling rules:

1. **File I/O Errors** (level files, INI files):
   - Log error to stderr
   - Log error to `data/logs/error-<timestamp>.log` with millisecond timestamp
   - For level files: abort level load, show error to user, return to menu
   - For settings/progress: use defaults, continue (non-fatal)

2. **Validation Errors** (malformed data):
   - Abort immediately on validation failure (don't try to "fix" data)
   - Log specific error with context (line number, expected vs actual)
   - For levels: refuse to load, mark as invalid
   - For INI: use defaults for invalid values, log warning

3. **System Initialization Errors** (audio, graphics):
   - Log error, continue if possible (e.g., no audio → silent mode)
   - Graphics failure → fatal error, cannot continue

4. **Resource Loading Errors** (missing fonts, assets):
   - Log error with missing resource path
   - Use fallback resource if available (e.g., default font)
   - If no fallback → fatal error for critical resources

---

## Testing Contracts

Each system interface must have:

1. **Unit Tests**: Test each method in isolation with mocks
   - Example: `IPhysicsEngine::CheckBrickCollision()` with various brick widths
   - Example: `ILevelLoader::ValidateLevel()` with malformed Markdown

2. **Integration Tests**: Test system interactions
   - Example: Ball collision → brick destroyed → score updated → achievement earned
   - Example: Load level → start gameplay → complete level → unlock next → save progress

3. **Performance Tests**: Verify performance requirements
   - Example: `IPhysicsEngine::Update()` with 100 bricks + 1 ball < 1ms
   - Example: `IRenderer::DrawBrick()` for 874 bricks (full screen) at 60 FPS

---

## Next Steps

API contracts are complete. Proceed to:
1. Create quickstart guide for developers
2. Update agent context with API contract information
3. Begin implementation (Phase 2 - tasks generation)

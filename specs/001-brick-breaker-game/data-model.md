# Data Model

**Feature**: Sporniket's Wrecking Ball - Brick Breaker Game
**Date**: 2025-12-04
**Status**: Phase 1 - Design Complete

## Overview

This document defines the core entities, their relationships, validation rules, and state transitions for the brick breaker game. All entities are designed to be platform-agnostic and serializable to INI or Markdown formats.

---

## Entity Definitions

### 1. Brick

Represents a destructible block in the game with position, type, dimensions, and durability.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `position_x` | `uint8_t` | X coordinate in grid units (0-39) | 0 ≤ x ≤ 39 |
| `position_y` | `uint8_t` | Y coordinate in grid units (0-24) | 0 ≤ y ≤ 24 |
| `width` | `uint8_t` | Width in grid units | 1 ≤ width ≤ 40, x + width ≤ 40 |
| `height` | `uint8_t` | Height in grid units (fixed) | height = 1 (fixed for all bricks) |
| `type` | `enum BrickType` | Brick type (regular, star, key, exit) | Must be valid enum value |
| `hits_required` | `uint8_t` | Number of hits needed to destroy | 1 ≤ hits ≤ 9 |
| `hits_remaining` | `uint8_t` | Current remaining hits | 0 ≤ remaining ≤ hits_required |
| `color_index` | `uint8_t` | Index into 16-color palette | 0 ≤ index < 16 |
| `is_active` | `bool` | Whether brick can be destroyed | Exit bricks: false until keys collected |
| `is_destroyed` | `bool` | Whether brick has been destroyed | Set to true when hits_remaining = 0 |

**Brick Types (enum)**:
```cpp
enum class BrickType : uint8_t {
    Empty = 0,      // No brick (.)
    Regular = 1,    // Standard breakable brick (1-9 for hits)
    Star = 2,       // Star-shaped brick (*)
    Key = 3,        // Key-shaped brick (k)
    Exit = 4        // Exit brick (x)
};
```

**Validation Rules**:
- `position_x + width ≤ 40` (must fit within grid width)
- `position_y ≥ 1` and `≤ 24` (not in HUD area, within grid)
- Star/Key/Exit bricks: `hits_required = 1` (single-hit destruction)
- Regular bricks: `1 ≤ hits_required ≤ 9`
- Exit bricks: `is_active = false` until all keys in level destroyed
- `hits_remaining` never exceeds `hits_required`

**State Transitions**:
```text
Created → Active (is_active=true if not exit brick, or keys collected)
Active → Damaged (hits_remaining decreases on collision)
Damaged → Destroyed (hits_remaining = 0, is_destroyed=true)

Exit brick special:
Created → Inactive (is_active=false)
Inactive → Activated (is_active=true when all keys destroyed)
Activated → Destroyed (on collision)
```

---

### 2. Ball

Represents a game ball with position, velocity, and state.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `position_x` | `float` | X position in pixels (apparent resolution) | 0.0 ≤ x ≤ 320.0 |
| `position_y` | `float` | Y position in pixels (apparent resolution) | 0.0 ≤ y ≤ 200.0 |
| `velocity_x` | `float` | Horizontal velocity (pixels/second) | -500.0 ≤ vx ≤ 500.0 |
| `velocity_y` | `float` | Vertical velocity (pixels/second) | -500.0 ≤ vy ≤ 500.0 |
| `radius` | `float` | Ball radius in pixels | radius = 3.0 (fixed) |
| `is_active` | `bool` | Whether ball is in play | true=moving, false=attached to paddle |
| `is_lost` | `bool` | Whether ball fell below paddle | true when y > 200 |

**Validation Rules**:
- Speed (magnitude of velocity vector) should be constant during normal play (e.g., 300 px/s)
- Ball cannot have zero velocity when active (must be moving)
- Position must be within screen bounds during active play (except when lost)

**State Transitions**:
```text
Spawned → Attached to Paddle (is_active=false, follows paddle X position)
Attached → Launched (is_active=true, velocity set based on launch angle)
Launched → Bouncing (velocity changes on collisions)
Bouncing → Lost (y > 200, is_lost=true)
Lost → Respawned (in casual mode) or Life Lost (in arcade mode)
```

**Physics Notes**:
- Bounce angle on paddle collision calculated based on impact position (left/center/right)
- Velocity magnitude preserved after collisions (elastic collisions)
- Collision detection for variable-width bricks must check entire brick surface

---

### 3. Paddle

Represents the player-controlled horizontal bar.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `position_x` | `float` | Center X position in pixels | 0.0 + width/2 ≤ x ≤ 320.0 - width/2 |
| `position_y` | `float` | Y position in pixels (fixed) | y = 180.0 (fixed, near bottom) |
| `width` | `float` | Paddle width in pixels | width = 32.0 (4 units) |
| `height` | `float` | Paddle height in pixels | height = 8.0 (1 unit) |
| `velocity_x` | `float` | Current horizontal velocity | -400.0 ≤ vx ≤ 400.0 px/s |

**Validation Rules**:
- Paddle must stay within screen bounds: `width/2 ≤ position_x ≤ 304 - width/2`
- Y position is fixed (does not move vertically)
- Velocity clamped to max speed (controlled by input sensitivity)

**State Transitions**:
```text
Idle (velocity_x = 0) ↔ Moving Left (velocity_x < 0)
Idle ↔ Moving Right (velocity_x > 0)
Moving ↔ Stopped (velocity_x set to 0 when input released)
```

---

### 4. Level

Represents a playable stage with brick layout, type, and completion state.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `level_id` | `uint16_t` | Unique level identifier | 1 ≤ id ≤ 9999 |
| `name` | `string` | Human-readable level name | Max 64 characters |
| `level_type` | `enum LevelType` | Level completion type | Must be valid enum value |
| `author` | `string` | Level creator (or "Built-in") | Max 64 characters |
| `bricks` | `vector<Brick>` | Collection of bricks in level | 0 ≤ size ≤ 874 (38×23 max) |
| `is_locked` | `bool` | Whether level is unlocked for play | false=can play, true=locked |
| `is_custom` | `bool` | Whether level is user-created | true=custom, false=built-in |
| `difficulty` | `uint8_t` | Difficulty rating (optional) | 1 ≤ difficulty ≤ 5 |

**Level Types (enum)**:
```cpp
enum class LevelType : uint8_t {
    Classical = 0,      // Break all breakable bricks
    ReachForStars = 1,  // Break all star bricks only
    GetTheKey = 2       // Break all key bricks, then break exit brick
};
```

**Validation Rules**:
- Classical levels: Must have at least one regular/star/key/exit brick (non-empty)
- ReachForStars levels: Must have at least one star brick (`BrickType::Star`)
- GetTheKey levels: Must have at least one key brick (`BrickType::Key`) AND at least one exit brick (`BrickType::Exit`)
- All brick positions must be within play field bounds (1 ≤ x ≤ 38, 1 ≤ y ≤ 23, accounting for HUD)
- No overlapping bricks (validate during level load)

**State Transitions**:
```text
Created → Locked (is_locked=true for non-first levels)
Locked → Unlocked (is_locked=false when previous level completed)
Unlocked → In Progress (level started by player)
In Progress → Completed (completion condition met based on level_type)
Completed → Unlocked Next Level (unlock level_id + 1)
```

**Completion Conditions by Type**:
- **Classical**: All bricks with `BrickType::Regular`, `Star`, `Key`, or `Exit` are destroyed
- **ReachForStars**: All bricks with `BrickType::Star` are destroyed (ignore others)
- **GetTheKey**: All bricks with `BrickType::Key` destroyed, then any `BrickType::Exit` brick destroyed

---

### 5. GameState

Represents the current gameplay session state.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `current_level` | `Level*` | Pointer to active level | Non-null during gameplay |
| `game_mode` | `enum GameMode` | Casual or Arcade mode | Must be valid enum value |
| `score` | `uint32_t` | Current score | 0 ≤ score ≤ 4,294,967,295 |
| `lives_remaining` | `uint8_t` | Remaining lives (arcade mode) | 0 ≤ lives ≤ 99 (0=game over) |
| `balls` | `vector<Ball>` | Active balls in play | Casual: unlimited, Arcade: 1 ball per life |
| `paddle` | `Paddle` | Player paddle | Always present |
| `keys_collected` | `uint8_t` | Key bricks destroyed (GetTheKey levels) | 0 ≤ keys ≤ total keys in level |
| `keys_total` | `uint8_t` | Total key bricks in level | Cached for performance |
| `stars_collected` | `uint8_t` | Star bricks destroyed (ReachForStars levels) | 0 ≤ stars ≤ total stars in level |
| `stars_total` | `uint8_t` | Total star bricks in level | Cached for performance |
| `is_paused` | `bool` | Whether game is paused | true=frozen, false=running |
| `level_completed` | `bool` | Whether current level is complete | Set when completion condition met |

**Game Modes (enum)**:
```cpp
enum class GameMode : uint8_t {
    Casual = 0,     // Unlimited balls, level selection
    Arcade = 1      // Limited lives, sequential levels from 1
};
```

**Validation Rules**:
- Arcade mode: `3 ≤ lives_remaining ≤ 7` at start (player choice: 3, 5, or 7)
- Casual mode: `lives_remaining` not used (or set to 255 for "infinite")
- `keys_collected ≤ keys_total` and `stars_collected ≤ stars_total`
- Ball count: Casual can have multiple balls, Arcade typically 1 at a time
- Game over: Arcade mode when `lives_remaining = 0`

**State Transitions**:
```text
Initialized → Level Loaded (current_level set, bricks populated)
Level Loaded → Active Gameplay (ball launched, paddle moving)
Active Gameplay ↔ Paused (is_paused toggle)
Active Gameplay → Ball Lost (ball.is_lost=true)
Ball Lost → Life Deducted (Arcade: lives_remaining--, Casual: respawn ball)
Life Deducted → Game Over (Arcade: lives_remaining=0)
Active Gameplay → Level Complete (level_completed=true when condition met)
Level Complete → Next Level / Return to Menu
```

---

### 6. PlayerProgress

Represents persistent player data across game sessions.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `unlocked_levels` | `set<uint16_t>` | Set of unlocked level IDs | Level 1 always unlocked |
| `completed_levels_classical` | `uint16_t` | Count of Classical levels completed | 0 ≤ count ≤ 9999 |
| `completed_levels_star` | `uint16_t` | Count of ReachForStars levels completed | 0 ≤ count ≤ 9999 |
| `completed_levels_key` | `uint16_t` | Count of GetTheKey levels completed | 0 ≤ count ≤ 9999 |
| `highest_score` | `uint32_t` | All-time highest score | 0 ≤ score ≤ 4,294,967,295 |
| `total_bricks_destroyed` | `uint64_t` | Lifetime brick destruction count | 0 ≤ count ≤ 2^64-1 |
| `total_stars_collected` | `uint32_t` | Lifetime star brick count | 0 ≤ count ≤ 4,294,967,295 |
| `total_keys_collected` | `uint32_t` | Lifetime key brick count | 0 ≤ count ≤ 4,294,967,295 |
| `earned_achievements` | `set<string>` | Set of earned achievement IDs | Achievement ID max 32 chars |

**Validation Rules**:
- Level 1 (`level_id=1`) must always be in `unlocked_levels`
- Sum of completed level counts should not exceed total available levels (but can for custom levels)
- Statistics can only increase, never decrease (monotonic)
- Achievement IDs must match predefined achievement set (no arbitrary strings)

**Persistence Format (INI)**:
```ini
[progress]
unlocked_levels = 1,2,3,4,5
completed_classical = 5
completed_star = 3
completed_key = 2

[stats]
highest_score = 125000
total_bricks = 4523
total_stars = 287
total_keys = 143

[achievements]
earned = first_level_complete,10_levels_complete,star_collector
```

---

### 7. GameSettings

Represents player preferences and configuration.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `master_volume` | `uint8_t` | Master audio volume (0-100) | 0 ≤ volume ≤ 100 |
| `music_volume` | `uint8_t` | Music volume (0-100) | 0 ≤ volume ≤ 100 |
| `sfx_volume` | `uint8_t` | Sound effects volume (0-100) | 0 ≤ volume ≤ 100 |
| `fullscreen` | `bool` | Fullscreen or windowed mode | true=fullscreen, false=windowed |
| `vsync_enabled` | `bool` | Enable VSync for 60Hz | true=vsync on (recommended) |
| `integer_scaling` | `bool` | Integer-only scaling for pixel-perfect | true=1x/2x/3x, false=any scale |
| `input_method` | `enum InputMethod` | Primary input device | Mouse (default), Keyboard, Controller |
| `key_paddle_left` | `SDL_Scancode` | Key for paddle left movement | Valid scancode |
| `key_paddle_right` | `SDL_Scancode` | Key for paddle right movement | Valid scancode |
| `key_launch` | `SDL_Scancode` | Key to launch ball | Valid scancode |
| `key_pause` | `SDL_Scancode` | Key to pause game | Valid scancode |
| `mouse_sensitivity` | `float` | Mouse movement sensitivity | 0.5 ≤ sensitivity ≤ 2.0 |
| `controller_deadzone` | `float` | Analog stick deadzone | 0.1 ≤ deadzone ≤ 0.5 |

**Input Methods (enum)**:
```cpp
enum class InputMethod : uint8_t {
    Mouse = 0,      // Mouse control (default)
    Keyboard = 1,   // Keyboard arrow keys / WASD
    Controller = 2  // Game controller (SDL_GameController)
};
```

**Validation Rules**:
- All volume values must be in range [0, 100]
- Sensitivity/deadzone values must be positive floats within specified ranges
- Key bindings must not conflict (warn user on duplicate mappings)
- Controller must be connected if `input_method = Controller`

**Persistence Format (INI)**:
```ini
[audio]
master_volume = 80
music_volume = 70
sfx_volume = 90

[display]
fullscreen = false
vsync_enabled = true
integer_scaling = true

[input]
input_method = mouse
mouse_sensitivity = 1.0
controller_deadzone = 0.2

[keybindings]
paddle_left = SDL_SCANCODE_LEFT
paddle_right = SDL_SCANCODE_RIGHT
launch = SDL_SCANCODE_SPACE
pause = SDL_SCANCODE_ESCAPE
```

---

### 8. Achievement

Represents a trophy/badge with unlock criteria.

**Fields**:

| Field | Type | Description | Constraints |
|-------|------|-------------|-------------|
| `achievement_id` | `string` | Unique achievement identifier | Max 32 chars, snake_case |
| `name` | `string` | Display name | Max 64 characters |
| `description` | `string` | How to unlock | Max 128 characters |
| `is_earned` | `bool` | Whether player has earned it | false initially |
| `unlock_criteria` | `function pointer` | Function to check if earned | Check against PlayerProgress |

**Example Achievements**:
```cpp
Achievement {
    .achievement_id = "first_level_complete",
    .name = "First Victory",
    .description = "Complete your first level",
    .unlock_criteria = [](const PlayerProgress& p) {
        return (p.completed_levels_classical + p.completed_levels_star + p.completed_levels_key) >= 1;
    }
}

Achievement {
    .achievement_id = "star_collector",
    .name = "Reach for the Stars",
    .description = "Complete 5 Reach for the Stars levels",
    .unlock_criteria = [](const PlayerProgress& p) {
        return p.completed_levels_star >= 5;
    }
}
```

---

## Entity Relationships

```text
Level (1) ──< contains >── (0..*) Brick
      │
      │
      └── loaded into ──> (1) GameState
                              │
                              ├──< contains >── (1) Paddle
                              ├──< contains >── (1..*) Ball
                              └──< references >── (1) Level

PlayerProgress ──< tracks >── (0..*) Level (unlocked)
               │
               └──< has earned >── (0..*) Achievement

GameSettings ──< configures >── (1) GameState (input, display)
```

---

## Serialization Formats

### Level File Format (Markdown)

```markdown
---
name: Classical Level 1
type: classical
author: Built-in
difficulty: 1
---

# Level 1: Getting Started

This is the first level. Break all bricks to win!

```brick-layout
......................................
..2---3---4---5---6---2---3---4---5-..
..1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-1-..
..2---2---2---2---2---2---2---2---2-..
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
......................................
```

**Brick Encoding**:
- `.` = empty space
- `1-9` = regular brick requiring N hits
- `*` = star brick
- `k` = key brick
- `x` = exit brick
- `-` = extend brick width to the right (e.g., `2---` = 4-unit wide brick, 2 hits)
```

### INI File Examples

See GameSettings and PlayerProgress sections above for INI format examples.

---

## Next Steps

Data model is complete. Proceed to:
1. Generate API contracts for internal game systems
2. Create quickstart guide for developers
3. Update agent context with data model information

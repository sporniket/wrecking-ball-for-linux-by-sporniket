# Feature Specification: Sporniket's Wrecking Ball - Complete Game

**Feature Branch**: `001-brick-breaker-game`
**Created**: 2025-12-02
**Status**: Draft
**Input**: User description: "Sporniket's Wrecking Ball is a brick breaker game, for Linux OS with a possibility of a version for Android OS. The application is structured around several screens : a main menu that serves as a hub for the other screens ; a casual game screen that allows to select a level among the unlocked ones and start to play, starting from the selected level, and with unlimited number of balls ; an arcade game screen that allows to play starting by the first level, with a limited amount of balls (3, 5, or 7, to be selected before starting to play) ; an hall of trophies/achievements screen, showing some metrics of the players as badges ; a 'my other games' screen to tease about other games availables ; a configuration screen to adjust player controls, the audio volume, etc... ; a level editor screen, that will allow players to go beyond the builtin levels."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Core Gameplay (Priority: P1)

A player launches the game, navigates to the casual game mode, selects an unlocked level, and plays the classic brick breaker gameplay: controlling a paddle to bounce a ball that destroys bricks. The player experiences smooth, responsive controls and clear visual feedback as bricks break and the score increases. When all bricks are destroyed, the level is completed and the next level is unlocked.

**Why this priority**: This is the minimum viable product - the core brick breaker gameplay that defines the game's identity. Without this, there is no game. All other features depend on having a playable game loop.

**Independent Test**: Can be fully tested by launching the game, starting a level, playing with paddle controls (keyboard/mouse), destroying bricks, and completing the level. Delivers immediate playable value and validates core game mechanics.

**Acceptance Scenarios**:

1. **Given** the game is launched and main menu is displayed, **When** the player selects casual mode and chooses a level, **Then** the game screen loads with paddle, ball(s), and brick layout visible
2. **Given** the player is in an active game, **When** the player moves the paddle left/right using input controls, **Then** the paddle moves smoothly and responsively with input latency below 16ms
3. **Given** the ball is in motion, **When** the ball collides with a brick, **Then** the brick is destroyed, the ball bounces with realistic physics, and the score increases
4. **Given** the ball is in motion, **When** the ball collides with the paddle, **Then** the ball bounces at an angle based on collision point
5. **Given** the ball is in motion, **When** the ball falls below the paddle, **Then** the ball respawns from the paddle (casual mode has unlimited balls)
6. **Given** all bricks in a level are destroyed, **When** the last brick is hit, **Then** the level completion screen appears, next level is unlocked, and player can proceed
7. **Given** the player is in an active game, **When** the player pauses the game, **Then** gameplay freezes and a pause menu appears with options to resume or exit

---

### User Story 2 - Main Menu & Navigation (Priority: P2)

A player launches the game and sees a main menu that serves as a central hub. From this menu, the player can navigate to different game modes (casual, arcade), view their achievements, access settings, browse other games, or use the level editor. The navigation is intuitive and the player can always return to the main menu.

**Why this priority**: Essential for game structure and user experience. Provides access to all game features and modes. Must be implemented early to enable testing of multiple game screens, but core gameplay (P1) is more critical.

**Independent Test**: Can be tested by launching the game, verifying all menu options are visible and functional, navigating to each screen, and returning to the main menu. Delivers complete navigation framework.

**Acceptance Scenarios**:

1. **Given** the game application starts, **When** the game finishes loading, **Then** the main menu screen is displayed with options for: Casual Game, Arcade Game, Trophies/Achievements, My Other Games, Settings, Level Editor, and Exit
2. **Given** the player is on the main menu, **When** the player selects any menu option, **Then** the corresponding screen loads and displays correctly
3. **Given** the player is on any non-menu screen, **When** the player requests to return to main menu (via back/menu button), **Then** the player is returned to the main menu
4. **Given** the player selects Exit from main menu, **When** the exit action is confirmed, **Then** the game closes gracefully

---

### User Story 3 - Arcade Mode (Priority: P3)

A player selects arcade mode from the main menu, chooses a starting ball count (3, 5, or 7), and plays through levels starting from level 1 with limited lives. Each time a ball falls below the paddle, one life is lost. When all lives are exhausted, the game ends and the player's final score and highest level reached are displayed.

**Why this priority**: Adds challenge and replayability by introducing limited lives. Builds on core gameplay (P1) and navigation (P2). Important for player engagement but not essential for initial playability.

**Independent Test**: Can be tested by selecting arcade mode, choosing ball count, playing until losing all lives, and verifying game over sequence. Delivers distinct gameplay mode with score tracking.

**Acceptance Scenarios**:

1. **Given** the player selects arcade mode from main menu, **When** the arcade mode screen loads, **Then** the player is presented with options to select 3, 5, or 7 balls
2. **Given** the player has selected a ball count, **When** the player starts the game, **Then** level 1 begins with the selected number of lives remaining displayed
3. **Given** the player is playing in arcade mode, **When** a ball falls below the paddle, **Then** one life is deducted and the remaining lives count is updated
4. **Given** the player is playing in arcade mode, **When** all lives reach zero, **Then** the game over screen appears showing final score and highest level reached
5. **Given** the player completes a level in arcade mode, **When** all bricks are destroyed, **Then** the next level loads automatically with current lives maintained

---

### User Story 4 - Level Selection & Progress Tracking (Priority: P4)

A player in casual mode sees a visual representation of available levels, including which levels are locked and which are unlocked. The player can select any unlocked level to play. As levels are completed, new levels are automatically unlocked. The player's progress is saved and persists across game sessions.

**Why this priority**: Enhances player experience by providing choice and tracking progress. Depends on core gameplay (P1) being complete. Important for player retention but can be added after basic gameplay works.

**Independent Test**: Can be tested by completing levels to unlock new ones, closing and reopening the game, and verifying progress is saved. Delivers progression system and player choice.

**Acceptance Scenarios**:

1. **Given** the player selects casual mode, **When** the level selection screen loads, **Then** a visual grid/list of levels is displayed showing locked and unlocked states
2. **Given** the player is viewing the level selection screen, **When** the player clicks on an unlocked level, **Then** that level starts immediately
3. **Given** the player is viewing the level selection screen, **When** the player clicks on a locked level, **Then** no action occurs or a message indicates the level is locked
4. **Given** the player completes a level, **When** the level completion occurs, **Then** the next sequential level is automatically unlocked
5. **Given** the player has played and closed the game, **When** the player reopens the game and goes to level selection, **Then** all previously unlocked levels remain unlocked

---

### User Story 5 - Achievements & Trophies (Priority: P5)

A player accesses the "Hall of Trophies" screen from the main menu and views their gameplay achievements displayed as badges or trophies. These include metrics such as total levels completed, highest score achieved, total bricks destroyed, and special accomplishments (e.g., "Complete 10 levels without losing a ball").

**Why this priority**: Adds motivation and rewards player accomplishments. Purely additive feature that enhances engagement but is not required for core gameplay or navigation.

**Independent Test**: Can be tested by playing the game to trigger various achievements, then viewing the trophies screen to verify they are displayed correctly. Delivers player reward system.

**Acceptance Scenarios**:

1. **Given** the player selects "Trophies/Achievements" from the main menu, **When** the trophies screen loads, **Then** a collection of achievement badges is displayed with earned/unearned states visible
2. **Given** the player is viewing the trophies screen, **When** the player views an achievement, **Then** details about the achievement (description, progress, unlock criteria) are shown
3. **Given** the player performs an action that triggers an achievement, **When** the achievement criteria is met, **Then** the achievement is marked as earned and a visual notification appears
4. **Given** the player has earned achievements, **When** the player returns to the trophies screen, **Then** all earned achievements are highlighted or visually distinguished from unearned ones

---

### User Story 6 - Settings & Configuration (Priority: P6)

A player accesses the settings screen from the main menu and configures game preferences including control mappings (keyboard/mouse settings), audio volume levels (master, music, sound effects), and display options. The player's settings choices are saved and applied immediately.

**Why this priority**: Important for player comfort and accessibility, but not blocking for core gameplay. Can be added after gameplay and navigation are solid.

**Independent Test**: Can be tested by accessing settings, changing various options, and verifying changes are applied and persisted. Delivers customization capability.

**Acceptance Scenarios**:

1. **Given** the player selects "Settings" from the main menu, **When** the settings screen loads, **Then** configuration options for controls, audio, and display are visible
2. **Given** the player is on the settings screen, **When** the player adjusts audio volume sliders, **Then** the audio volume changes immediately to reflect the new setting
3. **Given** the player is on the settings screen, **When** the player remaps a control (e.g., change paddle left key from 'A' to arrow key), **Then** the new control mapping is saved and used in gameplay
4. **Given** the player has changed settings, **When** the player exits settings and returns later, **Then** all previously configured settings are retained

---

### User Story 7 - Level Editor (Priority: P7)

A player accesses the level editor from the main menu and creates custom brick layouts by placing bricks in a grid pattern. The player can save custom levels and play them in casual mode. The level editor provides tools to add/remove bricks, select brick types, and test the level.

**Why this priority**: Extends game longevity and player creativity but is not essential for initial release. Most complex feature requiring UI for editing, file management, and validation.

**Independent Test**: Can be tested by creating a custom level in the editor, saving it, then loading and playing it in casual mode. Delivers user-generated content capability.

**Acceptance Scenarios**:

1. **Given** the player selects "Level Editor" from the main menu, **When** the editor screen loads, **Then** an empty grid and editing tools (add brick, remove brick, brick type selector, save, test) are displayed
2. **Given** the player is in the level editor, **When** the player clicks on an empty grid cell, **Then** a brick of the selected type is placed in that cell
3. **Given** the player is in the level editor, **When** the player clicks on an occupied grid cell, **Then** the brick is removed from that cell
4. **Given** the player has created a brick layout, **When** the player selects "Test Level", **Then** the level loads in a test gameplay mode where the player can play it
5. **Given** the player has created a brick layout, **When** the player selects "Save Level", **Then** the level is saved with a name provided by the player
6. **Given** the player has saved custom levels, **When** the player goes to casual mode level selection, **Then** custom levels appear alongside built-in levels

---

### User Story 8 - My Other Games (Priority: P8)

A player accesses the "My Other Games" screen from the main menu and views information about other games by the developer, including game titles, descriptions, and links/information about where to find them.

**Why this priority**: Marketing/promotional feature with no impact on core gameplay. Lowest priority as it provides no gameplay value.

**Independent Test**: Can be tested by accessing the screen and verifying game information is displayed correctly. Delivers promotional content.

**Acceptance Scenarios**:

1. **Given** the player selects "My Other Games" from the main menu, **When** the screen loads, **Then** a list or gallery of other games is displayed with titles and brief descriptions
2. **Given** the player is viewing the other games screen, **When** the player selects a game entry, **Then** additional details about that game are shown (full description, images, availability)

---

### Edge Cases

- What happens when the player closes the game during active gameplay? (Game state should be discarded; player returns to main menu on next launch)
- What happens when the player tries to save a custom level with the same name as an existing level? (System should prompt to overwrite or rename)
- What happens when the ball gets stuck in a corner or unreachable position? (Game should detect stuck ball after timeout and respawn it)
- What happens when the player achieves multiple achievements simultaneously? (All achievements should be awarded and queued notifications should display sequentially)
- What happens when no audio device is available? (Game should run silently without crashing; settings should indicate audio unavailable)
- What happens when the player attempts to remap a control to an already-used key? (System should warn of conflict and allow player to confirm or cancel)
- What happens when the level editor creates an unplayable level (no bricks)? (System should warn player before saving; or allow saving but mark as invalid for gameplay)

## Requirements *(mandatory)*

### Functional Requirements

**Core Gameplay:**

- **FR-001**: System MUST display a game screen with a movable paddle, one or more balls, and a grid of destructible bricks
- **FR-002**: System MUST accept player input via keyboard and mouse to control paddle horizontal movement
- **FR-003**: System MUST implement ball physics with realistic collision detection for paddle, bricks, and walls
- **FR-004**: System MUST calculate ball bounce angles based on collision points (especially paddle contact point)
- **FR-005**: System MUST remove bricks when struck by the ball and update the score
- **FR-006**: System MUST maintain minimum 60 FPS during active gameplay
- **FR-007**: System MUST keep input latency below 16ms

**Game Modes:**

- **FR-008**: System MUST provide a casual game mode with unlimited balls and level selection
- **FR-009**: System MUST provide an arcade game mode with selectable starting lives (3, 5, or 7) and sequential level progression starting from level 1
- **FR-010**: System MUST track lives in arcade mode and end the game when lives reach zero
- **FR-011**: System MUST respawn balls from the paddle position when they fall below the paddle

**Navigation & UI:**

- **FR-012**: System MUST display a main menu on game launch with navigation to all game screens
- **FR-013**: System MUST allow navigation between screens: Main Menu, Casual Game, Arcade Game, Level Selection, Trophies, Settings, Level Editor, My Other Games
- **FR-014**: System MUST provide a way to return to the main menu from any screen
- **FR-015**: System MUST display current score, level number, and remaining lives (arcade mode) during gameplay

**Level System:**

- **FR-016**: System MUST include multiple built-in brick breaker levels with varying brick layouts
- **FR-017**: System MUST track which levels are unlocked in casual mode
- **FR-018**: System MUST unlock the next sequential level when a level is completed
- **FR-019**: System MUST display locked and unlocked levels visually in level selection screen
- **FR-020**: System MUST allow players to select and start any unlocked level in casual mode

**Progression & Persistence:**

- **FR-021**: System MUST save player progress (unlocked levels, settings, achievements) and persist across game sessions
- **FR-022**: System MUST track player statistics (total levels completed, highest score, total bricks destroyed)
- **FR-023**: System MUST detect when achievement criteria are met and award achievements
- **FR-024**: System MUST display earned achievements in the Hall of Trophies screen

**Settings & Configuration:**

- **FR-025**: System MUST allow players to configure control mappings (keyboard keys for paddle movement, pause, etc.)
- **FR-026**: System MUST allow players to adjust audio volume for master, music, and sound effects separately
- **FR-027**: System MUST save player settings and apply them on subsequent game launches
- **FR-028**: System MUST apply setting changes immediately (audio volume changes heard instantly)

**Level Editor:**

- **FR-029**: System MUST provide a level editor interface with a grid for placing/removing bricks
- **FR-030**: System MUST allow players to select brick types when placing bricks in the editor
- **FR-031**: System MUST allow players to test custom levels directly from the editor
- **FR-032**: System MUST allow players to save custom levels with a player-provided name
- **FR-033**: System MUST include custom levels in the casual mode level selection screen
- **FR-034**: System MUST validate custom levels before saving (warn if level has no bricks)

**Other Features:**

- **FR-035**: System MUST display information about other games by the developer in the "My Other Games" screen
- **FR-036**: System MUST provide pause functionality during active gameplay
- **FR-037**: System MUST display level completion screen when all bricks are destroyed
- **FR-038**: System MUST display game over screen in arcade mode when lives are exhausted

**Platform Requirements:**

- **FR-039**: System MUST run as a native Linux desktop application
- **FR-040**: System MUST separate platform-specific code from core game logic to support future Android porting

**Legal & Licensing:**

- **FR-041**: All source code files MUST include copyright notice "Copyright (c) 2025 David SPORN" and SPDX license identifier "AGPL-3.0-or-later"
- **FR-042**: Repository MUST include LICENSE file with complete AGPL-3.0 license text

### Key Entities

- **Level**: Represents a playable stage with a specific brick layout, difficulty, and completion state (locked/unlocked)
- **Brick**: Represents a destructible block with position, type/color, and health/durability
- **Ball**: Represents a game ball with position, velocity vector, and active/inactive state
- **Paddle**: Represents the player-controlled horizontal bar with position and movement speed
- **Player Progress**: Represents persistent data including unlocked levels, highest scores, completed levels, and earned achievements
- **Achievement**: Represents a trophy/badge with unlock criteria, earned status, and descriptive information
- **Custom Level**: Represents a player-created level with brick layout and metadata (name, author, creation date)
- **Game Settings**: Represents player preferences including control mappings, audio volumes, and display options
- **Game State**: Represents current gameplay session with current level, score, remaining lives, ball positions, brick states

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: Players can launch the game, navigate to casual mode, and start playing a level within 30 seconds of application start
- **SC-002**: Gameplay maintains 60 FPS on target hardware (defined as: Linux desktop with integrated graphics from 2020 or newer) during active gameplay with full screen of bricks
- **SC-003**: Paddle responds to player input with latency below 16ms (one frame at 60 FPS)
- **SC-004**: Players can complete a full game loop (start level, destroy all bricks, complete level) and verify score increases and next level unlocks
- **SC-005**: 90% of players successfully navigate from main menu to their desired screen on first attempt without confusion
- **SC-006**: Player progress (unlocked levels, settings, achievements) persists correctly across game restarts with zero data loss
- **SC-007**: Players can create a custom level in the level editor, save it, and play it in casual mode within 5 minutes
- **SC-008**: Ball physics produces predictable and fair bounces - players can control ball direction by hitting with different paddle positions
- **SC-009**: All seven main screens (Main Menu, Casual Game, Arcade Game, Level Selection, Trophies, Settings, Level Editor, My Other Games) are accessible and functional
- **SC-010**: Game runs for 1 hour of continuous gameplay without crashes, memory leaks, or performance degradation
- **SC-011**: Audio volume adjustments in settings are immediately audible and persist across game sessions
- **SC-012**: Players complete arcade mode runs (start to game over) and final score/level statistics are accurate

### Assumptions

- Target hardware: Linux desktop/laptop with integrated graphics from 2020 or newer, 4GB RAM minimum
- Players are familiar with basic brick breaker/breakout game mechanics
- Default controls use standard keyboard keys (arrow keys or WASD for paddle movement) and mouse for menu navigation
- Built-in levels will number between 10-30 levels for initial release (exact count to be determined during design)
- Achievement criteria will be designed during implementation to provide reasonable unlock progression (mix of easy, medium, hard achievements)
- Audio assets (music, sound effects) will be provided or created separately; specification assumes they will be available
- "My Other Games" content will be provided as static content/data (game titles, descriptions, images) separately from this implementation

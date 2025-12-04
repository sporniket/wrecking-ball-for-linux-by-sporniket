# Tasks: Sporniket's Wrecking Ball - Brick Breaker Game

**Input**: Design documents from `/specs/001-brick-breaker-game/`
**Prerequisites**: plan.md (tech stack), spec.md (user stories), research.md (technology decisions), data-model.md (entities), contracts/ (API contracts)

**Tests**: Test tasks are included based on the specification's Testing & Quality Assurance requirements (Catch2 unit tests, integration tests, manual checklists).

**Organization**: Tasks are grouped by user story to enable independent implementation and testing of each story.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: Can run in parallel (different files, no dependencies)
- **[Story]**: Which user story this task belongs to (e.g., US1, US2, US3)
- Include exact file paths in descriptions

## Path Conventions

This project uses single native desktop application structure:
- **Source code**: `src/` at repository root
- **Tests**: `tests/` at repository root
- **Assets**: `assets/` at repository root
- **Runtime data**: `data/` (created at runtime)

---

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization, build system, and basic structure

- [ ] T001 Create project directory structure per plan.md (src/, tests/, assets/, data/ directories with subdirectories)
- [ ] T002 Initialize CMake build system with CMakeLists.txt (C++23, GCC 13+/Clang 16+, SDL2/SDL2_ttf/SDL2_mixer dependencies)
- [ ] T003 [P] Create LICENSE file with AGPL-3.0 license text in repository root
- [ ] T004 [P] Create README.md with project overview, build instructions, and copyright notice
- [ ] T005 [P] Create THIRD-PARTY-NOTICES.md template for dependency licenses (SDL2, inih, Catch2)
- [ ] T006 [P] Vendor inih library (INI parsing) in src/third_party/inih/ with copyright notice
- [ ] T007 [P] Setup Catch2 v3.x testing framework via CMake FetchContent in CMakeLists.txt
- [ ] T008 [P] Create .clang-format configuration file for code formatting in repository root
- [ ] T009 [P] Create copyright header template in .specify/templates/copyright-header.txt

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before ANY user story can be implemented

**⚠️ CRITICAL**: No user story work can begin until this phase is complete

### Platform Abstraction Interfaces

- [ ] T010 [P] Create IRenderer interface in src/platform/graphics/renderer_interface.hpp
- [ ] T011 [P] Create IAudioPlayer interface in src/platform/audio/audio_interface.hpp
- [ ] T012 [P] Create IInputHandler interface in src/platform/input/input_interface.hpp
- [ ] T013 [P] Create IFileIO interface in src/platform/files/file_interface.hpp

### Core Entity Definitions

- [ ] T014 [P] Create Brick entity struct in src/core/entities/brick.hpp (position, width, type, hits, color)
- [ ] T015 [P] Create Ball entity struct in src/core/entities/ball.hpp (position, velocity, radius, state)
- [ ] T016 [P] Create Paddle entity struct in src/core/entities/paddle.hpp (position, width, velocity)
- [ ] T017 [P] Create Level entity struct in src/core/entities/level.hpp (id, name, type, bricks vector, metadata)
- [ ] T018 [P] Create GameState entity struct in src/core/entities/game_state.hpp (current level, mode, score, lives, balls, paddle)
- [ ] T019 [P] Create PlayerProgress entity struct in src/core/entities/player_progress.hpp (unlocked levels, stats, achievements)
- [ ] T020 [P] Create GameSettings entity struct in src/core/entities/game_settings.hpp (volumes, controls, display options)
- [ ] T021 [P] Create Achievement entity struct in src/core/entities/achievement.hpp (id, name, description, criteria)

### Enums and Constants

- [ ] T022 [P] Create BrickType enum in src/core/entities/brick_type.hpp (Empty, Regular, Star, Key, Exit)
- [ ] T023 [P] Create LevelType enum in src/core/entities/level_type.hpp (Classical, ReachForStars, GetTheKey)
- [ ] T024 [P] Create GameMode enum in src/core/entities/game_mode.hpp (Casual, Arcade)
- [ ] T025 [P] Create GameScreen enum in src/core/entities/game_screen.hpp (MainMenu, LevelSelect, GameplayCasual, etc.)
- [ ] T026 [P] Create InputMethod enum in src/core/entities/input_method.hpp (Mouse, Keyboard, Controller)
- [ ] T027 [P] Create SoundEffect enum in src/platform/audio/sound_effect.hpp (BrickHit, BrickDestroyed, PaddleBounce, etc.)
- [ ] T028 [P] Create Constants file in src/core/constants.hpp (screen dimensions, grid units, 16-color palette, physics constants)

### Error Handling and Logging

- [ ] T029 [P] Create ErrorLogger class in src/core/logging/error_logger.hpp (log to stderr and error-<timestamp>.log)
- [ ] T030 [P] Implement ErrorLogger in src/core/logging/error_logger.cpp (create log file with millisecond timestamp)

### Main Application Structure

- [ ] T031 Create Application class in src/main.cpp (main game loop, initialization, cleanup)
- [ ] T032 Implement fixed time step game loop in Application::Run() (60 FPS target, accumulator pattern per research.md)

**Checkpoint**: Foundation ready - user story implementation can now begin in parallel

---

## Phase 3: User Story 1 - Core Gameplay with Multiple Level Types (Priority: P1) 🎯 MVP

**Goal**: Implement the minimum viable product - playable brick breaker game with three level types (Classical, Reach for the Stars, Get the Key), variable-width bricks, paddle control, ball physics, collision detection, and level completion logic.

**Independent Test**: Launch game, start each of the three level types, control paddle with mouse/keyboard, destroy bricks according to level-specific rules, verify level completion detection works correctly for all three types.

### Tests for User Story 1

> **NOTE: Write these tests FIRST, ensure they FAIL before implementation**

- [ ] T033 [P] [US1] Create Catch2 test for Brick entity in tests/unit/entities/brick_test.cpp
- [ ] T034 [P] [US1] Create Catch2 test for Ball entity in tests/unit/entities/ball_test.cpp
- [ ] T035 [P] [US1] Create Catch2 test for Paddle entity in tests/unit/entities/paddle_test.cpp
- [ ] T036 [P] [US1] Create Catch2 test for Level entity in tests/unit/entities/level_test.cpp
- [ ] T037 [P] [US1] Create Catch2 test for collision detection with 1-unit bricks in tests/unit/physics/collision_narrow_test.cpp
- [ ] T038 [P] [US1] Create Catch2 test for collision detection with wide bricks (10+ units) in tests/unit/physics/collision_wide_test.cpp
- [ ] T039 [P] [US1] Create Catch2 test for collision detection with full-width bricks (40 units) in tests/unit/physics/collision_fullwidth_test.cpp
- [ ] T040 [P] [US1] Create Catch2 test for paddle-ball collision angle calculation in tests/unit/physics/paddle_bounce_test.cpp
- [ ] T041 [P] [US1] Create Catch2 test for Classical level completion detection in tests/unit/level/classical_completion_test.cpp
- [ ] T042 [P] [US1] Create Catch2 test for ReachForStars level completion detection in tests/unit/level/star_completion_test.cpp
- [ ] T043 [P] [US1] Create Catch2 test for GetTheKey level completion detection in tests/unit/level/key_completion_test.cpp
- [ ] T044 [P] [US1] Create Catch2 test for exit brick activation logic in tests/unit/level/exit_activation_test.cpp
- [ ] T045 [P] [US1] Create manual playtest checklist for Classical levels in tests/manual/classical_levels.md
- [ ] T046 [P] [US1] Create manual playtest checklist for ReachForStars levels in tests/manual/star_levels.md
- [ ] T047 [P] [US1] Create manual playtest checklist for GetTheKey levels in tests/manual/key_levels.md

### SDL2 Platform Implementation

- [ ] T048 [P] [US1] Implement SDLRenderer class in src/sdl/renderer/sdl_renderer.hpp (implements IRenderer)
- [ ] T049 [US1] Implement SDLRenderer methods in src/sdl/renderer/sdl_renderer.cpp (BeginFrame, EndFrame, DrawBrick, DrawBall, DrawPaddle, DrawText)
- [ ] T050 [US1] Implement logical rendering size setup (320x200) in SDLRenderer::SetLogicalSize()
- [ ] T051 [US1] Implement nearest-neighbor texture filtering in SDLRenderer (disable SDL_HINT_RENDER_SCALE_QUALITY)
- [ ] T052 [P] [US1] Implement SDLInputHandler class in src/sdl/input/sdl_input_handler.hpp (implements IInputHandler)
- [ ] T053 [US1] Implement SDLInputHandler methods in src/sdl/input/sdl_input_handler.cpp (PollInput, GetInputState, mouse/keyboard support)
- [ ] T054 [P] [US1] Implement SDLAudioPlayer class stub in src/sdl/audio/sdl_audio_player.hpp (implements IAudioPlayer, silent for now)
- [ ] T055 [US1] Implement SDLAudioPlayer Initialize method in src/sdl/audio/sdl_audio_player.cpp (handle no audio device gracefully)

### Custom Markdown Level Parser

- [ ] T056 [P] [US1] Create LevelParser class in src/persistence/markdown/level_parser.hpp (LoadLevel, ValidateLevel, SaveLevel methods)
- [ ] T057 [US1] Implement LevelParser::LoadLevel() in src/persistence/markdown/level_parser.cpp (parse YAML front matter)
- [ ] T058 [US1] Implement fenced code block extraction in LevelParser::LoadLevel() (find ```brick-layout markers)
- [ ] T059 [US1] Implement brick encoding parser in LevelParser (parse <brick_code><dashes> format per data-model.md)
- [ ] T060 [US1] Implement brick width calculation in LevelParser (count dashes to determine span)
- [ ] T061 [US1] Implement level validation in LevelParser::ValidateLevel() (check line width = 38 units, line count = 23, brick codes valid)
- [ ] T062 [US1] Implement level type validation in LevelParser::ValidateLevel() (ReachForStars has ≥1 star, GetTheKey has ≥1 key and ≥1 exit)
- [ ] T063 [US1] Add error handling to LevelParser (abort on malformed data, log to stderr and error log file)
- [ ] T064 [P] [US1] Create Catch2 test for LevelParser with valid level file in tests/unit/persistence/level_parser_valid_test.cpp
- [ ] T065 [P] [US1] Create Catch2 test for LevelParser with malformed input in tests/unit/persistence/level_parser_malformed_test.cpp

### Physics Engine Implementation

- [ ] T066 [P] [US1] Create PhysicsEngine class in src/core/physics/physics_engine.hpp (implements IPhysicsEngine)
- [ ] T067 [US1] Implement PhysicsEngine::UpdateBalls() in src/core/physics/physics_engine.cpp (update ball positions based on velocity * dt)
- [ ] T068 [US1] Implement PhysicsEngine::CheckWallCollisions() (bounce ball off walls and ceiling)
- [ ] T069 [US1] Implement PhysicsEngine::CheckPaddleCollision() (AABB collision detection, angle calculation per research.md)
- [ ] T070 [US1] Implement PhysicsEngine::CalculatePaddleBounce() (calculate bounce angle based on impact position)
- [ ] T071 [US1] Implement PhysicsEngine::CheckBrickCollision() for variable-width bricks (AABB detection across full brick width)
- [ ] T072 [US1] Implement brick damage logic in PhysicsEngine::CheckBrickCollision() (decrement hits_remaining, set is_destroyed when 0)
- [ ] T073 [US1] Implement PhysicsEngine::IsBallLost() (check if ball Y position > paddle Y threshold)
- [ ] T074 [US1] Ensure ball speed magnitude preservation after collisions (elastic collisions per research.md)

### Level Completion System

- [ ] T075 [P] [US1] Create LevelCompletionChecker class in src/core/level/completion_checker.hpp (implements ILevelCompletionChecker)
- [ ] T076 [US1] Implement LevelCompletionChecker::IsLevelComplete() for Classical levels in src/core/level/completion_checker.cpp
- [ ] T077 [US1] Implement LevelCompletionChecker::IsLevelComplete() for ReachForStars levels (check only star bricks destroyed)
- [ ] T078 [US1] Implement LevelCompletionChecker::IsLevelComplete() for GetTheKey levels (check all keys + any exit destroyed)
- [ ] T079 [US1] Implement LevelCompletionChecker::UpdateExitBrickActivation() (set exit bricks is_active=true when all keys destroyed)
- [ ] T080 [US1] Implement LevelCompletionChecker::GetCompletionProgress() (return progress string for HUD display)

### Game State Management

- [ ] T081 [P] [US1] Create GameStateManager class in src/core/game/game_state_manager.hpp (implements IGameStateManager)
- [ ] T082 [US1] Implement GameStateManager::StartLevel() in src/core/game/game_state_manager.cpp (load level, initialize paddle, ball)
- [ ] T083 [US1] Implement GameStateManager::Update() (update paddle, balls, check collisions, check completion)
- [ ] T084 [US1] Implement GameStateManager::OnBrickDestroyed() (update score, play sound effect, update stats)
- [ ] T085 [US1] Implement GameStateManager::OnBallLost() for Casual mode (respawn ball immediately)
- [ ] T086 [US1] Implement GameStateManager::OnLevelComplete() (show completion screen, mark level complete)
- [ ] T087 [US1] Implement GameStateManager::TogglePause() (freeze/unfreeze gameplay, show pause menu)
- [ ] T088 [US1] Implement GameStateManager screen transition logic (SetScreen, GetCurrentScreen)

### Gameplay UI Screen

- [ ] T089 [P] [US1] Create GameplayScreen class in src/ui/gameplay/gameplay_screen.hpp
- [ ] T090 [US1] Implement GameplayScreen::Render() in src/ui/gameplay/gameplay_screen.cpp (render play field, bricks, ball, paddle, HUD)
- [ ] T091 [US1] Implement GameplayScreen::RenderBricks() (render variable-width bricks with correct colors based on type/hits)
- [ ] T092 [US1] Implement GameplayScreen::RenderHUD() (display score, level name, level type, completion progress)
- [ ] T093 [US1] Implement GameplayScreen::RenderPauseMenu() (overlay pause menu with resume/exit options)
- [ ] T094 [US1] Implement GameplayScreen::HandleInput() (forward input to GameStateManager, handle pause key)

### Built-in Level Assets

- [ ] T095 [P] [US1] Create Classical level 1 (easy) in assets/levels/001-classical-easy.md
- [ ] T096 [P] [US1] Create Classical level 2 (medium) in assets/levels/002-classical-medium.md
- [ ] T097 [P] [US1] Create ReachForStars level 1 (easy) in assets/levels/003-star-easy.md
- [ ] T098 [P] [US1] Create ReachForStars level 2 (medium) in assets/levels/004-star-medium.md
- [ ] T099 [P] [US1] Create GetTheKey level 1 (easy) in assets/levels/005-key-easy.md
- [ ] T100 [P] [US1] Create GetTheKey level 2 (medium) in assets/levels/006-key-medium.md

### Integration & Wiring

- [ ] T101 [US1] Wire SDLRenderer, SDLInputHandler, PhysicsEngine, LevelCompletionChecker into GameStateManager
- [ ] T102 [US1] Integrate GameplayScreen into Application main loop
- [ ] T103 [US1] Load default level (level 1) on game start in Application::Initialize()
- [ ] T104 [US1] Implement game loop integration (input → update → render) in Application::Run()

**Checkpoint**: At this point, User Story 1 should be fully functional - playable game with all three level types, variable-width bricks, and proper completion detection.

---

## Phase 4: User Story 2 - Main Menu & Navigation (Priority: P2)

**Goal**: Implement main menu as central hub with navigation to all game screens, enabling access to game modes, settings, achievements, level editor, and other games screen.

**Independent Test**: Launch game, verify main menu displays all options, navigate to each screen (even if screens are placeholders), verify back navigation returns to main menu, test exit functionality.

### Implementation for User Story 2

- [ ] T105 [P] [US2] Create MainMenuScreen class in src/ui/menu/main_menu_screen.hpp
- [ ] T106 [US2] Implement MainMenuScreen::Render() in src/ui/menu/main_menu_screen.cpp (display menu title, menu options list)
- [ ] T107 [US2] Implement menu option rendering with selection highlight in MainMenuScreen::RenderMenuOptions()
- [ ] T108 [US2] Implement MainMenuScreen::HandleInput() (navigate menu with keyboard/mouse, select option, exit game)
- [ ] T109 [US2] Implement menu option actions in MainMenuScreen::OnOptionSelected() (transition to corresponding screen)
- [ ] T110 [P] [US2] Create placeholder LevelSelectScreen class in src/ui/level_select/level_select_screen.hpp
- [ ] T111 [US2] Implement placeholder LevelSelectScreen::Render() (show "Level Selection" text, back to menu option)
- [ ] T112 [P] [US2] Create placeholder TrophiesScreen class in src/ui/trophies/trophies_screen.hpp
- [ ] T113 [US2] Implement placeholder TrophiesScreen::Render() (show "Achievements" text, back to menu option)
- [ ] T114 [P] [US2] Create placeholder SettingsScreen class in src/ui/settings/settings_screen.hpp
- [ ] T115 [US2] Implement placeholder SettingsScreen::Render() (show "Settings" text, back to menu option)
- [ ] T116 [P] [US2] Create placeholder LevelEditorScreen class in src/ui/level_editor/level_editor_screen.hpp
- [ ] T117 [US2] Implement placeholder LevelEditorScreen::Render() (show "Level Editor" text, back to menu option)
- [ ] T118 [P] [US2] Create placeholder OtherGamesScreen class in src/ui/other_games/other_games_screen.hpp
- [ ] T119 [US2] Implement placeholder OtherGamesScreen::Render() (show "My Other Games" text, back to menu option)
- [ ] T120 [US2] Update Application to start with MainMenuScreen instead of directly loading GameplayScreen
- [ ] T121 [US2] Implement screen manager in Application (manage current screen, handle screen transitions)
- [ ] T122 [US2] Implement back-to-menu navigation from all screens (listen for back button/ESC key)
- [ ] T123 [US2] Implement graceful application shutdown on Exit menu option (cleanup SDL, save state if needed)

**Checkpoint**: At this point, User Story 2 is complete - main menu provides navigation to all screens (even if some are placeholders), and User Story 1 gameplay remains accessible via menu.

---

## Phase 5: User Story 3 - Arcade Mode (Priority: P3)

**Goal**: Implement arcade mode with limited lives (3/5/7), life deduction on ball loss, game over screen, and sequential level progression from level 1.

**Independent Test**: Select arcade mode from main menu, choose life count, play until losing all lives, verify game over screen shows final score and highest level reached.

### Implementation for User Story 3

- [ ] T124 [P] [US3] Create ArcadeModeScreen class in src/ui/gameplay/arcade_mode_screen.hpp (extends or wraps GameplayScreen)
- [ ] T125 [US3] Implement ArcadeModeScreen::Render() in src/ui/gameplay/arcade_mode_screen.cpp (include lives display in HUD)
- [ ] T126 [US3] Implement arcade mode initialization in ArcadeModeScreen::Initialize() (start with level 1, set selected lives count)
- [ ] T127 [US3] Implement GameStateManager::OnBallLost() for Arcade mode (decrement lives, check for game over)
- [ ] T128 [US3] Implement game over detection in ArcadeModeScreen::Update() (lives == 0)
- [ ] T129 [P] [US3] Create GameOverScreen class in src/ui/gameplay/game_over_screen.hpp
- [ ] T130 [US3] Implement GameOverScreen::Render() in src/ui/gameplay/game_over_screen.cpp (display final score, highest level, return to menu option)
- [ ] T131 [US3] Implement GameOverScreen::HandleInput() (wait for confirmation, return to main menu)
- [ ] T132 [US3] Implement automatic level progression in ArcadeModeScreen::OnLevelComplete() (load level_id + 1)
- [ ] T133 [US3] Implement life count selection screen in ArcadeModeScreen::ShowLifeSelection() (show 3/5/7 options before starting)
- [ ] T134 [US3] Update MainMenuScreen to launch ArcadeModeScreen on Arcade Game option selection
- [ ] T135 [US3] Wire ArcadeModeScreen into Application screen manager

**Checkpoint**: At this point, User Story 3 is complete - arcade mode is playable with limited lives and game over sequence. User Stories 1 and 2 remain functional.

---

## Phase 6: User Story 4 - Level Selection & Progress Tracking (Priority: P4)

**Goal**: Implement level selection screen showing locked/unlocked levels, level progress persistence (INI file), and automatic level unlocking on completion.

**Independent Test**: Complete levels to unlock new ones, close and reopen game, verify unlocked levels persist, verify level selection screen shows correct locked/unlocked states.

### INI Persistence Implementation

- [ ] T136 [P] [US4] Create INIParser class in src/persistence/ini/ini_parser.hpp (wrapper around inih library)
- [ ] T137 [US4] Implement INIParser::ParseFile() in src/persistence/ini/ini_parser.cpp (parse INI file, return key-value map)
- [ ] T138 [US4] Implement INIParser::WriteFile() (write key-value map to INI file)
- [ ] T139 [US4] Add validation to INIParser (check bounds for numeric values, enforce length limits per data-model.md)
- [ ] T140 [P] [US4] Create Catch2 test for INIParser with valid INI in tests/unit/persistence/ini_parser_valid_test.cpp
- [ ] T141 [P] [US4] Create Catch2 test for INIParser with malformed INI in tests/unit/persistence/ini_parser_malformed_test.cpp

### Progress Manager Implementation

- [ ] T142 [P] [US4] Create ProgressManager class in src/persistence/progress_manager.hpp (implements IProgressManager)
- [ ] T143 [US4] Implement ProgressManager::LoadProgress() in src/persistence/progress_manager.cpp (load from data/progress/player-progress.ini)
- [ ] T144 [US4] Implement ProgressManager::SaveProgress() (write PlayerProgress to INI file)
- [ ] T145 [US4] Implement ProgressManager::UnlockLevel() (add level_id to unlocked_levels set, ensure level 1 always unlocked)
- [ ] T146 [US4] Implement ProgressManager::MarkLevelComplete() (update completed level counts by type, update high score if applicable)
- [ ] T147 [US4] Implement ProgressManager::UpdateStats() (increment bricks destroyed, stars/keys collected)
- [ ] T148 [US4] Implement ProgressManager::IsLevelUnlocked() (check if level_id in unlocked_levels set)
- [ ] T149 [US4] Handle file corruption in ProgressManager::LoadProgress() (create new progress file with defaults if corrupted)

### Level Selection Screen Implementation

- [ ] T150 [P] [US4] Implement full LevelSelectScreen::Render() in src/ui/level_select/level_select_screen.cpp (replace placeholder)
- [ ] T151 [US4] Implement level grid rendering in LevelSelectScreen::RenderLevelGrid() (show all levels with locked/unlocked icons)
- [ ] T152 [US4] Implement level info display in LevelSelectScreen::RenderLevelInfo() (show selected level name, type, difficulty)
- [ ] T153 [US4] Implement LevelSelectScreen::HandleInput() (navigate level grid, select level, back to menu)
- [ ] T154 [US4] Implement level selection action in LevelSelectScreen::OnLevelSelected() (load selected level in GameplayScreen)
- [ ] T155 [US4] Implement locked level handling in LevelSelectScreen::OnLevelSelected() (ignore clicks on locked levels, show message)

### Integration with Game State

- [ ] T156 [US4] Integrate ProgressManager into GameStateManager (pass to constructor or as dependency)
- [ ] T157 [US4] Call ProgressManager::MarkLevelComplete() in GameStateManager::OnLevelComplete()
- [ ] T158 [US4] Call ProgressManager::UnlockLevel() in GameStateManager::OnLevelComplete() (unlock level_id + 1)
- [ ] T159 [US4] Call ProgressManager::SaveProgress() after level completion in GameStateManager::OnLevelComplete()
- [ ] T160 [US4] Load progress on application start in Application::Initialize() (call ProgressManager::LoadProgress())
- [ ] T161 [US4] Pass unlocked levels to LevelSelectScreen in Application screen manager

**Checkpoint**: At this point, User Story 4 is complete - level selection works, progress persists across sessions, automatic unlocking works. All previous user stories remain functional.

---

## Phase 7: User Story 5 - Achievements & Trophies (Priority: P5)

**Goal**: Implement achievement system with predefined achievements, achievement tracking, trophy screen display, and achievement notifications.

**Independent Test**: Play game to trigger achievements, view trophies screen to verify earned achievements displayed correctly, verify notifications appear when achievements are earned.

### Achievement System Implementation

- [ ] T162 [P] [US5] Create AchievementTracker class in src/core/game/achievement_tracker.hpp (implements IAchievementTracker)
- [ ] T163 [US5] Define predefined achievements in src/core/game/achievements.cpp (first_level_complete, star_collector, key_master, high_scorer, etc.)
- [ ] T164 [US5] Implement AchievementTracker::CheckAchievements() in src/core/game/achievement_tracker.cpp (evaluate criteria against PlayerProgress)
- [ ] T165 [US5] Implement AchievementTracker::GetAllAchievements() (return full list of defined achievements)
- [ ] T166 [US5] Implement AchievementTracker::GetEarnedAchievements() (filter achievements by earned status from PlayerProgress)
- [ ] T167 [US5] Implement achievement notification queue in AchievementTracker (queue multiple simultaneous achievements)
- [ ] T168 [US5] Implement AchievementTracker::ShowAchievementNotification() (display overlay notification for 3 seconds)

### Trophies Screen Implementation

- [ ] T169 [P] [US5] Implement full TrophiesScreen::Render() in src/ui/trophies/trophies_screen.cpp (replace placeholder)
- [ ] T170 [US5] Implement achievement grid rendering in TrophiesScreen::RenderAchievementGrid() (show all achievements with earned/unearned states)
- [ ] T171 [US5] Implement achievement detail view in TrophiesScreen::RenderAchievementDetail() (show name, description, progress, unlock criteria)
- [ ] T172 [US5] Implement TrophiesScreen::HandleInput() (navigate achievement grid, select achievement, back to menu)
- [ ] T173 [US5] Implement visual distinction for earned achievements in TrophiesScreen (highlight, different color, badge icon)

### Integration

- [ ] T174 [US5] Integrate AchievementTracker into GameStateManager
- [ ] T175 [US5] Call AchievementTracker::CheckAchievements() after level completion in GameStateManager::OnLevelComplete()
- [ ] T176 [US5] Add earned achievements to PlayerProgress in ProgressManager::CheckAndAwardAchievements()
- [ ] T177 [US5] Render achievement notifications in GameplayScreen::Render() (overlay on game screen)

**Checkpoint**: At this point, User Story 5 is complete - achievement system tracks and awards achievements, trophies screen displays them correctly. All previous user stories remain functional.

---

## Phase 8: User Story 6 - Settings & Configuration (Priority: P6)

**Goal**: Implement settings screen with configurable controls, audio volumes, display options, and settings persistence (INI file).

**Independent Test**: Access settings screen, change options (volumes, controls, display), verify changes applied immediately, restart game and verify settings persisted.

### Settings Manager Implementation

- [ ] T178 [P] [US6] Create SettingsManager class in src/persistence/settings_manager.hpp (implements ISettingsManager)
- [ ] T179 [US6] Implement SettingsManager::LoadSettings() in src/persistence/settings_manager.cpp (load from data/settings/game-settings.ini)
- [ ] T180 [US6] Implement SettingsManager::SaveSettings() (write GameSettings to INI file)
- [ ] T181 [US6] Implement default settings creation in SettingsManager::LoadSettings() (create INI with defaults if file doesn't exist)
- [ ] T182 [US6] Implement settings validation in SettingsManager (validate volume 0-100, valid enum values, no conflicting key bindings)
- [ ] T183 [US6] Implement SettingsManager::ApplySettings() (update audio volumes, input method, display mode in platform layer)

### Settings Screen Implementation

- [ ] T184 [P] [US6] Implement full SettingsScreen::Render() in src/ui/settings/settings_screen.cpp (replace placeholder)
- [ ] T185 [US6] Implement audio settings UI in SettingsScreen::RenderAudioSettings() (volume sliders for master, music, SFX)
- [ ] T186 [US6] Implement display settings UI in SettingsScreen::RenderDisplaySettings() (fullscreen toggle, VSync toggle, integer scaling toggle)
- [ ] T187 [US6] Implement input settings UI in SettingsScreen::RenderInputSettings() (input method selector, key binding remapping)
- [ ] T188 [US6] Implement SettingsScreen::HandleInput() (navigate settings, adjust sliders, remap keys, back to menu)
- [ ] T189 [US6] Implement immediate setting application in SettingsScreen::OnSettingChanged() (call SettingsManager::ApplySettings())
- [ ] T190 [US6] Implement key remapping UI in SettingsScreen::ShowKeyRemappingDialog() (wait for key press, check for conflicts)

### SDL Audio Implementation

- [ ] T191 [US6] Implement full SDLAudioPlayer in src/sdl/audio/sdl_audio_player.cpp (replace stub, use SDL2_mixer)
- [ ] T192 [US6] Implement SDLAudioPlayer::PlayMusic() (load music file, loop playback)
- [ ] T193 [US6] Implement SDLAudioPlayer::PlaySoundEffect() (play one-shot sound effect, max 8 simultaneous)
- [ ] T194 [US6] Implement SDLAudioPlayer::SetMasterVolume(), SetMusicVolume(), SetSFXVolume() (apply volume changes immediately)
- [ ] T195 [US6] Load audio assets in Application::Initialize() (music tracks, sound effect files from assets/audio/)

### Game Controller Support

- [ ] T196 [US6] Implement controller detection in SDLInputHandler::Initialize() (use SDL_GameController API)
- [ ] T197 [US6] Implement controller input handling in SDLInputHandler::PollInput() (read analog stick, buttons)
- [ ] T198 [US6] Implement controller hot-plug handling in SDLInputHandler (detect add/remove events)
- [ ] T199 [US6] Apply controller deadzone from GameSettings in SDLInputHandler

### Integration

- [ ] T200 [US6] Load settings on application start in Application::Initialize() (call SettingsManager::LoadSettings())
- [ ] T201 [US6] Apply settings to SDL2 on startup (fullscreen, VSync, audio volumes)
- [ ] T202 [US6] Save settings on application exit in Application::Shutdown() (call SettingsManager::SaveSettings())

**Checkpoint**: At this point, User Story 6 is complete - settings are configurable and persist across sessions, audio works, game controller support is functional. All previous user stories remain functional.

---

## Phase 9: User Story 7 - Level Editor (Priority: P7)

**Goal**: Implement level editor for creating custom levels with brick placement tools, level type selection, level saving/loading, and integration with level selection screen.

**Independent Test**: Access level editor, create custom level with variable-width bricks, save level, navigate to level selection screen, verify custom level appears, play custom level.

### Level Editor Implementation

- [ ] T203 [P] [US7] Implement full LevelEditorScreen::Render() in src/ui/level_editor/level_editor_screen.cpp (replace placeholder)
- [ ] T204 [US7] Implement grid rendering in LevelEditorScreen::RenderEditorGrid() (38x23 grid with empty cells)
- [ ] T205 [US7] Implement editing tools UI in LevelEditorScreen::RenderToolbar() (brick type selector, width selector, save/test/clear buttons)
- [ ] T206 [US7] Implement level type selector in LevelEditorScreen::RenderLevelTypeSelector() (Classical, ReachForStars, GetTheKey dropdown)
- [ ] T207 [US7] Implement LevelEditorScreen::HandleInput() (click to place/remove brick, select tool, save/test level)
- [ ] T208 [US7] Implement brick placement logic in LevelEditorScreen::OnGridCellClicked() (place brick of selected type and width at clicked position)
- [ ] T209 [US7] Implement brick removal logic in LevelEditorScreen::OnGridCellClicked() (remove brick if cell occupied)
- [ ] T210 [US7] Implement width adjustment UI in LevelEditorScreen::HandleWidthSelection() (allow selecting brick width 1-40 units)
- [ ] T211 [US7] Implement visual preview of brick width in LevelEditorScreen::RenderBrickPreview() (show selected width before placement)
- [ ] T212 [US7] Implement level validation in LevelEditorScreen::ValidateLevel() (check for required bricks based on level type)
- [ ] T213 [US7] Implement save level dialog in LevelEditorScreen::ShowSaveLevelDialog() (prompt for level name, check for overwrite)
- [ ] T214 [US7] Implement LevelParser::SaveLevel() in src/persistence/markdown/level_parser.cpp (write Level to Markdown file)
- [ ] T215 [US7] Save custom levels to data/custom_levels/ directory in LevelEditorScreen::OnSaveLevel()
- [ ] T216 [US7] Implement test level functionality in LevelEditorScreen::OnTestLevel() (launch GameplayScreen with current editor level)
- [ ] T217 [US7] Implement clear level functionality in LevelEditorScreen::OnClearLevel() (reset all bricks to empty)

### Integration with Level Selection

- [ ] T218 [US7] Update LevelParser::GetAvailableLevels() to include custom levels from data/custom_levels/
- [ ] T219 [US7] Display custom levels in LevelSelectScreen alongside built-in levels (with "Custom" indicator)
- [ ] T220 [US7] Allow playing custom levels from LevelSelectScreen (load from data/custom_levels/)

**Checkpoint**: At this point, User Story 7 is complete - level editor allows creating, saving, and playing custom levels. All previous user stories remain functional.

---

## Phase 10: User Story 8 - My Other Games (Priority: P8)

**Goal**: Implement promotional screen displaying information about other games by the developer.

**Independent Test**: Access "My Other Games" screen, verify game list is displayed, verify game details can be viewed.

### Implementation for User Story 8

- [ ] T221 [P] [US8] Implement full OtherGamesScreen::Render() in src/ui/other_games/other_games_screen.cpp (replace placeholder)
- [ ] T222 [US8] Implement game list rendering in OtherGamesScreen::RenderGameList() (display game titles and brief descriptions)
- [ ] T223 [US8] Implement game detail view in OtherGamesScreen::RenderGameDetail() (show selected game's full description, images, availability)
- [ ] T224 [US8] Implement OtherGamesScreen::HandleInput() (navigate game list, select game, view details, back to menu)
- [ ] T225 [P] [US8] Create games data file in assets/other_games.txt (list of games with titles, descriptions, availability info)
- [ ] T226 [US8] Implement games data loading in OtherGamesScreen::LoadGamesData() (parse games data file)

**Checkpoint**: At this point, User Story 8 is complete - promotional screen displays other games. All user stories are now implemented.

---

## Phase 11: Polish & Cross-Cutting Concerns

**Purpose**: Final improvements, optimizations, and documentation that affect multiple user stories

- [ ] T227 [P] Add more built-in levels (10-30 total, balanced mix of types) in assets/levels/
- [ ] T228 [P] Create level completion screen UI in src/ui/gameplay/level_complete_screen.hpp
- [ ] T229 Implement level completion screen rendering (show score, stars/keys collected, next level button)
- [ ] T230 [P] Add sound effect assets in assets/audio/sfx/ (brick hit, destroyed, paddle bounce, wall bounce, ball lost, level complete, etc.)
- [ ] T231 [P] Add background music tracks in assets/audio/music/ (menu theme, gameplay theme, editor theme)
- [ ] T232 [P] Add TTF bitmap fonts in assets/fonts/ (retro-style 8-point font for text rendering)
- [ ] T233 Implement asset loading for all audio and font files in Application::Initialize()
- [ ] T234 Implement 16-color palette enforcement in SDLRenderer (use lookup table for all colors)
- [ ] T235 Implement stuck ball detection and auto-respawn in PhysicsEngine (timeout after 5 seconds of no brick collisions)
- [ ] T236 [P] Performance profiling with gprof or perf (verify 60 FPS maintained with full screen of bricks)
- [ ] T237 Optimize collision detection if needed (spatial partitioning if >100 bricks)
- [ ] T238 [P] Memory leak detection with Valgrind (verify no leaks in game loop)
- [ ] T239 [P] Code cleanup and formatting with clang-format (apply to all source files)
- [ ] T240 [P] Update THIRD-PARTY-NOTICES.md with all dependency licenses (SDL2, inih, Catch2, fonts, audio if applicable)
- [ ] T241 [P] Create comprehensive README.md with screenshots, build instructions, gameplay guide
- [ ] T242 Run full test suite with Catch2 (ctest --output-on-failure) and verify all tests pass
- [ ] T243 Run all manual playtesting checklists and verify acceptance criteria
- [ ] T244 Package game for distribution (create release build, bundle assets, create installer/archive)

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies - can start immediately
- **Foundational (Phase 2)**: Depends on Setup completion - BLOCKS all user stories
- **User Story 1 (Phase 3)**: Depends on Foundational (Phase 2) - No dependencies on other stories
- **User Story 2 (Phase 4)**: Depends on Foundational (Phase 2) - Integrates with US1 but independently testable
- **User Story 3 (Phase 5)**: Depends on Foundational (Phase 2), builds on US1/US2 - Independently testable
- **User Story 4 (Phase 6)**: Depends on Foundational (Phase 2), builds on US1 - Independently testable
- **User Story 5 (Phase 7)**: Depends on Foundational (Phase 2), builds on US4 (needs PlayerProgress) - Independently testable
- **User Story 6 (Phase 8)**: Depends on Foundational (Phase 2) - Mostly independent, enhances all stories
- **User Story 7 (Phase 9)**: Depends on Foundational (Phase 2), US1 (gameplay), US4 (level selection) - Independently testable
- **User Story 8 (Phase 10)**: Depends on Foundational (Phase 2), US2 (menu navigation) - Independently testable
- **Polish (Phase 11)**: Depends on all desired user stories being complete

### User Story Dependencies

```text
Foundational (Phase 2) → BLOCKS ALL
                    ↓
      ┌─────────────┼─────────────────────────┐
      ↓             ↓                         ↓
    US1 (P1)     US2 (P2)                  US6 (P6)
  (Gameplay)   (Menu/Nav)              (Settings)
      │             │
      ├─────────────┤
      ↓             ↓
    US3 (P3)     US4 (P4)
  (Arcade)    (Level Select)
      │             │
      │             ├────────┐
      │             ↓        ↓
      │          US5 (P5)  US7 (P7)
      │       (Achievements) (Editor)
      │
      └─────────────────────┐
                            ↓
                         US8 (P8)
                      (Other Games)
```

### Within Each User Story

- Tests MUST be written and FAIL before implementation
- Entity definitions before implementations that use them
- Platform implementations before game systems that use them
- Core game logic before UI screens
- Integration tasks after individual components are complete

### Parallel Opportunities

- **Setup (Phase 1)**: All [P] tasks can run in parallel (T003-T009)
- **Foundational (Phase 2)**: Platform interfaces (T010-T013), entities (T014-T021), enums (T022-T028) can all run in parallel
- **User Story 1 Tests**: All test creation tasks (T033-T047) can run in parallel
- **User Story 1 SDL**: Platform implementations (T048-T055) can run in parallel
- **User Story 1 Assets**: Level creation tasks (T095-T100) can run in parallel
- Once Foundational phase completes, **multiple user stories can be developed in parallel** by different team members

---

## Parallel Example: User Story 1 (Core Gameplay)

```bash
# Launch all test file creation tasks together:
Task T033: "Create Catch2 test for Brick entity in tests/unit/entities/brick_test.cpp"
Task T034: "Create Catch2 test for Ball entity in tests/unit/entities/ball_test.cpp"
Task T035: "Create Catch2 test for Paddle entity in tests/unit/entities/paddle_test.cpp"
# ... all T033-T047 can be created in parallel

# Launch all platform implementation tasks together:
Task T048: "Implement SDLRenderer class in src/sdl/renderer/sdl_renderer.hpp"
Task T052: "Implement SDLInputHandler class in src/sdl/input/sdl_input_handler.hpp"
Task T054: "Implement SDLAudioPlayer class stub in src/sdl/audio/sdl_audio_player.hpp"
# These are different files, can proceed in parallel

# Launch all level asset creation tasks together:
Task T095: "Create Classical level 1 in assets/levels/001-classical-easy.md"
Task T096: "Create Classical level 2 in assets/levels/002-classical-medium.md"
Task T097: "Create ReachForStars level 1 in assets/levels/003-star-easy.md"
# ... all T095-T100 are different files, fully parallel
```

---

## Implementation Strategy

### MVP First (User Story 1 Only)

1. Complete Phase 1: Setup (T001-T009)
2. Complete Phase 2: Foundational (T010-T032) - **CRITICAL BLOCKER**
3. Complete Phase 3: User Story 1 (T033-T104)
4. **STOP and VALIDATE**: Test User Story 1 independently using manual checklists
5. Build release, demo playable game with three level types

**MVP Deliverable**: Fully playable brick breaker game with:
- Three level types (Classical, Reach for the Stars, Get the Key)
- Variable-width bricks (1-40 units)
- Paddle control (mouse/keyboard)
- Ball physics and collision detection
- Level completion detection
- 6 built-in levels (2 of each type)

### Incremental Delivery

1. **Foundation**: Complete Setup + Foundational → Build succeeds, tests compile
2. **MVP (US1)**: Add Core Gameplay → Playable game → **Demo/Release v0.1**
3. **Navigation (US2)**: Add Main Menu → Full menu system → **Demo/Release v0.2**
4. **Challenge (US3)**: Add Arcade Mode → Game modes complete → **Demo/Release v0.3**
5. **Progression (US4)**: Add Level Selection & Persistence → Save system → **Demo/Release v0.4**
6. **Engagement (US5)**: Add Achievements → Reward system → **Demo/Release v0.5**
7. **Customization (US6)**: Add Settings → Fully configurable → **Demo/Release v0.6**
8. **Creativity (US7)**: Add Level Editor → User-generated content → **Demo/Release v0.7**
9. **Promotion (US8)**: Add Other Games Screen → Complete feature set → **Demo/Release v0.8**
10. **Polish (Phase 11)**: Final improvements → **Release v1.0**

Each increment is independently testable and deployable.

### Parallel Team Strategy

With 3 developers after Foundational phase completion:

1. **Team** completes Setup (Phase 1) + Foundational (Phase 2) together
2. Once Foundational is done:
   - **Developer A**: User Story 1 (Core Gameplay) - Priority P1
   - **Developer B**: User Story 2 (Main Menu & Navigation) - Priority P2
   - **Developer C**: User Story 6 (Settings & Configuration) - Priority P6 (independent)
3. After Developer A completes US1:
   - **Developer A**: User Story 3 (Arcade Mode) - builds on US1
4. After Developer B completes US2 and Developer A completes US3:
   - **Developer B**: User Story 4 (Level Selection) - builds on US1
   - **Developer A**: User Story 5 (Achievements) - builds on US4
5. After Developer B completes US4:
   - **Developer C**: User Story 7 (Level Editor) - builds on US1/US4
6. After all complete:
   - **Developer C**: User Story 8 (Other Games) - quick task
   - **Team**: Phase 11 (Polish) together

---

## Summary

**Total Tasks**: 244 tasks across 11 phases

**Task Count by Phase**:
- Phase 1 (Setup): 9 tasks
- Phase 2 (Foundational): 23 tasks
- Phase 3 (US1 - Core Gameplay): 72 tasks ← Largest phase (MVP)
- Phase 4 (US2 - Main Menu): 19 tasks
- Phase 5 (US3 - Arcade Mode): 12 tasks
- Phase 6 (US4 - Level Selection & Progress): 26 tasks
- Phase 7 (US5 - Achievements): 16 tasks
- Phase 8 (US6 - Settings & Configuration): 25 tasks
- Phase 9 (US7 - Level Editor): 18 tasks
- Phase 10 (US8 - Other Games): 6 tasks
- Phase 11 (Polish): 18 tasks

**Parallel Opportunities**:
- 89 tasks marked [P] can run in parallel within their phase
- After Foundational phase, 8 user stories can be developed in parallel (if team capacity allows)
- Test creation tasks are highly parallelizable (47 test tasks in total)

**Critical Path**:
1. Setup (9 tasks) → Foundational (23 tasks) → **32 tasks before any user story work**
2. User Story 1 is the MVP critical path (72 tasks after foundation)
3. Total MVP: 32 + 72 = **104 tasks to playable game**

**Independent Test Criteria**:
- Each user story phase includes specific test validation steps
- Manual playtest checklists provided for gameplay validation
- Unit tests with Catch2 ensure component correctness
- Integration testing validates user journeys

**Format Validation**: ✅ All 244 tasks follow checklist format with Task ID, [P] markers for parallel tasks, [Story] labels for user story tasks, and exact file paths in descriptions.

**Suggested MVP Scope**: Phases 1-3 (Setup + Foundational + User Story 1) = 104 tasks for playable brick breaker game with three level types.

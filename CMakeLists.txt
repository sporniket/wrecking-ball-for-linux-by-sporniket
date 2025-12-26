# Copyright (c) 2025 David SPORN
# SPDX-License-Identifier: AGPL-3.0-or-later

cmake_minimum_required(VERSION 3.25)

project(wreckingball
    VERSION 0.1.0
    DESCRIPTION "Sporniket's Wrecking Ball - Brick Breaker Game"
    LANGUAGES CXX
)

# C++23 standard requirement
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler requirements
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13.0")
        message(FATAL_ERROR "GCC 13+ required for C++23 support")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16.0")
        message(FATAL_ERROR "Clang 16+ required for C++23 support")
    endif()
endif()

# Build type default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

# Find SDL2 packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Fetch Catch2 for testing
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

# Source files
set(SOURCES
    src/main.cpp
    src/core/application.cpp
    src/core/logging/error_logger.cpp
    # Phase 3: Core game systems
    src/persistence/markdown/level_parser.cpp
    src/core/physics/physics_engine.cpp
    src/core/level/completion_checker.cpp
    src/core/game/game_state_manager.cpp
    # Phase 3: SDL platform implementations
    src/sdl/renderer/sdl_renderer.cpp
    src/sdl/input/sdl_input_handler.cpp
    src/sdl/audio/sdl_audio_player.cpp
    # Phase 3: UI screens
    src/ui/gameplay/gameplay_screen.cpp
    # Phase 4: Main menu and navigation screens
    src/ui/menu/main_menu_screen.cpp
    src/ui/level_select/level_select_screen.cpp
    src/ui/trophies/trophies_screen.cpp
    src/ui/settings/settings_screen.cpp
    src/ui/level_editor/level_editor_screen.cpp
    src/ui/other_games/other_games_screen.cpp
    # Core entities are header-only structs (Phase 2 - Foundational complete)
)

# Create executable
add_executable(wrecking-ball ${SOURCES})

# Include directories
target_include_directories(wrecking-ball PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party
    ${SDL2_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(wrecking-ball PRIVATE
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)

# Installation rules
install(TARGETS wrecking-ball
    RUNTIME DESTINATION bin
)

install(DIRECTORY assets/
    DESTINATION share/wrecking-ball/assets
)

# Testing
enable_testing()
include(CTest)

# Core entity tests
add_executable(entity_tests
    tests/unit/entities/brick_test.cpp
    tests/unit/entities/ball_test.cpp
    tests/unit/entities/paddle_test.cpp
    tests/unit/entities/level_test.cpp
)
target_link_libraries(entity_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(entity_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME EntityTests COMMAND entity_tests)

# Physics tests (collision detection)
add_executable(physics_tests
    tests/unit/physics/collision_narrow_test.cpp
    tests/unit/physics/collision_wide_test.cpp
    tests/unit/physics/collision_fullwidth_test.cpp
    tests/unit/physics/paddle_bounce_test.cpp
)
target_link_libraries(physics_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(physics_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME PhysicsTests COMMAND physics_tests)

# Level completion tests
add_executable(level_tests
    tests/unit/level/classical_completion_test.cpp
    tests/unit/level/star_completion_test.cpp
    tests/unit/level/key_completion_test.cpp
    tests/unit/level/exit_activation_test.cpp
)
target_link_libraries(level_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(level_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME LevelTests COMMAND level_tests)

# Parser tests
add_executable(parser_tests
    tests/unit/persistence/level_parser_valid_test.cpp
    tests/unit/persistence/level_parser_malformed_test.cpp
    src/persistence/markdown/level_parser.cpp
    src/core/logging/error_logger.cpp
)
target_link_libraries(parser_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(parser_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_test(NAME ParserTests COMMAND parser_tests)

# Enable sanitizers in debug mode (optional)
option(ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(wrecking-ball PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
    target_link_options(wrecking-ball PRIVATE
        -fsanitize=address,undefined
    )
endif()

# Print configuration summary
message(STATUS "=== Wrecking Ball Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "SDL2 found: ${SDL2_FOUND}")
message(STATUS "SDL2_ttf found: ${SDL2_ttf_FOUND}")
message(STATUS "SDL2_mixer found: ${SDL2_mixer_FOUND}")
message(STATUS "Sanitizers: ${ENABLE_SANITIZERS}")
message(STATUS "===================================")

// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/persistence/markdown/level_parser.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <filesystem>

using namespace wreckingball;

// Helper to create temp test file
static std::string CreateTempLevelFile(const std::string& content) {
    std::string temp_path = "/tmp/test_level_" + std::to_string(std::time(nullptr)) + ".md";
    std::ofstream file(temp_path);
    file << content;
    file.close();
    return temp_path;
}

// Helper to clean up temp file
static void DeleteTempFile(const std::string& path) {
    std::filesystem::remove(path);
}

TEST_CASE("LevelParser loads valid Classical level", "[level_parser][valid]") {
    std::string content = R"(---
level_id: 1
name: Test Classical
level_type: Classical
difficulty: Easy
author: Test Author
description: A test level
version: 1.0
tags: test, classical
---

```brick-layout
R----- R----- R----- R----- R----- R----- R-
R----- R----- R----- R----- R----- R----- R-
R----- R----- R----- R----- R----- R----- R-
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE(level.has_value());
    REQUIRE(level->level_id == 1);
    REQUIRE(level->name == "Test Classical");
    REQUIRE(level->level_type == LevelType::Classical);
    REQUIRE(level->difficulty_str == "Easy");
    REQUIRE(level->author == "Test Author");
    REQUIRE(level->description == "A test level");
    REQUIRE(level->version == "1.0");
    REQUIRE(level->tags == "test, classical");
    REQUIRE(level->bricks.size() == 21);  // 3 rows × 7 bricks per row

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser loads valid ReachForStars level", "[level_parser][valid]") {
    std::string content = R"(---
level_id: 2
name: Star Collector
level_type: ReachForStars
difficulty: Medium
---

```brick-layout
R----- R----- R----- R----- R----- R----- R-
R----- S-- R----- S-- R----- S-- R----- R-
R----- R----- R----- R----- R----- R----- R-
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE(level.has_value());
    REQUIRE(level->level_type == LevelType::ReachForStars);

    // Count star bricks
    int star_count = 0;
    for (const auto& brick : level->bricks) {
        if (brick.type == BrickType::Star) {
            star_count++;
        }
    }
    REQUIRE(star_count >= 1);  // Must have at least one star

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser loads valid GetTheKey level", "[level_parser][valid]") {
    std::string content = R"(---
level_id: 3
name: Key Master
level_type: GetTheKey
difficulty: Hard
---

```brick-layout
R----- R----- R----- R----- R----- R----- R-
R----- K-- R----- R----- R----- R----- R-
R----- R----- R----- R----- R----- E--- R-
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE(level.has_value());
    REQUIRE(level->level_type == LevelType::GetTheKey);

    // Count key and exit bricks
    int key_count = 0;
    int exit_count = 0;
    for (const auto& brick : level->bricks) {
        if (brick.type == BrickType::Key) key_count++;
        if (brick.type == BrickType::Exit) exit_count++;
    }
    REQUIRE(key_count >= 1);  // Must have at least one key
    REQUIRE(exit_count >= 1); // Must have at least one exit

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser handles variable brick widths", "[level_parser][valid]") {
    std::string content = R"(---
level_id: 4
name: Width Test
level_type: Classical
difficulty: Easy
---

```brick-layout
R R- R--- R--------- R------------------- R
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE(level.has_value());
    REQUIRE(level->bricks.size() == 6);

    // Check brick widths (1, 2, 4, 10, 20, 1)
    REQUIRE(level->bricks[0].width == 1);
    REQUIRE(level->bricks[1].width == 2);
    REQUIRE(level->bricks[2].width == 4);
    REQUIRE(level->bricks[3].width == 10);
    REQUIRE(level->bricks[4].width == 20);  // R------------------- = 20 chars
    REQUIRE(level->bricks[5].width == 1);

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser validates level after loading", "[level_parser][valid]") {
    std::string content = R"(---
level_id: 5
name: Validation Test
level_type: Classical
difficulty: Easy
---

```brick-layout
R----- R----- R----- R----- R----- R----- R-
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE(level.has_value());
    REQUIRE(parser.ValidateLevel(*level));

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser handles empty spaces", "[level_parser][valid]") {
    std::string content = R"(---
level_id: 6
name: Sparse Layout
level_type: Classical
difficulty: Easy
---

```brick-layout
R----- ..... R----- ..... R----- ..... R-
..... R----- ..... R----- ..... R----- R-
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE(level.has_value());
    // Empty spaces (.) should not create bricks
    REQUIRE(level->bricks.size() < 16);  // Less than if all were bricks

    DeleteTempFile(temp_path);
}

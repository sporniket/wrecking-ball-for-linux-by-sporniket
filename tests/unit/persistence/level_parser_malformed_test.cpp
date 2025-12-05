// Copyright (c) 2025 David SPORN
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "../../../src/persistence/markdown/level_parser.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <filesystem>

using namespace wreckingball;

// Helper to create temp test file
static std::string CreateTempLevelFile(const std::string& content) {
    std::string temp_path = "/tmp/test_level_malformed_" + std::to_string(std::time(nullptr)) + ".md";
    std::ofstream file(temp_path);
    file << content;
    file.close();
    return temp_path;
}

// Helper to clean up temp file
static void DeleteTempFile(const std::string& path) {
    std::filesystem::remove(path);
}

TEST_CASE("LevelParser rejects missing front matter", "[level_parser][malformed]") {
    std::string content = R"(```brick-layout
R---- R---- R---- R---- R---- R---- R---- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE_FALSE(level.has_value());  // Should fail to parse

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects missing brick layout", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: 1
name: Test Level
level_type: Classical
difficulty: Easy
---
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE_FALSE(level.has_value());  // Should fail without brick layout

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects invalid level type", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: 1
name: Test Level
level_type: InvalidType
difficulty: Easy
---

```brick-layout
R---- R---- R---- R---- R---- R---- R---- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE_FALSE(level.has_value());  // Should reject invalid type

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects ReachForStars without star bricks", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: 2
name: No Stars
level_type: ReachForStars
difficulty: Easy
---

```brick-layout
R---- R---- R---- R---- R---- R---- R---- R----
R---- R---- R---- R---- R---- R---- R---- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    // Level may parse, but validation should fail
    if (level.has_value()) {
        REQUIRE_FALSE(parser.ValidateLevel(*level));  // Should fail validation
    }

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects GetTheKey without key brick", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: 3
name: No Key
level_type: GetTheKey
difficulty: Easy
---

```brick-layout
R---- R---- R---- R---- R---- R---- R---- R----
R---- R---- R---- R---- R---- E--- R---- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    // Level may parse, but validation should fail
    if (level.has_value()) {
        REQUIRE_FALSE(parser.ValidateLevel(*level));  // Should fail validation
    }

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects GetTheKey without exit brick", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: 3
name: No Exit
level_type: GetTheKey
difficulty: Easy
---

```brick-layout
R---- R---- R---- R---- R---- R---- R---- R----
R---- K-- R---- R---- R---- R---- R---- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    // Level may parse, but validation should fail
    if (level.has_value()) {
        REQUIRE_FALSE(parser.ValidateLevel(*level));  // Should fail validation
    }

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects invalid brick codes", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: 4
name: Invalid Codes
level_type: Classical
difficulty: Easy
---

```brick-layout
R---- X---- Y---- Z---- R---- R---- R---- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    // Parser should reject invalid brick codes
    REQUIRE_FALSE(level.has_value());

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects line too wide", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: 5
name: Too Wide
level_type: Classical
difficulty: Easy
---

```brick-layout
R-------------------------------------------- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    // Level may parse, but validation should fail (exceeds 38 units)
    if (level.has_value()) {
        REQUIRE_FALSE(parser.ValidateLevel(*level));
    }

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects non-existent file", "[level_parser][malformed]") {
    LevelParser parser;
    auto level = parser.LoadLevel("/tmp/nonexistent_level_file_xyz123.md");

    REQUIRE_FALSE(level.has_value());  // Should fail to load
}

TEST_CASE("LevelParser handles empty file gracefully", "[level_parser][malformed]") {
    std::string content = "";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE_FALSE(level.has_value());  // Should fail on empty file

    DeleteTempFile(temp_path);
}

TEST_CASE("LevelParser rejects malformed YAML", "[level_parser][malformed]") {
    std::string content = R"(---
level_id: not a number
name: Test Level
level_type: Classical
difficulty: Easy
---

```brick-layout
R---- R---- R---- R---- R---- R---- R---- R----
```
)";

    std::string temp_path = CreateTempLevelFile(content);

    LevelParser parser;
    auto level = parser.LoadLevel(temp_path);

    REQUIRE_FALSE(level.has_value());  // Should fail on invalid YAML

    DeleteTempFile(temp_path);
}

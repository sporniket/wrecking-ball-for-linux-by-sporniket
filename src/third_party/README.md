# Third-Party Dependencies

This directory contains vendored third-party libraries.

## inih (INI Not Invented Here)

**Version**: r56 or later
**License**: BSD 3-Clause
**Source**: https://github.com/benhoyt/inih

### Manual Installation Instructions

Until automated fetching is implemented, manually download and place inih files here:

```bash
# From repository root
cd src/third_party/

# Create inih directory
mkdir -p inih

# Download inih files (r56 or later)
wget https://raw.githubusercontent.com/benhoyt/inih/master/ini.h -O inih/ini.h
wget https://raw.githubusercontent.com/benhoyt/inih/master/ini.c -O inih/ini.c
wget https://raw.githubusercontent.com/benhoyt/inih/master/cpp/INIReader.h -O inih/INIReader.h
wget https://raw.githubusercontent.com/benhoyt/inih/master/cpp/INIReader.cpp -O inih/INIReader.cpp

# Add copyright notice to each file (prepend to existing content)
# The files already contain BSD 3-Clause license headers
```

### Required Files

- `inih/ini.h` - C header
- `inih/ini.c` - C implementation
- `inih/INIReader.h` - C++ wrapper header
- `inih/INIReader.cpp` - C++ wrapper implementation

### CMake Integration

Once files are in place, add to CMakeLists.txt:

```cmake
# inih library
add_library(inih STATIC
    src/third_party/inih/ini.c
    src/third_party/inih/INIReader.cpp
)
target_include_directories(inih PUBLIC src/third_party/inih)

# Link to main executable
target_link_libraries(wrecking-ball PRIVATE inih)
```

### Usage in Code

```cpp
#include "inih/INIReader.h"

INIReader reader("path/to/file.ini");
if (reader.ParseError() < 0) {
    // Handle error
}

std::string value = reader.Get("section", "key", "default");
int number = reader.GetInteger("section", "number_key", 0);
```

---

## Future Dependencies

Additional third-party libraries will be added as implementation progresses.

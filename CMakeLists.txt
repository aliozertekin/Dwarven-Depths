cmake_minimum_required(VERSION 3.15)

# Set the project name and version
project(DwarvenDepths VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB_RECURSE SOURCES
    "Dwarven Depths/Dwarf.cpp"
    "Dwarven Depths/Enemy.cpp"
    "Dwarven Depths/Game.cpp"
    "Dwarven Depths/Renderer.cpp"
    "Dwarven Depths/Utils.cpp"
    "Dwarven Depths/Main.cpp"
    "Dwarven Depths/Dwarf.h"
    "Dwarven Depths/Enemy.h"
    "Dwarven Depths/Game.h"
    "Dwarven Depths/Renderer.h"
    "Dwarven Depths/Utils.h"
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable
add_executable(DwarvenDepths ${SOURCES})

# Add libraries if required (example for threading)
find_package(Threads REQUIRED)
target_link_libraries(DwarvenDepths PRIVATE Threads::Threads)

# Enable extra compiler warnings (optional)
if (MSVC)
    target_compile_options(DwarvenDepths PRIVATE /W4 /WX)
else()
    target_compile_options(DwarvenDepths PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

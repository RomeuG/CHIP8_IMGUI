cmake_minimum_required(VERSION 3.13)
project(CHIP8_IMGUI)

set(CMAKE_CXX_STANDARD 17)

# compiler
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -ggdb3 -O0 -lsfml-graphics -lsfml-window -lsfml-system -lGL")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -msse4.2 -mavx2 -march=native -fno-exceptions -fno-rtti -lsfml-graphics -lsfml-window -lsfml-system -lGL")

# debug print
add_definitions(-DDEBUG)

# compilation db
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(CHIP8_IMGUI src/main.cpp)

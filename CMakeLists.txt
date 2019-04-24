cmake_minimum_required(VERSION 3.13)
project(CHIP8_IMGUI)

set(CMAKE_CXX_STANDARD 17)

# compiler
set(CMAKE_CXX_COMPILER clang++)

# IMGUI
include_directories(lib/imgui)

# include
include_directories(include)

# linker flags
set(LIBSTDCXX_FS_LIBRARIES -lstdc++fs)
set(CMAKE_CXX_FLAGS "-ggdb3 -O0 -Wno-unused-command-line-argument -lsfml-graphics -lsfml-window -lsfml-system -lGL")

# debug print
add_definitions(-DDEBUG)

# compilation db
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(CHIP8_IMGUI
  lib/imgui/imgui.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/imgui_demo.cpp

  include/imgui-SFML.cpp

  src/gui.cpp
  src/chip8.cpp
  src/disasm.cpp
  src/logging.cpp
  src/main.cpp)

target_link_libraries(CHIP8_IMGUI ${LIBSTDCXX_FS_LIBRARIES})

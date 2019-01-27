cmake_minimum_required(VERSION 3.13)
project(CHIP8_IMGUI)

set(CMAKE_CXX_STANDARD 17)

# IMGUI
include_directories(lib/imgui)

# gl3w
add_subdirectory(lib/gl3w)
include_directories(lib/gl3w)

# include
include_directories(include)

# linker flags
set(LIBSTDCXX_FS_LIBRARIES -lstdc++fs)
set(CMAKE_CXX_FLAGS "-ggdb3 -O0 -lSDL2 -ldl")

# debug print
add_definitions(-DDEBUG)

# compilation db
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(CHIP8_IMGUI src/main.cpp
	    include/imgui_impl_opengl3.cpp
		include/imgui_impl_opengl3.h
		include/imgui_impl_sdl.cpp
		include/imgui_impl_sdl.h

		# imgui (TODO)
		lib/imgui/imgui.cpp
		lib/imgui/imgui_draw.cpp
		lib/imgui/imgui_widgets.cpp
		lib/imgui/imgui_demo.cpp
		lib/imgui/imgui.h
		lib/imgui/imstb_rectpack.h
		lib/imgui/imstb_textedit.h
		lib/imgui/imstb_truetype.h
		include/chip8.hpp
		include/gui.hpp
		include/disasm.hpp
		src/gui.cpp
		src/chip8.cpp
		src/disasm.cpp
		include/imgui_memory_editor.h)

	  target_link_libraries(CHIP8_IMGUI Gl3w ${LIBSTDCXX_FS_LIBRARIES})

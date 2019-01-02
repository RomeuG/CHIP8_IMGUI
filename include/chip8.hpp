#ifndef CHIP8_IMGUI_CHIP8_H
#define CHIP8_IMGUI_CHIP8_H

#include <array>
#include <cstdint>

namespace Constants {
constexpr std::size_t MEMORY_SIZE = 0x1000;
};

struct chip8 {
	std::array<std::uint8_t, Constants::MEMORY_SIZE> memory{0};
};

#endif //CHIP8_IMGUI_CHIP8_H

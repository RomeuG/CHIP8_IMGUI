#ifndef CHIP8_IMGUI_CHIP8_H
#define CHIP8_IMGUI_CHIP8_H

#include <array>
#include <vector>
#include <cstdint>

namespace Constants {
static constexpr std::size_t CH8_MEMORY_SIZE = 0x1000;
static constexpr std::size_t CH8_GFX_SIZE = 0x800;

static constexpr std::size_t CH8_REG_SIZE = 0x10;
static constexpr std::size_t CH8_STACK_SIZE = 0x10;
static constexpr std::size_t CH8_KEY_SIZE = 0x10;
static constexpr std::size_t CH8_FONT_SIZE = 0x10;

static constexpr std::array<std::uint8_t, Constants::CH8_MEMORY_SIZE> font_list = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

};

struct chip8 {
	std::uint16_t I{0};
	std::uint16_t pc{0x200};
	std::uint16_t sp{0};
	std::uint16_t opcode{0};

	std::uint8_t delay_timer{0};
	std::uint8_t sound_timer{0};

	std::array<std::uint8_t, Constants::CH8_MEMORY_SIZE> memory{0};
	std::array<std::uint8_t, Constants::CH8_GFX_SIZE> graphics{0};
	std::array<std::uint8_t, Constants::CH8_REG_SIZE> V{0};
	std::array<std::uint8_t, Constants::CH8_KEY_SIZE> keys{0};
	std::array<std::uint16_t, Constants::CH8_STACK_SIZE> stack{0};

	chip8()
	{
		std::copy(Constants::font_list.begin(), Constants::font_list.end(), memory.begin());
	}
};

#endif //CHIP8_IMGUI_CHIP8_H

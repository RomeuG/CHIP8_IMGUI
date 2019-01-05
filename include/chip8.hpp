#ifndef CHIP8_IMGUI_CHIP8_H
#define CHIP8_IMGUI_CHIP8_H

#include <array>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>

namespace Constants {
static constexpr std::size_t ROM_LOCATION = 0x200;

static constexpr std::size_t CH8_MEMORY_SIZE = 0x1000;
static constexpr std::size_t CH8_GFX_SIZE = 0x800;

static constexpr std::size_t CH8_REG_SIZE = 0x10;
static constexpr std::size_t CH8_STACK_SIZE = 0x10;
static constexpr std::size_t CH8_KEY_SIZE = 0x10;
static constexpr std::size_t CH8_FONT_SIZE = 0x10;

static constexpr std::array<std::uint8_t, Constants::CH8_MEMORY_SIZE> FONT_LIST = {
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
		load_font();
		std::unordered_map<unsigned short int, std::function<void(void)>> _al{
				{0x0000, []() { return; }},
				{0x1000, [this]() { return; }},
				{0x2000, [this]() { return; }},
				{0x3000, [this]() { return; }},
				{0x4000, [this]() { return; }},
				{0x5000, [this]() { return; }},
				{0x6000, [this]() { return; }},
				{0x7000, [this]() { return; }},
				{0x8000, [this]() { return; }},
				{0xA000, [this]() { return; }},
				{0xB000, [this]() { return; }},
				{0xC000, [this]() { return; }},
				{0xD000, [this]() { return; }},
				{0xE000, [this]() { return; }},
				{0xF000, [this]() { return; }},
		};
	}

	inline void load_font()
	{
		std::copy(Constants::FONT_LIST.begin(), Constants::FONT_LIST.end(), memory.begin());
	}

	inline void load_rom(std::vector<char>&& buffer)
	{
		auto it = memory.begin();
		std::advance(it, Constants::ROM_LOCATION);
		std::copy(buffer.begin(), buffer.end(), it);
	}
};

#endif //CHIP8_IMGUI_CHIP8_H

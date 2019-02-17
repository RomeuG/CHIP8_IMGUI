#ifndef CHIP8_IMGUI_CHIP8_H
#define CHIP8_IMGUI_CHIP8_H

#include <array>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <functional>
#include <random>
#include <iostream>

#include <SDL2/SDL.h>

#include "logging.hpp"

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

static constexpr unsigned int sdl_keymap[16] = {
		SDLK_1, SDLK_2, SDLK_3, SDLK_4,
		SDLK_q, SDLK_w, SDLK_e, SDLK_r,
		SDLK_a, SDLK_s, SDLK_d, SDLK_f,
		SDLK_z, SDLK_x, SDLK_c, SDLK_v
};

};

// DEBUG PRINT
#ifdef DEBUG
#define DEBUG_PRINT(output, fmt, ...)                                    \
    do { fprintf(output, "%s:%d:%s(): " fmt, __FILE__,        \
                            __LINE__, __func__, __VA_ARGS__);fflush(stdout);fflush(stderr); } while (0)
#else
#define DEBUG_PRINT(output, fmt, ...) ((void)0)
#endif

struct chip8 {
	Logging* logger;
	SDL_Surface* screen;

	std::string file_name{0};
	std::uint32_t file_size{0};
	std::vector<std::string> disassembly;

	std::uint16_t I{0};
	std::uint16_t pc{0x200};
	std::uint16_t sp{0};
	std::uint16_t opcode{0};

	std::uint8_t delay_timer{0};
	std::uint8_t sound_timer{0};

	bool draw_flag{false};
	bool draw_flag_blocked{false};

	std::array<std::uint8_t, Constants::CH8_MEMORY_SIZE> memory{0};
	std::array<std::uint8_t, Constants::CH8_GFX_SIZE> graphics{0};
	std::array<std::uint8_t, Constants::CH8_REG_SIZE> V{0};
	std::array<std::uint8_t, Constants::CH8_KEY_SIZE> keys{0};
	std::array<std::uint16_t, Constants::CH8_STACK_SIZE> stack{0};

	std::unordered_map<unsigned short int, std::function<void(void)>> instruction_table{
			{0x0000, [this]() {
			  switch (opcode & 0x000F) {
				  case 0x0: {
					  std::fill(graphics.begin(), graphics.end(), 0);
					  pc += 2;

					  break;
				  }
				  case 0xE: {
					  pc = stack[--sp];
					  break;
				  }
				  default: break;
			  }
			}},
			{0x1000, [this]() {
			  pc = opcode & 0x00FF;
			}},

			{0x2000, [this]() {
			  stack[sp++] = pc;
			  pc = opcode & 0x0FFF;
			}},
			{0x3000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;
			  auto _nn = (opcode & 0x00FF);

			  if (V[_x] == _nn) { pc += 2; }
			  pc += 2;
			}},
			{0x4000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;
			  auto _nn = (opcode & 0x00FF);

			  if (V[_x] != _nn) { pc += 2; }
			  pc += 2;
			}},
			{0x5000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;
			  auto _y = (opcode & 0x00F0) >> 4;

			  if (V[_x] == V[_y]) { pc += 2; }
			  pc += 2;
			}},
			{0x6000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;
			  auto _nn = (opcode & 0x00FF);

			  V[_x] = _nn;
			  pc += 2;
			}},
			{0x7000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;
			  auto _nn = (opcode & 0x00FF);

			  V[_x] += _nn;
			  pc += 2;
			}},
			{0x8000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;
			  auto _y = (opcode & 0x00F0) >> 4;

			  switch (opcode & 0x000F) {
				  case 0x0: {
					  V[_x] = V[_y];
					  break;
				  }
				  case 0x1: {
					  V[_x] |= V[_y];
					  break;
				  }
				  case 0x2: {
					  V[_x] &= V[_y];
					  break;
				  }
				  case 0x3: {
					  V[_x] ^= V[_y];
					  break;
				  }
				  case 0x4: {
					  // TODO: verify 'V[_x] -= 0x100;'
					  V[_x] += V[_y];
					  if (V[_x] > 255) {
						  V[0xF] = 1;
						  V[_x] -= 0x100;
					  }
					  else { V[0xF] = 0; }
					  break;
				  }
				  case 0x5: {
					  if (V[_x] > V[_y]) {
						  V[0xF] = 1;
					  }
					  else {
						  V[0xF] = 0;
					  }

					  V[_x] -= V[_y];
					  if (V[_x] < 0) { V[_x] += 0x100; }
					  break;
				  }
				  case 0x6: {
					  V[0xF] = V[_x] & 0x1;
					  V[_x] >>= 1;
					  break;
				  }
				  case 0x7: {
					  if (V[_y] > V[_x]) { V[0xF] = 1; }
					  else { V[0xF] = 0; }

					  V[_x] = V[_y] - V[_x];
					  if (V[_x] < 0) { V[_x] += 0x100; }

					  break;
				  }
				  case 0xE: {
					  V[0xF] = V[_x] & 0x80;
					  V[_x] <<= 1;
					  if (V[_x] > 0xFF) { V[_x] -= 0x100; }
					  break;
				  }

				  default: break;
			  }

			  pc += 2;
			}},
			{0xA000, [this]() {
			  I = opcode & 0x0FFF;
			  pc += 2;
			}},
			{0xB000, [this]() {
			  pc = (opcode & 0x0FFF) + V[0x0];
			}},
			{0xC000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;

			  std::default_random_engine rand_eng;
			  std::uniform_int_distribution<std::uint8_t> distribution(0x0, 0xFF);

			  V[_x] = distribution(rand_eng) & (opcode & 0x00FF);
			  pc += 2;
			}},
			{0xD000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;
			  auto _y = (opcode & 0x00F0) >> 4;
			  auto _n = (opcode & 0x000F);

			  V[0xF] &= 0x0;
			  // TODO: draw stuff
			  for (auto i = 0; i < _n; i++) {
				  auto pixel = memory[I + i];

				  for (auto j = 0; j < 8; j++) {
					  if (pixel & (0x80 >> j)) {
						  if (graphics[j + V[_x] + (i + V[_y]) * 64]) {
							  V[0xF] = 1;
						  }

						  graphics[j + V[_x] + (i + V[_y]) * 64] ^= 1;
					  }
				  }
			  }

			  //if (!draw_flag_blocked) draw_flag = true;
			  draw_flag = true;
			  pc += 2;
			}},
			{0xE000, [this]() {
			  // TODO: keypad stuff
			}},
			{0xF000, [this]() {
			  auto _x = (opcode & 0x0F00) >> 8;

			  switch (opcode & 0x00FF) {
				  case 0x07: {
					  V[_x] = delay_timer;
					  break;
				  }
				  case 0x0A: {
					  // TODO: keypad stuff
					  break;
				  }
				  case 0x15: {
					  delay_timer = V[_x];
					  break;
				  }
				  case 0x18: {
					  sound_timer = V[_x];
					  break;
				  }
				  case 0x1E: {
					  I += V[_x];
					  break;
				  }
				  case 0x29: {
					  I = V[_x] * 5;
					  break;
				  }
				  case 0x33: {
					  // TODO: check
					  memory[I] = V[_x] / 100;
					  memory[I + 1] = (V[_x] / 10) % 10;
					  memory[I + 2] = (V[_x] % 100) % 10;
#if 0
					  memory[I] = V[_x] / 100;
					  memory[I + 1] = (V[_x] / 10) % 10;
					  memory[I + 2] = V[_x] % 10;
#endif

					  break;
				  }
				  case 0x55: {
					  for (int i = 0; i <= _x; i++) {
						  memory[I + i] = V[i];
					  }
					  break;
				  }
				  case 0x65: {
					  for (int i = 0; i <= _x; i++) {
						  V[i] = memory[I + i];
					  }
					  break;
				  }
				  default: break;
			  }

			  pc += 2;
			}},
	};

	chip8();

	void load_font();
	void load_rom(char* rom_name);
	void cycle();
};

#endif //CHIP8_IMGUI_CHIP8_H

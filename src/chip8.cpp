#include <filesystem>
#include <fstream>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

#define LOG_INFO "info"
#define LOG_WARN "warn"
#define LOG_CRIT "crit"

// DEBUG PRINT
#ifdef DEBUG
#define DEBUG_PRINT(output, fmt, ...)                                     \
    do {                                                                  \
        fprintf(output, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, \
                __VA_ARGS__);                                             \
        fflush(stdout);                                                   \
        fflush(stderr);                                                   \
    } while (0)
#else
#define DEBUG_PRINT(output, fmt, ...) ((void)0)
#endif

struct Chip8 {
    Logging* logger;

    sf::RenderWindow* window;
    sf::Texture texture;

    std::string file_name{ 0 };
    std::uint32_t file_size{ 0 };
    std::vector<std::string> disassembly;

    std::uint16_t I{ 0 };
    std::uint16_t pc{ 0x200 };
    std::uint16_t sp{ 0 };
    std::uint16_t opcode{ 0 };

    std::uint8_t delay_timer{ 0 };
    std::uint8_t sound_timer{ 0 };

    bool draw_flag{ false };
    bool draw_flag_blocked{ false };

    std::array<std::uint8_t, CONSTANTS::CH8_MEMORY_SIZE> memory{ 0 };
    std::array<std::uint8_t, CONSTANTS::CH8_GFX_SIZE> graphics{ 0 };
    std::array<std::uint8_t, CONSTANTS::CH8_REG_SIZE> V{ 0 };
    std::array<std::uint8_t, CONSTANTS::CH8_KEY_SIZE> keys{ 0 };
    std::array<std::uint16_t, CONSTANTS::CH8_STACK_SIZE> stack{ 0 };

    std::unordered_map<unsigned short int, std::function<void(void)>>
        instruction_table{
            { 0x0000,
              [this]() {
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
                      default:
                          break;
                  }
              } },
            { 0x1000, [this]() { pc = opcode & 0x0FFF; } },

            { 0x2000,
              [this]() {
                  stack[sp++] = pc + 2;
                  pc = opcode & 0x0FFF;
              } },
            { 0x3000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _nn = (opcode & 0x00FF);

                  if (V[_x] == _nn) {
                      pc += 2;
                  }
                  pc += 2;
              } },
            { 0x4000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _nn = (opcode & 0x00FF);

                  if (V[_x] != _nn) {
                      pc += 2;
                  }
                  pc += 2;
              } },
            { 0x5000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _y = (opcode & 0x00F0) >> 4;

                  if (V[_x] == V[_y]) {
                      pc += 2;
                  }
                  pc += 2;
              } },
            { 0x6000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _nn = (opcode & 0x00FF);

                  V[_x] = _nn;
                  pc += 2;
              } },
            { 0x7000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _nn = (opcode & 0x00FF);

                  V[_x] += _nn;
                  pc += 2;
              } },
            { 0x8000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _y = (opcode & 0x00F0) >> 4;

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
                          if (V[_x] + V[_y] > 255) {
                              V[0xF] = 1;
                          } else {
                              V[0xF] = 0;
                          }

                          V[_x] += V[_y];

                          break;
                      }
                      case 0x5: {
                          if (V[_x] > V[_y]) {
                              V[0xF] = 1;
                          } else {
                              V[0xF] = 0;
                          }

                          V[_x] -= V[_y];

                          break;
                      }
                      case 0x6: {
                          V[0xF] = (V[_x] & 0x01);
                          V[_x] = V[_x] >> 1;
                          break;
                      }
                      case 0x7: {
                          if (V[_y] > V[_x]) {
                              V[0xF] = 1;
                          } else {
                              V[0xF] = 0;
                          }

                          V[_x] = V[_y] - V[_x];

                          break;
                      }
                      case 0xE: {
                          V[0xF] = (V[_x] >> 7);
                          V[_x] = V[_x] << 1;
                          break;
                      }

                      default:
                          break;
                  }

                  pc += 2;
              } },
            { 0x9000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _y = (opcode & 0x00F0) >> 4;

                  if (V[_x] != V[_y]) {
                      pc += 2;
                  }
                  pc += 2;
              } },
            { 0xA000,
              [this]() {
                  I = opcode & 0x0FFF;
                  pc += 2;
              } },
            { 0xB000, [this]() { pc = (opcode & 0x0FFF) + V[0x0]; } },
            { 0xC000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;

                  std::default_random_engine rand_eng;
                  std::uniform_int_distribution<std::uint8_t> distribution(
                      0x0, 0xFF);

                  V[_x] = distribution(rand_eng) & (opcode & 0x00FF);
                  pc += 2;
              } },
            { 0xD000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;
                  auto const _y = (opcode & 0x00F0) >> 4;
                  auto const _n = (opcode & 0x000F);

                  V[0xF] &= 0x0;

                  for (auto i = 0; i < _n; i++) {
                      auto const pixel = memory[I + i];

                      for (auto j = 0; j < 8; j++) {
                          if (pixel & (0x80 >> j)) {
                              auto const idx = j + V[_x] + (i + V[_y]) * 64;

                              if (graphics[idx]) {
                                  V[0xF] = 1;
                              }

                              graphics[idx] ^= 1;
                          }
                      }
                  }

                  if (!draw_flag_blocked)
                      draw_flag = true;
                  pc += 2;
              } },
            { 0xE000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;

                  switch (opcode & 0x00FF) {
                      case 0x9E:
                          if (sf::Keyboard::isKeyPressed(
                                  CONSTANTS::sdl_keymap[V[_x]])) {
                              pc += 4;
                          } else {
                              pc += 2;
                          }
                          break;
                      case 0xA1:
                          if (!sf::Keyboard::isKeyPressed(
                                  CONSTANTS::sdl_keymap[V[_x]])) {
                              pc += 4;
                          } else {
                              pc += 2;
                          }
                          break;
                      default:
                          break;
                  }
              } },
            { 0xF000,
              [this]() {
                  auto const _x = (opcode & 0x0F00) >> 8;

                  switch (opcode & 0x00FF) {
                      case 0x07: {
                          V[_x] = delay_timer;
                          break;
                      }
                      case 0x0A: {
                          for (auto i = 0; i < CONSTANTS::CH8_KEY_SIZE; i++) {
                              if (sf::Keyboard::isKeyPressed(
                                      CONSTANTS::sdl_keymap[i])) {
                                  V[_x] = i;
                                  pc += 2;
                              }
                          }
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
                          if (I + V[_x] > 0xFFF) {
                              V[0xF] = 1;
                          } else {
                              V[0xF] = 0;
                          }
                          I += V[_x];
                          break;
                      }
                      case 0x29: {
                          I = V[_x] * 5;
                          break;
                      }
                      case 0x33: {
                          memory[I] = V[_x] / 100;
                          memory[I + 1] = (V[_x] / 10) % 10;
                          memory[I + 2] = V[_x] % 10;

                          break;
                      }
                      case 0x55: {
                          for (auto i = 0; i <= _x; i++) {
                              memory[I + i] = V[i];
                          }
                          break;
                      }
                      case 0x65: {
                          for (auto i = 0; i <= _x; i++) {
                              V[i] = memory[I + i];
                          }
                          break;
                      }
                      default:
                          break;
                  }

                  pc += 2;
              } },
        };

    Chip8();

    auto load_font() -> void;
    auto load_rom(std::string_view rom_name) -> void;
    auto cycle() -> void;
    auto reset() -> void;
};

Chip8::Chip8()
{
    // initialize logger
    logger = Logging::get_instance();

    // init font
    load_font();

    // init texture
    texture.create(64, 32);
}

auto Chip8::load_font() -> void
{
    std::copy(CONSTANTS::FONT_LIST.begin(), CONSTANTS::FONT_LIST.end(),
              memory.begin());
}

auto Chip8::load_rom(std::string_view rom_name) -> void
{
    // read file
    std::ifstream f(rom_name.data(), std::ios::binary);
    std::vector<char> v(std::istreambuf_iterator<char>{ f }, {});

    // file size
    file_size = std::filesystem::file_size(rom_name);

    // load into memory
    auto it = memory.begin();
    std::advance(it, CONSTANTS::ROM_LOCATION);
    std::copy(v.begin(), v.end(), it);

    // disassemble rom
    std::uint32_t pc = 0x0;
    while (pc < (file_size)) {
        auto disasm = disasm_opcode(v[pc], v[pc + 1], pc);
        disassembly.push_back(disasm);
        pc += 2;
    }
}

auto Chip8::cycle() -> void
{
    opcode = (memory[pc] << 8 | memory[pc + 1]);

    // reset timers
    if (delay_timer > 0)
        delay_timer--;
    if (sound_timer > 0)
        sound_timer--;
    if (sound_timer == 0)
        ; //beep;

    auto const instruction = (std::uint16_t)(opcode & 0xF000);
    auto const a = instruction_table.find(instruction);
    if (a == instruction_table.cend()) {
        //DEBUG_PRINT(stdout, "%s\n", "Unknown instruction.");
        logger->add_log("[%05d] [%s] Opcode: %s\n", ImGui::GetFrameCount(),
                        LOG_WARN, "Unknown");
    } else {
        logger->add_log("[%05d] [%s] Opcode: 0x%02X\n", ImGui::GetFrameCount(),
                        LOG_INFO, instruction);
        (a->second)();
    }

    // draw flag events
    if (draw_flag) {
        draw_flag = false;
    }
}

auto Chip8::reset() -> void
{
    this->I = 0;
    this->pc = 0;
    this->sp = 0;
    this->opcode = 0;

    this->delay_timer = 0;
    this->sound_timer = 0;

    this->memory.fill(0);
    this->graphics.fill(0);
    this->V.fill(0);
    this->stack.fill(0);
    this->keys.fill(0);

    this->load_font();
}

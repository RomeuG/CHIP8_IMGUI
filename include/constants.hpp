#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <iostream>

namespace CONSTANTS
{
static constexpr std::size_t ROM_LOCATION = 0x200;

static constexpr std::size_t CH8_MEMORY_SIZE = 0x1000;
static constexpr std::size_t CH8_GFX_SIZE = 0x800;

static constexpr std::size_t CH8_REG_SIZE = 0x10;
static constexpr std::size_t CH8_STACK_SIZE = 0x10;
static constexpr std::size_t CH8_KEY_SIZE = 0x10;
static constexpr std::size_t CH8_FONT_SIZE = 0x10;

static constexpr std::array<std::uint8_t, CONSTANTS::CH8_MEMORY_SIZE>
    FONT_LIST = {
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
        0xF0, 0x80, 0xF0, 0x80, 0x80 // F
    };

static constexpr std::array<sf::Keyboard::Key, CONSTANTS::CH8_KEY_SIZE>
    sdl_keymap = { sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,
                   sf::Keyboard::Num4, sf::Keyboard::Q,    sf::Keyboard::W,
                   sf::Keyboard::E,    sf::Keyboard::R,    sf::Keyboard::A,
                   sf::Keyboard::S,    sf::Keyboard::D,    sf::Keyboard::F,
                   sf::Keyboard::Z,    sf::Keyboard::X,    sf::Keyboard::C,
                   sf::Keyboard::V };

constexpr auto FRAMERATE = 300;
constexpr auto FRAMERATE_MIN = 60;
constexpr auto FRAMERATE_MAX = 350;

constexpr auto SCALE = 10;
constexpr auto SCALE_MIN = 5;
constexpr auto SCALE_MAX = 15;

constexpr char GLSL_VERSION[] = "#version 130";

namespace CLEAR_COLOR
{
    constexpr float X = 0.45f;
    constexpr float Y = 0.55f;
    constexpr float Z = 0.60f;
    constexpr float W = 1.00f;
}
}

#endif

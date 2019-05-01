#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "imgui.h"

#include "chip8.hpp"
#include "logging.hpp"
#include "imgui-SFML.h"
#include "constants.hpp"

#include <imgui_memory_editor.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

void win_menu_bar(sf::Window& window);
void win_game(sf::RenderWindow& window, std::array<std::uint8_t, CONSTANTS::CH8_GFX_SIZE>& graphics, sf::Texture& texture);
void win_mem_hex_editor(std::array<std::uint8_t, CONSTANTS::CH8_MEMORY_SIZE>& memory);
void win_gfx_hex_editor(std::array<std::uint8_t, CONSTANTS::CH8_GFX_SIZE>& graphics);
void win_registers(std::array<std::uint8_t, CONSTANTS::CH8_REG_SIZE>& registers);
void win_flags(bool& draw_flag);
void win_timers(std::uint8_t& sound_timer, std::uint8_t& delay_timer);
void win_disasm(std::vector<std::string>& vec);

void win_log();

#endif

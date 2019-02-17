#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "chip8.hpp"
#include "logging.hpp"

void win_menu_bar();
void win_game(SDL_Surface* screen);
void win_mem_hex_editor(std::array<std::uint8_t, Constants::CH8_MEMORY_SIZE>& memory);
void win_gfx_hex_editor(std::array<std::uint8_t, Constants::CH8_GFX_SIZE>& graphics);
void win_registers(std::array<std::uint8_t, Constants::CH8_REG_SIZE>& registers);
void win_flags(bool &draw_flag);
void win_timers(std::uint8_t &sound_timer, std::uint8_t &delay_timer);
void win_disasm(std::vector<std::string> &vec);

void win_log();

#endif

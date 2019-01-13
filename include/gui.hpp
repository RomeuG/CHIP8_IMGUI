#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "chip8.hpp"

void ShowExampleAppMainMenuBar();
void win_game();
void win_hex_editor(std::array<std::uint8_t, Constants::CH8_MEMORY_SIZE>& memory);
void win_registers(std::array<std::uint8_t, Constants::CH8_REG_SIZE>& registers);
void win_flags(bool &draw_flag);
void win_timers(std::uint8_t &sound_timer, std::uint8_t &delay_timer);

#endif

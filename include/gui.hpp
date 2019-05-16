#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "imgui.h"

#include "chip8.hpp"
#include "logging.hpp"
#include "imgui-SFML.h"
#include "constants.hpp"

#include <imgui_memory_editor.h>
#include "imgui_filebrowser.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct Gui {

	// chip8
	Chip8 emulator;

	// file browser
	bool file_browser_open{false};
	ImGui::FileBrowser file_browser;

	// window flags
	bool window_game_enabled{true};
	bool window_mem_hex_enabled{true};
	bool window_gfx_hex_enabled{true};
	bool window_registers_enabled{true};
	bool window_flags_enabled{true};
	bool window_timers_enabled{true};
	bool window_disasm_enabled{true};
	bool window_log_enabled{true};

	int game_scale = CONSTANTS::SCALE;
	int game_framerate = CONSTANTS::FRAMERATE;

	Gui() = default;
	~Gui();

	// menu bar functions
	auto win_menu_bar_file() -> void;
	auto win_menu_bar_windows() -> void;

	// single window methods
	auto win_menu_bar() -> void;
	auto win_game() -> void;
	auto win_mem_hex_editor() -> void;
	auto win_gfx_hex_editor() -> void;
	auto win_registers() -> void;
	auto win_flags() -> void;
	auto win_timers() -> void;
	auto win_disasm() -> void;
	auto win_log() -> void;
	auto win_file_browser() -> void;

	// rendering
	auto render_windows() -> void;
};

#endif

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

struct Gui {

	// chip8
	Chip8 emulator;

	// window flags
	bool window_game_enabled{true};
	bool window_mem_hex_enabled{true};
	bool window_gfx_hex_enabled{true};
	bool window_registers_enabled{true};
	bool window_flags_enabled{true};
	bool window_timers_enabled{true};
	bool window_disasm_enabled{true};
	bool window_log_enabled{true};

	Gui() = default;
	~Gui();

	// single window methods
	void win_menu_bar();
	void win_game();
	void win_mem_hex_editor();
	void win_gfx_hex_editor();
	void win_registers();
	void win_flags();
	void win_timers();
	void win_disasm();
	void win_log();

	// rendering
	void render_windows();
};

#endif

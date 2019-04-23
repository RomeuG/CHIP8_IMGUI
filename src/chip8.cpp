#include "chip8.hpp"
#include "disasm.hpp"

#include <fstream>
#include <filesystem>

#include <SDL2/SDL_opengl.h>

#define LOG_INFO "info"
#define LOG_WARN "warn"
#define LOG_CRIT "crit"

chip8::chip8()
{
	// initialize logger
	logger = Logging::get_instance();

	// init font
	load_font();

	// init texture
	texture.create(64, 32);
}

void chip8::load_font()
{
	std::copy(Constants::FONT_LIST.begin(), Constants::FONT_LIST.end(), memory.begin());
}

void chip8::load_rom(char *rom_name)
{
	// read file
	std::ifstream f(rom_name, std::ios::binary);
	std::vector<char> v(std::istreambuf_iterator<char>{f}, {});

	// file size
	file_size = std::filesystem::file_size(rom_name);

	// load into memory
	auto it = memory.begin();
	std::advance(it, Constants::ROM_LOCATION);
	std::copy(v.begin(), v.end(), it);

	// disassemble rom
	std::uint32_t pc = 0x0;
	while (pc < (file_size)) {
		auto disasm = disasm_opcode(v[pc], v[pc + 1], pc);
		disassembly.push_back(disasm);
		pc += 2;
	}
}

void chip8::cycle()
{
	// static int next_frame = SDL_GetTicks() + 60;

	opcode = (memory[pc] << 8 | memory[pc + 1]);

	// reset timers
	if (delay_timer > 0) delay_timer--;
	if (sound_timer > 0) sound_timer--;
	if (sound_timer == 0); //beep;

	auto instruction = (std::uint16_t) (opcode & 0xF000);
	auto a = instruction_table.find(instruction);
	if (a == instruction_table.cend()) {
		//DEBUG_PRINT(stdout, "%s\n", "Unknown instruction.");
		logger->add_log("[%05d] [%s] Opcode: %s\n", ImGui::GetFrameCount(), LOG_WARN, "Unknown");
	} else {
		logger->add_log("[%05d] [%s] Opcode: 0x%02X\n", ImGui::GetFrameCount(), LOG_INFO, instruction);
		(a->second)();
	}

	// draw flag events
	if (draw_flag) {
		// graphics update
		graphics_update();
		draw_flag = false;
	}

	// input events
	input_new_event();

	// fps
	//fps_lock((std::uint32_t) next_frame, 60);
	//next_frame = SDL_GetTicks() + 60;
}

void chip8::graphics_clear()
{
	// SDL_FillRect(screen, nullptr, 0);
}

void chip8::graphics_update()
{
	// graphics_clear();

	// if (!draw_flag_blocked) {
	// 	for (auto y = 0; y < 32; y++) {
	// 		for (auto x = 0; x < 64; x++) {
	// 			if (graphics[x + (y * 64)]) {
	// 				draw_pixel(x, y);
	// 			}
	// 		}
	// 	}
	// }
}

void chip8::fps_lock(std::uint32_t next_frame, std::uint32_t max_fps)
{
	// unsigned int n_ticks = SDL_GetTicks();

	// if (next_frame < n_ticks) {
	// 	return;
	// }

	// if (next_frame > (n_ticks + max_fps)) {
	// 	SDL_Delay(max_fps);
	// } else {
	// 	SDL_Delay(next_frame - n_ticks);
	// }
}

void chip8::draw_pixel(int x, int y)
{
	// std::uint8_t *pixel;
	// unsigned int i, j;

	// auto window_view = window->getView();
	// auto window_pixels = window->getViewport(window_view);

	// pixel = (std::uint8_t *) screen->pixels + (y * 8) * screen->pitch + (x * 8);
	// for (i = 0; i < 8; i++) {
	// 	for (j = 0; j < 8; j++) {
	// 		pixel[j] = 0xFF;
	// 	}

	// 	pixel += screen->pitch;
	// }
}

int chip8::input_new_event()
{
	// SDL_Event event;

	// while (SDL_PollEvent(&event)) {
	// 	switch (event.type) {
	// 		case SDL_QUIT: return 1;
	// 		case SDL_KEYDOWN:
	// 			for (auto i = 0; i < Constants::CH8_KEY_SIZE; i++) {
	// 				if (Constants::sdl_keymap[i] == event.key.keysym.sym) {
	// 					keys[i] = 1;
	// 					break;
	// 				}
	// 			}
	// 			break;
	// 		case SDL_KEYUP:
	// 			for (auto i = 0; i < Constants::CH8_KEY_SIZE; i++) {
	// 				if (Constants::sdl_keymap[i] == event.key.keysym.sym) {
	// 					keys[i] = 0;
	// 					break;
	// 				}
	// 			}
	// 			break;
	// 		default: break;
	// 	}
	// }

	return 0;
}

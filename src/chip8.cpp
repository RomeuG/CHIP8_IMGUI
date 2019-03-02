#include "chip8.hpp"
#include "disasm.hpp"

#include <fstream>
#include <filesystem>

#define LOG_INFO "info"
#define LOG_WARN "warn"
#define LOG_CRIT "crit"

chip8::chip8()
{
	// initialize logger
	logger = Logging::get_instance();

	// init font
	load_font();
}

void chip8::load_font()
{
	std::copy(Constants::FONT_LIST.begin(), Constants::FONT_LIST.end(), memory.begin());
}

void chip8::load_rom(char* rom_name)
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
	// input events
	// fps
}

void chip8::draw_pixel(int x, int y)
{
	Uint8 *pixel;
	unsigned int i, j;

	pixel = (Uint8 *) screen->pixels + (y * 8) * screen->pitch + (x * 8);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			pixel[j] = 0xFF;
		}

		pixel += screen->pitch;
	}
}

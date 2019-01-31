#include "chip8.hpp"
#include "disasm.hpp"

#include <fstream>
#include <filesystem>

#define LOG_INFO "info"

chip8::chip8()
{
	// initialize logger
	logger = logging::get_instance();

	// init font
	load_font();
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
	while(pc < (file_size)) {
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

	auto a = instruction_table.find(opcode & 0xF000);
	if (a == instruction_table.cend()) {
		DEBUG_PRINT(stdout, "%s\n", "Unknown instruction.");
	}
	else {
		logger->add_log("[%05d] [%s] Opcode: 0x%02X\n",
						ImGui::GetFrameCount(), LOG_INFO, opcode & 0xF000);
		(a->second)();
	}

	// draw flag events
	// input events
	// fps
}

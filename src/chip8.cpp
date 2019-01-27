#include "chip8.hpp"

#include <fstream>

auto _get_file_size(std::ifstream &f)
{
	const auto _begin = f.tellg();
	f.seekg(0, std::ios::end);
	const auto _end = f.tellg();
	f.seekg(0, std::ios::beg);

	return _end - _begin;
}

chip8::chip8()
{
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
	file_size = _get_file_size(f);

	auto it = memory.begin();
	std::advance(it, Constants::ROM_LOCATION);
	std::copy(v.begin(), v.end(), it);

	// disassemble rom
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
		(a->second)();
	}

	// draw flag events
	// input events
	// fps
}

#include "chip8.hpp"

chip8::chip8()
{
	load_font();
}

void chip8::load_font()
{
	std::copy(Constants::FONT_LIST.begin(), Constants::FONT_LIST.end(), memory.begin());
}

void chip8::load_rom(std::vector<char>&& buffer)
{
	auto it = memory.begin();
	std::advance(it, Constants::ROM_LOCATION);
	std::copy(buffer.begin(), buffer.end(), it);
}

void chip8::cycle()
{
//	for (;;) {
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
//	}

	// draw flag events
	// input events
	// fps
}

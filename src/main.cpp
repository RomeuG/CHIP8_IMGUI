#include "imgui.h"
#include "imgui-SFML.h"

#include "chip8.hpp"
#include "gui.hpp"

#include <cstring>
#include <fstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace Constants {
	constexpr char GLSL_VERSION[] = "#version 130";

	namespace CLEAR_COLOR {
		constexpr float X = 0.45f;
		constexpr float Y = 0.55f;
		constexpr float Z = 0.60f;
		constexpr float W = 1.00f;
	}
}

void render_windows(chip8 &c8)
{

	// menu bar
	win_menu_bar();

	// insert windows here
	win_game(*c8.window, c8.graphics, c8.texture);

	// memory hex editor
	win_mem_hex_editor(c8.memory);

	// graphics hex editor
	win_gfx_hex_editor(c8.graphics);

	// register window
	win_registers(c8.V);

	// flag window
	win_flags(c8.draw_flag);

	// timer window
	win_timers(c8.sound_timer, c8.delay_timer);

	// disasm window
	win_disasm(c8.disassembly);

	// demo window
	//ImGui::ShowDemoWindow();

	// log window
	win_log();
}

int main(int argc, char** argv)
{
    // check arguments
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    // tests
	if (std::strncmp(argv[1], "tests", 5) == 0) {
        // testing here
        chip8 a;
        a.load_rom(argv[2]);
		a.cycle();
        return EXIT_SUCCESS;
    }

	// init chip8
	chip8 a;
	a.load_rom(argv[1]);
	a.window = new sf::RenderWindow(sf::VideoMode(640, 480), "ImGui + SFML = <3");
	a.window->setFramerateLimit(300);

	ImGui::SFML::Init(*a.window);

	sf::Clock deltaClock;

	// imgui style
	ImGui::StyleColorsDark();

	// Main loop
	while (a.window->isOpen()) {
		sf::Event event;

		while (a.window->pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				a.window->close();
			}
		}

		ImGui::SFML::Update(*a.window, deltaClock.restart());

		// function that has all windows
		render_windows(a);

		// chip8 cycle
		a.cycle();

		// Rendering
		a.window->clear();
		ImGui::SFML::Render(*a.window);
		a.window->display();
	}

	// Cleanup
	ImGui::SFML::Shutdown();
	ImGui::DestroyContext();

	return EXIT_SUCCESS;
}

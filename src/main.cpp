#include "imgui.h"
#include "imgui-SFML.h"

#include "chip8.hpp"
#include "gui.hpp"

#include <cstring>

auto main(int argc, char** argv) -> int
{
    // check arguments
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    // tests
	if (std::strncmp(argv[1], "tests", 5) == 0) {
        // testing here
        Chip8 a;
        a.load_rom(argv[2]);
		a.cycle();
        return EXIT_SUCCESS;
    }

	Gui gui;

	// init chip8
	gui.emulator.load_rom(argv[1]);
	gui.emulator.window = new sf::RenderWindow(sf::VideoMode(640, 480), "ImGui + SFML = <3");
	gui.emulator.window->setFramerateLimit(300);

	ImGui::SFML::Init(*gui.emulator.window);

	sf::Clock deltaClock;

	// imgui style
	ImGui::StyleColorsDark();

	// Main loop
	while (gui.emulator.window->isOpen()) {
		sf::Event event;

		while (gui.emulator.window->pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				gui.emulator.window->close();
			}
		}

		ImGui::SFML::Update(*gui.emulator.window, deltaClock.restart());

		// function that has all windows
		gui.render_windows();

		// chip8 cycle
		gui.emulator.cycle();

		// Rendering
		gui.emulator.window->clear();
		ImGui::SFML::Render(*gui.emulator.window);
		gui.emulator.window->display();
	}

	// Cleanup
	ImGui::SFML::Shutdown();
	ImGui::DestroyContext();

	return EXIT_SUCCESS;
}

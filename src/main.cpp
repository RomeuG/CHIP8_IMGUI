#include "imgui-SFML.hpp"
// #include "imgui.h"
#include "imgui.cpp"
#include "imgui_draw.cpp"
#include "imgui_widgets.cpp"
#include "imgui-SFML.cpp"

#include "imgui_filebrowser.hpp"
#include "imgui_memory_editor.h"

#include "constants.hpp"

#include "disasm.cpp"
#include "logging.cpp"
#include "chip8.cpp"
#include "gui.cpp"

#include <cstring>

auto main(int argc, char** argv) -> int
{
    // check arguments
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    Gui gui;

    // init chip8
    gui.emulator.load_rom(argv[1]);
    gui.emulator.window =
        new sf::RenderWindow(sf::VideoMode(640, 480), "Chip8 ImGui");
    // gui.emulator.window->setFramerateLimit(300);

    ImGui::SFML::Init(*gui.emulator.window);

    sf::Clock delta_clock;

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

        ImGui::SFML::Update(*gui.emulator.window, delta_clock.restart());

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

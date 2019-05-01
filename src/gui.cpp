#include "gui.hpp"

static void win_menu_bar_file(sf::Window& window)
{
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    ImGui::Separator();
	if (ImGui::BeginMenu("Settings")) {

		static auto framerate_slider = CONSTANTS::FRAMERATE;
		if (ImGui::SliderInt("Slider", &framerate_slider, CONSTANTS::FRAMERATE_MIN, CONSTANTS::FRAMERATE_MAX)) {
			window.setFramerateLimit(framerate_slider);
		}

		ImGui::EndMenu();
    }
	if (ImGui::MenuItem("Checked", nullptr, true)) { }
    if (ImGui::MenuItem("Quit", "Alt+F4")) { exit(0); }
}

void win_menu_bar(sf::Window& window)
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
            win_menu_bar_file(window);
            ImGui::EndMenu();
        }

		if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }

		ImGui::EndMainMenuBar();
    }
}

// TODO: this is completely garbage
void win_game(sf::RenderWindow& window, std::array<std::uint8_t, CONSTANTS::CH8_GFX_SIZE>& graphics, sf::Texture& texture)
{
	ImGui::Begin("Game Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	std::vector<std::uint8_t> data;
	for (auto &i : graphics) {
		if (i == 0) {
			data.push_back(0x0);
			data.push_back(0x0);
			data.push_back(0x0);
			data.push_back(0xFF);
		} else {
			data.push_back(0xFF);
			data.push_back(0xFF);
			data.push_back(0xFF);
			data.push_back(0xFF);
		}
	}

	texture.update(graphics.data());
	texture.update(data.data());

	sf::Sprite sprite;
	sprite.setTexture(texture, false);
	sprite.scale(10, 10);

	ImGui::Image(sprite);
ImGui::End();
}

void win_mem_hex_editor(std::array<std::uint8_t, CONSTANTS::CH8_MEMORY_SIZE>& memory)
{
    static MemoryEditor mem_edit;
	mem_edit.DrawWindow("Memory Hex Editor", &memory, CONSTANTS::CH8_MEMORY_SIZE);
}

void win_gfx_hex_editor(std::array<std::uint8_t, CONSTANTS::CH8_GFX_SIZE>& graphics)
{
	static MemoryEditor gfx_edit;
	gfx_edit.DrawWindow("Graphics Hex Editor", &graphics, CONSTANTS::CH8_GFX_SIZE);
}

void win_registers(std::array<std::uint8_t, CONSTANTS::CH8_REG_SIZE>& registers)
{
    ImGui::Begin("CHIP8 Registers");

	ImGui::Columns(4, nullptr, true);
	static bool selected[16] = {false};
	for (auto i = 0; i < 16; i++) {
        char label[32];
        sprintf(label, "V[0x%.1X] = 0x%.2X", i, registers[i]);
        if (ImGui::Selectable(label, &selected[i])) {}
        ImGui::NextColumn();
    }

	ImGui::End();
}

void win_flags(bool &draw_flag)
{
	ImGui::Begin("CHIP8 Flags");
	ImGui::CheckboxFlags("Draw Flag", (unsigned int*)&draw_flag, ImGuiComboFlags_PopupAlignLeft);
	ImGui::End();
}

void win_timers(std::uint8_t &sound_timer, std::uint8_t &delay_timer)
{
	ImGui::Begin("CHIP8 Timer");

	ImGui::Columns(2, nullptr, true);
	static bool selected[2] = {false};
	for (auto i = 0; i < 2; i++) {
        char label[32];
        sprintf(label, "%s = 0x%.2X", i == 0 ? "Sound Timer" : "Delay Timer", i == 0 ? sound_timer : delay_timer);
		if (ImGui::Selectable(label, &selected[i])) {}
		ImGui::NextColumn();
    }

	ImGui::End();
}

void win_disasm(std::vector<std::string> &vec)
{
	ImGui::Begin("Disasm window");
	for (auto &disasm_str : vec) {
	 	ImGui::Selectable(disasm_str.c_str());
	}
	ImGui::End();
}

void win_log()
{
	auto logger = Logging::get_instance();

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Chip8 Logging");
    ImGui::End();

    logger->draw("Chip8 Logging");
}

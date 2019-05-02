#include "gui.hpp"

Gui::~Gui() {}

static auto win_menu_bar_file(sf::Window& window) -> void
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

auto Gui::win_menu_bar() -> void
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
            //win_menu_bar_file(emulator.window);
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

auto Gui::win_game() -> void
{
	ImGui::Begin("Game Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	std::vector<std::uint8_t> data;
	for (auto &i : emulator.graphics) {
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

	emulator.texture.update(emulator.graphics.data());
	emulator.texture.update(data.data());

	sf::Sprite sprite;
	sprite.setTexture(emulator.texture, false);
	sprite.scale(10, 10);

	ImGui::Image(sprite);
	ImGui::End();
}

auto Gui::win_mem_hex_editor() -> void
{
    static MemoryEditor mem_edit;
	mem_edit.DrawWindow("Memory Hex Editor", &emulator.memory, CONSTANTS::CH8_MEMORY_SIZE);
}

auto Gui::win_gfx_hex_editor() -> void
{
	static MemoryEditor gfx_edit;
	gfx_edit.DrawWindow("Graphics Hex Editor", &emulator.graphics, CONSTANTS::CH8_GFX_SIZE);
}

auto Gui::win_registers() -> void
{
    ImGui::Begin("CHIP8 Registers");

	ImGui::Columns(4, nullptr, true);
	static bool selected[16] = {false};
	for (auto i = 0; i < 16; i++) {
        char label[32];
        sprintf(label, "V[0x%.1X] = 0x%.2X", i, emulator.V[i]);
        if (ImGui::Selectable(label, &selected[i])) {}
        ImGui::NextColumn();
    }

	ImGui::End();
}

auto Gui::win_flags() -> void
{
	ImGui::Begin("CHIP8 Flags");
	ImGui::CheckboxFlags("Draw Flag", (unsigned int*)&emulator.draw_flag, ImGuiComboFlags_PopupAlignLeft);
	ImGui::End();
}

auto Gui::win_timers() -> void
{
	ImGui::Begin("CHIP8 Timer");

	ImGui::Columns(2, nullptr, true);
	static bool selected[2] = {false};
	for (auto i = 0; i < 2; i++) {
        char label[32];
        sprintf(label, "%s = 0x%.2X", i == 0 ? "Sound Timer" : "Delay Timer", i == 0 ? emulator.sound_timer : emulator.delay_timer);
		if (ImGui::Selectable(label, &selected[i])) {}
		ImGui::NextColumn();
    }

	ImGui::End();
}

auto Gui::win_disasm() -> void
{

	ImGui::Begin("Disasm window");
	for (auto &disasm_str : emulator.disassembly) {
	 	ImGui::Selectable(disasm_str.c_str());
	}
	ImGui::End();
}

auto Gui::win_log() -> void
{
	auto logger = Logging::get_instance();

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Chip8 Logging");
	ImGui::End();

	logger->draw("Chip8 Logging");
}

auto Gui::render_windows() -> void
{
	win_menu_bar();
	win_game();
	win_mem_hex_editor();
	win_gfx_hex_editor();
	win_registers();
	win_flags();
	win_timers();
	win_disasm();
	//ImGui::ShowDemoWindow();
	win_log();
}

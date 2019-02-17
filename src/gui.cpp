#include "gui.hpp"

#include <imgui_memory_editor.h>

#include <GL/gl3w.h>

static void win_menu_bar_file()
{
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    ImGui::Separator();
	if (ImGui::BeginMenu("Options")) {
		static auto enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);

		for (auto i = 0; i < 10; i++) {
			ImGui::Text("Scrolling Text %d", i);
		}

        ImGui::EndChild();

		static auto f = 0.5f;
		static auto n = 0;
		static auto b = true;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::Checkbox("Check", &b);
        ImGui::EndMenu();
    }
	if (ImGui::MenuItem("Checked", nullptr, true)) { }
    if (ImGui::MenuItem("Quit", "Alt+F4")) { exit(0); }
}

void win_menu_bar()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
            win_menu_bar_file();
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

void win_game(SDL_Surface* screen)
{
	static bool generate_texture = true;
	static GLuint texture_id = 0;

    ImGui::Begin("Game Window");

	if (generate_texture) {
		glGenTextures(1, &texture_id);
		generate_texture = false;
	}

	if (screen != nullptr) {
		auto gl_mode = GL_RGB;
		auto window_size = ImGui::GetWindowSize();

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, gl_mode, screen->w, screen->h, 0, gl_mode, GL_UNSIGNED_BYTE, screen->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		ImGui::Image((void*) texture_id, ImVec2(window_size.x, window_size.y - 50));
	}
	else {
		std::printf("SDL_Surface is null: %s\n", SDL_GetError());
	}

    ImGui::End();
}

void win_hex_editor(std::array<std::uint8_t, Constants::CH8_MEMORY_SIZE>& memory)
{
    static MemoryEditor mem_edit;
    mem_edit.DrawWindow("Memory Editor", &memory, Constants::CH8_MEMORY_SIZE);
}

void win_registers(std::array<std::uint8_t, Constants::CH8_REG_SIZE>& registers)
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

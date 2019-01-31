#include "gui.hpp"

#include <imgui_memory_editor.h>

static void win_menu_bar_file()
{
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();

        static float f = 0.5f;
        static int n = 0;
        static bool b = true;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ImGui::Checkbox("Check", &b);
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    if (ImGui::MenuItem("Quit", "Alt+F4")) { exit(0); }
}

void win_menu_bar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            win_menu_bar_file();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
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

void win_game()
{
    ImGui::Begin("Game Window");
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 100, p.y + 100), ImGui::GetColorU32((ImGuiCol) 1));
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

    ImGui::Columns(4, NULL, true);
    static bool selected[16] = { 0 };
    for (int i = 0; i < 16; i++) {
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

	ImGui::Columns(2, NULL, true);
    static bool selected[2] = { 0 };
    for (int i = 0; i < 2; i++) {
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
	//static logging log;
	//logging g_logger;
	logging *g_logger = logging::get_instance();

	// For the demo: add a debug button before the normal log window contents
	// We take advantage of the fact that multiple calls to Begin()/End() are appending to the same window.
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Chip8 Logging");
    if (ImGui::SmallButton("Add 5 entries"))
    {
        static int counter = 0;
        for (int n = 0; n < 5; n++)
        {
            const char* categories[3] = { "info", "warn", "error" };
            const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
            g_logger->add_log("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
							  ImGui::GetFrameCount(), categories[counter % IM_ARRAYSIZE(categories)], ImGui::GetTime(), words[counter % IM_ARRAYSIZE(words)]);
            counter++;
        }
	}
    ImGui::End();

    g_logger->draw("Chip8 Logging");
}

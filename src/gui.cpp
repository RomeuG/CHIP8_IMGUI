#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

struct Gui {
    // chip8
    Chip8 emulator;

    // file browser
    bool file_browser_open{ false };
    ImGui::FileBrowser file_browser;

    // window flags
    bool window_game_enabled{ true };
    bool window_mem_hex_enabled{ true };
    bool window_gfx_hex_enabled{ true };
    bool window_registers_enabled{ true };
    bool window_flags_enabled{ true };
    bool window_timers_enabled{ true };
    bool window_disasm_enabled{ true };
    bool window_log_enabled{ true };

    int game_scale = CONSTANTS::SCALE;
    int game_framerate = CONSTANTS::FRAMERATE;

    Gui() = default;
    ~Gui();

    // menu bar functions
    auto win_menu_bar_file() -> void;
    auto win_menu_bar_windows() -> void;

    // single window methods
    auto win_menu_bar() -> void;
    auto win_game() -> void;
    auto win_mem_hex_editor() -> void;
    auto win_gfx_hex_editor() -> void;
    auto win_registers() -> void;
    auto win_flags() -> void;
    auto win_timers() -> void;
    auto win_disasm() -> void;
    auto win_log() -> void;
    auto win_file_browser() -> void;

    // rendering
    auto render_windows() -> void;
};

Gui::~Gui() = default;

auto Gui::win_menu_bar_file() -> void
{
    if (ImGui::MenuItem("Open Rom...", "Ctrl+O")) {
        if (!file_browser.IsOpened()) {
            file_browser.Open();
            file_browser_open = true;
        } else {
            file_browser.Close();
            file_browser.Open();
            file_browser_open = true;
        }
    }
    ImGui::Separator();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    if (ImGui::BeginMenu("Settings")) {
        // game framerate
        if (ImGui::SliderInt("Framerate", &game_framerate,
                             CONSTANTS::FRAMERATE_MIN,
                             CONSTANTS::FRAMERATE_MAX)) {
            emulator.window->setFramerateLimit(game_framerate);
        }

        // game scale
        if (ImGui::SliderInt("Scale", &game_scale, CONSTANTS::SCALE_MIN,
                             CONSTANTS::SCALE_MAX)) {
        }

        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Checked", nullptr, true)) {
    }
    if (ImGui::MenuItem("Quit", "Alt+F4")) {
        exit(0);
    }
}

auto Gui::win_menu_bar_windows() -> void
{
    if (ImGui::MenuItem("Game Window", nullptr, &window_game_enabled)) {
    }
    if (ImGui::MenuItem("Disasm Window", nullptr, &window_disasm_enabled)) {
    }
    if (ImGui::MenuItem("Flags Window", nullptr, &window_flags_enabled)) {
    }
    if (ImGui::MenuItem("Graphics Hex Window", nullptr,
                        &window_gfx_hex_enabled)) {
    }
    if (ImGui::MenuItem("Memory Hex Window", nullptr,
                        &window_mem_hex_enabled)) {
    }
    if (ImGui::MenuItem("Log Window", nullptr, &window_log_enabled)) {
    }
    if (ImGui::MenuItem("Registers Window", nullptr,
                        &window_registers_enabled)) {
    }
    if (ImGui::MenuItem("Timers Window", nullptr, &window_timers_enabled)) {
    }
}

auto Gui::win_menu_bar() -> void
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            win_menu_bar_file();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {
            win_menu_bar_windows();
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

auto Gui::win_game() -> void
{
    if (!window_game_enabled) {
        return;
    }

    ImGui::Begin("Game Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    int j = 0;
    std::array<std::uint8_t, 0x800 * 4> data{ 0 };
    for (auto& i : emulator.graphics) {
        if (i == 0) {
            j += 3;
            data[j++] = 0xFF;
        } else {
            data[j++] = 0xFF;
            data[j++] = 0xFF;
            data[j++] = 0xFF;
            data[j++] = 0xFF;
        }
    }

    emulator.texture.update(data.data());

    sf::Sprite sprite;
    sprite.setTexture(emulator.texture, false);
    sprite.scale(game_scale, game_scale);

    ImGui::Image(sprite);

    ImGui::End();
}

auto Gui::win_mem_hex_editor() -> void
{
    if (!window_mem_hex_enabled) {
        return;
    }

    static MemoryEditor mem_edit;
    mem_edit.DrawWindow("Memory Hex Editor", &emulator.memory,
                        CONSTANTS::CH8_MEMORY_SIZE);
}

auto Gui::win_gfx_hex_editor() -> void
{
    if (!window_gfx_hex_enabled) {
        return;
    }

    static MemoryEditor gfx_edit;
    gfx_edit.DrawWindow("Graphics Hex Editor", &emulator.graphics,
                        CONSTANTS::CH8_GFX_SIZE);
}

auto Gui::win_registers() -> void
{
    if (!window_registers_enabled) {
        return;
    }

    ImGui::Begin("CHIP8 Registers");

    ImGui::Columns(4, nullptr, true);
    static bool selected[16] = { false };
    for (auto i = 0; i < 16; i++) {
        char label[32];
        sprintf(label, "V[0x%.1X] = 0x%.2X", i, emulator.V[i]);
        if (ImGui::Selectable(label, &selected[i])) {
        }
        ImGui::NextColumn();
    }

    ImGui::End();
}

auto Gui::win_flags() -> void
{
    if (!window_flags_enabled) {
        return;
    }

    ImGui::Begin("CHIP8 Flags");
    ImGui::CheckboxFlags("Draw Flag", (unsigned int*)&emulator.draw_flag,
                         ImGuiComboFlags_PopupAlignLeft);
    ImGui::End();
}

auto Gui::win_timers() -> void
{
    if (!window_timers_enabled) {
        return;
    }

    ImGui::Begin("CHIP8 Timer");

    ImGui::Columns(2, nullptr, true);
    static bool selected[2] = { false };
    for (auto i = 0; i < 2; i++) {
        char label[32];
        sprintf(label, "%s = 0x%.2X", i == 0 ? "Sound Timer" : "Delay Timer",
                i == 0 ? emulator.sound_timer : emulator.delay_timer);
        if (ImGui::Selectable(label, &selected[i])) {
        }
        ImGui::NextColumn();
    }

    ImGui::End();
}

auto Gui::win_disasm() -> void
{
    if (!window_disasm_enabled) {
        return;
    }

    ImGui::Begin("Disasm window");
    for (auto& disasm_str : emulator.disassembly) {
        ImGui::Selectable(disasm_str.c_str());
    }
    ImGui::End();
}

auto Gui::win_log() -> void
{
    if (!window_log_enabled) {
        return;
    }

    auto logger = Logging::get_instance();

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Chip8 Logging");
    ImGui::End();

    logger->draw("Chip8 Logging");
}

auto Gui::win_file_browser() -> void
{
    if (file_browser_open) {
        file_browser.Display();

        if (file_browser.HasSelected()) {
            std::printf("File browser: %s\n",
                        file_browser.GetSelected().string().c_str());

            emulator.reset();
            emulator.load_rom(file_browser.GetSelected().string().c_str());

            file_browser.ClearSelected();
            file_browser_open = false;
        }
    }
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
    win_file_browser();
}

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "chip8.hpp"

#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <cstring>
#include <fstream>
#include <imgui_memory_editor.h>

namespace Constants {
constexpr char GLSL_VERSION[] = "#version 130";

namespace CLEAR_COLOR {
constexpr float X = 0.45f;
constexpr float Y = 0.55f;
constexpr float Z = 0.60f;
constexpr float W = 1.00f;
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

    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x0]", (unsigned int*)&registers[0x0], ImGuiComboFlags_HeightLargest);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x1]", (unsigned int*)&registers[0x1], ImGuiComboFlags_HeightLargest);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x2]", (unsigned int*)&registers[0x2], ImGuiComboFlags_HeightLargest);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x3]", (unsigned int*)&registers[0x3], ImGuiComboFlags_HeightLargest);
    ImGui::CheckboxFlags("V[0x4]", (unsigned int*)&registers[0x4], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x5]", (unsigned int*)&registers[0x5], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x6]", (unsigned int*)&registers[0x6], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x7]", (unsigned int*)&registers[0x7], ImGuiComboFlags_PopupAlignLeft);
    ImGui::CheckboxFlags("V[0x8]", (unsigned int*)&registers[0x8], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0x9]", (unsigned int*)&registers[0x9], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0xA]", (unsigned int*)&registers[0xA], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0xB]", (unsigned int*)&registers[0xB], ImGuiComboFlags_PopupAlignLeft);
    ImGui::CheckboxFlags("V[0xC]", (unsigned int*)&registers[0xC], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0xD]", (unsigned int*)&registers[0xD], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0xE]", (unsigned int*)&registers[0xE], ImGuiComboFlags_PopupAlignLeft);
    ImGui::SameLine();
    ImGui::CheckboxFlags("V[0xF]", (unsigned int*)&registers[0xF], ImGuiComboFlags_PopupAlignLeft);

    ImGui::End();
}

int main(int argc, char** argv)
{
    // check arguments
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    // tests
    if (std::strcmp(argv[1], "tests") == 0) {
        // testing here

        // read file
        std::ifstream f(argv[2], std::ios::binary);
        std::vector<char> v(std::istreambuf_iterator<char>{f}, {});

        chip8 a;
        a.load_rom(std::move(v));
        a.cycle();
        return EXIT_SUCCESS;
    }

    // init chip8
    std::ifstream f(argv[1], std::ios::binary);
    std::vector<char> v(std::istreambuf_iterator<char>{f}, {});

    chip8 a;
    a.load_rom(std::move(v));

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window* window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    bool err = gl3wInit() != 0;
    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return EXIT_FAILURE;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    // imgui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(Constants::GLSL_VERSION);

    // Main loop
    bool done = false;
    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                done = true;
            }

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
                    && event.window.windowID == SDL_GetWindowID(window)) {
                done = true;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // insert windows here
        win_game();

        // hex editor
        win_hex_editor(a.memory);

        // register window
        win_registers(a.V);

        // demo window
        ImGui::ShowDemoWindow();

        // chip8 cycle
        a.cycle();

        // Rendering
        ImGui::Render();

        SDL_GL_MakeCurrent(window, gl_context);
        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(Constants::CLEAR_COLOR::X, Constants::CLEAR_COLOR::Y, Constants::CLEAR_COLOR::Z, Constants::CLEAR_COLOR::W);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

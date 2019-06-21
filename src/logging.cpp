#include "logging.hpp"

#include <iostream>

Logging* Logging::instance = nullptr;

Logging::~Logging()
{
    delete instance;
}

auto Logging::get_instance() -> Logging*
{
    if (instance == nullptr) {
        instance = new Logging();
        instance->active = true;
    }

    return instance;
}

auto Logging::clear() -> void
{
    text_buffer.clear();
    line_offsets.clear();
    line_offsets.push_back(0);
}

auto Logging::add_log(const char* fmt, ...) -> void
{
    if (!active) {
        return;
    }

    auto old_size = text_buffer.size();
    va_list args;
    va_start(args, fmt);
    text_buffer.appendfv(fmt, args);
    va_end(args);

    for (auto new_size = text_buffer.size(); old_size < new_size; old_size++) {
        if (text_buffer[old_size] == '\n') {
            line_offsets.push_back(old_size + 1);
        }
    }

    scroll_to_bottom = true;
}

auto Logging::draw(const char* title, bool* p_open) -> void
{
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::Button("Clear")) {
        clear();
    }

    ImGui::SameLine();
    auto copy = ImGui::Button("Copy");
    ImGui::SameLine();
    if (ImGui::Button(active ? "Stop" : "Start")) {
        active = !active;
    }
    ImGui::SameLine();
    text_filter.Draw("Filter", -100.0f);
    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false,
                      ImGuiWindowFlags_HorizontalScrollbar);
    if (copy) {
        ImGui::LogToClipboard();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    auto buf = text_buffer.begin();
    auto buf_end = text_buffer.end();
    if (text_filter.IsActive()) {
        for (auto line_no = 0; line_no < line_offsets.Size; line_no++) {
            auto line_start = buf + line_offsets[line_no];
            auto line_end = (line_no + 1 < line_offsets.Size) ?
                                (buf + line_offsets[line_no + 1] - 1) :
                                buf_end;
            if (text_filter.PassFilter(line_start, line_end)) {
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
    } else {
        ImGuiListClipper clipper;
        clipper.Begin(line_offsets.Size);
        while (clipper.Step()) {
            for (auto line_no = clipper.DisplayStart;
                 line_no < clipper.DisplayEnd; line_no++) {
                auto line_start = buf + line_offsets[line_no];
                auto line_end = (line_no + 1 < line_offsets.Size) ?
                                    (buf + line_offsets[line_no + 1] - 1) :
                                    buf_end;
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
        clipper.End();
    }
    ImGui::PopStyleVar();

    if (scroll_to_bottom) {
        ImGui::SetScrollHereY(1.0f);
    }

    scroll_to_bottom = false;
    ImGui::EndChild();
    ImGui::End();
}

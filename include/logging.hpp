#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

// singleton
struct Logging
{
	static Logging *instance;

    ImGuiTextBuffer text_buffer;
    ImGuiTextFilter text_filter;
    ImVector<int> line_offsets;
    bool scroll_to_bottom;

	static Logging *get_instance();

	~Logging();

	void clear();
	void add_log(const char* fmt, ...) IM_FMTARGS(2);
	void draw(const char* title, bool* p_open = nullptr);
};

#endif

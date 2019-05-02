#ifndef _LOGGING_HPP_
#define _LOGGING_HPP_

#include "imgui.h"

// singleton
struct Logging
{
	static Logging *instance;

    ImGuiTextBuffer text_buffer;
    ImGuiTextFilter text_filter;
    ImVector<int> line_offsets;

    bool scroll_to_bottom;
	bool active;

	static auto get_instance() -> Logging*;

	~Logging();

	auto clear() -> void;
	void add_log(const char* fmt, ...) IM_FMTARGS(2);
	auto draw(const char* title, bool* p_open = nullptr) -> void;
};

#endif

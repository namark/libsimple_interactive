#include <cstdio>
#include <thread>
#include <chrono>
#include <string>

#include "simple/interactive/initializer.h"
#include "simple/interactive/event.h"
#include "simple/support/function_utils.hpp"
#include "simple/support/misc.hpp"
#include "../common/sdl_input_grabber.h"

#include "../common/sdl_input_grabber.cpp"

using namespace simple::interactive;
using namespace std::chrono_literals;
using namespace std::string_literals;

void render_screen(int2 size, int2 cursor_position, char bg, char cursor, bool break_lines = false);

int main(int argc, char const* argv[]) try
{

	using simple::support::ston;
	const int screen_size_x = argc > 1 ? ston<int>(argv[1]) : 79;
	const int screen_size_y = argc > 2 ? ston<int>(argv[2]) : 23;
	const std::chrono::milliseconds frametime(argc > 3 ? ston<int>(argv[3]) : 16);
	const bool break_lines = argc > 4 ? "false"s != argv[4] : true;

	const float2 screen_size{float(screen_size_x), float(screen_size_y)};

	initializer init;
	sdl_input_grabber input_grabber;
	input_grabber.grab();
	relative_mouse_mode(true);

	float2 cursor_position{};

	bool run = true;
	while(run)
	{
		while(auto e = next_event()) std::visit( simple::support::overloaded{
			[&cursor_position, &screen_size](const mouse_motion& event)
			{
				// using value_or here since SDL sends motion events with invalid window id until window gains focus for the first time,
				// and screen_normalized_motion selects the screen/display based on the window id.
				const float2 motion = event.screen_normalized_motion().value_or(float2{});
				cursor_position += screen_size * motion;
				cursor_position.clamp(float2::zero(), screen_size - 1);
			},
			[&run](mouse_up)
			{
				run = false;
			},
			[&run](key_pressed)
			{
				run = false;
			},
			[](auto) { }
		}, *e);

		std::puts("\nPress any key or click to quit.");
		render_screen(int2(screen_size), int2(cursor_position), ' ', '*', break_lines);
		std::this_thread::sleep_for(frametime);
	}
	std::puts("");
	return 0;
}
catch(...)
{
	if(errno)
		std::perror("ERROR");

	const char* sdl_error = SDL_GetError();
	if(*sdl_error)
		std::puts(sdl_error);

	throw;
}

void render_screen(int2 size, int2 cursor_position, char bg, char cursor, bool break_lines)
{
	static std::string buffer;
	int linear_size = size.y() * size.x();
	if(break_lines)
		linear_size += size.y() - 1;
	buffer.resize(linear_size);

	auto row = buffer.begin();
	for(int y = 0; y < size.y(); ++y)
	{
		row = std::fill_n(row, size.x(), bg);
		if(break_lines && y != (size.y() - 1))
			*row++ = '\n';
	}

	int pitch = break_lines ? size.x() + 1 : size.x();
	int linear_cursor_position = cursor_position.y() * pitch + cursor_position.x();
	buffer[linear_cursor_position] = cursor;

	std::fputs(buffer.c_str(), stdout);
	std::fflush(stdout);
}


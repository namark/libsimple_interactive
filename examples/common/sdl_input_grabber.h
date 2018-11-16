#ifndef SIMPLE_INTERACTIVE_EXAMPLES_SDL_INPUT_GRABBER_H
#define SIMPLE_INTERACTIVE_EXAMPLES_SDL_INPUT_GRABBER_H

#include <memory>
#include "simple/sdlcore/initializer.h"
#include "simple/support/enum_flags_operators.hpp"

class sdl_input_grabber
{
	struct sdl_window_del
	{
		void operator()(SDL_Window * w) noexcept
		{
			SDL_DestroyWindow(w);
		}
	};

	simple::sdlcore::initializer video_init;
	std::unique_ptr<SDL_Window, sdl_window_del> window;
	public:
	sdl_input_grabber();
	void grab() noexcept;
	void release() noexcept;
};

#endif /* end of include guard */

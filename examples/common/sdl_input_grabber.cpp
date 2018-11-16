#include "sdl_input_grabber.h"
#include "simple/sdlcore/utils.hpp"

sdl_input_grabber::sdl_input_grabber() :
	video_init( simple::sdlcore::system_flag::video ),
	window(SDL_CreateWindow("input grabber", 0,0,0,0, SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS))
{
	simple::sdlcore::utils::throw_error(window.get());
}

void sdl_input_grabber::grab() noexcept
{
	SDL_SetWindowGrab(window.get(), SDL_TRUE);
}

void sdl_input_grabber::release() noexcept
{
	SDL_SetWindowGrab(window.get(), SDL_FALSE);
}

#include "codes.h"
#include "simple/support/enum.hpp"

using namespace simple::interactive;
using simple::support::to_integer;

scancode to_scancode(keycode code) noexcept
{
	return static_cast<scancode>(SDL_GetScancodeFromKey(to_integer(code)));
}

keycode to_keycode(scancode code) noexcept
{
	return static_cast<keycode>(
		SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(to_integer(code))) );
}

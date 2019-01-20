#include "codes.h"
#include "simple/support/enum.hpp"

using simple::support::to_integer;

namespace simple::interactive
{

	scancode to_scancode(keycode code) noexcept
	{
		return static_cast<scancode>(SDL_GetScancodeFromKey(to_integer(code)));
	}

	keycode to_keycode(scancode code) noexcept
	{
		return static_cast<keycode>(
			SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(to_integer(code))) );
	}

	bool pressed(scancode code) noexcept
	{
		const uint8_t * keyboard_state = SDL_GetKeyboardState(nullptr);
		return keyboard_state[to_integer(code)];
	}

} // namespace simple::interactive

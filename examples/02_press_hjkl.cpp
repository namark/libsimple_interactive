#include <cstdio>
#include <thread>
#include <chrono>

#include "simple/interactive/initializer.h"
#include "simple/interactive/event.h"
#include "common/sdl_input_grabber.h"

#include "common/sdl_input_grabber.cpp"

using namespace simple::interactive;
using namespace std::chrono_literals;

int main() try
{

	initializer init;

	sdl_input_grabber input_grabber;
	std::puts("Press H J or K L! (casue 2 keys is the limit for some keyoards!)");
	while(true)
	{
		next_event(); // update the key states

		if ((pressed(scancode::h) && pressed(scancode::j)) ||
			(pressed(scancode::k) && pressed(scancode::l)) ||
			pressed(scancode::escape))
			break;

		std::this_thread::sleep_for(20ms);
	}

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

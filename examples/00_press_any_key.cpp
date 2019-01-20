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
	std::puts("Press any key!");
	while(true)
		if (auto e = next_event(); !e || !std::holds_alternative<key_pressed>(*e))
			std::this_thread::sleep_for(20ms);
		else break;

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

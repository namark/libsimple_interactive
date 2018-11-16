#include "initializer.h"

using namespace simple;
using namespace simple::interactive;

using flag = sdlcore::system_flag;

initializer::initializer() :
	sdlcore::initializer(flag::event | flag::haptic | flag::joystick | flag::game_controller)
{}

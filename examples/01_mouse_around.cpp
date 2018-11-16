#include "simple/interactive/initializer.h"

#if SDL_VERSION_ATLEAST(2,0,4)
#include "versions/01_mouse_around_capture.cpp"
#else
#include "versions/01_mouse_around_grab.cpp"
#endif

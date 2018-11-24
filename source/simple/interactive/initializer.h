#ifndef SIMPLE_INTERACTIVE_INITIALIZER_H
#define SIMPLE_INTERACTIVE_INITIALIZER_H
#include "simple/sdlcore/initializer.h"

namespace simple::interactive
{

	class initializer : private sdlcore::initializer
	{
		public:
		initializer();
	};

} // namespace simple::graphical

#endif /* end of include guard */

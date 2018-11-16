#ifndef SIMPLE_GRAPHICAL_INITIALIZER_H
#define SIMPLE_GRAPHICAL_INITIALIZER_H
#include <memory>
#include "simple/sdlcore/initializer.h"
#include "simple/support/enum_flags_operators.hpp"

namespace simple::interactive
{

	class initializer : private sdlcore::initializer
	{
		public:
		initializer();
	};

} // namespace simple::graphical

#endif /* end of include guard */

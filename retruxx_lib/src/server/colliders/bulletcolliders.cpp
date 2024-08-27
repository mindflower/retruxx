#include "bulletcolliders.h"

#include <stdexcept>

namespace ai
{
	int CollideShellAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}
}

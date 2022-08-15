#include "breakableobjectcolliders.h"
#include <stdexcept>

namespace ai
{
	int CollideVehicleAndBreakableObject(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}
}

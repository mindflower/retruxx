#include "wheelasphaltcollider.h"

#include <stdexcept>

namespace ai
{
	int CollideWheelDefault(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideWheelAndAsphalt(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideWheelAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideWheelAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}
}

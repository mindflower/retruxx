#include "vehiclesplintercolliders.h"

#include <stdexcept>

namespace ai
{
	int CollideParticleSplinter(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollidePOAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehicleSplinterAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideParticleSplinterAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}
}

#include "vehiclecolliders.h"

#include <stdexcept>

namespace ai
{
	int CollideDummyAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehiclePartAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehicleAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehicleAndStatics(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehicleAndRoad(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehicleAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehiclePartAndGeomObj(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideVehicleAndPassCell(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}
}

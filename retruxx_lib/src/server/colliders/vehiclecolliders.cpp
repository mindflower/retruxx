#include "vehiclecolliders.h"

#include <stdexcept>

#include "thirdparty/containers.h"

namespace ai
{
	int CollideDummyAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideVehiclePartAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideVehicleAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideVehicleAndStatics(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideVehicleAndRoad(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideVehicleAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideVehiclePartAndGeomObj(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideVehicleAndPassCell(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}

#include "vehiclecolliders.h"

#include <stdexcept>

#include "retruxx/common.h"

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
		// TODO: implement CollideVehicleAndLandscape
		// RETRUXX_NOT_IMPLEMENTED;
        return 1;
	}

	int CollideVehicleAndStatics(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
        // TODO: implement CollideVehicleAndLandscape
        // RETRUXX_NOT_IMPLEMENTED;
        return 1;
	}

	int CollideVehicleAndRoad(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		// TODO: implement CollideVehicleAndRoad
        // RETRUXX_NOT_IMPLEMENTED;
        return 1;
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

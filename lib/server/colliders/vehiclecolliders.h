#pragma once
#include "ode/contact.h"

namespace m3d
{
	class Object;
}

namespace ai
{
	int CollideDummyAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehiclePartAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndStatics(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndRoad(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehiclePartAndGeomObj(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndPassCell(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}

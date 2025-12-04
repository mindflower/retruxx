#pragma once
#include "ode/contact.h"

namespace m3d
{
	class Object;
}

namespace ai
{
	int CollidePhysicUnitAndVehicle(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollidePhysicUnitAndShell(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollidePhysicUnitAndBlastWave(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideGeomObjAndShell(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}

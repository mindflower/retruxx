#pragma once
#include "ode/contact.h"

namespace m3d
{
	class Object;
}

namespace ai
{
	// Closing speed of the two bodies along the contact normal; 0 when they separate.
	float CalcContactDeltaSpeed(dContact const& contact);

	int CollidePhysicUnitAndVehicle(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollidePhysicUnitAndShell(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollidePhysicUnitAndBlastWave(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideGeomObjAndShell(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}

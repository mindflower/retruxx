#pragma once
#include "ode/contact.h"

namespace m3d
{
	class Object;
}

namespace ai
{
	int CollideVehicleAndBreakableObject(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}

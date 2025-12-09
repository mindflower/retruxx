#pragma once
#include "ode/contact.h"

namespace m3d
{
	class Object;
}

namespace ai
{
	int CollideWheelDefault(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideWheelAndAsphalt(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideWheelAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideWheelAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}

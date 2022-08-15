#pragma once
#include "ode/contact.h"

namespace m3d
{
	class Object;
}

namespace ai
{
	int CollideParticleSplinter(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollidePOAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleSplinterAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideParticleSplinterAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}

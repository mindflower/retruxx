#include "physicunitcolliders.h"

#include <stdexcept>

namespace ai
{
	int CollidePhysicUnitAndVehicle(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollidePhysicUnitAndShell(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollidePhysicUnitAndBlastWave(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideGeomObjAndShell(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}
}

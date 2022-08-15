#include "defaultcollider.h"

#include <stdexcept>

namespace ai
{
	int DefaultCollider(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int EmptyCollider(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideGeomObjAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	int CollideBreakableObjectAndGeomObject(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}
}

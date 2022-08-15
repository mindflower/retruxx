#pragma once
#include "ode/contact.h"

namespace m3d
{
	class Object;
}

namespace ai
{
	int DefaultCollider(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int EmptyCollider(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideGeomObjAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideShellAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideBreakableObjectAndGeomObject(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}

#include "wheelasphaltcollider.h"

#include <stdexcept>

#include "retruxx/common.h"

namespace ai
{
	int CollideWheelDefault(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideWheelAndAsphalt(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideWheelAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int CollideWheelAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}

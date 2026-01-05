#include "defaultcollider.h"

#include <stdexcept>

#include "retruxx/common.h"

namespace ai
{
    int DefaultCollider(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
    {
        return 1;
    }

    int EmptyCollider(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
    {
        return 0;
    }

    int CollideGeomObjAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int CollideBreakableObjectAndGeomObject(m3d::Object*, m3d::Object*, dContact*, unsigned&, bool)
    {
        // TODO: implement CollideBreakableObjectAndGeomObject
        // RETRUXX_NOT_IMPLEMENTED;
        return 1;
    }
}  // namespace ai

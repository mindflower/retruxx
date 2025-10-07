#include "box.h"
#include <stdexcept>
#include <math/vector.h>

namespace ai
{
    ai::Box* Box::CreateObject(dxSpace* space, const CVector& size, void(*movedCalback)(dxGeom*))
    {
        auto realSize = size;
        if (realSize.x < 0.1)
            realSize.x = 0.1;
        if (realSize.y < 0.1)
            realSize.y = 0.1;
        if (realSize.z < 0.1)
            realSize.z = 0.1;
         auto box = dCreateBox(space, realSize.x, realSize.y, realSize.z);
         return new Box(box, movedCalback);
    }

    CVector Box::GetSize() const
    {
        throw std::logic_error("Not implemented");
    }

    void Box::SetSize(const CVector& size)
    {
        auto realSize = size;
        if (realSize.x < 0.1)
            realSize.x = 0.1;
        if (realSize.y < 0.1)
            realSize.y = 0.1;
        if (realSize.z < 0.1)
            realSize.z = 0.1;
        dGeomBoxSetLengths(this->m_geomId, realSize.x, realSize.y, realSize.z);
    }

    Box::Box(dxGeom* const geomId, void(* movedCallback)(dxGeom*)) :
        Geom(geomId, movedCallback)
    {
    }
}

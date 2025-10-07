#include "box.h"
#include <stdexcept>
#include <math/vector.h>

namespace ai
{
    ai::Box* Box::CreateObject(dxSpace* space, const CVector& size, void(*movedCalback)(dxGeom*))
    {
        throw std::logic_error("Not implemented");
    }

    CVector Box::GetSize() const
    {
        throw std::logic_error("Not implemented");
    }

    void Box::SetSize(const CVector& size)
    {
        throw std::logic_error("Not implemented");
    }

    Box::Box(dxGeom* const geomId, void(* movedCallback)(dxGeom*)) :
        Geom(geomId, movedCallback)
    {
        throw std::logic_error("Not implemented");
    }
}

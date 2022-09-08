#include "box.h"
#include <stdexcept>
#include <math/vector.h>

namespace ai
{
    CVector Box::GetSize() const
    {
        throw std::logic_error("Not implemented");
    }

    Box* Box::CreateObject(dxSpace*, CVector const&, void(__fastcall *)(dxGeom*))
    {
        throw std::logic_error("Not implemented");
    }

    void Box::SetSize(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    Box::Box(dxGeom* const geomId, void(* movedCallback)(dxGeom*)) :
        Geom(geomId, movedCallback)
    {
        throw std::logic_error("Not implemented");
    }
}

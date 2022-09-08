#include "ray.h"

#include <stdexcept>
#include <ode/collision.h>

#include "math/vector.h"

namespace ai
{
    void Ray::SetDirection(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    float Ray::GetLength() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector Ray::GetDirection() const
    {
        throw std::logic_error("Not implemented");
    }

    Ray* Ray::CreateObject(dxSpace* space, float length, void(*movedCallback)(dxGeom*))
    {
        return new Ray(dCreateRay(space, length), movedCallback);
    }

    void Ray::SetLength(float)
    {
        throw std::logic_error("Not implemented");
    }

    Ray::Ray(dxGeom* const geomId, void(*movedCallback)(dxGeom*)) : Geom(geomId, movedCallback)
    {
    }
}

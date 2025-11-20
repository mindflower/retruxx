#include "ray.h"

#include <stdexcept>
#include <ode/collision.h>

#include "math/vector.h"

namespace ai
{
    void Ray::SetDirection(CVector const& direction)
    {
        auto pos = dGeomGetPosition(this->m_geomId);
        dGeomRaySet(this->m_geomId, pos[0], pos[1], pos[2], direction.x, direction.y, direction.z);
    }

    float Ray::GetLength() const
    {
        return dGeomRayGetLength(this->m_geomId);
    }

    CVector Ray::GetDirection() const
    {
        float dir[4];
        float start[4];

        dGeomRayGet(this->m_geomId, start, dir);
        CVector result = dir;
        return result;
    }

    Ray* Ray::CreateObject(dxSpace* space, float length, void(*movedCallback)(dxGeom*))
    {
        return new Ray(dCreateRay(space, length), movedCallback);
    }

    void Ray::SetLength(float length)
    {
        dGeomRaySetLength(m_geomId, length);
    }

    Ray::Ray(dxGeom* const geomId, void(*movedCallback)(dxGeom*)) : Geom(geomId, movedCallback)
    {
    }
}

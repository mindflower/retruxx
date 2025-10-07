#include "sphere.h"

#include <stdexcept>
#include <ode/collision.h>

namespace ai
{
    ai::Sphere* Sphere::CreateObject(dxSpace* space, float radius, void(* movedCallback)(dxGeom*))
    {
        if (radius < 0.1)
            radius = 0.1;
        auto sphere = dCreateSphere(space, radius);
        return new ai::Sphere(sphere, movedCallback);
    }

    float Sphere::GetRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    void Sphere::SetRadius(float radius)
    {
        if (radius < 0.1)
            radius = 0.1;
        dGeomSphereSetRadius(this->m_geomId, radius);
    }

    Sphere::Sphere(dxGeom* const geomId, void(*movedCallback)(dxGeom*)) : Geom(geomId, movedCallback)
    {
    }
}

#include "cylinder.h"

namespace ai
{
    ai::Cylinder* Cylinder::CreateObject(dxSpace* space, float radius, float length, void(* movedCalback)(dxGeom*))
    {
        // RVA 0x893410 - ODE calls a capsule a "capped cylinder".
        return new Cylinder(dCreateCCylinder(space, radius, length), movedCalback);
    }

    float Cylinder::GetRadius()
    {
        // RVA 0x893370
        float radius = 0.0f;
        float length = 0.0f;
        dGeomCCylinderGetParams(m_geomId, &radius, &length);
        return radius;
    }

    float Cylinder::GetLength()
    {
        // RVA 0x893390
        float radius = 0.0f;
        float length = 0.0f;
        dGeomCCylinderGetParams(m_geomId, &radius, &length);
        return length;
    }

    Cylinder::Cylinder(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
        // RVA 0x8933B0 - the base constructor has already done the work; a cylinder adds
        // nothing of its own beyond its own vtable.
    }
}

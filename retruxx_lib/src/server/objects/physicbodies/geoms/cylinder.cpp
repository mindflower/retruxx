#include "cylinder.h"

namespace ai
{
    ai::Cylinder* Cylinder::CreateObject(dxSpace* space, float radius, float length, void(* movedCalback)(dxGeom*))
    {
        throw std::logic_error("Not implemented");
    }

    float Cylinder::GetRadius()
    {
        throw std::logic_error("Not implemented");
    }

    float Cylinder::GetLength()
    {
        throw std::logic_error("Not implemented");
    }

    Cylinder::Cylinder(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
        throw std::logic_error("Not implemented");
    }
}

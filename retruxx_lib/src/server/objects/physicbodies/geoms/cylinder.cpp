#include "cylinder.h"

namespace ai
{
    ai::Cylinder* Cylinder::CreateObject(dxSpace* space, float radius, float length, void(* movedCalback)(dxGeom*))
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Cylinder::GetRadius()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Cylinder::GetLength()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Cylinder::Cylinder(dxGeom* const geomId, void(*movedCalback)(dxGeom*)) : Geom(geomId, movedCalback)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

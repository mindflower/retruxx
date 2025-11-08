#pragma once
#include "geom.h"

namespace ai
{
    class Cylinder : public ai::Geom
    {
    public:
        static ai::Cylinder* CreateObject(dxSpace* space, float radius, float length, void (*movedCalback)(dxGeom*));
        float GetRadius();
        float GetLength();

    private:
        Cylinder(dxGeom* const geomId, void (*movedCalback)(dxGeom*));
    }; /* size: 0x0018 */
}

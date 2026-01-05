#pragma once
#include "geom.h"

namespace ai
{
    class Sphere : public ai::Geom
    {
    public:
        static ai::Sphere* __fastcall CreateObject(dxSpace* space, float radius, void (*movedCallback)(dxGeom*));
        float GetRadius() const;
        void SetRadius(float radius);

    protected:
        Sphere(dxGeom* const geomId, void (*movedCalback)(dxGeom*));
    }; /* size: 0x0018 */
}

#pragma once
#include "geom.h"

namespace ai
{
    class Cylinder : public Geom
    {
    public:
        float GetRadius();
        float GetLength();
        static Cylinder * CreateObject(dxSpace *,float,float,void (__fastcall*)(dxGeom *));

    private:
        Cylinder(dxGeom * const,void (__fastcall*)(dxGeom *));
    };
}

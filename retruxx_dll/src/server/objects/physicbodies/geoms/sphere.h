#pragma once
#include "geom.h"

namespace ai
{
    class Sphere : public Geom
    {
    public:
        static Sphere * CreateObject(dxSpace *,float,void (__fastcall*)(dxGeom *));
        void SetRadius(float);
        float GetRadius() const ;

    protected:
        Sphere(dxGeom * const,void (__fastcall*)(dxGeom *));

    };
}

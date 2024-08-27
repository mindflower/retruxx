#pragma once
#include "geom.h"

namespace ai
{
    class GeomTransform :  public Geom
    {
    public:
        Geom * GetGeom();
        virtual ~GeomTransform();
        void SetGeom(Geom *);
        static class GeomTransform * CreateObject(dxSpace *,void (__fastcall*)(dxGeom *));

    private:
        GeomTransform(dxGeom * const,void (__fastcall*)(dxGeom *));
        Geom *m_innerGeom;
    };
}

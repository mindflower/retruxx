#pragma once
#include "geom.h"

namespace ai
{
    class Box :  public Geom
    {
    public:
        CVector GetSize() const ;
        static Box * CreateObject(dxSpace *,CVector const &,void (__fastcall*)(dxGeom *));
        void SetSize(CVector const &);

    private:
        Box(dxGeom * const,void (*)(dxGeom *));
    };
}

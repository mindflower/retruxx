#pragma once
#include "geom.h"

namespace ai
{
    class Box :  public Geom
    {
    public:
        CVector GetSize() const ;
        static Box * __fastcall CreateObject(dxSpace *,CVector const &,void (__fastcall*)(dxGeom *));
        void SetSize(CVector const &);

    private:
        Box(dxGeom * const,void (__fastcall*)(dxGeom *));
    };
}

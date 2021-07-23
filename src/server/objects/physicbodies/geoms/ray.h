#pragma once
#include "geom.h"

namespace ai
{
    class Ray : public Geom
    {
    public:
        void SetDirection(CVector const &);
        float GetLength() const ;
        CVector GetDirection() const ;
        static class Ray * __fastcall CreateObject(dxSpace *,float,void (__fastcall*)(dxGeom *));
        void SetLength(float);

    private:
        Ray(dxGeom * const,void (__fastcall*)(dxGeom *));
    };
}

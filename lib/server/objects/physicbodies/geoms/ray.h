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
        static Ray * CreateObject(dxSpace *,float,void (*)(dxGeom *));
        void SetLength(float);

    private:
        Ray(dxGeom * const,void (*)(dxGeom *));
    };
}

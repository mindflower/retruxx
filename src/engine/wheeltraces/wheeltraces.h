#pragma once
#include <math/vector.h>

namespace m3d
{
    class SkidStrip
    {
    public:
        SkidStrip();
    protected:
    private:
        int m_stripSize;
        SkidQuad m_last[2];
        bool m_binUse;
        int m_timeStamp;
        int m_lastFramestamp;
        int m_soilType;
        float m_texCoord;
        CVector m_boundCenter;
        float m_boundRadius;
    };
}

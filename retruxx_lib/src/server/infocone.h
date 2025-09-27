#pragma once
#include <math/vector.h>

namespace ai
{
    class Ray;
    class Sphere;

    class InfoCone
    {
    public:
        InfoCone(const ai::InfoCone&);
        InfoCone();
        ~InfoCone();
        bool SetVehicleId(int vehicleId);
        int GetInfoObjId() const;
        void RenderDebugInfo() const;

    private:
        /* 0x0000 */ int m_vehicleId;
        /* 0x0004 */ CVector m_cameraPos;
        /* 0x0010 */ CVector m_lookAt;
        /* 0x001c */ CVector m_lookVector;
        /* 0x0028 */ CVector m_lookDir;
        /* 0x0034 */ float m_lookDistance;
        /* 0x0038 */ float m_halfConeCos;
        /* 0x003c */ float m_sphereRadiusCoef;
        /* 0x0040 */ ai::Sphere* m_sphere;
        /* 0x0044 */ ai::Ray* m_ray;
        bool TraceTo(const CVector& dst, CVector& newDst, float length) const;
    }; /* size: 0x0048 */
}

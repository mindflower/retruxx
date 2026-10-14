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
        int m_vehicleId;
        CVector m_cameraPos;
        CVector m_lookAt;
        CVector m_lookVector;
        CVector m_lookDir;
        float m_lookDistance;
        float m_halfConeCos;
        float m_sphereRadiusCoef;
        ai::Sphere* m_sphere;
        ai::Ray* m_ray;
        bool TraceTo(const CVector& dst, CVector& newDst, float length) const;
    };
}

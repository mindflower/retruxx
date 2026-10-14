#pragma once
#include <math/vector.h>

namespace ai
{
    class Ray;
    class Sphere;

    class InfoCone
    {
    public:
        InfoCone(ai::InfoCone const&);
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
        bool TraceTo(CVector const& dst, CVector& newDst, float length) const;
    };
}  // namespace ai

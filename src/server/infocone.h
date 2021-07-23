#pragma once
#include <math/vector.h>

namespace ai
{
    class Ray;
    class Sphere;

    class  InfoCone
    {
    public:
        InfoCone();
        void RenderDebugInfo() const;
        int GetInfoObjId() const;
        bool SetVehicleId(int);
        ~InfoCone();

    private:
        bool TraceTo(CVector const&, CVector&, float) const;
        int m_vehicleId;
        CVector m_cameraPos;
        CVector m_lookAt;
        CVector m_lookVector;
        CVector m_lookDir;
        float m_lookDistance;
        float m_halfConeCos;
        float m_sphereRadiusCoef;
        Sphere* m_sphere;
        Ray* m_ray;
    };
}

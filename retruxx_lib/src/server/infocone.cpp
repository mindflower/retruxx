#include "infocone.h"

#include <stdexcept>

#include "objects/physicbodies/geoms/ray.h"
#include "objects/physicbodies/geoms/sphere.h"

namespace ai
{
    InfoCone::InfoCone(const ai::InfoCone&)
    {
        throw std::logic_error("Not implemented");
    }

    InfoCone::InfoCone()
    {
        this->m_vehicleId = -1;
        this->m_cameraPos = CVector(0.0, 0.0, 0.0);
        this->m_lookAt = CVector(0.0, 0.0, 0.0);
        this->m_lookVector = CVector(0.0, 0.0, 0.0);
        this->m_lookDir = CVector(0.0, 0.0, 0.0);
        this->m_lookDistance = 0.0;
        this->m_halfConeCos = 1.0;
        this->m_sphereRadiusCoef = 0.0;
        this->m_sphere = ai::Sphere::CreateObject(0, 1.0, 0);
        this->m_ray = ai::Ray::CreateObject(0, 1.0, 0);
    }

    InfoCone::~InfoCone()
    {
        throw std::logic_error("Not implemented");
    }

    bool InfoCone::SetVehicleId(int vehicleId)
    {
        throw std::logic_error("Not implemented");
    }

    int InfoCone::GetInfoObjId() const
    {
        throw std::logic_error("Not implemented");
    }

    void InfoCone::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool InfoCone::TraceTo(const CVector& dst, CVector& newDst, float length) const
    {
        throw std::logic_error("Not implemented");
    }
}

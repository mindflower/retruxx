#include "infocone.h"

#include <stdexcept>

#include "objects/physicbodies/geoms/ray.h"
#include "objects/physicbodies/geoms/sphere.h"

namespace ai
{
    InfoCone::InfoCone(const ai::InfoCone&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfoCone::SetVehicleId(int vehicleId)
    {
        // TODO: implement InfoCone::SetVehicleId
        //RETRUXX_NOT_IMPLEMENTED;
        return false;
    }

    int InfoCone::GetInfoObjId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfoCone::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfoCone::TraceTo(const CVector& dst, CVector& newDst, float length) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

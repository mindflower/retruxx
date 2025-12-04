#include "raybody.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RayBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RayBody);

    RayBody::RayBody(const ai::RayBodyPrototypeInfo& prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RayBody::RayBody(float length)
    {
        auto* geom = m_pGeoms[0];
        auto* obj = ai::Ray::CreateObject(0, length, ai::CommonGeomMovedCallback);
        m_pGeoms[0]->SetGeom(obj);
        dMassSetSphereTotal(&m_mass, 1.0, 1.0);
    }

    RayBody::RayBody()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RayBody::RayBody(const ai::RayBody& rhs)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RayBody::~RayBody() = default;

    m3d::Object* RayBody::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RayBody::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RayBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    m3d::Class* RayBody::GetClass() const
    {
        return RT_CLASS_LOCAL(RayBody);
    }

    float RayBody::GetLength() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RayBody::SetLength(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RayBody::GetDirection() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RayBody::SetDirection(const CVector&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RayBody::SetMass(float newMassValue)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::Ray* RayBody::_Ray() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Ray* RayBody::_Ray()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

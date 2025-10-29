#include "genericbody.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(GenericBody)
    RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(GenericBody);

    GenericBody::GenericBody(float massValue)
    {
        _ClearGeoms();
        SetMass(massValue);
    }

    GenericBody::GenericBody()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GenericBody::GenericBody(const ai::GenericBody& rhs)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GenericBody::~GenericBody()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* GenericBody::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* GenericBody::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* GenericBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicBody);
    }

    m3d::Class* GenericBody::GetClass() const
    {
        return RT_CLASS_LOCAL(GenericBody);
    }

    void GenericBody::SetMass(float newMassValue)
    {
        if (!m_pGeoms.empty())
        {
            auto aabb = m_pGeoms.front()->GetAabb();
            CVector size;
            size.x = aabb.m_box[3] - aabb.m_box[0];
            size.y = aabb.m_box[4] - aabb.m_box[1];
            size.z = aabb.m_box[5] - aabb.m_box[2];

            auto radius = size.y;
            if ((aabb.m_box[4] - aabb.m_box[1]) <= (aabb.m_box[3] - aabb.m_box[0]))
                radius = size.x;

            if ((aabb.m_box[5] - aabb.m_box[2]) > radius)
                radius = size.z;

            dMassSetSphereTotal(&m_mass, newMassValue, radius * 0.5);
        }
        else
        {
            dMassSetSphereTotal(&m_mass, newMassValue, 1.0);
        }
    }
}

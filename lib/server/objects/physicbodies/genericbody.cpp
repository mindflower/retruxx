#include "genericbody.h"

extern "C" void __cdecl _assert(char const* message, char const* file, unsigned line);

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
        // RVA 0x904510
        // NOTE: the shipped constructor is an assert(0), so a GenericBody can never be
        // built. The class is only here to hold a slot in the body hierarchy.
        _assert("0", "e:\\Builders\\ExMachina\\tmpBuildDir5084\\truxx\\Server\\Objects\\PhysicBodies\\GenericBody.h", 17);
    }

    GenericBody::GenericBody(const ai::GenericBody&)
    {
        // RVA 0x9044E0 - likewise an assert(0); a body is not copyable.
        _assert("0", "e:\\Builders\\ExMachina\\tmpBuildDir5084\\truxx\\Server\\Objects\\PhysicBodies\\GenericBody.h", 18);
    }

    GenericBody::~GenericBody() = default;

    m3d::Object* GenericBody::Clone()
    {
        // RVA 0x904550
        // NOTE: it really does build one and then return nullptr - the constructor
        // asserts, so the return is unreachable tail code the compiler emitted anyway.
        new GenericBody();
        return nullptr;
    }

    m3d::Object* GenericBody::CreateObject()
    {
        // RVA 0x9045A0 - see Clone above.
        new GenericBody();
        return nullptr;
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

#include "raybody.h"

namespace ai
{
    RayBody::RayBody(const ai::RayBodyPrototypeInfo& prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    RayBody::RayBody(float length)
    {
        throw std::logic_error("Not implemented");
    }

    RayBody::RayBody()
    {
        throw std::logic_error("Not implemented");
    }

    RayBody::RayBody(const ai::RayBody& rhs)
    {
        throw std::logic_error("Not implemented");
    }

    RayBody::~RayBody()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RayBody::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RayBody::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RayBody::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RayBody::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    float RayBody::GetLength() const
    {
        throw std::logic_error("Not implemented");
    }

    void RayBody::SetLength(float)
    {
        throw std::logic_error("Not implemented");
    }

    CVector RayBody::GetDirection() const
    {
        throw std::logic_error("Not implemented");
    }

    void RayBody::SetDirection(const CVector&)
    {
        throw std::logic_error("Not implemented");
    }

    void RayBody::SetMass(float newMassValue)
    {
        throw std::logic_error("Not implemented");
    }

    const ai::Ray* RayBody::_Ray() const
    {
        throw std::logic_error("Not implemented");
    }

    ai::Ray* RayBody::_Ray()
    {
        throw std::logic_error("Not implemented");
    }
}

#include "genericbody.h"

namespace ai
{
    GenericBody::GenericBody(float massValue)
    {
        throw std::logic_error("Not implemented");
    }

    GenericBody::GenericBody()
    {
        throw std::logic_error("Not implemented");
    }

    GenericBody::GenericBody(const ai::GenericBody& rhs)
    {
        throw std::logic_error("Not implemented");
    }

    GenericBody::~GenericBody()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* GenericBody::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* GenericBody::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* GenericBody::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* GenericBody::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void GenericBody::SetMass(float newMassValue)
    {
        throw std::logic_error("Not implemented");
    }
}

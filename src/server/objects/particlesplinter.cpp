#include "particlesplinter.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ParticleSplinter)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ParticleSplinter);

    Obj* ParticleSplinterPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ParticleSplinter::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSplinter::ParticleSplinter(ParticleSplinterPrototypeInfo const& prototype) : DummyObject(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSplinterPrototypeInfo const* ParticleSplinter::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ParticleSplinter::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSplinter::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    ParticleSplinter::~ParticleSplinter()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ParticleSplinter::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ParticleSplinter::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void ParticleSplinter::CheckDisablePhysics()
    {
        throw std::logic_error("Not implemented");
    }
}

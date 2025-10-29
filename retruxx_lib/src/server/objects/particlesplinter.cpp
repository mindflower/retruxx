#include "particlesplinter.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ParticleSplinter)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ParticleSplinter);

    Obj* ParticleSplinterPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ParticleSplinter::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    ParticleSplinter::ParticleSplinter(ParticleSplinterPrototypeInfo const& prototype) : DummyObject(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticleSplinterPrototypeInfo const* ParticleSplinter::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ParticleSplinter::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSplinter::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ParticleSplinter::~ParticleSplinter()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ParticleSplinter::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ParticleSplinter::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ParticleSplinter::CheckDisablePhysics()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

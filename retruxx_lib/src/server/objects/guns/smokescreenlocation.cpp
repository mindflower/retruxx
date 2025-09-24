#include "smokescreenlocation.h"

#include <stdexcept>

namespace ai
{
    SmokeScreenLocationPrototypeInfo::SmokeScreenLocationPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* SmokeScreenLocationPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* SmokeScreenLocation::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* SmokeScreenLocation::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* SmokeScreenLocation::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* SmokeScreenLocation::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void SmokeScreenLocation::OnObjectIn(ai::Obj* object)
    {
        throw std::logic_error("Not implemented");
    }

    void SmokeScreenLocation::OnObjectOut(ai::Obj* object)
    {
        throw std::logic_error("Not implemented");
    }

    SmokeScreenLocation::~SmokeScreenLocation()
    {
        throw std::logic_error("Not implemented");
    }

    SmokeScreenLocation::SmokeScreenLocation(const ai::SmokeScreenLocationPrototypeInfo& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    const ai::SmokeScreenLocationPrototypeInfo* SmokeScreenLocation::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }
}

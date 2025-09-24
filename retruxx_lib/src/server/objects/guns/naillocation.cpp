#include "naillocation.h"

#include <stdexcept>

namespace ai
{
    NailLocationPrototypeInfo::NailLocationPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* NailLocationPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* NailLocation::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* NailLocation::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* NailLocation::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* NailLocation::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void NailLocation::OnObjectIn(ai::Obj* object)
    {
        throw std::logic_error("Not implemented");
    }

    void NailLocation::OnObjectOut(ai::Obj* object)
    {
        throw std::logic_error("Not implemented");
    }

    NailLocation::~NailLocation()
    {
        throw std::logic_error("Not implemented");
    }

    NailLocation::NailLocation(const ai::NailLocationPrototypeInfo& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    const ai::NailLocationPrototypeInfo* NailLocation::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }
}

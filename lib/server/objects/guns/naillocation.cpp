#include "naillocation.h"

#include <stdexcept>

namespace ai
{
    NailLocationPrototypeInfo::NailLocationPrototypeInfo() = default;

    ai::Obj* NailLocationPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* NailLocation::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* NailLocation::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* NailLocation::GetBaseClass()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* NailLocation::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NailLocation::OnObjectIn(ai::Obj* object)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void NailLocation::OnObjectOut(ai::Obj* object)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NailLocation::~NailLocation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    NailLocation::NailLocation(ai::NailLocationPrototypeInfo const& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::NailLocationPrototypeInfo const* NailLocation::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

#include "smokescreenlocation.h"

#include <stdexcept>

namespace ai
{
    SmokeScreenLocationPrototypeInfo::SmokeScreenLocationPrototypeInfo() = default;

    ai::Obj* SmokeScreenLocationPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SmokeScreenLocation::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* SmokeScreenLocation::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* SmokeScreenLocation::GetBaseClass()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* SmokeScreenLocation::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SmokeScreenLocation::OnObjectIn(ai::Obj* object)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SmokeScreenLocation::OnObjectOut(ai::Obj* object)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SmokeScreenLocation::~SmokeScreenLocation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SmokeScreenLocation::SmokeScreenLocation(ai::SmokeScreenLocationPrototypeInfo const& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::SmokeScreenLocationPrototypeInfo const* SmokeScreenLocation::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

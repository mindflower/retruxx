#include "engineoillocation.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(EngineOilLocation)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(EngineOilLocation);

    EngineOilLocationPrototypeInfo::EngineOilLocationPrototypeInfo() = default;

    Obj* EngineOilLocationPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    EngineOilLocation::~EngineOilLocation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    EngineOilLocation::EngineOilLocation(EngineOilLocationPrototypeInfo const& prototypeInfo) : TemporaryLocation(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* EngineOilLocation::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* __fastcall EngineOilLocation::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* __fastcall EngineOilLocation::GetBaseClass()
    {
        return RT_CLASS_LOCAL(TemporaryLocation);
    }

    m3d::Class* EngineOilLocation::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    EngineOilLocationPrototypeInfo const* EngineOilLocation::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void EngineOilLocation::OnObjectIn(Obj* object)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void EngineOilLocation::OnObjectOut(Obj* object)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

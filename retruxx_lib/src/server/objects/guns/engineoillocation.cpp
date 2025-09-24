#include "engineoillocation.h"

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(EngineOilLocation)
    RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(EngineOilLocation);

    EngineOilLocationPrototypeInfo::EngineOilLocationPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* EngineOilLocationPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    EngineOilLocation::~EngineOilLocation()
    {
        throw std::logic_error("Not implemented");
    }

    EngineOilLocation::EngineOilLocation(const EngineOilLocationPrototypeInfo& prototypeInfo): TemporaryLocation(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    EngineOilLocation::EngineOilLocation(const EngineOilLocation& location): TemporaryLocation(location)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* EngineOilLocation::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* __fastcall EngineOilLocation::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* __fastcall EngineOilLocation::GetBaseClass()
    {
        return RT_CLASS_LOCAL(TemporaryLocation);
    }

    m3d::Class* EngineOilLocation::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const EngineOilLocationPrototypeInfo* EngineOilLocation::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void EngineOilLocation::OnObjectIn(Obj* object)
    {
        throw std::logic_error("Not implemented");
    }

    void EngineOilLocation::OnObjectOut(Obj* object)
    {
        throw std::logic_error("Not implemented");
    }
}

#include "temporarylocation.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(TemporaryLocation)
    RT_CLASS_EXPORTS_END;

    RT_CLASS_DEFINE(TemporaryLocation);

    TemporaryLocationPrototypeInfo::TemporaryLocationPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* TemporaryLocationPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool TemporaryLocationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TemporaryLocation::CreateEffectNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TemporaryLocation::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TemporaryLocation::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TemporaryLocation::TemporaryLocation(TemporaryLocationPrototypeInfo const& prototype) : Location(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TemporaryLocation::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TemporaryLocationPrototypeInfo const* TemporaryLocation::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TemporaryLocation::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* TemporaryLocation::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Location);
    }

    void TemporaryLocation::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TemporaryLocation::~TemporaryLocation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TemporaryLocation::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* TemporaryLocation::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TemporaryLocation::OnActivate()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

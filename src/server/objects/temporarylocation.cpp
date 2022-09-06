#include "temporarylocation.h"

#include <stdexcept>

namespace ai
{
    TemporaryLocationPrototypeInfo::TemporaryLocationPrototypeInfo()
    {
    }

    Obj* TemporaryLocationPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool TemporaryLocationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void TemporaryLocation::CreateEffectNode()
    {
        throw std::logic_error("Not implemented");
    }

    void TemporaryLocation::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void TemporaryLocation::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    TemporaryLocation::TemporaryLocation(TemporaryLocationPrototypeInfo const& prototype) : Location(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TemporaryLocation::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    TemporaryLocationPrototypeInfo const* TemporaryLocation::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void TemporaryLocation::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* TemporaryLocation::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Location);
    }

    void TemporaryLocation::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    TemporaryLocation::~TemporaryLocation()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TemporaryLocation::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* TemporaryLocation::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void TemporaryLocation::OnActivate()
    {
        throw std::logic_error("Not implemented");
    }
}

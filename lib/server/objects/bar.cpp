#include "bar.h"
#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Bar)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Bar);

    Obj* BarPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BarPrototypeInfo::BarPrototypeInfo()
    {
        m_bWithBarman = true;
    }

    bool BarPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = BuildingPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeBoolAttrib(m_bWithBarman, xmlNode, "WithBarman");
            return 1;
        }
        return result;
    }

    void BarPrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Bar::bWithBarman() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Bar::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Bar::Bar(BarPrototypeInfo const& prototypeInfo) : Building(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Bar::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bar::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Bar::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BarPrototypeInfo const* Bar::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Bar::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Building);
    }

    void Bar::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bar::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Bar::~Bar()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Bar::CreateBarman()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Bar::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Bar::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

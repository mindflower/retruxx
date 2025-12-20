#include "bar.h"
#include "core/ini.h"
#include "npc.h"
#include <core/log.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Bar)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Bar);

    Obj* BarPrototypeInfo::CreateTargetObject() const
    {
        return new Bar(*this);
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

    bool Bar::CanChildBeAdded(m3d::Class* pClass) const
    {
        return Building::CanChildBeAdded(pClass) || pClass->IsKindOf(&ai::Npc::m_classNpc);
    }

    Bar::Bar(BarPrototypeInfo const& prototypeInfo) : Building(prototypeInfo)
    {
        m_barmanId = -1;
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
        return RT_CLASS_LOCAL(Bar);
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

    void Bar::AddChild(Obj* pObj)
    {
        Building::AddChild(pObj);
        if (pObj && pObj->IsKindOf(&ai::Npc::m_classNpc) && ((Npc*)pObj)->GetNpcType() == Npc::NPC_BARMAN)
        {
            if (m_barmanId == -1)
            {
                m_barmanId = pObj->GetId();
            }
            else
            {
                M3D_LOG_INFO(
                    "Bar::AddChild warning - the barman already exists; dublicate barman-npc " + CStr(pObj->GetName()) +
                    " would be deleted");
                pObj->Remove();
            }
        }
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

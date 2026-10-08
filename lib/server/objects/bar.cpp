#include "bar.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "core/ini.h"
#include "npc.h"
#include <core/kernel.h>
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

    void BarPrototypeInfo::_InternalCopyFrom(PrototypeInfo const& rhs)
    {
        // RVA 0x6EA2F0 - a plain assignment from another prototype of the same class.
        *this = static_cast<BarPrototypeInfo const&>(rhs);
    }

    bool Bar::bWithBarman() const
    {
        // RVA 0x6E9C90 - NOTE: the prototype info is dereferenced without a null check.
        return GetPrototypeInfo()->m_bWithBarman;
    }

    bool Bar::CanChildBeAdded(m3d::Class* pClass) const
    {
        return Building::CanChildBeAdded(pClass) || pClass->IsKindOf(&ai::Npc::m_classNpc);
    }

    Bar::Bar(BarPrototypeInfo const& prototypeInfo) : Building(prototypeInfo)
    {
        m_barmanId = -1;
    }

    bool Bar::RemoveChild(Obj* pChild)
    {
        // RVA 0x6E9DA0 - NOTE: the base class' result is discarded; only the barman's departure is
        // reported back.
        Building::RemoveChild(pChild);
        if (!pChild || pChild->GetId() != m_barmanId)
        {
            return false;
        }
        m_barmanId = -1;
        return true;
    }

    void Bar::Update(float elapsedTime, unsigned workTime)
    {
        ai::Obj::Update(elapsedTime, workTime);
        if (elapsedTime != 0.0)
            CreateBarman();
    }

    m3d::Class* Bar::GetClass() const
    {
        return RT_CLASS_LOCAL(Bar);
    }

    BarPrototypeInfo const* Bar::GetPrototypeInfo() const
    {
        // RVA 0x6EA2C0
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BarPrototypeInfo const);
    }

    m3d::Class* Bar::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Building);
    }

    void Bar::CreateChildren()
    {
        // RVA 0x6E9C50 - the bar adds nothing of its own; the barman is hired on the first update.
        Obj::CreateChildren();
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
        // RVA 0x6E9C40 - the base class does all the work.
    }

    void Bar::CreateBarman()
    {
        // RVA 0x6EA310 - a bar that wants a barman and has none hires one named after itself.
        if (!bWithBarman() || m_barmanId != -1)
        {
            return;
        }
        int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("NPC"));
        int const barmanId =
            theObjects->CreateNewObject(prototypeId, (CStr(GetName()) + CStr("_Barman")).c_str(), -1, -1);
        // NOTE: the new npc is not checked for null before its type is written.
        auto* const barman = static_cast<Npc*>(theObjects->GetEntityByObjId(barmanId));
        barman->m_npcType = Npc::NPC_BARMAN;
        AddChild(barman);
    }

    m3d::Object* Bar::CreateObject()
    {
        // RVA 0x6E9FA0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Bar::Clone()
    {
        // RVA 0x6E9DE0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

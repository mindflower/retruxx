#include "building.h"
#include "base/prototypemanager.h"
#include "core/ini.h"
#include "npc.h"

#include <algorithm>
#include <core/kernel.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Building)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Building);

    bool BuildingPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            CStr strBuildingType;
            m3d::SafeStrAttrib(strBuildingType, xmlNode, "BuildingType");
            m_buildingType = ai::Building::GetBuildingTypeByName(strBuildingType);
        }
        return result;
    }

    BuildingPrototypeInfo::BuildingPrototypeInfo()
    {
        m_buildingType = NUM_BUILDINGTYPES;
    }

    Obj* BuildingPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x704910
        return new Building(*this);
    }

    Building::Building(BuildingPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
    }

    m3d::Class* Building::GetClass() const
    {
        // RVA 0x703AC0
        return RT_CLASS_LOCAL(Building);
    }

    bool Building::RemoveChild(Obj* pChild)
    {
        // RVA 0x7047D0
        Obj::RemoveChild(pChild);
        if (pChild && pChild->IsKindOf(&ai::Npc::m_classNpc))
        {
            auto const it = std::find(m_npcs.begin(), m_npcs.end(), static_cast<Npc*>(pChild));
            if (it != m_npcs.end())
            {
                m_npcs.erase(it);
            }
        }
        // NOTE: false is returned even when the npc was found and dropped.
        return false;
    }

    BuildingType Building::GetBuildingType() const
    {
        // RVA 0x45C930
        BuildingPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        return prototypeInfo ? prototypeInfo->m_buildingType : INVALID_BUILDINGTYPE;
    }

    void Building::Remove()
    {
        // RVA 0x704480 - the building takes its npcs with it.
        Obj::Remove();
        for (Npc* const npc : m_npcs)
        {
            npc->Remove();
        }
    }

    m3d::Class* Building::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    BuildingType Building::GetBuildingTypeByName(CStr const& buildingTypeName)
    {
        for (int i = 0; i < 5; ++i)
        {
            if (m_buildingTypeNames[i] == buildingTypeName)
            {
                return static_cast<BuildingType>(i);
            }
        }
        return INVALID_BUILDINGTYPE;
    }

    bool Building::CanChildBeAdded(m3d::Class* pClass) const
    {
        return Obj::CanChildBeAdded(pClass) || pClass->IsKindOf(&ai::Npc::m_classNpc);
    }

    retruxx::vector<Npc*> const& Building::GetNpcs() const
    {
        // RVA 0x49BBC0
        return m_npcs;
    }

    void Building::AddChild(Obj* pObj)
    {
        Obj::AddChild(pObj);
        if (pObj)
        {
            if (pObj->IsKindOf(&ai::Npc::m_classNpc))
            {
                pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
                m_npcs.push_back((ai::Npc*)pObj);
            }
        }
    }

    CStr Building::GetBuildingTypeName(BuildingType buildingType)
    {
        // RVA 0x704330 - INVALID_BUILDINGTYPE, and anything past it, has no name.
        if (buildingType < NUM_BUILDINGTYPES)
        {
            return m_buildingTypeNames[buildingType];
        }
        return CStr();
    }

    BuildingPrototypeInfo const* Building::GetPrototypeInfo() const
    {
        // RVA 0x7047A0
        return RT_DYNCAST(
            thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BuildingPrototypeInfo const);
    }

    Building::~Building()
    {
        // RVA 0x704870 - the npc vector holds no ownership of the npcs it points at.
    }

    m3d::Object* Building::CreateObject()
    {
        // RVA 0x7040F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Building::Clone()
    {
        // RVA 0x703F30
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

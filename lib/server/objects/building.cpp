#include "building.h"
#include "core/ini.h"
#include "npc.h"

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Building::Building(BuildingPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
    }

    m3d::Class* Building::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Building::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BuildingType Building::GetBuildingType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Building::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

    CStr Building::GetBuildingTypeName(BuildingType)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    BuildingPrototypeInfo const* Building::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Building::~Building()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Building::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Building::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

#include "building.h"
#include "core/ini.h"

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

    Building::Building(BuildingPrototypeInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    BuildingType Building::GetBuildingTypeByName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Building::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<Npc*> const& Building::GetNpcs() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Building::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

#include "town.h"
#include "article.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SpawnCaravanToLocation)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, GetOpenGateToPlayer)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetOpenGateToPlayer)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetRuined)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, IsRuined)
{
    throw retruxx::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Town)
        RT_CLASS_EXPORT(Town, m3d::METHOD, SpawnCaravanToLocation, "", "", "")
        RT_CLASS_EXPORT(Town, m3d::METHOD, GetOpenGateToPlayer, "", "", "")
        RT_CLASS_EXPORT(Town, m3d::METHOD, SetOpenGateToPlayer, "", "", "")
        RT_CLASS_EXPORT(Town, m3d::METHOD, SetRuined, "", "", "")
        RT_CLASS_EXPORT(Town, m3d::METHOD, IsRuined, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Town);

    TownPrototypeInfo::TownPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* TownPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void TownPrototypeInfo::PostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void TownPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool TownPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void TownPrototypeInfo::_LoadFromXmlResourceIdToRandomCoeffMap(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::PrepareVehicleForPath(Vehicle*, retruxx::vector<CVector2, retruxx::allocator<CVector2>> const&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Town::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Town::CanChildBeAdded(m3d::Class*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::AddChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::SetRuined(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Town::Town(TownPrototypeInfo const& prototype) : Settlement(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::SendVehicleOff(Vehicle*, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Team* Town::SpawnCaravanToLocation(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Team* Town::SpawnCaravanToLocation(Location*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Town::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Workshop* Town::GetShop()
    {
        throw retruxx::logic_error("Not implemented");
    }

    TownPrototypeInfo const* Town::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Town::GetObjResourceCoeff(Obj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<Building*> const& Town::GetAllBuildings() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Town::IsRuined() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Town::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    void Town::CreateChildren()
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<Building*> Town::GetBuildingByType(BuildingType) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Town::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::GetBuildingTypes(retruxx::set<BuildingType>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::SetPath(TownPath, retruxx::vector<CVector2> const&, retruxx::vector<CVector> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Workshop* Town::GetWorkshopByObject(Obj const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::SendVehicleIn(Vehicle*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Town::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::GetPath(TownPath, retruxx::vector<CVector2>&, retruxx::vector<CVector>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::SetOpenGateToPlayer(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::GetPropertiesIDs(retruxx::set<int>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::RelinkSceneGraphNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Town::OnEvent(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<int, retruxx::allocator<int>> Town::GetDynamicQuestIds() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Workshop* Town::GetWorkshop()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Town::GetOpenGateToPlayer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::GetPropertiesNames(retruxx::set<CStr>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::Registration()
    {
        m_propertiesMap["MaxDefenders"] = 64;
        m_propertiesMap["OpenGateToPlayer"] = 65;
    }

    m3d::Class* Town::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::GenerateNewDynamicQuestIfNeeded()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Workshop* Town::GetWorkshopByPrototypeId(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_InternalCreateVisualPart()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Town::~Town()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Town::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Town::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_OnPlayerVehicleHorn(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Town::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Team* Town::_GetDefendingTeam()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Town::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_OnObjectEntersLocation(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_InitializeWorkshops()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_OnTargetReached(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_OnInCinematic(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_OpenGates()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_StartCinematic(Vehicle*, retruxx::vector<CVector, retruxx::allocator<CVector>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_OnSkipCinematic(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_CloseGates()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_OnObjectLeavesLocation(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Town::_SynchronizeGatesState()
    {
        throw retruxx::logic_error("Not implemented");
    }

    DynamicQuest* Town::_GenerateDynamicQuest()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

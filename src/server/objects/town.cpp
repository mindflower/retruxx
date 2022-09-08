#include "town.h"
#include "article.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SpawnCaravanToLocation)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, GetOpenGateToPlayer)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetOpenGateToPlayer)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetRuined)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, IsRuined)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    Obj* TownPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void TownPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void TownPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool TownPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void TownPrototypeInfo::_LoadFromXmlResourceIdToRandomCoeffMap(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::PrepareVehicleForPath(Vehicle*, std::vector<CVector2, std::allocator<CVector2>> const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Town::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Town::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::SetRuined(bool)
    {
        throw std::logic_error("Not implemented");
    }

    Town::Town(TownPrototypeInfo const& prototype) : Settlement(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::SendVehicleOff(Vehicle*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    Team* Town::SpawnCaravanToLocation(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    Team* Town::SpawnCaravanToLocation(Location*)
    {
        throw std::logic_error("Not implemented");
    }

    CStr Town::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    Workshop* Town::GetShop()
    {
        throw std::logic_error("Not implemented");
    }

    TownPrototypeInfo const* Town::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    float Town::GetObjResourceCoeff(Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<Building*> const& Town::GetAllBuildings() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Town::IsRuined() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Town::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    void Town::CreateChildren()
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<Building*> Town::GetBuildingByType(BuildingType) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Town::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::GetBuildingTypes(std::set<BuildingType>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::SetPath(TownPath, std::vector<CVector2> const&, std::vector<CVector> const&)
    {
        throw std::logic_error("Not implemented");
    }

    Workshop* Town::GetWorkshopByObject(Obj const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::SendVehicleIn(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Town::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::GetPath(TownPath, std::vector<CVector2>&, std::vector<CVector>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::SetOpenGateToPlayer(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::GetPropertiesIDs(std::set<int>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::RelinkSceneGraphNode()
    {
        throw std::logic_error("Not implemented");
    }

    int Town::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<int, std::allocator<int>> Town::GetDynamicQuestIds() const
    {
        throw std::logic_error("Not implemented");
    }

    Workshop* Town::GetWorkshop()
    {
        throw std::logic_error("Not implemented");
    }

    bool Town::GetOpenGateToPlayer() const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::GetPropertiesNames(std::set<CStr>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Town::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::GenerateNewDynamicQuestIfNeeded()
    {
        throw std::logic_error("Not implemented");
    }

    Workshop* Town::GetWorkshopByPrototypeId(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }

    Town::~Town()
    {
        throw std::logic_error("Not implemented");
    }

    bool Town::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void Town::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool Town::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_OnPlayerVehicleHorn(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Town::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    Team* Town::_GetDefendingTeam()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Town::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_OnObjectEntersLocation(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_InitializeWorkshops()
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_OnTargetReached(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_OnInCinematic(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_OpenGates()
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_StartCinematic(Vehicle*, std::vector<CVector, std::allocator<CVector>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_OnSkipCinematic(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_CloseGates()
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_OnObjectLeavesLocation(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Town::_SynchronizeGatesState()
    {
        throw std::logic_error("Not implemented");
    }

    DynamicQuest* Town::_GenerateDynamicQuest()
    {
        throw std::logic_error("Not implemented");
    }
}

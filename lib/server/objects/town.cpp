#include "town.h"
#include "article.h"
#include "include/math/random.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SpawnCaravanToLocation)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, GetOpenGateToPlayer)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetOpenGateToPlayer)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetRuined)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, IsRuined)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        m_maxDefenders = 1;
        m_DesiredGunsInWorkshop = 0;
        m_GunAffixesCount = 0;
        m_CabinsAndBasketsAffixesCount = 0;
        m_numCollisionLayersBelowVehicle = 2;
        m_bCollisionTrimeshAllowed = true;
    }

    Obj* TownPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TownPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void TownPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool TownPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::SettlementPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            // TODO: implement TownPrototypeInfo::LoadFromXML
        }
        return result;
    }

    void TownPrototypeInfo::_LoadFromXmlResourceIdToRandomCoeffMap(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::PrepareVehicleForPath(Vehicle*, retruxx::vector<CVector2, retruxx::allocator<CVector2>> const&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Town::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Town::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SetRuined(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Town::Town(TownPrototypeInfo const& prototype) : Settlement(prototype), m_gateTime(0.0, 0.0)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SendVehicleOff(Vehicle*, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* Town::SpawnCaravanToLocation(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* Town::SpawnCaravanToLocation(Location*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Town::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop* Town::GetShop()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    TownPrototypeInfo const* Town::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Town::GetObjResourceCoeff(Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<Building*> const& Town::GetAllBuildings() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Town::IsRuined() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Town::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    void Town::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<Building*> Town::GetBuildingByType(BuildingType) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Town::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::GetBuildingTypes(retruxx::set<BuildingType>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SetPath(TownPath, retruxx::vector<CVector2> const&, retruxx::vector<CVector> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop* Town::GetWorkshopByObject(Obj const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SendVehicleIn(Vehicle*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Town::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::GetPath(TownPath, retruxx::vector<CVector2>&, retruxx::vector<CVector>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SetOpenGateToPlayer(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::GetPropertiesIDs(retruxx::set<int>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::RelinkSceneGraphNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Town::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<int, retruxx::allocator<int>> Town::GetDynamicQuestIds() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop* Town::GetWorkshop()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Town::GetOpenGateToPlayer() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::GetPropertiesNames(retruxx::set<CStr>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::Registration()
    {
        m_propertiesMap["MaxDefenders"] = 64;
        m_propertiesMap["OpenGateToPlayer"] = 65;
    }

    m3d::Class* Town::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::GenerateNewDynamicQuestIfNeeded()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop* Town::GetWorkshopByPrototypeId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Town::~Town()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Town::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Town::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OnPlayerVehicleHorn(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Town::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* Town::_GetDefendingTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Town::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OnObjectEntersLocation(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_InitializeWorkshops()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OnTargetReached(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OnInCinematic(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OpenGates()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_StartCinematic(Vehicle*, retruxx::vector<CVector, retruxx::allocator<CVector>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OnSkipCinematic(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_CloseGates()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OnObjectLeavesLocation(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_SynchronizeGatesState()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuest* Town::_GenerateDynamicQuest()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

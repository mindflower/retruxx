#include "town.h"
#include "article.h"
#include "include/math/random.h"
#include "vehicle.h"
#include "team.h"
#include <stdexcept>
#include <core/kernel.h>
#include <server/processmanager.h>
#include "player.h"
#include "base/prototypemanager.h"

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
        return new Town(*this);
    }

    void TownPrototypeInfo::PostLoad()
    {
        SettlementPrototypeInfo::PostLoad();
        // TODO: implement ::PostLoad
    }

    void TownPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        SettlementPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
        // TODO: implement ::RefreshFromXml
    }

    bool TownPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::SettlementPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_FROM_MODEL);
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

    int Town::GetPropertyId(char const* propName) const
    {
        auto it = Town::m_propertiesMap.find(propName);
        if (it != Town::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::Settlement::GetPropertyId(propName);
    }

    bool Town::CanChildBeAdded(m3d::Class* pClass) const
    {
        return Settlement::CanChildBeAdded(pClass) || pClass->IsKindOf(&ai::Building::m_classBuilding);
    }

    void Town::AddChild(Obj* pObj)
    {
        Settlement::AddChild(pObj);
        if (pObj)
        {
            if (pObj->IsKindOf(&ai::Building::m_classBuilding))
            {
                pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
                pObj->SetBelong(GetBelong());
                m_buildings.push_back((ai::Building*)pObj);
            }
        }
    }

    void Town::SetRuined(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Town::Town(TownPrototypeInfo const& prototype) : Settlement(prototype), m_gateTime(0.0, 0.0)
    {
        m_gateState = GATE_CLOSED;
        m_PointOfViewInInterface.x = 5.0f;
        m_PointOfViewInInterface.y = 20.0f;
        m_PointOfViewInInterface.z = 5.0f;

        m_targetClasses.insert(RT_CLASS_LOCAL(Vehicle));
        m_gateNode = 0;
        m_playerEnteringTownCount = 0;
        m_PlayerPathIndex = -1;
        m_QuestsGenerated = 0;
        m_VehicleShouldBeMoved = 0;
        m_NewPosForVehicle = ZeroVector;
        m_NewDirForVehicle = {0.0, 0.0, 1.0};
        m_VehicleToBeMoved = 0;
        m_bRuined = 0;
        m_timeFromLastEnterTown = 0.0;
        m_OldCameraMode = CM_LAST;
        m_bOpenGateToPlayer = 1;
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
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), TownPrototypeInfo const);
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

    void Town::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Settlement::LoadFromXML(xmlFile, xmlNode);
        // TODO: implement Town::LoadFromXML
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

    bool Town::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId == 64)
        {
            m_maxDefenders = newValue.GetAsID();
            M3D_ASSERT(m_maxDefenders <= MAX_VEHICLES_IN_TEAM);
            return 1;
        }
        else if (propertyId == 65)
        {
            m_bOpenGateToPlayer = newValue.GetAsID() != 0;
            return 1;
        }
        return SimplePhysicObj::SetPropertyById(propertyId, newValue);
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

    int Town::OnEvent(Event const& evn)
    {
        int result = ai::Settlement::OnEvent(evn);
        switch (evn.m_eventId)
        {
        case GE_OBJECT_ENTERS_LOCATION:
            _OnObjectEntersLocation(evn);
            result = 1;
            break;

        case GE_OBJECT_LEAVES_LOCATION:
            _OnObjectLeavesLocation(evn);
            result = 1;
            break;

        case GE_TARGET_REACHED:
            _OnTargetReached(evn);
            result = 1;
            break;

        case GE_SKIP_CINEMATIC:
        case GE_END_CINEMATIC:
            _OnSkipCinematic(evn);
            result = 1;
            break;
        case GE_IN_CINEMATIC:
        case GE_CINEMATIC_ENTER_FADE_IN:
            _OnInCinematic(evn);
            result = 1;
            break;

        case GE_PLAYER_VEHICLE_HORN:
            _OnPlayerVehicleHorn(evn);
            result = 1;
            break;

        default:
            return result;
        }
        return result;
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

    void Town::Update(float elapsedTime, unsigned workTime)
    {
        SimplePhysicObj::Update(elapsedTime, workTime);
        // TODO: implement Town::Update
    }

    void Town::Registration()
    {
        m_propertiesMap["MaxDefenders"] = 64;
        m_propertiesMap["OpenGateToPlayer"] = 65;
    }

    m3d::Class* Town::GetClass() const
    {
        return RT_CLASS_LOCAL(Town);
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
        SimplePhysicObj::_InternalCreateVisualPart();
        // TODO: implement Town::_InternalCreateVisualPart
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
        Settlement::_InternalPostLoad();
        ai::theProcessManager->PostMessageA(2, ai::thePlayer->GetId(), GetId(), 0.0f, {60}, {}, 1);
        ai::theProcessManager->PostMessageA(2, ai::thePlayer->GetId(), GetId(), 0.0f, {61}, {}, 1);
        ai::theProcessManager->PostMessageA(2, ai::thePlayer->GetId(), GetId(), 0.0f, {64}, {}, 1);
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
        // TODO: implement Town::_OnObjectEntersLocation
        // RETRUXX_NOT_IMPLEMENTED;
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
        if (m_VehicleShouldBeMoved)
        {
            if (m_VehicleToBeMoved)
            {
                if (sqrt(
                        m_NewDirForVehicle.x * m_NewDirForVehicle.x + m_NewDirForVehicle.y * m_NewDirForVehicle.y +
                        m_NewDirForVehicle.z * m_NewDirForVehicle.z) > 0.1)
                    m_VehicleToBeMoved->SetDirection(m_NewDirForVehicle);
                m_VehicleToBeMoved->SetGamePositionOnGround(m_NewPosForVehicle, 1, 0);
                m_VehicleToBeMoved->SetLinearVelocity(ZeroVector);
                m_VehicleToBeMoved->SetAngularVelocity(ZeroVector);
            }
            m_VehicleShouldBeMoved = 0;
        }
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
        // TODO: implement Town::_OnObjectLeavesLocation
        // RETRUXX_NOT_IMPLEMENTED;
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

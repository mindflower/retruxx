#include "town.h"
#include "article.h"
#include "include/math/random.h"
#include "vehicle.h"
#include "team.h"
#include "workshop.h"
#include <cstdlib>
#include <stdexcept>
#include <core/kernel.h>
#include <core/ini.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <m3dapp.h>
#include <landscape.h>
#include <world.h>
#include <server/processmanager.h>
#include <server/event.h>
#include <server/relationship.h>
#include <server/resourcemanager.h>
#include <server/server.h>
#include "player.h"
#include "ware.h"
#include "gadget.h"
#include "guns/gun.h"
#include "guns/compoundgun.h"
#include "cabin.h"
#include "basket.h"
#include "chassis.h"
#include "base/prototypemanager.h"
#include "base/prototypeinfo.h"
#include "base/objcontainer.h"
#include "physicbodies/physicbody.h"

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

    namespace
    {
        // Mirrors ai::Town::GetShop / GetWorkshop / GetBuildingByType: the building type comes
        // from the building's prototype info, falling back to INVALID_BUILDINGTYPE when absent.
        BuildingType BuildingTypeOf(Building const* building)
        {
            auto const* proto = building->GetPrototypeInfo();
            return proto ? proto->m_buildingType : INVALID_BUILDINGTYPE;
        }
    }  // namespace

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
        m_GunGeneratorPrototypeId = ai::thePrototypeManager->GetPrototypeId(m_gunGeneratorPrototypeName);
        m_GunAffixGeneratorPrototypeId = ai::thePrototypeManager->GetPrototypeId(m_gunAffixGeneratorPrototypeName);
        m_CabinsAndBasketsAffixGeneratorPrototypeId =
            ai::thePrototypeManager->GetPrototypeId(m_cabinsAndBasketsAffixGeneratorPrototypeName);
        for (auto& article : m_Articles)
        {
            article.PostLoad();
        }
    }

    void TownPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        SimplePhysicObjPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
        // TODO: recompute m_gateOpeningTime / m_gateClosingTime from the gate model's
        // animation lengths (needs m3d::DataServer::GetItemByName + EngineConfig::GetAnimationLength).
    }

    bool TownPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::SettlementPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (!result)
        {
            return result;
        }

        _SetGeomType(GEOM_TYPE_FROM_MODEL);
        m3d::SafeStrAttrib(m_musicName, xmlNode, "MusicName");
        m3d::SafeStrAttrib(m_gateModelName, xmlNode, "GateModelFile");

        if (!xmlNode->IsEmpty())
        {
            if (auto const* attr = xmlNode->GetAttribute("MaxDefenders"))
            {
                m_maxDefenders = atoi(attr);
            }
        }
        M3D_ASSERT(m_maxDefenders <= MAX_VEHICLES_IN_TEAM);

        m3d::SafeStrAttrib(m_gunGeneratorPrototypeName, xmlNode, "GunGenerator");
        if (!xmlNode->IsEmpty())
        {
            if (auto const* attr = xmlNode->GetAttribute("DesiredGunsInWorkshop"))
            {
                int const value = atoi(attr);
                if (value >= 0)
                {
                    m_DesiredGunsInWorkshop = value;
                }
            }
        }

        m3d::SafeStrAttrib(m_gunAffixGeneratorPrototypeName, xmlNode, "GunAffixGenerator");
        if (!xmlNode->IsEmpty())
        {
            if (auto const* attr = xmlNode->GetAttribute("GunAffixesCount"))
            {
                int const value = atoi(attr);
                if (value >= 0)
                {
                    m_GunAffixesCount = value;
                }
            }
        }

        m3d::SafeStrAttrib(m_cabinsAndBasketsAffixGeneratorPrototypeName, xmlNode, "CabinsAndBasketsAffixGenerator");
        if (!xmlNode->IsEmpty())
        {
            if (auto const* attr = xmlNode->GetAttribute("CabinsAndBasketsAffixesCount"))
            {
                int const value = atoi(attr);
                if (value >= 0)
                {
                    m_CabinsAndBasketsAffixesCount = value;
                }
            }
        }

        if (!xmlNode->IsEmpty())
        {
            if (auto const* attr = xmlNode->GetAttribute("NumCollisionLayersBelowVehicle"))
            {
                int const value = atoi(attr);
                if (value >= 0)
                {
                    m_numCollisionLayersBelowVehicle = value;
                }
            }
        }

        ai::Article::LoadArticlesFromNode(m_Articles, xmlFile, xmlNode);
        _LoadFromXmlResourceIdToRandomCoeffMap(xmlFile, xmlNode);
        return true;
    }

    void TownPrototypeInfo::_LoadFromXmlResourceIdToRandomCoeffMap(
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode const* xmlNode)
    {
        m_resourceIdToRandomCoeffMap.clear();

        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (xmlNode->GetFirstChild(node, "ResourceCoeff"); !node->IsEmpty();
             node->GetNextSibling(node, "ResourceCoeff"))
        {
            RandomCoeffWithDispersion newRandomCoeff;
            newRandomCoeff.baseCoeff = 1.0f;
            newRandomCoeff.baseDispersion = 0.0f;
            if (auto const* attr = node->GetAttribute("Coeff"))
            {
                newRandomCoeff.baseCoeff = static_cast<float>(atof(attr));
            }
            if (auto const* attr = node->GetAttribute("Dispersion"))
            {
                newRandomCoeff.baseDispersion = static_cast<float>(atof(attr));
            }

            CStr resourceName;
            if (auto const* attr = node->GetAttribute("Resource"))
            {
                resourceName = attr;
            }

            int const resourceId = ai::theResourceManager->GetResourceId(resourceName);
            if (resourceId == -1)
            {
                M3D_LOG_INFO("Unknown resource name: " + resourceName);
                continue;
            }
            m_resourceIdToRandomCoeffMap[resourceId] = newRandomCoeff;
        }
    }

    void Town::PrepareVehicleForPath(Vehicle*, retruxx::vector<CVector2, retruxx::allocator<CVector2>> const&, bool)
    {
        // TODO: implement (drops the vehicle onto the town path start, needs Vehicle path helpers).
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        ai::SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);

        m3d::SafeBoolAttrib(m_shouldInitializeWorkshops, xmlNode, "ShouldInitializeWorkshops");
        m3d::SafeBoolAttrib(m_QuestsGenerated, xmlNode, "QuestsGenerated");

        if (!xmlNode->IsEmpty())
        {
            if (auto const* attr = xmlNode->GetAttribute("GateState"))
            {
                m_gateState = static_cast<GateState>(atoi(attr));
            }
        }

        float gateTime = m_gateTime.value().get();
        if (!xmlNode->IsEmpty())
        {
            if (auto const* attr = xmlNode->GetAttribute("GateTime"))
            {
                gateTime = static_cast<float>(atof(attr));
            }
        }
        m_gateTime.value().set(gateTime);

        m_resourceIdToCoeff.clear();
        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        for (xmlNode->GetFirstChild(node, "ResourceCoeff"); !node->IsEmpty();
             node->GetNextSibling(node, "ResourceCoeff"))
        {
            CStr resourceName;
            if (auto const* attr = node->GetAttribute("Resource"))
            {
                resourceName = attr;
            }

            int const resourceId = ai::theResourceManager->GetResourceId(resourceName);
            if (resourceId == -1)
            {
                M3D_LOG_INFO("Unknown resource name " + resourceName);
                continue;
            }

            float coeff = 1.0f;
            if (auto const* attr = node->GetAttribute("Coeff"))
            {
                coeff = static_cast<float>(atof(attr));
            }
            m_resourceIdToCoeff[resourceId] = coeff;
        }
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

    void Town::SetRuined(bool bRuined)
    {
        m_bRuined = bRuined;
        M3D_APP->EnqueueMessage(66565, GetId(), bRuined ? 1 : 0, 0, 0, {}, {});
    }

    Town::Town(TownPrototypeInfo const& prototype) : Settlement(prototype), m_gateTime(0.0, 0.0)
    {
        m_gateState = GATE_CLOSED;
        m_PointOfViewInInterface.x = 5.0f;
        m_PointOfViewInInterface.y = 20.0f;
        m_PointOfViewInInterface.z = 5.0f;

        m_targetClasses.insert(RT_CLASS_LOCAL(Vehicle));
        m_gateNode = 0;
        m_shouldInitializeWorkshops = true;
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
        // TODO: implement (runs the exit cinematic path + restores the camera mode).
        RETRUXX_NOT_IMPLEMENTED;
    }

    Team* Town::SpawnCaravanToLocation(char const* locationName)
    {
        auto* obj = ai::theObjects->GetEntityByObjName(locationName);
        if (obj && obj->IsKindOf(&ai::Location::m_classLocation))
        {
            return SpawnCaravanToLocation(static_cast<ai::Location*>(obj));
        }

        M3D_LOG_ERR(CStr("Error: SpawnCaravanToLocation: location '") + CStr(locationName) + "' doesn't exist.");
        return nullptr;
    }

    Team* Town::SpawnCaravanToLocation(Location* location)
    {
        if (!location)
        {
            M3D_LOG_ERR(CStr("Error: NULL location passed to SpawnCaravanToLocation for ") + GetDebugDescription());
            return nullptr;
        }

        CStr const teamName = m_name + "_Caravan" + CStr(static_cast<int>(m_caravans.size()));
        int const prototypeId = ai::thePrototypeManager->GetPrototypeId("caravanTeam");
        int const newObjId = ai::theObjects->CreateNewObject(prototypeId, teamName.c_str(), -1, -1);

        auto* team = RT_DYNCAST(ai::theObjects->GetEntityByObjId(newObjId), ai::Team);
        if (!team)
        {
            return nullptr;
        }

        AddChild(team);
        if (auto* vehicle = _SpawnVehicle())
        {
            team->AddChild(vehicle);
        }
        team->SetDestination(location->GetPosition());
        return team;
    }

    CStr Town::GetPropertyName(int id) const
    {
        for (auto const& [name, propId] : Town::m_propertiesMap)
        {
            if (propId == id)
            {
                return name;
            }
        }
        return ai::SimplePhysicObj::GetPropertyName(id);
    }

    Workshop* Town::GetShop()
    {
        for (auto* building : m_buildings)
        {
            if (BuildingTypeOf(building) == SHOP)
            {
                return static_cast<ai::Workshop*>(building);
            }
        }
        return nullptr;
    }

    TownPrototypeInfo const* Town::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), TownPrototypeInfo const);
    }

    float Town::GetObjResourceCoeff(Obj const* obj) const
    {
        if (!obj)
        {
            return 1.0f;
        }
        int const resourceId = obj->GetPrototypeInfo()->m_resourceId;
        for (auto const& [rid, coeff] : m_resourceIdToCoeff)
        {
            if (ai::theResourceManager->bResourceIsKindOf(resourceId, rid))
            {
                return coeff;
            }
        }
        return 1.0f;
    }

    retruxx::vector<Building*> const& Town::GetAllBuildings() const
    {
        return m_buildings;
    }

    bool Town::IsRuined() const
    {
        return m_bRuined;
    }

    m3d::Class* Town::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Settlement);
    }

    void Town::CreateChildren()
    {
        Settlement::CreateChildren();

        CStr const defendingTeamName = m_name + "_DefendTeam";
        int const prototypeId = ai::thePrototypeManager->GetPrototypeId("settlementTeam");
        int const newObjId = ai::theObjects->CreateNewObject(prototypeId, defendingTeamName.c_str(), -1, -1);
        auto* defendTeam = RT_DYNCAST(ai::theObjects->GetEntityByObjId(newObjId), ai::Team);
        SetTeamByType("DefendTeam", defendTeam);

        CreateChildLocation(ai::Location::LOCATION_DEFEND, "_defend", GetDefendPos());
        CreateChildLocation(ai::Location::LOCATION_DEPLOY, "_deploy", GetDeployPos());
        CreateChildLocation(ai::Location::LOCATION_ENTER, "_enter", GetEnterPos());
        CreateChildLocation(ai::Location::LOCATION_CARAVAN_ARRIVE, "_caravan", GetCaravanArrivePos());
    }

    retruxx::vector<Building*> Town::GetBuildingByType(BuildingType buildingType) const
    {
        retruxx::vector<Building*> result;
        for (auto* building : m_buildings)
        {
            if (BuildingTypeOf(building) == buildingType)
            {
                result.push_back(building);
            }
        }
        return result;
    }

    retruxx::vector<Building*> Town::GetBuildingByType(int buildingType) const
    {
        return GetBuildingByType(static_cast<ai::BuildingType>(buildingType));
    }

    eGObjPropertySaveStatus Town::GetPropertySaveStatus(int id) const
    {
        auto it = Town::m_propertiesSaveStatesMap.find(id);
        if (it != Town::m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return ai::SimplePhysicObj::GetPropertySaveStatus(id);
    }

    void Town::GetBuildingTypes(retruxx::set<BuildingType>& buildingTypeSet) const
    {
        for (auto* building : m_buildings)
        {
            buildingTypeSet.insert(BuildingTypeOf(building));
        }
    }

    void Town::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Settlement::LoadFromXML(xmlFile, xmlNode);

        ref_ptr entryNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(entryNode, "EntryPath");
        m_entryPath.LoadFromXml(GetDebugDescription(), xmlFile, entryNode);

        ref_ptr exitNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(exitNode, "ExitPath");
        m_exitPath.LoadFromXml(GetDebugDescription(), xmlFile, exitNode);

        if (m_entryPath.GetVehiclePoints().size() < 2)
        {
            m_entryPath.clear();
        }
        if (m_exitPath.GetVehiclePoints().size() < 2)
        {
            m_exitPath.clear();
        }

        m3d::SafeVectorAttrib(m_PointOfViewInInterface, xmlNode, "PointOfViewInInterface");
        m3d::SafeStrAttrib(m_caravanLocationName, xmlNode, "CaravansDest");
        m3d::SafeBoolAttrib(m_bRuined, xmlNode, "IsRuined");
    }

    void Town::SetPath(TownPath, retruxx::vector<CVector2> const&, retruxx::vector<CVector> const&)
    {
        // TODO: implement (needs the full ai::Town::TownPath enum, only forward-declared here).
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop* Town::GetWorkshopByObject(Obj const* obj)
    {
        if (obj->IsKindOf(&ai::Ware::m_classWare) || obj->IsKindOf(&ai::Gadget::m_classGadget) ||
            obj->IsKindOf(&ai::Gun::m_classGun) || obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
        {
            return GetShop();
        }
        if (obj->IsKindOf(&ai::Cabin::m_classCabin) || obj->IsKindOf(&ai::Basket::m_classBasket) ||
            obj->IsKindOf(&ai::Vehicle::m_classVehicle) || obj->IsKindOf(&ai::Chassis::m_classChassis))
        {
            return GetWorkshop();
        }
        return nullptr;
    }

    void Town::SendVehicleIn(Vehicle*)
    {
        // TODO: implement (runs the entry cinematic path or the interface fly-in camera).
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
        // TODO: implement (needs the full ai::Town::TownPath enum, only forward-declared here).
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        ai::SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);

        xmlNode->SetAttribute("ShouldInitializeWorkshops", CStr(m_shouldInitializeWorkshops).c_str());
        xmlNode->SetAttribute("QuestsGenerated", CStr(m_QuestsGenerated).c_str());
        xmlNode->SetAttribute("GateState", CStr(static_cast<int>(m_gateState)).c_str());
        xmlNode->SetAttribute("GateTime", CStr(m_gateTime.value().get()).c_str());

        for (auto const& [resourceId, coeff] : m_resourceIdToCoeff)
        {
            ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "ResourceCoeff");
            xmlNode->AddChild(node);
            node->SetAttribute("Resource", ai::theResourceManager->GetResourceName(resourceId).c_str());
            node->SetAttribute("Coeff", CStr(coeff).c_str());
        }
    }

    void Town::SetOpenGateToPlayer(bool bOpen)
    {
        m_bOpenGateToPlayer = bOpen;
    }

    void Town::GetPropertiesIDs(retruxx::set<int>& Props) const
    {
        ai::SimplePhysicObj::GetPropertiesIDs(Props);
        for (auto const& [name, id] : Town::m_propertiesMap)
        {
            Props.insert(id);
        }
    }

    void Town::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        Settlement::SaveToXML(xmlFile, xmlNode);

        ref_ptr entryNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "EntryPath");
        xmlNode->AddChild(entryNode);
        m_entryPath.SaveToXml(xmlFile, entryNode);

        ref_ptr exitNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "ExitPath");
        xmlNode->AddChild(exitNode);
        m_exitPath.SaveToXml(xmlFile, exitNode);

        xmlNode->SetAttribute("PointOfViewInInterface", CStr(m_PointOfViewInInterface).c_str());
        xmlNode->SetAttribute("CaravansDest", m_caravanLocationName.c_str());
        if (m_bRuined)
        {
            xmlNode->SetAttribute("IsRuined", CStr(m_bRuined).c_str());
        }
    }

    void Town::RelinkSceneGraphNode()
    {
        bool const needToRelink = m_physicBody->bNeedToRelinkNode();
        ai::SimplePhysicObj::RelinkSceneGraphNode();
        if (needToRelink && m_gateNode)
        {
            auto& landscape = ai::pServer->GetWorld()->GetLandscape();
            landscape.LinkNodeAndChildrenCollisionGeomsToCell(m_gateNode);
            landscape.SetNodeCollisionGeomsEnabled(m_gateNode, m_gateState != GATE_OPEN);
        }
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
        // TODO: implement (walks ai::theObjects for DynamicQuest children of this town,
        // filtering out an already-taken hunt quest; needs DynamicQuest internals).
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop* Town::GetWorkshop()
    {
        for (auto* building : m_buildings)
        {
            if (BuildingTypeOf(building) == WORKSHOP)
            {
                return static_cast<ai::Workshop*>(building);
            }
        }
        return nullptr;
    }

    bool Town::GetOpenGateToPlayer() const
    {
        return m_bOpenGateToPlayer;
    }

    void Town::GetPropertiesNames(retruxx::set<CStr>& Props) const
    {
        ai::SimplePhysicObj::GetPropertiesNames(Props);
        for (auto const& [name, id] : Town::m_propertiesMap)
        {
            Props.insert(name);
        }
    }

    void Town::Update(float elapsedTime, unsigned workTime)
    {
        SimplePhysicObj::Update(elapsedTime, workTime);

        if (elapsedTime != 0.0f)
        {
            _GenerateDynamicQuest();
            _InitializeWorkshops();
        }

        m_timeFromLastEnterTown += elapsedTime;

        if (m_gateNode && m_gateTime.value().get() > m_gateTime.minValue().get())
        {
            m_gateTime.value().sub(elapsedTime);
            if (m_gateTime.value().get() == m_gateTime.minValue().get())
            {
                if (m_gateState == GATE_CLOSING)
                {
                    m_gateState = GATE_CLOSED;
                }
                else if (m_gateState == GATE_OPENING)
                {
                    m_gateState = GATE_OPEN;
                    ai::pServer->GetWorld()->GetLandscape().SetNodeCollisionGeomsEnabled(m_gateNode, false);
                }
            }
        }
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
        // TODO: implement (scans ai::theObjects for a live DynamicQuest owned by this town and
        // sets m_QuestsGenerated accordingly; needs DynamicQuest parent/state internals).
        RETRUXX_NOT_IMPLEMENTED;
    }

    Workshop* Town::GetWorkshopByPrototypeId(int pId)
    {
        auto const* proto = ai::thePrototypeManager->GetPrototypeInfo(pId);
        if (!proto)
        {
            return nullptr;
        }
        if (proto->IsPrototypeOf(&ai::Ware::m_classWare) || proto->IsPrototypeOf(&ai::Gadget::m_classGadget) ||
            proto->IsPrototypeOf(&ai::Gun::m_classGun) || proto->IsPrototypeOf(&ai::CompoundGun::m_classCompoundGun))
        {
            return GetShop();
        }
        if (proto->IsPrototypeOf(&ai::Cabin::m_classCabin) || proto->IsPrototypeOf(&ai::Basket::m_classBasket) ||
            proto->IsPrototypeOf(&ai::Vehicle::m_classVehicle) || proto->IsPrototypeOf(&ai::Chassis::m_classChassis))
        {
            return GetWorkshop();
        }
        return nullptr;
    }

    void Town::_InternalCreateVisualPart()
    {
        SimplePhysicObj::_InternalCreateVisualPart();
        // TODO: create the gate scene-graph node from the prototype's gate model and hook up its
        // "LP_GATE" load point (needs PhysicBody::CreateNode + AnimatedModelsServer bone matrices).
    }

    Town::~Town() = default;

    bool Town::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        if (propertyId == 64)
        {
            retVal = m_maxDefenders;
            return true;
        }
        if (propertyId == 65)
        {
            retVal = m_bOpenGateToPlayer ? 1 : 0;
            return true;
        }
        return ai::SimplePhysicObj::_GetPropertyInternal(propertyId, retVal);
    }

    void Town::_InternalPostLoad()
    {
        Settlement::_InternalPostLoad();
        ai::theProcessManager->PostMessageA(2, ai::thePlayer->GetId(), GetId(), 0.0f, {60}, {}, 1);
        ai::theProcessManager->PostMessageA(2, ai::thePlayer->GetId(), GetId(), 0.0f, {61}, {}, 1);
        ai::theProcessManager->PostMessageA(2, ai::thePlayer->GetId(), GetId(), 0.0f, {64}, {}, 1);
    }

    void Town::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool Town::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        if (propertyId == 64)
        {
            retVal = GetPrototypeInfo() ? GetPrototypeInfo()->m_maxDefenders : 0;
            return true;
        }
        if (propertyId == 65)
        {
            retVal = 1;
            return true;
        }
        return ai::SimplePhysicObj::_GetPropertyDefaultInternal(propertyId, retVal);
    }

    void Town::_OnPlayerVehicleHorn(Event const&)
    {
        auto* location = GetLocation(ai::Location::LOCATION_ENTER);
        if (!location)
        {
            return;
        }

        auto* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
        if (!vehicle)
        {
            return;
        }

        if ((vehicle->GetFlags() & 8) == 0 && (vehicle->GetFlags() & 2) == 0 && !vehicle->GetParentRepository() &&
            location->IsObjectInside(vehicle->GetId()) && vehicle->bIsControlledByPlayer() && m_bOpenGateToPlayer &&
            ai::theRelationship->CheckTolerance(vehicle->GetBelong(), GetBelong()) > RS_ENEMY)
        {
            SendVehicleIn(vehicle);
        }
    }

    m3d::Object* Town::Clone()
    {
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    Team* Town::_GetDefendingTeam()
    {
        return ai::Settlement::GetTeamByType("DefendTeam");
    }

    m3d::Object* Town::CreateObject()
    {
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void Town::_OnObjectEntersLocation(Event const& evn)
    {
        if (evn.m_param1.GetType() != m3d::AIPARAM_ID || evn.m_param2.GetType() != m3d::AIPARAM_ID)
        {
            return;
        }

        int const locationType = evn.m_param2.GetAsID();
        auto* vehicle = RT_DYNCAST(ai::theObjects->GetEntityByObjId(evn.m_param1.GetAsID()), ai::Vehicle);
        if (!vehicle || vehicle->GetClass() != &ai::Vehicle::m_classVehicle)
        {
            return;
        }

        if (locationType == 1)
        {
            CauseEvent(GE_OBJECT_ENTERS_LOCATION, 0.0f, {vehicle->GetId()}, {});

            if (vehicle->bIsControlledByPlayer() && !m_bRuined)
            {
                if (ai::theRelationship->CheckTolerance(GetBelong(), vehicle->GetBelong()) > RS_ENEMY)
                {
                    if (m_bOpenGateToPlayer)
                    {
                        M3D_APP->EnqueueMessage(66563, 6, 0, 0, 0, {}, {});
                        M3D_APP->EnqueueMessage(66568, GetId(), 1, 0, 0, {}, {});
                    }
                    else
                    {
                        M3D_APP->EnqueueMessage(66563, 5, 0, 0, 0, {}, {});
                    }
                }
                else
                {
                    M3D_APP->EnqueueMessage(66563, 4, 0, 0, 0, {}, {});
                }
            }
        }
        else if (
            locationType == 2 && ai::theRelationship->CheckTolerance(vehicle->GetBelong(), GetBelong()) <= RS_ENEMY)
        {
            auto* defendTeam = _GetDefendingTeam();
            if (defendTeam && static_cast<int>(defendTeam->GetNumVehicles()) < m_maxDefenders)
            {
                if (auto* spawned = ai::Settlement::_SpawnVehicle())
                {
                    _GetDefendingTeam()->AddChild(spawned);
                }
            }
        }
    }

    void Town::_InitializeWorkshops()
    {
        if (!m_shouldInitializeWorkshops)
        {
            return;
        }
        m_shouldInitializeWorkshops = 0;
        // TODO: populate the town's shop/workshop repositories from the prototype's articles and
        // affix generators (needs Workshop::AddArticle / GenerateGunRepository /
        // ApplyAffixGeneratorToRepository and the resource-id-to-coeff map builder).
    }

    void Town::_OnTargetReached(Event const&)
    {
        // TODO: implement (handles caravan arrival + the player's town-path index reaching its end;
        // needs ProcessManager messaging and the cinematic-mode change impulse).
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
        if (!m_gateNode)
        {
            return;
        }
        if (m_gateState == GATE_OPENING || m_gateState == GATE_OPEN)
        {
            return;
        }

        auto const* proto = GetPrototypeInfo();
        m_gateState = GATE_OPENING;
        m_gateTime.value().set(proto ? proto->m_gateOpeningTime : 0.0f);
        ai::pServer->GetWorld()->GetLandscape().SetNodeCollisionGeomsEnabled(m_gateNode, true);
        _SynchronizeGatesState();
    }

    void Town::_StartCinematic(Vehicle*, retruxx::vector<CVector, retruxx::allocator<CVector>> const&)
    {
        // TODO: implement (kicks off the town entry/exit cinematic; needs the cinematic subsystem).
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_OnSkipCinematic(Event const&)
    {
        // TODO: implement (aborts the running town cinematic, restores camera, opens the gate);
        // needs ProcessManager messaging + camera/cinematic state.
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Town::_CloseGates()
    {
        if (!m_gateNode)
        {
            return;
        }
        if (m_gateState == GATE_CLOSED || m_gateState == GATE_CLOSING)
        {
            return;
        }

        auto const* proto = GetPrototypeInfo();
        m_gateState = GATE_CLOSING;
        m_gateTime.value().set(proto ? proto->m_gateClosingTime : 0.0f);
        ai::pServer->GetWorld()->GetLandscape().SetNodeCollisionGeomsEnabled(m_gateNode, true);
        _SynchronizeGatesState();
    }

    void Town::_OnObjectLeavesLocation(Event const& evn)
    {
        if (evn.m_param1.GetType() != m3d::AIPARAM_ID || evn.m_param2.GetType() != m3d::AIPARAM_ID)
        {
            return;
        }

        int const locationType = evn.m_param2.GetAsID();
        auto* vehicle = RT_DYNCAST(ai::theObjects->GetEntityByObjId(evn.m_param1.GetAsID()), ai::Vehicle);
        if (!vehicle || vehicle->GetClass() != &ai::Vehicle::m_classVehicle)
        {
            return;
        }

        if (locationType == 1 && vehicle->bIsControlledByPlayer())
        {
            M3D_APP->EnqueueMessage(66568, GetId(), 0, 0, 0, {}, {});
        }
    }

    void Town::_SynchronizeGatesState()
    {
        // TODO: implement (drives the gate SgNode's open/close animation to match m_gateState);
        // needs m3d::SgNode::SetProperty + ai::SetNodeElapsedAnimationTimeInMs.
        RETRUXX_NOT_IMPLEMENTED;
    }

    DynamicQuest* Town::_GenerateDynamicQuest()
    {
        if (m_QuestsGenerated)
        {
            return nullptr;
        }
        m_QuestsGenerated = 1;
        // TODO: implement the random dynamic-quest generation (destroy / reach / convoy / hunt);
        // needs DynamicQuestManager + the DynamicQuest* subclasses + ai::theGlobProp.
        return nullptr;
    }
}  // namespace ai

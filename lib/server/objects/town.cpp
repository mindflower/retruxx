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
#include "dynamicquestdestroy.h"
#include "dynamicquesthunt.h"
#include "base/globalproperties.h"
#include "physicbodies/physichelpers.h"
#include "player.h"
#include "workshop.h"

#include <cinematic.h>
#include <game/m3dgame.h>
#include <impulses/i_impulses.h>
#include <math/random.h>
#include <scene/servers/serveranimatedmodel.h>
#include <server/dynamicquestmanager.h>
#include <server/processmanager.h>
#include <server/resourcemanager.h>

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SpawnCaravanToLocation)
{
    // RVA 0x6F1570
    auto* town = (ai::Town*)context->asObject(0, "Town");
    context->pushObject(town->SpawnCaravanToLocation(context->asString(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, GetOpenGateToPlayer)
{
    // RVA 0x6EB110
    auto* town = (ai::Town*)context->asObject(0, "Town");
    context->pushBool(town->GetOpenGateToPlayer());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetOpenGateToPlayer)
{
    // RVA 0x6EB140 - sets the flag directly, so it does not go through Town::SetOpenGateToPlayer.
    auto* town = (ai::Town*)context->asObject(0, "Town");
    town->SetOpenGateToPlayer(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, SetRuined)
{
    // RVA 0x6F39B0
    auto* town = (ai::Town*)context->asObject(0, "Town");
    town->SetRuined(context->asBool(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Town, IsRuined)
{
    // RVA 0x6EB170
    auto* town = (ai::Town*)context->asObject(0, "Town");
    context->pushBool(town->IsRuined());
    return 1;
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
        // RVA 0x5CCAF0 - note this is rand() scaled by RAND_MAX+1, so the top bound is exclusive.
        int IntRandom(int highBound)
        {
            return highBound * rand() / 0x8000;
        }

        // RVA 0x6EFE20 - rolls each resource's price coefficient once, within its dispersion (given in percent).
        void CreateResourceIdToCoeffMap(
            retruxx::map<int, RandomCoeffWithDispersion> const& rndResourceIdToCoeff,
            retruxx::map<int, float>& resourceIdToCoeff)
        {
            resourceIdToCoeff.clear();
            for (auto const& [resourceId, randomCoeff] : rndResourceIdToCoeff)
            {
                float const dispersion = randomCoeff.baseDispersion * 0.0099999998f;
                float const lo = 1.0f - dispersion;
                float const hi = 1.0f + dispersion;
                resourceIdToCoeff[resourceId] = (rand() * 0.000030518509f * (hi - lo) + lo) * randomCoeff.baseCoeff;
            }
        }
    }  // namespace

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
        // Not emitted in the shipped binary: this override adds nothing to the base version.
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

    void Town::PrepareVehicleForPath(
        Vehicle* pVehicle,
        retruxx::vector<CVector2, retruxx::allocator<CVector2>> const& points,
        bool delayed)
    {
        // RVA 0x6EF200 - puts the vehicle on the first point of the path, facing its last point. A delayed move is
        // carried out once the cinematic is running.
        CVector const newPos(points.front().x, 0.0f, points.front().y);
        CVector dir(points.back().x - newPos.x, 0.0f, points.back().y - newPos.z);
        float const invLen = 1.0f / sqrt(dir.z * dir.z + dir.x * dir.x + 0.00000011920929f);
        CVector const newDir(dir.x * invLen, dir.y * invLen, dir.z * invLen);

        if (delayed)
        {
            m_NewPosForVehicle = newPos;
            m_NewDirForVehicle = newDir;
            m_VehicleShouldBeMoved = true;
            m_VehicleToBeMoved = pVehicle;
            return;
        }
        if (sqrt(newDir.x * newDir.x + newDir.y * newDir.y + newDir.z * newDir.z) > 0.1)
        {
            pVehicle->SetDirection(newDir);
        }
        pVehicle->SetGamePositionOnGround(newPos, true, false);
        pVehicle->SetLinearVelocity(ZeroVector);
        pVehicle->SetAngularVelocity(ZeroVector);
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

    void Town::SendVehicleOff(Vehicle* pVehicle, bool bQuick)
    {
        // RVA 0x6F3A90 - drives the player's vehicle out of town along the exit path, with the exit cinematic unless
        // the caller wants it over with at once.
        if (pVehicle)
        {
            auto const& vehiclePoints = m_exitPath.GetVehiclePoints();
            if (!vehiclePoints.empty())
            {
                PrepareVehicleForPath(pVehicle, vehiclePoints, false);
                m_PlayerPathIndex = pVehicle->SetExternalPath(vehiclePoints);
                theProcessManager->PostMessageA(2, pVehicle->GetId(), GetId(), 0.0f, m3d::AIParam(51), m3d::AIParam(), 1);
                if (bQuick)
                {
                    pVehicle->PlaceToEndOfPath();
                    M3D_APP->m_player.m_cameraMode = m_OldCameraMode;
                    m_playerEnteringTownCount = 0;
                    return;
                }

                retruxx::vector<CVector> cameraPoints = m_exitPath.GetCameraPoints();
                for (CVector& point : cameraPoints)
                {
                    CVector const pos = GetPosition();
                    point.x += pos.x;
                    point.y += pos.y;
                    point.z += pos.z;
                }
                _StartCinematic(pVehicle, cameraPoints);
                M3D_APP->HandleCinematic(M3D_APP->m_cinematic->m_fadePeriod.GetF());
            }
            else
            {
                M3D_APP->m_player.m_cameraMode = m_OldCameraMode;
            }
        }
        m_playerEnteringTownCount = 0;
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

    void Town::SetPath(
        TownPath path,
        retruxx::vector<CVector2> const& vehiclePoints,
        retruxx::vector<CVector> const& cameraPoints)
    {
        // RVA 0x6EB950 - the camera points are stored relative to the town.
        CinematicPath* cinematicPath = nullptr;
        switch (path)
        {
        case TP_ENTRY_PATH:
            cinematicPath = &m_entryPath;
            break;
        case TP_EXIT_PATH:
            cinematicPath = &m_exitPath;
            break;
        default:
            SYS_ERROR("0");
            return;
        }
        cinematicPath->SetCameraPoints(cameraPoints);
        cinematicPath->SetVehiclePoints(vehiclePoints);
        CVector const pos = GetPosition();
        cinematicPath->ShiftCameraPoints(CVector(pos.x * -1.0f, pos.y * -1.0f, pos.z * -1.0f));
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

    void Town::SendVehicleIn(Vehicle* pVehicle)
    {
        // RVA 0x6F3C90 - opens the gate and either drives the vehicle in along the entry path with the cinematic, or,
        // for a town without one, just opens the town interface and parks the camera at its point of view.
        if (m_playerEnteringTownCount > 0)
        {
            return;
        }
        m_timeFromLastEnterTown = 0.0f;
        _OpenGates();
        m_OldCameraMode = M3D_APP->m_player.m_cameraMode;
        pVehicle->SetHorn(false);

        auto const& vehiclePoints = m_entryPath.GetVehiclePoints();
        if (!vehiclePoints.empty())
        {
            PrepareVehicleForPath(pVehicle, m_entryPath.GetVehiclePoints(), true);
            pVehicle->setGodMode(true);
            m_PlayerPathIndex = pVehicle->SetExternalPath(m_entryPath.GetVehiclePoints());
            theProcessManager->PostMessageA(2, pVehicle->GetId(), GetId(), 0.0f, m3d::AIParam(51), m3d::AIParam(), 1);
            m_playerEnteringTownCount = 2;

            retruxx::vector<CVector> cameraPoints = m_entryPath.GetCameraPoints();
            for (CVector& point : cameraPoints)
            {
                CVector const pos = GetPosition();
                point.x += pos.x;
                point.y += pos.y;
                point.z += pos.z;
            }
            _StartCinematic(pVehicle, cameraPoints);
            return;
        }

        m_playerEnteringTownCount = 1;
        M3D_APP->EnqueueMessage(66540, GetId(), 0, 0, 0, CStr(), m3d::AIParam());
        CVector const pos = GetPosition();
        M3D_APP->m_curCamera.m_worldOrigin =
            CVector(m_PointOfViewInInterface.x + pos.x, m_PointOfViewInInterface.y + pos.y, m_PointOfViewInInterface.z + pos.z);
        M3D_APP->m_curCamera.lookAt(pVehicle->GetPosition());
        M3D_APP->m_player.m_cameraMode = CM_CONST;
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

    void Town::GetPath(TownPath path, retruxx::vector<CVector2>& vehiclePoints, retruxx::vector<CVector>& cameraPoints) const
    {
        // RVA 0x6F2110 - the stored camera points are relative to the town, so they come back shifted into the world.
        CinematicPath const* cinematicPath = nullptr;
        switch (path)
        {
        case TP_ENTRY_PATH:
            cinematicPath = &m_entryPath;
            break;
        case TP_EXIT_PATH:
            cinematicPath = &m_exitPath;
            break;
        default:
            SYS_ERROR("0");
            return;
        }
        cameraPoints = cinematicPath->GetCameraPoints();
        vehiclePoints = cinematicPath->GetVehiclePoints();
        for (CVector& point : cameraPoints)
        {
            CVector const pos = GetPosition();
            point.x += pos.x;
            point.y += pos.y;
            point.z += pos.z;
        }
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
        // RVA 0x6F0760 - every quest this town gave out, less a hunt quest the player already took.
        retruxx::vector<int> res;
        for (Obj* obj : *theObjects)
        {
            if (!obj->IsKindOf(&DynamicQuest::m_classDynamicQuest))
            {
                continue;
            }
            if (obj->IsKindOf(&DynamicQuestHunt::m_classDynamicQuestHunt) && thePlayer->HuntQuestIsTaken())
            {
                continue;
            }
            if (static_cast<DynamicQuest*>(obj)->GetHirerObjId() == GetId())
            {
                res.push_back(obj->GetId());
            }
        }
        return res;
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
        // RVA 0x6F1310 - despite the name this only notes whether the town still has a quest of its own running, so
        // that _GenerateDynamicQuest may make a new one.
        m_QuestsGenerated = false;
        for (Obj* obj : *theObjects)
        {
            if (!obj->IsKindOf(&DynamicQuest::m_classDynamicQuest))
            {
                continue;
            }
            auto* const quest = static_cast<DynamicQuest*>(obj);
            if (quest->GetHirerObjId() == GetId() && quest->GetQuestStatus() < DynamicQuest::STATUS_COMPLETE)
            {
                m_QuestsGenerated = true;
                return;
            }
        }
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
        // RVA 0x6ECA40 - hangs the gate model on the town's own model at its LP_GATE load point.
        SimplePhysicObj::_InternalCreateVisualPart();
        auto const* const proto = GetPrototypeInfo();
        if (GetPassedToAnotherMapStatus() || proto->m_gateModelName.empty())
        {
            return;
        }

        m_gateNode = PhysicBody::CreateNode(proto->m_gateModelName, 0, CVector(1.0f, 1.0f, 1.0f), nullptr, false);
        m_physicBody->m_Node->AddChild(m_gateNode);

        CMatrix mat;
        auto& server = static_cast<m3d::AnimatedModelsServer&>(M3D_APP->GetAnimatedModelsServer());
        if (!server.GetBoneMatrixByNameFromModelName(proto->GetEngineModelName().c_str(), CStr("LP_GATE"), mat, false))
        {
            M3D_LOG_ERR(
                CStr("Error: LoadPoint not found! Model = '") + proto->GetEngineModelName() + CStr("', lp = ") +
                CStr("LP_GATE") + CStr(" for ") + GetDebugDescription());
            mat.identity();
        }
        Quaternion rot;
        rot.FromMatrix(mat);
        m_gateNode->SetRotation(rot);
        m_gateNode->SetOriginAbs(CVector(mat._41, mat._42, mat._43));
        m_gateNode->SetName(CStr(m_physicBody->m_Node->GetName()) + CStr("Gate"));
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
        // RVA 0x6F1E00 - stocks the town's shops and workshops: the prototype's own articles, every "GOODS"
        // prototype, then the generated guns and the affixes applied to them.
        m_shouldInitializeWorkshops = 0;
        auto const* const proto = GetPrototypeInfo();
        for (Article const& article : proto->m_Articles)
        {
            if (Workshop* const workshop = GetWorkshopByPrototypeId(article.getPrototypeId()))
            {
                workshop->AddArticle(article);
            }
        }

        retruxx::vector<int> waresPrototypes;
        thePrototypeManager->GetPrototypeIdsByResourceId(theResourceManager->GetResourceId(CStr("GOODS")), waresPrototypes);
        for (int const prototypeId : waresPrototypes)
        {
            if (Workshop* const workshop = GetWorkshopByPrototypeId(prototypeId))
            {
                workshop->AddArticle(prototypeId);
            }
        }

        retruxx::vector<Building*> bb = GetBuildingByType(SHOP);
        retruxx::vector<Building*> const workshops = GetBuildingByType(WORKSHOP);
        bb.insert(bb.end(), workshops.begin(), workshops.end());
        CreateResourceIdToCoeffMap(proto->m_resourceIdToRandomCoeffMap, m_resourceIdToCoeff);

        for (Building* const building : bb)
        {
            auto* const workshop = static_cast<Workshop*>(building);
            if (proto->m_GunGeneratorPrototypeId != -1)
            {
                workshop->GenerateGunRepository(proto->m_GunGeneratorPrototypeId, proto->m_DesiredGunsInWorkshop);
            }
            if (proto->m_GunAffixGeneratorPrototypeId != -1)
            {
                workshop->ApplyAffixGeneratorToRepository(
                    WORKSHOP_GUNS_AND_GADGETS,
                    proto->m_GunAffixGeneratorPrototypeId,
                    proto->m_GunAffixesCount,
                    theResourceManager->GetResourceId(CStr("GUN")));
            }
            if (proto->m_CabinsAndBasketsAffixGeneratorPrototypeId != -1)
            {
                workshop->ApplyAffixGeneratorToRepository(
                    WORKSHOP_CABINS_AND_BASKETS,
                    proto->m_CabinsAndBasketsAffixGeneratorPrototypeId,
                    proto->m_CabinsAndBasketsAffixesCount,
                    theResourceManager->GetResourceId(CStr("VEHICLE_PART")));
            }
        }
    }

    void Town::_OnTargetReached(Event const& evn)
    {
        // RVA 0x6F3170 - a caravan of this town that arrived is sent on its way again when the player is close enough
        // to see it, and removed otherwise; the player's vehicle finishing the entry path ends the drive-in.
        Obj* const obj = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (!obj)
        {
            return;
        }
        if (obj->IsKindOf(&Team::m_classTeam))
        {
            auto* const team = static_cast<Team*>(obj);
            if (std::find(m_caravans.begin(), m_caravans.end(), team) == m_caravans.end())
            {
                return;
            }
            Vehicle* const playerVehicle = thePlayer ? thePlayer->GetVehicle() : nullptr;
            if (playerVehicle && team->GetDistToPhysicObj(playerVehicle) <= theGlobProp.m_distanceFromPlayerToMoveout)
            {
                theProcessManager->PostMessageA(51, GetId(), evn.m_senderObjId, 5.0f, m3d::AIParam(), m3d::AIParam(), 1);
            }
            else
            {
                team->Remove();
            }
            return;
        }
        if (obj->GetClass() == &Vehicle::m_classVehicle && static_cast<Vehicle*>(obj)->bIsControlledByPlayer() &&
            m_PlayerPathIndex == evn.m_param1.GetAsID())
        {
            if (m_playerEnteringTownCount <= 0)
            {
                _CloseGates();
            }
            --m_playerEnteringTownCount;
            static_cast<Vehicle*>(obj)->setGodMode(false);
            theProcessManager->PostMessageA(3, obj->GetId(), GetId(), 0.0f, m3d::AIParam(51), m3d::AIParam(), 1);
            M3D_APP->OnChangeMode(m3d::AuxImpulseInfo(3, true, -1, 1, 0));
        }
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

    void Town::_StartCinematic(Vehicle* pVehicle, retruxx::vector<CVector, retruxx::allocator<CVector>> const& points)
    {
        // RVA 0x6F2920 - flies the camera along the given points while looking at the vehicle. With no points the
        // camera simply stays where it is.
        theProcessManager->PostMessageA(2, thePlayer->GetId(), GetId(), 0.0f, m3d::AIParam(56), m3d::AIParam(), 1);
        theProcessManager->PostMessageA(2, thePlayer->GetId(), GetId(), 0.0f, m3d::AIParam(57), m3d::AIParam(), 1);

        m3d::Cinematic* const cinematic = M3D_APP->m_cinematic;
        cinematic->LoadDefaults();
        cinematic->SetFlags(3);
        M3D_APP->OnChangeMode(m3d::AuxImpulseInfo(2, true, -1, 0, 0));

        retruxx::vector<m3d::CameraPathState> cameraStates;
        cameraStates.resize(points.size());
        for (unsigned i = 0; i < cameraStates.size(); ++i)
        {
            cameraStates[i].m_point = points[i];
            cameraStates[i].m_rotation = IdentityQuaternion;
            cameraStates[i].m_zoom = 1.0f;
        }
        if (cameraStates.empty())
        {
            cameraStates.resize(1, m3d::CameraPathState(ZeroVector, IdentityQuaternion, 1.0f, 1.0f, 0.0f));
            cameraStates.front().m_point = M3D_APP->m_curCamera.m_worldOrigin;
            cameraStates.front().m_rotation = IdentityQuaternion;
            cameraStates.front().m_zoom = 1.0f;
        }
        cinematic->SetCameraStates(cameraStates);
        cinematic->SetLookTo(true);
        cinematic->SetAimToID(pVehicle->GetId());
        cinematic->SetWaitWhenStop(true);
        cinematic->Play(7.0f);
        cinematic->StartCinematic();
    }

    void Town::_OnSkipCinematic(Event const& evn)
    {
        // RVA 0x6F33C0 - the player skipped the drive-in or drive-out: the vehicle jumps to the end of the path, and
        // either the town interface opens or the old camera mode comes back.
        if (!theObjects->GetEntityByObjId(evn.m_senderObjId))
        {
            return;
        }
        m_VehicleShouldBeMoved = false;
        Vehicle* const playerVehicle = thePlayer->GetVehicle();
        if (playerVehicle)
        {
            playerVehicle->PlaceToEndOfPath();
            playerVehicle->setGodMode(false);
        }
        theProcessManager->PostMessageA(3, thePlayer->GetId(), GetId(), 0.0f, m3d::AIParam(56), m3d::AIParam(), 1);
        theProcessManager->PostMessageA(3, thePlayer->GetId(), GetId(), 0.0f, m3d::AIParam(57), m3d::AIParam(), 1);

        if (m_playerEnteringTownCount <= 0)
        {
            M3D_APP->m_player.m_cameraMode = m_OldCameraMode;
            --m_playerEnteringTownCount;
            return;
        }

        M3D_APP->EnqueueMessage(66540, GetId(), 0, 0, 0, CStr(), m3d::AIParam());
        CVector const pos = GetPosition();
        M3D_APP->m_curCamera.m_worldOrigin =
            CVector(m_PointOfViewInInterface.x + pos.x, m_PointOfViewInInterface.y + pos.y, m_PointOfViewInInterface.z + pos.z);
        if (playerVehicle)
        {
            M3D_APP->m_curCamera.lookAt(playerVehicle->GetPosition());
        }
        M3D_APP->m_player.m_cameraMode = CM_CONST;
        if (m_gateNode)
        {
            m_gateState = GATE_OPEN;
            m_gateTime.setToMin();
            _SynchronizeGatesState();
        }
        --m_playerEnteringTownCount;
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
        // RVA 0x6EB5E0 - puts the gate model on its closing or opening animation, wound forward to however much of the
        // gate's time has already run out. Only a fully open gate lets things through.
        auto const* const proto = GetPrototypeInfo();
        float elapsed = 0.0f;
        int action = 14;
        switch (m_gateState)
        {
        case GATE_CLOSED:
            action = 14;
            elapsed = proto->m_gateClosingTime;
            break;
        case GATE_OPENING:
            action = 15;
            elapsed = proto->m_gateOpeningTime - m_gateTime.value().get();
            break;
        case GATE_OPEN:
            action = 15;
            elapsed = proto->m_gateOpeningTime;
            break;
        case GATE_CLOSING:
            action = 14;
            elapsed = proto->m_gateClosingTime - m_gateTime.value().get();
            break;
        default:
            break;
        }
        if (m_gateState < GATE_NUM_STATES)
        {
            m_gateNode->SetProperty(8704, &action);
            ai::SetNodeElapsedAnimationTimeInMs(m_gateNode, static_cast<int>(elapsed * 1000.0f));
        }
        pServer->GetWorld()->GetLandscape().SetNodeCollisionGeomsEnabled(m_gateNode, m_gateState != GATE_OPEN);
    }

    DynamicQuest* Town::_GenerateDynamicQuest()
    {
        if (m_QuestsGenerated)
        {
            return nullptr;
        }
        // RVA 0x6F0830 - makes up to three attempts at a quest of a random kind.
        m_QuestsGenerated = true;
        if (rand() * 0.000030518509f > theGlobProp.m_probabilityToGenerateDynamicQuestInTown)
        {
            return nullptr;
        }

        DynamicQuest* res = nullptr;
        for (int iTrial = 0; iTrial < 3 && !res; ++iTrial)
        {
            switch (IntRandom(4))
            {
            case 0:
                // Destroy a named enemy vehicle.
                if (DynamicQuestDestroy::FreeNameForTargetExists())
                {
                    retruxx::vector<Obj*> candidates;
                    for (Obj* obj : *theObjects)
                    {
                        if (obj->GetClass() != &Vehicle::m_classVehicle || !obj->IsAlive() || obj->GetParentRepository() ||
                            !obj->IsUpdating())
                        {
                            continue;
                        }
                        if (obj->GetBelong() != 1002 && obj->GetBelong() != 1005)
                        {
                            continue;
                        }
                        if (CStr(obj->GetName()).empty() || theObjects->GetObjectFullName(CStr(obj->GetName())).empty())
                        {
                            continue;
                        }
                        candidates.push_back(obj);
                    }
                    if (!candidates.empty())
                    {
                        Obj* const target = candidates[IntRandom(static_cast<int>(candidates.size()))];
                        theObjects->SetObjName(target->GetId(), DynamicQuestDestroy::GetRandomNameForTarget());
                        res = DynamicQuestManager::CreateQuest(
                            DynamicQuestManager::TYPE_DESTROY, target->GetId(), GetId());
                    }
                }
                break;

            case 1:
            {
                // Reach another town.
                retruxx::vector<Obj*> towns;
                for (Obj* obj : *theObjects)
                {
                    if (obj->GetClass() == &Town::m_classTown && obj->GetId() != GetId() && obj->IsAlive() &&
                        !static_cast<Town*>(obj)->IsRuined() && !CStr(obj->GetName()).empty())
                    {
                        towns.push_back(obj);
                    }
                }
                if (!towns.empty())
                {
                    res = DynamicQuestManager::CreateQuest(
                        DynamicQuestManager::TYPE_REACH, towns[IntRandom(static_cast<int>(towns.size()))]->GetId(), GetId());
                }
                break;
            }

            case 2:
            {
                // Escort a caravan to this town's caravan destination.
                if (m_caravanLocationName.empty() || !GetLocation(Location::LOCATION_DEPLOY))
                {
                    break;
                }
                Obj* const dest = theObjects->GetEntityByObjName(m_caravanLocationName);
                if (!dest || (!dest->IsKindOf(&Location::m_classLocation) && !dest->IsKindOf(&Town::m_classTown)))
                {
                    M3D_LOG_ERR("Error: invalid location name for caravan: '" + m_caravanLocationName + CStr("'"));
                    break;
                }
                if (!dest->IsKindOf(&Town::m_classTown))
                {
                    res = DynamicQuestManager::CreateQuest(DynamicQuestManager::TYPE_CONVOY, dest->GetId(), GetId());
                    break;
                }
                auto* const destTown = static_cast<Town*>(dest);
                if (!destTown->IsAlive() || destTown->IsRuined())
                {
                    break;
                }
                if (!destTown->GetLocation(Location::LOCATION_CARAVAN_ARRIVE))
                {
                    M3D_LOG_ERR(
                        "Error: town for dynamic quest hasn't caravan location: '" + m_caravanLocationName + CStr("'"));
                    break;
                }
                res = DynamicQuestManager::CreateQuest(DynamicQuestManager::TYPE_CONVOY, dest->GetId(), GetId());
                break;
            }

            case 3:
                // Hunt the player.
                if (thePlayer && thePlayer->GetVehicle())
                {
                    res = DynamicQuestManager::CreateQuest(
                        DynamicQuestManager::TYPE_HUNT, thePlayer->GetVehicle()->GetId(), GetId());
                }
                break;

            default:
                SYS_ERROR("0");
                break;
            }
        }
        return res;
    }
}  // namespace ai

#include "infectionzone.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include <core/kernel.h>
#include <core/log.h>
#include <m3dapp.h>
#include <world.h>
#include <scene/scenegraph.h>
#include "core/ini.h"
#include "core/aiparam.h"
#include "base/globalproperties.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "infectionlair.h"
#include "infectionteam.h"
#include "player.h"
#include "vehicle.h"
#include "../obstacle.h"
#include "../relationship.h"
#include "../server.h"
#include "../utils.h"

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, ResetTimeOut)
{
    // RVA 0x83B440
    auto zone = (ai::InfectionZone*)context->asObject(0, "InfectionZone");
    zone->ResetTimeOut();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, IsPlayerInside)
{
    // RVA 0x83FDD0
    auto zone = (ai::InfectionZone*)context->asObject(0, "InfectionZone");
    context->pushBool(zone->IsPlayerInside());
    return 1;
}

namespace ai
{
    namespace
    {
        float const VISCELL_EDGE_LENGTH_INV = 0.0078125f;

        // Orders drop out points from the nearest to the furthest from m_origin.
        struct CVectorDistComparer
        {
            CVector m_origin;

            explicit CVectorDistComparer(CVector const& origin) : m_origin(origin)
            {
            }

            bool operator()(CVector const& a, CVector const& b) const
            {
                // RVA 0x83BB00
                float const ax = m_origin.x - a.x;
                float const az = m_origin.z - a.z;
                float const ay = m_origin.y - a.y;
                float const bz = m_origin.z - b.z;
                float const by = m_origin.y - b.y;
                float const bx = m_origin.x - b.x;
                float const distSqA = ax * ax + az * az + ay * ay;
                float const distSqB = bz * bz + by * by + bx * bx;
                return distSqB > distSqA;
            }
        };
    }  // namespace

    RT_CLASS_EXPORTS_BEGIN(InfectionZone)
    RT_CLASS_EXPORT(InfectionZone, m3d::METHOD, ResetTimeOut, "", "", "")
    RT_CLASS_EXPORT(InfectionZone, m3d::METHOD, IsPlayerInside, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(InfectionZone);

    InfectionZonePrototypeInfo::InfectionZonePrototypeInfo()
    {
        this->m_minDistToPlayer = 100.0;
        this->m_criticalTeamDist = 1000000.0;
        this->m_criticalTeamTime = 0.0;
        this->m_blindTeamDist = 1000000.0;
        this->m_blindTeamTime = 0.0;
        this->m_dropOutSegmentAngle = 180;
    }

    bool InfectionZonePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_minDistToPlayer, xmlNode, "MinDistToPlayer");
            m3d::SafeFloatAttrib(m_criticalTeamDist, xmlNode, "CriticalTeamDist");
            m3d::SafeFloatAttrib(m_criticalTeamTime, xmlNode, "CriticalTeamTime");
            m3d::SafeFloatAttrib(m_blindTeamDist, xmlNode, "BlindTeamDist");
            m3d::SafeFloatAttrib(m_blindTeamTime, xmlNode, "BlindTeamTime");
            m3d::SafeIntAttrib(m_dropOutSegmentAngle, xmlNode, "DropOutSegmentAngle");
            return 1;
        }
        return result;
    }

    Obj* InfectionZonePrototypeInfo::CreateTargetObject() const
    {
        return new InfectionZone(*this);
    }

    void InfectionZone::GetPropertiesNames(
        retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x83F910
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    void InfectionZone::Registration()
    {
        m_propertiesMap["MinDistToPlayer"] = 70;
        m_propertiesMap["CriticalTeamDist"] = 71;
        m_propertiesMap["CriticalTeamTime"] = 72;
        m_propertiesMap["InfectionTeamPrototypeName"] = 73;
        m_propertiesMap["DropOutTimeOut"] = 74;
    }

    void InfectionZone::DeleteDropOutPoint(unsigned j)
    {
        // RVA 0x83FAC0
        m_dropOutPoints.erase(m_dropOutPoints.begin() + j);
    }

    void InfectionZone::Update(float elapsedTime, unsigned)
    {
        if (elapsedTime >= 0.001)
        {
            _WatchPlayerInside();
            if (m_hadPlayerInside)
                _WatchRespawn(elapsedTime);
        }
    }

    void InfectionZone::ResetTimeOut()
    {
        // RVA 0x83B430
        m_timeForRespawn = m_timeoutForRespawn;
    }

    void InfectionZone::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x83BCE0 - a save without the current timeout falls back to the base one.
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m_timeoutForRespawn = m_baseTimeoutForRespawn;
        m3d::SafeFloatAttrib(m_timeoutForRespawn, xmlNode, "CurrentDropOutTimeOut");
        m3d::SafeFloatAttrib(m_timeForRespawn, xmlNode, "DropOutTime");
        m3d::SafeBoolAttrib(m_hadPlayerInside, xmlNode, "HadPlayerInside");
    }

    void InfectionZone::AddPolygonPoint(float x, float y, unsigned j)
    {
        // RVA 0x83FF40 - inserts before point j.
        m_infectionPolygon.m_Points.insert(m_infectionPolygon.m_Points.begin() + j, geom2d::Point2<float>(x, y));
    }

    void InfectionZone::SetBelong(int newBelong)
    {
        ai::Obj::SetBelong(newBelong);
        auto* infectionTeam = theObjects->GetEntityByObjId(m_infectionTeamId);
        if (infectionTeam)
        {
            infectionTeam->SetBelong(newBelong);
        }
    }

    void InfectionZone::SetPolygonPoint(float x, float y, unsigned j)
    {
        // RVA 0x83DAF0 - no range check.
        m_infectionPolygon.m_Points[j].x = x;
        m_infectionPolygon.m_Points[j].y = y;
    }

    m3d::Class* InfectionZone::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void InfectionZone::DeletePolygonPoint(unsigned j)
    {
        // RVA 0x83FA80
        m_infectionPolygon.m_Points.erase(m_infectionPolygon.m_Points.begin() + j);
    }

    bool InfectionZone::IsPlayerInside()
    {
        // RVA 0x83FB00 - the polygon lies in the ground plane, so it is tested against x and z.
        Vehicle const* playerVehicle = _GetPlayerVehicle();
        if (!playerVehicle)
        {
            return false;
        }

        geom2d::Point2<float> const posPlayer(playerVehicle->GetPosition().x, playerVehicle->GetPosition().z);
        return m_infectionPolygon.isPointInside(posPlayer);
    }

    bool InfectionZone::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        int res = 1;
        switch (propertyId)
        {
        case 70:
            m_minDistToPlayer = newValue.GetAsFloat();
            res = 1;
            break;

        case 71:
            m_criticalTeamDist = newValue.GetAsFloat();
            res = 1;
            break;

        case 72:
            m_criticalTeamTime = newValue.GetAsFloat();
            res = 1;
            break;

        case 73:
            m_infectionTeamPrototypeName = newValue.GetAsStr();
            res = 1;
            break;

        case 74:
            m_baseTimeoutForRespawn = newValue.GetAsFloat();
            m_timeoutForRespawn = newValue.GetAsFloat();
            res = 1;
            break;

        default:
            res = ai::Obj::SetPropertyById(propertyId, newValue);
            break;
        }
        return res;
    }

    InfectionZone::InfectionZone(InfectionZonePrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_minDistToPlayer = prototypeInfo.m_minDistToPlayer;
        m_criticalTeamDist = prototypeInfo.m_criticalTeamDist;
        m_criticalTeamTime = prototypeInfo.m_criticalTeamTime;
        m_blindTeamDist = prototypeInfo.m_blindTeamDist;
        m_blindTeamTime = prototypeInfo.m_blindTeamTime;
        m_hadPlayerInside = false;
        m_infectionTeamId = -1;
        m_infectionLairId = -1;
        m_baseTimeoutForRespawn = 30.0f;
        m_timeoutForRespawn = 30.0f;
        m_dropOutCos = 0.0f;
        m_timeForRespawn = 0.0f;
        m_dropOutTimeOut = 0.0f;
        m_lastFramePlayerInsideWithoutEnemies = 0.0f;
        m_dropOutCos = cos((double)(prototypeInfo.m_dropOutSegmentAngle / 2) * 0.017453292);
    }

    void InfectionZone::AddChild(Obj* pObj)
    {
        // RVA 0x83CB00 - the zone owns at most one infection team and one lair.
        Obj::AddChild(pObj);
        if (!pObj)
        {
            return;
        }

        if (pObj->IsKindOf(&InfectionTeam::m_classInfectionTeam))
        {
            if (m_infectionTeamId != -1)
            {
                SYS_ERROR("m_infectionTeamId == INVALID_ID");
            }
            m_infectionTeamId = pObj->GetId();
        }
        else if (pObj->IsKindOf(&InfectionLair::m_classInfectionLair))
        {
            if (m_infectionLairId != -1)
            {
                SYS_ERROR("m_infectionLairId == INVALID_ID");
            }
            m_infectionLairId = pObj->GetId();
        }
        else
        {
            return;
        }
        pObj->LinkToParent(GetId(), Obj::HIERARCHY_CHILD);
    }

    void InfectionZone::_InternalPostLoad()
    {
        auto* infectionTeam = RT_DYNCAST(theObjects->GetEntityByObjId(m_infectionTeamId), InfectionTeam);
        if (infectionTeam)
        {
            infectionTeam->SetCriticalDistAndTime(m_criticalTeamDist, m_criticalTeamTime);
            infectionTeam->SetBlindDistAndTime(m_blindTeamDist, m_blindTeamTime);
        }
    }

    void InfectionZone::AddDropOutPoint(float x, float y)
    {
        // RVA 0x840810
        m_dropOutPoints.push_back(geom2d::Point2<float>(x, y));
    }

    bool InfectionZone::RemoveChild(Obj* pChild)
    {
        // RVA 0x83B520 - NOTE: whatever Obj::RemoveChild returns is ignored.
        Obj::RemoveChild(pChild);
        if (pChild)
        {
            if (pChild->IsKindOf(&InfectionTeam::m_classInfectionTeam))
            {
                if (m_infectionTeamId != -1)
                {
                    m_infectionTeamId = -1;
                    return true;
                }
            }
            else if (pChild->IsKindOf(&InfectionLair::m_classInfectionLair) && m_infectionLairId != -1)
            {
                m_infectionLairId = -1;
                return true;
            }
        }
        return false;
    }

    void InfectionZone::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x83F990
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    m3d::Class* InfectionZone::GetClass() const
    {
        return RT_CLASS_LOCAL(InfectionZone);
    }

    void InfectionZone::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x83D560
        Obj::SaveToXML(xmlFile, xmlNode);

        ref_ptr polygonNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Polygon");
        xmlNode->AddChild(polygonNode);
        for (auto const& point : m_infectionPolygon.m_Points)
        {
            ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
            polygonNode->AddChild(pointNode);
            pointNode->SetAttribute("x", CStr(point.x).c_str());
            pointNode->SetAttribute("y", CStr(point.y).c_str());
        }

        ref_ptr dropOutNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "DropOut");
        xmlNode->AddChild(dropOutNode);
        for (auto const& point : m_dropOutPoints)
        {
            ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
            dropOutNode->AddChild(pointNode);
            pointNode->SetAttribute("x", CStr(point.x).c_str());
            pointNode->SetAttribute("y", CStr(point.y).c_str());
        }

        xmlNode->SetAttribute("InfectionTeamPrototypeName", m_infectionTeamPrototypeName.c_str());
        // NOTE: the lair is saved under "InfectionLair" with the team prototype name as its value,
        // while LoadFromXML reads "InfectionLairName", so the lair name never survives a round trip.
        xmlNode->SetAttribute("InfectionLair", m_infectionTeamPrototypeName.c_str());
        xmlNode->SetAttribute("HadPlayerInside", CStr(static_cast<int>(m_hadPlayerInside)).c_str());
    }

    int InfectionZone::GetPropertyId(char const* propName) const
    {
        auto it = InfectionZone::m_propertiesMap.find(propName);
        if (it != InfectionZone::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::Obj::GetPropertyId(propName);
    }

    InfectionZonePrototypeInfo const* InfectionZone::GetPrototypeInfo() const
    {
        // RVA 0x83E840 - NOTE: the prototype is cast without a type check.
        return static_cast<InfectionZonePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    CStr InfectionZone::GetPropertyName(int id) const
    {
        // RVA 0x83FA10 - a linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    eGObjPropertySaveStatus InfectionZone::GetPropertySaveStatus(int id) const
    {
        // RVA 0x83F8D0
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertySaveStatus(id);
    }

    void InfectionZone::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x83BD70
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("CurrentDropOutTimeOut", CStr(m_timeoutForRespawn).c_str());
        xmlNode->SetAttribute("DropOutTime", CStr(m_timeForRespawn).c_str());
        xmlNode->SetAttribute("HadPlayerInside", CStr(static_cast<int>(m_hadPlayerInside)).c_str());
    }

    bool InfectionZone::CanChildBeAdded(m3d::Class* pClass) const
    {
        // RVA 0x83B4D0
        if (Obj::CanChildBeAdded(pClass))
        {
            return true;
        }
        return pClass->IsKindOf(&InfectionLair::m_classInfectionLair) ||
            pClass->IsKindOf(&InfectionTeam::m_classInfectionTeam);
    }

    void InfectionZone::Remove()
    {
        // RVA 0x83ED10 - the team goes with the zone.
        Obj::Remove();
        Obj* infectionTeam = theObjects->GetEntityByObjId(m_infectionTeamId);
        if (infectionTeam)
        {
            infectionTeam->Remove();
        }
    }

    void InfectionZone::RenderDebugInfo() const
    {
        // RVA 0x83D920 - the zone outline in white and a yellow cross above each drop out point.
        size_t const numPoints = m_infectionPolygon.m_Points.size();
        for (size_t i = 0; i < numPoints; ++i)
        {
            size_t next = i + 1;
            if (next == numPoints)
            {
                next = 0;
            }

            // NOTE: the original leaves the height of both ends uninitialized before dropping them
            // to the ground; it is zeroed here.
            CVector start(m_infectionPolygon.m_Points[i].x, 0.0f, m_infectionPolygon.m_Points[i].y);
            start = GetGroundPos(start, true, false);
            start.y += 2.0f;

            CVector finish(m_infectionPolygon.m_Points[next].x, 0.0f, m_infectionPolygon.m_Points[next].y);
            finish = GetGroundPos(finish, true, false);
            finish.y += 2.0f;

            M3D_APP->DrawLine(start, finish, 0xFFFFFFFF);
        }

        for (auto const& point : m_dropOutPoints)
        {
            CVector pos = GetGroundPos(CVector(point.x, 0.0f, point.y), true, false);
            pos.y += 50.0f;
            M3D_APP->DrawCross(pos, 5.0f, 0xFFFFFF00);
        }
    }

    void InfectionZone::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x840190 - the polygon and the drop out points both come as lists of <Point x="" y=""/>.
        Obj::LoadFromXML(xmlFile, xmlNode);

        // NOTE: the coordinates are not reset between points, so a point that lacks "x" or "y"
        // repeats the previous point's value (the original starts from uninitialized values).
        m_infectionPolygon.m_Points.clear();
        ref_ptr polygonNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(polygonNode, "Polygon");
        ref_ptr polygonPointNode = xmlFile->CreateNode();
        float polygonX = 0.0f;
        float polygonY = 0.0f;
        for (polygonNode->GetFirstChild(polygonPointNode, "Point"); !polygonPointNode->IsEmpty();
             polygonPointNode->GetNextSibling(polygonPointNode, "Point"))
        {
            m3d::SafeFloatAttrib(polygonX, polygonPointNode, "x");
            m3d::SafeFloatAttrib(polygonY, polygonPointNode, "y");
            m_infectionPolygon.m_Points.push_back(geom2d::Point2<float>(polygonX, polygonY));
        }

        m_dropOutPoints.clear();
        ref_ptr dropOutNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(dropOutNode, "DropOut");
        ref_ptr dropOutPointNode = xmlFile->CreateNode();
        float dropOutX = 0.0f;
        float dropOutY = 0.0f;
        for (dropOutNode->GetFirstChild(dropOutPointNode, "Point"); !dropOutPointNode->IsEmpty();
             dropOutPointNode->GetNextSibling(dropOutPointNode, "Point"))
        {
            m3d::SafeFloatAttrib(dropOutX, dropOutPointNode, "x");
            m3d::SafeFloatAttrib(dropOutY, dropOutPointNode, "y");
            m_dropOutPoints.push_back(geom2d::Point2<float>(dropOutX, dropOutY));
        }

        if (m_dropOutPoints.empty())
        {
            M3D_LOG_ERR(CStr("Error: ") + GetDebugDescription() + CStr(" has no drop out points"));
        }

        m3d::SafeStrAttrib(m_infectionTeamPrototypeName, xmlNode, "InfectionTeamPrototypeName");
        m3d::SafeStrAttrib(m_infectionLairName, xmlNode, "InfectionLairName");
        m3d::SafeBoolAttrib(m_hadPlayerInside, xmlNode, "HadPlayerInside");
    }

    bool InfectionZone::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x840EA0
        switch (propertyId)
        {
        case 70:
            retVal = GetPrototypeInfo()->m_minDistToPlayer;
            return true;
        case 71:
            retVal = GetPrototypeInfo()->m_criticalTeamDist;
            return true;
        case 72:
            retVal = GetPrototypeInfo()->m_criticalTeamTime;
            return true;
        case 73:
            retVal = CStr("");
            return true;
        case 74:
            retVal = GetPrototypeInfo()->m_dropOutTimeOut;
            return true;
        default:
            return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    bool InfectionZone::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x840E00
        switch (propertyId)
        {
        case 70:
            retVal = m_minDistToPlayer;
            return true;
        case 71:
            retVal = m_criticalTeamDist;
            return true;
        case 72:
            retVal = m_criticalTeamTime;
            return true;
        case 73:
            retVal = m_infectionTeamPrototypeName;
            return true;
        case 74:
            retVal = m_baseTimeoutForRespawn;
            return true;
        default:
            return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    void InfectionZone::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x840100 - the default save status is not stored.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    InfectionZone::~InfectionZone() = default;

    void InfectionZone::_TryDropOut(unsigned restSchwarz)
    {
        // RVA 0x840B80 - creates the zone's team on first use, then, if the team has no vehicles
        // left and the player is an enemy, spawns a fresh wave out of the player's sight and sends
        // it after the player.
        InfectionTeam* infectionTeam = nullptr;
        if (m_infectionTeamId != -1)
        {
            infectionTeam = _GetInfectionTeam();
        }

        if (!infectionTeam)
        {
            int const prototypeId = thePrototypeManager->GetPrototypeId(m_infectionTeamPrototypeName);
            if (prototypeId == -1)
            {
                return;
            }
            int const teamId = theObjects->CreateNewObject(prototypeId, "", -1, GetBelong());
            AddChild(theObjects->GetEntityByObjId(teamId));

            infectionTeam = _GetInfectionTeam();
            if (!infectionTeam)
            {
                return;
            }
        }

        if (!infectionTeam->GetVehicles().empty())
        {
            return;
        }

        // NOTE: the player's vehicle is dereferenced without a null check.
        Vehicle* playerVehicle = thePlayer ? thePlayer->GetVehicle() : nullptr;
        if (theRelationship->CheckTolerance(GetBelong(), playerVehicle->GetBelong()) <= RS_ENEMY)
        {
            retruxx::vector<CVector> points;
            _GetGoodRandomDropOutPoints(points);
            if (!points.empty())
            {
                infectionTeam->Generate(points, restSchwarz);
                infectionTeam->SetCriticalDistAndTime(m_criticalTeamDist, m_criticalTeamTime);
                infectionTeam->SetBlindDistAndTime(m_blindTeamDist, m_blindTeamTime);
                infectionTeam->AttackNow(thePlayer->GetVehicle()->GetId());
            }
        }
    }

    unsigned InfectionZone::_GetPlayerSchwarz() const
    {
        // RVA 0x83B5B0
        Vehicle const* playerVehicle = _GetPlayerVehicle();
        return playerVehicle ? playerVehicle->GetSchwarz() : 0;
    }

    InfectionTeam* InfectionZone::_GetInfectionTeam()
    {
        // RVA 0x83E870 - NOTE: the object is cast without a type check.
        return static_cast<InfectionTeam*>(theObjects->GetEntityByObjId(m_infectionTeamId));
    }

    void InfectionZone::_WatchRespawn(float elapsedTime)
    {
        // RVA 0x840F80 - the respawn clock only runs while the player is inside and the previous
        // wave is gone.
        InfectionTeam* infectionTeam = _GetInfectionTeam();
        if (!m_hadPlayerInside || (infectionTeam && !infectionTeam->GetVehicles().empty()))
        {
            m_timeForRespawn = 0.0f;
        }
        else
        {
            _WatchRespawnTimeoutFinished(elapsedTime);
        }
    }

    void InfectionZone::_WatchRespawnTimeoutFinished(float elapsedTime)
    {
        // RVA 0x840D40 - once the timeout runs out, a wave is sent if the enemies already near the
        // player are worth less than half of the player. Each wave that does spawn makes the next
        // one take longer.
        if (m_timeForRespawn < m_timeoutForRespawn)
        {
            m_timeForRespawn = elapsedTime + m_timeForRespawn;
            return;
        }

        unsigned const enemiesSchwarz = _GetEnemiesSchwarz();
        unsigned const playerSchwarz = _GetPlayerSchwarz();
        if (enemiesSchwarz < playerSchwarz >> 1)
        {
            _TryDropOut(playerSchwarz - enemiesSchwarz);
            InfectionTeam* infectionTeam = _GetInfectionTeam();
            if (infectionTeam && !infectionTeam->GetVehicles().empty())
            {
                m_timeoutForRespawn = m_timeoutForRespawn * theGlobProp.m_zoneRespawnTimeOutIncreaseCoeff;
                m_timeForRespawn = 0.0f;
            }
        }
    }

    Vehicle const* InfectionZone::_GetPlayerVehicle() const
    {
        // RVA 0x83B590
        return thePlayer ? thePlayer->GetVehicle() : nullptr;
    }

    unsigned InfectionZone::_GetEnemiesSchwarz() const
    {
        // RVA 0x83E8F0 - the combined strength of the hostile vehicles around the player.
        Vehicle const* playerVehicle = _GetPlayerVehicle();
        if (!playerVehicle)
        {
            return 0;
        }

        unsigned enemiesSchwarz = 0;
        for (auto const& obstacle : playerVehicle->GetNearbyObstacles())
        {
            PhysicObj* obj = obstacle->GetOwnerPhysicObj();
            if (obj && obj->IsKindOf(&Vehicle::m_classVehicle) &&
                theRelationship->CheckTolerance(playerVehicle->GetBelong(), obj->GetBelong()) <= RS_ENEMY)
            {
                enemiesSchwarz += obj->GetSchwarz();
            }
        }
        return enemiesSchwarz;
    }

    void InfectionZone::_WatchPlayerInside()
    {
        // RVA 0x83FF80 - entering the zone starts the respawn clock part of the way through, so the
        // first wave comes somewhat sooner than the later ones. Leaving it resets the clock.
        bool const isPlayerInside = IsPlayerInside();
        if (isPlayerInside == m_hadPlayerInside)
        {
            return;
        }

        if (m_hadPlayerInside)
        {
            m_hadPlayerInside = isPlayerInside;
            m_timeoutForRespawn = m_baseTimeoutForRespawn;
            m_timeForRespawn = 0.0f;
        }
        else
        {
            m_timeoutForRespawn = m_baseTimeoutForRespawn;
            float const timeForRespawn = m_timeoutForRespawn -
                static_cast<float>(rand()) * 0.000030518509f * theGlobProp.m_zoneDefaultFirstSpawnTime;
            m_timeForRespawn = 0.0f > timeForRespawn ? 0.0f : timeForRespawn;
            m_hadPlayerInside = isPlayerInside;
        }
    }

    m3d::Object* InfectionZone::CreateObject()
    {
        // RVA 0x83C940
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    InfectionLair* InfectionZone::_GetInfectionLair()
    {
        // RVA 0x83E8B0 - NOTE: the object is cast without a type check.
        return static_cast<InfectionLair*>(theObjects->GetEntityByObjId(m_infectionLairId));
    }

    m3d::Object* InfectionZone::Clone()
    {
        // RVA 0x83C780
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void InfectionZone::_GetGoodRandomDropOutPoints(retruxx::vector<CVector, retruxx::allocator<CVector>>& points) const
    {
        // RVA 0x840970 - picks up to two drop out points that the player cannot see, that are far
        // enough away and that lie ahead of the player within the drop out segment, nearest first.
        // They are inserted at the front of points. NOTE: the player and the player's vehicle are
        // dereferenced without null checks.
        retruxx::vector<CVector> candidates;
        CVector const playerPos = thePlayer->GetVehicle()->GetPosition();
        CVector const playerDir = thePlayer->GetVehicle()->GetDirection();

        for (auto const& dropOutPoint : m_dropOutPoints)
        {
            CVector const point = GetGroundPos(CVector2(dropOutPoint.x, dropOutPoint.y), true);

            int const cellX = static_cast<int>(VISCELL_EDGE_LENGTH_INV * point.x);
            int const cellZ = static_cast<int>(VISCELL_EDGE_LENGTH_INV * point.z);
            if (pServer->GetWorld()->GetGraph().IsCellVisible(cellX, cellZ))
            {
                continue;
            }

            // The original evaluates this on the x87 stack; only the distance is rounded to float.
            double const dx = double(point.x) - playerPos.x;
            double const dy = double(point.y) - playerPos.y;
            double const dz = double(point.z) - playerPos.z;
            float const dist = static_cast<float>(std::sqrt(dz * dz + dy * dy + dx * dx));
            if (dist > m_minDistToPlayer)
            {
                double const dirLen = std::sqrt(
                    double(playerDir.z) * playerDir.z + double(playerDir.y) * playerDir.y +
                    double(playerDir.x) * playerDir.x);
                double const cosToPoint = (playerDir.z * dz + playerDir.y * dy + dx * playerDir.x) / (dirLen * dist);
                if (cosToPoint > m_dropOutCos)
                {
                    candidates.push_back(point);
                }
            }
        }

        if (candidates.empty())
        {
            return;
        }

        // NOTE: std::sort is not the shipped sort, so points at exactly the same distance may come
        // out in a different order.
        std::sort(candidates.begin(), candidates.end(), CVectorDistComparer(playerPos));
        size_t const count = std::min<size_t>(candidates.size(), 2);
        points.insert(points.begin(), candidates.begin(), candidates.begin() + count);
    }
}  // namespace ai

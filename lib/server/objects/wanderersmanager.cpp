#include "wanderersmanager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <world.h>
#include <scene/scenegraph.h>
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "caravanteam.h"
#include "location.h"
#include "vagabondteam.h"
#include "vehicle.h"
#include "../event.h"
#include "../processmanager.h"
#include "../server.h"

namespace ai
{
    namespace
    {
        float const VISCELL_EDGE_LENGTH_INV = 0.0078125f;

        // The shipped randomUniform<int> and randomUniform<float> templates.
        int RandomUniformInt(int left, int right)
        {
            return left + rand() % (right - left + 1);
        }

        float RandomUniformFloat(float left, float right)
        {
            return static_cast<float>(static_cast<double>(rand()) * 0.000030518509 * (right - left) + left);
        }

        bool IsWayPointVisible(CVector2 const& position)
        {
            int const cellX = static_cast<int>(position.x * VISCELL_EDGE_LENGTH_INV);
            int const cellZ = static_cast<int>(position.y * VISCELL_EDGE_LENGTH_INV);
            return pServer->GetWorld()->GetGraph().IsCellVisible(cellX, cellZ);
        }
    }  // namespace

    RT_CLASS_EXPORTS_BEGIN(WanderersManager)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(WanderersManager);

    WanderersManagerPrototypeInfo::WanderersManagerPrototypeInfo() = default;

    bool WanderersManagerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        return result;
    }

    ai::Obj* WanderersManagerPrototypeInfo::CreateTargetObject() const
    {
        return new WanderersManager(*this);
    }

    WanderersManager::WayPoint::WayPoint(
        bool transition,
        CVector2 const& position,
        CStr const& wayPointName,
        int locationId) :
        m_transition(transition),
        m_position(position),
        m_locationId(locationId),
        m_name(wayPointName)
    {
        // RVA 0x866180
    }

    WanderersManager::WayPoint::WayPoint(WayPoint const&) = default;

    void WanderersManager::CaravanInfo::LoadFromXML(
        WanderersManager const* manager,
        m3d::cmn::XmlFile*,
        m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8698A0 - "Locations" lists the waypoints the caravan visits in order.
        m3d::SafeStrAttrib(m_prototypeName, xmlNode, "Prototype");

        CStr strWayPoints;
        m3d::SafeStrAttrib(strWayPoints, xmlNode, "Locations");
        retruxx::vector<CStr> wayPointNames;
        m3d::Tokenize(strWayPoints, wayPointNames, "(), ;\t");
        for (auto const& wayPointName : wayPointNames)
        {
            int const index = manager->_GetWayPointIndex(wayPointName);
            // NOTE: an unknown waypoint is reported but its -1 index is still stored.
            if (index == -1)
            {
                SYS_ERROR("index != -1");
            }
            m_wayPointIndices.push_back(index);
        }

        if (m_wayPointIndices.size() < 2)
        {
            M3D_LOG_ERR(CStr("Error: not enough points for caravan in ") + manager->GetDebugDescription());
            SYS_ERROR("!\"Critical error, see log\"");
        }
    }

    WanderersManager::WandererState::WandererState() : m_timeout(0.0f)
    {
        // RVA 0x868640 - the indices are left uninitialized.
    }

    WanderersManager::WandererState::WandererState(WandererState const&) = default;

    void WanderersManager::WandererState::LoadFromXML(
        WanderersManager* manager,
        m3d::cmn::XmlFile*,
        m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x869E90 - the path itself is not saved; it is rebuilt from its two ends.
        m3d::SafeIntAttrib(m_wayPointIndex, xmlNode, "WayPointIndex");
        m3d::SafeIntAttrib(m_precisePathIndex, xmlNode, "PrecisePathIndex");
        m3d::SafeIntAttrib(m_caravanInfoIndex, xmlNode, "CaravanInfoIndex");
        m3d::SafeIntAttrib(m_precisePathBegin, xmlNode, "PrecisePathBegin");
        m3d::SafeIntAttrib(m_precisePathEnd, xmlNode, "PrecisePathEnd");
        m3d::SafeFloatAttrib(m_timeout, xmlNode, "Timeout");
        manager->_SetStatePrecisePath(*this, m_precisePathBegin, m_precisePathEnd);
    }

    WanderersManager::~WanderersManager() = default;

    WanderersManager::WanderersManager(ai::WanderersManagerPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_rebornTimeout = 10.0f;
        m_timeBeforeReborn = 0.0f;
    }

    m3d::Object* WanderersManager::Clone()
    {
        // RVA 0x8661C0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* WanderersManager::CreateObject()
    {
        // RVA 0x866380
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* WanderersManager::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::Class* WanderersManager::GetClass() const
    {
        return RT_CLASS_LOCAL(WanderersManager);
    }

    ai::WanderersManagerPrototypeInfo const* WanderersManager::GetPrototypeInfo() const
    {
        // RVA 0x867770 - NOTE: the prototype is cast without a type check.
        return static_cast<WanderersManagerPrototypeInfo const*>(
            thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    int WanderersManager::OnEvent(ai::Event const& evn)
    {
        // RVA 0x86FF10
        int result = Obj::OnEvent(evn);
        switch (evn.m_eventId)
        {
        case GE_OBJECT_ENTERS_LOCATION:
            _OnObjectEntersLocation(evn);
            result = 1;
            break;
        case GE_OBJECT_DIE:
            _OnObjectDie(evn);
            result = 1;
            break;
        case GE_LOST_GUARDS_NEED_DIRECTION:
            _OnLostGuardsNeedDirection(evn);
            result = 1;
            break;
        case GE_TARGET_REACHED:
            _OnTargetReached(evn);
            result = 1;
            break;
        default:
            break;
        }
        return result;
    }

    bool WanderersManager::CanChildBeAdded(m3d::Class* pClass) const
    {
        // RVA 0x864C50
        return pClass->IsKindOf(&VagabondTeam::m_classVagabondTeam) ||
            pClass->IsKindOf(&CaravanTeam::m_classCaravanTeam) || Obj::CanChildBeAdded(pClass);
    }

    void WanderersManager::AddChild(ai::Obj* pObj)
    {
        // RVA 0x866540 - the teams take the manager's side. NOTE: pObj is not checked for null.
        Obj::AddChild(pObj);
        pObj->SetBelong(GetBelong());
        pObj->LinkToParent(GetId(), Obj::HIERARCHY_CHILD);
    }

    void WanderersManager::Update(float elapsedTime, unsigned int)
    {
        // RVA 0x870120 - a new team is spawned every m_rebornTimeout seconds while there is room
        // for one, and the first vagabond whose rest is over is sent on its way.
        if (static_cast<int>(m_wandererStates.size()) >= m_maxTeamCount)
        {
            m_timeBeforeReborn = 0.0f;
        }
        else if (m_rebornTimeout <= m_timeBeforeReborn)
        {
            // NOTE: the timer restarts even when the spawn point turned out to be visible and
            // nothing was spawned.
            _SpawnWanderer();
            m_timeBeforeReborn = 0.0f;
        }
        else
        {
            m_timeBeforeReborn = m_timeBeforeReborn + elapsedTime;
        }

        for (auto it = m_waitingVagabonds.begin(); it != m_waitingVagabonds.end(); ++it)
        {
            if (m_wandererStates.find(*it) == m_wandererStates.end())
            {
                SYS_ERROR("m_wandererStates.find( *it ) != m_wandererStates.end()");
            }

            WandererState& state = m_wandererStates[*it];
            state.m_timeout = state.m_timeout - elapsedTime;
            if (state.m_timeout < 0.0f)
            {
                // NOTE: the team is cast without a type or null check, and only one vagabond is
                // released per update.
                auto* team = static_cast<Team*>(theObjects->GetEntityByObjId(*it));
                state.m_precisePathIndex = 1;
                _SendTeamToPrecisePathPoint(team, state);
                m_waitingVagabonds.erase(it);
                return;
            }
        }
    }

    void WanderersManager::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x86F4C0
        Obj::LoadFromXML(xmlFile, xmlNode);
        if (CStr(GetName()).empty())
        {
            M3D_LOG_ERR(CStr("Error: name of ") + GetDebugDescription() + CStr(" is empty"));
            SYS_ERROR("!\"Critical error, see log\"");
        }

        m3d::SafeIntAttrib(m_maxTeamCount, xmlNode, "TeamCount");
        m3d::SafeFloatAttrib(m_rebornTimeout, xmlNode, "RebornTimeout");

        _LoadWayPointsFromXML(xmlFile, xmlNode);
        _LoadPrecisePathsFromXML(xmlFile, xmlNode);
        _LoadCaravansInfoFromXML(xmlFile, xmlNode);
        _LoadVagabondsInfoFromXML(xmlFile, xmlNode);
        _LoadWanderersStatesFromXML(xmlFile, xmlNode);

        // Vagabonds are spawned at transition waypoints.
        if (m_transitionIndices.empty() && !m_vagabondPrototypeIds.empty())
        {
            M3D_LOG_ERR(CStr("Error: ") + GetDebugDescription() + CStr(" has vagabonds but no transitions"));
            SYS_ERROR("!\"Critical error, see log\"");
        }

        if (m_caravanInfos.empty() && m_vagabondPrototypeIds.empty())
        {
            M3D_LOG_ERR(CStr("Error: ") + GetDebugDescription() + CStr(" has no caravans and no vagabonds"));
            SYS_ERROR("!\"Critical error, see log\"");
        }
    }

    void WanderersManager::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8682B0
        Obj::SaveToXML(xmlFile, xmlNode);
        xmlNode->SetAttribute("TeamCount", CStr(m_maxTeamCount).c_str());
        xmlNode->SetAttribute("RebornTimeout", CStr(m_rebornTimeout).c_str());
        _SaveWayPointsToXML(xmlFile, xmlNode);
        _SavePrecisePathsToXML(xmlFile, xmlNode);
        _SaveCaravansInfoToXML(xmlFile, xmlNode);
        _SaveVagabondInfoToXML(xmlFile, xmlNode);
        _SaveWanderersStatesToXML(xmlFile, xmlNode);
    }

    void WanderersManager::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8658D0
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeFloatAttrib(m_timeBeforeReborn, xmlNode, "TimeBeforeReborn");
    }

    void WanderersManager::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x865920
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TimeBeforeReborn", CStr(m_timeBeforeReborn).c_str());
    }

    unsigned int WanderersManager::GetCaravansNum() const
    {
        // NOTE: never instantiated in the shipped binary, so this body does not come from it.
        return static_cast<unsigned int>(m_caravanInfos.size());
    }

    ai::WanderersManager::CaravanInfo const& WanderersManager::GetCaravanInfo(unsigned int index) const
    {
        // NOTE: never instantiated in the shipped binary, so this body does not come from it.
        return m_caravanInfos[index];
    }

    unsigned int WanderersManager::GetWayPointsNum() const
    {
        // NOTE: never instantiated in the shipped binary, so this body does not come from it.
        return static_cast<unsigned int>(m_wayPoints.size());
    }

    ai::WanderersManager::WayPoint const& WanderersManager::GetWayPoint(unsigned int index) const
    {
        // NOTE: never instantiated in the shipped binary, so this body does not come from it.
        return m_wayPoints[index];
    }

    void WanderersManager::_InternalPostLoad()
    {
        // RVA 0x86A3B0 - outside the editor, every waypoint without a live location gets a
        // generic one, and the manager subscribes to it to learn when teams arrive.
        if (theObjects->m_SaveType == ObjContainer::SAVE_EDITOR)
        {
            return;
        }

        int cnt = 1;
        for (auto& wayPoint : m_wayPoints)
        {
            Obj* location = theObjects->GetEntityByObjId(wayPoint.m_locationId);
            if (location && location->IsKindOf(&Location::m_classLocation))
            {
                continue;
            }

            CStr const locationName = CStr(GetName()) + CStr("_Location_") + CStr(cnt);
            ++cnt;
            int const locationId = theObjects->CreateNewObject(
                thePrototypeManager->GetPrototypeId(CStr("genericLocation")), locationName.c_str(), -1, -1);

            // NOTE: the new location is used without a null check.
            auto* newLocation = static_cast<Location*>(theObjects->GetEntityByObjId(locationId));
            newLocation->SetPositionSelf(CVector(wayPoint.m_position.x, 0.0f, wayPoint.m_position.y));
            newLocation->SetRadius(30.0f);
            wayPoint.m_locationId = locationId;

            theProcessManager->PostMessageA(
                GE_SUBSCRIBE,
                locationId,
                GetId(),
                0.0f,
                m3d::AIParam(static_cast<int>(GE_OBJECT_ENTERS_LOCATION)),
                m3d::AIParam(),
                1);
        }
    }

    void WanderersManager::_LoadWayPointsFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x86A730
        ref_ptr wayPointsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(wayPointsNode, "Points");
        if (!wayPointsNode->IsEmpty())
        {
            ref_ptr pointNode = xmlFile->CreateNode();
            for (wayPointsNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty();
                 pointNode->GetNextSibling(pointNode, "Point"))
            {
                CVector2 locationPos;
                CStr wayPointName;
                bool transition = false;
                int locationId = -1;

                m3d::SafeStrAttrib(wayPointName, pointNode, "Name");
                if (wayPointName.empty())
                {
                    SYS_ERROR("!wayPointName.empty()");
                }
                m3d::SafeVector2Attrib(locationPos, pointNode, "Coord");
                m3d::SafeBoolAttrib(transition, pointNode, "Transition");
                m3d::SafeIntAttrib(locationId, pointNode, "LocationId");

                if (m_wayPointIndexMap.find(wayPointName) != m_wayPointIndexMap.end())
                {
                    M3D_LOG_ERR(
                        CStr("Error: duplicate waypoint name: '") + wayPointName + CStr("' for ") +
                        GetDebugDescription());
                    SYS_ERROR("!\"Critical error, see log\"");
                }

                m_wayPointIndexMap[wayPointName] = static_cast<int>(m_wayPoints.size());
                if (transition)
                {
                    m_transitionIndices.push_back(static_cast<int>(m_wayPoints.size()));
                }
                m_wayPoints.push_back(WayPoint(transition, locationPos, wayPointName, locationId));
            }
        }

        if (m_wayPoints.empty())
        {
            M3D_LOG_ERR(CStr("Error: no waypoints for ") + GetDebugDescription());
            SYS_ERROR("!\"Critical error, see log\"");
        }
    }

    void WanderersManager::_LoadPrecisePathsFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x86C8D0 - the hand-placed routes between pairs of waypoints.
        ref_ptr precisePathsNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(precisePathsNode, "Paths");
        if (precisePathsNode->IsEmpty())
        {
            return;
        }

        ref_ptr pathNode = xmlFile->CreateNode();
        for (precisePathsNode->GetFirstChild(pathNode, "Path"); !pathNode->IsEmpty();
             pathNode->GetNextSibling(pathNode, "Path"))
        {
            CStr beginName;
            CStr endName;
            PrecisePath path;
            m3d::SafeStrAttrib(beginName, pathNode, "Begin");
            m3d::SafeStrAttrib(endName, pathNode, "End");
            // NOTE: unknown waypoint names are not reported; the path is stored under index -1.
            int const beginIndex = _GetWayPointIndex(beginName);
            int const endIndex = _GetWayPointIndex(endName);

            // NOTE: a point without "Coord" repeats the previous point.
            CVector2 point;
            ref_ptr pointNode = xmlFile->CreateNode();
            for (pathNode->GetFirstChild(pointNode, "Point"); !pointNode->IsEmpty();
                 pointNode->GetNextSibling(pointNode, "Point"))
            {
                m3d::SafeVector2Attrib(point, pointNode, "Coord");
                path.push_back(point);
            }

            if (path.empty())
            {
                M3D_LOG_ERR(CStr("Error: precise path has no points in ") + GetDebugDescription());
                SYS_ERROR("!\"Critical error, see log\"");
            }

            WayPointsPair const key(beginIndex, endIndex);
            if (m_precisePaths.find(key) != m_precisePaths.end())
            {
                M3D_LOG_ERR(
                    CStr("Error: duplicate path from '") + beginName + CStr("' to '") + endName + CStr("' for ") +
                    GetDebugDescription());
                SYS_ERROR("!\"Critical error, see log\"");
            }
            m_precisePaths[key] = path;
        }
    }

    void WanderersManager::_LoadCaravansInfoFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x86D6C0
        ref_ptr caravansNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(caravansNode, "Caravans");
        if (caravansNode->IsEmpty())
        {
            return;
        }

        ref_ptr caravanNode = xmlFile->CreateNode();
        for (caravansNode->GetFirstChild(caravanNode, "Caravan"); !caravanNode->IsEmpty();
             caravanNode->GetNextSibling(caravanNode, "Caravan"))
        {
            m_caravanInfos.push_back(CaravanInfo());
            m_caravanInfos.back().LoadFromXML(this, xmlFile, caravanNode);
        }
    }

    void WanderersManager::_LoadVagabondsInfoFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x869120
        ref_ptr vagabondsNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(vagabondsNode, "Vagabonds");
        if (vagabondsNode->IsEmpty())
        {
            return;
        }

        CStr strPrototypes;
        m3d::SafeStrAttrib(strPrototypes, vagabondsNode, "Prototypes");
        retruxx::vector<CStr> vagabondPrototypeNames;
        m3d::Tokenize(strPrototypes, vagabondPrototypeNames, "(), ;\t");

        m_vagabondPrototypeIds.clear();
        for (auto const& prototypeName : vagabondPrototypeNames)
        {
            m_vagabondPrototypeIds.push_back(thePrototypeManager->GetPrototypeId(prototypeName));
            if (m_vagabondPrototypeIds.back() == -1)
            {
                // NOTE: the message never closes the quote it opens around the prototype name.
                M3D_LOG_ERR(
                    CStr("Error: invalid vagabond prototype: '") + prototypeName + CStr(" in ") +
                    GetDebugDescription());
                SYS_ERROR("!\"Critical error, see log\"");
            }
        }
    }

    void WanderersManager::_LoadWanderersStatesFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x86DA80 - the teams that were on the road when the game was saved.
        ref_ptr wandererStatesNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(wandererStatesNode, "Wanderers");
        if (wandererStatesNode->IsEmpty())
        {
            return;
        }

        CStr strWaitingVagabonds;
        m3d::SafeStrAttrib(strWaitingVagabonds, wandererStatesNode, "WaitingVagabonds");
        retruxx::vector<CStr> waitingVagabonds;
        m3d::Tokenize(strWaitingVagabonds, waitingVagabonds, ",");
        for (auto const& waitingVagabond : waitingVagabonds)
        {
            m_waitingVagabonds.push_back(atoi(waitingVagabond.c_str()));
        }

        // NOTE: a state without "TeamId" reuses the previous one (the original starts from an
        // uninitialized value).
        int teamId = -1;
        ref_ptr stateNode = xmlFile->CreateNode();
        for (wandererStatesNode->GetFirstChild(stateNode, "TeamState"); !stateNode->IsEmpty();
             stateNode->GetNextSibling(stateNode, "TeamState"))
        {
            m3d::SafeIntAttrib(teamId, stateNode, "TeamId");
            m_wandererStates[teamId] = WandererState();
            m_wandererStates[teamId].LoadFromXML(this, xmlFile, stateNode);
        }
    }

    void WanderersManager::_SaveWayPointsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x866570 - the location ids are only meaningful in a full save.
        ref_ptr wayPointsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Points");
        xmlNode->AddChild(wayPointsNode);
        for (auto const& wayPoint : m_wayPoints)
        {
            ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
            wayPointsNode->AddChild(pointNode);
            pointNode->SetAttribute("Name", wayPoint.m_name.c_str());
            pointNode->SetAttribute(
                "Coord", (CStr(wayPoint.m_position.x) + CStr(" ") + CStr(wayPoint.m_position.y)).c_str());
            pointNode->SetAttribute("Transition", wayPoint.m_transition ? "true" : "false");
            if (theObjects->m_SaveType == ObjContainer::SAVE_FULL)
            {
                pointNode->SetAttribute("LocationId", CStr(wayPoint.m_locationId).c_str());
            }
        }
    }

    void WanderersManager::_SavePrecisePathsToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8677C0
        if (m_precisePaths.empty())
        {
            return;
        }

        ref_ptr precisePathsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Paths");
        xmlNode->AddChild(precisePathsNode);
        for (auto const& [wayPointsPair, path] : m_precisePaths)
        {
            ref_ptr pathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Path");
            precisePathsNode->AddChild(pathNode);
            pathNode->SetAttribute("Begin", m_wayPoints[wayPointsPair.first].m_name.c_str());
            pathNode->SetAttribute("End", m_wayPoints[wayPointsPair.second].m_name.c_str());
            for (auto const& point : path)
            {
                ref_ptr pointNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Point");
                pathNode->AddChild(pointNode);
                pointNode->SetAttribute("Coord", (CStr(point.x) + CStr(" ") + CStr(point.y)).c_str());
            }
        }
    }

    void WanderersManager::_SaveCaravansInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8670E0
        if (m_caravanInfos.empty())
        {
            return;
        }

        ref_ptr caravansNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Caravans");
        xmlNode->AddChild(caravansNode);
        for (auto const& caravanInfo : m_caravanInfos)
        {
            ref_ptr caravanNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Caravan");
            caravansNode->AddChild(caravanNode);
            caravanNode->SetAttribute("Prototype", caravanInfo.m_prototypeName.c_str());

            CStr locations;
            for (int wayPointIndex : caravanInfo.m_wayPointIndices)
            {
                if (!locations.empty())
                {
                    locations += CStr(" ") + m_wayPoints[wayPointIndex].m_name;
                }
                else
                {
                    locations = m_wayPoints[wayPointIndex].m_name;
                }
            }
            caravanNode->SetAttribute("Locations", locations.c_str());
        }
    }

    void WanderersManager::_SaveVagabondInfoToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8668B0
        if (m_vagabondPrototypeIds.empty())
        {
            return;
        }

        ref_ptr vagabondsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Vagabonds");
        xmlNode->AddChild(vagabondsNode);

        CStr prototypes;
        for (int prototypeId : m_vagabondPrototypeIds)
        {
            CStr const prototypeName = thePrototypeManager->GetPrototypeName(prototypeId);
            if (!prototypes.empty())
            {
                prototypes += CStr(" ") + prototypeName;
            }
            else
            {
                prototypes = prototypeName;
            }
        }
        vagabondsNode->SetAttribute("Prototypes", prototypes.c_str());
    }

    void WanderersManager::_SaveWanderersStatesToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x867B90
        if (m_wandererStates.empty())
        {
            return;
        }

        ref_ptr wandererStatesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Wanderers");
        xmlNode->AddChild(wandererStatesNode);

        if (!m_waitingVagabonds.empty())
        {
            CStr waitingVagabonds;
            for (int waitingVagabond : m_waitingVagabonds)
            {
                if (!waitingVagabonds.empty())
                {
                    waitingVagabonds += CStr(",") + CStr(waitingVagabond);
                }
                else
                {
                    waitingVagabonds = CStr(waitingVagabond);
                }
            }
            wandererStatesNode->SetAttribute("WaitingVagabonds", waitingVagabonds.c_str());
        }

        for (auto const& [teamId, state] : m_wandererStates)
        {
            ref_ptr stateNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "TeamState");
            wandererStatesNode->AddChild(stateNode);
            stateNode->SetAttribute("TeamId", CStr(teamId).c_str());
            stateNode->SetAttribute("WayPointIndex", CStr(state.m_wayPointIndex).c_str());
            stateNode->SetAttribute("PrecisePathIndex", CStr(state.m_precisePathIndex).c_str());
            stateNode->SetAttribute("CaravanInfoIndex", CStr(state.m_caravanInfoIndex).c_str());
            stateNode->SetAttribute("PrecisePathBegin", CStr(state.m_precisePathBegin).c_str());
            stateNode->SetAttribute("PrecisePathEnd", CStr(state.m_precisePathEnd).c_str());
            stateNode->SetAttribute("Timeout", CStr(state.m_timeout).c_str());
        }
    }

    void WanderersManager::_SpawnWanderer()
    {
        // RVA 0x86FD70 - caravans and vagabond prototypes are equally likely.
        int const caravansCount = static_cast<int>(m_caravanInfos.size());
        int const vagabondsCount = static_cast<int>(m_vagabondPrototypeIds.size());
        int const index = rand() % (caravansCount + vagabondsCount);
        if (index < caravansCount)
        {
            _SpawnCaravanTeam(index);
        }
        else
        {
            _SpawnVagabondTeam(index - caravansCount);
        }
    }

    void WanderersManager::_SpawnCaravanTeam(int index)
    {
        // RVA 0x86DD60 - the caravan appears at the first waypoint of its route, unless the
        // player can see that spot.
        CaravanInfo const& caravanInfo = m_caravanInfos[index];
        WayPoint const& startWayPoint = m_wayPoints[caravanInfo.m_wayPointIndices[0]];
        if (IsWayPointVisible(startWayPoint.m_position))
        {
            return;
        }

        int const protoId = thePrototypeManager->GetPrototypeId(caravanInfo.m_prototypeName);
        if (protoId == -1)
        {
            SYS_ERROR("protoId != INVALID_ID");
        }
        int const teamId = theObjects->CreateNewObject(protoId, "", -1, -1);
        // NOTE: the team is cast without a type or null check.
        auto* team = static_cast<CaravanTeam*>(theObjects->GetEntityByObjId(teamId));
        AddChild(team);
        team->GenerateAndPlace(CVector(startWayPoint.m_position.x, 0.0f, startWayPoint.m_position.y));

        WandererState wandererState;
        _SetStatePrecisePath(wandererState, caravanInfo.m_wayPointIndices[0], caravanInfo.m_wayPointIndices[1]);
        wandererState.m_wayPointIndex = 1;
        wandererState.m_precisePathIndex = 1;
        wandererState.m_caravanInfoIndex = index;
        _SendTeamToPrecisePathPoint(team, wandererState);
        m_wandererStates[teamId] = wandererState;

        theProcessManager->PostMessageA(
            GE_SUBSCRIBE, teamId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_TARGET_REACHED)), m3d::AIParam(), 1);
        theProcessManager->PostMessageA(
            GE_SUBSCRIBE, teamId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);

        M3D_LOG_INFO(
            CStr("Spawning caravan: ") + GetDebugDescription() + CStr(" spawns ") + team->GetDebugDescription());
    }

    void WanderersManager::_SpawnVagabondTeam(int index)
    {
        // RVA 0x86E3D0 - vagabonds enter at a random transition waypoint and head for any other
        // waypoint, unless the player can see the entry point.
        if (static_cast<unsigned int>(index) >= m_vagabondPrototypeIds.size())
        {
            SYS_ERROR("( uint )index < m_vagabondPrototypeIds.size()");
        }

        // NOTE: there is no check for an empty transition list (LoadFromXML only reports it), and
        // a level with a single waypoint never gets out of the loop below.
        int const beginIndex = m_transitionIndices[rand() % m_transitionIndices.size()];
        int endIndex = rand() % m_wayPoints.size();
        while (endIndex == beginIndex)
        {
            endIndex = rand() % m_wayPoints.size();
        }

        WayPoint const& startWayPoint = m_wayPoints[beginIndex];
        if (IsWayPointVisible(startWayPoint.m_position))
        {
            return;
        }

        int const protoId = m_vagabondPrototypeIds[index];
        if (protoId == -1)
        {
            SYS_ERROR("protoId != INVALID_ID");
        }
        int const teamId = theObjects->CreateNewObject(protoId, "", -1, -1);
        // NOTE: the team is cast without a type or null check.
        auto* team = static_cast<VagabondTeam*>(theObjects->GetEntityByObjId(teamId));
        AddChild(team);
        team->GenerateAndPlace(CVector(startWayPoint.m_position.x, 0.0f, startWayPoint.m_position.y));

        WandererState wandererState;
        // NOTE: the original leaves m_caravanInfoIndex uninitialized for a vagabond and saves
        // whatever it holds; it is zeroed here.
        wandererState.m_caravanInfoIndex = 0;
        _SetStatePrecisePath(wandererState, beginIndex, endIndex);
        wandererState.m_wayPointIndex = 1;
        wandererState.m_precisePathIndex = 1;
        _SendTeamToPrecisePathPoint(team, wandererState);
        m_wandererStates[teamId] = wandererState;

        theProcessManager->PostMessageA(
            GE_SUBSCRIBE, teamId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_TARGET_REACHED)), m3d::AIParam(), 1);
        theProcessManager->PostMessageA(
            GE_SUBSCRIBE, teamId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);

        M3D_LOG_INFO(
            CStr("Spawning vagabond: ") + GetDebugDescription() + CStr(" spawns ") + team->GetDebugDescription());
    }

    int WanderersManager::_GetWayPointIndex(CStr const& wayPointName) const
    {
        // RVA 0x868370
        auto it = m_wayPointIndexMap.find(wayPointName);
        if (it == m_wayPointIndexMap.end())
        {
            return -1;
        }
        return it->second;
    }

    void
        WanderersManager::_SetStatePrecisePath(ai::WanderersManager::WandererState& state, int beginIndex, int endIndex)
    {
        // RVA 0x8696E0 - uses the hand-placed path between the two waypoints (walked backwards if
        // it was only placed the other way round) and makes sure it starts and ends at the
        // waypoints themselves.
        CVector2 const beginPos = m_wayPoints[beginIndex].m_position;
        CVector2 const endPos = m_wayPoints[endIndex].m_position;

        PrecisePath path = _GetPrecisePath(beginIndex, endIndex);
        if (path.empty())
        {
            path = _GetPrecisePath(endIndex, beginIndex);
            std::reverse(path.begin(), path.end());
        }

        if (path.empty() ||
            std::sqrt(
                (beginPos.y - path.front().y) * (beginPos.y - path.front().y) +
                (beginPos.x - path.front().x) * (beginPos.x - path.front().x)) > 1.0f)
        {
            path.insert(path.begin(), beginPos);
        }
        if (std::sqrt(
                (endPos.y - path.back().y) * (endPos.y - path.back().y) +
                (endPos.x - path.back().x) * (endPos.x - path.back().x)) > 1.0f)
        {
            path.push_back(endPos);
        }

        state.m_precisePathBegin = beginIndex;
        state.m_precisePathEnd = endIndex;
        state.m_precisePath = path;
    }

    retruxx::vector<CVector2, retruxx::allocator<CVector2>> WanderersManager::_GetPrecisePath(
        int beginIndex,
        int endIndex) const
    {
        // RVA 0x868D50
        auto it = m_precisePaths.find(WayPointsPair(beginIndex, endIndex));
        if (it == m_precisePaths.end())
        {
            return PrecisePath();
        }
        return it->second;
    }

    void WanderersManager::_SendTeamToPrecisePathPoint(ai::Team* team, ai::WanderersManager::WandererState const& state)
        const
    {
        // RVA 0x8673C0
        CVector2 const& point = state.m_precisePath[state.m_precisePathIndex];
        team->SetDestination(CVector(point.x, 0.0f, point.y));
    }

    unsigned int WanderersManager::_GetWanderersCount() const
    {
        // RVA 0x8659B0
        return static_cast<unsigned int>(m_wandererStates.size());
    }

    void WanderersManager::_OnTargetReached(ai::Event const& evn)
    {
        // RVA 0x86EC80 - a team reports each point of its precise path; a location id in the
        // event (sent by _OnObjectEntersLocation) means it has reached the path's end waypoint.
        int teamId = evn.m_senderObjId;
        int const locationId = evn.m_param1.GetType() == m3d::AIPARAM_UNDEFINE ? -1 : evn.m_param1.GetAsID();
        if (m_wandererStates.find(teamId) == m_wandererStates.end())
        {
            M3D_LOG_ERR(CStr("Error: unknown team reached target in ") + GetDebugDescription());
            return;
        }

        WandererState& state = m_wandererStates[teamId];
        Obj* team = theObjects->GetEntityByObjId(teamId);

        ++state.m_precisePathIndex;
        if (state.m_precisePathIndex < static_cast<int>(state.m_precisePath.size()) && locationId == -1)
        {
            _SendTeamToPrecisePathPoint(static_cast<Team*>(team), state);
            return;
        }

        // NOTE: the team is not checked for null before these type checks.
        if (team->IsKindOf(&CaravanTeam::m_classCaravanTeam))
        {
            if (locationId == -1)
            {
                return;
            }

            // A caravan carries on to the next waypoint of its route, and waits for the player
            // at the last one.
            int const wayPointIndex = ++state.m_wayPointIndex;
            CaravanInfo const& caravanInfo = m_caravanInfos[state.m_caravanInfoIndex];
            if (wayPointIndex >= static_cast<int>(caravanInfo.m_wayPointIndices.size()))
            {
                static_cast<CaravanTeam*>(team)->SetWaitingPlayerToMoveout();
                return;
            }

            _SetStatePrecisePath(state, state.m_precisePathEnd, caravanInfo.m_wayPointIndices[wayPointIndex]);
            state.m_precisePathIndex = 1;
            _SendTeamToPrecisePathPoint(static_cast<Team*>(team), state);
            return;
        }

        if (team->IsKindOf(&VagabondTeam::m_classVagabondTeam) && locationId != -1)
        {
            // A vagabond leaves the map at a transition waypoint; anywhere else it rests for a
            // while and then heads for another random waypoint.
            int const precisePathEnd = state.m_precisePathEnd;
            if (m_wayPoints[precisePathEnd].m_transition)
            {
                team->Remove();
                return;
            }

            // NOTE: with a single waypoint this loop never ends.
            int next = RandomUniformInt(0, static_cast<int>(m_wayPoints.size()) - 1);
            while (next == precisePathEnd)
            {
                next = rand() % m_wayPoints.size();
            }
            _SetStatePrecisePath(state, precisePathEnd, next);
            state.m_timeout = RandomUniformFloat(30.0f, 120.0f);
            m_waitingVagabonds.push_back(teamId);
        }
    }

    void WanderersManager::_OnObjectDie(ai::Event const& evn)
    {
        // RVA 0x86FE40
        int const teamId = evn.m_senderObjId;
        m_wandererStates.erase(teamId);
        auto it = std::find(m_waitingVagabonds.begin(), m_waitingVagabonds.end(), teamId);
        if (it != m_waitingVagabonds.end())
        {
            m_waitingVagabonds.erase(it);
        }
    }

    void WanderersManager::_OnObjectEntersLocation(ai::Event const& evn)
    {
        // RVA 0x86B3D0 - when a wanderer's vehicle enters the location at the end of its current
        // path, the team is told it has reached its target.
        int const locationId = evn.m_senderObjId;
        Obj* obj = theObjects->GetEntityByObjId(evn.m_param1.GetAsID());
        // NOTE: the object is not checked for null before the type check.
        if (!obj->IsKindOf(&Vehicle::m_classVehicle))
        {
            return;
        }

        int const teamId = obj->GetParentId();
        auto it = m_wandererStates.find(teamId);
        if (it != m_wandererStates.end() && m_wayPoints[it->second.m_precisePathEnd].m_locationId == locationId)
        {
            theProcessManager->PostMessageA(
                GE_TARGET_REACHED, GetId(), teamId, 0.0f, m3d::AIParam(locationId), m3d::AIParam(), 1);
        }
    }

    void WanderersManager::_OnLostGuardsNeedDirection(ai::Event const& evn)
    {
        // RVA 0x86EFA0 - NOTE: only a team that has run off the end of its path is sent anywhere
        // (back to the path's last point); a team still on its path gets no new direction.
        int const teamId = evn.m_senderObjId;
        if (m_wandererStates.find(teamId) == m_wandererStates.end())
        {
            M3D_LOG_ERR(CStr("Error: unknown guard needs direction in ") + GetDebugDescription());
            return;
        }

        WandererState& state = m_wandererStates[teamId];
        auto* team = static_cast<Team*>(theObjects->GetEntityByObjId(teamId));
        if (state.m_precisePathIndex >= static_cast<int>(state.m_precisePath.size()))
        {
            state.m_precisePathIndex = static_cast<int>(state.m_precisePath.size()) - 1;
            _SendTeamToPrecisePathPoint(team, state);
        }
    }
}  // namespace ai

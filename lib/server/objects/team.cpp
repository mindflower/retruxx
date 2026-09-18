#include "team.h"
#include <core/kernel.h>
#include <core/log.h>
#include <stdexcept>
#include <core/aiparam.h>
#include <server/ai/aimessage.h>
#include <server/ai/aipassagestate.h>

#include "base/prototypemanager.h"
#include "core/ini.h"
#include "server/ai/aimanager.h"
#include "base/objcontainer.h"
#include "server/roles/combatmastermind.h"
#include <server/processmanager.h>
#include "player.h"
#include "server/roles/teamrolemanager.h"
#include "server/roles/teamtacticmanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Team, SetDestination)
{
    auto* team = (ai::Team*)context->asObject(0, "Team");
    auto& dest = context->asVector(1);
    team->SetDestination(dest);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, AdjustBehaviour)
{
    // RVA 0x659CF0
    auto* team = (ai::Team*)context->asObject(0, "Team");
    team->_AdjustBehaviour();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, HoldFire)
{
    // RVA 0x657AF0
    auto* team = (ai::Team*)context->asObject(0, "Team");
    team->HoldFire(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, GetNumVehicles)
{
    // RVA 0x656110
    auto* team = (ai::Team*)context->asObject(0, "Team");
    context->pushInt(team->GetNumVehicles());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, GetVehicle)
{
    // RVA 0x6573C0
    auto* team = (ai::Team*)context->asObject(0, "Team");
    context->pushObject(team->GetVehicle(context->asInt(1)));
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Team)
    RT_CLASS_EXPORT(Team, m3d::METHOD, SetDestination, "", "", "")
    RT_CLASS_EXPORT(Team, m3d::METHOD, AdjustBehaviour, "", "", "")
    RT_CLASS_EXPORT(Team, m3d::METHOD, HoldFire, "", "", "")
    RT_CLASS_EXPORT(Team, m3d::METHOD, GetNumVehicles, "", "", "")
    RT_CLASS_EXPORT(Team, m3d::METHOD, GetVehicle, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Team);

    extern AIManager* theAIManager;

    namespace
    {
        char const* TEAM_DEFAULT_FORMATION_PROTOTYPE = "caravanFormation";
        float const TEAM_LINEAR_VELOCITY = 100.f;
        float const TIMEOUT_FOR_ADJUSTING_VEHICLES = 0.30000001f;
    }  // namespace

    void TeamPrototypeInfo::PostLoad()
    {
        m_formationPrototypeId = thePrototypeManager->GetPrototypeId(m_formationPrototypeName);
    }

    ai::Obj* TeamPrototypeInfo::CreateTargetObject() const
    {
        return new Team(*this);
    }

    bool TeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            CStr decisionMatrixName;
            m3d::SafeStrAttrib(decisionMatrixName, xmlNode, "DecisionMatrix");

            ai::theAIManager->LoadMatrix(decisionMatrixName.c_str());
            m_decisionMatrixNum = theAIManager->GetMatrixNum(decisionMatrixName);

            m3d::SafeBoolAttrib(this->m_bRemoveWhenChildrenDead, xmlNode, "RemoveWhenChildrenDead");

            ref_ptr node = xmlFile->CreateNode();
            xmlNode->GetFirstChild(node, "Formation");

            if (!node->IsEmpty())
            {
                m3d::SafeStrAttrib(m_formationPrototypeName, node, "Prototype");

                ref_ptr protoNode = node->CreateAttribute();
                node->GetFirstAttribute(protoNode);

                if (!protoNode->IsEmpty())
                {
                    m_overridesDistBetweenVehicles = 1;
                    m3d::SafeFloatAttrib(m_formationDistBetweenVehicles, node, "DistBetweenVehicles");
                }
            }
        }
        return result;
    }

    float TeamPrototypeInfo::GetFormationDistBetweenVehicles() const
    {
        return this->m_formationDistBetweenVehicles;
    }

    TeamPrototypeInfo::TeamPrototypeInfo()
    {
        this->m_decisionMatrixNum = -1;
        this->m_bRemoveWhenChildrenDead = 1;
        this->m_formationPrototypeName = TEAM_DEFAULT_FORMATION_PROTOTYPE;
        this->m_overridesDistBetweenVehicles = 0;
        this->m_bIsUpdating = 0;
        this->m_formationDistBetweenVehicles = 30.0;
    }

    bool TeamPrototypeInfo::GetOverridesDistBetweenVehicles() const
    {
        return m_overridesDistBetweenVehicles;
    }

    retruxx::map<int, CVector> const& Team::GetSteeringForceMap() const
    {
        return m_steeringForceMap;
    }

    void Team::SetDestination(CVector const& destination)
    {
        m_AI.PutCommand(1, destination, {}, {});
    }

    CVector Team::_GetAggregatedTargetsPos() const
    {
        // RVA 0x658800
        retruxx::vector<CVector> targetsPositions;
        _GetTargetsPositions(targetsPositions);

        CVector res = ZeroVector;
        for (auto const& pos : targetsPositions)
        {
            res += pos;
        }
        if (!targetsPositions.empty())
        {
            res *= 1.0f / targetsPositions.size();
        }
        return res;
    }

    void Team::_GetTargetsPositions(retruxx::vector<CVector>& positions) const
    {
        // RVA 0x658780 - without a tactic there are no assigned targets, so the list comes back
        // empty rather than stale.
        if (theObjects->GetEntityByObjId(m_TeamTacticId))
        {
            TeamRoleManager::GetTargetsPositions(this, positions);
        }
        else
        {
            positions.clear();
        }
    }

    CVector Team::_GetAggregatedPos() const
    {
        // RVA 0x656F50 - the team's position is the centre of its vehicles.
        CVector res = ZeroVector;
        for (auto const* vehicle : m_vehicles)
        {
            res += vehicle->GetPosition();
        }
        if (!m_vehicles.empty())
        {
            res *= 1.0f / m_vehicles.size();
        }
        return res;
    }

    bool Team::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        if (propertyId == 14)
        {
            m_bRemoveWhenChildrenDead = newValue.GetAsID() != 0;
            return 1;
        }
        if (propertyId == 15)
        {
            m_TeamTacticName = newValue.GetAsStr();
            return 1;
        }
        if (propertyId != 16)
            return ai::Obj::SetPropertyById(propertyId, newValue);
        m_TeamTacticShouldBeAssigned = newValue.GetAsID() != 0;
        return 1;
    }

    void Team::AttackNow(int id)
    {
        _AdjustRoles(id);
        m3d::AIParam param1(id);
        m_AI.InsCommand(2, param1, {}, {});
    }

    m3d::AIParam Team::TeamAIOnMoveFinished(Obj*)
    {
        // RVA 0x659C90 - the move has already been wound up by whatever finished it.
        return m3d::AIParam(0);
    }

    m3d::AIParam Team::TeamAIOnStartSearch(Obj* pObj)
    {
        // RVA 0x659770 - starts an asynchronous path search from the formation's position to the
        // destination the AI was given.
        auto* team = RT_DYNCAST(pObj, Team);
        if (team->m_vehicles.empty())
        {
            return m3d::AIParam(2);
        }

        CVector const destination = Obj::AIGetState1Param1(pObj).GetAsVector();

        // NOTE: the formation is re-seated on the middle vehicle of the team, not on the first.
        team->m_formation->SetPosition(
            team->m_vehicles[(team->m_vehicles.size() - 1) / 2]->GetPosition());
        CVector const startPos = team->m_formation->GetPosition();

        if (team->m_formation)
        {
            team->m_formation->SetPath(nullptr, true);
        }
        delete team->m_pPath;
        team->m_pPath = new Path;
        team->m_pPath->StartSearch(Map::theGlobalMap, CVector2(startPos.x, startPos.z),
            CVector2(destination.x, destination.z), 10.0f, 0xFA);
        return m3d::AIParam(1);
    }

    m3d::AIParam Team::TeamAIOnAttack(Obj*)
    {
        return m3d::AIParam(0);
    }

    void Team::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6580C0 - a team saved mid-mission carries its AI state, its path and its
        // formation; a team read straight from a map has only the formation, which is then
        // created as a fresh child object.
        Obj::LoadFromXML(xmlFile, xmlNode);

        ref_ptr aiNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(aiNode, "AI");
        if (!aiNode->IsEmpty())
        {
            m_AI.LoadAIFromXML(xmlFile, aiNode);
        }

        ref_ptr pathNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(pathNode, "Path");
        if (!pathNode->IsEmpty())
        {
            delete m_pPath;
            m_pPath = new Path;
            m_pPath->LoadFromXML(xmlFile, pathNode, Map::theGlobalMap);
        }

        ref_ptr formationNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(formationNode, "Formation");
        if (formationNode->IsEmpty())
        {
            return;
        }
        if (m_formation)
        {
            m_formation->LoadFromXML(xmlFile, formationNode);
            return;
        }

        retruxx::vector<m3d::Class*> allowedClasses;
        int const formationId =
            gDynamicScene->ReadNewObjectFromXml(xmlFile, formationNode, allowedClasses);
        m_formation = static_cast<Formation*>(theObjects->GetEntityByObjId(formationId));
        m_formation->LinkToParent(GetId(), HIERARCHY_COMPONENT);
    }

    eGObjPropertySaveStatus Team::GetPropertySaveStatus(int id) const
    {
        // RVA 0x654A20
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return Obj::GetPropertySaveStatus(id);
    }

    Team::Team(TeamPrototypeInfo const& prototypeInfo) : ai::Obj(prototypeInfo)
    {
        m_bRemoveWhenChildrenDead = prototypeInfo.m_bRemoveWhenChildrenDead;
        m_bUseStandardUpdatingBehavior = 1;
        m_AI.SetDecisionMatrix(prototypeInfo.m_decisionMatrixNum);
        m_formation = 0;
        m_combatMastermind = 0;
        m_pPath = 0;
        m_bFrozen = 0;
        m_bMustMoveToTarget = 0;
        m_maxTeamSpeed = 13.888889;
        m_needAdjustBehaviour = 1;
        m_TeamTacticId = -1;
        m_TeamTacticShouldBeAssigned = true;
    }

    bool Team::RemoveChild(Obj* pChild)
    {
        Obj::RemoveChild(pChild);
        if (!pChild || !IS_KIND_OF(pChild, Vehicle))
        {
            return false;
        }

        auto* vehicle = RT_DYNCAST(pChild, Vehicle);
        auto it = std::find(m_vehicles.begin(), m_vehicles.end(), vehicle);
        if (it == m_vehicles.end())
        {
            return false;
        }

        m_needAdjustBehaviour = true;
        vehicle->SetParentInvalid();
        vehicle->SetIndexInTeam(-1);
        if (m_formation)
        {
            m_formation->RemoveVehicle(vehicle);
        }

        m_vehicles.erase(it);

        for (int i = 0; i < m_vehicles.size(); ++i)
        {
            m_vehicles[i]->SetIndexInTeam(i);
        }

        if (m_formation)
        {
            auto velocity = _GetTeamVelocity();
            m_formation->SetLinearVelocity(velocity);
        }

        if (m_vehicles.empty() && !m_bRemoveWhenChildrenDead && m_bUseStandardUpdatingBehavior)
        {
            theObjects->AddObjToNotUpdate(this);
            theObjects->AddObjToNotUpdate(m_formation);
        }

        return true;
    }

    bool Team::bIsEqualToPrototype() const
    {
        // RVA 0x656670 - a team that has picked up vehicles is no longer its bare prototype.
        return Obj::bIsEqualToPrototype() && m_vehicles.empty();
    }

    m3d::AIParam Team::TeamAIOnIdle(Obj*)
    {
        return m3d::AIParam(0);
    }

    m3d::AIParam Team::TeamAIOnStartAttack(Obj* pObj)
    {
        auto* team = RT_DYNCAST(pObj, Team);
        if (team->m_formation)
        {
            team->m_formation->SetPath(nullptr, true);
        }
        delete team->m_pPath;
        team->m_pPath = nullptr;

        team->_AdjustBehaviour();
        return m3d::AIParam(0);
    }

    m3d::Class* Team::GetClass() const
    {
        return RT_CLASS_LOCAL(Team);
    }

    retruxx::vector<Vehicle*> const& Team::GetVehicles() const
    {
        return m_vehicles;
    }

    void Team::SetTeamFrozen(bool bFrozen)
    {
        // RVA 0x657460 - a frozen team is hidden as well as stopped.
        if (m_bFrozen == bFrozen)
        {
            return;
        }
        for (auto* vehicle : m_vehicles)
        {
            vehicle->ShowVehicle(!bFrozen);
        }
        m_bFrozen = bFrozen;
    }

    Vehicle* Team::GetVehicle(unsigned index) const
    {
        if (index < m_vehicles.size())
        {
            return m_vehicles[index];
        }
        return nullptr;
    }

    void Team::Registration()
    {
        m_propertiesMap["RemoveWhenChildrenDead"] = 14;
        m_propertiesMap["TeamTacticPrototype"] = 15;
        m_propertiesMap["TeamTacticShouldBeAssigned"] = 16;
        theAIManager->RegisterFunc("TeamAIGetCurPos", &Team::TeamAIGetCurPos);
        theAIManager->RegisterFunc("TeamAIGetCurAngle", &Team::TeamAIGetCurAngle);
        theAIManager->RegisterFunc("TeamAIOnAttack", &Team::TeamAIOnAttack);
        theAIManager->RegisterFunc("TeamAIOnStartAttack", &Team::TeamAIOnStartAttack);
        theAIManager->RegisterFunc("TeamAIOnAttackOrder", &Team::TeamAIOnAttackOrder);
        theAIManager->RegisterFunc("TeamAIOnDefend", &Team::TeamAIOnDefend);
        theAIManager->RegisterFunc("TeamAIOnEnemyDestroyed", &Team::TeamAIOnEnemyDestroyed);
        theAIManager->RegisterFunc("TeamAIOnIdle", &Team::TeamAIOnIdle);
        theAIManager->RegisterFunc("TeamAIOnMove", &Team::TeamAIOnMove);
        theAIManager->RegisterFunc("TeamAIOnStartSearch", &Team::TeamAIOnStartSearch);
        theAIManager->RegisterFunc("TeamAIOnPathFind", &Team::TeamAIOnPathFind);
        theAIManager->RegisterFunc("TeamAIOnMoveAlongPath", &Team::TeamAIOnMoveAlongPath);
        theAIManager->RegisterFunc("TeamAIOnStartDefend", &Team::TeamAIOnStartDefend);
        theAIManager->RegisterFunc("TeamAIOnTargetReached", &Team::TeamAIOnTargetReached);
        theAIManager->RegisterFunc("TeamAIOnTargetUnreachable", &Team::TeamAIOnTargetUnreachable);
        theAIManager->RegisterFunc("TeamAIOnMoveFinished", &Team::TeamAIOnMoveFinished);
    }

    void Team::SetTeamTactic(TeamTactic* tactic)
    {
        if (auto curTactic = theObjects->GetEntityByObjId(m_TeamTacticId))
        {
            curTactic->Remove();
        }
        if (tactic)
        {
            m_TeamTacticId = tactic->GetId();
        }
        else
        {
            TeamRoleManager::ClearRoles(this);
            m_TeamTacticId = -1;
        }
    }

    void Team::Update(float elapsedTime, unsigned workTime)
    {
        if (!m_bFrozen)
        {
            ai::Obj::Update(elapsedTime, workTime);
            if (elapsedTime >= 0.001)
            {
                _TeamUpdate(elapsedTime, workTime);
            }
        }
    }

    void Team::GetPropertiesNames(retruxx::set<CStr>& Props) const
    {
        // RVA 0x654A60
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        Obj::GetPropertiesNames(Props);
    }

    m3d::AIParam Team::TeamAIOnEnemyDestroyed(Obj*)
    {
        // RVA 0x659740 - nothing to do; the team falls back to its idle behaviour on its own.
        return m3d::AIParam(0);
    }

    m3d::AIParam Team::TeamAIGetCurAngle(Obj* pObj)
    {
        // RVA 0x659240 - the team faces wherever its lead vehicle faces.
        auto* team = RT_DYNCAST(pObj, Team);
        if (team->m_vehicles.empty())
        {
            return m3d::AIParam(0.0f);
        }
        CVector const direction = team->m_vehicles.front()->GetDirection();
        return m3d::AIParam(atan2f(direction.z, direction.x));
    }

    int Team::OnEvent(Event const& evn)
    {
        int result = Obj::OnEvent(evn);
        switch (evn.m_eventId)
        {
        case GE_OBJECT_DIE:
            _OnObjectDie(evn);
            result = 1;
            break;

        case GE_UNDER_ATTACK:
            _OnUnderAttack(evn);
            result = 1;
            break;

        case GE_NOTICE_ENEMY:
            _DoNoticeEnemy(evn.m_param1.GetAsID());
            result = 1;
            break;

        case GE_PLAYER_VEHICLE_CHANGED:
            m_needAdjustBehaviour = true;
            result = 1;
            break;

        default:
            return result;
        }
        return result;
    }

    void Team::HoldFire(int msc)
    {
        // RVA 0x657430
        for (auto* vehicle : m_vehicles)
        {
            vehicle->HoldFire(msc);
        }
    }

    m3d::AIParam Team::TeamAIOnTargetReached(Obj* pObj)
    {
        // RVA 0x659B00
        auto* team = RT_DYNCAST(pObj, Team);
        if (team->m_formation)
        {
            team->m_formation->SetPath(nullptr, true);
        }
        delete team->m_pPath;
        team->m_pPath = nullptr;
        pObj->CauseEvent(GE_TARGET_REACHED, 0.0f, {}, {});
        return m3d::AIParam(1);
    }

    void Team::GetPropertiesIDs(retruxx::set<int>& Props) const
    {
        // RVA 0x654AC0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        Obj::GetPropertiesIDs(Props);
    }

    void Team::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x656460 - the AI state and the path are only written when the team is actually
        // following one; the formation always is.
        Obj::SaveToXML(xmlFile, xmlNode);

        if (m_pPath)
        {
            ref_ptr aiNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "AI");
            xmlNode->AddChild(aiNode);
            m_AI.SaveAIToXML(xmlFile, aiNode);

            ref_ptr pathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Path");
            xmlNode->AddChild(pathNode);
            m_pPath->SaveToXML(xmlFile, pathNode);
        }

        ref_ptr formationNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Formation");
        xmlNode->AddChild(formationNode);
        m_formation->SaveToXML(xmlFile, formationNode);
    }

    unsigned Team::GetNumVehicles() const
    {
        return m_vehicles.size();
    }

    bool Team::GetTeamFrozen() const
    {
        // RVA 0x655BC0
        return m_bFrozen;
    }

    void Team::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6565E0
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TeamTacticId", CStr(m_TeamTacticId).c_str());
        xmlNode->SetAttribute("Frozen", CStr(m_bFrozen).c_str());
    }

    TeamTactic* Team::GetTeamTactic() const
    {
        return RT_DYNCAST(theObjects->GetEntityByObjId(m_TeamTacticId), TeamTactic);
    }

    void Team::_AdjustBehaviour()
    {
        auto const state = m_AI.GetCurState2Name();
        if (state == "Attack")
        {
            auto id = AIGetState2Param1(this).GetAsID();
            _AdjustRoles(id);
        }
        else
        {
            SetTeamTactic(nullptr);
        }
    }

    float Team::GetDistToPhysicObj(PhysicObj const* obj) const
    {
        // RVA 0x656D50 - the distance to the nearest of the team's vehicles.
        if (!obj)
        {
            M3D_LOG_ERR(CStr("Error: NULL object passed to GetDistToPhysicObj of ") +
                GetDebugDescription());
            return 1.0e10f;
        }

        CVector const objPos = obj->GetPosition();
        float res = 1.0e10f;
        for (auto const* vehicle : m_vehicles)
        {
            float const dist = (vehicle->GetPosition() - objPos).length();
            if (dist < res)
            {
                res = dist;
            }
        }
        return res;
    }

    TeamPrototypeInfo const* Team::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), TeamPrototypeInfo const);
    }

    CStr Team::GetPropertyName(int id) const
    {
        // RVA 0x654B20
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return Obj::GetPropertyName(id);
    }

    m3d::AIParam Team::TeamAIOnMoveAlongPath(Obj* pObj)
    {
        // RVA 0x6599C0 - keeps every vehicle steering at the slot the formation assigns it, and
        // reports done once nothing is moving any more.
        auto* team = RT_DYNCAST(pObj, Team);

        bool anyMoving = false;
        for (auto* vehicle : team->m_vehicles)
        {
            if (vehicle->GetMoveStatus())
            {
                anyMoving = true;
                break;
            }
        }
        if (!anyMoving && !team->m_formation->bIsMoving())
        {
            return m3d::AIParam(1);
        }

        for (unsigned i = 0; i < team->m_vehicles.size(); ++i)
        {
            CVector destination;
            CVector direction;
            team->m_formation->GetPositionAndDirectionForVehicle(
                i, team->m_vehicles.size(), destination, direction);
            // NOTE: the height is flattened to zero on both sides, so the vehicles steer in the
            // horizontal plane only.
            destination.y = 0.0f;
            team->m_vehicles[i]->SetExternalDestination(destination);
        }
        return m3d::AIParam(0);
    }

    m3d::AIParam Team::TeamAIOnDefend(Obj* pObj)
    {
        auto* team = RT_DYNCAST(pObj, Team);
        if (theObjects->GetEntityByObjId(team->m_TeamTacticId))
        {
            team->m_needAdjustBehaviour = true;
        }
        return m3d::AIParam(0);
    }

    m3d::AIParam Team::TeamAIGetCurPos(Obj* pObj)
    {
        auto* team = RT_DYNCAST(pObj, Team);
        if (!team->m_vehicles.empty())
        {
            Vehicle const* vehicle = team->m_vehicles.front();
            CVector pos = vehicle->GetPosition();
            return m3d::AIParam(pos);
        }
        return m3d::AIParam(ZeroVector);
    }

    m3d::AIParam Team::TeamAIOnTargetUnreachable(Obj* pObj)
    {
        // RVA 0x659BE0
        pObj->CauseEvent(GE_TARGET_UNREACHED, 0.0f, {}, {});
        return m3d::AIParam(1);
    }

    void Team::SetRemoveWhenChilrenDead(bool bRemoveWhenChildrenDead)
    {
        // RVA 0x655BB0
        m_bRemoveWhenChildrenDead = bRemoveWhenChildrenDead;
    }

    m3d::AIParam Team::TeamAIOnPathFind(Obj* pObj)
    {
        // RVA 0x659900 - the search is spread over frames, and the whole game shares one budget
        // of path-finding quanta per frame.
        auto* team = RT_DYNCAST(pObj, Team);
        Path* const path = team->m_pPath;
        if (path)
        {
            if (path->GetSearchStatus())
            {
                if (Path::QuantAmount <= Path::QuantMax)
                {
                    ++Path::QuantAmount;
                    path->ProceedSearchDijkstraStraightment(2u);
                }
                return m3d::AIParam(0);
            }
            if (path->m_SearchResult == PATH_OK)
            {
                team->m_formation->SetPath(path, true);
                return m3d::AIParam(1);
            }
        }
        team->_DoPosUnreachable();
        return m3d::AIParam(3);
    }

    void Team::AddChild(Obj* pObj)
    {
        Obj::AddChild(pObj);
        if (pObj && IS_KIND_OF(pObj, Vehicle))
        {
            m_needAdjustBehaviour = true;
            auto* vehicle = RT_DYNCAST(pObj, Vehicle);
            vehicle->SetCruisingSpeed(m_maxTeamSpeed);
            vehicle->SetIndexInTeam(m_vehicles.size());
            m_vehicles.push_back(vehicle);
            if (m_formation)
            {
                _AddVehicleToFormation(vehicle);
                theObjects->AddObjToUpdate(m_formation);
            }
            theObjects->AddObjToUpdate(this);
            pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
        }
    }

    void Team::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x656330
        Obj::LoadRuntimeValues(xmlFile, xmlNode);

        ref_ptr pathNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(pathNode, "Path");
        if (!pathNode->IsEmpty())
        {
            delete m_pPath;
            m_pPath = new Path;
            m_pPath->LoadFromXML(xmlFile, pathNode, Map::theGlobalMap);
        }

        m3d::SafeIntAttrib(m_TeamTacticId, xmlNode, "TeamTacticId");
        m3d::SafeBoolAttrib(m_bFrozen, xmlNode, "Frozen");
    }

    bool Team::CanChildBeAdded(m3d::Class* pClass) const
    {
        return ai::Obj::CanChildBeAdded(pClass) || pClass->IsKindOf(&ai::Vehicle::m_classVehicle);
    }

    int Team::GetPropertyId(char const* propName) const
    {
        auto it = Team::m_propertiesMap.find(propName);
        if (it != Team::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::Obj::GetPropertyId(propName);
    }

    void Team::CreateChildren()
    {
        ai::Obj::CreateChildren();
        ai::Team::_CreateFormation();
    }

    Formation* Team::GetFormation() const
    {
        return m_formation;
    }

    m3d::Class* Team::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::AIParam Team::TeamAIOnAttackOrder(Obj* pObj)
    {
        // RVA 0x659590 - holds the team in its attack state while any vehicle is still engaged,
        // and keeps them from shooting through each other while they are.
        auto* team = RT_DYNCAST(pObj, Team);

        bool allIdle = true;
        bool anyIdle = false;
        for (auto const* vehicle : team->m_vehicles)
        {
            bool const idle = vehicle->GetAttackStatus() == Vehicle::ATTACK_IDLE;
            allIdle &= idle;
            anyIdle |= idle;
        }
        if (allIdle)
        {
            return m3d::AIParam(1);
        }

        if (anyIdle && !theObjects->GetEntityByObjId(team->m_TeamTacticId))
        {
            team->m_needAdjustBehaviour = true;
        }

        if (!theObjects->GetEntityByObjId(team->m_TeamTacticId))
        {
            team->m_needAdjustBehaviour = true;
        }
        else if (team->_IsTooFarFromTargets())
        {
            // Closing the distance is only this team's business if nothing else is driving it.
            bool driven = false;
            for (auto const* vehicle : team->m_vehicles)
            {
                if (vehicle->GetNpcMotionControllerId() != -1 ||
                    vehicle->bIsMovingAlongExternalPath())
                {
                    driven = true;
                    break;
                }
            }
            if (!driven)
            {
                team->m_bMustMoveToTarget = true;
            }
            return m3d::AIParam(0);
        }

        team->m_combatMastermind->GenerateLineOfFireAvoidanceSteeringForces(team->m_steeringForceMap);
        return m3d::AIParam(0);
    }

    void Team::Remove()
    {
        Obj::Remove();
        for (auto& vehicle : m_vehicles)
        {
            vehicle->Remove();
        }

        if (m_formation)
        {
            m_formation->Remove();
        }

        auto tactic = theObjects->GetEntityByObjId(m_TeamTacticId);
        if (tactic)
        {
            tactic->Remove();
        }

        if (m_TeamTacticId != -1)
        {
            TeamRoleManager::ClearRoles(this);
            m_TeamTacticId = -1;
        }
    }

    m3d::AIParam Team::TeamAIOnStartDefend(Obj* pObj)
    {
        pObj->CauseEvent(GE_ENEMY_DESTROYED, 0.0, {}, {});
        return m3d::AIParam(1);
    }

    void Team::RenderDebugInfo() const
    {
        // RVA 0x6566B0 - NOTE: the formation is not null-checked.
        m_formation->RenderDebugInfo();
    }

    void Team::Stop()
    {
        // RVA 0x658A30 - every vehicle is told to stand where it is, the path is thrown away and
        // the AI is dropped back into its idle state.
        for (auto* vehicle : m_vehicles)
        {
            vehicle->SetExternalDestination(vehicle->GetPosition());
        }
        if (m_formation)
        {
            m_formation->SetPath(nullptr, true);
        }
        delete m_pPath;
        m_pPath = nullptr;
        m_AI.PutCommand(3, {}, {}, {});
    }

    m3d::AIParam Team::TeamAIOnMove(Obj* pObj)
    {
        auto* team = RT_DYNCAST(pObj, Team);
        if (team->GetTeamTactic())
        {
            team->SetTeamTactic(nullptr);
            for (auto* vehicle : team->GetVehicles())
            {
                vehicle->SetAttackStatus(Vehicle::VehicleAttackStatus::ATTACK_IDLE);
            }
        }
        return {0};
    }

    void Team::SetPassedToAnotherMapStatus()
    {
        // RVA 0x658C10 - NOTE: the formation is not null-checked.
        Obj::SetPassedToAnotherMapStatus();
        m_formation->SetPassedToAnotherMapStatus();
        for (auto* vehicle : m_vehicles)
        {
            vehicle->SetPassedToAnotherMapStatus();
        }
        if (Obj* const teamTactic = theObjects->GetEntityByObjId(m_TeamTacticId))
        {
            teamTactic->SetPassedToAnotherMapStatus();
        }
        m_AI.PutCommand(3, {}, {}, {});
        m_AI.AIUpdate(this);
        m_needAdjustBehaviour = true;
    }

    void Team::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x658900 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    void Team::_InternalPostLoad()
    {
        m_combatMastermind = new CombatMastermind(GetId());
        m_needAdjustBehaviour = true;
        theProcessManager->PostMessageA(2, thePlayer->GetId(), GetId(), 0.0, {65}, {}, 1);
        if (!m_formation)
        {
            _CreateFormation();
        }
        if (!m_vehicles.empty())
        {
            theObjects->AddObjToUpdate(m_formation);
        }
        if (m_pPath)
        {
            m_formation->SetPath(m_pPath, false);
        }
        else
        {
            m_AI.AIInit();
        }

        for (auto& vehicle : m_vehicles)
        {
            m_formation->AddVehicle(vehicle);
        }

        m_formation->SetLinearVelocity(_GetTeamVelocity());
    }

    void Team::_DoPosUnreachable()
    {
        // RVA 0x655BD0 - a team that cannot reach its destination simply gives up quietly;
        // subclasses override this to react.
    }

    void Team::_RemoveVehicles()
    {
        // RVA 0x6574B0 - NOTE: Vehicle::Remove calls back into Team::RemoveChild, which erases
        // from m_vehicles while this loop is walking it.
        for (auto* vehicle : m_vehicles)
        {
            vehicle->Remove();
        }
    }

    AI* Team::GetAIPtr()
    {
        return &m_AI;
    }

    void Team::_DoNoticeEnemy(int objId)
    {
        auto const& curStateName = m_AI.GetCurState2Name();
        if (curStateName != "Attack")
        {
            m_AI.InsCommand(2, objId, {}, {});
        }
        m_needAdjustBehaviour = true;
    }

    bool Team::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x6590D0
        switch (propertyId)
        {
            case PROPERTY_REMOVE_WHEN_CHILDREN_DEAD:
                retVal = static_cast<unsigned>(GetPrototypeInfo()->m_bRemoveWhenChildrenDead);
                return true;
            case PROPERTY_TEAMTACTIC_PROTOTYPE: retVal = CStr(""); return true;
            case PROPERTY_TEAMTACTIC_SHOULD_BE_ASSIGNED: retVal = 1u; return true;
            default: return Obj::_GetPropertyDefaultInternal(propertyId, retVal);
        }
    }

    bool Team::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x659040
        switch (propertyId)
        {
            case PROPERTY_REMOVE_WHEN_CHILDREN_DEAD:
                retVal = static_cast<unsigned>(m_bRemoveWhenChildrenDead);
                return true;
            case PROPERTY_TEAMTACTIC_PROTOTYPE: retVal = m_TeamTacticName; return true;
            case PROPERTY_TEAMTACTIC_SHOULD_BE_ASSIGNED:
                retVal = static_cast<unsigned>(m_TeamTacticShouldBeAssigned);
                return true;
            default: return Obj::_GetPropertyInternal(propertyId, retVal);
        }
    }

    void Team::_DoUnderAttack(int attackerId)
    {
        AttackNow(attackerId);
    }

    void Team::_TeamUpdate(float, unsigned)
    {
        if (this->m_needAdjustBehaviour)
        {
            _AdjustBehaviour();
            this->m_needAdjustBehaviour = false;
        }

        if (m_vehicles.empty() && m_bRemoveWhenChildrenDead)
        {
            Remove();
        }

        auto const flags = GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !GetParentRepository() && TimeOutFinished())
        {
            m_AI.AIUpdate(this);
            SetTimeOut(TIMEOUT_FOR_ADJUSTING_VEHICLES);
            if (m_bMustMoveToTarget)
            {
                m_bMustMoveToTarget = false;
                auto pos = _GetAggregatedTargetsPos();
                m_AI.InsCommand(1, pos, {}, {});
            }
        }
    }

    Team::~Team()
    {
        // RVA 0x65A1D0 - the mastermind and the path belong to the team; the formation and
        // the vehicles are objects in their own right and are not deleted here.
        delete m_combatMastermind;
        m_combatMastermind = nullptr;

        if (m_formation)
        {
            m_formation->SetPath(nullptr, true);
        }

        delete m_pPath;
        m_pPath = nullptr;
        m_formation = nullptr;
    }

    void Team::_OnPlayerVehicleChanged(Event const&)
    {
        // RVA 0x655C30 - who the player is driving changes who this team should be fighting.
        m_needAdjustBehaviour = true;
    }

    void Team::_OnObjectDie(Event const& evn)
    {
        // RVA 0x657E00
        Obj* const sender = theObjects->GetEntityByObjId(evn.m_senderObjId);
        if (sender)
        {
            RemoveChild(sender);
        }
        m_needAdjustBehaviour = true;
    }

    void Team::_CreateFormation()
    {
        auto* protoInfo = GetPrototypeInfo();
        auto objId = theObjects->CreateNewObject(protoInfo->m_formationPrototypeId, {}, -1, -1);
        auto* obj = theObjects->GetEntityByObjId(objId);
        m_formation = dynamic_cast<Formation*>(obj);
        m_formation->LinkToParent(GetId(), HIERARCHY_COMPONENT);
        if (protoInfo->GetOverridesDistBetweenVehicles())
        {
            m_formation->SetDistBetweenVehicles(protoInfo->GetFormationDistBetweenVehicles());
        }
        m_formation->SetAngularVelocity(0.5);
        m_formation->SetLinearVelocity(100.0);
    }

    void Team::_OnUnderAttack(Event const& evn)
    {
        auto const id = evn.m_param1.GetAsID();
        auto* obj = ai::theObjects->GetEntityByObjId(id);
        if (bIsEnemyWith(obj))
        {
            _DoUnderAttack(id);
        }
    }

    void Team::_AddVehicleToFormation(Vehicle* pVehicle)
    {
        m_formation->AddVehicle(pVehicle);
        m_formation->SetLinearVelocity(_GetTeamVelocity());
    }

    bool Team::_IsTooFarFromTargets() const
    {
        // RVA 0x658860
        return (_GetAggregatedPos() - _GetAggregatedTargetsPos()).length() > 250.0f;
    }

    float Team::_GetTeamVelocity() const
    {
        float speed = TEAM_LINEAR_VELOCITY;
        for (auto const& vehicle : m_vehicles)
        {
            auto vehicleSpeed = vehicle->GetCruisingSpeed();
            if (vehicleSpeed < speed)
            {
                speed = vehicleSpeed;
            }
        }
        return speed;
    }

    void Team::_TuneFormation()
    {
        // RVA 0x655BE0 - NOTE: these are fixed values, not derived from the team's vehicles, so
        // the linear velocity set here is immediately overwritten wherever _GetTeamVelocity is
        // applied.
        m_formation->SetAngularVelocity(0.5f);
        m_formation->SetLinearVelocity(100.0f);
    }

    m3d::Object* Team::Clone()
    {
        // RVA 0x656990
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void Team::_AdjustRoles(int targetId)
    {
        // RVA 0x657C30
        if (!m_TeamTacticShouldBeAssigned)
        {
            SetTeamTactic(nullptr);
            return;
        }

        Vehicle::VehicleAttackStatus attackStatus = Vehicle::VehicleAttackStatus::ATTACK_IDLE;

        // Resolve target object from ID
        auto* target = theObjects->GetEntityByObjId(targetId);

        // Check if target is valid and an enemy
        if (target && target->bIsEnemyWith(this))
        {
            attackStatus = Vehicle::VehicleAttackStatus::ATTACK_ATTACKING;

            TeamTacticManager manager;

            // Assign team tactic based on target type
            if (m_TeamTacticName.empty())
            {
                // Auto-assign best tactic
                if (IS_KIND_OF(target, Vehicle))
                {
                    auto* vehicle = RT_DYNCAST(target, Vehicle);
                    manager.AssignBestTacticAgainstVehicle(this, vehicle);
                }
                else if (IS_KIND_OF(target, Team))
                {
                    auto* team = RT_DYNCAST(target, Team);
                    manager.AssignBestTacticAgainstTeam(this, team);
                }
                else
                {
                    manager.AssignBestTacticAgainstObj(this, target);
                }
            }
            else
            {
                // Use predefined tactic
                if (IS_KIND_OF(target, Vehicle))
                {
                    auto* vehicle = RT_DYNCAST(target, Vehicle);
                    manager.AssignTacticAgainstVehicle(this, vehicle, m_TeamTacticName);
                }
                else if (IS_KIND_OF(target, Team))
                {
                    auto* team = RT_DYNCAST(target, Team);
                    manager.AssignTacticAgainstTeam(this, team, m_TeamTacticName);
                }
                else
                {
                    manager.AssignTacticAgainstObj(this, target, m_TeamTacticName);
                }
            }
        }

        // Update attack status for all vehicles in the team
        for (auto* vehicle : m_vehicles)
        {
            vehicle->SetAttackStatus(attackStatus);
        }
    }

    void Team::_OnNoticeEnemy(Event const& evn)
    {
        // RVA 0x655C10
        _DoNoticeEnemy(evn.m_param1.GetAsID());
    }

    m3d::Object* Team::CreateObject()
    {
        // RVA 0x656B50
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai

#include "team.h"
#include <stdexcept>
#include <core/aiparam.h>
#include <server/ai/aimessage.h>
#include <server/ai/aipassagestate.h>

#include "core/ini.h"
#include "server/ai/aimanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Team, SetDestination)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, AdjustBehaviour)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, HoldFire)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, GetNumVehicles)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, GetVehicle)
{
    throw retruxx::logic_error("Not implemented");
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
        const char* TEAM_DEFAULT_FORMATION_PROTOTYPE = "caravanFormation";
    }

    void TeamPrototypeInfo::PostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    ai::Obj* TeamPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool TeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        throw std::logic_error("Not implemented");
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        //if (result)
        //{
        //    CStr decisionMatrixName;
        //    m3d::SafeStrAttrib(decisionMatrixName, xmlNode, "DecisionMatrix");
        //
        //    ai::theAIManager->LoadMatrix(decisionMatrixName.c_str());
        //    m_decisionMatrixNum = theAIManager->GetMatrixNum(decisionMatrixName);
        //
        //    m3d::SafeBoolAttrib(this->m_bRemoveWhenChildrenDead, xmlNode, "RemoveWhenChildrenDead");
        //
        //    ref_ptr node = xmlFile->CreateNode();
        //    xmlNode->GetFirstChild(node, "Formation");
        //
        //    if (!node->IsEmpty())
        //    {
        //        m3d::SafeStrAttrib(m_formationPrototypeName, node, "Prototype");
        //
        //        ref_ptr protoNode = xmlFile->CreateNode();
        //        node->GetFirstAttribute((m3d::cmn::XmlAttrib*)&*protoNode);
        //
        //        if (!protoNode->IsEmpty())
        //        {
        //            m_overridesDistBetweenVehicles = 1;
        //            m3d::SafeFloatAttrib(m_formationDistBetweenVehicles, node, "DistBetweenVehicles");
        //        }
        //
        //    }
        //}
        return result;
    }

    float TeamPrototypeInfo::GetFormationDistBetweenVehicles() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::map<int, CVector> const& Team::GetSteeringForceMap() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::SetDestination(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Team::_GetAggregatedTargetsPos() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_GetTargetsPositions(retruxx::vector<CVector>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Team::_GetAggregatedPos() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::AttackNow(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnMoveFinished(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnStartSearch(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnAttack(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Team::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Team::Team(TeamPrototypeInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::RemoveChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::bIsEqualToPrototype() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnIdle(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnStartAttack(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Team::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    retruxx::vector<Vehicle*> const& Team::GetVehicles() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::SetTeamFrozen(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Vehicle* Team::GetVehicle(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
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

    void Team::SetTeamTactic(TeamTactic*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::GetPropertiesNames(retruxx::set<CStr>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnEnemyDestroyed(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIGetCurAngle(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Team::OnEvent(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::HoldFire(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnTargetReached(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::GetPropertiesIDs(retruxx::set<int>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Team::GetNumVehicles() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::GetTeamFrozen() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    TeamTactic* Team::GetTeamTactic() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_AdjustBehaviour()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Team::GetDistToPhysicObj(PhysicObj const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    TeamPrototypeInfo const* Team::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Team::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnMoveAlongPath(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnDefend(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIGetCurPos(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnTargetUnreachable(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::SetRemoveWhenChilrenDead(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnPathFind(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::AddChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::CanChildBeAdded(m3d::Class*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Team::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::CreateChildren()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Formation* Team::GetFormation() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Team::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::AIParam Team::TeamAIOnAttackOrder(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::Remove()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnStartDefend(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::RenderDebugInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::Stop()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnMove(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::SetPassedToAnotherMapStatus()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_DoPosUnreachable()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_RemoveVehicles()
    {
        throw retruxx::logic_error("Not implemented");
    }

    AI* Team::GetAIPtr()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_DoNoticeEnemy(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_DoUnderAttack(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_TeamUpdate(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Team::~Team()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_OnPlayerVehicleChanged(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_OnObjectDie(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_CreateFormation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_OnUnderAttack(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_AddVehicleToFormation(Vehicle*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Team::_IsTooFarFromTargets() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Team::_GetTeamVelocity() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_TuneFormation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Team::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_AdjustRoles(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Team::_OnNoticeEnemy(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Team::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

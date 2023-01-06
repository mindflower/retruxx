#include "team.h"
#include <stdexcept>
#include <core/aiparam.h>
#include <server/ai/aimessage.h>
#include <server/ai/aipassagestate.h>

#include "server/ai/aimanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Team, SetDestination)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, AdjustBehaviour)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, HoldFire)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, GetNumVehicles)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Team, GetVehicle)
{
    throw std::logic_error("Not implemented");
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

    void TeamPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* TeamPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool TeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float TeamPrototypeInfo::GetFormationDistBetweenVehicles() const
    {
        throw std::logic_error("Not implemented");
    }

    TeamPrototypeInfo::TeamPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool TeamPrototypeInfo::GetOverridesDistBetweenVehicles() const
    {
        throw std::logic_error("Not implemented");
    }

    std::map<int, CVector> const& Team::GetSteeringForceMap() const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::SetDestination(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Team::_GetAggregatedTargetsPos() const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_GetTargetsPositions(std::vector<CVector>&) const
    {
        throw std::logic_error("Not implemented");
    }

    CVector Team::_GetAggregatedPos() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::AttackNow(int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnMoveFinished(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnStartSearch(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnAttack(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Team::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    Team::Team(TeamPrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::bIsEqualToPrototype() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnIdle(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnStartAttack(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Team::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<Vehicle*> const& Team::GetVehicles() const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::SetTeamFrozen(bool)
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* Team::GetVehicle(unsigned) const
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void Team::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::GetPropertiesNames(std::set<CStr>&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnEnemyDestroyed(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIGetCurAngle(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    int Team::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::HoldFire(int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnTargetReached(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::GetPropertiesIDs(std::set<int>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Team::GetNumVehicles() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::GetTeamFrozen() const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    TeamTactic* Team::GetTeamTactic() const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_AdjustBehaviour()
    {
        throw std::logic_error("Not implemented");
    }

    float Team::GetDistToPhysicObj(PhysicObj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    TeamPrototypeInfo const* Team::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Team::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnMoveAlongPath(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnDefend(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIGetCurPos(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnTargetUnreachable(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::SetRemoveWhenChilrenDead(bool)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnPathFind(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Team::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::CreateChildren()
    {
        throw std::logic_error("Not implemented");
    }

    Formation* Team::GetFormation() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Team::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    m3d::AIParam Team::TeamAIOnAttackOrder(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnStartDefend(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::Stop()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::AIParam Team::TeamAIOnMove(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void Team::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_DoPosUnreachable()
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_RemoveVehicles()
    {
        throw std::logic_error("Not implemented");
    }

    AI* Team::GetAIPtr()
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_DoNoticeEnemy(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_DoUnderAttack(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_TeamUpdate(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    Team::~Team()
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_OnPlayerVehicleChanged(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_OnObjectDie(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_CreateFormation()
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_OnUnderAttack(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_AddVehicleToFormation(Vehicle*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Team::_IsTooFarFromTargets() const
    {
        throw std::logic_error("Not implemented");
    }

    float Team::_GetTeamVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_TuneFormation()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Team::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_AdjustRoles(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Team::_OnNoticeEnemy(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Team::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

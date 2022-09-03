#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/vector.h>
#include <server/ai/ai.h>
#include <server/formations/formation.h>

namespace ai
{
    class TeamTactic;
    class PhysicObj;
    class Path;
    class Vehicle;
    class CombatMastermind;

    class TeamPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual void PostLoad(void);
        virtual ai::Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        float GetFormationDistBetweenVehicles() const;
        TeamPrototypeInfo();
        bool GetOverridesDistBetweenVehicles() const;

    private:
        int m_decisionMatrixNum;
        bool m_bRemoveWhenChildrenDead;
        int m_formationPrototypeId;
        CStr m_formationPrototypeName;
        bool m_overridesDistBetweenVehicles;
        float m_formationDistBetweenVehicles;
    };

    class Team :  public Obj
    {
    public:
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        void AttackNow(int);
        static m3d::AIParam __fastcall TeamAIOnMoveFinished(Obj *);
        static m3d::AIParam __fastcall TeamAIOnStartSearch(Obj *);
        static m3d::AIParam __fastcall TeamAIOnAttack(Obj *);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        Team(TeamPrototypeInfo const &);
        virtual bool RemoveChild(Obj *);
        virtual bool bIsEqualToPrototype() const ;
        static m3d::AIParam __fastcall TeamAIOnIdle(Obj *);
        static m3d::AIParam __fastcall TeamAIOnStartAttack(Obj *);
        virtual m3d::Class * GetClass() const ;
        std::vector<Vehicle *> const & GetVehicles() const ;
        void SetTeamFrozen(bool);
        Vehicle * GetVehicle(unsigned int) const ;
        static void __fastcall Registration();
        void SetTeamTactic(TeamTactic *);
        virtual void Update(float,unsigned int);
        virtual void GetPropertiesNames(std::set<CStr> &) const ;
        std::map<int,CVector> const & GetSteeringForceMap() const ;
        static m3d::AIParam __fastcall TeamAIOnEnemyDestroyed(Obj *);
        static m3d::AIParam __fastcall TeamAIGetCurAngle(Obj *);
        virtual int OnEvent(Event const &);
        void HoldFire(int);
        static m3d::AIParam __fastcall TeamAIOnTargetReached(Obj *);
        virtual void GetPropertiesIDs(std::set<int> &) const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        unsigned int GetNumVehicles() const ;
        bool GetTeamFrozen() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        TeamTactic * GetTeamTactic() const ;
        void _AdjustBehaviour();
        float GetDistToPhysicObj(PhysicObj const *) const ;
        virtual TeamPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        static m3d::AIParam __fastcall TeamAIOnMoveAlongPath(Obj *);
        static m3d::AIParam __fastcall TeamAIOnDefend(Obj *);
        static m3d::AIParam __fastcall TeamAIGetCurPos(Obj *);
        static m3d::AIParam __fastcall TeamAIOnTargetUnreachable(Obj *);
        void SetRemoveWhenChilrenDead(bool);
        static m3d::AIParam __fastcall TeamAIOnPathFind(Obj *);
        virtual void AddChild(Obj *);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual int GetPropertyId(char const *) const ;
        virtual void CreateChildren();
        Formation * GetFormation() const ;
        static m3d::Class * GetBaseClass();
        static m3d::AIParam __fastcall TeamAIOnAttackOrder(Obj *);
        virtual void Remove();
        static m3d::AIParam __fastcall TeamAIOnStartDefend(Obj *);
        void SetDestination(CVector const &);
        virtual void RenderDebugInfo() const ;
        void Stop();
        static m3d::AIParam __fastcall TeamAIOnMove(Obj *);
        virtual void SetPassedToAnotherMapStatus();

    protected:
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual void _InternalPostLoad();
        virtual void _DoPosUnreachable();
        void _RemoveVehicles();
        virtual AI * GetAIPtr();
        virtual void _DoNoticeEnemy(int);
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _DoUnderAttack(int);
        virtual void _TeamUpdate(float,unsigned int);
        virtual ~Team();

    private:
        void _OnPlayerVehicleChanged(Event const &);
        void _OnObjectDie(Event const &);
        void _CreateFormation();
        void _OnUnderAttack(Event const &);
        CVector _GetAggregatedTargetsPos() const ;
        void _AddVehicleToFormation(Vehicle *);
        void _GetTargetsPositions(std::vector<CVector> &) const ;
        bool _IsTooFarFromTargets() const ;
        float _GetTeamVelocity() const ;
        void _TuneFormation();
        virtual m3d::Object * Clone();
        CVector _GetAggregatedPos() const ;
        void _AdjustRoles(int);
        void _OnNoticeEnemy(Event const &);
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(Team);

    private:
        bool m_bRemoveWhenChildrenDead;
        bool m_bUseStandardUpdatingBehavior;
        AI m_AI;
        std::vector<Vehicle *> m_vehicles;
        float m_maxTeamSpeed;
        Formation *m_formation;
        CombatMastermind *m_combatMastermind;
        Path *m_pPath;
        std::map<int,CVector> m_steeringForceMap;
        bool m_needAdjustBehaviour;
        bool m_bFrozen;
        CStr m_TeamTacticName;
        int m_TeamTacticId;
        bool m_TeamTacticShouldBeAssigned;
        bool m_bMustMoveToTarget;
    };
}

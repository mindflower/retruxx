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

    int constexpr MAX_VEHICLES_IN_TEAM = 5;

    class TeamPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        int m_decisionMatrixNum;
        bool m_bRemoveWhenChildrenDead;
        int m_formationPrototypeId;
        TeamPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
        float GetFormationDistBetweenVehicles() const;
        bool GetOverridesDistBetweenVehicles() const;

    protected:
        CStr m_formationPrototypeName;

    private:
        bool m_overridesDistBetweenVehicles;
        float m_formationDistBetweenVehicles;
    };

    class Team : public ai::Obj
    {
        friend class TeamPrototypeInfo;

    protected:
        virtual ~Team() override;

    protected:
        Team(ai::TeamPrototypeInfo const& prototypeInfo);
        Team(ai::Team const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classTeam;
        virtual ai::TeamPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;

    protected:
        static void RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

        using VehicleVector = retruxx::vector<ai::Vehicle*, retruxx::allocator<ai::Vehicle*>>;

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual bool bIsEqualToPrototype() const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void Remove() override;
        virtual void CreateChildren() override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void RenderDebugInfo() const override;
        unsigned int GetNumVehicles() const;
        ai::Vehicle* GetVehicle(unsigned int index) const;
        retruxx::vector<ai::Vehicle*, retruxx::allocator<ai::Vehicle*>> const& GetVehicles() const;
        void SetDestination(CVector const& destination);
        void Stop();
        ai::Formation* GetFormation() const;
        retruxx::map<int, CVector, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector>>> const&
            GetSteeringForceMap() const;
        void AttackNow(int id);
        void HoldFire(int msc);
        virtual void SetPassedToAnotherMapStatus() override;
        void SetRemoveWhenChilrenDead(bool bRemoveWhenChildrenDead);
        bool GetTeamFrozen() const;
        void SetTeamFrozen(bool bFrozen);
        ai::TeamTactic* GetTeamTactic() const;
        void SetTeamTactic(ai::TeamTactic* tactic);
        void _AdjustBehaviour();
        float GetDistToPhysicObj(ai::PhysicObj const* obj) const;

    protected:
        bool m_bRemoveWhenChildrenDead;
        bool m_bUseStandardUpdatingBehavior;
        virtual void _TeamUpdate(float elapsedTime, unsigned int workTime);
        virtual ai::AI* GetAIPtr() override;
        virtual void _InternalPostLoad() override;
        virtual void _DoNoticeEnemy(int objId);
        virtual void _DoUnderAttack(int attackerId);
        virtual void _DoPosUnreachable();
        void _RemoveVehicles();

    private:
        ai::AI m_AI;
        retruxx::vector<ai::Vehicle*, retruxx::allocator<ai::Vehicle*>> m_vehicles;
        float m_maxTeamSpeed;
        ai::Formation* m_formation;
        ai::CombatMastermind* m_combatMastermind;
        ai::Path* m_pPath;
        retruxx::map<int, CVector, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector>>>
            m_steeringForceMap;
        bool m_needAdjustBehaviour;
        bool m_bFrozen;
        CStr m_TeamTacticName;
        int m_TeamTacticId;
        bool m_TeamTacticShouldBeAssigned;
        bool m_bMustMoveToTarget;
        float _GetTeamVelocity() const;
        void _AdjustRoles(int targetId);
        void _GetTargetsPositions(retruxx::vector<CVector, retruxx::allocator<CVector>>& positions) const;
        void _AddVehicleToFormation(ai::Vehicle* pVehicle);
        void _CreateFormation();
        void _TuneFormation();
        CVector _GetAggregatedPos() const;
        CVector _GetAggregatedTargetsPos() const;
        bool _IsTooFarFromTargets() const;
        void _OnObjectDie(ai::Event const& evn);
        void _OnNoticeEnemy(ai::Event const& evn);
        void _OnPlayerVehicleChanged(ai::Event const& evn);
        void _OnUnderAttack(ai::Event const& evn);

    public:
        static void Registration();
        static m3d::AIParam TeamAIGetCurPos(ai::Obj* pObj);
        static m3d::AIParam TeamAIGetCurAngle(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnDefend(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnAttack(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnMove(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnStartDefend(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnIdle(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnStartAttack(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnAttackOrder(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnEnemyDestroyed(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnStartSearch(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnPathFind(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnMoveAlongPath(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnFallIn(ai::Obj*);
        static m3d::AIParam TeamAIOnTargetReached(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnTargetUnreachable(ai::Obj* pObj);
        static m3d::AIParam TeamAIOnMoveFinished(ai::Obj* pObj);
    };
}  // namespace ai

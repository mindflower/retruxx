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

    class TeamPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        /* 0x0040 */ int m_decisionMatrixNum;
        /* 0x0044 */ bool m_bRemoveWhenChildrenDead;
        /* 0x0045 */ char Padding_263[3];
        /* 0x0048 */ int m_formationPrototypeId;
        TeamPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x08 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        float GetFormationDistBetweenVehicles() const;
        bool GetOverridesDistBetweenVehicles() const;

    protected:
        /* 0x004c */ CStr m_formationPrototypeName;

    private:
        /* 0x0058 */ bool m_overridesDistBetweenVehicles;
        /* 0x0059 */ char Padding_264[3];
        /* 0x005c */ float m_formationDistBetweenVehicles;
    }; /* size: 0x0060 */

    static_assert(sizeof(TeamPrototypeInfo) == 0x0060);

    class Team : public ai::Obj
    {
    protected:
        virtual  ~Team() override /* 0x00 */;

    private:
        Team(const ai::TeamPrototypeInfo& prototypeInfo);
        Team(const ai::Team&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classTeam;
        virtual const ai::TeamPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x40 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

        using VehicleVector = retruxx::vector<ai::Vehicle*, retruxx::allocator<ai::Vehicle*> >;

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xac */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb4 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual bool bIsEqualToPrototype() const override /* 0xbc */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void Remove() override /* 0x54 */;
        virtual void CreateChildren() override /* 0xa4 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x94 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x9c */;
        virtual void RenderDebugInfo() const override /* 0xe4 */;
        unsigned int GetNumVehicles() const;
        ai::Vehicle* GetVehicle(unsigned int index) const;
        const retruxx::vector<ai::Vehicle*, retruxx::allocator<ai::Vehicle*> >& GetVehicles() const;
        void SetDestination(const CVector& destination);
        void Stop();
        ai::Formation* GetFormation() const;
        const retruxx::map<int, CVector, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector> > >& GetSteeringForceMap() const;
        void AttackNow(int id);
        void HoldFire(int msc);
        virtual void SetPassedToAnotherMapStatus() override /* 0x50 */;
        void SetRemoveWhenChilrenDead(bool bRemoveWhenChildrenDead);
        bool GetTeamFrozen() const;
        void SetTeamFrozen(bool bFrozen);
        ai::TeamTactic* GetTeamTactic() const;
        void SetTeamTactic(ai::TeamTactic* tactic);
        void _AdjustBehaviour();
        float GetDistToPhysicObj(const ai::PhysicObj* obj) const;

    protected:
        /* 0x00c0 */ bool m_bRemoveWhenChildrenDead;
        /* 0x00c1 */ bool m_bUseStandardUpdatingBehavior;
        virtual void _TeamUpdate(float elapsedTime, unsigned int workTime) /* 0x110 */;
        virtual ai::AI* GetAIPtr() override /* 0x44 */;
        virtual void _InternalPostLoad() override /* 0xfc */;
        virtual void _DoNoticeEnemy(int objId) /* 0x114 */;
        virtual void _DoUnderAttack(int attackerId) /* 0x118 */;
        virtual void _DoPosUnreachable() /* 0x11c */;
        void _RemoveVehicles();
        /* 0x00c2 */ char Padding_217[2];

    private:
        /* 0x00c4 */ ai::AI m_AI;
        /* 0x0124 */ retruxx::vector<ai::Vehicle*, retruxx::allocator<ai::Vehicle*> > m_vehicles;
        /* 0x0134 */ float m_maxTeamSpeed;
        /* 0x0138 */ ai::Formation* m_formation;
        /* 0x013c */ ai::CombatMastermind* m_combatMastermind;
        /* 0x0140 */ ai::Path* m_pPath;
        /* 0x0144 */ retruxx::map<int, CVector, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector> > > m_steeringForceMap;
        /* 0x0150 */ bool m_needAdjustBehaviour;
        /* 0x0151 */ bool m_bFrozen;
        /* 0x0152 */ char Padding_218[2];
        /* 0x0154 */ CStr m_TeamTacticName;
        /* 0x0160 */ int m_TeamTacticId;
        /* 0x0164 */ bool m_TeamTacticShouldBeAssigned;
        /* 0x0165 */ bool m_bMustMoveToTarget;
        float _GetTeamVelocity() const;
        void _AdjustRoles(int targetId);
        void _GetTargetsPositions(retruxx::vector<CVector, retruxx::allocator<CVector> >& positions) const;
        void _AddVehicleToFormation(ai::Vehicle* pVehicle);
        void _CreateFormation();
        void _TuneFormation();
        CVector _GetAggregatedPos() const;
        CVector _GetAggregatedTargetsPos() const;
        bool _IsTooFarFromTargets() const;
        void _OnObjectDie(const ai::Event& evn);
        void _OnNoticeEnemy(const ai::Event& evn);
        void _OnPlayerVehicleChanged(const ai::Event& evn);
        void _OnUnderAttack(const ai::Event& evn);

    public:
        static void __fastcall Registration();
        static m3d::AIParam __fastcall TeamAIGetCurPos(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIGetCurAngle(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnDefend(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnAttack(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnMove(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnStartDefend(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnIdle(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnStartAttack(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnAttackOrder(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnEnemyDestroyed(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnStartSearch(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnPathFind(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnMoveAlongPath(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnFallIn(ai::Obj*);
        static m3d::AIParam __fastcall TeamAIOnTargetReached(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnTargetUnreachable(ai::Obj* pObj);
        static m3d::AIParam __fastcall TeamAIOnMoveFinished(ai::Obj* pObj);
    }; /* size: 0x0168 */

    static_assert(sizeof(Team) == 0x0168);
}

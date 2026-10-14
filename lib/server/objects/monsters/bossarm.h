#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/physicbodies/vehiclepart.h>

namespace ai
{
    class BossArmPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        struct AttackActionInfo
        {
            int m_frameToReleaseLoad;
            ActionType m_action;
            AttackActionInfo();
            void LoadFromXML(const m3d::cmn::XmlNode* xmlNode);
        };

        using AttackActionInfoVector = retruxx::vector<ai::BossArmPrototypeInfo::AttackActionInfo, retruxx::allocator<ai::BossArmPrototypeInfo::AttackActionInfo> >;

    public:
        float m_turningSpeed;
        int m_frameToPickUpLoad;
        int m_lpIdForLoad;
        int m_cruticalNumExplodedLoads;
        retruxx::vector<ai::BossArmPrototypeInfo::AttackActionInfo, retruxx::allocator<ai::BossArmPrototypeInfo::AttackActionInfo> > m_attacks;
        BossArmPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class BossArm : public ai::VehiclePart
    {
        friend class BossArmPrototypeInfo;

    protected:
        virtual  ~BossArm() override;

    protected:
        BossArm(const ai::BossArmPrototypeInfo& prototypeInfo);
        BossArm(const ai::BossArm&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBossArm;
        virtual const ai::BossArmPrototypeInfo* GetPrototypeInfo() const override;
        virtual int OnEvent(const ai::Event& evn) override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        int GetNumExploadedLoads() const;

        enum AttackState
        {
            ATTACK_IDLE = 0,
            ATTACK_CUSTOM = 1,
            ATTACK_CHARGING = 2,
            ATTACK_ATTACKING = 3,
        };

    protected:
        int m_loadObjId;
        ai::BossArm::AttackState _GetAttackState() const;
        void _SetAttackState(ai::BossArm::AttackState newState);
        void _PlaceLoadOnLoadpoint(float elapsedTime);
        void _SetLoadPrototypeIds(const retruxx::vector<int, retruxx::allocator<int> >& loadPrototypeIds);
        void _TurnToDir(const CVector& desiredDir, float elapsedTime);
        const CVector& _GetCurLoadVelocity() const;

    private:
        const float m_turningSpeed;
        ai::BossArm::AttackState m_attackState;
        CVector m_dirForCharging;
        int m_curAttackAction;
        int m_numExplodedLoads;
        bool m_bCurLoadExploded;
        retruxx::vector<int, retruxx::allocator<int> > m_loadProrotypeIds;
        CVector m_curLoadVelocity;
        void _OnObjectDie(const ai::Event& evn);
    };
}

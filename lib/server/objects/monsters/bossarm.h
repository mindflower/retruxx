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
            void LoadFromXML(m3d::cmn::XmlNode const* xmlNode);
        };

        using AttackActionInfoVector = retruxx::vector<
            ai::BossArmPrototypeInfo::AttackActionInfo,
            retruxx::allocator<ai::BossArmPrototypeInfo::AttackActionInfo>>;

    public:
        float m_turningSpeed;
        int m_frameToPickUpLoad;
        int m_lpIdForLoad;
        int m_cruticalNumExplodedLoads;
        retruxx::vector<
            ai::BossArmPrototypeInfo::AttackActionInfo,
            retruxx::allocator<ai::BossArmPrototypeInfo::AttackActionInfo>>
            m_attacks;
        BossArmPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class BossArm : public ai::VehiclePart
    {
        friend class BossArmPrototypeInfo;

    protected:
        virtual ~BossArm() override;

    protected:
        BossArm(ai::BossArmPrototypeInfo const& prototypeInfo);
        BossArm(ai::BossArm const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBossArm;
        virtual ai::BossArmPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
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
        void _SetLoadPrototypeIds(retruxx::vector<int, retruxx::allocator<int>> const& loadPrototypeIds);
        void _TurnToDir(CVector const& desiredDir, float elapsedTime);
        CVector const& _GetCurLoadVelocity() const;

    private:
        float const m_turningSpeed;
        ai::BossArm::AttackState m_attackState;
        CVector m_dirForCharging;
        int m_curAttackAction;
        int m_numExplodedLoads;
        bool m_bCurLoadExploded;
        retruxx::vector<int, retruxx::allocator<int>> m_loadProrotypeIds;
        CVector m_curLoadVelocity;
        void _OnObjectDie(ai::Event const& evn);
    };
}  // namespace ai

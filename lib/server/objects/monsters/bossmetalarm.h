#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/base/simplephysicobj.h>

namespace ai
{
    class BossMetalArmPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
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
            ai::BossMetalArmPrototypeInfo::AttackActionInfo,
            retruxx::allocator<ai::BossMetalArmPrototypeInfo::AttackActionInfo>>;

    public:
        CStr m_explosionEffectName;
        float m_turningSpeed;
        int m_frameToPickUpLoad;
        int m_lpIdForLoad;
        retruxx::vector<int, retruxx::allocator<int>> m_loadProrotypeIds;
        retruxx::vector<
            ai::BossMetalArmPrototypeInfo::AttackActionInfo,
            retruxx::allocator<ai::BossMetalArmPrototypeInfo::AttackActionInfo>>
            m_attacks;
        int m_numExplodedLoadsToDie;
        BossMetalArmPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_loadPtototypeNames;
    };

    class BossMetalArm : public ai::SimplePhysicObj
    {
        friend class BossMetalArmPrototypeInfo;

    protected:
        virtual ~BossMetalArm() override;

    private:
        BossMetalArm(ai::BossMetalArmPrototypeInfo const& prototypeInfo);
        BossMetalArm(ai::BossMetalArm const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBossMetalArm;
        virtual ai::BossMetalArmPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;

        enum AttackState
        {
            ATTACK_IDLE = 0,
            ATTACK_NOTICED_PLAYER = 1,
            ATTACK_CHARGING = 2,
            ATTACK_ATTACKING = 3,
        };

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void SetPositionSelf(CVector const& pos) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        void Die();
        float GetMaxHealth() const;
        float GetHealth() const;

    private:
        int m_loadObjId;
        float const m_turningSpeed;
        ai::BossMetalArm::AttackState m_attackState;
        CVector m_dirForCharging;
        int m_curAttackAction;
        int m_numExplodedLoads;
        bool m_bCurLoadExploded;
        void _SetAttackState(ai::BossMetalArm::AttackState newState);
        int _GetCurAnimationFrame() const;
        void _TurnToDir(CVector const& desiredDir, float elapsedTime);
        void _PlaceLoadOnLoadpoint();
        void _OnObjectDie(ai::Event const& evn);
    };
}  // namespace ai

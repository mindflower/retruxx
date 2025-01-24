#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/base/simplephysicobj.h>

namespace ai
{
    class BossMetalArmPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        struct AttackActionInfo;
        using AttackActionInfoVector = retruxx::vector<ai::BossMetalArmPrototypeInfo::AttackActionInfo, retruxx::allocator<ai::BossMetalArmPrototypeInfo::AttackActionInfo> >;

    public:
        /* 0x0080 */ CStr m_explosionEffectName;
        /* 0x008c */ float m_turningSpeed;
        /* 0x0090 */ int m_frameToPickUpLoad;
        /* 0x0094 */ int m_lpIdForLoad;
        /* 0x0098 */ retruxx::vector<int, retruxx::allocator<int> > m_loadProrotypeIds;
        /* 0x00a8 */ retruxx::vector<ai::BossMetalArmPrototypeInfo::AttackActionInfo, retruxx::allocator<ai::BossMetalArmPrototypeInfo::AttackActionInfo> > m_attacks;
        /* 0x00b8 */ int m_numExplodedLoadsToDie;
        BossMetalArmPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x00bc */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_loadPtototypeNames;
    }; /* size: 0x00cc */

    static_assert(sizeof(BossMetalArmPrototypeInfo) == 0x00cc);

    class BossMetalArm : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~BossMetalArm() override /* 0x00 */;

    private:
        BossMetalArm(const ai::BossMetalArmPrototypeInfo& prototypeInfo);
        BossMetalArm(const ai::BossMetalArm&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBossMetalArm;
        virtual const ai::BossMetalArmPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;

        enum AttackState;

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        void Die();
        float GetMaxHealth() const;
        float GetHealth() const;

    private:
        /* 0x0144 */ int m_loadObjId;
        /* 0x0148 */ const float m_turningSpeed;
        /* 0x014c */ ai::BossMetalArm::AttackState m_attackState;
        /* 0x0150 */ CVector m_dirForCharging;
        /* 0x015c */ int m_curAttackAction;
        /* 0x0160 */ int m_numExplodedLoads;
        /* 0x0164 */ bool m_bCurLoadExploded;
        void _SetAttackState(ai::BossMetalArm::AttackState newState);
        int _GetCurAnimationFrame() const;
        void _TurnToDir(const CVector& desiredDir, float elapsedTime);
        void _PlaceLoadOnLoadpoint();
        void _OnObjectDie(const ai::Event& evn);
    }; /* size: 0x0168 */

    static_assert(sizeof(BossMetalArm) == 0x0168);
}

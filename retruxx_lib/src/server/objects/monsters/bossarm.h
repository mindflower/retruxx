#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/physicbodies/vehiclepart.h>

namespace ai
{
    class BossArmPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        struct AttackActionInfo;
        using AttackActionInfoVector = retruxx::vector<ai::BossArmPrototypeInfo::AttackActionInfo, retruxx::allocator<ai::BossArmPrototypeInfo::AttackActionInfo> >;

    public:
        /* 0x0110 */ float m_turningSpeed;
        /* 0x0114 */ int m_frameToPickUpLoad;
        /* 0x0118 */ int m_lpIdForLoad;
        /* 0x011c */ int m_cruticalNumExplodedLoads;
        /* 0x0120 */ retruxx::vector<ai::BossArmPrototypeInfo::AttackActionInfo, retruxx::allocator<ai::BossArmPrototypeInfo::AttackActionInfo> > m_attacks;
        BossArmPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x0c */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0130 */

    static_assert(sizeof(BossArmPrototypeInfo) == 0x0130);

    class BossArm : public ai::VehiclePart
    {
    protected:
        virtual  ~BossArm() override /* 0x00 */;

    private:
        BossArm(const ai::BossArmPrototypeInfo& prototypeInfo);
        BossArm(const ai::BossArm&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBossArm;
        virtual const ai::BossArmPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        int GetNumExploadedLoads() const;

        enum AttackState;

    protected:
        /* 0x02c8 */ int m_loadObjId;
        ai::BossArm::AttackState _GetAttackState() const;
        void _SetAttackState(ai::BossArm::AttackState newState);
        void _PlaceLoadOnLoadpoint(float elapsedTime);
        void _SetLoadPrototypeIds(const retruxx::vector<int, retruxx::allocator<int> >& loadPrototypeIds);
        void _TurnToDir(const CVector& desiredDir, float elapsedTime);
        const CVector& _GetCurLoadVelocity() const;

    private:
        /* 0x02cc */ const float m_turningSpeed;
        /* 0x02d0 */ ai::BossArm::AttackState m_attackState;
        /* 0x02d4 */ CVector m_dirForCharging;
        /* 0x02e0 */ int m_curAttackAction;
        /* 0x02e4 */ int m_numExplodedLoads;
        /* 0x02e8 */ bool m_bCurLoadExploded;
        /* 0x02e9 */ char Padding_147[3];
        /* 0x02ec */ retruxx::vector<int, retruxx::allocator<int> > m_loadProrotypeIds;
        /* 0x02fc */ CVector m_curLoadVelocity;
        void _OnObjectDie(const ai::Event& evn);
    }; /* size: 0x0308 */

    static_assert(sizeof(BossArm) == 0x0308);
}

#pragma once
#include <server/components/numericinrange.h>
#include <server/objects/dummyobject.h>

struct dContact;

namespace ai
{
    class BossMetalArmLoadPrototypeInfo : public ai::DummyObjectPrototypeInfo
    {
    public:
        /* 0x0084 */ int m_blastWavePrototypeId;
        /* 0x0088 */ CStr m_explosionEffectName;
        /* 0x0094 */ float m_maxHealth;
        BossMetalArmLoadPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x0098 */ CStr m_blastWavePrototypeName;
    }; /* size: 0x00a4 */

    static_assert(sizeof(BossMetalArmLoadPrototypeInfo) == 0x00a4);

    class BossMetalArmLoad : public ai::DummyObject
    {
    protected:
        virtual  ~BossMetalArmLoad() override /* 0x00 */;

    private:
        BossMetalArmLoad(const ai::BossMetalArmLoadPrototypeInfo& prototypeInfo);
        BossMetalArmLoad(const ai::BossMetalArmLoad&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBossMetalArmLoad;
        virtual const ai::BossMetalArmLoadPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::BossMetalArmLoad, float, void>;
        enum CollisionMode;

    public:
        virtual bool ApplyModifier(const ai::Modifier& modifier) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        void SetCollisionMode(ai::BossMetalArmLoad::CollisionMode collisionMode);
        void Explode();
        static int __fastcall CollideBossMetalArmLoadWithObject(m3d::Object* obj1, m3d::Object* obj2, dContact* contacts, unsigned int& numContacts, bool reverse);

    private:
        /* 0x0150 */ ai::BossMetalArmLoad::CollisionMode m_collisionMode;
        /* 0x0154 */ ai::NumericInRange<float> m_health;
        void _CreateBlastWave();
        void _OnAfterHealthValueChange(float oldHealth);
    }; /* size: 0x0200 */

    static_assert(sizeof(BossMetalArmLoad) == 0x0200);
}

#pragma once
#include <server/components/numericinrange.h>
#include <server/objects/dummyobject.h>

struct dContact;

namespace ai
{
    class BossMetalArmLoadPrototypeInfo : public ai::DummyObjectPrototypeInfo
    {
    public:
        int m_blastWavePrototypeId;
        CStr m_explosionEffectName;
        float m_maxHealth;
        BossMetalArmLoadPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_blastWavePrototypeName;
    };

    class BossMetalArmLoad : public ai::DummyObject
    {
        // The arms set the collision mode directly when they let go of a load.
        friend class BossMetalArmLoadPrototypeInfo;
        friend class BossArm;
        friend class BossMetalArm;

    protected:
        virtual ~BossMetalArmLoad() override;

    private:
        BossMetalArmLoad(ai::BossMetalArmLoadPrototypeInfo const& prototypeInfo);
        BossMetalArmLoad(ai::BossMetalArmLoad const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBossMetalArmLoad;
        virtual ai::BossMetalArmLoadPrototypeInfo const* GetPrototypeInfo() const override;

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::BossMetalArmLoad, float, void>;
        enum CollisionMode
        {
            COLLIDE_NONE = 0,
            COLLIDE_NORMAL = 1,
            COLLIDE_EXPLODE = 2,
        };

    public:
        virtual bool ApplyModifier(ai::Modifier const& modifier) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void SetCollisionMode(ai::BossMetalArmLoad::CollisionMode collisionMode);
        void Explode();
        static int CollideBossMetalArmLoadWithObject(
            m3d::Object* obj1,
            m3d::Object* obj2,
            dContact* contacts,
            unsigned int& numContacts,
            bool reverse);

    private:
        ai::BossMetalArmLoad::CollisionMode m_collisionMode;
        ai::NumericInRange<float> m_health;
        void _CreateBlastWave();
        void _OnAfterHealthValueChange(float oldHealth);
    };
}  // namespace ai

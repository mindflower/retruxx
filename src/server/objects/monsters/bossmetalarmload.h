#pragma once
#include <server/components/numericinrange.h>
#include <server/objects/dummyobject.h>

struct dContact;

namespace ai
{
    class BossMetalArmLoadPrototypeInfo : public DummyObjectPrototypeInfo
    {
    public:
        BossMetalArmLoadPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        virtual void PostLoad();

    private:
        int m_blastWavePrototypeId;
        CStr m_explosionEffectName;
        float m_maxHealth;
        CStr m_blastWavePrototypeName;
    };

    class BossMetalArmLoad :  public DummyObject
    {
    public:
        enum CollisionMode
        {
            COLLIDE_NONE = 0x0,
            COLLIDE_NORMAL = 0x1,
            COLLIDE_EXPLODE = 0x2,
        };

    public:
        virtual bool ApplyModifier(Modifier const &);
        static m3d::Class * GetBaseClass();
        void SetCollisionMode(CollisionMode);
        BossMetalArmLoad(BossMetalArmLoadPrototypeInfo const &);
        virtual BossMetalArmLoadPrototypeInfo const * GetPrototypeInfo() const ;
        static int CollideBossMetalArmLoadWithObject(m3d::Object *,m3d::Object *,dContact *,unsigned int &,bool);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual m3d::Class * GetClass() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

        void Explode();
    protected:
        virtual ~BossMetalArmLoad();

    private:
        static m3d::Object * CreateObject();
        void _OnAfterHealthValueChange(float);
        virtual m3d::Object * Clone();
        //MemberFunctionOneArg<BossMetalArmLoad,float,void>::MemberFunctionOneArg<BossMetalArmLoad,float,void>(BossMetalArmLoad &,void (*const)(float));
        void _CreateBlastWave();

    public:
        RT_CLASS_INLINE_DECLARE(BossMetalArmLoad);

    private:
        CollisionMode m_collisionMode;
        NumericInRange<float> m_health;
    };
}

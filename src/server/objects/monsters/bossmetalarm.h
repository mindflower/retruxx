#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/base/simplephysicobj.h>

namespace ai
{
    class BossMetalArmPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        class AttackActionInfo
        {
        public:
            AttackActionInfo();
            void LoadFromXML(m3d::cmn::XmlNode const*);

        private:
            int m_frameToReleaseLoad;
            ActionType m_action;
        };

    public:
        BossMetalArmPrototypeInfo();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        virtual Obj* CreateTargetObject() const;

    private:
        CStr m_explosionEffectName;
        float m_turningSpeed;
        int m_frameToPickUpLoad;
        int m_lpIdForLoad;
        std::vector<int> m_loadProrotypeIds;
        std::vector<AttackActionInfo> m_attacks;
        int m_numExplodedLoadsToDie;
        std::vector<CStr> m_loadPtototypeNames;
    };

    class BossMetalArm : public SimplePhysicObj
    {
    public:
        using AttackState = Vehicle::VehicleAttackStatus;

    public:
        static m3d::Class * GetBaseClass();
        BossMetalArm(BossMetalArmPrototypeInfo const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual BossMetalArmPrototypeInfo const * GetPrototypeInfo() const ;
        float GetMaxHealth() const ;
        float GetHealth() const ;
        virtual int OnEvent(Event const &);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual m3d::Class * GetClass() const ;
        virtual void Update(float,unsigned int);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void Die();
        virtual void SetPositionSelf(CVector const &);

    protected:
        virtual ~BossMetalArm();

    private:
        void _TurnToDir(CVector const &,float);
        int _GetCurAnimationFrame() const ;
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
        void _SetAttackState(AttackState);
        void _PlaceLoadOnLoadpoint();
        void _OnObjectDie(Event const &);

    public:
        RT_CLASS_INLINE_DECLARE(BossMetalArm);

    private:
        int m_loadObjId;
        const float m_turningSpeed;
        AttackState m_attackState;
        CVector m_dirForCharging;
        int m_curAttackAction;
        int m_numExplodedLoads;
        bool m_bCurLoadExploded;
    };
}

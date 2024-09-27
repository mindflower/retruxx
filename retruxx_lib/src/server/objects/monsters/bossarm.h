#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/physicbodies/vehiclepart.h>

namespace ai
{
    class BossArmPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        class AttackActionInfo
        {
        public:
            void LoadFromXML(m3d::cmn::XmlNode const*);
            AttackActionInfo();

        private:
            int m_frameToReleaseLoad;
            ActionType m_action;
        };

    public:
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        BossArmPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_turningSpeed;
        int m_frameToPickUpLoad;
        int m_lpIdForLoad;
        int m_cruticalNumExplodedLoads;
        std::vector<AttackActionInfo> m_attacks;

    };

    class BossArm : public VehiclePart
    {
    public:
        virtual m3d::Class * GetClass() const ;
        virtual BossArmPrototypeInfo const * GetPrototypeInfo() const ;
        BossArm(BossArmPrototypeInfo const &);
        int GetNumExploadedLoads() const ;
        virtual void Update(float,unsigned int);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual int OnEvent(Event const &);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        static m3d::Class * GetBaseClass();

        enum AttackState
        {
            ATTACK_IDLE = 0,
            ATTACK_NOTICED_PLAYER = 1,
            ATTACK_CHARGING = 2,
            ATTACK_ATTACKING = 3,
        };

    protected:
        void _PlaceLoadOnLoadpoint(float);
        void _SetAttackState(AttackState);
        AttackState _GetAttackState() const ;
        void _SetLoadPrototypeIds(std::vector<int,std::allocator<int> > const &);
        virtual ~BossArm();
        void _TurnToDir(CVector const &,float);
        CVector const & _GetCurLoadVelocity() const ;

    public:
        RT_CLASS_DECLARE(BossArm);

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
        void _OnObjectDie(Event const &);
        int m_loadObjId;
        //const float m_turningSpeed;
        AttackState m_attackState;
        CVector m_dirForCharging;
        int m_curAttackAction;
        int m_numExplodedLoads;
        bool m_bCurLoadExploded;
        std::vector<int> m_loadProrotypeIds;
        CVector m_curLoadVelocity;
    };
}

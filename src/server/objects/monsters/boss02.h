#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/base/complexphysicobj.h>

namespace ai
{
    class Boss02Arm;

    class Boss02PrototypeInfo : public ComplexPhysicObjPrototypeInfo
    {
    public:
        class StateInfo
        {
        public:
            void SaveToXML(m3d::cmn::XmlNode*) const;
            void PostLoad();
            StateInfo(StateInfo const&);
            void LoadFromXML(m3d::cmn::XmlNode const*);

        private:
            std::vector<int> m_loadPrototypeIds;
            CVector m_position;
            std::vector<CStr> m_loadPrototypeNames;
        };

    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        Boss02PrototypeInfo();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        std::vector<StateInfo> m_stateInfos;
        float m_speed;
        int m_containerPrototypeId;
        CVector m_relPosForContainerPickUp;
        Quaternion m_relRotForContainerPickUp;
        CVector m_relPosForContainerPutDown;
        CStr m_containerPrototypeName;
    };

    class Boss02 : public ComplexPhysicObj
    {
    public:
        using MoveState = Vehicle::VehicleMoveStatus;

    public:
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual void CreateChildren();
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual m3d::Class* GetClass() const;
        Boss02(Boss02PrototypeInfo const&);
        void NextState();
        virtual bool CanChildBeAdded(m3d::Class*) const;
        virtual int OnEvent(Event const&);
        void SetJustMovedContainerToBlock();
        virtual void Update(float, unsigned int);
        static m3d::Class* __fastcall GetBaseClass();
        virtual Boss02PrototypeInfo const* GetPrototypeInfo() const;
        virtual bool RemoveChild(Obj*);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        float GetHealth() const;
        virtual void AddChild(Obj*);
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void SetPositionSelf(CVector const&);
        float GetMaxHealth() const;
        void Die();

    protected:
        virtual void _InternalPostLoad();
        virtual void _InternalCreateVisualPart();
        virtual ~Boss02();

    private:
        void _SetMoveState(MoveState);
        void _PlaceContainerToInitialPosition();
        VehiclePart* _GetPlatform();
        void _OnBossCriticalLoadsExploded(Event const&);
        static m3d::Object* __fastcall CreateObject();
        void _SetMovePosition(CVector const&);
        virtual m3d::Object* Clone();
        Boss02Arm* _GetArm();
        Boss02Arm const* _GetArm() const;
        void _SetToDeadPose();
        void _OnBossArmActionFinished(Event const&);

    private:
        std::vector<Boss02PrototypeInfo::StateInfo> m_stateInfos;
        int m_numState;
        MoveState m_moveState;
        CVector m_velocity;
        int m_containerId;
    };
}

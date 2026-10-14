#pragma once
#include <server/objects/vehicle.h>
#include <server/objects/base/complexphysicobj.h>

namespace ai
{
    class Boss02Arm;

    class Boss02PrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    public:
        struct StateInfo
        {
            retruxx::vector<int, retruxx::allocator<int>> m_loadPrototypeIds;
            CVector m_position;
            StateInfo(ai::Boss02PrototypeInfo::StateInfo const& __that);
            StateInfo();
            void LoadFromXML(m3d::cmn::XmlNode const* xmlNode);
            void SaveToXML(m3d::cmn::XmlNode* xmlNode) const;
            void PostLoad();
            retruxx::vector<CStr, retruxx::allocator<CStr>> m_loadPrototypeNames;
        };

        using StateInfoVector =
            retruxx::vector<ai::Boss02PrototypeInfo::StateInfo, retruxx::allocator<ai::Boss02PrototypeInfo::StateInfo>>;

    public:
        retruxx::vector<ai::Boss02PrototypeInfo::StateInfo, retruxx::allocator<ai::Boss02PrototypeInfo::StateInfo>>
            m_stateInfos;
        float m_speed;
        int m_containerPrototypeId;
        CVector m_relPosForContainerPickUp;
        Quaternion m_relRotForContainerPickUp;
        CVector m_relPosForContainerPutDown;
        Boss02PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_containerPrototypeName;
    };

    class Boss02 : public ai::ComplexPhysicObj
    {
        friend class Boss02PrototypeInfo;

    protected:
        virtual ~Boss02() override;

    private:
        Boss02(ai::Boss02PrototypeInfo const& prototypeInfo);
        Boss02(ai::Boss02 const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss02;
        virtual ai::Boss02PrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;
        enum MoveState
        {
            MOVE_IDLE = 0,
            MOVE_MOVING = 1,
        };
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void CreateChildren() override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void SetPositionSelf(CVector const& pos) override;
        void NextState();
        void Die();
        void SetJustMovedContainerToBlock();
        float GetMaxHealth() const;
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;

    private:
        retruxx::vector<ai::Boss02PrototypeInfo::StateInfo, retruxx::allocator<ai::Boss02PrototypeInfo::StateInfo>>
            m_stateInfos;
        int m_numState;
        ai::Boss02::MoveState m_moveState;
        CVector m_velocity;
        int m_containerId;
        void _SetMovePosition(CVector const& movePosition);
        void _SetMoveState(ai::Boss02::MoveState moveState);
        ai::Boss02Arm const* _GetArm() const;
        ai::Boss02Arm* _GetArm();
        ai::VehiclePart* _GetPlatform();
        void _PlaceContainerToInitialPosition();
        void _SetToDeadPose();
        void _OnBossCriticalLoadsExploded(ai::Event const& evn);
        void _OnBossArmActionFinished(ai::Event const& evn);
    };
}  // namespace ai

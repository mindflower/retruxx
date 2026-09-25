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
            /* 0x0000 */ retruxx::vector<int, retruxx::allocator<int> > m_loadPrototypeIds;
            /* 0x0010 */ CVector m_position;
            StateInfo(const ai::Boss02PrototypeInfo::StateInfo& __that);
            StateInfo();
            void LoadFromXML(const m3d::cmn::XmlNode* xmlNode);
            void SaveToXML(m3d::cmn::XmlNode* xmlNode) const;
            void PostLoad();
            /* 0x001c */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_loadPrototypeNames;
        }; /* size: 0x002c */

        static_assert(sizeof(StateInfo) == 0x002c);

        using StateInfoVector = retruxx::vector<ai::Boss02PrototypeInfo::StateInfo, retruxx::allocator<ai::Boss02PrototypeInfo::StateInfo> >;

    public:
        /* 0x0090 */ retruxx::vector<ai::Boss02PrototypeInfo::StateInfo, retruxx::allocator<ai::Boss02PrototypeInfo::StateInfo> > m_stateInfos;
        /* 0x00a0 */ float m_speed;
        /* 0x00a4 */ int m_containerPrototypeId;
        /* 0x00a8 */ CVector m_relPosForContainerPickUp;
        /* 0x00b4 */ Quaternion m_relRotForContainerPickUp;
        /* 0x00c4 */ CVector m_relPosForContainerPutDown;
        Boss02PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x00d0 */ CStr m_containerPrototypeName;
    }; /* size: 0x00dc */

    static_assert(sizeof(Boss02PrototypeInfo) == 0x00dc);

    class Boss02 : public ai::ComplexPhysicObj
    {
        friend class Boss02PrototypeInfo;

    protected:
        virtual ~Boss02() override /* 0x00 */;

    private:
        Boss02(const ai::Boss02PrototypeInfo& prototypeInfo);
        Boss02(const ai::Boss02&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss02;
        virtual const ai::Boss02PrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;
        enum MoveState
        {
            MOVE_IDLE = 0,
            MOVE_MOVING = 1,
        };
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void CreateChildren() override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        void NextState();
        void Die();
        void SetJustMovedContainerToBlock();
        float GetMaxHealth() const;
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x100 */;

    private:
        /* 0x014c */ retruxx::vector<ai::Boss02PrototypeInfo::StateInfo, retruxx::allocator<ai::Boss02PrototypeInfo::StateInfo> > m_stateInfos;
        /* 0x015c */ int m_numState;
        /* 0x0160 */ ai::Boss02::MoveState m_moveState;
        /* 0x0164 */ CVector m_velocity;
        /* 0x0170 */ int m_containerId;
        void _SetMovePosition(const CVector& movePosition);
        void _SetMoveState(ai::Boss02::MoveState moveState);
        const ai::Boss02Arm* _GetArm() const;
        ai::Boss02Arm* _GetArm();
        ai::VehiclePart* _GetPlatform();
        void _PlaceContainerToInitialPosition();
        void _SetToDeadPose();
        void _OnBossCriticalLoadsExploded(const ai::Event& evn);
        void _OnBossArmActionFinished(const ai::Event& evn);
    }; /* size: 0x0174 */

    static_assert(sizeof(Boss02) == 0x0174);
}

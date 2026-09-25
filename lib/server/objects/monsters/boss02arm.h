#pragma once
#include "bossarm.h"

namespace ai
{
    class Boss02ArmPrototypeInfo : public ai::BossArmPrototypeInfo
    {
    public:
        /* 0x0130 */ int m_frameToPickUpContainerForBlock;
        /* 0x0134 */ int m_frameToReleaseContainerForBlock;
        /* 0x0138 */ int m_frameToPickUpContainerForDie;
        /* 0x013c */ int m_frameToReleaseContainerForDie;
        /* 0x0140 */ ActionType m_actionForBlock;
        /* 0x0144 */ ActionType m_actionForDie;
        /* 0x0148 */ int m_blockingContainerPrototypeId;
        Boss02ArmPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x0c */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x014c */ CStr m_blockingContainerPrototypeName;
    }; /* size: 0x0158 */

    static_assert(sizeof(Boss02ArmPrototypeInfo) == 0x0158);

    class Boss02Arm : public ai::BossArm
    {
        friend class Boss02ArmPrototypeInfo;

    protected:
        virtual ~Boss02Arm() override /* 0x00 */;

    private:
        Boss02Arm(const ai::Boss02ArmPrototypeInfo& prototypeInfo);
        Boss02Arm(const ai::Boss02Arm&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss02Arm;
        virtual const ai::Boss02ArmPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        enum CustomState
        {
            CUSTOM_NONE = 0,
            CUSTOM_MOVING = 1,
            CUSTOM_MOVING_CONTAINER_TO_BLOCK = 2,
            CUSTOM_DYING = 3,
        };
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        void SetRelativePositionsForContainer(const CVector& relPosForPickUp, const CVector& relPosForPutDown);
        void BeginMovingContainerToBlock(int containerId);
        void BeginAttack(const retruxx::vector<int, retruxx::allocator<int> >& loadPrototypeIds);
        void StopAttack();
        void BeginMove();
        void BeginDie();
        void SetToDeadPose();

    protected:
        virtual void _InternalCreateVisualPart() override /* 0x00 */;

    private:
        /* 0x0308 */ ai::Boss02Arm::CustomState m_customState;
        /* 0x030c */ int m_containerId;
        /* 0x0310 */ CVector m_relPosForContainerPickUp;
        /* 0x031c */ CVector m_relPosForContainerPutDown;
        /* 0x0328 */ bool m_bEffectsEnabled;
        void _SetCustomState(ai::Boss02Arm::CustomState customState);
        void _SetCustomNodeAction(int action, bool forceRestartAction);
        void _SetEffectsDisabled();
    }; /* size: 0x032c */

    static_assert(sizeof(Boss02Arm) == 0x032c);
}

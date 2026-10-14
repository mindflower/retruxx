#pragma once
#include "bossarm.h"

namespace ai
{
    class Boss02ArmPrototypeInfo : public ai::BossArmPrototypeInfo
    {
    public:
        int m_frameToPickUpContainerForBlock;
        int m_frameToReleaseContainerForBlock;
        int m_frameToPickUpContainerForDie;
        int m_frameToReleaseContainerForDie;
        ActionType m_actionForBlock;
        ActionType m_actionForDie;
        int m_blockingContainerPrototypeId;
        Boss02ArmPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_blockingContainerPrototypeName;
    };

    class Boss02Arm : public ai::BossArm
    {
        friend class Boss02ArmPrototypeInfo;

    protected:
        virtual ~Boss02Arm() override;

    private:
        Boss02Arm(ai::Boss02ArmPrototypeInfo const& prototypeInfo);
        Boss02Arm(ai::Boss02Arm const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss02Arm;
        virtual ai::Boss02ArmPrototypeInfo const* GetPrototypeInfo() const override;
        enum CustomState
        {
            CUSTOM_NONE = 0,
            CUSTOM_MOVING = 1,
            CUSTOM_MOVING_CONTAINER_TO_BLOCK = 2,
            CUSTOM_DYING = 3,
        };
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        void SetRelativePositionsForContainer(CVector const& relPosForPickUp, CVector const& relPosForPutDown);
        void BeginMovingContainerToBlock(int containerId);
        void BeginAttack(retruxx::vector<int, retruxx::allocator<int>> const& loadPrototypeIds);
        void StopAttack();
        void BeginMove();
        void BeginDie();
        void SetToDeadPose();

    protected:
        virtual void _InternalCreateVisualPart() override;

    private:
        ai::Boss02Arm::CustomState m_customState;
        int m_containerId;
        CVector m_relPosForContainerPickUp;
        CVector m_relPosForContainerPutDown;
        bool m_bEffectsEnabled;
        void _SetCustomState(ai::Boss02Arm::CustomState customState);
        void _SetCustomNodeAction(int action, bool forceRestartAction);
        void _SetEffectsDisabled();
    };
}  // namespace ai

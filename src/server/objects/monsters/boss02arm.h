#pragma once
#include "bossarm.h"

namespace ai
{
    class Boss02ArmPrototypeInfo : public BossArmPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        Boss02ArmPrototypeInfo();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        virtual Obj* CreateTargetObject() const;

    private:
        int m_frameToPickUpContainerForBlock;
        int m_frameToReleaseContainerForBlock;
        int m_frameToPickUpContainerForDie;
        int m_frameToReleaseContainerForDie;
        ActionType m_actionForBlock;
        ActionType m_actionForDie;
        int m_blockingContainerPrototypeId;
        CStr m_blockingContainerPrototypeName;
    };

    class Boss02Arm : public BossArm
    {
    public:
        enum CustomState
        {
            CUSTOM_NONE = 0x0,
            CUSTOM_MOVING = 0x1,
            CUSTOM_MOVING_CONTAINER_TO_BLOCK = 0x2,
            CUSTOM_DYING = 0x3,
        };

    public:
        static inline m3d::Class m_classBoss02Arm;

    public:
        Boss02Arm(Boss02ArmPrototypeInfo const&);
        virtual void Update(float, unsigned int);
        void BeginMovingContainerToBlock(int);
        void StopAttack();
        static m3d::Class* __fastcall GetBaseClass();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void SetToDeadPose();
        void BeginDie();
        virtual m3d::Class* GetClass() const;
        virtual void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual Boss02ArmPrototypeInfo const* GetPrototypeInfo() const;
        void BeginAttack(std::vector<int, std::allocator<int> > const&);
        void SetRelativePositionsForContainer(CVector const&, CVector const&);
        virtual void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void BeginMove();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

    protected:
        virtual void _InternalCreateVisualPart();
        virtual ~Boss02Arm();
    private:
        void _SetCustomState(CustomState);
        void _SetEffectsDisabled();
        void _SetCustomNodeAction(int, bool);
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();

    private:
        CustomState m_customState;
        int m_containerId;
        CVector m_relPosForContainerPickUp;
        CVector m_relPosForContainerPutDown;
        bool m_bEffectsEnabled;
    };
}

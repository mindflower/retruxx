#pragma once
#include "base/simplephysicobj.h"

struct dxJoint;

namespace ai
{
    class SphericBody;
    class Vehicle;

    class WheelPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        CStr m_suspensionModelName;
        float m_suspensionRange;
        float m_suspensionCFM;
        float m_suspensionERP;
        float m_mU;
        CStr m_typeName;
        CStr m_blowEffectName;
        WheelPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Wheel : public ai::SimplePhysicObj
    {
        friend class WheelPrototypeInfo;

    protected:
        virtual ~Wheel() override;

    private:
        Wheel(ai::WheelPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classWheel;
        virtual ai::WheelPrototypeInfo const* GetPrototypeInfo() const override;

        enum WheelSteering
        {
            STEERING_NO = 0,
            STEERING_CORRECT = 1,
            STEERING_INVERSE = -1,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void Remove() override;
        dxJoint* m_jointID;
        int m_driven;
        ai::Wheel::WheelSteering m_steering;
        m3d::SgNode* m_SplashEffect;
        short m_SplashType;
        bool m_MakeSplash;
        unsigned int m_wheelType;
        float m_curAngle;
        bool m_bModelBroken;
        m3d::SgNode* m_suspensionNode;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void LinkGeomsToCollisionCells() override;
        virtual void UnlinkGeomsFromCollisionCells() override;
        virtual void RelinkGeomsToCollisionCells() override;
        virtual void SetPassedToAnotherMapStatus() override;
        virtual void RenderDebugInfo() const override;
        ai::SphericBody const* _SphericBody() const;
        ai::SphericBody* _SphericBody();
        float GetRadius() const;
        float GetWidth() const;
        virtual CVector GetDirection() const override;
        bool AttachToPhysicObj(ai::PhysicObj const* physicObj);
        void DetachFromPhysicObj();
        ai::Vehicle* GetVehicle() const;
        void CreateSuspensionNode();
        void BreakModel();
        void HealModel();
        Quaternion const& GetInitialRotation() const;
        void SetInitialRotation(Quaternion const& rot);
        static inline CVector const AXIS_FOR_WHEEL = {-1.0, 0.0, 0.0};
        static float const STEERING_LIMIT;

    protected:
        virtual void _InternalCreateVisualPart() override;

    private:
        Quaternion m_initialRotation;
    };
}  // namespace ai

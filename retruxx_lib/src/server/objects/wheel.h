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
        /* 0x0080 */ CStr m_suspensionModelName;
        /* 0x008c */ float m_suspensionRange;
        /* 0x0090 */ float m_suspensionCFM;
        /* 0x0094 */ float m_suspensionERP;
        /* 0x0098 */ float m_mU;
        /* 0x009c */ CStr m_typeName;
        /* 0x00a8 */ CStr m_blowEffectName;
        WheelPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x00b4 */

    class Wheel : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~Wheel() override /* 0x00 */;

    private:
        Wheel(const ai::WheelPrototypeInfo& prototypeInfo);
        Wheel(const ai::Wheel&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classWheel;
        virtual const ai::WheelPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

        enum WheelSteering
        {
            STEERING_NO = 0,
            STEERING_CORRECT = 1,
            STEERING_INVERSE = -1,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void Remove() override /* 0x00 */;
        /* 0x0144 */ dxJoint* m_jointID;
        /* 0x0148 */ int m_driven;
        /* 0x014c */ ai::Wheel::WheelSteering m_steering;
        /* 0x0150 */ m3d::SgNode* m_SplashEffect;
        /* 0x0154 */ short m_SplashType;
        /* 0x0156 */ bool m_MakeSplash;
        /* 0x0157 */ char Padding_303;
        /* 0x0158 */ unsigned int m_wheelType;
        /* 0x015c */ float m_curAngle;
        /* 0x0160 */ bool m_bModelBroken;
        /* 0x0161 */ char Padding_304[3];
        /* 0x0164 */ m3d::SgNode* m_suspensionNode;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void LinkGeomsToCollisionCells() override /* 0x00 */;
        virtual void UnlinkGeomsFromCollisionCells() override /* 0x00 */;
        virtual void RelinkGeomsToCollisionCells() override /* 0x00 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        const ai::SphericBody* _SphericBody() const;
        ai::SphericBody* _SphericBody();
        float GetRadius() const;
        float GetWidth() const;
        virtual CVector GetDirection() const override /* 0x00 */;
        bool AttachToPhysicObj(const ai::PhysicObj* physicObj);
        void DetachFromPhysicObj();
        ai::Vehicle* GetVehicle() const;
        void CreateSuspensionNode();
        void BreakModel();
        void HealModel();
        const Quaternion& GetInitialRotation() const;
        void SetInitialRotation(const Quaternion& rot);
        static const CVector AXIS_FOR_WHEEL;
        static const float STEERING_LIMIT;

    protected:
        virtual void _InternalCreateVisualPart() override /* 0x00 */;

    private:
        /* 0x0168 */ Quaternion m_initialRotation;
    }; /* size: 0x0178 */
}

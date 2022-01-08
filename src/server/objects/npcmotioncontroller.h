#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/vector.h>

namespace ai
{
    class Vehicle;

    class NPCMotionControllerPrototypeInfo : public PrototypeInfo
    {
    public:
        NPCMotionControllerPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

   class NPCMotionController : public Obj
    {
    public:
        enum MotionStyle
        {
            msWithPlayer = 0x0,
            msCircle = 0x1,
            msPendulumRight = 0x2,
            msPendulumLeft = 0x3,
            msPendulumTop = 0x4,
            msPendulumBottom = 0x5,
            msChief = 0x6,
            msChiefPendulum = 0x7,
            msCount = 0x8,
        };

    public:
        RT_CLASS_DECLARE(NPCMotionController);

    public:
        virtual int OnEvent(Event const&);
        void setVehicleUnderControl(Vehicle*);
        void setVehicleUnderControl(m3d::Object*);
        virtual void RenderDebugInfo() const;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void Update(float, unsigned int);
        virtual m3d::Class* GetClass() const;
        Vehicle* getVehicleUnderControl() const;
        virtual NPCMotionControllerPrototypeInfo const* GetPrototypeInfo() const;
        static m3d::Class* __fastcall GetBaseClass();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual void SetPassedToAnotherMapStatus();
        NPCMotionController(NPCMotionControllerPrototypeInfo const&);

    protected:
        virtual ~NPCMotionController();

    private:
        static m3d::Object* __fastcall CreateObject();
        CVector2 getCurrentVehiclePosition() const;
        void _OnObjectDie(Event const&);
        virtual m3d::Object* Clone();
        void setDesiredVehiclePosition(CVector2 const&);

   private:
        int m_vehicleUnderControlId;
        MotionStyle m_style;
        float m_elapsedStateTime;
        float m_timeForState;
        CVector m_lastDesiredPosition;
        float m_characteristicDist;
        float m_characteristicPeriod;
    };
}

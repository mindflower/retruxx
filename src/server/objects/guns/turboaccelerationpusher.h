#pragma once
#include "gun.h"

namespace ai
{
    class TurboAccelerationPusherPrototypeInfo : public GunPrototypeInfo
    {
    public:
        TurboAccelerationPusherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        float m_AccelerationValue;
        float m_AccelerationTime;

    };

    class TurboAccelerationPusher : public Gun
    {
    public:
        RT_CLASS_DECLARE(TurboAccelerationPusher);

    public:
        TurboAccelerationPusher(TurboAccelerationPusherPrototypeInfo const&);
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const;
        virtual TurboAccelerationPusherPrototypeInfo const* GetPrototypeInfo() const;

    protected:
        virtual void _LaunchShells();
        virtual ~TurboAccelerationPusher();

    private:
        virtual m3d::Object* Clone();
        virtual bool isLookAtPoint(CVector const&, float) const;
        static m3d::Object* __fastcall CreateObject();
    };
}

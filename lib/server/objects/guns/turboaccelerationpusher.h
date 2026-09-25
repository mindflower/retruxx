#pragma once
#include "gun.h"

namespace ai
{
    class TurboAccelerationPusherPrototypeInfo : public GunPrototypeInfo
    {
        friend class TurboAccelerationPusher;

    public:
        TurboAccelerationPusherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        /* 0x0198 */ float m_AccelerationValue;
        /* 0x019c */ float m_AccelerationTime;
    }; /* size: 0x01a0 */

    static_assert(sizeof(TurboAccelerationPusherPrototypeInfo) == 0x1a0);

    class TurboAccelerationPusher : public Gun
    {
    public:
        TurboAccelerationPusher(TurboAccelerationPusherPrototypeInfo const&);
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const;
        virtual TurboAccelerationPusherPrototypeInfo const* GetPrototypeInfo() const;

    protected:
        virtual void _LaunchShells();
        virtual ~TurboAccelerationPusher();

    private:
        virtual m3d::Object* Clone();
        virtual bool isLookAtPoint(CVector const&, float) const;
        static m3d::Object* CreateObject();

    public:
        RT_CLASS_DECLARE(TurboAccelerationPusher);
    };
}

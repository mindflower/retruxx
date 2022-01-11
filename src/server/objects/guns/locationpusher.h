#pragma once
#include "gun.h"

namespace ai
{
    class LocationPusherPrototypeInfo : public GunPrototypeInfo
    {
    public:
        LocationPusherPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
    };

    class LocationPusher : public Gun
    {
    public:
        static m3d::Class* GetBaseClass();
        LocationPusher(LocationPusherPrototypeInfo const&);
        virtual LocationPusherPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~LocationPusher();
        virtual void _LaunchShells();

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();
        virtual bool isLookAtPoint(CVector const&, float) const;

    public:
        RT_CLASS_INLINE_DECLARE(LocationPusher);
    };
}

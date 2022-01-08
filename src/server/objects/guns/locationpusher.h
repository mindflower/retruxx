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
        RT_CLASS_DECLARE(LocationPusher);

    public:
        static m3d::Class* __fastcall GetBaseClass();
        LocationPusher(LocationPusherPrototypeInfo const&);
        virtual LocationPusherPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~LocationPusher();
        virtual void _LaunchShells();

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
        virtual bool isLookAtPoint(CVector const&, float) const;
    };
}

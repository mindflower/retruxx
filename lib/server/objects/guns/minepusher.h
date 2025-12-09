#pragma once
#include "gun.h"

namespace ai
{
    class MinePusherPrototypeInfo : public GunPrototypeInfo
    {
    public:
        MinePusherPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
    };

    class MinePusher : public Gun
    {
    public:
        MinePusher(MinePusherPrototypeInfo const&);
        virtual m3d::Class* GetClass() const;
        virtual MinePusherPrototypeInfo const* GetPrototypeInfo() const;
        static m3d::Class* GetBaseClass();

    protected:
        virtual ~MinePusher();
        virtual void _LaunchShells();

    private:
        virtual bool isLookAtPoint(CVector const&, float) const;
        virtual m3d::Object* Clone();
        static m3d::Object* CreateObject();

    public:
        RT_CLASS_DECLARE(MinePusher);
    };
}

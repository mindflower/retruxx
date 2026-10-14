#pragma once
#include "gun.h"

namespace ai
{
    class LocationPusherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        LocationPusherPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override;
    };

    // A "gun" that drops a temporary location (an oil slick and the like) behind the vehicle.
    class LocationPusher : public ai::Gun
    {
        friend class LocationPusherPrototypeInfo;

    protected:
        virtual ~LocationPusher() override;

    private:
        LocationPusher(const ai::LocationPusherPrototypeInfo& prototypeInfo);
        LocationPusher(const ai::LocationPusher&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(LocationPusher);
        virtual const ai::LocationPusherPrototypeInfo* GetPrototypeInfo() const override;

    private:
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const override;

    protected:
        virtual void _LaunchShells() override;
    };
}

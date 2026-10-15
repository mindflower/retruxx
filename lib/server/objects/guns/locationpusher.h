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
        LocationPusher(ai::LocationPusherPrototypeInfo const& prototypeInfo);
        LocationPusher(ai::LocationPusher const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(LocationPusher);
        virtual ai::LocationPusherPrototypeInfo const* GetPrototypeInfo() const override;

    private:
        virtual bool isLookAtPoint(CVector const& lookAt, float eps) const override;

    protected:
        virtual void _LaunchShells() override;
    };
}  // namespace ai

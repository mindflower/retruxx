#pragma once
#include "gun.h"

namespace ai
{
    class LocationPusherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        LocationPusherPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0198 */

    static_assert(sizeof(LocationPusherPrototypeInfo) == 0x0198);

    // A "gun" that drops a temporary location (an oil slick and the like) behind the vehicle.
    class LocationPusher : public ai::Gun
    {
        friend class LocationPusherPrototypeInfo;

    protected:
        virtual ~LocationPusher() override /* 0x00 */;

    private:
        LocationPusher(const ai::LocationPusherPrototypeInfo& prototypeInfo);
        LocationPusher(const ai::LocationPusher&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(LocationPusher);
        virtual const ai::LocationPusherPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    private:
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const override /* 0x16c */;

    protected:
        virtual void _LaunchShells() override /* 0x198 */;
    }; /* size: 0x0330 */

    static_assert(sizeof(LocationPusher) == 0x0330);
}

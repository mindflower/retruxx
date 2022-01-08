#pragma once
#include "dummyobject.h"

namespace ai
{
    class VehicleSplinterPrototypeInfo : public DummyObjectPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
    };

    class VehicleSplinter : public DummyObject
    {
    public:
        RT_CLASS_DECLARE(VehicleSplinter);

    public:
        VehicleSplinter(VehicleSplinterPrototypeInfo const&);
        static m3d::Class* __fastcall GetBaseClass();
        virtual VehicleSplinterPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~VehicleSplinter();

    private:
        virtual m3d::Object* Clone();
        static m3d::Object* __fastcall CreateObject();
    };
}

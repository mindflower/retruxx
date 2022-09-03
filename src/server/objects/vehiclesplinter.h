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
        VehicleSplinter(VehicleSplinterPrototypeInfo const&);
        static m3d::Class* GetBaseClass();
        virtual VehicleSplinterPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~VehicleSplinter();

    private:
        virtual m3d::Object* Clone();
        static m3d::Object* CreateObject();

    public:
        RT_CLASS_DECLARE(VehicleSplinter);
    };
}

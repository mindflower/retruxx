#pragma once
#include "vehiclerole.h"
#include <math/vector.h>

namespace ai
{
    class VehicleRoleCowardPrototypeInfo : public VehicleRolePrototypeInfo
    {
    public:
        VehicleRoleCowardPrototypeInfo();
        virtual float FitAgainstObj(Vehicle const*, Obj const*) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const;
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*) const;
    };

    class VehicleRoleCoward : public VehicleRole
    {
    public:
        VehicleRoleCoward(VehicleRoleCowardPrototypeInfo const&);
        static m3d::Class* GetBaseClass();
        virtual void setTargetVehicle(Vehicle const*);
        virtual void setTargetTeam(Team const*);
        virtual VehicleRoleCowardPrototypeInfo const* GetPrototypeInfo() const;
        virtual void setTargetObj(Obj const*);
        virtual m3d::Class* GetClass() const;
        virtual bool UpdateVehicle(float, Vehicle*);

    protected:
        virtual ~VehicleRoleCoward();

    private:
        CVector getCowardPosition(Vehicle*);
        virtual m3d::Object* Clone();
        static m3d::Object* CreateObject();

    public:
        RT_CLASS_INLINE_DECLARE(VehicleRoleCoward);
    };
}

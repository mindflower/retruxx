#pragma once
#include "VehicleRole.h"

namespace ai
{
    class VehicleRoleBarrierPrototypeInfo : public VehicleRolePrototypeInfo
    {
    public:
        VehicleRoleBarrierPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual float FitAgainstObj(Vehicle const*, Obj const*) const;
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const;
        virtual Obj* CreateTargetObject() const;
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*) const;
    };

    class VehicleRoleBarrier : public VehicleRole
    {
    public:
        static m3d::Class m_classVehicleRoleBarrier;

    public:
        virtual m3d::Class* GetClass() const;
        VehicleRoleBarrier(VehicleRoleBarrierPrototypeInfo const&);
        virtual void setTargetVehicle(Vehicle const*);
        static m3d::Class* __fastcall GetBaseClass();
        virtual bool UpdateVehicle(float, Vehicle*);
        virtual void setTargetTeam(Team const*);
        virtual void setTargetObj(Obj const*);
        virtual VehicleRoleBarrierPrototypeInfo const* GetPrototypeInfo() const;

    protected:
        virtual ~VehicleRoleBarrier();

    private:
        static m3d::Object* __fastcall CreateObject();
        CVector getBarrierPosition(Vehicle*);
        virtual m3d::Object* Clone();
    };
}

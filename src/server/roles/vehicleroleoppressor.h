#pragma once
#include "vehiclerole.h"
#include <math/vector2.h>

namespace ai
{
    class VehicleRoleOppressorPrototypeInfo : public VehicleRolePrototypeInfo
    {
    public:
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const;
        virtual float FitAgainstObj(Vehicle const*, Obj const*) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        VehicleRoleOppressorPrototypeInfo();
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*) const;
        virtual Obj* CreateTargetObject() const;

    private:
        CVector2 m_oppressionShift;
    };

    class VehicleRoleOppressor : public VehicleRole
    {
    public:
        static m3d::Class m_classVehicleRoleOppressor;

    public:
        VehicleRoleOppressor(VehicleRoleOppressorPrototypeInfo const&);
        virtual void setTargetVehicle(Vehicle const*);
        static m3d::Class* __fastcall GetBaseClass();
        virtual bool UpdateVehicle(float, Vehicle*);
        virtual void setTargetObj(Obj const*);
        virtual void setTargetTeam(Team const*);
        virtual VehicleRoleOppressorPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~VehicleRoleOppressor();

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
        CVector getOppressorPosition(Vehicle*);
    };
}

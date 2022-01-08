#pragma once
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

namespace ai
{
    class Vehicle;
    class Team;

    class VehicleRolePrototypeInfo : public PrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        VehicleRolePrototypeInfo();
        virtual Vehicle* getBestOpponentFromTeam(Vehicle const*, Team const*) const;
        virtual Obj* CreateTargetObject() const;

    private:
        float m_vehicleFiringRangeCoeff;
    };

    class VehicleRole : public Obj
    {
    public:
        virtual bool UpdateVehicle(float, Vehicle*);
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*);
        virtual void setTargetTeam(Team const*);
        virtual void setTargetVehicle(Vehicle const*);
        static m3d::Class* GetBaseClass();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**);
        virtual float FitAgainstObj(Vehicle const*, Obj const*);
        Team* getTargetTeam() const;
        Vehicle* getTargetVehicle() const;
        virtual m3d::Class* GetClass() const;
        virtual void setTargetObj(Obj const*);
        virtual VehicleRolePrototypeInfo const* GetPrototypeInfo() const;
        VehicleRole(VehicleRolePrototypeInfo const&);
        Obj* getTargetObj() const;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    protected:
        virtual void _LookAndFireToEnemy(Vehicle*, float);
        virtual Vehicle* getBestOpponentFromTeam(Vehicle const*, Team const*);
        virtual ~VehicleRole();

    private:
        virtual m3d::Object* Clone();
        static m3d::Object* CreateObject();

    private:
        int m_TargetVehicleId;
        int m_TargetTeamId;
        int m_TargetObjId;
    };
}

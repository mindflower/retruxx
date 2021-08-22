#pragma once
#include "vehiclerole.h"
#include <math/vector2.h>

namespace ai
{
    class VehicleRolePendulumPrototypeInfo : public VehicleRolePrototypeInfo
    {
    public:
        VehicleRolePendulumPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const;
        virtual float FitAgainstObj(Vehicle const*, Obj const*) const;
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        CVector2 m_oppressionShift;
        float m_A;
        float m_B;
    };

    class VehicleRolePendulum : public VehicleRole
    {
    public:
        static inline m3d::Class m_classVehicleRolePendulum;

    public:
        VehicleRolePendulum(VehicleRolePendulumPrototypeInfo const&);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual VehicleRolePendulumPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual void setTargetObj(Obj const*);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        static m3d::Class* __fastcall GetBaseClass();
        virtual void setTargetVehicle(Vehicle const*);
        virtual void setTargetTeam(Team const*);
        virtual bool UpdateVehicle(float, Vehicle*);

    protected:
        virtual ~VehicleRolePendulum();

    private:
        CVector getPendulumPosition(Vehicle*, float);
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
        CVector2 m_Direction;
        float m_angle;
    };
}

#pragma once
#include "vehiclerole.h"
#include <math/vector.h>
#include <math/vector2.h>

namespace ai
{
    class VehicleRolePendulumPrototypeInfo : public ai::VehicleRolePrototypeInfo
    {
    public:
        VehicleRolePendulumPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        virtual float FitAgainstVehicle(ai::Vehicle const* v, ai::Vehicle const* target) const override /* 0x1c */;
        virtual float FitAgainstTeam(ai::Vehicle const* v, ai::Team const* target, ai::Vehicle** targetVehicle) const override /* 0x20 */;
        virtual float FitAgainstObj(ai::Vehicle const* v, ai::Obj const* obj) const override /* 0x24 */;
        /* 0x0044 */ CVector2 m_oppressionShift;
        /* 0x004c */ float m_A;
        /* 0x0050 */ float m_B;
    }; /* size: 0x0054 */

    class VehicleRolePendulum : public VehicleRole
    {
    public:
        VehicleRolePendulum(VehicleRolePendulumPrototypeInfo const&);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual VehicleRolePendulumPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual void setTargetObj(Obj const*);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        static m3d::Class* GetBaseClass();
        virtual void setTargetVehicle(Vehicle const*);
        virtual void setTargetTeam(Team const*);
        virtual bool UpdateVehicle(float, Vehicle*);

    protected:
        virtual ~VehicleRolePendulum();

    private:
        CVector getPendulumPosition(Vehicle*, float);
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(VehicleRolePendulum);

    private:
        CVector2 m_Direction;
        float m_angle;
    };
}

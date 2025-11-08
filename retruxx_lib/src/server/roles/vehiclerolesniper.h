#pragma once
#include "vehiclerole.h"
#include <math/point2d.h>
#include <math/vector.h>

namespace ai
{
    class VehicleRoleSniperPrototypeInfo : public VehicleRolePrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        VehicleRoleSniperPrototypeInfo();
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*) const;
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual float FitAgainstObj(Vehicle const*, Obj const*) const;
    };

    class VehicleRoleSniper : public VehicleRole
    {
    public:
        enum SniperState
        {
            ssIdle = 0x0,
            ssMovingToPeak = 0x1,
            ssFiring = 0x2,
        };

    public:
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual m3d::Class* GetClass() const;
        virtual bool UpdateVehicle(float, Vehicle*);
        virtual VehicleRoleSniperPrototypeInfo const* GetPrototypeInfo() const;
        virtual void setTargetObj(Obj const*);
        virtual void setTargetTeam(Team const*);
        static m3d::Class* GetBaseClass();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void setTargetVehicle(Vehicle const*);
        VehicleRoleSniper(VehicleRoleSniperPrototypeInfo const&);

    protected:
        virtual ~VehicleRoleSniper();

    private:
        bool CurrentPositionIsBad(Vehicle const*);
        virtual m3d::Object* Clone();
        PointBase<float> getPeakPositionInt(Vehicle const*, PointBase<float> const&, PointBase<float> const&, float);
        static m3d::Object* CreateObject();
        CVector getPeakPosition(Vehicle*);
        std::vector<int, std::allocator<int> > getTargetObjs() const;
        virtual void _LookAndFireToEnemy(Vehicle*, float);
        bool PositionIsBad(Vehicle const*, CVector const&);

    public:
        RT_CLASS_DECLARE(VehicleRoleSniper);

    private:
        SniperState m_SniperState;
        CVector m_Peak;
        float m_DeniedHeight;
        float m_LastChange;
    };
}

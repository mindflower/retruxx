#pragma once
#include "vehiclerole.h"
#include <math/vector.h>

namespace ai
{
    class ChaseMotionTactics;

    class VehicleRoleCheaterPrototypeInfo : public VehicleRolePrototypeInfo
    {
    public:
        VehicleRoleCheaterPrototypeInfo();
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const;
        virtual float FitAgainstObj(Vehicle const*, Obj const*) const;
        virtual Obj* CreateTargetObject() const;
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

    class VehicleRoleCheater : public VehicleRole
    {
    public:
        static m3d::Class* GetBaseClass();
        virtual void setTargetVehicle(Vehicle const*);
        VehicleRoleCheater(VehicleRoleCheaterPrototypeInfo const&);
        virtual bool UpdateVehicle(float, Vehicle*);
        virtual void setTargetTeam(Team const*);
        virtual void setTargetObj(Obj const*);
        virtual VehicleRoleCheaterPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;

    protected:
        virtual ~VehicleRoleCheater();

    private:
        ChaseMotionTactics* CreateChaseMotionTactic(Vehicle const*) const;
        static m3d::Object* CreateObject();
        CVector _EvaluateChasePointToMove(float);
        virtual m3d::Object* Clone();
        void _CreateChaseTacticsIfNeeded(Vehicle*);

    public:
        RT_CLASS_INLINE_DECLARE(VehicleRoleCheater);

    private:
        ChaseMotionTactics* m_chaseTactics;
        int m_chaseTargetId;
        bool m_needCreateChaseTactics;
    };
}

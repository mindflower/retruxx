#pragma once
#include "vehiclerole.h"
#include <math/vector.h>

namespace ai
{
    class ChaseMotionTactics;

    class VehicleRoleMeatPrototypeInfo : public VehicleRolePrototypeInfo
    {
    public:
        VehicleRoleMeatPrototypeInfo();
        virtual float FitAgainstVehicle(Vehicle const*, Vehicle const*) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        virtual float FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const;
        virtual float FitAgainstObj(Vehicle const*, Obj const*) const;
    };

    class VehicleRoleMeat : public VehicleRole
    {
    public:
        RT_CLASS_DECLARE(VehicleRoleMeat);

    public:
        static m3d::Class* __fastcall GetBaseClass();
        VehicleRoleMeat(VehicleRoleMeatPrototypeInfo const&);
        virtual void setTargetTeam(Team const*);
        virtual void setTargetVehicle(Vehicle const*);
        virtual bool UpdateVehicle(float, Vehicle*);
        virtual void setTargetObj(Obj const*);
        virtual m3d::Class* GetClass() const;
        virtual VehicleRoleMeatPrototypeInfo const* GetPrototypeInfo() const;
    protected:
        virtual ~VehicleRoleMeat();

    private:
        CVector _EvaluateChasePointToMove(float);
        virtual m3d::Object* Clone();
        void _CreateChaseTacticsIfNeeded(Vehicle*);
        ChaseMotionTactics* CreateChaseMotionTactic(Vehicle const*) const;
        static m3d::Object* __fastcall CreateObject();
        ChaseMotionTactics* m_chaseTactics;
        int m_chaseTargetId;
        bool m_needCreateChaseTactics;
    };
}

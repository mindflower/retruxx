#pragma once
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

namespace ai
{
    class Vehicle;
    class Team;

    class VehicleRolePrototypeInfo : public ai::PrototypeInfo
    {
    public:
        float m_vehicleFiringRangeCoeff;
        VehicleRolePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        virtual float FitAgainstVehicle(ai::Vehicle const*, ai::Vehicle const*) const = 0;
        virtual float FitAgainstTeam(ai::Vehicle const*, ai::Team const*, ai::Vehicle**) const = 0;
        virtual float FitAgainstObj(ai::Vehicle const*, ai::Obj const*) const = 0;
        virtual ai::Vehicle* getBestOpponentFromTeam(ai::Vehicle const* v, ai::Team const* target) const;
    };

    class VehicleRole : public ai::Obj
    {
    protected:
        virtual ~VehicleRole() override;
        VehicleRole(ai::VehicleRolePrototypeInfo const& prototypeInfo);
        VehicleRole(ai::VehicleRole const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classVehicleRole;
        virtual ai::VehicleRolePrototypeInfo const* GetPrototypeInfo() const override;
        virtual float FitAgainstVehicle(ai::Vehicle const* v, ai::Vehicle const* target);
        virtual float FitAgainstTeam(ai::Vehicle const* v, ai::Team const* target, ai::Vehicle** targetVehicle);
        virtual float FitAgainstObj(ai::Vehicle const* v, ai::Obj const* obj);
        virtual bool UpdateVehicle(float elapsedTime, ai::Vehicle* v) = 0;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        ai::Vehicle* getTargetVehicle() const;
        virtual void setTargetVehicle(ai::Vehicle const* vehicle);
        ai::Team* getTargetTeam() const;
        virtual void setTargetTeam(ai::Team const* team);
        ai::Obj* getTargetObj() const;
        virtual void setTargetObj(ai::Obj const* obj);

    protected:
        virtual void _LookAndFireToEnemy(ai::Vehicle* v, float elapsedTime);
        virtual ai::Vehicle* getBestOpponentFromTeam(ai::Vehicle const* v, ai::Team const* target);

    private:
        int m_TargetVehicleId;
        int m_TargetTeamId;
        int m_TargetObjId;
    };
}  // namespace ai

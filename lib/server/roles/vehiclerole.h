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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        virtual float FitAgainstVehicle(const ai::Vehicle*, const ai::Vehicle*) const = 0;
        virtual float FitAgainstTeam(const ai::Vehicle*, const ai::Team*, ai::Vehicle**) const = 0;
        virtual float FitAgainstObj(const ai::Vehicle*, const ai::Obj*) const = 0;
        virtual ai::Vehicle* getBestOpponentFromTeam(const ai::Vehicle* v, const ai::Team* target) const;
    };

    class VehicleRole : public ai::Obj
    {
    protected:
        virtual  ~VehicleRole() override;
        VehicleRole(const ai::VehicleRolePrototypeInfo& prototypeInfo);
        VehicleRole(const ai::VehicleRole&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classVehicleRole;
        virtual const ai::VehicleRolePrototypeInfo* GetPrototypeInfo() const override;
        virtual float FitAgainstVehicle(const ai::Vehicle* v, const ai::Vehicle* target);
        virtual float FitAgainstTeam(const ai::Vehicle* v, const ai::Team* target, ai::Vehicle** targetVehicle);
        virtual float FitAgainstObj(const ai::Vehicle* v, const ai::Obj* obj);
        virtual bool UpdateVehicle(float elapsedTime, ai::Vehicle* v) = 0;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        ai::Vehicle* getTargetVehicle() const;
        virtual void setTargetVehicle(const ai::Vehicle* vehicle);
        ai::Team* getTargetTeam() const;
        virtual void setTargetTeam(const ai::Team* team);
        ai::Obj* getTargetObj() const;
        virtual void setTargetObj(const ai::Obj* obj);

    protected:
        virtual void _LookAndFireToEnemy(ai::Vehicle* v, float elapsedTime);
        virtual ai::Vehicle* getBestOpponentFromTeam(const ai::Vehicle* v, const ai::Team* target);

    private:
        int m_TargetVehicleId;
        int m_TargetTeamId;
        int m_TargetObjId;
    };
}

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
        /* 0x0040 */ float m_vehicleFiringRangeCoeff;
        VehicleRolePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        virtual float FitAgainstVehicle(const ai::Vehicle*, const ai::Vehicle*) const = 0 /* 0x1c */;
        virtual float FitAgainstTeam(const ai::Vehicle*, const ai::Team*, ai::Vehicle**) const = 0 /* 0x20 */;
        virtual float FitAgainstObj(const ai::Vehicle*, const ai::Obj*) const = 0 /* 0x24 */;
        virtual ai::Vehicle* getBestOpponentFromTeam(const ai::Vehicle* v, const ai::Team* target) const /* 0x28 */;
    }; /* size: 0x0044 */

    class VehicleRole : public ai::Obj
    {
    protected:
        virtual  ~VehicleRole() override = 0 /* 0x00 */;
        VehicleRole(const ai::VehicleRolePrototypeInfo& prototypeInfo);
        VehicleRole(const ai::VehicleRole&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classVehicleRole;
        virtual const ai::VehicleRolePrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual float FitAgainstVehicle(const ai::Vehicle* v, const ai::Vehicle* target) /* 0x110 */;
        virtual float FitAgainstTeam(const ai::Vehicle* v, const ai::Team* target, ai::Vehicle** targetVehicle) /* 0x114 */;
        virtual float FitAgainstObj(const ai::Vehicle* v, const ai::Obj* obj) /* 0x118 */;
        virtual bool UpdateVehicle(float elapsedTime, ai::Vehicle* v) = 0 /* 0x11c */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        ai::Vehicle* getTargetVehicle() const;
        virtual void setTargetVehicle(const ai::Vehicle* vehicle) /* 0x120 */;
        ai::Team* getTargetTeam() const;
        virtual void setTargetTeam(const ai::Team* team) /* 0x124 */;
        ai::Obj* getTargetObj() const;
        virtual void setTargetObj(const ai::Obj* obj) /* 0x128 */;

    protected:
        virtual void _LookAndFireToEnemy(ai::Vehicle* v, float elapsedTime) /* 0x12c */;
        virtual ai::Vehicle* getBestOpponentFromTeam(const ai::Vehicle* v, const ai::Team* target) /* 0x130 */;

    private:
        /* 0x00c0 */ int m_TargetVehicleId;
        /* 0x00c4 */ int m_TargetTeamId;
        /* 0x00c8 */ int m_TargetObjId;
    }; /* size: 0x00cc */
}

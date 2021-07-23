#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

struct dContact;

namespace ai
{
    class Boss04PartPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    };

    class Boss04Part : public VehiclePart
    {
    public:
        static m3d::Class m_classBoss04Part;

    public:
        static m3d::Class* __fastcall GetBaseClass();
        Boss04Part(Boss04PartPrototypeInfo const&);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Boss04PartPrototypeInfo const* GetPrototypeInfo() const;
        static int __fastcall CollideBoss04PartAndVehiclePart(Boss04Part*, VehiclePart*, dContact*, unsigned int&, bool);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual m3d::Class* GetClass() const;

    protected:
        virtual bool _OnDurabilityValueBeforeApplyModifier(Modifier const&, float&);
        virtual ~Boss04Part();
        virtual void _OnDurabilityValueAfterChange(float);

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
        bool m_bIsDamageable;
    };
}

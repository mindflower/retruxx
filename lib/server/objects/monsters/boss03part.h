#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

struct dContact;

namespace ai
{
    class Boss03PartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Boss03Part : public ai::VehiclePart
    {
        friend class Boss03PartPrototypeInfo;

    protected:
        virtual ~Boss03Part() override;

    private:
        Boss03Part(ai::Boss03PartPrototypeInfo const& prototypeInfo);
        Boss03Part(ai::Boss03Part const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss03Part;
        virtual ai::Boss03PartPrototypeInfo const* GetPrototypeInfo() const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        bool bIsDamageable() const;
        void SetDamageable(bool damageable);
        static int __fastcall CollideBoss03PartAndVehiclePart(
            ai::Boss03Part* boss03Part,
            ai::VehiclePart* vp,
            dContact* contact,
            unsigned int& numContacts,
            bool reverse);

    protected:
        virtual bool _OnDurabilityValueBeforeApplyModifier(ai::Modifier const& modifier, float& newDurabilityValue)
            override;
        virtual void _OnDurabilityValueAfterChange(float oldDurabilityValue) override;

    private:
        bool m_bIsDamageable;
    };
}  // namespace ai

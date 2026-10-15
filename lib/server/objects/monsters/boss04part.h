#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

struct dContact;

namespace ai
{
    class Boss04PartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Boss04Part : public ai::VehiclePart
    {
        friend class Boss04PartPrototypeInfo;

    protected:
        virtual ~Boss04Part() override;

    private:
        Boss04Part(ai::Boss04PartPrototypeInfo const& prototypeInfo);
        Boss04Part(ai::Boss04Part const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss04Part;
        virtual ai::Boss04PartPrototypeInfo const* GetPrototypeInfo() const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        bool bIsDamageable() const;
        void SetDamageable(bool);
        static int CollideBoss04PartAndVehiclePart(
            ai::Boss04Part* boss04Part,
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

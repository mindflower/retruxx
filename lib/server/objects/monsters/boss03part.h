#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

struct dContact;

namespace ai
{
    class Boss03PartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0110 */

    static_assert(sizeof(Boss03PartPrototypeInfo) == 0x0110);

    class Boss03Part : public ai::VehiclePart
    {
        friend class Boss03PartPrototypeInfo;

    protected:
        virtual ~Boss03Part() override /* 0x00 */;

    private:
        Boss03Part(const ai::Boss03PartPrototypeInfo& prototypeInfo);
        Boss03Part(const ai::Boss03Part&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss03Part;
        virtual const ai::Boss03PartPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        bool bIsDamageable() const;
        void SetDamageable(bool damageable);
        static int __fastcall CollideBoss03PartAndVehiclePart(ai::Boss03Part* boss03Part, ai::VehiclePart* vp, dContact* contact, unsigned int& numContacts, bool reverse);

    protected:
        virtual bool _OnDurabilityValueBeforeApplyModifier(const ai::Modifier& modifier, float& newDurabilityValue) override /* 0x160 */;
        virtual void _OnDurabilityValueAfterChange(float oldDurabilityValue) override /* 0x164 */;

    private:
        /* 0x02c8 */ bool m_bIsDamageable;
    }; /* size: 0x02cc */

    static_assert(sizeof(Boss03Part) == 0x02cc);
}

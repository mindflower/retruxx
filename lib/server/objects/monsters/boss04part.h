#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

struct dContact;

namespace ai
{
    class Boss04PartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0110 */

    static_assert(sizeof(Boss04PartPrototypeInfo) == 0x0110);

    class Boss04Part : public ai::VehiclePart
    {
        friend class Boss04PartPrototypeInfo;

    protected:
        virtual ~Boss04Part() override /* 0x00 */;

    private:
        Boss04Part(const ai::Boss04PartPrototypeInfo& prototypeInfo);
        Boss04Part(const ai::Boss04Part&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss04Part;
        virtual const ai::Boss04PartPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        bool bIsDamageable() const;
        void SetDamageable(bool);
        static int __fastcall CollideBoss04PartAndVehiclePart(ai::Boss04Part* boss04Part, ai::VehiclePart* vp, dContact* contact, unsigned int& numContacts, bool reverse);

    protected:
        virtual bool _OnDurabilityValueBeforeApplyModifier(const ai::Modifier& modifier, float& newDurabilityValue) override /* 0x160 */;
        virtual void _OnDurabilityValueAfterChange(float oldDurabilityValue) override /* 0x164 */;

    private:
        /* 0x02c8 */ bool m_bIsDamageable;
    }; /* size: 0x02cc */

    static_assert(sizeof(Boss04Part) == 0x02cc);
}

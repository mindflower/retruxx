#pragma once
#include "vehiclepart.h"

namespace ai
{
    class CompoundVehiclePartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        struct TPartInfo;
        using StrPartInfoMap = retruxx::map<CStr, ai::CompoundVehiclePartPrototypeInfo::TPartInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePartPrototypeInfo::TPartInfo> > >;
        class StrPartInfoMapConstIterator;
        class StrPartInfoMapIterator;

    public:
        /* 0x0110 */ retruxx::map<CStr, ai::CompoundVehiclePartPrototypeInfo::TPartInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePartPrototypeInfo::TPartInfo> > > m_PartInfo;
        CompoundVehiclePartPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
    }; /* size: 0x011c */

    static_assert(sizeof(CompoundVehiclePartPrototypeInfo) == 0x011c);

    class CompoundVehiclePart : public ai::VehiclePart
    {
    protected:
        virtual  ~CompoundVehiclePart() override = 0 /* 0x00 */;

    protected:
        CompoundVehiclePart(const ai::CompoundVehiclePartPrototypeInfo& prototypeInfo);
        CompoundVehiclePart(const ai::CompoundVehiclePart&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classCompoundVehiclePart;
        virtual const ai::CompoundVehiclePartPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

        struct TVehiclePart;

    public:
        virtual void SetVisible() override /* 0x00 */;
        virtual void SetInvisible() override /* 0x00 */;
        virtual void SetBelong(int newBelong) override /* 0x00 */;
        virtual void Remove() override /* 0x00 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x00 */;
        virtual void CreateChildren() override /* 0x00 */;
        virtual void SetOwner(ai::PhysicObj* owner) override /* 0x00 */;
        virtual void RelinkToSpace(dxSpace* newSpace) override /* 0x00 */;
        virtual float GetMass() const override /* 0x00 */;
        float GetDurability() const;
        float GetMaxDurability() const;
        void SetDurability(float durability);
        void RegenerateDurability(float elapsedTime);
        void SetDurabilityRegeneration(float value);
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override /* 0x00 */;

        using VehiclePartsMap = retruxx::map<CStr, ai::CompoundVehiclePart::TVehiclePart, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart> > >;
        using VehiclePartsMapValueType = retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart>;

    protected:
        /* 0x02c8 */ retruxx::map<CStr, ai::CompoundVehiclePart::TVehiclePart, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart> > > m_vehicleParts;
        virtual void _InternalCreateVisualPart() override /* 0x00 */;
        virtual void _InternalPostLoad() override /* 0x00 */;

        class iterator;
        class const_iterator;

    public:
        VehiclePartsMap::const_iterator begin() const;
        VehiclePartsMap::iterator begin();
        VehiclePartsMap::const_iterator end() const;
        VehiclePartsMap::iterator end();
        VehiclePartsMap::const_iterator find(const CStr&) const;
        VehiclePartsMap::iterator find(const CStr& s);

        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        virtual void ClearSavedStatus() override /* 0x00 */;
    }; /* size: 0x02d4 */

    static_assert(sizeof(CompoundVehiclePart) == 0x02d4);
}

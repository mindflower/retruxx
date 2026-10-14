#pragma once
#include "vehiclepart.h"

namespace ai
{
    class CompoundVehiclePartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        struct TPartInfo
        {
            int prototypeId;
            CStr prototypeName;
            unsigned int index;
        };

        using StrPartInfoMap = retruxx::map<CStr, ai::CompoundVehiclePartPrototypeInfo::TPartInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePartPrototypeInfo::TPartInfo> > >;
        class StrPartInfoMapConstIterator;
        class StrPartInfoMapIterator;

    public:
        retruxx::map<CStr, ai::CompoundVehiclePartPrototypeInfo::TPartInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePartPrototypeInfo::TPartInfo> > > m_PartInfo;
        CompoundVehiclePartPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
    };

    class CompoundVehiclePart : public ai::VehiclePart
    {
    protected:
        virtual  ~CompoundVehiclePart() override/* 0x00 */;

    protected:
        CompoundVehiclePart(const ai::CompoundVehiclePartPrototypeInfo& prototypeInfo);
        CompoundVehiclePart(const ai::CompoundVehiclePart&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

        // GadgetPrototypeInfo::ApplyToVp passes a modification on to every part of a compound gun.
        friend class GadgetPrototypeInfo;

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classCompoundVehiclePart;
        virtual const ai::CompoundVehiclePartPrototypeInfo* GetPrototypeInfo() const override;

        struct TVehiclePart
        {
            ai::VehiclePart* vp;
            unsigned int index;
            TVehiclePart(ai::VehiclePart* _vp, unsigned int _index);
            TVehiclePart();
        };

    public:
        virtual void SetVisible() override;
        virtual void SetInvisible() override;
        virtual void SetBelong(int newBelong) override;
        virtual void Remove() override;
        virtual void SetPassedToAnotherMapStatus() override;
        virtual void CreateChildren() override;
        virtual void SetOwner(ai::PhysicObj* owner) override;
        virtual void RelinkToSpace(dxSpace* newSpace) override;
        virtual float GetMass() const override;
        float GetDurability() const;
        float GetMaxDurability() const;
        void SetDurability(float durability);
        void RegenerateDurability(float elapsedTime);
        void SetDurabilityRegeneration(float value);
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override;

        using VehiclePartsMap = retruxx::map<CStr, ai::CompoundVehiclePart::TVehiclePart, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart> > >;
        using VehiclePartsMapValueType = retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart>;

    protected:
        retruxx::map<CStr, ai::CompoundVehiclePart::TVehiclePart, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart> > > m_vehicleParts;
        virtual void _InternalCreateVisualPart() override;
        virtual void _InternalPostLoad() override;

        class iterator;
        class const_iterator;

    public:
        VehiclePartsMap::const_iterator begin() const;
        VehiclePartsMap::iterator begin();
        VehiclePartsMap::const_iterator end() const;
        VehiclePartsMap::iterator end();
        VehiclePartsMap::const_iterator find(const CStr&) const;
        VehiclePartsMap::iterator find(const CStr& s);

        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pObj) override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void RenderDebugInfo() const override;
        virtual void ClearSavedStatus() override;
    };
}

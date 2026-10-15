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

        using StrPartInfoMap = retruxx::map<
            CStr,
            ai::CompoundVehiclePartPrototypeInfo::TPartInfo,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePartPrototypeInfo::TPartInfo>>>;
        class StrPartInfoMapConstIterator;
        class StrPartInfoMapIterator;

    public:
        retruxx::map<
            CStr,
            ai::CompoundVehiclePartPrototypeInfo::TPartInfo,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePartPrototypeInfo::TPartInfo>>>
            m_PartInfo;
        CompoundVehiclePartPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
    };

    class CompoundVehiclePart : public ai::VehiclePart
    {
    protected:
        virtual ~CompoundVehiclePart() override /* 0x00 */;

    protected:
        CompoundVehiclePart(ai::CompoundVehiclePartPrototypeInfo const& prototypeInfo);
        CompoundVehiclePart(ai::CompoundVehiclePart const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

        // GadgetPrototypeInfo::ApplyToVp passes a modification on to every part of a compound gun.
        friend class GadgetPrototypeInfo;

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classCompoundVehiclePart;
        virtual ai::CompoundVehiclePartPrototypeInfo const* GetPrototypeInfo() const override;

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
        virtual unsigned int GetPrice(ai::IPriceCoeffProvider const* priceCoeffProvider) const override;

        using VehiclePartsMap = retruxx::map<
            CStr,
            ai::CompoundVehiclePart::TVehiclePart,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart>>>;
        using VehiclePartsMapValueType = retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart>;

    protected:
        retruxx::map<
            CStr,
            ai::CompoundVehiclePart::TVehiclePart,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, ai::CompoundVehiclePart::TVehiclePart>>>
            m_vehicleParts;
        virtual void _InternalCreateVisualPart() override;
        virtual void _InternalPostLoad() override;

        class iterator;
        class const_iterator;

    public:
        VehiclePartsMap::const_iterator begin() const;
        VehiclePartsMap::iterator begin();
        VehiclePartsMap::const_iterator end() const;
        VehiclePartsMap::iterator end();
        VehiclePartsMap::const_iterator find(CStr const&) const;
        VehiclePartsMap::iterator find(CStr const& s);

        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pObj) override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void RenderDebugInfo() const override;
        virtual void ClearSavedStatus() override;
    };
}  // namespace ai

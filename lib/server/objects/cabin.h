#pragma once
#include "physicbodies/vehiclepart.h"
#include <math/point2d.h>

namespace ai
{
    class CabinPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
        using GadgetSlotsMap = retruxx::map<CStr, PointBase<int>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, PointBase<int> > > >;
        using GadgetSlotsPair = retruxx::pair<CStr, PointBase<int> >;

    public:
        CabinPrototypeInfo();
        float m_maxPower;
        float m_maxTorque;
        float m_maxSpeed;
        float m_fuelConsumption;
        retruxx::map<CStr, PointBase<int>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, PointBase<int> > > > m_gadgetSlots;
        float m_control;
        CStr m_engineHighSoundName;
        CStr m_engineLowSoundName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        int GetMaxGadgets(const CStr& gadgetResourceName) const;
    };

    class Cabin : public ai::VehiclePart
    {
        friend class CabinPrototypeInfo;
    protected:
        virtual  ~Cabin() override;

    private:
        Cabin(const ai::CabinPrototypeInfo& prototypeInfo);
        Cabin(const ai::Cabin&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classCabin;
        virtual const ai::CabinPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        float GetMaxSpeed() const;
        void SetMaxSpeed(float speed);
        float GetFuelConsumption() const;
        float GetMaxPower() const;
        void SetMaxPower(float maxPower);
        float GetMaxTorque() const;
        void SetMaxTorque(float maxTorque);
        int GetMaxGadgets(const CStr& gadgetResourceName) const;
        static void __fastcall Registration();
        float GetControl() const;

    private:
        float m_maxPower;
        float m_maxTorque;
        float m_maxSpeed;
        float m_fuelConsumption;
        int m_maxGadgets;
        float m_control;
    };
}

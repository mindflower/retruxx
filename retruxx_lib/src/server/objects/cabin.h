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
        /* 0x0110 */ float m_maxPower;
        /* 0x0114 */ float m_maxTorque;
        /* 0x0118 */ float m_maxSpeed;
        /* 0x011c */ float m_fuelConsumption;
        /* 0x0120 */ retruxx::map<CStr, PointBase<int>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, PointBase<int> > > > m_gadgetSlots;
        /* 0x012c */ float m_control;
        /* 0x0130 */ CStr m_engineHighSoundName;
        /* 0x013c */ CStr m_engineLowSoundName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        int GetMaxGadgets(const CStr& gadgetResourceName) const;
    }; /* size: 0x0148 */

    static_assert(sizeof(CabinPrototypeInfo) == 0x0148);

    class Cabin : public ai::VehiclePart
    {
    protected:
        virtual  ~Cabin() override /* 0x00 */;

    private:
        Cabin(const ai::CabinPrototypeInfo& prototypeInfo);
        Cabin(const ai::Cabin&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classCabin;
        virtual const ai::CabinPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

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
        /* 0x02c8 */ float m_maxPower;
        /* 0x02cc */ float m_maxTorque;
        /* 0x02d0 */ float m_maxSpeed;
        /* 0x02d4 */ float m_fuelConsumption;
        /* 0x02d8 */ int m_maxGadgets;
        /* 0x02dc */ float m_control;
    }; /* size: 0x02e0 */

    static_assert(sizeof(Cabin) == 0x02e0);
}

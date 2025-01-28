#pragma once
#include "physicbodies/vehiclepart.h"

namespace ai
{
    class ChassisPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        ChassisPrototypeInfo();
        /* 0x0110 */ float m_maxHealth;
        /* 0x0114 */ float m_maxFuel;
        /* 0x0118 */ CStr m_brakingSoundName;
        /* 0x0124 */ CStr m_pneumoSoundName;
        /* 0x0130 */ CStr m_gearShiftSoundName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x013c */

    static_assert(sizeof(ChassisPrototypeInfo) == 0x013c);

    class Chassis : public ai::VehiclePart
    {
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Chassis, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Chassis, ai::Modifier, float, bool>;

    protected:
        virtual  ~Chassis() override /* 0x00 */;

    private:
        Chassis(const ai::ChassisPrototypeInfo& prototypeInfo);
        Chassis(const ai::Chassis&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classChassis;
        virtual const ai::ChassisPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

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
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        const ai::NumericInRangeRegenerating<float>& Health() const;
        ai::NumericInRangeRegenerating<float>& Health();
        const ai::NumericInRangeRegenerating<float>& Fuel() const;
        ai::NumericInRangeRegenerating<float>& Fuel();
        static void __fastcall Registration();

    private:
        /* 0x02c8 */ ai::NumericInRangeRegenerating<float> m_health;
        /* 0x03a0 */ ai::NumericInRangeRegenerating<float> m_fuel;
        bool _OnHealthValueBeforeApplyModifier(const ai::Modifier& modifier, float& newHealth);
    }; /* size: 0x0478 */

    static_assert(sizeof(Chassis) == 0x0478);
}

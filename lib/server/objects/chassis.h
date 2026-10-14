#pragma once
#include "physicbodies/vehiclepart.h"

namespace ai
{
    class ChassisPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        ChassisPrototypeInfo();
        float m_maxHealth;
        float m_maxFuel;
        CStr m_brakingSoundName;
        CStr m_pneumoSoundName;
        CStr m_gearShiftSoundName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Chassis : public ai::VehiclePart
    {
        friend class ChassisPrototypeInfo;
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Chassis, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Chassis, ai::Modifier, float, bool>;

    protected:
        virtual ~Chassis() override;

    private:
        Chassis(ai::ChassisPrototypeInfo const& prototypeInfo);
        Chassis(ai::Chassis const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classChassis;
        virtual ai::ChassisPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        ai::NumericInRangeRegenerating<float> const& Health() const;
        ai::NumericInRangeRegenerating<float>& Health();
        ai::NumericInRangeRegenerating<float> const& Fuel() const;
        ai::NumericInRangeRegenerating<float>& Fuel();
        static void __fastcall Registration();

    private:
        ai::NumericInRangeRegenerating<float> m_health;
        ai::NumericInRangeRegenerating<float> m_fuel;
        bool _OnHealthValueBeforeApplyModifier(ai::Modifier const& modifier, float& newHealth);
    };
}  // namespace ai

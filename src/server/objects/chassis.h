#pragma once
#include "physicbodies/vehiclepart.h"

namespace ai
{
    class ChassisPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        ChassisPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        float m_maxHealth;
        float m_maxFuel;
        CStr m_brakingSoundName;
        CStr m_pneumoSoundName;
        CStr m_gearShiftSoundName;
    };

    class Chassis :  public VehiclePart
    {
    public:
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        static m3d::Class * GetBaseClass();
        Chassis(ChassisPrototypeInfo const &);
        NumericInRangeRegenerating<float> const & Health() const ;
        NumericInRangeRegenerating<float> & Health();
        virtual int GetPropertyId(char const *) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        static void __fastcall Registration();
        NumericInRangeRegenerating<float> const & Fuel() const ;
        NumericInRangeRegenerating<float> & Fuel();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual ChassisPrototypeInfo const * GetPrototypeInfo() const ;
        virtual m3d::Class * GetClass() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
    protected:

        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual ~Chassis();

    private:
        virtual m3d::Object * Clone();
        bool _OnHealthValueBeforeApplyModifier(Modifier const &,float &);
        //MemberFunctionTwoArgsRef<Chassis,Modifier,float,bool>::MemberFunctionTwoArgsRef<Chassis,Modifier,float,bool>(Chassis &,bool (*const)(Modifier const &,float &));
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_INLINE_DECLARE(Chassis);

    private:
        NumericInRangeRegenerating<float> m_health;
        NumericInRangeRegenerating<float> m_fuel;
    };
}

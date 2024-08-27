#pragma once
#include "physicbodies/vehiclepart.h"
#include <math/point2d.h>

namespace ai
{
    class CabinPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        CabinPrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        int GetMaxGadgets(CStr const&) const;

    private:
        float m_maxPower;
        float m_maxTorque;
        float m_maxSpeed;
        float m_fuelConsumption;
        std::map<CStr, PointBase<int>> m_gadgetSlots;
        float m_control;
        CStr m_engineHighSoundName;
        CStr m_engineLowSoundName;
    };

    class Cabin :  public VehiclePart
    {
    public:
        float GetMaxSpeed() const ;
        void SetMaxPower(float);
        float GetFuelConsumption() const ;
        float GetMaxTorque() const ;
        Cabin(CabinPrototypeInfo const &);
        void SetMaxSpeed(float);
        float GetMaxPower() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual m3d::Class * GetClass() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        float GetControl() const ;
        void SetMaxTorque(float);
        virtual CabinPrototypeInfo const * GetPrototypeInfo() const ;
        static m3d::Class * GetBaseClass();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        int GetMaxGadgets(CStr const &) const ;

    protected:
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~Cabin();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(Cabin);

    private:
        float m_maxPower;
        float m_maxTorque;
        float m_maxSpeed;
        float m_fuelConsumption;
        int m_maxGadgets;
        float m_control;
    };
}

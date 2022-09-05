#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include "guns/gun.h"
#include <core/aiparam.h>

namespace ai
{
    class VehiclePart;
    class Vehicle;

    class GadgetPrototypeInfo : public PrototypeInfo
    {
    public:
        enum GadgetAppliers
        {
            GA_VEHICLE = 0x0,
            GA_OBJECT_BY_RESOURCE = 0x1,
            GA_GUN_BY_TYPE = 0x2,
        };

        class GadgetApplicationInfo
        {
        public:
            GadgetApplicationInfo();

        private:
            GadgetAppliers applierType;
            int targetResourceId;
            ai::FiringTypes targetFiringType;
        };

        class ModificationInfo
        {
        public:
            enum ModificationType
            {
                MULTIPLY = 0x0,
                ADD = 0x1,
            };

        public:
            bool ApplyToObj(ai::Obj*, bool) const;
            ModificationInfo(CStr const&, ai::GadgetPrototypeInfo const*);

        private:
            GadgetApplicationInfo m_applierInfo;
            CStr m_propertyName;
            ModificationType m_modificationType;
            m3d::AIParam m_value;
        };

    public:
        virtual ai::Obj* CreateTargetObject() const;
        bool ApplyToVp(ai::VehiclePart*, bool) const;
        GadgetPrototypeInfo();
        int GetSkinNum() const;
        CStr const& GetModelName() const;
        virtual bool ApplyToVehicle(ai::Vehicle*, bool) const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        std::vector<ModificationInfo, std::allocator<ModificationInfo> > const& GetModifications() const;

    private:
        std::vector<ai::GadgetPrototypeInfo::ModificationInfo> m_modifications;
        CStr m_modelName;
        int m_skinNum;
    };

    class Gadget :  public Obj
    {
    public:
        virtual int GetPropertyId(char const *) const ;
        int GetSlotNum() const ;
        void SetSlotNum(int);
        virtual GadgetPrototypeInfo const * GetPrototypeInfo() const ;
        Gadget(GadgetPrototypeInfo const &);
        bool ApplyToVehicle(Vehicle *,bool) const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        static m3d::Class * GetBaseClass();
        bool ApplyToVp(VehiclePart *,bool) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual m3d::Class * GetClass() const ;
        static void __fastcall Registration();
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;

    protected:
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~Gadget();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(Gadget);

    private:
        int m_slotNum;
    };
}

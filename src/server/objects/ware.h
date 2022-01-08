#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <server/components/numericinrange.h>

namespace ai
{
    class WarePrototypeInfo : public PrototypeInfo
    {
    public:
        CStr const& GetModelName() const;
        WarePrototypeInfo();
        int GetMinCount() const;
        float GetPriceDispersion() const;
        int GetMaxCount() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        unsigned int m_maxItems;
        float m_maxDurability;
        float m_priceDispersion;
        CStr m_modelName;
        int m_minCount;
        int m_maxCount;
    };

    class Ware : public Obj
    {
    public:
        Ware(WarePrototypeInfo const &);
        virtual bool ApplyModifier(Modifier const &);
        static m3d::Class * GetBaseClass();
        virtual unsigned int GetPrice(IPriceCoeffProvider const *) const ;
        virtual int GetPropertyId(char const *) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        virtual WarePrototypeInfo const * GetPrototypeInfo() const ;
        virtual CStr GetPropertyName(int) const ;
        NumericInRange<float> const & Durability() const ;
        NumericInRange<float> & Durability();
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual m3d::Class * GetClass() const ;
        static void __fastcall Registration();
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);

    protected:
        virtual ~Ware();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
        unsigned int m_maxItems;
        NumericInRange<float> m_durability;
    };
}

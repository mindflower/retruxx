#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <server/components/numericinrange.h>

namespace ai
{
    // Rounds a computed price to an integer, never below 1.
    int GetIntPrice(float price);

    // Like GetIntPrice, but a (near-)zero price stays 0 - used for service costs
    // (repair / refuel / recharge) that can legitimately be free.
    int GetIntRepairPrice(float price);

    // Fraction of full price a used item fetches: currentDurability / maxDurability (0 when max ~ 0).
    float GetDurabilityPriceCoeff(const ai::NumericInRange<float>& durability);

    // Fraction of full price a full repair costs: (max - current) / max * 0.75
    // (0 when max ~ 0). The 0.75 keeps repairing cheaper than replacing.
    float GetDurabilityRepairCoeff(const ai::NumericInRange<float>& durability);

    class WarePrototypeInfo : public ai::PrototypeInfo
    {
    public:
        WarePrototypeInfo();
        unsigned int m_maxItems;
        float m_maxDurability;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        float GetPriceDispersion() const;
        virtual ai::Obj* CreateTargetObject() const override;
        const CStr& GetModelName() const;
        int GetMinCount() const;
        int GetMaxCount() const;

    private:
        float m_priceDispersion;
        CStr m_modelName;
        int m_minCount;
        int m_maxCount;
    };

    class Ware : public ai::Obj
    {
        friend class WarePrototypeInfo;
    protected:
        virtual  ~Ware() override;

    private:
        Ware(const ai::WarePrototypeInfo& prototypeInfo);
        Ware(const ai::Ware&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classWare;
        virtual const ai::WarePrototypeInfo* GetPrototypeInfo() const override;

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
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual bool ApplyModifier(const ai::Modifier& modifier) override;
        unsigned int GetMaxItems() const;
        const ai::NumericInRange<float>& Durability() const;
        ai::NumericInRange<float>& Durability();
        static void __fastcall Registration();
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override;

    private:
        unsigned int m_maxItems;
        ai::NumericInRange<float> m_durability;
    };
}

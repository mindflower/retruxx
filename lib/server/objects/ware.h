#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <server/components/numericinrange.h>

namespace ai
{
    // Rounds a computed price to an integer, never below 1.
    int GetIntPrice(float price);

    // Fraction of full price a used item fetches: currentDurability / maxDurability (0 when max ~ 0).
    float GetDurabilityPriceCoeff(const ai::NumericInRange<float>& durability);

    class WarePrototypeInfo : public ai::PrototypeInfo
    {
    public:
        WarePrototypeInfo();
        /* 0x0040 */ unsigned int m_maxItems;
        /* 0x0044 */ float m_maxDurability;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        float GetPriceDispersion() const;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
        const CStr& GetModelName() const;
        int GetMinCount() const;
        int GetMaxCount() const;

    private:
        /* 0x0048 */ float m_priceDispersion;
        /* 0x004c */ CStr m_modelName;
        /* 0x0058 */ int m_minCount;
        /* 0x005c */ int m_maxCount;
    }; /* size: 0x0060 */

    static_assert(sizeof(WarePrototypeInfo) == 0x0060);

    class Ware : public ai::Obj
    {
        friend class WarePrototypeInfo;
    protected:
        virtual  ~Ware() override /* 0x00 */;

    private:
        Ware(const ai::WarePrototypeInfo& prototypeInfo);
        Ware(const ai::Ware&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classWare;
        virtual const ai::WarePrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x98 */;
        virtual bool ApplyModifier(const ai::Modifier& modifier) override /* 0x88 */;
        unsigned int GetMaxItems() const;
        const ai::NumericInRange<float>& Durability() const;
        ai::NumericInRange<float>& Durability();
        static void __fastcall Registration();
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override /* 0xec */;

    private:
        /* 0x00c0 */ unsigned int m_maxItems;
        /* 0x00c4 */ ai::NumericInRange<float> m_durability;
    }; /* size: 0x0170 */

    static_assert(sizeof(Ware) == 0x0170);
}

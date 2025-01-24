#pragma once
#include <server/objects/base/complexphysicobj.h>
#include <server/components/functions/memberfunctiononearg.h>
#include <server/components/functions/memberfunctionstwoargsref.h>

namespace ai
{
    class Boss04StationPrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    public:
        Boss04StationPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0090 */

    static_assert(sizeof(Boss04StationPrototypeInfo) == 0x0090);

    class Boss04Station : public ai::ComplexPhysicObj
    {
    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Boss04Station, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Boss04Station, ai::Modifier, float, bool>;

    protected:
        virtual  ~Boss04Station() override /* 0x00 */;

    private:
        Boss04Station(const ai::Boss04StationPrototypeInfo& prototypeInfo);
        Boss04Station(const ai::Boss04Station&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss04Station;
        virtual const ai::Boss04StationPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void InflictDamage(const ai::DamageInfo& damageInfo) override /* 0x00 */;
        void EvaluateToDead();
        bool bDestroyed() const;
        static void __fastcall Registration();

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x100 */;

    private:
        /* 0x014c */ bool m_bDestroyed;
    }; /* size: 0x0150 */

    static_assert(sizeof(Boss04Station) == 0x0150);
}

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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Boss04Station : public ai::ComplexPhysicObj
    {
        friend class Boss04StationPrototypeInfo;

    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Boss04Station, float, void>;
        using BeforeApplyModifierFloatCallback =
            ai::MemberFunctionTwoArgsRef<ai::Boss04Station, ai::Modifier, float, bool>;

    protected:
        virtual ~Boss04Station() override;

    private:
        Boss04Station(ai::Boss04StationPrototypeInfo const& prototypeInfo);
        Boss04Station(ai::Boss04Station const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss04Station;
        virtual ai::Boss04StationPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void RegisterProperty(char const*, int, ai::eGObjPropertySaveStatus);

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
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void InflictDamage(ai::DamageInfo const& damageInfo) override;
        void EvaluateToDead();
        bool bDestroyed() const;
        static void Registration();

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;

    private:
        bool m_bDestroyed;
    };
}  // namespace ai

#pragma once
#include "base/complexphysicobj.h"
#include <server/components/numericinrange.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class Gun;

    class StaticAutoGunPrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(ai::PrototypeInfo const& rhs) override;

    public:
        float m_maxHealth;
        CStr m_destroyedModelName;
        StaticAutoGunPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class StaticAutoGun : public ai::ComplexPhysicObj
    {
        friend class StaticAutoGunPrototypeInfo;

    protected:
        virtual ~StaticAutoGun() override;

    private:
        StaticAutoGun(ai::StaticAutoGunPrototypeInfo const& prototypeInfo);
        StaticAutoGun(ai::StaticAutoGun const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classStaticAutoGun;
        virtual ai::StaticAutoGunPrototypeInfo const* GetPrototypeInfo() const override;

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

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::StaticAutoGun, float, void>;
        using BeforeApplyModifierFloatCallback =
            ai::MemberFunctionTwoArgsRef<ai::StaticAutoGun, ai::Modifier, float, bool>;

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        ai::Gun* getGun();
        bool PointIsReachableForGun(
            CVector const& pos,
            retruxx::vector<int, retruxx::allocator<int>> const& exceptions);
        void FireFromWeapon(bool enable);
        void WeaponLookAtPoint(CVector const& lookAt, float elapsedTime);
        virtual void RenderDebugInfo() const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pObj) override;
        virtual void SetPositionSelf(CVector const& pos) override;
        ai::NumericInRange<float> const& Health() const;
        ai::NumericInRange<float>& Health();
        CStr const& GetDestroyedModelName() const;
        virtual void InflictDamage(ai::DamageInfo const& damageInfo) override;
        virtual bool ApplyModifier(ai::Modifier const& modifier) override;
        static void __fastcall Registration();
        float GetMaxFiringRangeAI() const;

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _Construct(bool bForAnimation) override;

    private:
        ai::NumericInRange<float> m_health;
        ai::NumericInRangeRegenerating<float> m_timeForNextCheck;
        CStr m_destroyedModelName;
        retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*>> m_targetClasses;
        int m_currentEnemyId;
        bool _OnHealthValueBeforeApplyModifier(ai::Modifier const& modifier, float& newHealth);
        void _OnHealthValueAfterChange(float oldHealthValue);
    };
}  // namespace ai

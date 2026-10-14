#pragma once
#include "gun.h"

namespace ai
{
    class BulletLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        BulletLauncherPrototypeInfo();
        int m_numBulletsInShot;
        float m_groupingAngle;
        CStr m_BlastWavePrototypeName;
        int m_tracerRange;
        CStr m_tracerEffectName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
        virtual float GetDamageForOneShell() const override;
    };

    class BulletLauncher : public ai::Gun
    {
        friend class BulletLauncherPrototypeInfo;

    protected:
        virtual ~BulletLauncher() override;

    private:
        BulletLauncher(ai::BulletLauncherPrototypeInfo const& prototypeInfo);
        BulletLauncher(ai::BulletLauncher const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBulletLauncher;
        virtual ai::BulletLauncherPrototypeInfo const* GetPrototypeInfo() const override;

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
        static void __fastcall Registration();
        float GetGroupingAngle() const;
        float GetGroupingAngleClamped() const;
        float GetAccuracy() const;
        float GetAccuracyClamped() const;
        virtual float GetDamageForOneShell() const override;
        static float __fastcall GroupingAngle2Accuracy(float groupingAngle);
        static float __fastcall Accuracy2GroupingAngle(float accuracy);

    protected:
        virtual void _LaunchShells() override;

    private:
        unsigned int m_numBulletsInShot;
        float m_groupingAngle;
        int m_numBulletsToTracer;
    };
}  // namespace ai

#pragma once
#include "gun.h"

namespace ai
{
    class RocketLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        RocketLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
        bool m_withAngleLimit;
    };

    class RocketLauncher : public ai::Gun
    {
        friend class RocketLauncherPrototypeInfo;

    protected:
        virtual ~RocketLauncher() override;

    protected:
        RocketLauncher(ai::RocketLauncherPrototypeInfo const& prototypeInfo);
        RocketLauncher(ai::RocketLauncher const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classRocketLauncher;
        virtual ai::RocketLauncherPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const*, int, ai::eGObjPropertySaveStatus);

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
        virtual bool isLookAtPoint(CVector const& lookAt, float eps) const override;
        virtual bool CanLookAtTarget() const override;

    protected:
        virtual void _LaunchShells() override;
    };
}  // namespace ai

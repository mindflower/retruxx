#pragma once
#include "gun.h"

namespace ai
{
    class RocketLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        RocketLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        /* 0x0198 */ bool m_withAngleLimit;
    }; /* size: 0x019c */

    static_assert(sizeof(RocketLauncherPrototypeInfo) == 0x019c);

    class RocketLauncher : public ai::Gun
    {
    protected:
        virtual  ~RocketLauncher() override /* 0x00 */;

    private:
        RocketLauncher(const ai::RocketLauncherPrototypeInfo& prototypeInfo);
        RocketLauncher(const ai::RocketLauncher&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classRocketLauncher;
        virtual const ai::RocketLauncherPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

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
        static void __fastcall Registration();
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const override /* 0x16c */;
        virtual bool CanLookAtTarget() const override /* 0x18c */;

    protected:
        virtual void _LaunchShells() override /* 0x198 */;
    }; /* size: 0x0330 */

    static_assert(sizeof(RocketLauncher) == 0x0330);
}

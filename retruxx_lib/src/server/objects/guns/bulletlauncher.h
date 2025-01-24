#pragma once
#include "gun.h"

namespace ai
{
    class BulletLauncherPrototypeInfo : public ai::GunPrototypeInfo
    {
    public:
        BulletLauncherPrototypeInfo();
        /* 0x0198 */ int m_numBulletsInShot;
        /* 0x019c */ float m_groupingAngle;
        /* 0x01a0 */ CStr m_BlastWavePrototypeName;
        /* 0x01ac */ int m_tracerRange;
        /* 0x01b0 */ CStr m_tracerEffectName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        virtual float GetDamageForOneShell() const override /* 0x1c */;
    }; /* size: 0x01bc */

    static_assert(sizeof(BulletLauncherPrototypeInfo) == 0x01bc);

    class BulletLauncher : public ai::Gun
    {
    protected:
        virtual  ~BulletLauncher() override /* 0x00 */;

    private:
        BulletLauncher(const ai::BulletLauncherPrototypeInfo& prototypeInfo);
        BulletLauncher(const ai::BulletLauncher&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBulletLauncher;
        virtual const ai::BulletLauncherPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

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
        float GetGroupingAngle() const;
        float GetGroupingAngleClamped() const;
        float GetAccuracy() const;
        float GetAccuracyClamped() const;
        virtual float GetDamageForOneShell() const override /* 0x188 */;
        static float __fastcall GroupingAngle2Accuracy(float groupingAngle);
        static float __fastcall Accuracy2GroupingAngle(float accuracy);

    protected:
        virtual void _LaunchShells() override /* 0x198 */;

    private:
        /* 0x0330 */ unsigned int m_numBulletsInShot;
        /* 0x0334 */ float m_groupingAngle;
        /* 0x0338 */ int m_numBulletsToTracer;
    }; /* size: 0x033c */

    static_assert(sizeof(BulletLauncher) == 0x033c);
}

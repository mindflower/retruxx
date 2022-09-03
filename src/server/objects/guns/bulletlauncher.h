#pragma once
#include "gun.h"

namespace ai
{
    class BulletLauncherPrototypeInfo : public GunPrototypeInfo
    {
    public:
        virtual void PostLoad();
        virtual float GetDamageForOneShell() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        BulletLauncherPrototypeInfo();

    private:
        int m_numBulletsInShot;
        float m_groupingAngle;
        CStr m_BlastWavePrototypeName;
        int m_tracerRange;
        CStr m_tracerEffectName;
    };

    class BulletLauncher : public Gun
    {
    public:
        float GetAccuracyClamped() const;
        virtual CStr GetPropertyName(int) const;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const;
        BulletLauncher(BulletLauncherPrototypeInfo const&);
        static float __fastcall Accuracy2GroupingAngle(float);
        float GetGroupingAngle() const;
        virtual float GetDamageForOneShell() const;
        float GetAccuracy() const;
        virtual m3d::Class* GetClass() const;
        virtual void GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int> >&) const;
        static void __fastcall Registration();
        virtual BulletLauncherPrototypeInfo const* GetPrototypeInfo() const;
        virtual bool SetPropertyById(int, m3d::AIParam const&);
        virtual int GetPropertyId(char const*) const;
        virtual void GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr> >&) const;
        static float __fastcall GroupingAngle2Accuracy(float);
        static m3d::Class* GetBaseClass();
        float GetGroupingAngleClamped() const;

    protected:
        static void __fastcall RegisterProperty(char const*, int, eGObjPropertySaveStatus);
        virtual bool _GetPropertyInternal(int, m3d::AIParam&) const;
        virtual void _LaunchShells();
        virtual ~BulletLauncher();
        virtual bool _GetPropertyDefaultInternal(int, m3d::AIParam&) const;

    private:
        virtual m3d::Object* Clone();
        static m3d::Object* CreateObject();

    public:
        RT_CLASS_DECLARE(BulletLauncher);

    private:
        unsigned int m_numBulletsInShot;
        float m_groupingAngle;
        int m_numBulletsToTracer;
    };
}

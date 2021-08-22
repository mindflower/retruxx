#pragma once
#include "gun.h"

namespace ai
{
    class ThunderboltLauncherPrototypeInfo : public GunPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        ThunderboltLauncherPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();

    private:
        float m_ActionDist;
    };

    class ThunderboltLauncher : public Gun
    {
    public:
        static inline m3d::Class m_classThunderboltLauncher;

    public:
        virtual m3d::Class* GetClass() const;
        virtual ThunderboltLauncherPrototypeInfo const* GetPrototypeInfo() const;
        static m3d::Class* __fastcall GetBaseClass();
        virtual bool CanFire() const;
        ThunderboltLauncher(ThunderboltLauncherPrototypeInfo const&);

    protected:
        virtual void _LaunchShells();
        virtual ~ThunderboltLauncher();

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();

    private:
        std::vector<int> m_enemies;
    };
}

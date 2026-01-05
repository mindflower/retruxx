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
        virtual m3d::Class* GetClass() const;
        virtual ThunderboltLauncherPrototypeInfo const* GetPrototypeInfo() const;
        static m3d::Class* GetBaseClass();
        virtual bool CanFire() const;
        ThunderboltLauncher(ThunderboltLauncherPrototypeInfo const&);

    protected:
        virtual void _LaunchShells();
        virtual ~ThunderboltLauncher();

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_DECLARE(ThunderboltLauncher);

    private:
        std::vector<int> m_enemies;
    };
}

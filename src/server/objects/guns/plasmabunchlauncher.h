#pragma once
#include "gun.h"

namespace ai
{
    class PlasmaBunchLauncherPrototypeInfo : public GunPrototypeInfo
    {
    public:
        PlasmaBunchLauncherPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();

    private:
        CStr m_bunchPrototypeName;
    };

    class PlasmaBunchLauncher : public Gun
    {
    public:
        static inline m3d::Class m_classPlasmaBunchLauncher;

    public:
        virtual m3d::Class* GetClass() const;
        virtual PlasmaBunchLauncherPrototypeInfo const* GetPrototypeInfo() const;
        PlasmaBunchLauncher(PlasmaBunchLauncherPrototypeInfo const&);
        static m3d::Class* __fastcall GetBaseClass();

    protected:
        virtual void _LaunchShells();
        virtual ~PlasmaBunchLauncher();

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
    };
}

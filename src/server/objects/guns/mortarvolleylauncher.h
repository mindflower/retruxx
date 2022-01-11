#pragma once
#include "mortar.h"

namespace ai
{
    class MortarVolleyLauncherPrototypeInfo : public MortarPrototypeInfo
    {
    public:
        MortarVolleyLauncherPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
    };

    class MortarVolleyLauncher : public Mortar
    {
    public:
        static m3d::Class* GetBaseClass();
        MortarVolleyLauncher(MortarVolleyLauncherPrototypeInfo const&);
        virtual MortarVolleyLauncherPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual bool Fire(bool);

    protected:
        virtual ~MortarVolleyLauncher();

    private:
        static m3d::Object* CreateObject();
        virtual m3d::Object* Clone();

    public:
        RT_CLASS_INLINE_DECLARE(MortarVolleyLauncher);
    };
}

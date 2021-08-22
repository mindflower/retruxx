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
        static inline m3d::Class m_classMortarVolleyLauncher;

    public:
        static m3d::Class* __fastcall GetBaseClass();
        MortarVolleyLauncher(MortarVolleyLauncherPrototypeInfo const&);
        virtual MortarVolleyLauncherPrototypeInfo const* GetPrototypeInfo() const;
        virtual m3d::Class* GetClass() const;
        virtual bool Fire(bool);

    protected:
        virtual ~MortarVolleyLauncher();

    private:
        static m3d::Object* __fastcall CreateObject();
        virtual m3d::Object* Clone();
    };
}

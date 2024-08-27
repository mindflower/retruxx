#include "mortarvolleylauncher.h"
#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MortarVolleyLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MortarVolleyLauncher);

    MortarVolleyLauncherPrototypeInfo::MortarVolleyLauncherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* MortarVolleyLauncherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* MortarVolleyLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Mortar);
    }

    MortarVolleyLauncher::MortarVolleyLauncher(MortarVolleyLauncherPrototypeInfo const& prototype) : Mortar(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    MortarVolleyLauncherPrototypeInfo const* MortarVolleyLauncher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* MortarVolleyLauncher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    bool MortarVolleyLauncher::Fire(bool)
    {
        throw std::logic_error("Not implemented");
    }

    MortarVolleyLauncher::~MortarVolleyLauncher()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* MortarVolleyLauncher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* MortarVolleyLauncher::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

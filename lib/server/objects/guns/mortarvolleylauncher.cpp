#include "mortarvolleylauncher.h"
#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MortarVolleyLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MortarVolleyLauncher);

    MortarVolleyLauncherPrototypeInfo::MortarVolleyLauncherPrototypeInfo() = default;

    Obj* MortarVolleyLauncherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* MortarVolleyLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Mortar);
    }

    MortarVolleyLauncher::MortarVolleyLauncher(MortarVolleyLauncherPrototypeInfo const& prototype) : Mortar(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MortarVolleyLauncherPrototypeInfo const* MortarVolleyLauncher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* MortarVolleyLauncher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool MortarVolleyLauncher::Fire(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    MortarVolleyLauncher::~MortarVolleyLauncher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* MortarVolleyLauncher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* MortarVolleyLauncher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

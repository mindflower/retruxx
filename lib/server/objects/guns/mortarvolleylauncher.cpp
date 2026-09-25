#include "mortarvolleylauncher.h"
#include <math/matrix.h>
#include <stdexcept>

#include "core/kernel.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(MortarVolleyLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(MortarVolleyLauncher);

    MortarVolleyLauncherPrototypeInfo::MortarVolleyLauncherPrototypeInfo() = default;

    Obj* MortarVolleyLauncherPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x848E50
        return new MortarVolleyLauncher(*this);
    }

    m3d::Class* MortarVolleyLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Mortar);
    }

    MortarVolleyLauncher::MortarVolleyLauncher(MortarVolleyLauncherPrototypeInfo const& prototype) : Mortar(prototype)
    {
        // RVA 0x848E20
    }

    MortarVolleyLauncherPrototypeInfo const* MortarVolleyLauncher::GetPrototypeInfo() const
    {
        // RVA 0x849300
        // NOTE: the prototype is cast without a type check.
        return static_cast<MortarVolleyLauncherPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* MortarVolleyLauncher::GetClass() const
    {
        // RVA 0x848E10
        return RT_CLASS_LOCAL(MortarVolleyLauncher);
    }

    bool MortarVolleyLauncher::Fire(bool enable)
    {
        // RVA 0x848EC0
        // A volley: one shot from every barrel at once. The shot timer is forced past the
        // fire rate before each shot, then reset if anything fired.
        bool doFire = enable;
        if (enable && !CanFire())
        {
            doFire = false;
        }
        m_bIsFiring = doFire;
        if (!doFire)
        {
            return false;
        }
        unsigned const barrels = static_cast<unsigned>(GetPrototypeInfo()->m_fireLpMatrices.size());
        float const timeFromLastShot = m_timeFromLastShot;
        m_curBarrelIndex = 0;
        for (unsigned i = 0; i < barrels; ++i)
        {
            // NOTE: every barrel fires even after one has failed.
            doFire &= _DoFire();
            m_timeFromLastShot = 1000000.0f;
        }
        m_timeFromLastShot = doFire ? 0.0f : timeFromLastShot;
        return doFire;
    }

    // RVA 0x848E40
    MortarVolleyLauncher::~MortarVolleyLauncher() = default;

    m3d::Object* MortarVolleyLauncher::CreateObject()
    {
        // RVA 0x849140
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* MortarVolleyLauncher::Clone()
    {
        // RVA 0x848F80
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

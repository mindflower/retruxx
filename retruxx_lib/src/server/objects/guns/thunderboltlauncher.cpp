#include "thunderboltlauncher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ThunderboltLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ThunderboltLauncher);

    Obj* ThunderboltLauncherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ThunderboltLauncherPrototypeInfo::ThunderboltLauncherPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool ThunderboltLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ThunderboltLauncherPrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ThunderboltLauncher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ThunderboltLauncherPrototypeInfo const* ThunderboltLauncher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* ThunderboltLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    bool ThunderboltLauncher::CanFire() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ThunderboltLauncher::ThunderboltLauncher(ThunderboltLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void ThunderboltLauncher::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ThunderboltLauncher::~ThunderboltLauncher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ThunderboltLauncher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* ThunderboltLauncher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

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
        throw std::logic_error("Not implemented");
    }

    ThunderboltLauncherPrototypeInfo::ThunderboltLauncherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool ThunderboltLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ThunderboltLauncherPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ThunderboltLauncher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    ThunderboltLauncherPrototypeInfo const* ThunderboltLauncher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ThunderboltLauncher::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    bool ThunderboltLauncher::CanFire() const
    {
        throw std::logic_error("Not implemented");
    }

    ThunderboltLauncher::ThunderboltLauncher(ThunderboltLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void ThunderboltLauncher::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    ThunderboltLauncher::~ThunderboltLauncher()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ThunderboltLauncher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ThunderboltLauncher::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

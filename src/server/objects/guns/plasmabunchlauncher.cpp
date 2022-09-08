#include "plasmabunchlauncher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PlasmaBunchLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PlasmaBunchLauncher);

    PlasmaBunchLauncherPrototypeInfo::PlasmaBunchLauncherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* PlasmaBunchLauncherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PlasmaBunchLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void PlasmaBunchLauncherPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PlasmaBunchLauncher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    PlasmaBunchLauncherPrototypeInfo const* PlasmaBunchLauncher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    PlasmaBunchLauncher::PlasmaBunchLauncher(PlasmaBunchLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PlasmaBunchLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    void PlasmaBunchLauncher::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    PlasmaBunchLauncher::~PlasmaBunchLauncher()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* PlasmaBunchLauncher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* PlasmaBunchLauncher::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

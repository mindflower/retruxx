#include "rocketlauncher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RocketLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RocketLauncher);

    Obj* RocketLauncherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    RocketLauncherPrototypeInfo::RocketLauncherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketLauncherPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketLauncher::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    RocketLauncher::RocketLauncher(RocketLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RocketLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    RocketLauncherPrototypeInfo const* RocketLauncher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketLauncher::isLookAtPoint(CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketLauncher::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketLauncher::CanLookAtTarget() const
    {
        throw std::logic_error("Not implemented");
    }

    int RocketLauncher::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RocketLauncher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void RocketLauncher::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketLauncher::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr RocketLauncher::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus RocketLauncher::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketLauncher::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketLauncher::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void RocketLauncher::_LaunchShells()
    {
        throw std::logic_error("Not implemented");
    }

    RocketLauncher::~RocketLauncher()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RocketLauncher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RocketLauncher::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

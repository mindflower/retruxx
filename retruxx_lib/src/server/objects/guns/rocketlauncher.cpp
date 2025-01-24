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
        throw retruxx::logic_error("Not implemented");
    }

    bool RocketLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    RocketLauncherPrototypeInfo::RocketLauncherPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RocketLauncherPrototypeInfo::PostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RocketLauncher::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    RocketLauncher::RocketLauncher(RocketLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* RocketLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    RocketLauncherPrototypeInfo const* RocketLauncher::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool RocketLauncher::isLookAtPoint(CVector const&, float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool RocketLauncher::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool RocketLauncher::CanLookAtTarget() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int RocketLauncher::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* RocketLauncher::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RocketLauncher::Registration()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RocketLauncher::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr RocketLauncher::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus RocketLauncher::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool RocketLauncher::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool RocketLauncher::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void RocketLauncher::_LaunchShells()
    {
        throw retruxx::logic_error("Not implemented");
    }

    RocketLauncher::~RocketLauncher()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* RocketLauncher::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* RocketLauncher::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

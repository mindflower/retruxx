#include "rocketvolleylauncher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RocketVolleyLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RocketVolleyLauncher);

    bool RocketVolleyLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* RocketVolleyLauncherPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    RocketVolleyLauncherPrototypeInfo::RocketVolleyLauncherPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketVolleyLauncher::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketVolleyLauncher::Fire(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void RocketVolleyLauncher::SetInvisible()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketVolleyLauncher::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    RocketVolleyLauncher::RocketVolleyLauncher(ai::RocketVolleyLauncherPrototypeInfo const& prototype) : RocketLauncher(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RocketVolleyLauncher::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RocketVolleyLauncher::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketVolleyLauncher::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    ai::RocketVolleyLauncherPrototypeInfo const* RocketVolleyLauncher::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketVolleyLauncher::_bIsUsingVolley() const
    {
        throw std::logic_error("Not implemented");
    }

    bool RocketVolleyLauncher::_bIsVolleyFiring() const
    {
        throw std::logic_error("Not implemented");
    }

    RocketVolleyLauncher::~RocketVolleyLauncher()
    {
        throw std::logic_error("Not implemented");
    }

    void RocketVolleyLauncher::_TryToLaunch()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RocketVolleyLauncher::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RocketVolleyLauncher::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

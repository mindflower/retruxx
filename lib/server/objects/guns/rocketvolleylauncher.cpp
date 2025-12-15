#include "rocketvolleylauncher.h"

#include <math/matrix.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RocketVolleyLauncher)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RocketVolleyLauncher);

    bool RocketVolleyLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = RocketLauncherPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_actionDist, xmlNode, "ActionDist");
        }
        return result;
    }

    Obj* RocketVolleyLauncherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RocketVolleyLauncherPrototypeInfo::RocketVolleyLauncherPrototypeInfo()
    {
        m_actionDist = 0.0f;
        m_WithShellsPoolLimit = 1;
    }

    void RocketVolleyLauncher::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketVolleyLauncher::Fire(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RocketVolleyLauncher::SetInvisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RocketVolleyLauncher::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RocketVolleyLauncher::RocketVolleyLauncher(ai::RocketVolleyLauncherPrototypeInfo const& prototype) : RocketLauncher(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RocketVolleyLauncher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RocketVolleyLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(RocketLauncher);
    }

    void RocketVolleyLauncher::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::RocketVolleyLauncherPrototypeInfo const* RocketVolleyLauncher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketVolleyLauncher::_bIsUsingVolley() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketVolleyLauncher::_bIsVolleyFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RocketVolleyLauncher::~RocketVolleyLauncher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RocketVolleyLauncher::_TryToLaunch()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RocketVolleyLauncher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RocketVolleyLauncher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

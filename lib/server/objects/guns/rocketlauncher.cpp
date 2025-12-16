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
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeBoolAttrib(m_withAngleLimit, xmlNode, "WithAngleLimit");
        }
        return result;
    }

    RocketLauncherPrototypeInfo::RocketLauncherPrototypeInfo()
    {
        m_withAngleLimit = true;
        m_damageType = DAMAGE_BLAST;
        m_WithShellsPoolLimit = true;
    }

    void RocketLauncherPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    void RocketLauncher::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RocketLauncher::RocketLauncher(RocketLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RocketLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    RocketLauncherPrototypeInfo const* RocketLauncher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketLauncher::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketLauncher::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketLauncher::CanLookAtTarget() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int RocketLauncher::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RocketLauncher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RocketLauncher::Registration()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RocketLauncher::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr RocketLauncher::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus RocketLauncher::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketLauncher::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RocketLauncher::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RocketLauncher::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RocketLauncher::~RocketLauncher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RocketLauncher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RocketLauncher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

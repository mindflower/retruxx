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
        m_damageType = DAMAGE_ENERGY;
        m_WithShellsPoolLimit = true;
    }

    Obj* PlasmaBunchLauncherPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PlasmaBunchLauncherPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return GunPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    void PlasmaBunchLauncherPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    m3d::Class* PlasmaBunchLauncher::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PlasmaBunchLauncherPrototypeInfo const* PlasmaBunchLauncher::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PlasmaBunchLauncher::PlasmaBunchLauncher(PlasmaBunchLauncherPrototypeInfo const& prototype) : Gun(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PlasmaBunchLauncher::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Gun);
    }

    void PlasmaBunchLauncher::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PlasmaBunchLauncher::~PlasmaBunchLauncher()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PlasmaBunchLauncher::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PlasmaBunchLauncher::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

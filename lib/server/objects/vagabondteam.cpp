#include "vagabondteam.h"

#include <stdexcept>

#include "core/ini.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VagabondTeam)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VagabondTeam);

    std::vector<CStr, std::allocator<CStr>> const& VagabondTeamPrototypeInfo::GetWaresPrototypes() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VagabondTeamPrototypeInfo::VagabondTeamPrototypeInfo()
    {
        m_bRemoveWhenChildrenDead = 1;
    }

    Obj* VagabondTeamPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& VagabondTeamPrototypeInfo::GetVehiclesGeneratorPrototypeName() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VagabondTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = TeamPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(m_vehiclesGeneratorPrototype, xmlNode, "VehicleGeneratorPrototype");

            CStr strWares;
            m3d::SafeStrAttrib(strWares, xmlNode, "WaresPrototypes");
            m3d::Tokenize(strWares, m_waresPrototypes, "(), ;\t");
        }
        return result;
    }

    VagabondTeam::VagabondTeam(VagabondTeamPrototypeInfo const& prototype) : Team(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VagabondTeamPrototypeInfo const* VagabondTeam::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VagabondTeam::GenerateAndPlace(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VagabondTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void VagabondTeam::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VagabondTeam::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VagabondTeam::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VagabondTeam::~VagabondTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VagabondTeam::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VagabondTeam::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VagabondTeam::_DoNoticeEnemy(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

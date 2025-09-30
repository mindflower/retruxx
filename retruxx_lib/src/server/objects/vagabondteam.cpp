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
        throw std::logic_error("Not implemented");
    }

    VagabondTeamPrototypeInfo::VagabondTeamPrototypeInfo()
    {
        m_bRemoveWhenChildrenDead = 1;
    }

    Obj* VagabondTeamPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& VagabondTeamPrototypeInfo::GetVehiclesGeneratorPrototypeName() const
    {
        throw std::logic_error("Not implemented");
    }

    bool VagabondTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        throw std::logic_error("Not implemented");
        auto result = ai::TeamPrototypeInfo::LoadFromXML( xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(this->m_vehiclesGeneratorPrototype, xmlNode, "VehicleGeneratorPrototype");

            CStr strWares;
            m3d::SafeStrAttrib(strWares, xmlNode, "WaresPrototypes");
            m3d::Tokenize(strWares, this->m_waresPrototypes, "(), ;\t");
            return 1;
        }
        return result;
    }

    VagabondTeam::VagabondTeam(VagabondTeamPrototypeInfo const& prototype) : Team(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    VagabondTeamPrototypeInfo const* VagabondTeam::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void VagabondTeam::GenerateAndPlace(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VagabondTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void VagabondTeam::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void VagabondTeam::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* VagabondTeam::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    VagabondTeam::~VagabondTeam()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VagabondTeam::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* VagabondTeam::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void VagabondTeam::_DoNoticeEnemy(int)
    {
        throw std::logic_error("Not implemented");
    }
}

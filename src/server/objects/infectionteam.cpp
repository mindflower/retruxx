#include "infectionteam.h"
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(InfectionTeam)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(InfectionTeam);

    void InfectionTeam::SetCriticalDistAndTime(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionTeam::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    InfectionTeam::InfectionTeam(InfectionTeamPrototypeInfo const& prototypeInfo) : Team(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionTeam::SetBlindDistAndTime(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* InfectionTeam::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionTeam::Generate(std::vector<CVector, std::allocator<CVector>> const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* InfectionTeam::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    InfectionTeamPrototypeInfo const* InfectionTeam::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionTeam::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionTeam::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    InfectionTeam::~InfectionTeam()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionTeam::_TeamUpdate(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* InfectionTeam::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* InfectionTeam::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

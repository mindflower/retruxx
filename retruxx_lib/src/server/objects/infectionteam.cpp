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
        return RT_CLASS_LOCAL(Team);
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

    InfectionTeamPrototypeInfo::Item::Item(const ai::InfectionTeamPrototypeInfo::Item& __that)
    {
        throw std::logic_error("Not implemented");
    }

    InfectionTeamPrototypeInfo::Item::Item(const CStr& protoName, int count)
    {
        throw std::logic_error("Not implemented");
    }

    InfectionTeamPrototypeInfo::Item::Item()
    {
        throw std::logic_error("Not implemented");
    }

    InfectionTeamPrototypeInfo::InfectionTeamPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionTeamPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* InfectionTeamPrototypeInfo::CreateTargetObject() const
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

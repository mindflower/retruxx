#include "caravanteam.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CaravanTeam)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CaravanTeam);

    CaravanTeamPrototypeInfo::CaravanTeamPrototypeInfo()
    {
        m_bRemoveWhenChildrenDead = 1;
        m_formationPrototypeName = "caravanFormation";
    }

    std::vector<CStr, std::allocator<CStr>> const& CaravanTeamPrototypeInfo::GetWaresPrototypes() const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* CaravanTeamPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeamPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    bool CaravanTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::SetWaitingPlayerToMoveout()
    {
        throw std::logic_error("Not implemented");
    }

    CaravanTeamPrototypeInfo const* CaravanTeam::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CaravanTeam::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int CaravanTeam::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    CaravanTeam::CaravanTeam(CaravanTeamPrototypeInfo const& prototype) : Team(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::GenerateAndPlace(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CaravanTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void CaravanTeam::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_DoNoticeEnemy(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_TeamUpdate(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_DoUnderAttack(int)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_DoPosUnreachable()
    {
        throw std::logic_error("Not implemented");
    }

    CaravanTeam::~CaravanTeam()
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_RemoveChildrenWhenPlayerIsFarEnough()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CaravanTeam::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<int, std::allocator<int>> CaravanTeam::_GenerateWithVehicleGenerator(int, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool CaravanTeam::_HasAvailableGuards() const
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_RemoveUnlessChildrenExist()
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_OnEnemyDestroyed(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void CaravanTeam::_EnsureGuardsAreInSeparateTeam()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CaravanTeam::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

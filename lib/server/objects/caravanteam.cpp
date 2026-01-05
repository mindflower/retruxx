#include "caravanteam.h"
#include "core/log.h"
#include "core/kernel.h"

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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* CaravanTeamPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeamPrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    bool CaravanTeamPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = TeamPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeStrAttrib(m_tradersGeneratorPrototypeName, xmlNode, "TradersVehiclesGeneratorName");
            m3d::SafeStrAttrib(m_guardsGeneratorPrototypeName, xmlNode, "GuardVehiclesGeneratorName");

            CStr strWares;
            m3d::SafeStrAttrib(strWares, xmlNode, "WaresPrototypes");
            m3d::Tokenize(strWares, m_waresPrototypes, "(), ;\t");

            if (!m_tradersGeneratorPrototypeName.empty() && m_waresPrototypes.empty())
            {
                M3D_CRITICAL_ERROR("no wares for caravan with traders: " + m_prototypeName + "'");
            }
        }
        return result;
    }

    void CaravanTeam::SetWaitingPlayerToMoveout()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CaravanTeamPrototypeInfo const* CaravanTeam::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CaravanTeam::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int CaravanTeam::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CaravanTeam::CaravanTeam(CaravanTeamPrototypeInfo const& prototype) : Team(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::GenerateAndPlace(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CaravanTeam::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Team);
    }

    void CaravanTeam::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_DoNoticeEnemy(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_TeamUpdate(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_DoUnderAttack(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_DoPosUnreachable()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CaravanTeam::~CaravanTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_RemoveChildrenWhenPlayerIsFarEnough()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CaravanTeam::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    std::vector<int, std::allocator<int>> CaravanTeam::_GenerateWithVehicleGenerator(int, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CaravanTeam::_HasAvailableGuards() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_RemoveUnlessChildrenExist()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_OnEnemyDestroyed(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CaravanTeam::_EnsureGuardsAreInSeparateTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CaravanTeam::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

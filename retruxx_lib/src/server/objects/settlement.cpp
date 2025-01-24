#include "settlement.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Settlement)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Settlement);

    SettlementPrototypeInfo::auxZoneInfo::auxZoneInfo(auxZoneInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SettlementPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SettlementPrototypeInfo::PostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SettlementPrototypeInfo::SettlementPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Settlement::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::Registration()
    {
    }

    void Settlement::Remove()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Settlement::Settlement(SettlementPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_population(5, 0, 5)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::CreateChildren()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Settlement::GetDeployPos() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Settlement::GetDefendPos() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Location* Settlement::GetLocation(Location::LocationType) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Settlement::CanChildBeAdded(m3d::Class*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::SetRotationSelf(Quaternion const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Settlement::GetCaravanArrivePos() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Settlement::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    int Settlement::OnEvent(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::SetPosition(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Team* Settlement::GetTeamByType(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::SetTeamByType(CStr const&, Team*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::SetPositionSelf(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SettlementPrototypeInfo const* Settlement::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Settlement::RemoveChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::SetRotation(Quaternion const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Settlement::GetAttackPos() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::AddChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Settlement::GetEnterPos() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Vehicle* Settlement::_SpawnVehicle()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Settlement::_GetNumSpawnedVehicles()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Settlement::_GetOverallPopulation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::CreateChildLocation(Location::LocationType, CStr const&, CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Settlement::_GetMaxOverallPopulaition()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Settlement::_GetMaxSpawnedVehicles()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Settlement::~Settlement()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Settlement::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Settlement::FillingFeedBackParam()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Settlement::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

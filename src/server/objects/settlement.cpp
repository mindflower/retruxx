#include "settlement.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Settlement)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Settlement);

    SettlementPrototypeInfo::auxZoneInfo::auxZoneInfo(auxZoneInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool SettlementPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void SettlementPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    SettlementPrototypeInfo::SettlementPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Settlement::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    Settlement::Settlement(SettlementPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::CreateChildren()
    {
        throw std::logic_error("Not implemented");
    }

    CVector Settlement::GetDeployPos() const
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    CVector Settlement::GetDefendPos() const
    {
        throw std::logic_error("Not implemented");
    }

    Location* Settlement::GetLocation(Location::LocationType) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Settlement::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::SetRotationSelf(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Settlement::GetCaravanArrivePos() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Settlement::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    int Settlement::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::SetPosition(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    Team* Settlement::GetTeamByType(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::SetTeamByType(CStr const&, Team*)
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::SetPositionSelf(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    SettlementPrototypeInfo const* Settlement::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Settlement::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::SetRotation(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Settlement::GetAttackPos() const
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Settlement::GetEnterPos() const
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle* Settlement::_SpawnVehicle()
    {
        throw std::logic_error("Not implemented");
    }

    int Settlement::_GetNumSpawnedVehicles()
    {
        throw std::logic_error("Not implemented");
    }

    int Settlement::_GetOverallPopulation()
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::CreateChildLocation(Location::LocationType, CStr const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Settlement::_GetMaxOverallPopulaition()
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    int Settlement::_GetMaxSpawnedVehicles()
    {
        throw std::logic_error("Not implemented");
    }

    Settlement::~Settlement()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Settlement::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void Settlement::FillingFeedBackParam()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Settlement::Clone()
    {
        throw std::logic_error("Not implemented");
    }
}

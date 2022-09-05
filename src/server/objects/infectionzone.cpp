#include "infectionzone.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, ResetTimeOut)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, IsPlayerInside)
{
    throw std::logic_error("Not implemented");
}


namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(InfectionZone)
        RT_CLASS_EXPORT(InfectionZone, m3d::METHOD, ResetTimeOut, "", "", "")
        RT_CLASS_EXPORT(InfectionZone, m3d::METHOD, IsPlayerInside, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(InfectionZone);

    InfectionZonePrototypeInfo::InfectionZonePrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionZonePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    Obj* InfectionZonePrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::DeleteDropOutPoint(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::ResetTimeOut()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::AddPolygonPoint(float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::SetBelong(int)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::SetPolygonPoint(float, float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* InfectionZone::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::DeletePolygonPoint(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionZone::IsPlayerInside()
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionZone::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    InfectionZone::InfectionZone(InfectionZonePrototypeInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::AddDropOutPoint(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionZone::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* InfectionZone::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    int InfectionZone::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    InfectionZonePrototypeInfo const* InfectionZone::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr InfectionZone::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus InfectionZone::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionZone::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionZone::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool InfectionZone::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    InfectionZone::~InfectionZone()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::_TryDropOut(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned InfectionZone::_GetPlayerSchwarz() const
    {
        throw std::logic_error("Not implemented");
    }

    InfectionTeam* InfectionZone::_GetInfectionTeam()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::_WatchRespawn(float)
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::_WatchRespawnTimeoutFinished(float)
    {
        throw std::logic_error("Not implemented");
    }

    Vehicle const* InfectionZone::_GetPlayerVehicle() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned InfectionZone::_GetEnemiesSchwarz() const
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::_WatchPlayerInside()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* InfectionZone::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    InfectionLair* InfectionZone::_GetInfectionLair()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* InfectionZone::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void InfectionZone::_GetGoodRandomDropOutPoints(std::vector<CVector, std::allocator<CVector>>&) const
    {
        throw std::logic_error("Not implemented");
    }
}

#include "infectionzone.h"

#include <stdexcept>

#include "core/ini.h"

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, ResetTimeOut)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, IsPlayerInside)
{
    throw retruxx::logic_error("Not implemented");
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
        this->m_minDistToPlayer = 100.0;
        this->m_criticalTeamDist = 1000000.0;
        this->m_criticalTeamTime = 0.0;
        this->m_blindTeamDist = 1000000.0;
        this->m_blindTeamTime = 0.0;
        this->m_dropOutSegmentAngle = 180;
    }

    bool InfectionZonePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_minDistToPlayer, xmlNode, "MinDistToPlayer");
            m3d::SafeFloatAttrib(m_criticalTeamDist, xmlNode, "CriticalTeamDist");
            m3d::SafeFloatAttrib(m_criticalTeamTime, xmlNode, "CriticalTeamTime");
            m3d::SafeFloatAttrib(m_blindTeamDist, xmlNode, "BlindTeamDist");
            m3d::SafeFloatAttrib(m_blindTeamTime, xmlNode, "BlindTeamTime");
            m3d::SafeIntAttrib(m_dropOutSegmentAngle, xmlNode, "DropOutSegmentAngle");
            return 1;
        }
        return result;
    }

    Obj* InfectionZonePrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::Registration()
    {
        m_propertiesMap["MinDistToPlayer"] = 70;
        m_propertiesMap["CriticalTeamDist"] = 71;
        m_propertiesMap["CriticalTeamTime"] = 72;
        m_propertiesMap["InfectionTeamPrototypeName"] = 73;
        m_propertiesMap["DropOutTimeOut"] = 74;
    }

    void InfectionZone::DeleteDropOutPoint(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::ResetTimeOut()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::AddPolygonPoint(float, float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::SetBelong(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::SetPolygonPoint(float, float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* InfectionZone::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void InfectionZone::DeletePolygonPoint(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool InfectionZone::IsPlayerInside()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool InfectionZone::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    InfectionZone::InfectionZone(InfectionZonePrototypeInfo const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::AddChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::AddDropOutPoint(float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool InfectionZone::RemoveChild(Obj*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* InfectionZone::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int InfectionZone::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    InfectionZonePrototypeInfo const* InfectionZone::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr InfectionZone::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus InfectionZone::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool InfectionZone::CanChildBeAdded(m3d::Class*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::Remove()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::RenderDebugInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool InfectionZone::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool InfectionZone::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    InfectionZone::~InfectionZone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::_TryDropOut(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned InfectionZone::_GetPlayerSchwarz() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    InfectionTeam* InfectionZone::_GetInfectionTeam()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::_WatchRespawn(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::_WatchRespawnTimeoutFinished(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Vehicle const* InfectionZone::_GetPlayerVehicle() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned InfectionZone::_GetEnemiesSchwarz() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::_WatchPlayerInside()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* InfectionZone::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    InfectionLair* InfectionZone::_GetInfectionLair()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* InfectionZone::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void InfectionZone::_GetGoodRandomDropOutPoints(retruxx::vector<CVector, retruxx::allocator<CVector>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }
}

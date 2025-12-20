#include "infectionzone.h"

#include <stdexcept>

#include "core/ini.h"
#include "core/aiparam.h"
#include "base/objcontainer.h"

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, ResetTimeOut)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(InfectionZone, IsPlayerInside)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        return new InfectionZone(*this);
    }

    void InfectionZone::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::ResetTimeOut()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::AddPolygonPoint(float, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::SetBelong(int newBelong)
    {
        ai::Obj::SetBelong(newBelong);
        auto* infectionTeam = theObjects->GetEntityByObjId(m_infectionTeamId);
        if (infectionTeam)
        {
            infectionTeam->SetBelong(newBelong);
        }
    }

    void InfectionZone::SetPolygonPoint(float, float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* InfectionZone::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void InfectionZone::DeletePolygonPoint(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfectionZone::IsPlayerInside()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfectionZone::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        int res = 1;
        switch (propertyId)
        {
        case 70:
            m_minDistToPlayer = newValue.GetAsFloat();
            res = 1;
            break;

        case 71:
            m_criticalTeamDist = newValue.GetAsFloat();
            res = 1;
            break;

        case 72:
            m_criticalTeamTime = newValue.GetAsFloat();
            res = 1;
            break;

        case 73:
            m_infectionTeamPrototypeName = newValue.GetAsStr();
            res = 1;
            break;

        case 74:
            m_baseTimeoutForRespawn = newValue.GetAsFloat();
            m_timeoutForRespawn = newValue.GetAsFloat();
            res = 1;
            break;

        default:
            res = ai::Obj::SetPropertyById(propertyId, newValue);
            break;
        }
        return res;
    }

    InfectionZone::InfectionZone(InfectionZonePrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_minDistToPlayer = prototypeInfo.m_minDistToPlayer;
        m_criticalTeamDist = prototypeInfo.m_criticalTeamDist;
        m_criticalTeamTime = prototypeInfo.m_criticalTeamTime;
        m_blindTeamDist = prototypeInfo.m_blindTeamDist;
        m_blindTeamTime = prototypeInfo.m_blindTeamTime;
        m_hadPlayerInside = false;
        m_infectionTeamId = -1;
        m_infectionLairId = -1;
        m_baseTimeoutForRespawn = 30.0f;
        m_timeoutForRespawn = 30.0f;
        m_dropOutCos = 0.0f;
        m_timeForRespawn = 0.0f;
        m_dropOutTimeOut = 0.0f;
        m_lastFramePlayerInsideWithoutEnemies = 0.0f;
        m_dropOutCos = cos((double)(prototypeInfo.m_dropOutSegmentAngle / 2) * 0.017453292);
    }

    void InfectionZone::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::AddDropOutPoint(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfectionZone::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* InfectionZone::GetClass() const
    {
        return RT_CLASS_LOCAL(InfectionZone);
    }

    void InfectionZone::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int InfectionZone::GetPropertyId(char const* propName) const
    {
        auto it = InfectionZone::m_propertiesMap.find(propName);
        if (it != InfectionZone::m_propertiesMap.end())
        {
            return it->second;
        }

        return ai::Obj::GetPropertyId(propName);
    }

    InfectionZonePrototypeInfo const* InfectionZone::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr InfectionZone::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus InfectionZone::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfectionZone::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::Remove()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        // TODO: implement InfectionZone::LoadFromXML
    }

    bool InfectionZone::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool InfectionZone::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionZone::~InfectionZone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::_TryDropOut(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned InfectionZone::_GetPlayerSchwarz() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionTeam* InfectionZone::_GetInfectionTeam()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::_WatchRespawn(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::_WatchRespawnTimeoutFinished(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Vehicle const* InfectionZone::_GetPlayerVehicle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned InfectionZone::_GetEnemiesSchwarz() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::_WatchPlayerInside()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* InfectionZone::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    InfectionLair* InfectionZone::_GetInfectionLair()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* InfectionZone::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void InfectionZone::_GetGoodRandomDropOutPoints(retruxx::vector<CVector, retruxx::allocator<CVector>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

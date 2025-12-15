#include "boss03.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartPlacingDrone)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, StartMoving)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss03, NoticePlayer)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss03)
    RT_CLASS_EXPORT(Boss03, m3d::METHOD, StartPlacingDrone, "", "", "")
    RT_CLASS_EXPORT(Boss03, m3d::METHOD, StartMoving, "", "", "")
    RT_CLASS_EXPORT(Boss03, m3d::METHOD, NoticePlayer, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss03);

    bool Boss03PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            CStr strDronePrototypeNames;
            m3d::SafeStrAttrib(strDronePrototypeNames, xmlNode, "DronePrototypes");
            m3d::Tokenize(strDronePrototypeNames, m_dronePrototypeNames, "(), ;\t");

            m3d::SafeIntAttrib(m_maxDrones, xmlNode, "MaxDrones");
            m3d::SafeFloatAttrib(m_maxHealth, xmlNode, "MaxHealth");
            m3d::SafeFloatAttrib(m_maxHorizAngularVelocity, xmlNode, "MaxHorizAngularVelocity");
            m3d::SafeFloatAttrib(m_horizAngularAcceleration, xmlNode, "HorizAngularAcceleration");
            m3d::SafeFloatAttrib(m_maxVertAngularVelocity, xmlNode, "MaxVertAngularVelocity");
            m3d::SafeFloatAttrib(m_vertAngularAcceleration, xmlNode, "VertAngularAcceleration");
            m3d::SafeFloatAttrib(m_maxLinearVelocity, xmlNode, "MaxLinearVelocity");
            m3d::SafeFloatAttrib(m_linearAcceleration, xmlNode, "LinearAcceleration");

            m3d::SafeFloatAttrib(m_pathTrackTiltAngle, xmlNode, "PathTrackTiltAngle");
            m_pathTrackTiltAngle *= 0.017453292;

            m3d::SafeFloatAttrib(m_maxShootingTime, xmlNode, "MaxShootingTime");
            m3d::SafeFloatAttrib(m_defaultHover, xmlNode, "DefaultHover");
            m3d::SafeFloatAttrib(m_hoverForPlacingDrones, xmlNode, "HoverForPlacingDrones");
            m3d::SafeFloatAttrib(m_pathTrackTiltAngle, xmlNode, "PathTrackTiltAngle");
        }
        return result;
    }

    Boss03PrototypeInfo::Boss03PrototypeInfo()
    {
        m_maxDrones = 1;
        m_droneRelPosition = ZeroVector;
        m_droneRelRotation = IdentityQuaternion;
        m_maxHorizAngularVelocity = 0.0;
        m_horizAngularAcceleration = 0.0;
        m_maxVertAngularVelocity = 0.0;
        m_vertAngularAcceleration = 0.0;
        m_maxLinearVelocity = 0.0;
        m_linearAcceleration = 0.0;
        m_pathTrackTiltAngle = 0.0;
        m_maxHealth = 1.0f;
        m_maxShootingTime = 1.0f;
        m_defaultHover = 10.0f;
        m_hoverForPlacingDrones = 10.0f;
    }

    void Boss03PrototypeInfo::PostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* Boss03PrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::Registration()
    {
    }

    Boss03::Boss03(Boss03PrototypeInfo const& prototype) : AnimatedComplexPhysicObj(prototype), m_health(prototype.m_maxHealth, 0.0, prototype.m_maxHealth)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::InflictDamage(DamageInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::StartPlacingDrone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss03::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Boss03::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::StartMoving(CVector const&, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Boss03::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss03::GetBaseClass()
    {
        return RT_CLASS_LOCAL(AnimatedComplexPhysicObj);
    }

    void Boss03::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::ApplyModifier(Modifier const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::NoticePlayer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss03::GetHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss03PrototypeInfo const* Boss03::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Boss03::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss03::GetMaxHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_Construct(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss03::~Boss03()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_SetAllPartsNotDamageable()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_UpdateLiveFightingWithWings(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_SetDroneSpawningStatus(DroneSpawningStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss03Part* Boss03::_GetBossPartByName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::_OnHealthValueBeforeApplyModifier(Modifier const&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::_ProceedSettingVerticalAngle(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_SpawnDroneNow()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_UpdatePathTracking(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss03Part* Boss03::_GetMainPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss03Part* Boss03::_GetDroneSpawner()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_SetLiveStatus(LiveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_RecalcFlyPath()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_UpdateLookAtPlayer(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_UpdateLivePlacingDrones(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_UpdateSwinging(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss03::_ProceedSettingHorizontalDirection(float, CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& Boss03::_DroneSpawningStatusToStr(DroneSpawningStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss03::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss03::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr const& Boss03::_LiveStatusToStr(LiveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss03::_SetPathTrackingStatus(PathTrackingStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss03::CalcKeyPartsMaxDurability() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

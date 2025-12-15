#include "physicunit.h"

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, AddWalkPathByName)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, SetWalkPathByName)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PhysicUnit)
    RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, AddWalkPathByName, "", "", "")
    RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, SetWalkPathByName, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PhysicUnit);

    bool PhysicUnitPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_FROM_MODEL);
            m3d::SafeFloatAttrib(m_walkSpeed, xmlNode, "WalkSpeed");
            m3d::SafeFloatAttrib(m_maxStandTime, xmlNode, "MaxStandTime");
            m3d::SafeFloatAttrib(m_turnSpeed, xmlNode, "TurnSpeed");
            m_turnSpeed *= 0.017453292;
        }
        return result;
    }

    Obj* PhysicUnitPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicUnitPrototypeInfo::PhysicUnitPrototypeInfo()
    {
        m_walkSpeed = 1.0f;
        m_turnSpeed = 1.0f;
        m_maxStandTime = 1.0f;
    }

    void PhysicUnit::SetCauseForce(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::OnCollideWithStandingVehicle()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus PhysicUnit::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicUnit::PhysicUnit(PhysicUnitPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_standTtl(0.0, 0.0, m_maxStandTime, -1.0),
        m_walkTtl(0.0, 0.0, 1.0, 0.0)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr PhysicUnit::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SetState(States)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::Registration()
    {
        m_propertiesMap["PathsNames"] = 77;
    }

    void PhysicUnit::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PhysicUnit::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int PhysicUnit::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SetCausePos(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicUnit::SetWalkPathByName(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SetPositionSelf(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicUnit::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicUnitPrototypeInfo const* PhysicUnit::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicUnit::AddWalkPathByName(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PhysicUnit::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void PhysicUnit::SetInitVelocities(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::SetDirection(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicUnit::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PhysicUnit::~PhysicUnit()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicUnit::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicUnit::_SetWalkState(WalkState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PhysicUnit::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* PhysicUnit::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

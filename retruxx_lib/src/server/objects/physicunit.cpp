#include "physicunit.h"

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, AddWalkPathByName)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, SetWalkPathByName)
{
    throw retruxx::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PhysicUnit)
        RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, AddWalkPathByName, "", "", "")
        RT_CLASS_EXPORT(PhysicUnit, m3d::METHOD, SetWalkPathByName, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PhysicUnit);

    bool PhysicUnitPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* PhysicUnitPrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    PhysicUnitPrototypeInfo::PhysicUnitPrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::SetCauseForce(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::OnCollideWithStandingVehicle()
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus PhysicUnit::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    PhysicUnit::PhysicUnit(PhysicUnitPrototypeInfo const& prototype) :
        SimplePhysicObj(prototype),
        m_standTtl(0.0, 0.0, m_maxStandTime, -1.0),
        m_walkTtl(0.0, 0.0, 1.0, 0.0)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr PhysicUnit::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::SetState(States)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::Registration()
    {
        m_propertiesMap["PathsNames"] = 77;
    }

    void PhysicUnit::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::RenderDebugInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* PhysicUnit::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int PhysicUnit::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::SetCausePos(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool PhysicUnit::SetWalkPathByName(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::SetPositionSelf(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool PhysicUnit::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    PhysicUnitPrototypeInfo const* PhysicUnit::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool PhysicUnit::AddWalkPathByName(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* PhysicUnit::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void PhysicUnit::SetInitVelocities(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::SetDirection(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool PhysicUnit::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    PhysicUnit::~PhysicUnit()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool PhysicUnit::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void PhysicUnit::_SetWalkState(WalkState)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* PhysicUnit::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* PhysicUnit::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

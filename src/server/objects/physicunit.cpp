#include "physicunit.h"

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, AddWalkPathByName)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(PhysicUnit, SetWalkPathByName)
{
    throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    Obj* PhysicUnitPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    PhysicUnitPrototypeInfo::PhysicUnitPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::SetCauseForce(float)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::OnCollideWithStandingVehicle()
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus PhysicUnit::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    PhysicUnit::PhysicUnit(PhysicUnitPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    CStr PhysicUnit::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::SetState(States)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PhysicUnit::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    int PhysicUnit::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::SetCausePos(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicUnit::SetWalkPathByName(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::SetPositionSelf(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicUnit::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    PhysicUnitPrototypeInfo const* PhysicUnit::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicUnit::AddWalkPathByName(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* PhysicUnit::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void PhysicUnit::SetInitVelocities(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::SetDirection(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicUnit::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    PhysicUnit::~PhysicUnit()
    {
        throw std::logic_error("Not implemented");
    }

    bool PhysicUnit::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void PhysicUnit::_SetWalkState(WalkState)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* PhysicUnit::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* PhysicUnit::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

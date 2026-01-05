#include "boss04drone.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04Drone, SetCustomControl)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04Drone)
    RT_CLASS_EXPORT(Boss04Drone, m3d::METHOD, SetCustomControl, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04Drone);

    Obj* Boss04DronePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04DronePrototypeInfo::Boss04DronePrototypeInfo()
    {
        m_maxLinearVelocity = 0.0f;
    }

    bool Boss04DronePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ai::ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_maxLinearVelocity, xmlNode, "MaxLinearVelocity");
        }
        return result;
    }

    void Boss04Drone::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::SetCustomControl(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::InflictDamage(DamageInfo const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04Drone::Boss04Drone(Boss04DronePrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04DronePrototypeInfo const* Boss04Drone::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04Drone::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Boss04Drone::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04Drone::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::SetFlyPathName(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Boss04Drone::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss04Drone::GetMaxHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Boss04Drone::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::Registration()
    {
    }

    m3d::Class* Boss04Drone::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    float Boss04Drone::GetHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04Drone::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04Drone::~Boss04Drone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04Drone::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04Drone::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04Drone::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::_RecalcFlyPath()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04Drone::_UpdateLookAtPlayer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04Drone::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

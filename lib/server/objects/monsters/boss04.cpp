#include "boss04.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Boss04, NextState)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss04)
    RT_CLASS_EXPORT(Boss04, m3d::METHOD, NextState, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss04);

    void Boss04PrototypeInfo::PostLoad()
    {
        // TODO: implement ::PostLoad
    }

    Boss04PrototypeInfo::Boss04PrototypeInfo()
    {
        m_stationPrototypeId = -1;
        m_dronePrototypeId = -1;
        m_timeBetweenDrones.x = 10.0f;
        m_timeBetweenDrones.y = 20.0f;
        m_maxDrones = 0;
    }

    bool Boss04PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            // TODO: implement Boss04PrototypeInfo::LoadFromXML
        }
        return result;
    }

    Obj* Boss04PrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // TODO: implement ::RefreshFromXml
    }

    eGObjPropertySaveStatus Boss04::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::NextState()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04::Boss04(Boss04PrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Boss04::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::Registration()
    {
    }

    void Boss04::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::CreateChildren()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss04::GetMaxHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Boss04::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04PrototypeInfo const* Boss04::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04::RemoveChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::SetRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Boss04::GetHealth() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::AddChild(Obj*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Boss04::GetBaseClass()
    {
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    int Boss04::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::SetPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Boss04::~Boss04()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Boss04::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::_OnBoss04StationDestroyed(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::_SpawnDrone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::_SetState(Boss04State)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Boss04::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Boss04::_CreateStations(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

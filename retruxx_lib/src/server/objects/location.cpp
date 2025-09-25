#include "location.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Location)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Location);

    Obj* LocationPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    LocationPrototypeInfo::LocationPrototypeInfo()
    {
    }

    bool LocationPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            return 1;
        }
        return result;
    }

    LocationPrototypeInfo const* Location::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Location::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Location::GetLocationNameFromPassageAddress(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    Location::Location(LocationPrototypeInfo const& prototype) : SimplePhysicObj(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<Npc*, std::allocator<Npc*>> const& Location::GetNpcs() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::CanChildBeAdded(m3d::Class*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Location::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Location::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    void Location::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    CStr Location::GetLevelNameFromPassageAddress(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    Location* Location::GetCorrespondingPassageLocation() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::SetPositionSelf(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Location::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::IsObjectInside(int) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Location::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::SetActive(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::SetRadius(float)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::Registration()
    {
        m_propertiesMap["Radius"] = 49;
        m_propertiesMap["Active"] = 53;
        m_propertiesMap["LookingTimeOut"] = 52;
        m_propertiesMap["PassageAddress"] = 54;
        m_propertiesMap["ExitLocation"] = 55;
        m_propertiesMap["PassageActive"] = 56;
    }

    void Location::SetLocationType(LocationType)
    {
        throw std::logic_error("Not implemented");
    }

    int Location::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Location::GetPassageAddress() const
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::IsPassageActive() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Location::LocationType Location::GetLocationType() const
    {
        throw std::logic_error("Not implemented");
    }

    float Location::GetRadius() const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::OnObjectIn(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::OnObjectOut(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    void Location::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    Location::~Location()
    {
        throw std::logic_error("Not implemented");
    }

    void Location::_OnGameStart(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Location::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Location::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    bool Location::_MustCheckObject(Obj const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Location::_CheckIncomingOutgoingObjects(
        std::set<ref_ptr<Obstacle>, std::less<ref_ptr<Obstacle>>, std::allocator<ref_ptr<Obstacle>>>&)
    {
        throw std::logic_error("Not implemented");
    }
}

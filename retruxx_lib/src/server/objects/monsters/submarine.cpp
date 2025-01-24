#include "submarine.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, InitPosition)
{
    throw retruxx::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, StartMotionToPort)
{
    throw retruxx::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Submarine)
        RT_CLASS_EXPORT(Submarine, m3d::METHOD, InitPosition, "", "", "")
        RT_CLASS_EXPORT(Submarine, m3d::METHOD, StartMotionToPort, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Submarine);

    namespace
    {
        const char* STR_PLACE_POSITION = "PlacePosition";
        const char* STR_PORT_POSITION = "PortPosition";
    }

    void SubmarinePrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SubmarinePrototypeInfo::SubmarinePrototypeInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Obj* SubmarinePrototypeInfo::CreateTargetObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SubmarinePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::_InternalPostLoad()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Submarine::GetPropertyId(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Submarine::GetPropertySaveStatus(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr Submarine::GetPropertyName(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    SubmarinePrototypeInfo const* Submarine::GetPrototypeInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Submarine::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Submarine::OnEvent(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::RenderDebugInfo() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::Registration()
    {
        m_propertiesMap[STR_PLACE_POSITION] = 75;
        m_propertiesMap[STR_PORT_POSITION] = 76;
    }

    void Submarine::Update(float, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Submarine::Submarine(SubmarinePrototypeInfo const& prototype) : DummyObject(prototype)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::InitPosition()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::StartMotionToPort()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Submarine::SetPropertyById(int, m3d::AIParam const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Class* Submarine::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    void Submarine::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Submarine::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Submarine::~Submarine()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Submarine::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::_OnSkipCinematic(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::_UpdatePositionOnMove(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::_SetSubmarineState(eSubmarineState)
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Submarine::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    m3d::Object* Submarine::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::_WatchPlatformRotation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::_FillCameraStates(retruxx::vector<m3d::CameraPathState, retruxx::allocator<m3d::CameraPathState>>&,
        CVector const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Submarine::_OnTargetReached(Event const&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

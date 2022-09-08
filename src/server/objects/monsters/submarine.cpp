#include "submarine.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, InitPosition)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, StartMotionToPort)
{
    throw std::logic_error("Not implemented");
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Submarine)
        RT_CLASS_EXPORT(Submarine, m3d::METHOD, InitPosition, "", "", "")
        RT_CLASS_EXPORT(Submarine, m3d::METHOD, StartMotionToPort, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Submarine);

    void SubmarinePrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    SubmarinePrototypeInfo::SubmarinePrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    Obj* SubmarinePrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool SubmarinePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::_InternalPostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    int Submarine::GetPropertyId(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    eGObjPropertySaveStatus Submarine::GetPropertySaveStatus(int) const
    {
        throw std::logic_error("Not implemented");
    }

    CStr Submarine::GetPropertyName(int) const
    {
        throw std::logic_error("Not implemented");
    }

    SubmarinePrototypeInfo const* Submarine::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Submarine::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int Submarine::OnEvent(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::Registration()
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::Update(float, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    Submarine::Submarine(SubmarinePrototypeInfo const& prototype) : DummyObject(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::InitPosition()
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::StartMotionToPort()
    {
        throw std::logic_error("Not implemented");
    }

    bool Submarine::SetPropertyById(int, m3d::AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* Submarine::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    void Submarine::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        throw std::logic_error("Not implemented");
    }

    bool Submarine::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    Submarine::~Submarine()
    {
        throw std::logic_error("Not implemented");
    }

    bool Submarine::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::_OnSkipCinematic(Event const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::_UpdatePositionOnMove(float)
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::_SetSubmarineState(eSubmarineState)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Submarine::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* Submarine::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::_WatchPlatformRotation()
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::_FillCameraStates(std::vector<m3d::CameraPathState, std::allocator<m3d::CameraPathState>>&,
        CVector const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Submarine::_OnTargetReached(Event const&)
    {
        throw std::logic_error("Not implemented");
    }
}

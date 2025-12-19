#include "submarine.h"

#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, InitPosition)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, StartMotionToPort)
{
    RETRUXX_NOT_IMPLEMENTED;
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
        char const* STR_PLACE_POSITION = "PlacePosition";
        char const* STR_PORT_POSITION = "PortPosition";
    }  // namespace

    void SubmarinePrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // TODO: implement ::RefreshFromXml
    }

    SubmarinePrototypeInfo::SubmarinePrototypeInfo()
    {
        m_maxLinearVelocity = 0.0f;
        m_linearAcceleration = 0.0f;
        m_platformOpenFps = 2;
        m_vehicleMaxSpeed = 72.0f;
        m_vehicleRelativePosition = ZeroVector;
        m_bIsUpdating = true;
    }

    SubmarinePrototypeInfo::~SubmarinePrototypeInfo() = default;

    Obj* SubmarinePrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool SubmarinePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = DummyObjectPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_maxLinearVelocity, xmlNode, "MaxLinearVelocity");
            m3d::SafeFloatAttrib(m_linearAcceleration, xmlNode, "LinearAcceleration");
            m3d::SafeIntAttrib(m_platformOpenFps, xmlNode, "PlatformOpenFps");
            m3d::SafeFloatAttrib(m_vehicleMaxSpeed, xmlNode, "VehicleMaxSpeed");
            m3d::SafeFloatAttrib(m_maxLinearVelocity, xmlNode, "MaxLinearVelocity");
            m_maxLinearVelocity *= 0.27777779;
            m_vehicleMaxSpeed *= 0.27777779;
        }
        return result;
    }

    void Submarine::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Submarine::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Submarine::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Submarine::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SubmarinePrototypeInfo const* Submarine::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Submarine::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Submarine::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::Registration()
    {
        m_propertiesMap[STR_PLACE_POSITION] = 75;
        m_propertiesMap[STR_PORT_POSITION] = 76;
    }

    void Submarine::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Submarine::Submarine(SubmarinePrototypeInfo const& prototype) : DummyObject(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::InitPosition()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::StartMotionToPort()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Submarine::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Submarine::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    void Submarine::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Submarine::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Submarine::~Submarine()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Submarine::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::_OnSkipCinematic(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::_UpdatePositionOnMove(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::_SetSubmarineState(eSubmarineState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Submarine::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Submarine::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::_WatchPlatformRotation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::_FillCameraStates(retruxx::vector<m3d::CameraPathState, retruxx::allocator<m3d::CameraPathState>>&, CVector const&)
        const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Submarine::_OnTargetReached(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

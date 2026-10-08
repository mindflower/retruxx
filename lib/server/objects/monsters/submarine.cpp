#include "submarine.h"

#include <stdexcept>

#include "cinematic.h"
#include "m3dapp.h"
#include "impulses/i_impulses.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "core/log.h"
#include "scene/servers/dataserver.h"
#include "server/event.h"
#include "server/processmanager.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/physicbody.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, InitPosition)
{
    // RVA 0x802630
    auto* submarine = static_cast<ai::Submarine*>(context->asObject(0, "Submarine"));
    submarine->InitPosition();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Submarine, StartMotionToPort)
{
    // RVA 0x803FC0
    auto* submarine = static_cast<ai::Submarine*>(context->asObject(0, "Submarine"));
    submarine->StartMotionToPort();
    return 1;
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

        // The load point on the submarine's model where the player's vehicle is carried.
        CStr const LP_NAME_FOR_VEHICLE("LP_PART01");

        // v rotated by q (row vector times q's rotation matrix), as expanded inline in the
        // shipped code.
        CVector RotateByQuaternion(CVector const& v, Quaternion const& q)
        {
            float const xz = q.x * q.z;
            float const xw = q.x * q.w;
            float const xx = q.x * q.x;
            float const xy = q.x * q.y;
            float const zy = q.z * q.y;
            float const yy = q.y * q.y;
            float const zz = q.z * q.z;
            float const yw = q.y * q.w;
            float const zw = q.z * q.w;
            float const m11 = 1.0f - (zz + yy) * 2.0f;
            float const m21 = (xy - zw) * 2.0f;
            float const m31 = (yw + xz) * 2.0f;
            float const m12 = (zw + xy) * 2.0f;
            float const m22 = 1.0f - (zz + xx) * 2.0f;
            float const m32 = (zy - xw) * 2.0f;
            float const m13 = (xz - yw) * 2.0f;
            float const m23 = (xw + zy) * 2.0f;
            float const m33 = 1.0f - (yy + xx) * 2.0f;
            return CVector(
                (v.x * m11 + m21 * v.y) + m31 * v.z,
                (v.x * m12 + m22 * v.y) + m32 * v.z,
                (v.x * m13 + m23 * v.y) + m33 * v.z);
        }
    }  // namespace

    void SubmarinePrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x802A20
        // Tunes the model: the platform animations do not chain on, the opening one plays at
        // PlatformOpenFps, and the vehicle rides at the model's LP_PART01 load point.
        // NOTE: skips DummyObjectPrototypeInfo::RefreshFromXml.
        SimplePhysicObjPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
        m3d::DataServer* server = &M3D_APP->GetAnimatedModelsServer();
        int const itemId = server->GetItemByName(GetEngineModelName().c_str(), true);
        if (itemId == -1)
        {
            return;
        }
        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(itemId, 16394, &mdl);
        if (!mdl)
        {
            return;
        }
        mdl->SetNextForAnimation(AT_STAND1, -1);
        mdl->SetNextForAnimation(AT_STAND2, -1);
        short const openAnimation = mdl->m_animRemap[1];
        if (openAnimation >= 0 && openAnimation < mdl->m_header.m_numAnimations)
        {
            mdl->m_animations[openAnimation].m_fps = static_cast<short>(m_platformOpenFps);
        }
        CMatrix mat;
        if (mdl->GetBoneMatrixByName(LP_NAME_FOR_VEHICLE, mat, false))
        {
            m_vehicleRelativePosition = CVector(mat._41, mat._42, mat._43);
        }
        else
        {
            M3D_LOG_ERR(CStr("Error: LoadPoint not found: ") + LP_NAME_FOR_VEHICLE + CStr(" for ") + GetEngineModelName());
        }
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
        // RVA 0x803780
        return new Submarine(*this);
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

    void Submarine::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        DummyObject::GetPropertiesIDs(Props);
    }

    void Submarine::_InternalPostLoad()
    {
        // RVA 0x802C70
        // The submarine sails from PlacePosition to PortPosition; it stops where its vehicle
        // load point is over the port.
        SimplePhysicObj::_InternalPostLoad();
        float const dz = m_portPosition.z - m_placePosition.z;
        float const dy = m_portPosition.y - m_placePosition.y;
        float const dx = m_portPosition.x - m_placePosition.x;
        float const invLen = static_cast<float>(1.0 / sqrt(dz * dz + dy * dy + dx * dx + 1.1920929e-7));
        m_moveDirection = CVector(invLen * dx, dy * invLen, dz * invLen);
        InitPosition();
        // NOTE: the model name is copied and never used.
        CStr const modelName = GetPrototypeInfo()->GetEngineModelName();
        CVector const offset = RotateByQuaternion(GetPrototypeInfo()->m_vehicleRelativePosition, GetRotation());
        m_placeToMove = CVector(m_portPosition.x - offset.x, m_portPosition.y - offset.y, m_portPosition.z - offset.z);
    }

    int Submarine::GetPropertyId(char const* PropertyName) const
    {
        // RVA 0x803160
        auto it = m_propertiesMap.find(PropertyName);
        if (it != m_propertiesMap.end())
        {
            return it->second;
        }
        return DummyObject::GetPropertyId(PropertyName);
    }

    eGObjPropertySaveStatus Submarine::GetPropertySaveStatus(int id) const
    {
        // RVA 0x8030E0
        auto it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return DummyObject::GetPropertySaveStatus(id);
    }

    CStr Submarine::GetPropertyName(int id) const
    {
        // RVA 0x803480
        // A linear search, since the map is keyed by name.
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return DummyObject::GetPropertyName(id);
    }

    SubmarinePrototypeInfo const* Submarine::GetPrototypeInfo() const
    {
        // RVA 0x802FE0
        // NOTE: the prototype is cast without a type check.
        return static_cast<SubmarinePrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Submarine::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x802060
        // NOTE: skips DummyObject::SaveToXML.
        Obj::SaveToXML(xmlFile, xmlNode);
        ref_ptr entryPathNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "EntryPath");
        xmlNode->AddChild(entryPathNode);
        m_entryPath.SaveToXml(xmlFile, entryPathNode);
        xmlNode->SetAttribute("NextMap", m_nextMap.c_str());
        xmlNode->SetAttribute("NextMapLocation", m_nextMapLocation.c_str());
        if (m_nextMapAngle != -1)
        {
            xmlNode->SetAttribute("NextMapAngle", CStr(m_nextMapAngle).c_str());
        }
    }

    m3d::Class* Submarine::GetClass() const
    {
        // RVA 0x801CC0
        return RT_CLASS_LOCAL(Submarine);
    }

    int Submarine::OnEvent(Event const& evn)
    {
        // RVA 0x802650
        // NOTE: skips DummyObject::OnEvent.
        int const result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_TARGET_REACHED)
        {
            _OnTargetReached(evn);
            return 1;
        }
        if (evn.m_eventId == GE_SKIP_CINEMATIC || evn.m_eventId == GE_END_CINEMATIC)
        {
            _OnSkipCinematic(evn);
            return 1;
        }
        return result;
    }

    void Submarine::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x801F50
        // NOTE: skips DummyObject::LoadFromXML.
        Obj::LoadFromXML(xmlFile, xmlNode);
        ref_ptr entryPathNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(entryPathNode, "EntryPath");
        m_entryPath.LoadFromXml(GetDebugDescription(), xmlFile, entryPathNode);
        m3d::SafeStrAttrib(m_nextMap, xmlNode, "NextMap");
        m3d::SafeStrAttrib(m_nextMapLocation, xmlNode, "NextMapLocation");
        m3d::SafeIntAttrib(m_nextMapAngle, xmlNode, "NextMapAngle");
    }

    void Submarine::RenderDebugInfo() const
    {
        // RVA 0x802210
        SimplePhysicObj::RenderDebugInfo();
        m_entryPath.RenderDebugInfo();
        M3D_APP->DrawLine(m_placePosition, m_placeToMove, 0xFFFFFFFF);
        M3D_APP->DrawLine(m_placeToMove, m_portPosition, 0xFFFFFFFF);
        // A green cross on the vehicle load point.
        Quaternion const rot = GetRotation();
        float const xz = rot.x * rot.z;
        float const xw = rot.x * rot.w;
        float const xx = rot.x * rot.x;
        float const xy = rot.x * rot.y;
        float const zy = rot.z * rot.y;
        float const zz = rot.z * rot.z;
        float const yw = rot.y * rot.w;
        float const yy = rot.y * rot.y;
        float const zw = rot.z * rot.w;
        CVector const& rel = GetPrototypeInfo()->m_vehicleRelativePosition;
        CVector const offset(
            ((xy - zw) * 2.0f * rel.y + rel.x * (1.0f - (zz + yy) * 2.0f)) + (yw + xz) * 2.0f * rel.z,
            (rel.x * ((zw + xy) * 2.0f) + (1.0f - (zz + xx) * 2.0f) * rel.y) + (zy - xw) * 2.0f * rel.z,
            (rel.x * ((xz - yw) * 2.0f) + (xw + zy) * 2.0f * rel.y) + (1.0f - (yy + xx) * 2.0f) * rel.z);
        CVector const pos = GetPosition();
        M3D_APP->DrawCross(CVector(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z), 2.0f, 0xFF00FF00);
    }

    void Submarine::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // Declared in the PDB but never emitted in the shipped build; the usual property-map pattern.
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        DummyObject::GetPropertiesNames(Props);
    }

    void Submarine::Registration()
    {
        // RVA 0x803700
        m_propertiesMap[STR_PLACE_POSITION] = 75;
        m_propertiesMap[STR_PORT_POSITION] = 76;
    }

    void Submarine::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x8042B0
        // NOTE: skips DummyObject::Update.
        SimplePhysicObj::Update(elapsedTime, workTime);
        if (elapsedTime > 0.0f)
        {
            if (m_state == SUBMARINE_MOVES)
            {
                _UpdatePositionOnMove(elapsedTime);
            }
            else if (m_state == SUBMARINE_OPENS)
            {
                _WatchPlatformRotation();
            }
        }
    }

    Submarine::Submarine(SubmarinePrototypeInfo const& prototype) :
        DummyObject(prototype),
        m_nextMapAngle(-1)
    {
        // RVA 0x8035C0 - moved by hand in Update, so the geometry starts off.
        m_physicState |= 1u;
        SetCorrectEnabledCellsCounter();
        SimplePhysicObj::EnableGeometry(false);
        m_linearVelocity = ZeroVector;
        m_nextMap = CStr("");
        m_nextMapLocation = CStr("");
    }

    void Submarine::InitPosition()
    {
        // RVA 0x802150
        // Back at the start, facing along the (horizontal) way to the port, platform closed.
        SetPosition(m_placePosition);
        float const x = m_moveDirection.x;
        float const z = m_moveDirection.z;
        float const invLen = static_cast<float>(1.0 / sqrt(z * z + x * x + 1.1920929e-7));
        SetDirection(CVector(invLen * x, invLen * 0.0f, invLen * z));
        m_physicBody->SetNodeAnimAction(0, true);
        m_state = SUBMARINE_IDLE;
    }

    void Submarine::StartMotionToPort()
    {
        // RVA 0x803E60
        _SetSubmarineState(SUBMARINE_MOVES);
    }

    bool Submarine::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        // RVA 0x801CD0
        if (propertyId == 75)
        {
            m_placePosition = newValue.GetAsVector();
            return true;
        }
        if (propertyId == 76)
        {
            m_portPosition = newValue.GetAsVector();
            return true;
        }
        return DummyObject::SetPropertyById(propertyId, newValue);
    }

    m3d::Class* Submarine::GetBaseClass()
    {
        return RT_CLASS_LOCAL(DummyObject);
    }

    void Submarine::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x803560
        // The default save status is not stored.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
    }

    bool Submarine::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x803F50
        if (propertyId != 75 && propertyId != 76)
        {
            return DummyObject::_GetPropertyDefaultInternal(propertyId, retVal);
        }
        // NOTE: both positions default to an uninitialised vector in the shipped build; here it
        // is the zero vector.
        retVal = CVector(0.0f, 0.0f, 0.0f);
        return true;
    }

    // RVA 0x803010
    Submarine::~Submarine() = default;

    bool Submarine::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x803E70
        if (propertyId == 75)
        {
            retVal = m_placePosition;
            return true;
        }
        if (propertyId == 76)
        {
            retVal = m_portPosition;
            return true;
        }
        return DummyObject::_GetPropertyInternal(propertyId, retVal);
    }

    void Submarine::_OnSkipCinematic(Event const&)
    {
        // RVA 0x802590
        // Skipping (or finishing) the cinematic sends the player on to the next map at once.
        Vehicle* vehicle = thePlayer->GetVehicle();
        if (m_nextMap.c_str() && strlen(m_nextMap.c_str()) && m_nextMapLocation.c_str() && strlen(m_nextMapLocation.c_str()) && vehicle)
        {
            vehicle->UnlimitMaxSpeed();
            theObjects->PassToMap(m_nextMap, m_nextMapLocation, m_nextMapAngle, true);
        }
    }

    void Submarine::_UpdatePositionOnMove(float elapsedTime)
    {
        // RVA 0x803FE0
        // Sails towards m_placeToMove with smooth acceleration and braking; on arrival the
        // platform starts to open.
        SubmarinePrototypeInfo const* prototype = GetPrototypeInfo();
        CVector const pos = GetPosition();
        float const dy = m_placeToMove.y - pos.y;
        float const dx = m_placeToMove.x - pos.x;
        float const dz = m_placeToMove.z - pos.z;
        float const distSq = (dx * dx + dy * dy) + dz * dz;
        float const maxVelocity = prototype->m_maxLinearVelocity;
        float const acceleration = prototype->m_linearAcceleration;
        float const dist = static_cast<float>(sqrt(distSq));
        float const invLen = static_cast<float>(1.0 / sqrt(distSq + 1.1920929e-7f));
        CVector const dirToMove(invLen * dx, invLen * dy, invLen * dz);
        float const velocity = (m_linearVelocity.z * dirToMove.z + m_linearVelocity.y * dirToMove.y) + m_linearVelocity.x * dirToMove.x;
        float newPos;
        float newVel;
        bool const reached = GetSmoothAcceleratedValue(elapsedTime, dist, 0.0f, velocity, acceleration, maxVelocity, newPos, newVel);
        m_linearVelocity = CVector(dirToMove.x * newVel, dirToMove.y * newVel, dirToMove.z * newVel);
        CVector const curPos = GetPosition();
        SetPosition(CVector(curPos.x + dirToMove.x * newPos, curPos.y + dirToMove.y * newPos, curPos.z + dirToMove.z * newPos));
        if (reached)
        {
            m_linearVelocity = ZeroVector;
            if (thePlayer->GetVehicle())
            {
                SetNodeAction(1, true);
                m_state = SUBMARINE_OPENS;
            }
        }
    }

    void Submarine::_SetSubmarineState(eSubmarineState newState)
    {
        // RVA 0x803960
        Vehicle* vehicle = thePlayer->GetVehicle();
        if (!vehicle)
        {
            return;
        }
        SubmarinePrototypeInfo const* prototype = GetPrototypeInfo();

        if (newState == SUBMARINE_MOVES)
        {
            // Two camera shots: along the entry path (or from where the camera is), then from
            // above the player's vehicle. The player hears when the cinematic is skipped or ends.
            SetNodeAction(0, true);
            std::vector<m3d::CameraPathState> cameraStates;
            _FillCameraStates(cameraStates, m_portPosition);
            m3d::Cinematic* cinematic = M3D_APP->m_cinematic;
            cinematic->LoadDefaults();
            cinematic->SetFlags(1);
            M3D_APP->OnChangeMode(m3d::AuxImpulseInfo(2, true, -1, 0, 0));
            cinematic->SetLookTo(true);
            cinematic->SetAimToID(GetId());
            cinematic->SetWaitWhenStop(true);
            cinematic->SetCameraStates(cameraStates);
            cinematic->Play(10.0f);

            cameraStates.resize(1);
            CVector const vehiclePos = vehicle->GetPosition();
            cameraStates[0].m_point = CVector(vehiclePos.x, vehiclePos.y + 25.0f, vehiclePos.z);
            cameraStates[0].m_rotation = IdentityQuaternion;
            cameraStates[0].m_zoom = 1.0f;
            cinematic->SetFlags(2);
            cinematic->SetLookTo(true);
            cinematic->SetAimToID(vehicle->GetId());
            cinematic->SetWaitWhenStop(true);
            cinematic->SetCameraStates(cameraStates);
            cinematic->Play(10.0f);
            cinematic->StartCinematic();

            vehicle->SetHandBrake();
            theProcessManager->PostMessageA(GE_SUBSCRIBE, thePlayer->GetId(), GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_SKIP_CINEMATIC)), m3d::AIParam(), 1);
            theProcessManager->PostMessageA(GE_SUBSCRIBE, thePlayer->GetId(), GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_END_CINEMATIC)), m3d::AIParam(), 1);
            m_state = newState;
            return;
        }
        if (newState == SUBMARINE_OPENS)
        {
            SetNodeAction(1, true);
            m_state = SUBMARINE_OPENS;
            return;
        }
        if (newState != SUBMARINE_WAITS)
        {
            m_state = newState;
            return;
        }

        // The platform is open: drive the player's vehicle aboard, along the entry path or
        // straight in over the port, and hear when it gets there.
        M3D_APP->m_cinematic->StartCinematic();
        std::vector<CVector2> const& vehiclePoints = m_entryPath.GetVehiclePoints();
        retruxx::vector<CVector2> points;
        if (!vehiclePoints.empty())
        {
            points.assign(vehiclePoints.begin(), vehiclePoints.end());
            points.push_back(CVector2(m_portPosition.x, m_portPosition.z));
        }
        else
        {
            CVector const vehiclePos = vehicle->GetPosition();
            points.push_back(CVector2(vehiclePos.x, vehiclePos.z));
            points.push_back(CVector2(m_moveDirection.x * 30.0f + m_portPosition.x, m_portPosition.z + m_moveDirection.z * 30.0f));
            points.push_back(CVector2(m_portPosition.x, m_portPosition.z));
        }
        vehicle->SetExternalPath(points);
        vehicle->LimitMaxSpeed(prototype->m_vehicleMaxSpeed);
        theProcessManager->PostMessageA(GE_SUBSCRIBE, vehicle->GetId(), GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_TARGET_REACHED)), m3d::AIParam(), 1);
        m_state = newState;
    }

    m3d::Object* Submarine::CreateObject()
    {
        // RVA 0x802860
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Submarine::Clone()
    {
        // RVA 0x8026A0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void Submarine::_WatchPlatformRotation()
    {
        // RVA 0x804210
        // Once the opening animation is over the submarine waits for the vehicle.
        if (m_physicBody->GetNodeRealAction() == -1)
        {
            _SetSubmarineState(SUBMARINE_WAITS);
            CauseEvent(GE_TARGET_REACHED, 0.0f, m3d::AIParam(GetId()), m3d::AIParam());
        }
    }

    void Submarine::_FillCameraStates(retruxx::vector<m3d::CameraPathState, retruxx::allocator<m3d::CameraPathState>>& cameraStates, CVector const& offset) const
    {
        // RVA 0x8037B0
        // The entry path's camera points shifted by offset, or just the current camera position.
        std::vector<CVector> const& cameraPoints = m_entryPath.GetCameraPoints();
        if (!cameraPoints.empty())
        {
            cameraStates.resize(cameraPoints.size());
            for (size_t i = 0; i < cameraPoints.size(); ++i)
            {
                CVector const& p = cameraPoints[i];
                cameraStates[i].m_point = CVector(p.x + offset.x, p.y + offset.y, p.z + offset.z);
                cameraStates[i].m_rotation = IdentityQuaternion;
                cameraStates[i].m_zoom = 1.0f;
            }
        }
        else
        {
            cameraStates.resize(1);
            cameraStates[0].m_point = M3D_APP->m_curCamera.m_worldOrigin;
            cameraStates[0].m_rotation = IdentityQuaternion;
            cameraStates[0].m_zoom = 1.0f;
        }
    }

    void Submarine::_OnTargetReached(Event const& evn)
    {
        // RVA 0x8024F0
        // The player's vehicle is aboard: on to the next map, after the fade.
        int const senderId = evn.m_senderObjId;
        Vehicle* vehicle = thePlayer->GetVehicle();
        if (m_nextMap.c_str() && strlen(m_nextMap.c_str()) && m_nextMapLocation.c_str() && strlen(m_nextMapLocation.c_str()) && vehicle &&
            senderId == vehicle->GetId())
        {
            vehicle->UnlimitMaxSpeed();
            theObjects->PassToMap(m_nextMap, m_nextMapLocation, m_nextMapAngle, false);
        }
    }
}  // namespace ai

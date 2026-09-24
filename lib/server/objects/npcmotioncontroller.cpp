#include "npcmotioncontroller.h"

#include <stdexcept>

#include "player.h"
#include "team.h"
#include "vehicle.h"
#include "m3dapp.h"
#include "core/kernel.h"
#include "server/event.h"
#include "server/processmanager.h"
#include "base/objcontainer.h"
#include "base/physicobj.h"
#include "base/prototypemanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(NPCMotionController, getVehicleUnderControl)
{
    // RVA 0x7FB940
    auto* controller = static_cast<ai::NPCMotionController*>(context->asObject(0, "NPCMotionController"));
    context->pushObject(controller->getVehicleUnderControl());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(NPCMotionController, setVehicleUnderControl)
{
    // RVA 0x7FC060
    auto* controller = static_cast<ai::NPCMotionController*>(context->asObject(0, "NPCMotionController"));
    auto* vehicle = static_cast<ai::Vehicle*>(context->asObject(1, "Vehicle"));
    controller->setVehicleUnderControl(vehicle);
    return 1;
}


namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(NPCMotionController)
        RT_CLASS_EXPORT(NPCMotionController, m3d::METHOD, getVehicleUnderControl, "", "", "")
        RT_CLASS_EXPORT(NPCMotionController, m3d::METHOD, setVehicleUnderControl, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(NPCMotionController);

    NPCMotionControllerPrototypeInfo::NPCMotionControllerPrototypeInfo()
    {
    }

    Obj* NPCMotionControllerPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FB380
        return new NPCMotionController(*this);
    }

    bool NPCMotionControllerPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    int NPCMotionController::OnEvent(Event const& evn)
    {
        // RVA 0x7FADC0 - the controller goes away with its vehicle.
        int const result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_OBJECT_DIE)
        {
            if (evn.m_senderObjId == m_vehicleUnderControlId)
            {
                Remove();
            }
            return 1;
        }
        return result;
    }

    void NPCMotionController::setVehicleUnderControl(Vehicle* vehicle)
    {
        // RVA 0x7FBD40 - swaps the death subscription from the old vehicle to the new one.
        if (!vehicle)
        {
            SYS_ERROR("vehicle");
        }
        if (m_vehicleUnderControlId != -1)
        {
            theProcessManager->PostMessageA(GE_UNSUBSCRIBE, m_vehicleUnderControlId, GetId(), 0.0f,
                m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
        }
        m_vehicleUnderControlId = vehicle->GetId();
        if (m_vehicleUnderControlId != -1)
        {
            theProcessManager->PostMessageA(GE_SUBSCRIBE, m_vehicleUnderControlId, GetId(), 0.0f,
                m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
        }
        vehicle->SetNpcMotionControllerId(GetId());
    }

    void NPCMotionController::setVehicleUnderControl(m3d::Object* obj)
    {
        // RVA 0x7FC050 - a thunk to the Vehicle overload; the object is not type-checked.
        setVehicleUnderControl(static_cast<Vehicle*>(obj));
    }

    void NPCMotionController::RenderDebugInfo() const
    {
        // RVA 0x7FAFE0
        M3D_APP->DrawBoundingRadius(m_lastDesiredPosition, 1.0f, 0xFF0000FF);
    }

    void NPCMotionController::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FAEE0 - NOTE: CharacteristicPeriod is saved by nobody and not loaded either.
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_vehicleUnderControlId, xmlNode, "VehicleUnderControl");
        int style = m_style;
        m3d::SafeIntAttrib(style, xmlNode, "Style");
        m_style = static_cast<MotionStyle>(style);
        m3d::SafeFloatAttrib(m_elapsedStateTime, xmlNode, "ElapsedStateTime");
        m3d::SafeFloatAttrib(m_timeForState, xmlNode, "TimeForState");
        m3d::SafeFloatAttrib(m_characteristicDist, xmlNode, "CharacteristicDist");
    }

    void NPCMotionController::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x7FB9A0 - an NPC vehicle without a role wanders around the player's vehicle,
        // switching to a random motion style every 1-100 seconds.
        Obj::Update(elapsedTime, workTime);
        Vehicle* vehicle = getVehicleUnderControl();
        if (!vehicle || vehicle->GetRole())
        {
            return;
        }

        m_elapsedStateTime = m_elapsedStateTime + elapsedTime;
        if (m_elapsedStateTime > m_timeForState)
        {
            m_timeForState = static_cast<float>(rand()) * 0.000030518509f * 99.0f + 1.0f;
            m_elapsedStateTime = 0.0f;
            m_style = static_cast<MotionStyle>(rand() & 7);
            m_characteristicDist = static_cast<float>(rand()) * 0.000030518509f * 40.0f + 10.0f;
            m_characteristicPeriod = static_cast<float>(rand()) * 0.000030518509f * 8.0f + 2.0f;
        }

        // Positions are in the player's frame: x to the right, y ahead.
        float const phase = m_elapsedStateTime / m_characteristicPeriod;
        CVector2 desired;
        switch (m_style)
        {
        case msWithPlayer:
            desired = CVector2(0.0f, 0.0f);
            break;
        case msCircle:
        {
            // Keep circling: aim 30 degrees further round from where the vehicle is now.
            CVector2 const current = getCurrentVehiclePosition();
            double const c = std::cos(0.5235987901687622);
            double const s = std::sin(0.5235987901687622);
            desired.x = static_cast<float>(c * current.x - s * current.y);
            desired.y = static_cast<float>(s * current.x + c * current.y);
            if (std::sqrt(double(desired.y) * desired.y + double(desired.x) * desired.x) < 0.0000099999997)
            {
                desired = CVector2(1.0f, 0.0f);
            }
            CVector2 const n = desired.normalize();
            desired = CVector2(m_characteristicDist * n.x, m_characteristicDist * n.y);
            break;
        }
        case msPendulumRight:
            desired.x = 0.0f - m_characteristicDist;
            desired.y = static_cast<float>(std::sin(phase) * m_characteristicDist);
            break;
        case msPendulumLeft:
            desired.x = m_characteristicDist;
            desired.y = static_cast<float>(std::sin(phase) * m_characteristicDist);
            break;
        case msPendulumTop:
            desired.y = m_characteristicDist;
            desired.x = static_cast<float>(std::sin(phase) * m_characteristicDist);
            break;
        case msPendulumBottom:
            desired.y = 0.0f - m_characteristicDist;
            desired.x = static_cast<float>(std::sin(phase) * m_characteristicDist);
            break;
        case msChief:
            desired = CVector2(0.0f, m_characteristicDist);
            break;
        case msChiefPendulum:
            desired.y = m_characteristicDist;
            desired.x = static_cast<float>(std::sin(phase) * m_characteristicDist * 0.33333334f);
            break;
        default:
            return;
        }
        setDesiredVehiclePosition(desired);
    }

    m3d::Class* NPCMotionController::GetClass() const
    {
        // RVA 0x7FAD20
        return RT_CLASS_LOCAL(NPCMotionController);
    }

    Vehicle* NPCMotionController::getVehicleUnderControl() const
    {
        // RVA 0x7FB520 - NOTE: not type-checked.
        return static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_vehicleUnderControlId));
    }

    NPCMotionControllerPrototypeInfo const* NPCMotionController::GetPrototypeInfo() const
    {
        // RVA 0x7FB4F0 - NOTE: the prototype is cast without a type check.
        return static_cast<NPCMotionControllerPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* NPCMotionController::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void NPCMotionController::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7FB3B0
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("VehicleUnderControl", CStr(m_vehicleUnderControlId).c_str());
        xmlNode->SetAttribute("Style", CStr(static_cast<int>(m_style)).c_str());
        xmlNode->SetAttribute("ElapsedStateTime", CStr(m_elapsedStateTime).c_str());
        xmlNode->SetAttribute("TimeForState", CStr(m_timeForState).c_str());
        xmlNode->SetAttribute("CharacteristicDist", CStr(m_characteristicDist).c_str());
    }

    void NPCMotionController::SetPassedToAnotherMapStatus()
    {
        // RVA 0x7FB8C0 - passes on with its vehicle, or with the vehicle's whole team. NOTE: without a
        // vehicle the controller itself is not marked either.
        Obj* vehicle = theObjects->GetEntityByObjId(m_vehicleUnderControlId);
        if (!vehicle)
        {
            return;
        }
        Obj::SetPassedToAnotherMapStatus();
        Obj* parent = vehicle->GetParent();
        if (parent && parent->IsKindOf(RT_CLASS_LOCAL(Team)))
        {
            parent->SetPassedToAnotherMapStatus();
        }
        else
        {
            vehicle->SetPassedToAnotherMapStatus();
        }
    }

    NPCMotionController::NPCMotionController(NPCMotionControllerPrototypeInfo const& info) :
        Obj(info),
        m_vehicleUnderControlId(-1),
        m_style(msWithPlayer),
        m_elapsedStateTime(0.0f),
        m_timeForState(0.0f),
        m_lastDesiredPosition(0.0f, 0.0f, 0.0f),
        m_characteristicDist(10.0f),
        m_characteristicPeriod(5.0f)
    {
        // RVA 0x7FAE20
    }

    // RVA 0x7FAD70
    NPCMotionController::~NPCMotionController() = default;

    m3d::Object* NPCMotionController::CreateObject()
    {
        // RVA 0x7FB1C0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    CVector2 NPCMotionController::getCurrentVehiclePosition() const
    {
        // RVA 0x7FB720 - the controlled vehicle's position in the player's frame: x to the right of
        // the player's motion (or heading, below 5 m/s), y ahead.
        Obj* vehicle = theObjects->GetEntityByObjId(m_vehicleUnderControlId);
        if (!vehicle)
        {
            return CVector2(0.0f, 0.0f);
        }
        CVector const vPos = getPhysicObjOrPhysicBodyPosition(vehicle);
        Vehicle* playerVehicle = thePlayer->GetVehicle();
        if (!playerVehicle)
        {
            return CVector2(0.0f, 0.0f);
        }
        CVector const pPos = getPhysicObjOrPhysicBodyPosition(playerVehicle);
        CVector pVel = getPhysicObjOrPhysicBodyLinearVelocity(playerVehicle);
        if (std::sqrt(double(pVel.y) * pVel.y + double(pVel.x) * pVel.x + double(pVel.z) * pVel.z) < 5.0)
        {
            pVel = getPhysicObjOrPhysicBodyDirection(playerVehicle);
        }
        CVector2 dir = CVector2(pVel.x, pVel.z).normalize();
        if (!(std::sqrt(double(dir.y) * dir.y + double(dir.x) * dir.x) >= 0.0099999998))
        {
            dir = CVector2(1.0f, 0.0f);
        }
        float const dx = vPos.x - pPos.x;
        float const dz = vPos.z - pPos.z;
        return CVector2(dx * dir.y - dz * dir.x, dz * dir.y + dx * dir.x);
    }

    void NPCMotionController::_OnObjectDie(Event const& evn)
    {
        // RVA 0x7FAD80
        if (evn.m_senderObjId == m_vehicleUnderControlId)
        {
            Remove();
        }
    }

    m3d::Object* NPCMotionController::Clone()
    {
        // RVA 0x7FB000
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void NPCMotionController::setDesiredVehiclePosition(CVector2 const& point)
    {
        // RVA 0x7FB560 - converts a point in the player's frame (see getCurrentVehiclePosition) to
        // the world and sends the controlled vehicle there.
        Vehicle* vehicle = getVehicleUnderControl();
        if (!vehicle)
        {
            return;
        }
        Vehicle* playerVehicle = thePlayer->GetVehicle();
        if (!playerVehicle)
        {
            return;
        }
        CVector const vPos = playerVehicle->GetGeometricCenter();
        CVector vVel = playerVehicle->GetLinearVelocity();
        if (std::sqrt(double(vVel.y) * vVel.y + double(vVel.x) * vVel.x + double(vVel.z) * vVel.z) < 5.0)
        {
            vVel = playerVehicle->GetDirection();
        }
        CVector2 dir = CVector2(vVel.x, vVel.z).normalize();
        if (!(std::sqrt(double(dir.y) * dir.y + double(dir.x) * dir.x) >= 0.0099999998))
        {
            dir = CVector2(1.0f, 0.0f);
        }
        CVector desiredPos;
        desiredPos.x = point.y * dir.x + point.x * dir.y + vPos.x;
        desiredPos.y = vPos.y;
        desiredPos.z = (0.0f - dir.x) * point.x + point.y * dir.y + vPos.z;
        m_lastDesiredPosition = desiredPos;
        vehicle->SetExternalDestination(desiredPos);
    }
}

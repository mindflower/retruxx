#include "vehiclerolependulum.h"

#include <stdexcept>

#include "core/ini.h"
#include "server/utils.h"
#include "server/objects/vehicle.h"
#include <server/objects/base/prototypemanager.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRolePendulum)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRolePendulum);

    VehicleRolePendulumPrototypeInfo::VehicleRolePendulumPrototypeInfo() = default;

    Obj* VehicleRolePendulumPrototypeInfo::CreateTargetObject() const
    {
        return new VehicleRolePendulum(*this);
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        return 2.0;
    }

    bool VehicleRolePendulumPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeVector2Attrib(m_oppressionShift, xmlNode, "OppressionShift");
            m3d::SafeFloatAttrib(m_A, xmlNode, "A");
            m3d::SafeFloatAttrib(m_B, xmlNode, "B");
            return 1;
        }
        return result;
    }

    VehicleRolePendulum::VehicleRolePendulum(VehicleRolePendulumPrototypeInfo const& prototype) : VehicleRole(prototype)
    {
        this->m_Direction.x = 1.0;
        this->m_Direction.y = 0.0;
        this->m_angle = 0.0;
    }

    void VehicleRolePendulum::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleRolePendulumPrototypeInfo const* VehicleRolePendulum::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), VehicleRolePendulumPrototypeInfo const);
    }

    m3d::Class* VehicleRolePendulum::GetClass() const
    {
        return RT_CLASS_LOCAL(VehicleRolePendulum);
    }

    void VehicleRolePendulum::setTargetObj(Obj const* obj)
    {
        VehicleRole::setTargetObj(obj);
    }

    void VehicleRolePendulum::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* VehicleRolePendulum::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehicleRole);
    }

    void VehicleRolePendulum::setTargetVehicle(Vehicle const* vehicle)
    {
        VehicleRole::setTargetVehicle(vehicle);
        this->setTargetObj(vehicle);
    }

    void VehicleRolePendulum::setTargetTeam(Team const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehicleRolePendulum::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        auto updated = ai::VehicleRole::UpdateVehicle(elapsedTime, v);
        if (updated)
        {
            auto const pendulumPosition = getPendulumPosition(v, elapsedTime);
            v->SetExternalDestination(pendulumPosition);
            _LookAndFireToEnemy(v, elapsedTime);
        }
        return updated;
    }

    VehicleRolePendulum::~VehicleRolePendulum() = default;

    CVector VehicleRolePendulum::getPendulumPosition(Vehicle* vehicle, float elapsedTime)
    {
        // TODO: generated code VehicleRolePendulum::getPendulumPosition
        ai::VehicleRolePendulumPrototypeInfo const* prototype = GetPrototypeInfo();
        ai::Obj const* targetObj = getTargetObj();

        if (!targetObj)
        {
            // No target object, return vehicle's current position
            return vehicle->GetPosition();;
        }

        // Get target object position
        CVector targetPos = ai::getPhysicObjOrPhysicBodyPosition(targetObj);

        // Calculate pendulum offset using elliptical motion
        float sinAngle = std::sin(this->m_angle);
        float cosAngle = std::cos(this->m_angle);

        float offsetX = sinAngle * prototype->m_A * this->m_Direction.x - cosAngle * prototype->m_B * this->m_Direction.y;

        float offsetZ = cosAngle * prototype->m_B * this->m_Direction.x + sinAngle * prototype->m_A * this->m_Direction.y;

        // Update pendulum angle based on vehicle speed and time
        float speedFactor = std::sqrt(prototype->m_B * prototype->m_B + prototype->m_A * prototype->m_A);
        float timeScale = elapsedTime / speedFactor;
        float angleDelta = vehicle->GetMaxSpeed() * timeScale * 0.3f;

        this->m_angle += angleDelta;

        // Reset angle if it exceeds 6pi (3 full rotations) and randomize direction
        if (this->m_angle > 6.283185307f * 3.0f)
        {  // 18.849556f = 6pi
            this->m_angle = 0.0f;

            // Generate random direction vector
            this->m_Direction.x = static_cast<float>(std::rand()) / RAND_MAX * 32767.0f - 16383.5f;
            this->m_Direction.y = static_cast<float>(std::rand()) / RAND_MAX * 32767.0f - 16383.5f;

            // Normalize the direction
            m_Direction = m_Direction.normalize();
        }

        // Calculate final position with pendulum offset
        CVector pendulumPos;
        pendulumPos.x = targetPos.x + offsetX;
        pendulumPos.z = targetPos.z + offsetZ;  // Using Z for the second coordinate
        pendulumPos.y = targetPos.y;            // Keep original Y (height)

        // Clamp position to stay within landscape boundaries
        PointBase<float> clampedPos = ai::clampIntoLandscape(PointBase<float>(pendulumPos.x, pendulumPos.z));

        CVector result;
        result.x = clampedPos.x;
        result.y = pendulumPos.y;  // Maintain original height
        result.z = clampedPos.y;   // clampedPos.y contains the clamped Z coordinate

        return result;
    }

    m3d::Object* VehicleRolePendulum::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* VehicleRolePendulum::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

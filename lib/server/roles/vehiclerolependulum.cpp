#include "vehiclerolependulum.h"

#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include "core/ini.h"
#include "core/kernel.h"
#include "math/vector2.h"
#include "server/utils.h"
#include "server/objects/team.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRolePendulum)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRolePendulum);

    VehicleRolePendulumPrototypeInfo::VehicleRolePendulumPrototypeInfo()
    {
        // RVA 0x7FD040
    }

    bool VehicleRolePendulumPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FD090
        bool const result = VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeVector2Attrib(m_oppressionShift, xmlNode, "OppressionShift");
            m3d::SafeFloatAttrib(m_A, xmlNode, "A");
            m3d::SafeFloatAttrib(m_B, xmlNode, "B");
        }
        return result;
    }

    Obj* VehicleRolePendulumPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FD6B0
        return new VehicleRolePendulum(*this);
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstVehicle(Vehicle const*, Vehicle const*) const
    {
        // RVA 0x7FCFB0
        return 2.0f;
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstTeam(Vehicle const*, Team const*, Vehicle**) const
    {
        // RVA 0x7FCFC0 - NOTE: *targetVehicle is left untouched.
        return 2.0f;
    }

    float VehicleRolePendulumPrototypeInfo::FitAgainstObj(Vehicle const*, Obj const*) const
    {
        // RVA 0x7FCFD0
        return 2.0f;
    }

    VehicleRolePendulum::VehicleRolePendulum(VehicleRolePendulumPrototypeInfo const& prototype) :
        VehicleRole(prototype),
        m_Direction(1.0f, 0.0f),
        m_angle(0.0f)
    {
        // RVA 0x7FD120
    }

    // RVA 0x7FCFE0
    VehicleRolePendulum::~VehicleRolePendulum() = default;

    void VehicleRolePendulum::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FD710 - NOTE: a missing or empty Direction reads as (0, 0), not the current
        // direction.
        VehicleRole::LoadRuntimeValues(xmlFile, xmlNode);
        CStr strDirection = CStr::format_("%.3f %.3f", m_Direction.x, m_Direction.y);
        m3d::SafeStrAttrib(strDirection, xmlNode, "Direction");
        float x = 0.0f;
        float y = 0.0f;
        if (strDirection.c_str() && strlen(strDirection.c_str()))
        {
            sscanf(strDirection.c_str(), "%f %f", &x, &y);
        }
        m_Direction.x = x;
        m_Direction.y = y;
        m3d::SafeFloatAttrib(m_angle, xmlNode, "Angle");
    }

    void VehicleRolePendulum::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7FD830
        VehicleRole::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Direction", CStr::format_("%.3f %.3f", m_Direction.x, m_Direction.y).c_str());
        xmlNode->SetAttribute("Angle", CStr(m_angle).c_str());
    }

    void VehicleRolePendulum::setTargetVehicle(Vehicle const* vehicle)
    {
        // RVA 0x7FCFF0
        VehicleRole::setTargetVehicle(vehicle);
        setTargetObj(vehicle);
    }

    void VehicleRolePendulum::setTargetTeam(Team const* team)
    {
        // RVA 0x7FD010
        VehicleRole::setTargetTeam(team);
        setTargetObj(team);
    }

    void VehicleRolePendulum::setTargetObj(Obj const* obj)
    {
        // RVA 0x7FD030
        VehicleRole::setTargetObj(obj);
    }

    bool VehicleRolePendulum::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        // RVA 0x7FD8E0
        if (!VehicleRole::UpdateVehicle(elapsedTime, v))
        {
            return false;
        }
        v->SetExternalDestination(getPendulumPosition(v, elapsedTime));
        _LookAndFireToEnemy(v, elapsedTime);
        return true;
    }

    CVector VehicleRolePendulum::getPendulumPosition(Vehicle* v, float elapsedTime)
    {
        // RVA 0x7FD190 - swings along an ellipse (semi-axes A and B) round the target, the
        // ellipse turned to m_Direction; after three turns it picks a new random direction.
        // NOTE: m_oppressionShift is loaded but unused.
        VehicleRolePendulumPrototypeInfo const* prototype = GetPrototypeInfo();
        Obj const* target = getTargetObj();
        if (!target)
        {
            return v->GetPosition();
        }
        CVector const vPos = getPhysicObjOrPhysicBodyPosition(target);
        double const sinA = sin(m_angle) * prototype->m_A;
        double const cosB = cos(m_angle) * prototype->m_B;
        float const dx = static_cast<float>(sinA * m_Direction.x - cosB * m_Direction.y);
        float const dz = static_cast<float>(cosB * m_Direction.x + sinA * m_Direction.y);
        double const scaledTime = elapsedTime / sqrt(double(prototype->m_B) * prototype->m_B + double(prototype->m_A) * prototype->m_A);
        m_angle = static_cast<float>(v->GetMaxSpeed() * scaledTime * 0.30000001f + m_angle);
        if (m_angle > 18.849556f)
        {
            m_angle = 0.0f;
            // The first number drawn goes to y.
            float const y = static_cast<float>(rand()) - 16383.5f;
            float const x = static_cast<float>(rand()) - 16383.5f;
            m_Direction = CVector2(x, y).normalize();
        }
        PointBase<float> const clamped = clampIntoLandscape(PointBase<float>(vPos.x + dx, vPos.z + dz));
        return CVector(clamped.x, vPos.y, clamped.y);
    }

    m3d::Class* VehicleRolePendulum::GetClass() const
    {
        // RVA 0x7FCFA0
        return RT_CLASS_LOCAL(VehicleRolePendulum);
    }

    m3d::Class* VehicleRolePendulum::GetBaseClass()
    {
        // RVA 0x7FCF90
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRolePendulumPrototypeInfo const* VehicleRolePendulum::GetPrototypeInfo() const
    {
        // RVA 0x7FD940 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRolePendulumPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Object* VehicleRolePendulum::CreateObject()
    {
        // RVA 0x7FD4F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRolePendulum::Clone()
    {
        // RVA 0x7FD330
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

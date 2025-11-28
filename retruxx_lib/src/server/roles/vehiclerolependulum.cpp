#include "vehiclerolependulum.h"

#include <stdexcept>

#include "core/ini.h"
#include "server/objects/vehicle.h"

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
        RETRUXX_NOT_IMPLEMENTED;
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
            auto PendulumPosition = getPendulumPosition(v, elapsedTime);
            v->SetExternalDestination(PendulumPosition);
            this->_LookAndFireToEnemy(v, elapsedTime);
            return 1;
        }
        return updated;
    }

    VehicleRolePendulum::~VehicleRolePendulum()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleRolePendulum::getPendulumPosition(Vehicle*, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

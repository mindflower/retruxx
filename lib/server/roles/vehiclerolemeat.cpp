#include "vehiclerolemeat.h"

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
#include "chasemotiontactics.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleMeat)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleMeat);

    VehicleRoleMeatPrototypeInfo::VehicleRoleMeatPrototypeInfo()
    {
        // RVA 0x800230
    }

    bool VehicleRoleMeatPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x800100
        return VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Obj* VehicleRoleMeatPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x800280
        return new VehicleRoleMeat(*this);
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstVehicle(Vehicle const* v, Vehicle const*) const
    {
        // RVA 0x800120
        if (!v)
        {
            return 0.0f;
        }
        float const maxSpeed = v->GetMaxSpeed();
        return v->EstimateDamageAI() + maxSpeed;
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstTeam(Vehicle const* v, Team const* target, Vehicle** targetVehicle) const
    {
        // RVA 0x800150
        *targetVehicle = getBestOpponentFromTeam(v, target);
        return FitAgainstVehicle(v, *targetVehicle);
    }

    float VehicleRoleMeatPrototypeInfo::FitAgainstObj(Vehicle const* v, Obj const*) const
    {
        // RVA 0x800180 - the object plays no part in the fit.
        return FitAgainstVehicle(v, nullptr);
    }

    VehicleRoleMeat::VehicleRoleMeat(VehicleRoleMeatPrototypeInfo const& prototype) :
        VehicleRole(prototype),
        m_chaseTactics(nullptr),
        m_needCreateChaseTactics(false)
    {
        // RVA 0x800190
    }

    VehicleRoleMeat::~VehicleRoleMeat()
    {
        // RVA 0x8001C0
        delete m_chaseTactics;
        m_chaseTactics = nullptr;
    }

    void VehicleRoleMeat::setTargetVehicle(Vehicle const* vehicle)
    {
        // RVA 0x8001F0
        VehicleRole::setTargetVehicle(vehicle);
        setTargetObj(vehicle);
    }

    void VehicleRoleMeat::setTargetTeam(Team const* team)
    {
        // RVA 0x800210
        VehicleRole::setTargetTeam(team);
        setTargetObj(team);
    }

    void VehicleRoleMeat::setTargetObj(Obj const* obj)
    {
        // RVA 0x800320
        // A new target means a new chase, created on the next update.
        VehicleRole::setTargetObj(obj);
        delete m_chaseTactics;
        m_chaseTactics = nullptr;
        if (obj)
        {
            m_chaseTargetId = obj->GetId();
            m_needCreateChaseTactics = true;
        }
    }

    bool VehicleRoleMeat::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        // RVA 0x8008D0
        if (!VehicleRole::UpdateVehicle(elapsedTime, v))
        {
            return false;
        }
        _CreateChaseTacticsIfNeeded(v);
        v->SetExternalDestination(_EvaluateChasePointToMove(elapsedTime));
        _LookAndFireToEnemy(v, elapsedTime);
        return true;
    }

    void VehicleRoleMeat::_CreateChaseTacticsIfNeeded(Vehicle* v)
    {
        // RVA 0x8008A0
        if (m_needCreateChaseTactics)
        {
            m_chaseTactics = CreateChaseMotionTactic(v);
            m_needCreateChaseTactics = false;
        }
    }

    CVector VehicleRoleMeat::_EvaluateChasePointToMove(float elapsedTime)
    {
        // RVA 0x800370 - NOTE: with no chase the vehicle is sent to the origin.
        if (!m_chaseTactics)
        {
            return ZeroVector;
        }
        m_chaseTactics->Update(elapsedTime);
        return m_chaseTactics->EvaluateCurrentChasePoint(elapsedTime);
    }

    ChaseMotionTactics* VehicleRoleMeat::CreateChaseMotionTactic(Vehicle const* v) const
    {
        // RVA 0x800750 - either circles the target at 2..5 vehicle sizes, or loops a figure of eight
        // round it at 5..10.
        CVector const size = v->GetSize();
        double const sizeLen = sqrt(double(size.x) * size.x + double(size.y) * size.y + double(size.z) * size.z);
        if (rand() % 2 == 1)
        {
            float const radius = static_cast<float>((double(rand()) * 0.000091555528 + 2.0) * sizeLen);
            return new ChaseMotionTacticsCircle(m_chaseTargetId, v->GetId(), radius);
        }
        float const radius = static_cast<float>((double(rand()) * 0.00015259255 + 5.0) * sizeLen);
        return new ChaseMotionTacticsFigureOfEight(m_chaseTargetId, v->GetId(), radius);
    }

    m3d::Class* VehicleRoleMeat::GetClass() const
    {
        // RVA 0x8000F0
        return RT_CLASS_LOCAL(VehicleRoleMeat);
    }

    m3d::Class* VehicleRoleMeat::GetBaseClass()
    {
        // RVA 0x8000E0
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRoleMeatPrototypeInfo const* VehicleRoleMeat::GetPrototypeInfo() const
    {
        // RVA 0x800990 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRoleMeatPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Object* VehicleRoleMeat::CreateObject()
    {
        // RVA 0x800590
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRoleMeat::Clone()
    {
        // RVA 0x8003D0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

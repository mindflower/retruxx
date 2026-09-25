#include "vehiclerolecheater.h"

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
    RT_CLASS_EXPORTS_BEGIN(VehicleRoleCheater)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRoleCheater);

    VehicleRoleCheaterPrototypeInfo::VehicleRoleCheaterPrototypeInfo()
    {
        // RVA 0x7FC1F0
    }

    bool VehicleRoleCheaterPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7FC0C0
        return VehicleRolePrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    Obj* VehicleRoleCheaterPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7FC240
        return new VehicleRoleCheater(*this);
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstVehicle(Vehicle const* v, Vehicle const*) const
    {
        // RVA 0x7FC110
        if (!v)
        {
            return 0.0f;
        }
        float const maxSpeed = v->GetMaxSpeed();
        return v->EstimateDamageAI() + maxSpeed;
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstTeam(Vehicle const* v, Team const* target, Vehicle** targetVehicle) const
    {
        // RVA 0x7FC0E0
        *targetVehicle = getBestOpponentFromTeam(v, target);
        return FitAgainstVehicle(v, *targetVehicle);
    }

    float VehicleRoleCheaterPrototypeInfo::FitAgainstObj(Vehicle const* v, Obj const*) const
    {
        // RVA 0x7FC140 - the object plays no part in the fit.
        return FitAgainstVehicle(v, nullptr);
    }

    VehicleRoleCheater::VehicleRoleCheater(VehicleRoleCheaterPrototypeInfo const& prototype) :
        VehicleRole(prototype),
        m_chaseTactics(nullptr),
        m_needCreateChaseTactics(false)
    {
        // RVA 0x7FC150
    }

    VehicleRoleCheater::~VehicleRoleCheater()
    {
        // RVA 0x7FC180
        delete m_chaseTactics;
        m_chaseTactics = nullptr;
    }

    void VehicleRoleCheater::setTargetVehicle(Vehicle const* vehicle)
    {
        // RVA 0x7FC1B0
        VehicleRole::setTargetVehicle(vehicle);
        setTargetObj(vehicle);
    }

    void VehicleRoleCheater::setTargetTeam(Team const* team)
    {
        // RVA 0x7FC1D0
        VehicleRole::setTargetTeam(team);
        setTargetObj(team);
    }

    void VehicleRoleCheater::setTargetObj(Obj const* obj)
    {
        // RVA 0x7FC2E0
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

    bool VehicleRoleCheater::UpdateVehicle(float elapsedTime, Vehicle* v)
    {
        // RVA 0x7FC7B0
        if (!VehicleRole::UpdateVehicle(elapsedTime, v))
        {
            return false;
        }
        _CreateChaseTacticsIfNeeded(v);
        v->SetExternalDestination(_EvaluateChasePointToMove(elapsedTime));
        _LookAndFireToEnemy(v, elapsedTime);
        return true;
    }

    void VehicleRoleCheater::_CreateChaseTacticsIfNeeded(Vehicle* v)
    {
        // RVA 0x7FC370
        if (m_needCreateChaseTactics)
        {
            m_chaseTactics = CreateChaseMotionTactic(v);
            m_needCreateChaseTactics = false;
        }
    }

    CVector VehicleRoleCheater::_EvaluateChasePointToMove(float elapsedTime)
    {
        // RVA 0x7FC3D0 - NOTE: with no chase the vehicle is sent to the origin.
        if (!m_chaseTactics)
        {
            return ZeroVector;
        }
        m_chaseTactics->Update(elapsedTime);
        return m_chaseTactics->EvaluateCurrentChasePoint(elapsedTime);
    }

    ChaseMotionTactics* VehicleRoleCheater::CreateChaseMotionTactic(Vehicle const* v) const
    {
        // RVA 0x7FC330
        return new ChaseMotionTacticsCheater(m_chaseTargetId, v->GetId());
    }

    m3d::Class* VehicleRoleCheater::GetClass() const
    {
        // RVA 0x7FC0A0
        return RT_CLASS_LOCAL(VehicleRoleCheater);
    }

    m3d::Class* VehicleRoleCheater::GetBaseClass()
    {
        // RVA 0x7FC090
        return RT_CLASS_LOCAL(VehicleRole);
    }

    VehicleRoleCheaterPrototypeInfo const* VehicleRoleCheater::GetPrototypeInfo() const
    {
        // RVA 0x7FC850 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRoleCheaterPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Object* VehicleRoleCheater::CreateObject()
    {
        // RVA 0x7FC5F0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRoleCheater::Clone()
    {
        // RVA 0x7FC430
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

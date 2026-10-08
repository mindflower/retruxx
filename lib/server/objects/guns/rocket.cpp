#include "rocket.h"

#include <stdexcept>

#include "core/kernel.h"
#include "core/log.h"
#include "math/matrix.h"
#include "math/quaternion.h"
#include "server/objects/blastwave.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/physicbodies/simplephysicbody.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Rocket)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Rocket);

    Obj* RocketPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x84CFE0
        return new Rocket(*this);
    }

    RocketPrototypeInfo::RocketPrototypeInfo()
    {
        m_velocity = 1.0f;
        m_acceleration = 1.0f;
        m_minTurningRadius = 1.0f;
        m_flyTime = 1.0f;
        m_BlastWavePrototypeId = -1;
    }

    void RocketPrototypeInfo::PostLoad()
    {
        // RVA 0x84C910 - NOTE: ShellPrototypeInfo::PostLoad is not called.
        if (m_BlastWavePrototypeName.c_str() && strlen(m_BlastWavePrototypeName.c_str()))
        {
            m_BlastWavePrototypeId = thePrototypeManager->GetPrototypeId(m_BlastWavePrototypeName);
            if (m_BlastWavePrototypeId == -1)
            {
                M3D_LOG_INFO(CStr("Unknown blast wave prototype name: ") + m_BlastWavePrototypeName);
            }
        }
    }

    bool RocketPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = ShellPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeFloatAttrib(m_velocity, xmlNode, "Velocity");
            m3d::SafeFloatAttrib(m_acceleration, xmlNode, "Acceleration");
            m3d::SafeFloatAttrib(m_minTurningRadius, xmlNode, "MinTurningRadius");
            m3d::SafeFloatAttrib(m_flyTime, xmlNode, "FlyTime");
            m3d::SafeStrAttrib(m_BlastWavePrototypeName, xmlNode, "BlastWavePrototype");
        }
        return result;
    }

    RocketPrototypeInfo const* Rocket::GetPrototypeInfo() const
    {
        // RVA 0x84CDD0 - NOTE: the prototype is cast without a type check.
        return static_cast<RocketPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Rocket::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x84CA00 - NOTE: like PlasmaBunch, Velocity and LifeTime are read into one local that
        // the shipped code leaves uninitialized, so a missing Velocity stores garbage and a missing
        // LifeTime repeats the velocity. Here the local starts at 0.
        Shell::LoadRuntimeValues(xmlFile, xmlNode);
        float value = 0.0f;
        m3d::SafeFloatAttrib(value, xmlNode, "Velocity");
        m_velocity.value().set(value);
        m3d::SafeFloatAttrib(value, xmlNode, "LifeTime");
        m_lifeTime.value().set(value);
        m3d::SafeVectorAttrib(m_initialDirection, xmlNode, "InitialDirection");
        m3d::SafeVectorAttrib(m_initVelDir, xmlNode, "InitVelDir");
        m3d::SafeFloatAttrib(m_initVelValue, xmlNode, "InitVelValue");
        m3d::SafeIntAttrib(m_targetObjId, xmlNode, "TargetObjId");
        m3d::SafeBoolAttrib(m_withAngleLimit, xmlNode, "WithAngleLimit");
        m3d::SafeIntAttrib(m_numCircles, xmlNode, "NumCircles");
    }

    float Rocket::GetVelocity() const
    {
        // RVA 0x602730
        return m_velocity.value().get();
    }

    void Rocket::SetInitialVelocity(CVector const& initVel)
    {
        // RVA 0x7C35C0 - splits the launcher's velocity into a speed and a direction; the rocket
        // keeps drifting with it (see Update).
        m_initVelValue = static_cast<float>(
            std::sqrt(double(initVel.z) * initVel.z + double(initVel.y) * initVel.y + double(initVel.x) * initVel.x));
        if (m_initVelValue <= 0.001f)
        {
            m_initVelDir = CVector(0.0f, 0.0f, 0.0f);
        }
        else
        {
            float const invLen = static_cast<float>(1.0 / std::sqrt(double(initVel.y) * initVel.y +
                double(initVel.z) * initVel.z + double(initVel.x) * initVel.x + double(1.1920929e-7f)));
            m_initVelDir = CVector(initVel.x * invLen, invLen * initVel.y, invLen * initVel.z);
        }
    }

    Rocket::Rocket(RocketPrototypeInfo const& prototype) :
        Shell(prototype),
        m_targetObjId(-1),
        m_velocity(0.0f, 0.0f, prototype.m_velocity, prototype.m_acceleration),
        m_lifeTime(0.0f, 0.0f, prototype.m_flyTime, 1.0f),
        m_minTurningRadius(prototype.m_minTurningRadius),
        m_withAngleLimit(true),
        m_numCircles(0)
    {
        // RVA 0x84CEE0 - moved by hand in Update until its fuel runs out, so the body starts off.
        PhysicObj::DisablePhysics();
        m_physicState |= 1u;
        SetCorrectEnabledCellsCounter();
        SimplePhysicObj::EnableGeometry(false);
        m_physicBody->SetNodeAction(1, true);
    }

    Rocket::Rocket(Rocket const& rocket) :
        Shell(rocket),
        m_targetObjId(rocket.m_targetObjId),
        m_velocity(rocket.m_velocity.value().get(), rocket.m_velocity.minValue().get(),
            rocket.m_velocity.maxValue().get(), rocket.m_velocity.regeneration().get()),
        m_lifeTime(rocket.m_lifeTime.value().get(), rocket.m_lifeTime.minValue().get(),
            rocket.m_lifeTime.maxValue().get(), rocket.m_lifeTime.regeneration().get()),
        m_minTurningRadius(rocket.m_minTurningRadius),
        m_initialDirection(rocket.m_initialDirection),
        m_initVelDir(rocket.m_initVelDir),
        m_initVelValue(rocket.m_initVelValue),
        m_withAngleLimit(rocket.m_withAngleLimit),
        m_numCircles(rocket.m_numCircles)
    {
        // NOTE: declared in the PDB but never emitted in the shipped build (only Mine's equally
        // unused copy constructor refers to it). The components are rebuilt from the source's
        // values rather than copied, since their callbacks are bound to their owner.
    }

    void Rocket::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x84D010
        SimplePhysicObj::Update(elapsedTime, workTime);

        if (m_lifeTime.value().get() == m_lifeTime.maxValue().get())
        {
            // Out of fuel: ODE flies it now; just keep it pointing along its velocity.
            CVector const vel = GetLinearVelocity();
            float const invLen = static_cast<float>(
                1.0 / std::sqrt(double(vel.x) * vel.x + double(vel.y) * vel.y + double(vel.z) * vel.z + double(1.1920929e-7f)));
            SetDirection(CVector(vel.x * invLen, vel.y * invLen, vel.z * invLen));
            return;
        }

        m_velocity.regenerate(elapsedTime);
        m_lifeTime.regenerate(elapsedTime);

        CVector const curDir = GetDirection();
        double const dirLen = std::sqrt(
            double(curDir.x) * curDir.x + double(curDir.y) * curDir.y + double(curDir.z) * curDir.z + double(1.1920929e-7f));
        CVector const dir(
            static_cast<float>(1.0 / dirLen * curDir.x),
            static_cast<float>(1.0 / dirLen * curDir.y),
            static_cast<float>(1.0 / dirLen * curDir.z));

        if (m_lifeTime.value().get() == m_lifeTime.maxValue().get())
        {
            // The fuel ran out this frame: hand the rocket over to ODE at its current speed.
            EnablePhysics();
            float const speed = m_velocity.value().get();
            SetLinearVelocity(CVector(speed * dir.x, speed * dir.y, speed * dir.z));
            m_physicBody->SetNodeAction(8, true);
            return;
        }

        // The launcher's velocity fades out over the first second of flight.
        if (m_lifeTime.value().get() > 1.0f)
        {
            m_initVelValue = m_initVelValue - elapsedTime * 20.0f;
            if (m_initVelValue < 0.0f)
            {
                m_initVelValue = 0.0f;
            }
        }
        CVector const initVel(m_initVelDir.x * m_initVelValue, m_initVelDir.y * m_initVelValue, m_initVelDir.z * m_initVelValue);

        float timeForTurning = 0.0f;
        Obj* target = theObjects->GetEntityByObjId(m_targetObjId);
        if (target)
        {
            if (target->IsKindOf(RT_CLASS_LOCAL(PhysicObj)))
            {
                PhysicObj* targetObj = static_cast<PhysicObj*>(target);
                CVector const pos = GetPosition();
                CVector const targetPos = targetObj->GetGeometricCenter();
                CVector toTarget(targetPos.x - pos.x, targetPos.y - pos.y, targetPos.z - pos.z);
                float const distSq = toTarget.x * toTarget.x + (targetPos.z - pos.z) * (targetPos.z - pos.z) +
                    toTarget.y * toTarget.y;

                // A target with an anti-missile gadget can't be homed on from inside its radius.
                int const antiMissileId = targetObj->GetPropertyId("GadgetAntiMissileRadius");
                bool canHome = true;
                if (antiMissileId != -1)
                {
                    float const antiMissileRadius = targetObj->GetPropertyById(antiMissileId).GetAsFloat();
                    canHome = antiMissileRadius * antiMissileRadius <= distSq;
                }

                if (!canHome)
                {
                    m_targetObjId = -1;
                }
                else if (distSq > 0.0099999998f)
                {
                    toTarget.normalizeInplace();

                    // The turn happens in the plane of the current direction and the target.
                    CVector axis(toTarget.z * dir.y - toTarget.y * dir.z, toTarget.x * dir.z - toTarget.z * dir.x,
                        toTarget.y * dir.x - toTarget.x * dir.y);
                    bool changedNormal = false;
                    if (axis.lengthSq() < 0.00030000001f)
                    {
                        // Flying straight at (or away from) the target: pick a turn axis from a
                        // skewed direction instead.
                        float const skewedX = dir.x + 1000.0f;
                        CVector const skewed(axis.z * dir.y - axis.y * dir.z, dir.z * skewedX - axis.z * dir.x,
                            axis.y * dir.x - dir.y * skewedX);
                        axis = skewed.getNormalized();
                        changedNormal = true;
                    }

                    float cosAngle = toTarget.x * dir.x + toTarget.z * dir.z + toTarget.y * dir.y;
                    if (cosAngle < -0.99999899f)
                    {
                        cosAngle = -0.99999899f;
                    }
                    else if (cosAngle > 0.99999899f)
                    {
                        cosAngle = 0.99999899f;
                    }
                    float angle = static_cast<float>(std::acos(cosAngle));

                    if (angle > 0.02f && m_velocity.value().get() > 0.000001f)
                    {
                        if (changedNormal)
                        {
                            M3D_LOG_INFO(CStr("Changed normal!"));
                        }

                        // Turn along a circle of the minimum radius, as far as this frame allows.
                        float const maxTurnAngle = m_velocity.value().get() / m_minTurningRadius * elapsedTime;
                        angle = angle <= maxTurnAngle ? angle : maxTurnAngle;
                        timeForTurning = m_minTurningRadius / m_velocity.value().get() * angle;

                        CVector const toCircleCenterDir(axis.y * dir.z - axis.z * dir.y, axis.z * dir.x - dir.z * axis.x,
                            dir.y * axis.x - axis.y * dir.x);
                        float const radius = m_minTurningRadius;
                        CVector const centerDir = toCircleCenterDir.getNormalized();
                        CVector const toCircleCenter(centerDir.x * radius, centerDir.y * radius, centerDir.z * radius);

                        Quaternion turn;
                        turn.FromAxisAngle(axis.getNormalized(), angle);
                        Quaternion fullRot = turn * GetRotation();
                        float const normSq = fullRot.w * fullRot.w + fullRot.z * fullRot.z + fullRot.y * fullRot.y +
                            fullRot.x * fullRot.x;
                        if (normSq <= 0.0f)
                        {
                            fullRot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
                        }
                        else
                        {
                            double const invNorm = 1.0 / std::sqrt(normSq);
                            fullRot = Quaternion(static_cast<float>(fullRot.x * invNorm), static_cast<float>(fullRot.y * invNorm),
                                static_cast<float>(fullRot.z * invNorm), static_cast<float>(fullRot.w * invNorm));
                        }
                        SetRotation(fullRot);

                        // Rotate the position about the circle's centre by the same turn.
                        // The rotation matrix of the turn, as the shipped code expands it inline.
                        float const m11 = 1.0f - (turn.z * turn.z + turn.y * turn.y) * 2.0f;
                        float const m21 = (turn.y * turn.x - turn.w * turn.z) * 2.0f;
                        float const m31 = (turn.w * turn.y + turn.z * turn.x) * 2.0f;
                        float const m12 = (turn.w * turn.z + turn.y * turn.x) * 2.0f;
                        float const m22 = 1.0f - (turn.z * turn.z + turn.x * turn.x) * 2.0f;
                        float const m32 = (turn.z * turn.y - turn.w * turn.x) * 2.0f;
                        float const m13 = (turn.z * turn.x - turn.w * turn.y) * 2.0f;
                        float const m23 = (turn.w * turn.x + turn.z * turn.y) * 2.0f;
                        float const m33 = 1.0f - (turn.y * turn.y + turn.x * turn.x) * 2.0f;
                        CVector const rotated(
                            m31 * toCircleCenter.z + m21 * toCircleCenter.y + m11 * toCircleCenter.x,
                            m32 * toCircleCenter.z + m22 * toCircleCenter.y + m12 * toCircleCenter.x,
                            m33 * toCircleCenter.z + m23 * toCircleCenter.y + m13 * toCircleCenter.x);
                        SetPosition(CVector(toCircleCenter.x + pos.x - rotated.x, toCircleCenter.y + pos.y - rotated.y,
                            toCircleCenter.z + pos.z - rotated.z));
                    }
                }
            }
        }

        // Fly straight for the rest of the frame, and drift with the launcher's velocity.
        CVector const drift(initVel.x * elapsedTime, initVel.y * elapsedTime, initVel.z * elapsedTime);
        float const speed = m_velocity.value().get();
        float const timeForStraightFlight = elapsedTime - timeForTurning;
        CVector const flyDir = GetDirection();
        CVector const straight(flyDir.x * timeForStraightFlight * speed, flyDir.y * timeForStraightFlight * speed,
            flyDir.z * timeForStraightFlight * speed);
        CVector const curPos = GetPosition();
        CVector const newPos(curPos.x + straight.x + drift.x, curPos.y + straight.y + drift.y, curPos.z + straight.z + drift.z);

        // Stretch the collision box back over the distance covered beyond its own length, so a fast
        // rocket cannot pass through thin geometry between two frames.
        CVector const size = GetPrototypeInfo()->GetSize();
        CVector const oldPos = GetPosition();
        CVector const delta(newPos.x - oldPos.x, newPos.y - oldPos.y, newPos.z - oldPos.z);
        float const overshoot = static_cast<float>(
            std::sqrt(double(delta.z) * delta.z + double(delta.y) * delta.y + double(delta.x) * delta.x) - size.z);
        float const stretch = overshoot <= 0.0f ? 0.0f : overshoot;
        CollisionInfo& collisionInfo = m_collisionInfos.front();
        collisionInfo.m_relTranslation = CVector(0.0f, 0.0f, stretch * -0.5f);
        collisionInfo.m_size = CVector(size.x, size.y, size.z + stretch);
        _UpdatePhysicBodyByCollisionInfo(m_collisionInfos);
        SetPosition(newPos);

        // Give up the target after turning more than about 143 degrees from the launch direction
        // (when the angle is limited) or after circling it twice.
        CVector const newDir = GetDirection();
        float const invLen = static_cast<float>(1.0 / std::sqrt(
            double(newDir.x) * newDir.x + double(newDir.y) * newDir.y + double(newDir.z) * newDir.z + double(1.1920929e-7f)));
        CVector const newDirN(invLen * newDir.x, newDir.y * invLen, newDir.z * invLen);
        float const newDot = m_initialDirection.z * newDirN.z + m_initialDirection.y * newDirN.y + m_initialDirection.x * newDirN.x;
        float const oldDot = m_initialDirection.z * dir.z + m_initialDirection.y * dir.y + dir.x * m_initialDirection.x;
        if (newDot < -0.80000001f && oldDot >= -0.80000001f)
        {
            ++m_numCircles;
        }
        if ((m_withAngleLimit && newDot < -0.80000001f) || m_numCircles >= 2)
        {
            m_targetObjId = -1;
        }
    }

    m3d::Class* Rocket::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Shell);
    }

    void Rocket::CreateBlastWave() const
    {
        // RVA 0x84CE30
        // NOTE: the blast wave is not checked for null once created.
        int const blastWaveId = theObjects->CreateNewObject(GetPrototypeInfo()->m_BlastWavePrototypeId, "", -1, -1);
        if (blastWaveId != -1)
        {
            BlastWave* blastWave = static_cast<BlastWave*>(theObjects->GetEntityByObjId(blastWaveId));
            blastWave->SetRocketId(GetId());
            blastWave->SetPosition(GetPosition());
            blastWave->SetEmitterId(GetGunObjId());
            blastWave->SetBelong(GetBelong());
        }
    }

    void Rocket::Remove()
    {
        // RVA 0x84C3F0
        SimplePhysicObj::Remove();
    }

    bool Rocket::getWithAngleLimit() const
    {
        // NOTE: declared in the PDB but never instantiated in the shipped binary.
        return m_withAngleLimit;
    }

    void Rocket::setWithAngleLimit(bool value)
    {
        // RVA 0x7C34A0
        m_withAngleLimit = value;
    }

    void Rocket::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x84CBE0
        Shell::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("Velocity", CStr(m_velocity.value().get()).c_str());
        xmlNode->SetAttribute("LifeTime", CStr(m_lifeTime.value().get()).c_str());
        xmlNode->SetAttribute("InitialDirection", CStr(m_initialDirection).c_str());
        xmlNode->SetAttribute("InitVelDir", CStr(m_initVelDir).c_str());
        xmlNode->SetAttribute("InitVelValue", CStr(m_initVelValue).c_str());
        xmlNode->SetAttribute("TargetObjId", CStr(m_targetObjId).c_str());
        xmlNode->SetAttribute("WithAngleLimit", CStr(static_cast<int>(m_withAngleLimit)).c_str());
        xmlNode->SetAttribute("NumCircles", CStr(m_numCircles).c_str());
    }

    void Rocket::SetInitialDirection(CVector const& direction)
    {
        // RVA 0x7C3480
        m_initialDirection = direction;
    }

    m3d::Class* Rocket::GetClass() const
    {
        // RVA 0x84C3E0
        return RT_CLASS_LOCAL(Rocket);
    }

    // RVA 0x84CE00
    Rocket::~Rocket() = default;

    m3d::Object* Rocket::Clone()
    {
        // RVA 0x84C550
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Rocket::CreateObject()
    {
        // RVA 0x84C710
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai

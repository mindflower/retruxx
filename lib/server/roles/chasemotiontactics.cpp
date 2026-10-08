#include "chasemotiontactics.h"

#include <cmath>
#include <cstdlib>

#include "math/matrix.h"
#include "math/vector2.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"

namespace ai
{
    namespace
    {
        // Resolves an id without a type check, as the shipped code does.
        PhysicObj* PhysicObjById(int id)
        {
            return static_cast<PhysicObj*>(theObjects->GetEntityByObjId(id));
        }
    }  // namespace

    ChaseMotionData::ChaseMotionData(int targetId, int chaserId) :
        m_targetId(targetId),
        m_chaserId(chaserId),
        m_chaseMotionTime(0.0f)
    {
        // RVA 0x905E50 - records where the target and the chaser were when the chase began.
        if (PhysicObj* target = PhysicObjById(m_targetId))
        {
            m_initTargetPos = target->GetPosition();
            m_initTargetRot = target->GetRotation();
        }
        else
        {
            m_initTargetPos = CVector(0.0f, 0.0f, 0.0f);
            m_initTargetRot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }
        if (PhysicObj* chaser = PhysicObjById(m_chaserId))
        {
            m_initPos = chaser->GetPosition();
            m_initRot = chaser->GetRotation();
        }
        else
        {
            m_initPos = CVector(0.0f, 0.0f, 0.0f);
            m_initRot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }

    ChaseMotionData::ChaseMotionData(ChaseMotionData const&) = default;

    PhysicObj* ChaseMotionData::GetTarget() const
    {
        // RVA 0x905440 - NOTE: not type checked.
        return PhysicObjById(m_targetId);
    }

    Vehicle* ChaseMotionData::GetChaser() const
    {
        // RVA 0x905480 - NOTE: not type checked.
        return static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_chaserId));
    }

    ChaseMotionTactics::ChaseMotionTactics(int targetId, int chaserId) :
        m_chaseMotionData(-1, -1)
    {
        // RVA 0x906000
        m_chaseMotionData = ChaseMotionData(targetId, chaserId);
    }

    ChaseMotionTactics::ChaseMotionTactics(ChaseMotionTactics const&) = default;

    // RVA 0x9053A0
    ChaseMotionTactics::~ChaseMotionTactics() = default;

    void ChaseMotionTactics::SetTargetAndChaser(int targetId, int chaserId)
    {
        // RVA 0x905FD0
        m_chaseMotionData = ChaseMotionData(targetId, chaserId);
    }

    void ChaseMotionTactics::Update(float elapsedTime)
    {
        // RVA 0x9053E0
        m_chaseMotionData.m_chaseMotionTime = elapsedTime + m_chaseMotionData.m_chaseMotionTime;
    }

    void ChaseMotionTactics::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // RVA 0x9053B0 - nothing is saved.
    }

    void ChaseMotionTactics::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        // RVA 0x9053C0 - nothing is saved.
    }

    ChaseMotionData const& ChaseMotionTactics::GetChaseMotionData() const
    {
        // RVA 0x9053D0
        return m_chaseMotionData;
    }

    ChaseMotionTacticsCheater::ChaseMotionTacticsCheater(int targetId, int ChaseMotionrId) :
        ChaseMotionTactics(targetId, ChaseMotionrId),
        m_CheaterState(csNormal),
        m_elapsedTime(0.0f)
    {
        // RVA 0x9061C0 - each phase lasts a random 0..60 seconds.
        m_Period = static_cast<float>(rand()) * 0.000030518509f * 60.0f;
    }

    ChaseMotionTacticsCheater::ChaseMotionTacticsCheater(ChaseMotionTacticsCheater const&) = default;

    CVector ChaseMotionTacticsCheater::EvaluateCurrentChasePoint(float elapsedTime)
    {
        // RVA 0x905BF0 - cycles through three phases, each m_Period long: drive alongside
        // (off to the target's side), then pull out ahead of itself along the target's
        // velocity, then cut in ahead of the target.
        // NOTE: the target and the chaser are not checked for null.
        m_elapsedTime = elapsedTime + m_elapsedTime;
        PhysicObj* target = PhysicObjById(GetChaseMotionData().m_targetId);
        CVector const velTarget = target->GetLinearVelocity();
        CVector const posTarget = target->GetGeometricCenter();
        CVector const posThis = PhysicObjById(GetChaseMotionData().m_chaserId)->GetPosition();
        switch (m_CheaterState)
        {
        case csNormal:
            if (m_elapsedTime > m_Period)
            {
                m_CheaterState = csOut;
                m_elapsedTime = 0.0f;
            }
            return CVector(velTarget.z * 100.0f + posThis.x, posThis.y, posThis.z - velTarget.x * 100.0f);
        case csOut:
            if (m_elapsedTime > m_Period)
            {
                m_CheaterState = csIn;
                m_elapsedTime = 0.0f;
            }
            return CVector(
                (velTarget.x * elapsedTime) * 100.0f + posThis.x,
                (velTarget.y * elapsedTime) * 100.0f + posThis.y,
                (velTarget.z * elapsedTime) * 100.0f + posThis.z);
        case csIn:
            if (m_elapsedTime > m_Period)
            {
                m_CheaterState = csNormal;
                m_elapsedTime = 0.0f;
            }
            return CVector(
                (velTarget.x * elapsedTime) * 100.0f + posTarget.x,
                (velTarget.y * elapsedTime) * 100.0f + posTarget.y,
                (velTarget.z * elapsedTime) * 100.0f + posTarget.z);
        default:
            return ZeroVector;
        }
    }

    ChaseMotionTacticsCheater::CheaterState ChaseMotionTacticsCheater::getCheaterState() const
    {
        // RVA 0x905400
        return m_CheaterState;
    }

    ChaseMotionTacticsCircle::ChaseMotionTacticsCircle(int targetId, int chaserId, float radius) :
        ChaseMotionTactics(targetId, chaserId),
        m_radius(radius)
    {
        // RVA 0x906050 - circles one way or the other at random.
        m_clockwiseSign = (2 * rand() / 0x8000) ? 1.0f : -1.0f;
    }

    ChaseMotionTacticsCircle::ChaseMotionTacticsCircle(ChaseMotionTacticsCircle const&) = default;

    CVector ChaseMotionTacticsCircle::EvaluateCurrentChasePoint(float)
    {
        // RVA 0x9054C0 - the point on a circle of m_radius round the target, 30 degrees on
        // from where the chaser is now.
        PhysicObj* target = PhysicObjById(GetChaseMotionData().m_targetId);
        PhysicObj* chaser = PhysicObjById(GetChaseMotionData().m_chaserId);
        if (!target || !chaser)
        {
            return ZeroVector;
        }
        if (!target->IsAlive() || !chaser->IsAlive())
        {
            return chaser->GetPosition();
        }
        CVector const targetPos = target->GetPosition();
        CVector const chaserPos = chaser->GetPosition();
        CVector const rel(chaserPos.x - targetPos.x, chaserPos.y - targetPos.y, chaserPos.z - targetPos.z);

        // Rotate about the up axis (0, 1, 0).
        CVector const up(0.0f, 1.0f, 0.0f);
        double const halfAngle = (m_clockwiseSign * 0.52359879f) * 0.5f;
        float const s = static_cast<float>(sin(halfAngle));
        float const w = static_cast<float>(cos(halfAngle));
        float const qx = s * up.x;
        float const qz = s * up.z;
        float const qy = s * up.y;
        float const zy = qz * qy;
        float const yx = qy * qx;
        float const m11 = 1.0f - (qz * qz + qy * qy) * 2.0f;
        float const m21 = (yx - w * qz) * 2.0f;
        float const m31 = (w * qy + qz * qx) * 2.0f;
        float const m12 = (w * qz + yx) * 2.0f;
        float const m22 = 1.0f - (qz * qz + qx * qx) * 2.0f;
        float const m32 = (zy - w * qx) * 2.0f;
        float const m13 = (qz * qx - w * qy) * 2.0f;
        float const m23 = (w * qx + zy) * 2.0f;
        float const m33 = 1.0f - (qy * qy + qx * qx) * 2.0f;
        CVector const rotated(
            (m31 * rel.z + m21 * rel.y) + m11 * rel.x,
            (m32 * rel.z + m22 * rel.y) + m12 * rel.x,
            (m33 * rel.z + m23 * rel.y) + m13 * rel.x);
        CVector const n = rotated.getNormalized();
        CVector const offset(n.x * m_radius, n.y * m_radius, n.z * m_radius);
        CVector const center = target->GetPosition();
        return CVector(center.x + offset.x, center.y + offset.y, center.z + offset.z);
    }

    ChaseMotionTacticsFigureOfEight::ChaseMotionTacticsFigureOfEight(int targetId, int chaserId, float radius) :
        ChaseMotionTactics(targetId, chaserId),
        m_radius(radius),
        m_angle(0.0f)
    {
        // RVA 0x906100
        m_clockwiseSign = (2 * rand() / 0x8000) ? 1.0f : -1.0f;
    }

    ChaseMotionTacticsFigureOfEight::ChaseMotionTacticsFigureOfEight(ChaseMotionTacticsFigureOfEight const&) = default;

    CVector ChaseMotionTacticsFigureOfEight::EvaluateCurrentChasePoint(float elapsedTime)
    {
        // RVA 0x905950 - traces a figure of eight in the target's frame: one loop for the first
        // 2 pi of m_angle, the mirrored loop for the second.
        PhysicObj* target = PhysicObjById(GetChaseMotionData().m_targetId);
        Vehicle* chaser = static_cast<Vehicle*>(PhysicObjById(GetChaseMotionData().m_chaserId));
        if (!target || !chaser)
        {
            return ZeroVector;
        }
        if (!target->IsAlive() || !chaser->IsAlive())
        {
            return chaser->GetPosition();
        }
        CVector const vPos = target->GetGeometricCenter();
        CVector vVel = target->GetLinearVelocity();
        if (sqrt(double(vVel.y) * vVel.y + double(vVel.x) * vVel.x + double(vVel.z) * vVel.z) < 5.0)
        {
            vVel = target->GetDirection();
        }
        CVector2 const heading = CVector2(vVel.x, vVel.z).normalize();

        double a = m_clockwiseSign * m_angle;
        double c;
        if (m_angle <= 6.2831855f)
        {
            c = cos(a);
        }
        else
        {
            a = -a - 3.1415927f;
            c = cos(a) + 2.0;
        }
        float const along = static_cast<float>(c * m_radius);
        float const across = static_cast<float>(sin(a) * m_radius);
        CVector result;
        result.x = (heading.x * along - heading.y * across) + vPos.x;
        result.y = vPos.y;
        result.z = (heading.y * along + heading.x * across) + vPos.z;

        if (m_angle > 12.566371f)
        {
            m_angle = 0.0f;
        }
        m_angle = static_cast<float>(chaser->GetMaxSpeed() * elapsedTime * 0.029999999f + m_angle);
        return result;
    }
}  // namespace ai

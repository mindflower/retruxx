#define NOMINMAX
#include "vehicleupdater.h"

#include <stdexcept>

#include "world.h"
#include "server/server.h"
#include "server/utils.h"

namespace ai
{
    VehicleUpdater::VehicleUpdater(ai::VehicleUpdater const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehicleUpdater::VehicleUpdater(ai::Vehicle* vehicle)
    {
        m_vehicle = vehicle;
        m_velocity = {0.0, 0.0, 0.0};

        if (auto firstExistingWheel = m_vehicle->GetFirstExistingWheel())
        {
            m_wheelRadius = firstExistingWheel->GetRadius();
        }
        else
        {
            m_wheelRadius = 1.0;
        }
    }

    VehicleUpdater::~VehicleUpdater()
    {
    }

    void VehicleUpdater::Update(float elapsedTime)
    {
        // TODO: generated code
        _UpdateForceAndVelocity(elapsedTime);

        float steer = m_vehicle->GetCurrentSteerAngle();
        CVector vehicleDir = m_vehicle->GetDirection();

        Quaternion vehicleRot = m_vehicle->GetRotation();

        CVector pos = m_vehicle->GetPosition();

        auto& wheels = m_vehicle->m_wheels;
        if (!wheels.empty() && std::fabs(steer) > 0.0099999998f && m_vehicle->m_brake < 0.5f)
        {
            // Calculate wheel base (distance between front and rear wheels)
            float wheelBase = wheels.front().m_initialPos.z - wheels.back().m_initialPos.z;

            // Normalize velocity and calculate forward multiplier
            CVector normalizedVel = m_velocity.getNormalized();

            float forwardMult = normalizedVel.x * vehicleDir.x + normalizedVel.y * vehicleDir.y + normalizedVel.z * vehicleDir.z;

            // Calculate turning parameters
            float turnRadiusFactor = 2.0f / (1.0f - std::cos(steer));
            int turnDirection = (steer >= 0.0f) ? 1 : -1;

            // Build rotation matrix from quaternion
            CMatrix rotationMatrix;
            rotationMatrix._11 = 1.0f - 2.0f * (vehicleRot.y * vehicleRot.y + vehicleRot.z * vehicleRot.z);
            rotationMatrix._12 = 2.0f * (vehicleRot.x * vehicleRot.y + vehicleRot.w * vehicleRot.z);
            rotationMatrix._13 = 2.0f * (vehicleRot.x * vehicleRot.z - vehicleRot.w * vehicleRot.y);
            rotationMatrix._14 = 0.0f;

            rotationMatrix._21 = 2.0f * (vehicleRot.x * vehicleRot.y - vehicleRot.w * vehicleRot.z);
            rotationMatrix._22 = 1.0f - 2.0f * (vehicleRot.x * vehicleRot.x + vehicleRot.z * vehicleRot.z);
            rotationMatrix._23 = 2.0f * (vehicleRot.y * vehicleRot.z + vehicleRot.w * vehicleRot.x);
            rotationMatrix._24 = 0.0f;

            rotationMatrix._31 = 2.0f * (vehicleRot.x * vehicleRot.z + vehicleRot.w * vehicleRot.y);
            rotationMatrix._32 = 2.0f * (vehicleRot.y * vehicleRot.z - vehicleRot.w * vehicleRot.x);
            rotationMatrix._33 = 1.0f - 2.0f * (vehicleRot.x * vehicleRot.x + vehicleRot.y * vehicleRot.y);
            rotationMatrix._34 = 0.0f;

            rotationMatrix._41 = 0.0f;
            rotationMatrix._42 = 0.0f;
            rotationMatrix._43 = 0.0f;
            rotationMatrix._44 = 1.0f;

            // Transform up vector by rotation matrix
            CVector INITIAL_UP_DIRECTION_35{0.0, 1.0, 0.0};
            CVector up;
            up.x = rotationMatrix._11 * INITIAL_UP_DIRECTION_35.x + rotationMatrix._21 * INITIAL_UP_DIRECTION_35.y +
                rotationMatrix._31 * INITIAL_UP_DIRECTION_35.z;
            up.y = rotationMatrix._12 * INITIAL_UP_DIRECTION_35.x + rotationMatrix._22 * INITIAL_UP_DIRECTION_35.y +
                rotationMatrix._32 * INITIAL_UP_DIRECTION_35.z;
            up.z = rotationMatrix._13 * INITIAL_UP_DIRECTION_35.x + rotationMatrix._23 * INITIAL_UP_DIRECTION_35.y +
                rotationMatrix._33 * INITIAL_UP_DIRECTION_35.z;

            // Calculate rotation angle based on turning
            float speed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);

            float turnRate = std::sqrt(turnRadiusFactor);
            float rotationAngle = speed / (turnRate * -turnDirection * wheelBase) * forwardMult * elapsedTime;

            // Normalize up vector for rotation axis
            CVector rotationAxis = up.getNormalized();

            // Create rotation quaternion
            float halfAngle = rotationAngle * 0.5f;
            float sinHalf = std::sin(halfAngle);
            float cosHalf = std::cos(halfAngle);

            Quaternion rotationQuat;
            rotationQuat.x = rotationAxis.x * sinHalf;
            rotationQuat.y = rotationAxis.y * sinHalf;
            rotationQuat.z = rotationAxis.z * sinHalf;
            rotationQuat.w = cosHalf;

            // Combine rotations (rotationQuat * vehicleRot)
            Quaternion fullRot;
            fullRot.x = rotationQuat.w * vehicleRot.x + rotationQuat.x * vehicleRot.w + rotationQuat.y * vehicleRot.z -
                rotationQuat.z * vehicleRot.y;
            fullRot.y = rotationQuat.w * vehicleRot.y - rotationQuat.x * vehicleRot.z + rotationQuat.y * vehicleRot.w +
                rotationQuat.z * vehicleRot.x;
            fullRot.z = rotationQuat.w * vehicleRot.z + rotationQuat.x * vehicleRot.y - rotationQuat.y * vehicleRot.x +
                rotationQuat.z * vehicleRot.w;
            fullRot.w = rotationQuat.w * vehicleRot.w - rotationQuat.x * vehicleRot.x - rotationQuat.y * vehicleRot.y -
                rotationQuat.z * vehicleRot.z;

            // Normalize the resulting quaternion
            float lengthSq = fullRot.x * fullRot.x + fullRot.y * fullRot.y + fullRot.z * fullRot.z + fullRot.w * fullRot.w;

            if (lengthSq > 0.0f)
            {
                float invLength = 1.0f / std::sqrt(lengthSq);
                fullRot.x *= invLength;
                fullRot.y *= invLength;
                fullRot.z *= invLength;
                fullRot.w *= invLength;
            }
            else
            {
                // Identity quaternion if invalid
                fullRot.x = 0.0f;
                fullRot.y = 0.0f;
                fullRot.z = 0.0f;
                fullRot.w = 1.0f;
            }

            // Apply the new rotation
            m_vehicle->SetRotation(fullRot);
        }
    }

    CVector VehicleUpdater::GetLinearVelocity() const
    {
        return m_velocity;
    }

    void VehicleUpdater::CalcRpmsAndGear(float& wheelAVel, float& engineRpm, int& gear) const
    {
        wheelAVel = _CalcWheelAVel();
        for (gear = 0; gear < 5; ++gear)
        {
            engineRpm = ((Vehicle::GEAR_RATIOS[gear] * m_vehicle->m_diffRatio) * 1.8) * wheelAVel * 9.5492964;
            if (m_vehicle->m_highGearShiftLimit > engineRpm)
            {
                break;
            }
        }
    }

    float VehicleUpdater::_CalcWheelAVel() const
    {
        // TODO: check and refactor
        auto* wheel = m_vehicle->GetFirstExistingWheel();
        if (!wheel)
        {
            return 0.0;
        }

        auto const dir = m_vehicle->GetDirection();
        auto v6 = (float)((float)(this->m_velocity.y * dir.y) + (float)(this->m_velocity.z * dir.z)) + (float)(dir.x * this->m_velocity.x);
        auto v9 =
            (float)((float)((float)(dir.y * v6) * dir.y) + (float)((float)(dir.z * v6) * dir.z)) + (float)((float)(dir.x * v6) * dir.x);
        auto v11 = (float)((float)((float)(dir.z * v6) * (float)(dir.z * v6)) + (float)((float)(dir.y * v6) * (float)(dir.y * v6))) +
            (float)((float)(dir.x * v6) * (float)(dir.x * v6));
        auto radius = wheel->GetRadius();
        auto v8 = v9 >= 0.0;
        auto v10 = -1;
        if (v8)
            v10 = 1;
        return sqrt(v11) * ((double)v10 / radius);
    }

    void VehicleUpdater::_UpdateForceAndVelocity(float elapsedTime)
    {
        // RVA 0x7BFC60 - the simplified (point-mass) driving model: gravity along the slope, the
        // engine's pull and the tyres' grip (both bounded by the ground's reaction times the soil's
        // friction), and the soil's rolling resistance; then one integration step along the ground.
        CVector const groundPos = GetGroundPos(m_vehicle->GetPosition(), false, false);
        m3d::CWorld* world = pServer->GetWorld();
        float const cellSize = pServer->GetLevelSize() / static_cast<float>(world->GetLandscape().GetTileSize());
        DynamicScene::SoilProps const& soil = gDynamicScene->GetSoilProps(
            static_cast<int>((1.0f / cellSize) * groundPos.x + 0.5f),
            static_cast<int>((1.0f / cellSize) * groundPos.z + 0.5f));
        float const mu = soil.m_friction;
        float const sideMu = mu * 1.5f;
        float const mass = m_vehicle->GetMass();
        CVector const gravityForce(0.0f, mass * -9.8100004f, 0.0f);

        // The vehicle's up axis.
        static CVector const INITIAL_UP_DIRECTION(0.0f, 1.0f, 0.0f);
        Quaternion const rot = m_vehicle->GetRotation();
        float const xz = rot.z * rot.x;
        float const xw = rot.x * rot.w;
        float const zw = rot.z * rot.w;
        float const xx = rot.x * rot.x;
        float const xy = rot.y * rot.x;
        float const yz = rot.z * rot.y;
        float const zz = rot.z * rot.z;
        float const yy = rot.y * rot.y;
        float const yw = rot.y * rot.w;
        CMatrix m;
        m._11 = 1.0f - (zz + yy) * 2.0f;
        m._12 = (zw + xy) * 2.0f;
        m._13 = (xz - yw) * 2.0f;
        m._14 = 0.0f;
        m._21 = (xy - zw) * 2.0f;
        m._22 = 1.0f - (zz + xx) * 2.0f;
        m._23 = (xw + yz) * 2.0f;
        m._24 = 0.0f;
        m._31 = (yw + xz) * 2.0f;
        m._32 = (yz - xw) * 2.0f;
        m._33 = 1.0f - (yy + xx) * 2.0f;
        m._34 = 0.0f;
        m._41 = 0.0f;
        m._42 = 0.0f;
        m._43 = 0.0f;
        m._44 = 1.0f;
        CMatrix const rotation(m);
        CVector const up(
            rotation._31 * INITIAL_UP_DIRECTION.z + rotation._21 * INITIAL_UP_DIRECTION.y + rotation._11 * INITIAL_UP_DIRECTION.x,
            rotation._32 * INITIAL_UP_DIRECTION.z + rotation._22 * INITIAL_UP_DIRECTION.y + rotation._12 * INITIAL_UP_DIRECTION.x,
            rotation._33 * INITIAL_UP_DIRECTION.z + rotation._23 * INITIAL_UP_DIRECTION.y + rotation._13 * INITIAL_UP_DIRECTION.x);

        // Gravity splits into a pull down the slope and the part the ground holds up.
        CVector const projectedGravity = ProjectVectorOntoPlane(up, gravityForce);
        double const nx = gravityForce.x - projectedGravity.x;
        double const ny = gravityForce.y - projectedGravity.y;
        double const nz = gravityForce.z - projectedGravity.z;
        float const reactionOfSupport = static_cast<float>(sqrt(nz * nz + ny * ny + nx * nx));

        CVector engineForce = ZeroVector;
        CVector friction;
        if (m_vehicle->bIsBraking())
        {
            if (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z > 0.001f)
            {
                // Skid against the motion.
                float const inv = static_cast<float>(1.0 / sqrt(double(m_velocity.x) * m_velocity.x +
                                                                double(m_velocity.y) * m_velocity.y +
                                                                double(m_velocity.z) * m_velocity.z + 0.00000011920929));
                friction.x = (0.0f - inv * m_velocity.x) * reactionOfSupport * mu;
                friction.y = (0.0f - m_velocity.y * inv) * reactionOfSupport * mu;
                friction.z = (0.0f - m_velocity.z * inv) * reactionOfSupport * mu;
            }
            else
            {
                // Hold still against the slope, as far as the grip allows.
                float const pullSq = static_cast<float>(double(projectedGravity.y) * projectedGravity.y +
                                                        double(projectedGravity.z) * projectedGravity.z +
                                                        double(projectedGravity.x) * projectedGravity.x);
                float const inv = static_cast<float>(1.0 / sqrt(pullSq + 0.00000011920929));
                friction.x = (0.0f - inv * projectedGravity.x) * reactionOfSupport * mu;
                friction.z = (0.0f - projectedGravity.z * inv) * reactionOfSupport * mu;
                friction.y = (0.0f - projectedGravity.y * inv) * reactionOfSupport * mu;
                // Grip beyond the pull only cancels it; otherwise the vehicle slides at full grip.
                if (friction.x * friction.x + friction.z * friction.z + friction.y * friction.y > pullSq)
                {
                    friction.x = 0.0f - projectedGravity.x;
                    friction.y = 0.0f - projectedGravity.y;
                    friction.z = 0.0f - projectedGravity.z;
                }
            }
        }
        else
        {
            float wheelAVel;
            float engineRpm;
            int gear;
            CalcRpmsAndGear(wheelAVel, engineRpm, gear);
            float torque = m_vehicle->GetMaxTorque() * m_vehicle->m_realThrottle;
            float const maxSpeed =
                m_vehicle->GetAttackStatus() == Vehicle::ATTACK_ATTACKING ? m_vehicle->GetMaxSpeed() : m_vehicle->GetCruisingSpeed();
            if ((fabs(wheelAVel) * m_wheelRadius - maxSpeed > 0.1 && !m_vehicle->bIsBraking()) ||
                engineRpm > m_vehicle->m_maxEngineRpm || engineRpm < -4000.0f)
            {
                torque = 0.0f;
            }

            // The engine pushes along the vehicle, up to what the grip allows.
            CVector const dir = m_vehicle->GetDirection();
            float const gearRatio = Vehicle::GEAR_RATIOS[gear];
            float const diffRatio = m_vehicle->m_diffRatio;
            float const invWheelRadius = 1.0f / m_wheelRadius;
            engineForce.x = dir.x * gearRatio * diffRatio * 1.8f * torque * invWheelRadius;
            engineForce.y = dir.y * gearRatio * diffRatio * 1.8f * torque * invWheelRadius;
            engineForce.z = dir.z * gearRatio * diffRatio * 1.8f * torque * invWheelRadius;
            double const engineSq = double(engineForce.z) * engineForce.z + double(engineForce.y) * engineForce.y +
                double(engineForce.x) * engineForce.x;
            if (sqrt(engineSq) > reactionOfSupport * mu)
            {
                float const inv = static_cast<float>(1.0 / sqrt(engineSq + 0.00000011920929));
                engineForce.x = inv * engineForce.x * reactionOfSupport * mu;
                engineForce.y = engineForce.y * inv * reactionOfSupport * mu;
                engineForce.z = engineForce.z * inv * reactionOfSupport * mu;
            }

            // The tyres resist sliding sideways, with one and a half times the grip.
            CVector const side(dir.z * up.y - dir.y * up.z, up.z * dir.x - dir.z * up.x, dir.y * up.x - up.y * dir.x);
            float const sideSpeed = m_velocity.z * side.z + m_velocity.y * side.y + m_velocity.x * side.x;
            CVector const sideVel(side.x * sideSpeed, side.y * sideSpeed, side.z * sideSpeed);
            float const sideSq = sideVel.x * sideVel.x + sideVel.z * sideVel.z + sideVel.y * sideVel.y;
            if (sideSq > 0.1f)
            {
                float const inv = static_cast<float>(1.0 / sqrt(sideSq + 0.00000011920929f));
                friction.x = (0.0f - inv * sideVel.x) * reactionOfSupport * sideMu;
                friction.y = (0.0f - sideVel.y * inv) * reactionOfSupport * sideMu;
                friction.z = (0.0f - sideVel.z * inv) * reactionOfSupport * sideMu;
            }
            else
            {
                friction = CVector(0.0f, 0.0f, 0.0f);
            }
        }

        // The soil's rolling resistance, for every wheel slot.
        CVector resistance = friction;
        if (Wheel const* wheel = m_vehicle->GetFirstExistingWheel())
        {
            float const wheelCount = static_cast<float>(static_cast<unsigned>(m_vehicle->m_wheels.size()));
            float const rx = (0.0f - m_velocity.x) * soil.m_resistance * wheelCount;
            float const ry = (0.0f - m_velocity.y) * soil.m_resistance * wheelCount;
            float const rz = (0.0f - m_velocity.z) * soil.m_resistance * wheelCount;
            float const wheelMass = wheel->GetMass();
            float const invRadius = 1.0f / wheel->GetRadius();
            resistance.x = invRadius * (rx * wheelMass) + friction.x;
            resistance.y = (ry * wheelMass) * invRadius + friction.y;
            resistance.z = (rz * wheelMass) * invRadius + friction.z;
        }

        // Integrate.
        float const invMass = static_cast<float>(1.0 / mass);
        CVector const accel(
            (engineForce.x + projectedGravity.x + resistance.x) * invMass,
            (engineForce.y + projectedGravity.y + resistance.y) * invMass,
            (engineForce.z + projectedGravity.z + resistance.z) * invMass);
        CVector const dv(accel.x * elapsedTime, accel.y * elapsedTime, accel.z * elapsedTime);
        CVector newPosition(
            (m_velocity.x * elapsedTime + groundPos.x) + dv.x * elapsedTime * 0.5f,
            (groundPos.y + m_velocity.y * elapsedTime) + dv.y * elapsedTime * 0.5f,
            (m_velocity.z * elapsedTime + groundPos.z) + dv.z * elapsedTime * 0.5f);
        float const levelSizeX = pServer->GetLevelSize();
        if (newPosition.x < 0.0f)
        {
            newPosition.x = 0.0f;
        }
        if (newPosition.x > levelSizeX)
        {
            newPosition.x = levelSizeX;
        }
        float const levelSizeZ = pServer->GetLevelSize();
        if (newPosition.z < 0.0f)
        {
            newPosition.z = 0.0f;
        }
        if (newPosition.z > levelSizeZ)
        {
            newPosition.z = levelSizeZ;
        }

        // Unless the ground drops or rises straight up, keep the distance covered but lay it
        // along the ground.
        CVector const newPositionOnGround = GetGroundPos(newPosition, false, false);
        float const gx = newPositionOnGround.x - groundPos.x;
        float const gy = newPositionOnGround.y - groundPos.y;
        float const gz = newPositionOnGround.z - groundPos.z;
        if (sqrt(double(gx) * gx + double(gz) * gz + double(gy) * gy) > 0.1)
        {
            float const inv = static_cast<float>(1.0 / sqrt(double(gx) * gx + double(gz) * gz + double(gy) * gy + 0.00000011920929));
            if (fabs(double(gz * inv) * INITIAL_UP_DIRECTION.z + double(gy * inv) * INITIAL_UP_DIRECTION.y +
                     double(inv * gx) * INITIAL_UP_DIRECTION.x) < 0.99000001)
            {
                float const inv2 = static_cast<float>(1.0 / sqrt(double(gz) * gz + double(gy) * gy + double(gx) * gx + 0.00000011920929));
                double const mz = newPosition.z - groundPos.z;
                double const my = newPosition.y - groundPos.y;
                float const mx = newPosition.x - groundPos.x;
                float const dist = static_cast<float>(sqrt(mz * mz + my * my + double(mx) * mx));
                newPosition = CVector(
                    inv2 * gx * dist + groundPos.x,
                    gy * inv2 * dist + groundPos.y,
                    gz * inv2 * dist + groundPos.z);
            }
        }
        m_vehicle->SetGamePositionOnGround(newPosition, false, true);
        m_velocity.x = dv.x + m_velocity.x;
        m_velocity.y = dv.y + m_velocity.y;
        m_velocity.z = dv.z + m_velocity.z;

        // The speed can be no more than the distance actually covered allows.
        CVector const finalGroundPos = GetGroundPos(newPosition, true, false);
        float const invTime = static_cast<float>(1.0 / elapsedTime);
        float const vx = invTime * (finalGroundPos.x - groundPos.x);
        float const vy = (finalGroundPos.y - groundPos.y) * invTime;
        float const vz = (finalGroundPos.z - groundPos.z) * invTime;
        float const maxVel = static_cast<float>(sqrt(double(vz) * vz + double(vy) * vy + double(vx) * vx));
        if (sqrt(double(m_velocity.x) * m_velocity.x + double(m_velocity.y) * m_velocity.y + double(m_velocity.z) * m_velocity.z) >
            maxVel)
        {
            float const inv = static_cast<float>(1.0 / sqrt(double(m_velocity.x) * m_velocity.x + double(m_velocity.y) * m_velocity.y +
                                                            double(m_velocity.z) * m_velocity.z + 0.00000011920929));
            m_velocity.x = inv * m_velocity.x * maxVel;
            m_velocity.y = inv * m_velocity.y * maxVel;
            m_velocity.z = inv * m_velocity.z * maxVel;
        }
    }
}  // namespace ai

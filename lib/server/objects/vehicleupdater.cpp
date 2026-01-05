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
        // TODO: generated code
        // Get vehicle position and ground information
        CVector vehiclePos = m_vehicle->GetPosition();

        CVector m_velocityMaximum = ai::GetGroundPos(vehiclePos, false, false);

        // Get terrain properties based on position
        m3d::CWorld* world = ai::pServer->GetWorld();
        float levelSize = ai::pServer->GetLevelSize();
        int tileSize = world->GetLandscape().GetTileSize();
        float scaleFactor = levelSize / static_cast<float>(tileSize);

        int terrainX = static_cast<int>((1.0f / scaleFactor) * m_velocityMaximum.x + 0.5f);
        int terrainZ = static_cast<int>((1.0f / scaleFactor) * m_velocityMaximum.z + 0.5f);

        ai::DynamicScene::SoilProps const& soilProps = ai::gDynamicScene->GetSoilProps(terrainX, terrainZ);

        // Calculate basic physics properties
        float vehicleMass = m_vehicle->GetMass();
        float friction = soilProps.m_friction;
        float lateralFriction = friction * 1.5f;

        // Calculate gravity force
        CVector gravityForce = {0.0f, vehicleMass * -9.81f, 0.0f};

        // Get vehicle rotation and build rotation matrix
        Quaternion vehicleRot = m_vehicle->GetRotation();

        // Build rotation matrix from quaternion
        CMatrix rotationMatrix;
        float xx = vehicleRot.x * vehicleRot.x;
        float xy = vehicleRot.x * vehicleRot.y;
        float xz = vehicleRot.x * vehicleRot.z;
        float xw = vehicleRot.x * vehicleRot.w;
        float yy = vehicleRot.y * vehicleRot.y;
        float yz = vehicleRot.y * vehicleRot.z;
        float yw = vehicleRot.y * vehicleRot.w;
        float zz = vehicleRot.z * vehicleRot.z;
        float zw = vehicleRot.z * vehicleRot.w;

        rotationMatrix._11 = 1.0f - 2.0f * (yy + zz);
        rotationMatrix._12 = 2.0f * (xy + zw);
        rotationMatrix._13 = 2.0f * (xz - yw);
        rotationMatrix._14 = 0.0f;

        rotationMatrix._21 = 2.0f * (xy - zw);
        rotationMatrix._22 = 1.0f - 2.0f * (xx + zz);
        rotationMatrix._23 = 2.0f * (yz + xw);
        rotationMatrix._24 = 0.0f;

        rotationMatrix._31 = 2.0f * (xz + yw);
        rotationMatrix._32 = 2.0f * (yz - xw);
        rotationMatrix._33 = 1.0f - 2.0f * (xx + yy);
        rotationMatrix._34 = 0.0f;

        rotationMatrix._41 = 0.0f;
        rotationMatrix._42 = 0.0f;
        rotationMatrix._43 = 0.0f;
        rotationMatrix._44 = 1.0f;

        // Transform up vector by rotation matrix
        CVector INITIAL_UP_DIRECTION_35(0.0, 1.0, 0.0);
        CVector up;
        up.x = rotationMatrix._11 * INITIAL_UP_DIRECTION_35.x + rotationMatrix._21 * INITIAL_UP_DIRECTION_35.y +
            rotationMatrix._31 * INITIAL_UP_DIRECTION_35.z;
        up.y = rotationMatrix._12 * INITIAL_UP_DIRECTION_35.x + rotationMatrix._22 * INITIAL_UP_DIRECTION_35.y +
            rotationMatrix._32 * INITIAL_UP_DIRECTION_35.z;
        up.z = rotationMatrix._13 * INITIAL_UP_DIRECTION_35.x + rotationMatrix._23 * INITIAL_UP_DIRECTION_35.y +
            rotationMatrix._33 * INITIAL_UP_DIRECTION_35.z;

        // Project gravity force onto ground plane
        CVector projectedGravity = gravityForce;
        ai::ProjectVectorOntoPlane(up, projectedGravity);

        // Calculate reaction force magnitude
        CVector gravityDiff = {
            gravityForce.x - projectedGravity.x, gravityForce.y - projectedGravity.y, gravityForce.z - projectedGravity.z};
        float reactionForce = std::sqrt(gravityDiff.x * gravityDiff.x + gravityDiff.y * gravityDiff.y + gravityDiff.z * gravityDiff.z);

        CVector engineForce = ZeroVector;
        CVector frictionForce = ZeroVector;

        // Handle braking vs driving
        if (m_vehicle->bIsBraking())
        {
            // Braking logic
            float speedSq = m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z;

            if (speedSq > 0.001f)
            {
                // Apply friction against current velocity direction
                float invSpeed = 1.0f / std::sqrt(speedSq + 1.1920929e-07f);
                frictionForce.x = (-m_velocity.x * invSpeed) * reactionForce * friction;
                frictionForce.y = (-m_velocity.y * invSpeed) * reactionForce * friction;
                frictionForce.z = (-m_velocity.z * invSpeed) * reactionForce * friction;
            }
            else
            {
                // Apply friction against gravity direction when stationary
                float gravityMagSq = projectedGravity.x * projectedGravity.x + projectedGravity.y * projectedGravity.y +
                    projectedGravity.z * projectedGravity.z;
                float invGravityMag = 1.0f / std::sqrt(gravityMagSq + 1.1920929e-07f);

                frictionForce.x = (-projectedGravity.x * invGravityMag) * reactionForce * friction;
                frictionForce.y = (-projectedGravity.y * invGravityMag) * reactionForce * friction;
                frictionForce.z = (-projectedGravity.z * invGravityMag) * reactionForce * friction;

                // Clamp friction force if it exceeds available force
                float frictionMagSq =
                    frictionForce.x * frictionForce.x + frictionForce.y * frictionForce.y + frictionForce.z * frictionForce.z;
                if (frictionMagSq > gravityMagSq)
                {
                    frictionForce.x = -projectedGravity.x;
                    frictionForce.y = -projectedGravity.y;
                    frictionForce.z = -projectedGravity.z;
                }
            }
        }
        else
        {
            // Driving logic - calculate engine force
            float wheelAngularVel, engineRpm;
            int currentGear;
            CalcRpmsAndGear(wheelAngularVel, engineRpm, currentGear);

            float maxTorque = m_vehicle->GetMaxTorque();
            float throttle = m_vehicle->m_realThrottle;
            float torque = maxTorque * throttle;

            // Determine speed limit based on attack status
            float speedLimit;
            if (m_vehicle->GetAttackStatus() == 1)
            {
                speedLimit = m_vehicle->GetMaxSpeed();
            }
            else
            {
                speedLimit = m_vehicle->GetCruisingSpeed();
            }

            // Limit torque if exceeding speed limits or RPM limits
            float currentSpeed = std::fabs(wheelAngularVel) * m_wheelRadius;
            if ((currentSpeed - speedLimit > 0.1f && !m_vehicle->bIsBraking()) || engineRpm > m_vehicle->m_maxEngineRpm ||
                engineRpm < -4000.0f)
            {
                torque = 0.0f;
            }

            // Calculate engine force in vehicle direction
            CVector vehicleDir = m_vehicle->GetDirection();

            float gearRatio = Vehicle::GEAR_RATIOS[currentGear];
            float diffRatio = m_vehicle->m_diffRatio;
            float finalDriveRatio = 1.8f;

            engineForce.x = vehicleDir.x * gearRatio * diffRatio * finalDriveRatio * torque;
            engineForce.y = vehicleDir.y * gearRatio * diffRatio * finalDriveRatio * torque;
            engineForce.z = vehicleDir.z * gearRatio * diffRatio * finalDriveRatio * torque;

            // Convert to linear force at wheels
            float invWheelRadius = 1.0f / m_wheelRadius;
            engineForce.x *= invWheelRadius;
            engineForce.y *= invWheelRadius;
            engineForce.z *= invWheelRadius;

            // Limit engine force by available traction
            float engineForceMagSq = engineForce.x * engineForce.x + engineForce.y * engineForce.y + engineForce.z * engineForce.z;
            if (std::sqrt(engineForceMagSq) > reactionForce * friction)
            {
                float invEngineForceMag = 1.0f / std::sqrt(engineForceMagSq + 1.1920929e-07f);
                engineForce.x = (engineForce.x * invEngineForceMag) * reactionForce * friction;
                engineForce.y = (engineForce.y * invEngineForceMag) * reactionForce * friction;
                engineForce.z = (engineForce.z * invEngineForceMag) * reactionForce * friction;
            }

            // Calculate lateral friction (sideways force)
            CVector lateralDir = {
                vehicleDir.y * up.z - up.y * vehicleDir.z,
                vehicleDir.z * up.x - vehicleDir.x * up.z,
                vehicleDir.x * up.y - vehicleDir.y * up.x};

            float lateralSpeed = m_velocity.x * lateralDir.x + m_velocity.y * lateralDir.y + m_velocity.z * lateralDir.z;

            CVector lateralFrictionForce = {lateralDir.x * lateralSpeed, lateralDir.y * lateralSpeed, lateralDir.z * lateralSpeed};

            float lateralFrictionMagSq = lateralFrictionForce.x * lateralFrictionForce.x + lateralFrictionForce.y * lateralFrictionForce.y +
                lateralFrictionForce.z * lateralFrictionForce.z;

            if (lateralFrictionMagSq > 0.1f)
            {
                float invLateralFrictionMag = 1.0f / std::sqrt(lateralFrictionMagSq + 1.1920929e-07f);
                frictionForce.x = (-lateralFrictionForce.x * invLateralFrictionMag) * reactionForce * lateralFriction;
                frictionForce.y = (-lateralFrictionForce.y * invLateralFrictionMag) * reactionForce * lateralFriction;
                frictionForce.z = (-lateralFrictionForce.z * invLateralFrictionMag) * reactionForce * lateralFriction;
            }
        }

        // Calculate rolling resistance from wheels
        CVector resistanceForce = ZeroVector;
        ai::Wheel const* firstWheel = m_vehicle->GetFirstExistingWheel();

        if (firstWheel)
        {
            float resistance = soilProps.m_resistance;
            int wheelCount = m_vehicle->m_wheels.size();

            resistanceForce.x = (-m_velocity.x * resistance) * wheelCount;
            resistanceForce.y = (-m_velocity.y * resistance) * wheelCount;
            resistanceForce.z = (-m_velocity.z * resistance) * wheelCount;

            float wheelMass = firstWheel->GetMass();
            resistanceForce.x *= wheelMass;
            resistanceForce.y *= wheelMass;
            resistanceForce.z *= wheelMass;

            // Convert to acceleration (assuming some radius conversion)
            float invRadius = 1.0f / firstWheel->GetRadius();
            resistanceForce.x *= invRadius;
            resistanceForce.y *= invRadius;
            resistanceForce.z *= invRadius;
        }

        // Calculate total acceleration
        CVector acceleration;
        acceleration.x = (engineForce.x + projectedGravity.x + frictionForce.x + resistanceForce.x) / vehicleMass;
        acceleration.y = (engineForce.y + projectedGravity.y + frictionForce.y + resistanceForce.y) / vehicleMass;
        acceleration.z = (engineForce.z + projectedGravity.z + frictionForce.z + resistanceForce.z) / vehicleMass;

        // Update position using kinematic equations
        CVector newPosition;
        newPosition.x = m_velocityMaximum.x + (m_velocity.x * elapsedTime) + (0.5f * acceleration.x * elapsedTime * elapsedTime);
        newPosition.y = m_velocityMaximum.y + (m_velocity.y * elapsedTime) + (0.5f * acceleration.y * elapsedTime * elapsedTime);
        newPosition.z = m_velocityMaximum.z + (m_velocity.z * elapsedTime) + (0.5f * acceleration.z * elapsedTime * elapsedTime);

        // Clamp position to level boundaries
        levelSize = ai::pServer->GetLevelSize();
        newPosition.x = std::max(0.0f, std::min(newPosition.x, levelSize));
        newPosition.z = std::max(0.0f, std::min(newPosition.z, levelSize));

        // Adjust position based on ground collision
        CVector newGroundPos = ai::GetGroundPos(newPosition, false, false);
        CVector groundOffset = {
            newGroundPos.x - m_velocityMaximum.x, newGroundPos.y - m_velocityMaximum.y, newGroundPos.z - m_velocityMaximum.z};

        float groundOffsetDist =
            std::sqrt(groundOffset.x * groundOffset.x + groundOffset.y * groundOffset.y + groundOffset.z * groundOffset.z);

        if (groundOffsetDist > 0.1f)
        {
            float invGroundOffsetDist = 1.0f / std::sqrt(groundOffsetDist * groundOffsetDist + 1.1920929e-07f);
            CVector groundDir = {
                groundOffset.x * invGroundOffsetDist, groundOffset.y * invGroundOffsetDist, groundOffset.z * invGroundOffsetDist};

            // Check if ground normal is not too steep
            float upDot =
                groundDir.x * INITIAL_UP_DIRECTION_35.x + groundDir.y * INITIAL_UP_DIRECTION_35.y + groundDir.z * INITIAL_UP_DIRECTION_35.z;

            if (std::fabs(upDot) < 0.99f)
            {
                // Project movement along ground surface
                CVector movement = {
                    newPosition.x - m_velocityMaximum.x, newPosition.y - m_velocityMaximum.y, newPosition.z - m_velocityMaximum.z};
                float movementDist = std::sqrt(movement.x * movement.x + movement.y * movement.y + movement.z * movement.z);

                newPosition.x = m_velocityMaximum.x + groundDir.x * movementDist;
                newPosition.y = m_velocityMaximum.y + groundDir.y * movementDist;
                newPosition.z = m_velocityMaximum.z + groundDir.z * movementDist;
            }
        }

        // Update vehicle position
        m_vehicle->SetGamePositionOnGround(newPosition, false, true);

        // Update velocity
        m_velocity.x += acceleration.x * elapsedTime;
        m_velocity.y += acceleration.y * elapsedTime;
        m_velocity.z += acceleration.z * elapsedTime;

        // Calculate ground-based velocity and clamp if necessary
        CVector finalGroundPos = ai::GetGroundPos(newPosition, true, false);
        m_velocityMaximum.x = (finalGroundPos.x - m_velocityMaximum.x) / elapsedTime;
        m_velocityMaximum.y = (finalGroundPos.y - m_velocityMaximum.y) / elapsedTime;
        m_velocityMaximum.z = (finalGroundPos.z - m_velocityMaximum.z) / elapsedTime;

        float groundSpeed = std::sqrt(
            m_velocityMaximum.x * m_velocityMaximum.x + m_velocityMaximum.y * m_velocityMaximum.y +
            m_velocityMaximum.z * m_velocityMaximum.z);

        float currentSpeed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);

        // Clamp velocity to ground speed if necessary
        if (currentSpeed > groundSpeed)
        {
            float invCurrentSpeed = 1.0f / std::sqrt(currentSpeed * currentSpeed + 1.1920929e-07f);
            m_velocity.x = (m_velocity.x * invCurrentSpeed) * groundSpeed;
            m_velocity.y = (m_velocity.y * invCurrentSpeed) * groundSpeed;
            m_velocity.z = (m_velocity.z * invCurrentSpeed) * groundSpeed;
        }
    }
}  // namespace ai

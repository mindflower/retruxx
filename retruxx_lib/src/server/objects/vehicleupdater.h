#pragma once
#include "vehicle.h"

namespace ai
{
    class VehicleUpdater
    {
    public:
        VehicleUpdater(const ai::VehicleUpdater&);
        VehicleUpdater(ai::Vehicle* vehicle);
        ~VehicleUpdater();
        void Update(float elapsedTime);
        CVector GetLinearVelocity() const;
        void CalcRpmsAndGear(float& wheelAVel, float& engineRpm, int& gear) const;

    private:
        /* 0x0000 */ ai::Vehicle* m_vehicle;
        /* 0x0004 */ float m_wheelRadius;
        /* 0x0008 */ CVector m_velocity;
        /* 0x0014 */ CVector m_relFrontPoint;
        /* 0x0020 */ CVector m_relRearPoint;
        float _CalcWheelAVel() const;
        void _UpdateForceAndVelocity(float elapsedTime);
    }; /* size: 0x002c */
}

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
        ai::Vehicle* m_vehicle;
        float m_wheelRadius;
        CVector m_velocity;
        CVector m_relFrontPoint;
        CVector m_relRearPoint;
        float _CalcWheelAVel() const;
        void _UpdateForceAndVelocity(float elapsedTime);
    };
}

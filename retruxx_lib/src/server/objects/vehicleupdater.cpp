#include "vehicleupdater.h"

#include <stdexcept>

namespace ai
{
    VehicleUpdater::VehicleUpdater(const ai::VehicleUpdater&)
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleUpdater::Update(float elapsedTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector VehicleUpdater::GetLinearVelocity() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleUpdater::CalcRpmsAndGear(float& wheelAVel, float& engineRpm, int& gear) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float VehicleUpdater::_CalcWheelAVel() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehicleUpdater::_UpdateForceAndVelocity(float elapsedTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

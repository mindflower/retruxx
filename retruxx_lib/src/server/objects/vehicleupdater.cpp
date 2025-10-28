#include "vehicleupdater.h"

#include <stdexcept>

namespace ai
{
    VehicleUpdater::VehicleUpdater(const ai::VehicleUpdater&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void VehicleUpdater::Update(float elapsedTime)
    {
        throw std::logic_error("Not implemented");
    }

    CVector VehicleUpdater::GetLinearVelocity() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleUpdater::CalcRpmsAndGear(float& wheelAVel, float& engineRpm, int& gear) const
    {
        throw std::logic_error("Not implemented");
    }

    float VehicleUpdater::_CalcWheelAVel() const
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleUpdater::_UpdateForceAndVelocity(float elapsedTime)
    {
        throw std::logic_error("Not implemented");
    }
}

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
        throw std::logic_error("Not implemented");
    }

    VehicleUpdater::~VehicleUpdater()
    {
        throw std::logic_error("Not implemented");
    }

    void VehicleUpdater::Update(float elapsedTime)
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

#pragma once
#include <math/vector.h>

namespace ai
{
    class Vehicle;

    class VehicleUpdater
    {
    public:
        void Update(float);
        VehicleUpdater(Vehicle *);
        CVector GetLinearVelocity() const ;
        ~VehicleUpdater();
        void CalcRpmsAndGear(float &,float &,int &) const ;

    private:
        void _UpdateForceAndVelocity(float);
        float _CalcWheelAVel() const ;

    private:
        Vehicle *m_vehicle;
        float m_wheelRadius;
        CVector m_velocity;
        CVector m_relFrontPoint;
        CVector m_relRearPoint;
    };
}

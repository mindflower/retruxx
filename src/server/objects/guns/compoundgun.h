#pragma once
#include "gun.h"
#include <server/objects/physicbodies/compoundvehiclepart.h>

namespace ai
{
    class CompoundGunPrototypeInfo : public CompoundVehiclePartPrototypeInfo
    {
    public:
        FiringTypes GetFiringType() const;

    protected:
        virtual Obj* CreateTargetObject() const;
    };

    class CompoundGun : public CompoundVehiclePart
    {
    public:
        virtual bool CanFire() const;
        virtual unsigned int GetPrice(IPriceCoeffProvider const*) const;
        void SetProperTargetId(int, int);
        static m3d::Class* GetBaseClass();
        void SetChargeState(Gun::ChargeState);
        bool IsDurabilityEnoughForFiring() const;
        DamageType GetDamageType() const;
        void SetShellsInCurrentCharge(unsigned int);
        bool IsWithShellsPoolLimit() const;
        float GetDamage() const;
        Gun::ChargeState GetChargeState() const;
        int GetShellPrototypeId() const;
        float EstimateDamageFromPosition(CVector const&, CVector const&, std::vector<int, std::allocator<int> > const&) const;
        void Recharge();
        void LookAtPoint(CVector const&, float);
        bool PointIsReachable(CVector const&, std::vector<int, std::allocator<int> > const&) const;
        unsigned int GetShellsInCurrentCharge() const;
        float GetCurrentRechargingTime() const;
        unsigned int GetShellsPoolSize() const;
        float GetFiringRange() const;
        unsigned int GetChargeSize() const;
        unsigned int GetShellsInPool() const;
        bool IsWithCharging() const;
        float GetFiringRate() const;
        CompoundGun(CompoundGunPrototypeInfo const&);
        bool CanShotToTarget(int) const;
        bool Fire(bool);
        void SetShellsInPool(unsigned int);
        virtual CompoundGunPrototypeInfo const* GetPrototypeInfo() const;
        float EstimateDamage(CVector const&, std::vector<int, std::allocator<int> > const&) const;
        float EstimateDamage() const;
        bool isLookAtPoint(CVector const&, float) const;
        virtual m3d::Class* GetClass() const;
        float GetRechargingTime() const;

    protected:
        virtual ~CompoundGun();

    private:
        virtual m3d::Object* Clone();
        static m3d::Object* CreateObject();

    public:
        RT_CLASS_DECLARE(CompoundGun);
    };
}

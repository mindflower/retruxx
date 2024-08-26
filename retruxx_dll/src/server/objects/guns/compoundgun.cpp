#include "compoundgun.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompoundGun)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompoundGun);

    FiringTypes CompoundGunPrototypeInfo::GetFiringType() const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* CompoundGunPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::CanFire() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CompoundGun::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void CompoundGun::SetProperTargetId(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CompoundGun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(CompoundVehiclePart);
    }

    void CompoundGun::SetChargeState(Gun::ChargeState)
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::IsDurabilityEnoughForFiring() const
    {
        throw std::logic_error("Not implemented");
    }

    DamageType CompoundGun::GetDamageType() const
    {
        throw std::logic_error("Not implemented");
    }

    void CompoundGun::SetShellsInCurrentCharge(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::IsWithShellsPoolLimit() const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::GetDamage() const
    {
        throw std::logic_error("Not implemented");
    }

    Gun::ChargeState CompoundGun::GetChargeState() const
    {
        throw std::logic_error("Not implemented");
    }

    int CompoundGun::GetShellPrototypeId() const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::EstimateDamageFromPosition(CVector const&, CVector const&,
        std::vector<int, std::allocator<int>> const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void CompoundGun::Recharge()
    {
        throw std::logic_error("Not implemented");
    }

    void CompoundGun::LookAtPoint(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::PointIsReachable(CVector const&, std::vector<int, std::allocator<int>> const&) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CompoundGun::GetShellsInCurrentCharge() const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::GetCurrentRechargingTime() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CompoundGun::GetShellsPoolSize() const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::GetFiringRange() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CompoundGun::GetChargeSize() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned CompoundGun::GetShellsInPool() const
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::IsWithCharging() const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::GetFiringRate() const
    {
        throw std::logic_error("Not implemented");
    }

    CompoundGun::CompoundGun(CompoundGunPrototypeInfo const& prototype) : CompoundVehiclePart(prototype)
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::CanShotToTarget(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::Fire(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void CompoundGun::SetShellsInPool(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    CompoundGunPrototypeInfo const* CompoundGun::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::EstimateDamage(CVector const&, std::vector<int, std::allocator<int>> const&) const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::EstimateDamage() const
    {
        throw std::logic_error("Not implemented");
    }

    bool CompoundGun::isLookAtPoint(CVector const&, float) const
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* CompoundGun::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    float CompoundGun::GetRechargingTime() const
    {
        throw std::logic_error("Not implemented");
    }

    CompoundGun::~CompoundGun()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CompoundGun::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* CompoundGun::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }
}

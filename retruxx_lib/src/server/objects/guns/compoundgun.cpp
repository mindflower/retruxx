#include "compoundgun.h"

#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompoundGun)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompoundGun);

    FiringTypes CompoundGunPrototypeInfo::GetFiringType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Obj* CompoundGunPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::CanFire() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned CompoundGun::GetPrice(IPriceCoeffProvider const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompoundGun::SetProperTargetId(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CompoundGun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(CompoundVehiclePart);
    }

    void CompoundGun::SetChargeState(Gun::ChargeState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::IsDurabilityEnoughForFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DamageType CompoundGun::GetDamageType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompoundGun::SetShellsInCurrentCharge(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::IsWithShellsPoolLimit() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::GetDamage() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::ChargeState CompoundGun::GetChargeState() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int CompoundGun::GetShellPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::EstimateDamageFromPosition(CVector const&, CVector const&,
        std::vector<int, std::allocator<int>> const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompoundGun::Recharge()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompoundGun::LookAtPoint(CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::PointIsReachable(CVector const&, std::vector<int, std::allocator<int>> const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned CompoundGun::GetShellsInCurrentCharge() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::GetCurrentRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned CompoundGun::GetShellsPoolSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::GetFiringRange() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned CompoundGun::GetChargeSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned CompoundGun::GetShellsInPool() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::IsWithCharging() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::GetFiringRate() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompoundGun::CompoundGun(CompoundGunPrototypeInfo const& prototype) : CompoundVehiclePart(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::CanShotToTarget(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::Fire(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void CompoundGun::SetShellsInPool(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompoundGunPrototypeInfo const* CompoundGun::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::EstimateDamage(CVector const&, std::vector<int, std::allocator<int>> const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::EstimateDamage() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool CompoundGun::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* CompoundGun::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float CompoundGun::GetRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CompoundGun::~CompoundGun()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CompoundGun::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* CompoundGun::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

#include "compoundgun.h"

#include <stdexcept>

#include "core/kernel.h"
#include "rocketlauncher.h"
#include "server/objects/base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompoundGun)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompoundGun);

    FiringTypes CompoundGunPrototypeInfo::GetFiringType() const
    {
        // RVA 0x6E5560 - the firing type of the first part, if that part is a gun.
        if (m_PartInfo.empty())
        {
            return FT_NUM_FIRING_TYPES;
        }
        PrototypeInfo const* partPrototype = thePrototypeManager->GetPrototypeInfo(m_PartInfo.begin()->second.prototypeId);
        if (!partPrototype)
        {
            return FT_NUM_FIRING_TYPES;
        }
        m3d::Class* partClass = m3d::g_Kernel->FindClass(partPrototype->m_className.c_str());
        if (!partClass || !partClass->IsKindOf(RT_CLASS_LOCAL(Gun)))
        {
            return FT_NUM_FIRING_TYPES;
        }
        return static_cast<GunPrototypeInfo const*>(partPrototype)->m_firingType;
    }

    Obj* CompoundGunPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x6E4A10
        return new CompoundGun(*this);
    }

    Gun* CompoundGun::_GetFirstGun() const
    {
        return static_cast<Gun*>(m_vehicleParts.begin()->second.vp);
    }

    bool CompoundGun::CanFire() const
    {
        // RVA 0x6E54E0 - true if any part can fire.
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (static_cast<Gun*>(part.vp)->CanFire())
            {
                return true;
            }
        }
        return false;
    }

    unsigned CompoundGun::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x6E56C0 - the parts' price plus the ammunition the first part carries.
        unsigned price = CompoundVehiclePart::GetPrice(priceCoeffProvider);
        float const priceCoeff = GetPriceCoeff(priceCoeffProvider);
        Gun* gun = _GetFirstGun();
        if (gun->IsWithCharging() && gun->IsWithShellsPoolLimit())
        {
            int const shellPrototypeId = gun->GetShellPrototypeId();
            if (shellPrototypeId != -1)
            {
                // NOTE: the shell prototype is not checked for null.
                PrototypeInfo const* shellPrototype = thePrototypeManager->GetPrototypeInfo(shellPrototypeId);
                unsigned const shells = gun->GetShellsInPool() + gun->GetShellsInCurrentCharge();
                price += static_cast<int>(static_cast<double>(shellPrototype->GetBasePrice() * shells) * priceCoeff);
            }
        }
        return price;
    }

    void CompoundGun::SetProperTargetId(int targetObjId, int lockedObjIdForRockets)
    {
        // RVA 0x6E4E70 - rocket launchers get the locked target, other guns the plain one.
        for (auto const& [name, part] : m_vehicleParts)
        {
            Gun* gun = static_cast<Gun*>(part.vp);
            if (gun->IsKindOf(RT_CLASS_LOCAL(RocketLauncher)))
            {
                gun->SetTargetId(lockedObjIdForRockets);
            }
            else
            {
                gun->SetTargetId(targetObjId);
            }
        }
    }

    m3d::Class* CompoundGun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(CompoundVehiclePart);
    }

    void CompoundGun::SetChargeState(Gun::ChargeState Value)
    {
        // RVA 0x6E5390
        for (auto const& [name, part] : m_vehicleParts)
        {
            static_cast<Gun*>(part.vp)->SetChargeState(Value);
        }
    }

    bool CompoundGun::IsDurabilityEnoughForFiring() const
    {
        // RVA 0x6E4A80 - the durability is truncated, so less than one point is not enough.
        return static_cast<int>(GetDurability()) > 0;
    }

    DamageType CompoundGun::GetDamageType() const
    {
        // RVA 0x6E52D0
        return _GetFirstGun()->GetDamageType();
    }

    void CompoundGun::SetShellsInCurrentCharge(unsigned value)
    {
        // RVA 0x6E55E0
        for (auto const& [name, part] : m_vehicleParts)
        {
            static_cast<Gun*>(part.vp)->SetShellsInCurrentCharge(value);
        }
    }

    bool CompoundGun::IsWithShellsPoolLimit() const
    {
        // RVA 0x6E5360
        return _GetFirstGun()->IsWithShellsPoolLimit();
    }

    float CompoundGun::GetDamage() const
    {
        // RVA 0x6E5240
        float res = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res = static_cast<Gun*>(part.vp)->GetDamage() + res;
        }
        return res;
    }

    Gun::ChargeState CompoundGun::GetChargeState() const
    {
        // RVA 0x6E5380
        return _GetFirstGun()->GetChargeState();
    }

    int CompoundGun::GetShellPrototypeId() const
    {
        // RVA 0x6E5370
        return _GetFirstGun()->GetShellPrototypeId();
    }

    float CompoundGun::EstimateDamageFromPosition(CVector const& position, CVector const& pos,
        std::vector<int, std::allocator<int>> const& exceptions) const
    {
        // RVA 0x6E5190
        float res = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res = static_cast<Gun*>(part.vp)->EstimateDamageFromPosition(position, pos, exceptions) + res;
        }
        return res;
    }

    void CompoundGun::Recharge()
    {
        // RVA 0x6E5410
        for (auto const& [name, part] : m_vehicleParts)
        {
            static_cast<Gun*>(part.vp)->Recharge();
        }
    }

    void CompoundGun::LookAtPoint(CVector const& lookAt, float elapsedTime)
    {
        // RVA 0x6E4FE0
        for (auto const& [name, part] : m_vehicleParts)
        {
            static_cast<Gun*>(part.vp)->LookAtPoint(lookAt, elapsedTime);
        }
    }

    bool CompoundGun::PointIsReachable(CVector const& pos, std::vector<int, std::allocator<int>> const& exceptions) const
    {
        // RVA 0x6E57C0 - true if any part can reach the point.
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (static_cast<Gun*>(part.vp)->PointIsReachable(pos, exceptions))
            {
                return true;
            }
        }
        return false;
    }

    unsigned CompoundGun::GetShellsInCurrentCharge() const
    {
        // RVA 0x6E5320
        return _GetFirstGun()->GetShellsInCurrentCharge();
    }

    float CompoundGun::GetCurrentRechargingTime() const
    {
        // RVA 0x6E5400
        return _GetFirstGun()->GetCurrentRechargingTime();
    }

    unsigned CompoundGun::GetShellsPoolSize() const
    {
        // RVA 0x6E5340
        return _GetFirstGun()->GetShellsPoolSize();
    }

    float CompoundGun::GetFiringRange() const
    {
        // RVA 0x6E52F0
        return _GetFirstGun()->GetFiringRange();
    }

    unsigned CompoundGun::GetChargeSize() const
    {
        // RVA 0x6E5310
        return _GetFirstGun()->GetChargeSize();
    }

    unsigned CompoundGun::GetShellsInPool() const
    {
        // RVA 0x6E5330
        return _GetFirstGun()->GetShellsInPool();
    }

    bool CompoundGun::IsWithCharging() const
    {
        // RVA 0x6E5350
        return _GetFirstGun()->IsWithCharging();
    }

    float CompoundGun::GetFiringRate() const
    {
        // RVA 0x6E52E0
        return _GetFirstGun()->GetFiringRate();
    }

    CompoundGun::CompoundGun(CompoundGunPrototypeInfo const& prototype) : CompoundVehiclePart(prototype)
    {
        // RVA 0x6E49C0
    }

    bool CompoundGun::CanShotToTarget(int targetObjId) const
    {
        // RVA 0x6E5480 - true if any part can shoot the target.
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (static_cast<Gun*>(part.vp)->CanShotToTarget(targetObjId))
            {
                return true;
            }
        }
        return false;
    }

    bool CompoundGun::Fire(bool enable)
    {
        // RVA 0x6E4F00 - every part is told, and the result is true if any of them fired.
        bool fired = false;
        for (auto const& [name, part] : m_vehicleParts)
        {
            fired |= static_cast<Gun*>(part.vp)->Fire(enable);
        }
        return fired;
    }

    void CompoundGun::SetShellsInPool(unsigned Shells)
    {
        // RVA 0x6E5650
        for (auto const& [name, part] : m_vehicleParts)
        {
            static_cast<Gun*>(part.vp)->SetShellsInPool(Shells);
        }
    }

    CompoundGunPrototypeInfo const* CompoundGun::GetPrototypeInfo() const
    {
        // RVA 0x6E5530 - NOTE: the prototype is cast without a type check.
        return static_cast<CompoundGunPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    float CompoundGun::EstimateDamage(CVector const& pos, std::vector<int, std::allocator<int>> const& exceptions) const
    {
        // RVA 0x6E5100
        float res = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res = static_cast<Gun*>(part.vp)->EstimateDamage(pos, exceptions) + res;
        }
        return res;
    }

    float CompoundGun::EstimateDamage() const
    {
        // RVA 0x6E5070
        float res = 0.0f;
        for (auto const& [name, part] : m_vehicleParts)
        {
            res = static_cast<Gun*>(part.vp)->EstimateDamage() + res;
        }
        return res;
    }

    bool CompoundGun::isLookAtPoint(CVector const& lookAt, float eps) const
    {
        // RVA 0x6E4F80 - every part has to be on target; with no parts this is true.
        for (auto const& [name, part] : m_vehicleParts)
        {
            if (!static_cast<Gun*>(part.vp)->isLookAtPoint(lookAt, eps))
            {
                return false;
            }
        }
        return true;
    }

    m3d::Class* CompoundGun::GetClass() const
    {
        // RVA 0x6E49B0
        return RT_CLASS_LOCAL(CompoundGun);
    }

    float CompoundGun::GetRechargingTime() const
    {
        // RVA 0x6E5300
        return _GetFirstGun()->GetRechargingTime();
    }

    // RVA 0x6E49E0
    CompoundGun::~CompoundGun() = default;

    m3d::Object* CompoundGun::Clone()
    {
        // RVA 0x6E4AD0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* CompoundGun::CreateObject()
    {
        // RVA 0x6E4C90
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}

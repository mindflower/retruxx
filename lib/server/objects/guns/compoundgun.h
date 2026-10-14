#pragma once
#include "gun.h"
#include <server/objects/physicbodies/compoundvehiclepart.h>

namespace ai
{
    class CompoundGunPrototypeInfo : public ai::CompoundVehiclePartPrototypeInfo
    {
    public:
        ai::FiringTypes GetFiringType() const;

    protected:
        virtual ai::Obj* CreateTargetObject() const override;
    };

    // A gun made of several guns (for example twin barrels). Commands go to every part; the
    // charging and ammunition queries answer for the first part only.
    class CompoundGun : public ai::CompoundVehiclePart
    {
        friend class CompoundGunPrototypeInfo;

    protected:
        virtual ~CompoundGun() override;

    private:
        CompoundGun(const ai::CompoundGunPrototypeInfo& prototypeInfo);
        CompoundGun(const ai::CompoundGun&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(CompoundGun);
        virtual const ai::CompoundGunPrototypeInfo* GetPrototypeInfo() const override;
        void SetProperTargetId(int targetObjId, int lockedObjIdForRockets);
        bool Fire(bool enable);
        void LookAtPoint(const CVector& lookAt, float elapsedTime);
        bool isLookAtPoint(const CVector& lookAt, float eps) const;
        bool PointIsReachable(const CVector& pos, const std::vector<int, std::allocator<int> >& exceptions) const;
        bool CanShotToTarget(int targetObjId) const;
        float EstimateDamage(const CVector& pos, const std::vector<int, std::allocator<int> >& exceptions) const;
        float EstimateDamage() const;
        float EstimateDamageFromPosition(const CVector& position, const CVector& pos, const std::vector<int, std::allocator<int> >& exceptions) const;
        float GetDamage() const;
        ai::DamageType GetDamageType() const;
        float GetFiringRate() const;
        float GetFiringRange() const;
        float GetRechargingTime() const;
        unsigned int GetChargeSize() const;
        void SetChargeState(ai::Gun::ChargeState Value);
        unsigned int GetShellsInPool() const;
        unsigned int GetShellsPoolSize() const;
        unsigned int GetShellsInCurrentCharge() const;
        bool IsWithCharging() const;
        bool IsWithShellsPoolLimit() const;
        void SetShellsInCurrentCharge(unsigned int value);
        void SetShellsInPool(unsigned int Shells);
        int GetShellPrototypeId() const;
        ai::Gun::ChargeState GetChargeState() const;
        float GetCurrentRechargingTime() const;
        void Recharge();
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override;
        bool IsDurabilityEnoughForFiring() const;
        virtual bool CanFire() const;

    private:
        // The first part answers the queries that are not summed over all parts. NOTE: the
        // shipped code does not check that there is one.
        Gun* _GetFirstGun() const;
    };
}

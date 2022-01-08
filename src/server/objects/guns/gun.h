#pragma once
#include <server/damageinfo.h>
#include <server/objects/physicbodies/vehiclepart.h>

class CMatrix;

namespace ai
{
    enum FiringTypes
    {
        FT_MACHINE_GUN = 0x0,
        FT_CANNON = 0x1,
        FT_SHOT_GUN = 0x2,
        FT_LASER = 0x3,
        FT_PLASMA = 0x4,
        FT_ROCKET = 0x5,
        FT_ARTILLERY = 0x6,
        FT_THUNDERBOLT = 0x7,
        FT_MINE = 0x8,
        FT_NAIL = 0x9,
        FT_TURBO = 0xA,
        FT_OIL = 0xB,
        FT_SMOKE = 0xC,
        FT_NUM_FIRING_TYPES = 0xD,
    };

    class GunPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        static CStr __fastcall DamageType2Str(DamageType);
        GunPrototypeInfo();
        void CreateBlastWave(CVector const&, int) const;
        static CStr __fastcall FiringType2Str(FiringTypes);
        static FiringTypes __fastcall Str2FiringType(CStr const&);
        static DamageType __fastcall Str2DamageType(CStr const&);
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual float GetDamageForOneShell() const;
        virtual void PostLoad();
        short GetExplosionType() const;

    private:
        bool _bIsRapidFiring() const;

    private:
        CStr m_barrelModelName;
        int m_shellPrototypeId;
        float m_damage;
        DamageType m_damageType;
        float m_firingRate;
        float m_firingRange;
        float m_lowStopAngle;
        float m_highStopAngle;
        bool m_ignoreStopAnglesWhenFire;
        unsigned __int16 m_explosionType;
        int m_decalId;
        float m_recoilForce;
        float m_turningSpeed;
        unsigned int m_ChargeSize;
        float m_ReChargingTime;
        float m_ReChargingTimePerShell;
        unsigned int m_ShellsPoolSize;
        bool m_WithCharging;
        bool m_WithShellsPoolLimit;
        int m_blastWavePrototypeId;
        FiringTypes m_firingType;
        std::vector<CMatrix> m_fireLpMatrices;
        CStr m_explosionTypeName;
        CStr m_shellPrototypeName;
        CStr m_blastWavePrototypeName;
    };

    class Gun : public VehiclePart
    {
    public:
        enum ChargeState
        {
            csReady = 0x0,
            csInCharging = 0x1,
        };

    public:
        m3d::SgNode* GetBarrelNode() const;
        virtual bool Fire(bool);
        virtual GunPrototypeInfo const* GetPrototypeInfo() const;
        void SetShellsInPool(unsigned int);
        void RenderGunDebugInfo() const;
        virtual float EstimateDamage(CVector const&, std::vector<int, std::allocator<int> > const&) const;
        virtual float EstimateDamage() const;
        bool bIs360DegreesHoriz() const;
        virtual CStr GetPropertyName(int) const;
        virtual bool CanLookAtTarget() const;
        float GetRechargingTime() const;
        virtual m3d::Class* GetClass() const;
        virtual bool isLookAtPoint(CVector const&, float) const;
        virtual void SetInvisible();
        virtual void GetPropertiesNames(std::set<CStr, std::less<CStr>, std::allocator<CStr> >&) const;
        virtual void Update(float, unsigned int);
        virtual float GetDamageForOneShell() const;
        bool CanShotToTarget(int) const;
        virtual void GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int> >&) const;
        virtual bool PointIsReachableFromPosition(CVector const&, CVector const&, std::vector<int, std::allocator<int> >) const;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        unsigned int GetShellsInPool() const;
        bool IsWithCharging() const;
        float GetFiringRate() const;
        CMatrix GetMatrixForShot(unsigned int) const;
        float GetCurrentRechargingTime() const;
        unsigned int GetShellsPoolSize() const;
        virtual int OnEvent(Event const&);
        void SetTargetId(int);
        unsigned int GetChargeSize() const;
        float GetFiringRange() const;
        virtual float EstimateDamageFromPosition(CVector const&, CVector const&, std::vector<int, std::allocator<int> > const&);
        virtual bool PointIsReachable(CVector const&, std::vector<int, std::allocator<int> >) const;
        unsigned int GetShellsInCurrentCharge() const;
        virtual void LookAtPoint(CVector const&, float);
        void Recharge();
        virtual int GetPropertyId(char const*) const;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const;
        static void __fastcall Registration();
        virtual void SetPassedToAnotherMapStatus();
        float GetDamage() const;
        void SetInitialHorizAngle(float);
        int GetShellPrototypeId() const;
        ChargeState GetChargeState() const;
        float GetTurningSpeed() const;
        virtual bool SetPropertyById(int, m3d::AIParam const&);
        void SetShellsInCurrentCharge(unsigned int);
        DamageType GetDamageType() const;
        bool IsDurabilityEnoughForFiring() const;
        bool IsWithShellsPoolLimit() const;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        virtual bool CanFire() const;
        virtual unsigned int GetPrice(IPriceCoeffProvider const*) const;
        Gun(GunPrototypeInfo const&);
        float GetInitialHorizAngle() const;
        void SetChargeState(int);
        void SetChargeState(ChargeState);
        void SetHorizontalStopAngles(float, float);
        static m3d::Class* GetBaseClass();

    protected:
        virtual void DoRecoil();
        static void __fastcall RegisterProperty(char const*, int, eGObjPropertySaveStatus);
        bool _DoFire();
        bool _IsDirVerticallyReachable(CVector const&) const;
        virtual bool _GetPropertyDefaultInternal(int, m3d::AIParam&) const;
        virtual void _InternalCreateVisualPart();
        virtual bool _bIsUsingVolley() const;
        unsigned int GetBarrelsNum();
        virtual void _LaunchShells();
        CVector _CalcDirForNextShot() const;
        virtual void _InternalPostLoad();
        CVector _CalcRoughPosForNextShot() const;
        virtual bool _bIsVolleyFiring() const;
        virtual ~Gun();
        CVector _CalcPosForNextShot() const;
        virtual bool _GetPropertyInternal(int, m3d::AIParam&) const;

    private:
        void _CreateBarrelNode();
        void _OnCinematic(Event const&);
        static m3d::Object* CreateObject();
        void _GetCurrentOffsetAngles(float&, float&) const;
        CMatrix _CalcMatrixForNextShot() const;
        void BeginReCharge();
        unsigned int getShellsForRecharge() const;
        void _GetOffsetAngles(CVector const&, float, float&, float&);
        bool _bIsRapidFiring() const;
        void _UpdateNodeFiringAction();
        virtual m3d::Object* Clone();

    public:
        float m_leftStopAngle;
        float m_rightStopAngle;
        float m_lowStopAngle;
        float m_highStopAngle;
        int m_targetObjId;
        float m_damage;
        unsigned int m_curBarrelIndex;
        float m_currentDesiredAlpha;
        float m_timeFromLastShot;
        bool m_bIsFiring;
        int m_shellPrototypeId;
        DamageType m_damageType;
        float m_firingRate;
        float m_firingRange;
        float m_recoilForce;
        float m_turningSpeed;
        unsigned int m_ChargeSize;
        float m_ReChargingTime;
        float m_ReChargingTimePerShell;
        unsigned int m_ShellsInPool;
        Gun::ChargeState m_ChargeState;
        float m_CurrentReChargingTime;
        unsigned int m_ShellsInCurrentCharge;
        m3d::SgNode* m_barrelNode;
        bool m_bWasShot;
        bool m_bJustShot;
        float m_initialHorizAngle;
    };
}

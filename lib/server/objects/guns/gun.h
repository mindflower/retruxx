#pragma once
#include <server/damageinfo.h>
#include <server/objects/physicbodies/vehiclepart.h>

struct CMatrix;

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

    class GunPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        GunPrototypeInfo();
        CStr m_barrelModelName;
        int m_shellPrototypeId;
        float m_damage;
        ai::DamageType m_damageType;
        float m_firingRate;
        float m_firingRange;
        float m_lowStopAngle;
        float m_highStopAngle;
        bool m_ignoreStopAnglesWhenFire;
        unsigned short m_explosionType;
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
        ai::FiringTypes m_firingType;

        using TMatrixVector = retruxx::vector<CMatrix, retruxx::allocator<CMatrix>>;

    public:
        retruxx::vector<CMatrix, retruxx::allocator<CMatrix>> m_fireLpMatrices;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        void CreateBlastWave(CVector const& pos, int gunId) const;
        short GetExplosionType() const;
        int GetDecalId() const;
        virtual float GetDamageForOneShell() const;
        static ai::FiringTypes Str2FiringType(CStr const& firingTypeStr);
        static CStr FiringType2Str(ai::FiringTypes firingType);
        static ai::DamageType Str2DamageType(CStr const& damageTypeStr);
        static CStr DamageType2Str(ai::DamageType damageType);

    private:
        bool _bIsRapidFiring() const;
        CStr m_explosionTypeName;
        CStr m_shellPrototypeName;
        CStr m_blastWavePrototypeName;
    };

    class Gun : public ai::VehiclePart
    {
    protected:
        virtual ~Gun() override = 0;

    protected:
        Gun(ai::GunPrototypeInfo const& prototypeInfo);
        Gun(ai::Gun const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classGun;
        virtual ai::GunPrototypeInfo const* GetPrototypeInfo() const override;
        virtual int OnEvent(ai::Event const& evn) override;

    protected:
        static void RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        float m_leftStopAngle;
        float m_rightStopAngle;
        float m_lowStopAngle;
        float m_highStopAngle;

        enum ChargeState
        {
            csReady = 0,
            csInCharging = 1,
        };

    public:
        virtual void LookAtPoint(CVector const& lookAt, float elapsedTime);
        virtual bool isLookAtPoint(CVector const& lookAt, float eps) const;
        bool CanShotToTarget(int targetId) const;
        void SetTargetId(int targetObjId);
        virtual bool Fire(bool enable);
        void SetHorizontalStopAngles(float leftStopAngle, float rightStopAngle);
        m3d::SgNode* GetBarrelNode() const;
        virtual void SetPassedToAnotherMapStatus() override;
        static void Registration();
        ai::Gun::ChargeState GetChargeState() const;
        void SetChargeState(int Value);
        void SetChargeState(ai::Gun::ChargeState Value);
        unsigned int GetShellsInCurrentCharge() const;
        void SetShellsInCurrentCharge(unsigned int Value);
        unsigned int GetShellsInPool() const;
        void SetShellsInPool(unsigned int Shells);
        unsigned int GetShellsPoolSize() const;
        bool IsWithCharging() const;
        bool IsWithShellsPoolLimit() const;
        unsigned int GetChargeSize() const;
        float GetRechargingTime() const;
        float GetCurrentRechargingTime() const;
        CMatrix GetMatrixForShot(unsigned int barrelIndex) const;
        virtual float EstimateDamage(
            CVector const& pos,
            retruxx::vector<int, retruxx::allocator<int>> const& exceptions) const;
        virtual float EstimateDamage() const;
        virtual bool PointIsReachable(CVector const& pos, retruxx::vector<int, retruxx::allocator<int>> exceptions)
            const;
        virtual bool PointIsReachableFromPosition(
            CVector const& newPosition,
            CVector const& target,
            retruxx::vector<int, retruxx::allocator<int>> exceptions) const;
        virtual float EstimateDamageFromPosition(
            CVector const& position,
            CVector const& pos,
            retruxx::vector<int, retruxx::allocator<int>> const& exceptions);
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SetInvisible() override;
        int GetShellPrototypeId() const;
        float GetTurningSpeed() const;
        float GetDamage() const;
        virtual float GetDamageForOneShell() const;
        ai::DamageType GetDamageType() const;
        float GetFiringRate() const;
        float GetFiringRange() const;
        float GetInitialHorizAngle() const;
        void SetInitialHorizAngle(float angle);
        virtual unsigned int GetPrice(ai::IPriceCoeffProvider const* priceCoeffProvider) const override;
        void Recharge();
        virtual bool CanLookAtTarget() const;
        bool bIs360DegreesHoriz() const;
        bool IsDurabilityEnoughForFiring() const;
        virtual bool CanFire() const;
        void RenderGunDebugInfo() const;

    protected:
        int m_targetObjId;
        float m_damage;
        unsigned int m_curBarrelIndex;
        float m_currentDesiredAlpha;
        float m_timeFromLastShot;
        bool m_bIsFiring;
        CVector _CalcPosForNextShot() const;
        CVector _CalcRoughPosForNextShot() const;
        CVector _CalcDirForNextShot() const;
        bool _IsDirVerticallyReachable(CVector const& dir) const;
        virtual void DoRecoil();
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;
        bool _DoFire();
        virtual void _LaunchShells() = 0;
        unsigned int GetBarrelsNum();
        virtual bool _bIsUsingVolley() const;
        virtual bool _bIsVolleyFiring() const;

    private:
        int m_shellPrototypeId;
        ai::DamageType m_damageType;
        float m_firingRate;
        float m_firingRange;
        float m_recoilForce;
        float m_turningSpeed;
        unsigned int m_ChargeSize;
        float m_ReChargingTime;
        float m_ReChargingTimePerShell;
        unsigned int m_ShellsInPool;
        ai::Gun::ChargeState m_ChargeState;
        float m_CurrentReChargingTime;
        unsigned int m_ShellsInCurrentCharge;
        m3d::SgNode* m_barrelNode;
        bool m_bWasShot;
        bool m_bJustShot;
        float m_initialHorizAngle;
        CMatrix _CalcMatrixForNextShot() const;
        void _GetCurrentOffsetAngles(float& alpha, float& beta) const;
        void _GetOffsetAngles(CVector const& lookAt, float elapsedTime, float& alpha, float& beta);
        unsigned int getShellsForRecharge() const;
        bool _bIsRapidFiring() const;
        void _UpdateNodeFiringAction();
        void BeginReCharge();
        void _CreateBarrelNode();
        void _OnCinematic(ai::Event const& evn);
    };
}  // namespace ai

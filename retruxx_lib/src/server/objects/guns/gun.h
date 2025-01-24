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

    class GunPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        GunPrototypeInfo();
        /* 0x0110 */ CStr m_barrelModelName;
        /* 0x011c */ int m_shellPrototypeId;
        /* 0x0120 */ float m_damage;
        /* 0x0124 */ ai::DamageType m_damageType;
        /* 0x0128 */ float m_firingRate;
        /* 0x012c */ float m_firingRange;
        /* 0x0130 */ float m_lowStopAngle;
        /* 0x0134 */ float m_highStopAngle;
        /* 0x0138 */ bool m_ignoreStopAnglesWhenFire;
        /* 0x0139 */ char Padding_128;
        /* 0x013a */ unsigned short m_explosionType;
        /* 0x013c */ int m_decalId;
        /* 0x0140 */ float m_recoilForce;
        /* 0x0144 */ float m_turningSpeed;
        /* 0x0148 */ unsigned int m_ChargeSize;
        /* 0x014c */ float m_ReChargingTime;
        /* 0x0150 */ float m_ReChargingTimePerShell;
        /* 0x0154 */ unsigned int m_ShellsPoolSize;
        /* 0x0158 */ bool m_WithCharging;
        /* 0x0159 */ bool m_WithShellsPoolLimit;
        /* 0x015a */ char Padding_129[2];
        /* 0x015c */ int m_blastWavePrototypeId;
        /* 0x0160 */ ai::FiringTypes m_firingType;

        using TMatrixVector = retruxx::vector<CMatrix, retruxx::allocator<CMatrix> >;

    public:
        /* 0x0164 */ retruxx::vector<CMatrix, retruxx::allocator<CMatrix> > m_fireLpMatrices;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x0c */;
        void CreateBlastWave(const CVector& pos, int gunId) const;
        short GetExplosionType() const;
        int GetDecalId() const;
        virtual float GetDamageForOneShell() const /* 0x1c */;
        static ai::FiringTypes __fastcall Str2FiringType(const CStr& firingTypeStr);
        static CStr __fastcall FiringType2Str(ai::FiringTypes firingType);
        static ai::DamageType __fastcall Str2DamageType(const CStr& damageTypeStr);
        static CStr __fastcall DamageType2Str(ai::DamageType damageType);

    private:
        bool _bIsRapidFiring() const;
        /* 0x0174 */ CStr m_explosionTypeName;
        /* 0x0180 */ CStr m_shellPrototypeName;
        /* 0x018c */ CStr m_blastWavePrototypeName;
    }; /* size: 0x0198 */

    static_assert(sizeof(GunPrototypeInfo) == 0x0198);

    class Gun : public ai::VehiclePart
    {
    protected:
        virtual  ~Gun() override = 0 /* 0x00 */;

    private:
        Gun(const ai::GunPrototypeInfo& prototypeInfo);
        Gun(const ai::Gun&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classGun;
        virtual const ai::GunPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        /* 0x02c8 */ float m_leftStopAngle;
        /* 0x02cc */ float m_rightStopAngle;
        /* 0x02d0 */ float m_lowStopAngle;
        /* 0x02d4 */ float m_highStopAngle;

        enum ChargeState;

    public:
        virtual void LookAtPoint(const CVector& lookAt, float elapsedTime) /* 0x168 */;
        virtual bool isLookAtPoint(const CVector& lookAt, float eps) const /* 0x16c */;
        bool CanShotToTarget(int targetId) const;
        void SetTargetId(int targetObjId);
        virtual bool Fire(bool enable) /* 0x170 */;
        void SetHorizontalStopAngles(float leftStopAngle, float rightStopAngle);
        m3d::SgNode* GetBarrelNode() const;
        virtual void SetPassedToAnotherMapStatus() override /* 0x00 */;
        static void __fastcall Registration();
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
        virtual float EstimateDamage(const CVector& pos, const retruxx::vector<int, retruxx::allocator<int> >& exceptions) const /* 0x178 */;
        virtual float EstimateDamage() const /* 0x178 */;
        virtual bool PointIsReachable(const CVector& pos, retruxx::vector<int, retruxx::allocator<int> > exceptions) const /* 0x17c */;
        virtual bool PointIsReachableFromPosition(const CVector& newPosition, const CVector& target, retruxx::vector<int, retruxx::allocator<int> > exceptions) const /* 0x180 */;
        virtual float EstimateDamageFromPosition(const CVector& position, const CVector& pos, const retruxx::vector<int, retruxx::allocator<int> >& exceptions) /* 0x184 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SetInvisible() override /* 0x00 */;
        int GetShellPrototypeId() const;
        float GetTurningSpeed() const;
        float GetDamage() const;
        virtual float GetDamageForOneShell() const /* 0x188 */;
        ai::DamageType GetDamageType() const;
        float GetFiringRate() const;
        float GetFiringRange() const;
        float GetInitialHorizAngle() const;
        void SetInitialHorizAngle(float angle);
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override /* 0x00 */;
        void Recharge();
        virtual bool CanLookAtTarget() const /* 0x18c */;
        bool bIs360DegreesHoriz() const;
        bool IsDurabilityEnoughForFiring() const;
        virtual bool CanFire() const /* 0x190 */;
        void RenderGunDebugInfo() const;

    protected:
        /* 0x02d8 */ int m_targetObjId;
        /* 0x02dc */ float m_damage;
        /* 0x02e0 */ unsigned int m_curBarrelIndex;
        /* 0x02e4 */ float m_currentDesiredAlpha;
        /* 0x02e8 */ float m_timeFromLastShot;
        /* 0x02ec */ bool m_bIsFiring;
        CVector _CalcPosForNextShot() const;
        CVector _CalcRoughPosForNextShot() const;
        CVector _CalcDirForNextShot() const;
        bool _IsDirVerticallyReachable(const CVector& dir) const;
        virtual void DoRecoil() /* 0x194 */;
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x00 */;
        bool _DoFire();
        virtual void _LaunchShells() = 0 /* 0x198 */;
        unsigned int GetBarrelsNum();
        virtual bool _bIsUsingVolley() const /* 0x19c */;
        virtual bool _bIsVolleyFiring() const /* 0x1a0 */;
        /* 0x02ed */ char Padding_35[3];

    private:
        /* 0x02f0 */ int m_shellPrototypeId;
        /* 0x02f4 */ ai::DamageType m_damageType;
        /* 0x02f8 */ float m_firingRate;
        /* 0x02fc */ float m_firingRange;
        /* 0x0300 */ float m_recoilForce;
        /* 0x0304 */ float m_turningSpeed;
        /* 0x0308 */ unsigned int m_ChargeSize;
        /* 0x030c */ float m_ReChargingTime;
        /* 0x0310 */ float m_ReChargingTimePerShell;
        /* 0x0314 */ unsigned int m_ShellsInPool;
        /* 0x0318 */ ai::Gun::ChargeState m_ChargeState;
        /* 0x031c */ float m_CurrentReChargingTime;
        /* 0x0320 */ unsigned int m_ShellsInCurrentCharge;
        /* 0x0324 */ m3d::SgNode* m_barrelNode;
        /* 0x0328 */ bool m_bWasShot;
        /* 0x0329 */ bool m_bJustShot;
        /* 0x032a */ char Padding_36[2];
        /* 0x032c */ float m_initialHorizAngle;
        CMatrix _CalcMatrixForNextShot() const;
        void _GetCurrentOffsetAngles(float& alpha, float& beta) const;
        void _GetOffsetAngles(const CVector& lookAt, float elapsedTime, float& alpha, float& beta);
        unsigned int getShellsForRecharge() const;
        bool _bIsRapidFiring() const;
        void _UpdateNodeFiringAction();
        void BeginReCharge();
        void _CreateBarrelNode();
        void _OnCinematic(const ai::Event& evn);
    }; /* size: 0x0330 */

    static_assert(sizeof(Gun) == 0x0330);
}

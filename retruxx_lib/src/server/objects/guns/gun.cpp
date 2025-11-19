#include "gun.h"

#include "core/log.h"

#include <stdexcept>

#include "math/matrix.h"
#include <server/objects/base/prototypemanager.h>

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInCurrentCharge)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInCurrentCharge)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInPool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInPool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetChargeState)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetChargeState)
{
    RETRUXX_NOT_IMPLEMENTED;
}

namespace ai
{
    namespace
    {
        const float DEFAULT_TURNING_SPEED = 180.0;

        struct _FiringType2Str
        {
            /* 0x0000 */ FiringTypes m_type;
            /* 0x0004 */ CStr m_name;
        }; /* size: 0x0010 */

        const _FiringType2Str l_firingType2Str[] = {
            {FT_MACHINE_GUN, "MachineGun"},
            {FT_CANNON, "Cannon"},
            {FT_SHOT_GUN, "ShotGun"},
            {FT_LASER, "Laser"},
            {FT_PLASMA, "Plasma"},
            {FT_ROCKET, "Rocket"},
            {FT_ARTILLERY, "Artillery"},
            {FT_THUNDERBOLT, "Thunderbolt"},
            {FT_MINE, "Mine"},
            {FT_NAIL, "Nail"},
            {FT_TURBO, "Turbo"},
            {FT_OIL, "Oil"},
            {FT_SMOKE, "Type_Smoke"},
        };

        struct _DamageType2Str
        {
            /* 0x0000 */ DamageType m_type;
            /* 0x0004 */ CStr m_name;
        }; /* size: 0x0010 */

        const _DamageType2Str l_damageType2Str[] = {
            {DAMAGE_PIERCING, "PIERCING"},
            {DAMAGE_BLAST, "BLAST"},
            {DAMAGE_ENERGY, "ENERGY"},
            {DAMAGE_WATER, "WATER"},
        };
    }

    RT_CLASS_EXPORTS_BEGIN(Gun)
        RT_CLASS_EXPORT(Gun, m3d::METHOD, GetShellsInCurrentCharge, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, SetShellsInCurrentCharge, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, GetShellsInPool, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, SetShellsInPool, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, GetChargeState, "", "", "")
        RT_CLASS_EXPORT(Gun, m3d::METHOD, SetChargeState, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Gun);

    void GunPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GunPrototypeInfo::DamageType2Str(DamageType)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GunPrototypeInfo::GunPrototypeInfo()
    {
        this->m_WithCharging = 1;
        this->m_WithShellsPoolLimit = 1;
        this->m_shellPrototypeId = -1;
        this->m_damage = 1.0;
        this->m_damageType = DAMAGE_PIERCING;
        this->m_firingRate = 1.0;
        this->m_firingRange = 1.0;
        this->m_lowStopAngle = 0.0;
        this->m_highStopAngle = 0.0;
        this->m_ignoreStopAnglesWhenFire = 0;
        this->m_decalId = -1;
        this->m_recoilForce = 0.0;
        this->m_turningSpeed = DEFAULT_TURNING_SPEED;
        this->m_ChargeSize = 20;
        this->m_ReChargingTime = 1.0;
        this->m_ReChargingTimePerShell = 0.0;
        this->m_ShellsPoolSize = 12;
        this->m_blastWavePrototypeId = -1;
        this->m_firingType = FT_MACHINE_GUN;
        this->m_explosionTypeName = "BIG";
    }

    void GunPrototypeInfo::CreateBlastWave(CVector const&, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr GunPrototypeInfo::FiringType2Str(FiringTypes)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    FiringTypes GunPrototypeInfo::Str2FiringType(const CStr& firingTypeStr)
    {
        for (const auto& firingType : l_firingType2Str)
        {
            if (firingType.m_name == firingTypeStr)
            {
                return firingType.m_type;
            }
        }
        return FT_NUM_FIRING_TYPES;
    }

    DamageType GunPrototypeInfo::Str2DamageType(const CStr& damageTypeStr)
    {
        for (const auto& damageType : l_damageType2Str)
        {
            if (damageType.m_name == damageTypeStr)
            {
                return damageType.m_type;
            }
        }
        return DAMAGE_NUM_TYPES;
    }

    bool GunPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        const auto res = VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (res)
        {
            m3d::SafeStrAttrib(m_shellPrototypeName, xmlNode, "BulletPrototype");
            m3d::SafeStrAttrib(m_blastWavePrototypeName, xmlNode, "BlastWavePrototype");
            m3d::SafeFloatAttrib(m_damage, xmlNode, "Damage");
            m3d::SafeFloatAttrib(m_firingRate, xmlNode, "FiringRate");
            m3d::SafeFloatAttrib(m_firingRange, xmlNode, "FiringRange");
            m3d::SafeStrAttrib(m_explosionTypeName, xmlNode, "ExplosionType");
            m3d::SafeFloatAttrib(m_recoilForce, xmlNode, "RecoilForce");

            CStr decal;
            m3d::SafeStrAttrib(decal, xmlNode, "Decal");
            m_decalId = gDynamicScene->AddDecalName(decal);

            CStr firingTypeName;
            m3d::SafeStrAttrib(firingTypeName, xmlNode, "FiringType");
            m_firingType = Str2FiringType(firingTypeName);
            if (m_firingType == FT_NUM_FIRING_TYPES)
            {
                M3D_LOG_INFO("Warning: Unknown firing type: '" + CStr(m_firingType) + "' for " + GetDebugDescription());
            }

            CStr damageType;
            m3d::SafeStrAttrib(damageType, xmlNode, "DamageType");
            if (!damageType.empty())
            {
                m_damageType = Str2DamageType(damageType);
            }
            if (m_damageType == DAMAGE_NUM_TYPES)
            {
                M3D_LOG_INFO("Warning: Unknown damage type: '" + CStr(m_damageType) + "' for " + GetDebugDescription());
            }

            m3d::SafeBoolAttrib(m_WithCharging, xmlNode, "WithCharging");
            m3d::SafeUintAttrib(m_ChargeSize, xmlNode, "ChargeSize");
            m3d::SafeFloatAttrib(m_ReChargingTime, xmlNode, "RechargingTime");
            m3d::SafeFloatAttrib(m_ReChargingTimePerShell, xmlNode, "ReChargingTimePerShell");
            m3d::SafeUintAttrib(m_ShellsPoolSize, xmlNode, "ShellsPoolSize");
            m3d::SafeBoolAttrib(m_WithShellsPoolLimit, xmlNode, "WithShellsPoolLimit");

            m3d::SafeFloatAttrib(m_turningSpeed, xmlNode, "TurningSpeed");
            m_turningSpeed = m_turningSpeed * 0.017453292;

            m_barrelModelName = m_engineModelName + "Gun";
            
            m3d::SafeBoolAttrib(m_ignoreStopAnglesWhenFire, xmlNode, "IgnoreStopAnglesWhenFire");
        }
        return res;
    }

    float GunPrototypeInfo::GetDamageForOneShell() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void GunPrototypeInfo::PostLoad()
    {
        m_explosionType = gDynamicScene->GetExplosionType(m_explosionTypeName);
        m_shellPrototypeId = thePrototypeManager->GetPrototypeId(m_shellPrototypeName);
        if (m_shellPrototypeId == -1 && !m_shellPrototypeName.empty())
        {
            M3D_CRITICAL_ERROR("shell prototype '" + m_shellPrototypeName + "' is invalid for '" + GetDebugDescription());
        }

        if (!m_blastWavePrototypeName.empty())
        {
            m_blastWavePrototypeId = thePrototypeManager->GetPrototypeId(m_blastWavePrototypeName);
            if (m_blastWavePrototypeId == -1)
            {
                M3D_LOG_INFO("Unknown blastwave prototype " + m_blastWavePrototypeName);
            }
        }
    }

    short GunPrototypeInfo::GetExplosionType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool GunPrototypeInfo::_bIsRapidFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::SgNode* Gun::GetBarrelNode() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::Fire(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    GunPrototypeInfo const* Gun::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetShellsInPool(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::RenderGunDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::EstimateDamage(CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::EstimateDamage() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::bIs360DegreesHoriz() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Gun::GetPropertyName(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::CanLookAtTarget() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gun::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::isLookAtPoint(CVector const&, float) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetInvisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::Update(float, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetDamageForOneShell() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::CanShotToTarget(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::PointIsReachableFromPosition(CVector const&, CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetShellsInPool() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::IsWithCharging() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetFiringRate() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CMatrix Gun::GetMatrixForShot(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetCurrentRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetShellsPoolSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gun::OnEvent(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetTargetId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetChargeSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetFiringRange() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::EstimateDamageFromPosition(CVector const&, CVector const&, retruxx::vector<int, retruxx::allocator<int>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::PointIsReachable(CVector const&, retruxx::vector<int, retruxx::allocator<int>>) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetShellsInCurrentCharge() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::LookAtPoint(CVector const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::Recharge()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gun::GetPropertyId(char const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eGObjPropertySaveStatus Gun::GetPropertySaveStatus(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::Registration()
    {
        m_propertiesMap["Damage"] = 28;
        m_propertiesMap["FiringRate"] = 29;
        m_propertiesMap["FiringRange"] = 30;
        m_propertiesMap["ChargeSize"] = 34;
        m_propertiesMap["ReChargingTime"] = 35;
        m_propertiesMap["ShellsInPool"] = 36;
    }

    void Gun::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetDamage() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetInitialHorizAngle(float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gun::GetShellPrototypeId() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::ChargeState Gun::GetChargeState() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetTurningSpeed() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::SetPropertyById(int, m3d::AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetShellsInCurrentCharge(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DamageType Gun::GetDamageType() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::IsDurabilityEnoughForFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::IsWithShellsPoolLimit() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::CanFire() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetPrice(IPriceCoeffProvider const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::Gun(GunPrototypeInfo const& prototype) : VehiclePart(prototype)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Gun::GetInitialHorizAngle() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetChargeState(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetChargeState(ChargeState)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::SetHorizontalStopAngles(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gun::GetBaseClass()
    {
        return RT_CLASS_LOCAL(VehiclePart);
    }

    void Gun::DoRecoil()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_DoFire()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_IsDirVerticallyReachable(CVector const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_bIsUsingVolley() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetBarrelsNum()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_LaunchShells()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Gun::_CalcDirForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_InternalPostLoad()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Gun::_CalcRoughPosForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_bIsVolleyFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::~Gun()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector Gun::_CalcPosForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_GetPropertyInternal(int, m3d::AIParam&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_CreateBarrelNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_OnCinematic(Event const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gun::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_GetCurrentOffsetAngles(float&, float&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CMatrix Gun::_CalcMatrixForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::BeginReCharge()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::getShellsForRecharge() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_GetOffsetAngles(CVector const&, float, float&, float&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_bIsRapidFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_UpdateNodeFiringAction()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Gun::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

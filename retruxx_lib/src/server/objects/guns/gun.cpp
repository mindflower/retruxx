#include "gun.h"

#include "compoundgun.h"
#include "m3dapp.h"
#include "core/log.h"

#include <stdexcept>

#include "math/matrix.h"
#include "scene/servers/dataserver.h"
#include "scene/servers/serveranimatedmodel.h"

#include <server/objects/base/prototypemanager.h>
#include <server/objects/player.h>
#include <server/processmanager.h>

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

        CStr GetFireLp(int i)
        {
            CStr res = i >= 10 ? "LP_FIRE" : "LP_FIRE0";
            res += CStr(i);
            return res;
        }
    }  // namespace

    RT_CLASS_EXPORTS_BEGIN(Gun)
    RT_CLASS_EXPORT(Gun, m3d::METHOD, GetShellsInCurrentCharge, "", "", "")
    RT_CLASS_EXPORT(Gun, m3d::METHOD, SetShellsInCurrentCharge, "", "", "")
    RT_CLASS_EXPORT(Gun, m3d::METHOD, GetShellsInPool, "", "", "")
    RT_CLASS_EXPORT(Gun, m3d::METHOD, SetShellsInPool, "", "", "")
    RT_CLASS_EXPORT(Gun, m3d::METHOD, GetChargeState, "", "", "")
    RT_CLASS_EXPORT(Gun, m3d::METHOD, SetChargeState, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Gun);

    void GunPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        VehiclePartPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);

        m_lowStopAngle = 0.0;
        m_highStopAngle = 0.0;

        auto* animatedModelsServer = reinterpret_cast<m3d::AnimatedModelsServer*>(&M3D_APP->GetAnimatedModelsServer());
        const auto item = animatedModelsServer->GetItemByName(m_engineModelName.c_str(), true);
        m3d::AnimatedModel* model = nullptr;
        if (item != -1)
        {
            animatedModelsServer->GetItemProperty(item, m3d::PROP_INTERNAL_GETMODEL, &model);

            const auto loadPointId = model->GetLoadPointIdByName("LP_GUN");
            const auto& loadPoint = model->GetBoneBounds(loadPointId);
            m_lowStopAngle = 0.0 - loadPoint.MaxRot.x;
            m_highStopAngle = 0.0 - loadPoint.MinRot.x;
        }

        if (m3d::SafeFloatAttrib(m_lowStopAngle, xmlNode, "LowStop"))
        {
            m_lowStopAngle = m_lowStopAngle * 0.017453292;
        }

        if (m3d::SafeFloatAttrib(m_highStopAngle, xmlNode, "HighStop"))
        {
            m_highStopAngle = m_highStopAngle * 0.017453292;
        }

        if (m_lowStopAngle < -3.1415927)
        {
            m_lowStopAngle = -1.0461504;
        }
        if (m_highStopAngle > 3.1415927)
        {
            m_highStopAngle = 1.0461504;
        }

        m_fireLpMatrices.clear();

        int i = 1;
        while (true)
        {
            CMatrix boneMatrix;
            const auto res = animatedModelsServer->GetBoneMatrixByNameFromModelName(m_barrelModelName.c_str(), GetFireLp(i), boneMatrix, false);
            if (!res)
            {
                break;
            }

            m_fireLpMatrices.push_back(std::move(boneMatrix));
            ++i;
        }

        if (m_fireLpMatrices.empty())
        {
            CMatrix mat;
            mat.identity();
            m_fireLpMatrices.push_back(std::move(mat));
        }

        if (m_firingRate <= 299.89999)
        {
            if (model)
            {
                model->SetNextForAnimation(AT_MOVE1, 0);
                const auto barrelItem = animatedModelsServer->GetItemByName(m_barrelModelName.c_str(), true);
                if (barrelItem != -1)
                {
                    m3d::AnimatedModel* barrelModel = nullptr;
                    animatedModelsServer->GetItemProperty(barrelItem, m3d::PROP_INTERNAL_GETMODEL, &barrelModel);
                    if (barrelModel)
                    {
                        barrelModel->SetNextForAnimation(AT_MOVE1, 0);
                    }
                }
            }
        }
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

    bool Gun::Fire(bool enable)
    {
        bool doFire = enable;
        if (enable && !CanFire())
        {
            doFire = false;
        }
        m_bIsFiring = doFire;
        return doFire && _DoFire();
    }

    GunPrototypeInfo const* Gun::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), const GunPrototypeInfo);
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
        return m_rightStopAngle - m_leftStopAngle > 6.283184482025146 ||
            (m_rightStopAngle + 0.1) > m_currentDesiredAlpha && m_currentDesiredAlpha > (m_leftStopAngle - 0.1);
    }

    float Gun::GetRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Gun::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::isLookAtPoint(const CVector& lookAt, float eps) const
    {
        // TODO: generated code Gun::isLookAtPoint
        CVector gunPosition;

        // Get the gun's world position from either barrel node, main node, or physics body
        if (m_barrelNode != nullptr)
        {
            gunPosition = m_barrelNode->GetOriginWorldAbs();
        }
        else if (m_Node != nullptr)
        {
            gunPosition = m_Node->GetOriginWorldAbs();
        }
        else
        {
            gunPosition = GetPosition();
        }

        // Calculate direction vector from gun to target
        CVector targetDir;
        targetDir.x = lookAt.x - gunPosition.x;
        targetDir.y = lookAt.y - gunPosition.y;
        targetDir.z = lookAt.z - gunPosition.z;

        // Normalize the target direction vector
        float invTargetLength = 1.0f / std::sqrt(targetDir.x * targetDir.x + targetDir.y * targetDir.y + targetDir.z * targetDir.z + 1.1920929e-7f);
        CVector normalizedTargetDir;
        normalizedTargetDir.x = targetDir.x * invTargetLength;
        normalizedTargetDir.y = targetDir.y * invTargetLength;
        normalizedTargetDir.z = targetDir.z * invTargetLength;

        // Get the gun's transformation matrix for the current barrel
        CMatrix gunMatrix = GetMatrixForShot(m_curBarrelIndex);

        // Calculate the gun's forward direction from the matrix
        // The forward vector appears to be calculated as a combination of matrix columns
        CVector gunForwardDir;
        gunForwardDir.x = gunMatrix._21;  // Combination of _31 + _11 * 0.0 + _21 = _21
        gunForwardDir.y = gunMatrix._22;  // Combination of _32 + _12 * 0.0 + _22 = _22
        gunForwardDir.z = gunMatrix._23;  // Combination of _33 + _13 * 0.0 + _23 = _23

        // Normalize the gun's forward direction vector
        float invGunLength =
            1.0f / std::sqrt(gunForwardDir.x * gunForwardDir.x + gunForwardDir.y * gunForwardDir.y + gunForwardDir.z * gunForwardDir.z + 1.1920929e-7f);
        CVector normalizedGunForwardDir;
        normalizedGunForwardDir.x = gunForwardDir.x * invGunLength;
        normalizedGunForwardDir.y = gunForwardDir.y * invGunLength;
        normalizedGunForwardDir.z = gunForwardDir.z * invGunLength;

        // Calculate the cross product between gun direction and target direction
        // This gives us the "error" vector - its magnitude indicates how misaligned we are
        CVector crossProduct;
        crossProduct.x = normalizedGunForwardDir.y * normalizedTargetDir.z - normalizedGunForwardDir.z * normalizedTargetDir.y;
        crossProduct.y = normalizedGunForwardDir.z * normalizedTargetDir.x - normalizedGunForwardDir.x * normalizedTargetDir.z;
        crossProduct.z = normalizedGunForwardDir.x * normalizedTargetDir.y - normalizedGunForwardDir.y * normalizedTargetDir.x;

        // Calculate the magnitude of the cross product (alignment error)
        float alignmentError = std::sqrt(crossProduct.x * crossProduct.x + crossProduct.y * crossProduct.y + crossProduct.z * crossProduct.z);

        // Return true if the alignment error is within the epsilon tolerance
        // This means the gun is pointing close enough to the target
        return eps > std::fabs(alignmentError);
    }

    void Gun::SetInvisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::Update(float elapsedTime, unsigned workTime)
    {
        VehiclePart::Update(elapsedTime, workTime);
        if (elapsedTime >= 0.001)
        {
            m_timeFromLastShot = m_timeFromLastShot + elapsedTime;
            _UpdateNodeFiringAction();
            const auto* protoInfo = GetPrototypeInfo();
            if (m_ChargeState == csInCharging)
            {
                m_CurrentReChargingTime = m_CurrentReChargingTime + elapsedTime;
                if (m_CurrentReChargingTime > GetRechargingTime())
                {
                    if (protoInfo->m_WithShellsPoolLimit)
                    {
                        if (m_ShellsInPool)
                        {
                            const auto shellsForRecharge = getShellsForRecharge();
                            m_ShellsInPool -= shellsForRecharge;
                            m_ShellsInCurrentCharge += shellsForRecharge;
                            m_ChargeState = csReady;
                            return;
                        }
                    }
                    else
                    {
                        m_ShellsInCurrentCharge = m_ChargeSize;
                    }
                    m_ChargeState = csReady;
                }
            }
        }
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
        // TODO: generated code Gun::GetMatrixForShot
        // Get the prototype information for this gun
        const ai::GunPrototypeInfo* prototypeInfo = this->GetPrototypeInfo();

        // Create the base fire matrix from prototype data
        CMatrix fireMat = prototypeInfo->m_fireLpMatrices[m_curBarrelIndex];

        // If we have a barrel node, transform the fire matrix by the barrel's current world transform
        if (m_barrelNode != nullptr)
        {
            const CMatrix& barrelXForm = m_barrelNode->GetCurrentMatrix();

            // Store original matrix values before transformation
            float orig_11 = fireMat._11;
            float orig_12 = fireMat._12;
            float orig_13 = fireMat._13;
            float orig_14 = fireMat._14;
            float orig_21 = fireMat._21;
            float orig_22 = fireMat._22;
            float orig_23 = fireMat._23;
            float orig_24 = fireMat._24;
            float orig_31 = fireMat._31;
            float orig_32 = fireMat._32;
            float orig_33 = fireMat._33;
            float orig_34 = fireMat._34;
            float orig_41 = fireMat._41;
            float orig_42 = fireMat._42;
            float orig_43 = fireMat._43;
            float orig_44 = fireMat._44;

            // Transform first row [11, 12, 13, 14]
            fireMat._11 = barrelXForm._11 * orig_11 + barrelXForm._21 * orig_12 + barrelXForm._31 * orig_13 + barrelXForm._41 * orig_14;
            fireMat._12 = barrelXForm._12 * orig_11 + barrelXForm._22 * orig_12 + barrelXForm._32 * orig_13 + barrelXForm._42 * orig_14;
            fireMat._13 = barrelXForm._13 * orig_11 + barrelXForm._23 * orig_12 + barrelXForm._33 * orig_13 + barrelXForm._43 * orig_14;
            fireMat._14 = barrelXForm._14 * orig_11 + barrelXForm._24 * orig_12 + barrelXForm._34 * orig_13 + barrelXForm._44 * orig_14;

            // Transform second row [21, 22, 23, 24]
            fireMat._21 = barrelXForm._11 * orig_21 + barrelXForm._21 * orig_22 + barrelXForm._31 * orig_23 + barrelXForm._41 * orig_24;
            fireMat._22 = barrelXForm._12 * orig_21 + barrelXForm._22 * orig_22 + barrelXForm._32 * orig_23 + barrelXForm._42 * orig_24;
            fireMat._23 = barrelXForm._13 * orig_21 + barrelXForm._23 * orig_22 + barrelXForm._33 * orig_23 + barrelXForm._43 * orig_24;
            fireMat._24 = barrelXForm._14 * orig_21 + barrelXForm._24 * orig_22 + barrelXForm._34 * orig_23 + barrelXForm._44 * orig_24;

            // Transform third row [31, 32, 33, 34]
            fireMat._31 = barrelXForm._11 * orig_31 + barrelXForm._21 * orig_32 + barrelXForm._31 * orig_33 + barrelXForm._41 * orig_34;
            fireMat._32 = barrelXForm._12 * orig_31 + barrelXForm._22 * orig_32 + barrelXForm._32 * orig_33 + barrelXForm._42 * orig_34;
            fireMat._33 = barrelXForm._13 * orig_31 + barrelXForm._23 * orig_32 + barrelXForm._33 * orig_33 + barrelXForm._43 * orig_34;
            fireMat._34 = barrelXForm._14 * orig_31 + barrelXForm._24 * orig_32 + barrelXForm._34 * orig_33 + barrelXForm._44 * orig_34;

            // Transform fourth row [41, 42, 43, 44]
            fireMat._41 = barrelXForm._11 * orig_41 + barrelXForm._21 * orig_42 + barrelXForm._31 * orig_43 + barrelXForm._41 * orig_44;
            fireMat._42 = barrelXForm._12 * orig_41 + barrelXForm._22 * orig_42 + barrelXForm._32 * orig_43 + barrelXForm._42 * orig_44;
            fireMat._43 = barrelXForm._13 * orig_41 + barrelXForm._23 * orig_42 + barrelXForm._33 * orig_43 + barrelXForm._43 * orig_44;
            fireMat._44 = barrelXForm._14 * orig_41 + barrelXForm._24 * orig_42 + barrelXForm._34 * orig_43 + barrelXForm._44 * orig_44;
        }

        // Copy the final matrix to the result
        return fireMat;
    }

    float Gun::GetCurrentRechargingTime() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned Gun::GetShellsPoolSize() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Gun::OnEvent(Event const& evn)
    {
        const auto result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_CINEMATIC_ENTER_FADE_IN)
        {
            if (m_Node)
            {
                int action = 0;
                m_Node->GetProperty(m3d::PROP_DM_ACTION, &action);
                if (action)
                {
                    SetNodeAction(0, 1);
                    m_bIsFiring = false;
                }
            }
        }
        return result;
    }

    void Gun::SetTargetId(int targetObjId)
    {
        m_targetObjId = targetObjId;
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

    void Gun::LookAtPoint(CVector const& lookAt, float elapsedTime)
    {
        float alpha = 0.0;
        float beta = 0.0;
        _GetOffsetAngles(lookAt, elapsedTime, alpha, beta);

        auto v4 = alpha * 0.5;

        Quaternion quatHorizRotation;
        quatHorizRotation.x = 0.0;
        quatHorizRotation.z = 0.0;
        quatHorizRotation.y = sin(v4);
        quatHorizRotation.w = cos(v4);
        SetNodeRelativeRotation(quatHorizRotation);

        auto v5 = (0.0 - beta) * 0.5;

        Quaternion quatElevation;
        quatElevation.y = 0.0;
        quatElevation.z = 0.0;
        quatElevation.x = sin(v5);
        quatElevation.w = cos(v5);
        if (m_barrelNode)
        {
            m_barrelNode->SetRotation(quatElevation);
        }
        ai::CommonGeomMovedCallback(m_pGeoms[0]->GetGeomId());
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

    void Gun::SetInitialHorizAngle(float angle)
    {
        m_initialHorizAngle = angle;
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
        auto* ownerCompoundVehiclePart = GetOwnerCompoundVehiclePart();
        if (ownerCompoundVehiclePart && IS_KIND_OF(ownerCompoundVehiclePart, CompoundGun))
        {
            auto* gun = RT_DYNCAST(ownerCompoundVehiclePart, const CompoundGun);
            return gun->IsDurabilityEnoughForFiring();
        }
        return Durability().value().get() > 0;
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
        return m_ChargeState == csReady && m_ShellsInCurrentCharge && (GetPrototypeInfo()->m_ignoreStopAnglesWhenFire || CanLookAtTarget()) &&
            IsDurabilityEnoughForFiring();
    }

    unsigned Gun::GetPrice(IPriceCoeffProvider const*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::Gun(GunPrototypeInfo const& prototypeInfo) : VehiclePart(prototypeInfo)
    {
        m_lowStopAngle = prototypeInfo.m_lowStopAngle;
        m_highStopAngle = prototypeInfo.m_highStopAngle;
        m_damage = prototypeInfo.m_damage;
        m_shellPrototypeId = prototypeInfo.m_shellPrototypeId;
        m_damageType = prototypeInfo.m_damageType;
        m_firingRate = prototypeInfo.m_firingRate;
        m_firingRange = prototypeInfo.m_firingRange;
        m_recoilForce = prototypeInfo.m_recoilForce;
        m_turningSpeed = prototypeInfo.m_turningSpeed;
        m_ChargeSize = prototypeInfo.m_ChargeSize;
        m_ReChargingTime = prototypeInfo.m_ReChargingTime;
        m_ReChargingTimePerShell = prototypeInfo.m_ReChargingTimePerShell;
        m_ShellsInPool = prototypeInfo.m_ShellsPoolSize;
        m_currentDesiredAlpha = 1000000.0;
        m_curBarrelIndex = 0;
        m_bIsFiring = 0;
        m_ChargeState = csReady;
        m_barrelNode = 0;
        m_bWasShot = 0;
        m_bJustShot = 0;
        m_leftStopAngle = 0.0;
        m_rightStopAngle = 0.0;
        m_targetObjId = -1;
        m_timeFromLastShot = 1000.0;
        m_CurrentReChargingTime = 0.0;
        m_ShellsInCurrentCharge = m_ChargeSize;
        m_initialHorizAngle = 0.0;
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

    void Gun::SetHorizontalStopAngles(float leftStopAngle, float rightStopAngle)
    {
        auto v3 = leftStopAngle;
        if (leftStopAngle >= -3.1425927)
        {
            if (leftStopAngle > 0.0)
                v3 = 0.0;
        }
        else
        {
            v3 = -3.1425927;
        }
        this->m_leftStopAngle = v3;

        auto v4 = rightStopAngle;
        if (rightStopAngle >= 0.0)
        {
            if (rightStopAngle > 3.1425927)
                v4 = 3.1425927;
            this->m_rightStopAngle = v4;
        }
        else
        {
            this->m_rightStopAngle = 0.0;
        }
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
        if ((60.0 / m_firingRate) > m_timeFromLastShot)
        {
            return false;
        }

        if (m_ShellsInCurrentCharge-- == 1)
        {
            BeginReCharge();
        }
        _LaunchShells();

        m_timeFromLastShot = 0.0;
        m_bWasShot = true;
        m_bJustShot = true;

        const auto* protoInfo = GetPrototypeInfo();
        ++m_curBarrelIndex;

        // TODO: check this
        int barrelCount = protoInfo->m_fireLpMatrices.size();
        if (m_curBarrelIndex == barrelCount)
        {
            m_curBarrelIndex = 0;
        }
        DoRecoil();
        return true;
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
        VehiclePart::_InternalCreateVisualPart();
        _CreateBarrelNode();
    }

    bool Gun::_bIsUsingVolley() const
    {
        return false;
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
        if (thePlayer)
        {
            // TODO: check IE_EV_SM_OBJECT_CREATED
            theProcessManager->PostMessageA(GE_SUBSCRIBE, thePlayer->GetId(), GetId(), 0.0, IE_EV_SM_OBJECT_CREATED, {}, 1);
        }
    }

    CVector Gun::_CalcRoughPosForNextShot() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Gun::_bIsVolleyFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Gun::~Gun() = default;

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
        // TODO: implement Gun::_CreateBarrelNode
        if (!m_modelname.empty())
        {
            const auto* prototypeInfo = GetPrototypeInfo();
            CVector scale(1.0, 1.0, 1.0);

            m_barrelNode = CreateNode(prototypeInfo->m_barrelModelName.c_str(), 0, scale, this, false);
            if (!m_barrelNode)
            {
                M3D_CRITICAL_ERROR("Couldn't create barrel for " + GetDebugDescription());
            }

            m_Node->AddChild(m_barrelNode);

            auto* serverAnimatedModels = static_cast<m3d::AnimatedModelsServer*>(&M3D_APP->GetAnimatedModelsServer());

            CMatrix boneMat;
            const auto boneRes = serverAnimatedModels->GetBoneMatrixByNameFromModelName(m_modelname.c_str(), "LP_GUN", boneMat, false);
            if (!boneRes)
            {
                M3D_LOG_ERR("Error: LoadPoint not found: LP_GUN for " + GetDebugDescription());
                boneMat.identity();
            }

            Quaternion rot;
            rot.FromMatrix(boneMat);
            m_barrelNode->SetRotation(rot);

            const CVector origin = boneMat.getOrg();
            m_barrelNode->SetOriginAbs(origin);

            m_barrelNode->UpdateXForm(false, true);
            m_barrelNode->SetName(m_Node->GetName() + CStr("Brl"));

            if (fabs(prototypeInfo->m_highStopAngle - prototypeInfo->m_lowStopAngle) < 0.0099999998)
            {
                RETRUXX_NOT_IMPLEMENTED;
            }
        }
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

    void Gun::_GetOffsetAngles(const CVector& lookAt, float elapsedTime, float& alpha, float& beta)
    {
        // TODO: generated code Gun::_GetOffsetAngles
        // Get the gun's world position
        CVector gunPosition;
        if (m_barrelNode)
        {
            gunPosition = m_barrelNode->GetOriginWorldAbs();
        }
        else
        {
            gunPosition = GetNodeAbsolutePosition();
        }

        // Calculate direction to target and normalize it
        CVector targetDir = lookAt - gunPosition;
        float invLength = 1.0f / std::sqrt(targetDir.x * targetDir.x + targetDir.y * targetDir.y + targetDir.z * targetDir.z + 1.1920929e-7f);
        targetDir.x *= invLength;
        targetDir.y *= invLength;
        targetDir.z *= invLength;

        // Get owner's inverse rotation to transform to local space
        ai::PhysicObj* owner = GetOwner();
        Quaternion ownerRot = owner->GetRotation();
        Quaternion invOwnerRot = ownerRot.getInversed();

        // Convert inverse owner rotation to matrix
        CMatrix ownerMat;
        float xx = invOwnerRot.x * invOwnerRot.x;
        float yy = invOwnerRot.y * invOwnerRot.y;
        float zz = invOwnerRot.z * invOwnerRot.z;
        float xy = invOwnerRot.x * invOwnerRot.y;
        float xz = invOwnerRot.x * invOwnerRot.z;
        float yz = invOwnerRot.y * invOwnerRot.z;
        float wx = invOwnerRot.w * invOwnerRot.x;
        float wy = invOwnerRot.w * invOwnerRot.y;
        float wz = invOwnerRot.w * invOwnerRot.z;

        ownerMat._11 = 1.0f - 2.0f * (yy + zz);
        ownerMat._12 = 2.0f * (xy + wz);
        ownerMat._13 = 2.0f * (xz - wy);
        ownerMat._14 = 0.0f;

        ownerMat._21 = 2.0f * (xy - wz);
        ownerMat._22 = 1.0f - 2.0f * (xx + zz);
        ownerMat._23 = 2.0f * (yz + wx);
        ownerMat._24 = 0.0f;

        ownerMat._31 = 2.0f * (xz + wy);
        ownerMat._32 = 2.0f * (yz - wx);
        ownerMat._33 = 1.0f - 2.0f * (xx + yy);
        ownerMat._34 = 0.0f;

        ownerMat._41 = 0.0f;
        ownerMat._42 = 0.0f;
        ownerMat._43 = 0.0f;
        ownerMat._44 = 1.0f;

        // Transform target direction to local space
        CVector localTargetDir;
        localTargetDir.x = ownerMat._11 * targetDir.x + ownerMat._21 * targetDir.y + ownerMat._31 * targetDir.z;
        localTargetDir.y = ownerMat._12 * targetDir.x + ownerMat._22 * targetDir.y + ownerMat._32 * targetDir.z;
        localTargetDir.z = ownerMat._13 * targetDir.x + ownerMat._23 * targetDir.y + ownerMat._33 * targetDir.z;

        // Get current gun rotation and convert to matrix
        Quaternion gunRot = GetNodeRelativeRotation();
        CMatrix gunMat;

        xx = gunRot.x * gunRot.x;
        yy = gunRot.y * gunRot.y;
        zz = gunRot.z * gunRot.z;
        xy = gunRot.x * gunRot.y;
        xz = gunRot.x * gunRot.z;
        yz = gunRot.y * gunRot.z;
        wx = gunRot.w * gunRot.x;
        wy = gunRot.w * gunRot.y;
        wz = gunRot.w * gunRot.z;

        gunMat._11 = 1.0f - 2.0f * (yy + zz);
        gunMat._12 = 2.0f * (xy + wz);
        gunMat._13 = 2.0f * (xz - wy);
        gunMat._14 = 0.0f;

        gunMat._21 = 2.0f * (xy - wz);
        gunMat._22 = 1.0f - 2.0f * (xx + zz);
        gunMat._23 = 2.0f * (yz + wx);
        gunMat._24 = 0.0f;

        gunMat._31 = 2.0f * (xz + wy);
        gunMat._32 = 2.0f * (yz - wx);
        gunMat._33 = 1.0f - 2.0f * (xx + yy);
        gunMat._34 = 0.0f;

        gunMat._41 = 0.0f;
        gunMat._42 = 0.0f;
        gunMat._43 = 0.0f;
        gunMat._44 = 1.0f;

        // Extract yaw, pitch, roll from gun matrix
        float currentYaw, currentPitch, roll;
        gunMat.getYPR(currentYaw, currentPitch, roll);

        currentYaw += m_initialHorizAngle;
        // Normalize angle to [-PI, PI]
        if (currentYaw > M_PI)
        {
            currentYaw -= 2.0f * M_PI;
        }
        else if (currentYaw < -M_PI)
        {
            currentYaw += 2.0f * M_PI;
        }

        // Get barrel pitch if barrel node exists
        float barrelPitch = 0.0f;
        if (m_barrelNode)
        {
            CMatrix barrelMat;
            const Quaternion& barrelRot = m_barrelNode->GetRotation();

            xx = barrelRot.x * barrelRot.x;
            yy = barrelRot.y * barrelRot.y;
            zz = barrelRot.z * barrelRot.z;
            xy = barrelRot.x * barrelRot.y;
            xz = barrelRot.x * barrelRot.z;
            yz = barrelRot.y * barrelRot.z;
            wx = barrelRot.w * barrelRot.x;
            wy = barrelRot.w * barrelRot.y;
            wz = barrelRot.w * barrelRot.z;

            barrelMat._11 = 1.0f - 2.0f * (yy + zz);
            barrelMat._12 = 2.0f * (xy + wz);
            barrelMat._13 = 2.0f * (xz - wy);
            barrelMat._14 = 0.0f;

            barrelMat._21 = 2.0f * (xy - wz);
            barrelMat._22 = 1.0f - 2.0f * (xx + zz);
            barrelMat._23 = 2.0f * (yz + wx);
            barrelMat._24 = 0.0f;

            barrelMat._31 = 2.0f * (xz + wy);
            barrelMat._32 = 2.0f * (yz - wx);
            barrelMat._33 = 1.0f - 2.0f * (xx + yy);
            barrelMat._34 = 0.0f;

            barrelMat._41 = 0.0f;
            barrelMat._42 = 0.0f;
            barrelMat._43 = 0.0f;
            barrelMat._44 = 1.0f;

            float barrelYaw, pitch, barrelRoll;
            barrelMat.getYPR(barrelYaw, pitch, barrelRoll);
            barrelPitch = -pitch;
        }

        // Get current relative direction for comparison
        CVector oldRelDir = GetNodeRelativeDirection();

        // Calculate desired alpha angle
        float desiredAlpha = std::atan2(localTargetDir.x, localTargetDir.z) + m_initialHorizAngle;
        // Normalize desired alpha
        if (desiredAlpha > M_PI)
        {
            desiredAlpha -= 2.0f * M_PI;
        }
        else if (desiredAlpha < -M_PI)
        {
            desiredAlpha += 2.0f * M_PI;
        }

        m_currentDesiredAlpha = desiredAlpha;

        // Handle alpha movement with constraints
        if (m_rightStopAngle - m_leftStopAngle > 2.0f * M_PI)
        {
            // Unconstrained movement
            float cross = localTargetDir.z * oldRelDir.x - localTargetDir.x * oldRelDir.z;
            int direction;
            if (cross > 1e-6f)
            {
                direction = 1;
            }
            else if (cross < -1e-6f)
            {
                direction = -1;
            }
            else
            {
                direction = 0;
            }

            alpha = currentYaw - (direction * m_turningSpeed * elapsedTime);

            // Check if we should stop at target
            float alphaDiff = alpha - desiredAlpha;
            float currentDiff = currentYaw - desiredAlpha;

            int alphaSign = (alphaDiff > 1e-6f) ? 1 : ((alphaDiff < -1e-6f) ? -1 : 0);
            int currentSign = (currentDiff > 1e-6f) ? 1 : ((currentDiff < -1e-6f) ? -1 : 0);

            if (alphaSign * currentSign <= 0)
            {
                alpha = desiredAlpha;
            }
        }
        else
        {
            // Constrained movement
            float oldAlpha = std::atan2(oldRelDir.x, oldRelDir.z) + m_initialHorizAngle;
            if (oldAlpha > M_PI)
            {
                oldAlpha -= 2.0f * M_PI;
            }
            else if (oldAlpha < -M_PI)
            {
                oldAlpha += 2.0f * M_PI;
            }

            // Clamp desired alpha to stop angles
            float clampedDesiredAlpha = desiredAlpha;
            if (m_leftStopAngle > desiredAlpha)
            {
                clampedDesiredAlpha = m_leftStopAngle;
            }
            if (clampedDesiredAlpha > m_rightStopAngle)
            {
                clampedDesiredAlpha = m_rightStopAngle;
            }

            // Determine movement direction
            float diff = oldAlpha - clampedDesiredAlpha;
            int direction;
            if (diff > 1e-6f)
            {
                direction = 1;
            }
            else if (diff < -1e-6f)
            {
                direction = -1;
            }
            else
            {
                direction = 0;
            }

            alpha = currentYaw - (direction * m_turningSpeed * elapsedTime);

            // Check if we should stop at target
            float alphaDiff = alpha - clampedDesiredAlpha;
            float currentDiff = currentYaw - clampedDesiredAlpha;

            int alphaSign = (alphaDiff > 1e-6f) ? 1 : ((alphaDiff < -1e-6f) ? -1 : 0);
            int currentSign = (currentDiff > 1e-6f) ? 1 : ((currentDiff < -1e-6f) ? -1 : 0);

            if (alphaSign * currentSign <= 0)
            {
                alpha = clampedDesiredAlpha;
            }

            // Clamp to stop angles
            if (m_leftStopAngle > alpha)
            {
                alpha = m_leftStopAngle;
            }
            if (alpha > m_rightStopAngle)
            {
                alpha = m_rightStopAngle;
            }
        }

        // Final alpha normalization
        alpha -= m_initialHorizAngle;
        if (alpha > M_PI)
        {
            alpha -= 2.0f * M_PI;
        }
        else if (alpha < -M_PI)
        {
            alpha += 2.0f * M_PI;
        }

        // Calculate beta angle
        float desiredBeta = std::asin(localTargetDir.y);

        // Apply beta constraints
        float clampedDesiredBeta = desiredBeta;
        if (m_lowStopAngle > desiredBeta)
        {
            clampedDesiredBeta = m_lowStopAngle;
        }
        if (clampedDesiredBeta > m_highStopAngle)
        {
            clampedDesiredBeta = m_highStopAngle;
        }

        // Smooth beta movement
        int betaDirection = (clampedDesiredBeta - barrelPitch >= 0.0f) ? 1 : -1;
        beta = barrelPitch + (betaDirection * m_turningSpeed * elapsedTime);

        // Check if we should stop at target beta
        float betaDiff = beta - clampedDesiredBeta;
        float currentBetaDiff = barrelPitch - clampedDesiredBeta;

        int betaSign = (betaDiff > 1e-6f) ? 1 : ((betaDiff < -1e-6f) ? -1 : 0);
        int currentBetaSign = (currentBetaDiff > 1e-6f) ? 1 : ((currentBetaDiff < -1e-6f) ? -1 : 0);

        if (betaSign * currentBetaSign <= 0)
        {
            beta = clampedDesiredBeta;
        }

        // Clamp beta to constraints
        if (m_lowStopAngle > beta)
        {
            beta = m_lowStopAngle;
        }
        if (beta > m_highStopAngle)
        {
            beta = m_highStopAngle;
        }
    }

    bool Gun::_bIsRapidFiring() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Gun::_UpdateNodeFiringAction()
    {
        // TODO: check this
        if (m_Node)
        {
            int action = 0;
            m_Node->GetProperty(m3d::PROP_DM_ACTION, &action);
            if (_bIsUsingVolley())
            {
                if (_bIsVolleyFiring())
                {
                    if (action == 2)
                        return;
                    SetNodeAction(2, 1);
                }
                if (action)
                    SetNodeAction(0, 1);
            }
            else if (m_bIsFiring)
            {
                if (m_bJustShot)
                {
                    m_bJustShot = 0;
                    if (m_firingRate <= 299.89999)
                        SetNodeAction(0, 1);
                    if (action != 2 || !_bIsRapidFiring())
                        SetNodeAction(2, 1);
                }
            }
            else if (m_bWasShot && (m_timeFromLastShot >= (float)(60.0 / m_firingRate) || m_ChargeState == csInCharging))
            {
                m_bWasShot = 0;
                SetNodeAction(0, 1);
            }
        }
    }

    m3d::Object* Gun::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

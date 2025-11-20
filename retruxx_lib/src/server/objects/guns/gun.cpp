#include "gun.h"

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

    bool Gun::Fire(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

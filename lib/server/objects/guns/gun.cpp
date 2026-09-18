#include "gun.h"

#include "compoundgun.h"
#include "m3dapp.h"
#include "core/log.h"

#include <stdexcept>

#include "math/matrix.h"
#include "scene/servers/dataserver.h"
#include "scene/servers/serveranimatedmodel.h"
#include "server/objects/blastwave.h"
#include "server/objects/vehicle.h"
#include "server/objects/base/objcontainer.h"

#include <server/objects/base/prototypemanager.h>
#include <server/objects/player.h>
#include <server/processmanager.h>
#include <server/server.h>
#include <server/statistic/statisticmanager.h>
#include <server/statistic/intstatistic.h>
#include <server/statistic/favoritestringstatistic.h>
#include <server/statistic/intintratiostatistic.h>
#include <level.h>
#include <world.h>

extern "C"
{
#include <ode/objects.h>
}
#include "ode/odecpp.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInCurrentCharge)
{
    auto* gun = (ai::Gun*)context->asObject(0, "Gun");
    context->pushInt(gun->GetShellsInCurrentCharge());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInCurrentCharge)
{
    auto* gun = (ai::Gun*)context->asObject(0, "Gun");
    gun->SetShellsInCurrentCharge(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetShellsInPool)
{
    auto* gun = (ai::Gun*)context->asObject(0, "Gun");
    context->pushInt(gun->GetShellsInPool());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetShellsInPool)
{
    auto* gun = (ai::Gun*)context->asObject(0, "Gun");
    gun->SetShellsInPool(context->asInt(1));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, GetChargeState)
{
    auto* gun = (ai::Gun*)context->asObject(0, "Gun");
    context->pushInt(gun->GetChargeState());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Gun, SetChargeState)
{
    auto* gun = (ai::Gun*)context->asObject(0, "Gun");
    gun->SetChargeState(context->asInt(1));
    return 1;
}

namespace ai
{
    namespace
    {
        float const DEFAULT_TURNING_SPEED = 180.0;

        struct _FiringType2Str
        {
            /* 0x0000 */ FiringTypes m_type;
            /* 0x0004 */ CStr m_name;
        }; /* size: 0x0010 */

        _FiringType2Str const l_firingType2Str[] = {
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

        _DamageType2Str const l_damageType2Str[] = {
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

        bool PointIsReachable(
            CVector const& src,
            CVector const& dir,
            CVector const& dst,
            std::vector<int> const& exceptions)
        {
            // TODO: check this
            static scoped_ptr Ray = ai::Ray::CreateObject(nullptr, 1000.0, nullptr);
            Ray->SetPosition(src);
            Ray->SetDirection(dir.getNormalized());
            Ray->SetLength(dir.length());

            bool res = true;
            ai::ObjIdExceptionalTraceLineCallback callback(exceptions);
            dContact closestContact;
            if (ai::TraceLine(*Ray, closestContact, 0, 0, 0, 0, &callback, 1, 0))
            {
                auto v9 = src.y - dst.y;
                auto v10 = src.z - dst.z;
                if ((float)((float)((float)((float)((float)(src.z - closestContact.geom.pos[2]) *
                                                    (float)(src.z - closestContact.geom.pos[2])) +
                                            (float)((float)(src.y - closestContact.geom.pos[1]) *
                                                    (float)(src.y - closestContact.geom.pos[1]))) +
                                    (float)((float)(src.x - closestContact.geom.pos[0]) *
                                            (float)(src.x - closestContact.geom.pos[0]))) +
                            0.0099999998) <= (float)((float)((float)(v10 * v10) + (float)(v9 * v9)) +
                                                     (float)((float)(src.x - dst.x) * (float)(src.x - dst.x))))
                {
                    res = false;
                }
            }
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
        auto const item = animatedModelsServer->GetItemByName(m_engineModelName.c_str(), true);
        m3d::AnimatedModel* model = nullptr;
        if (item != -1)
        {
            animatedModelsServer->GetItemProperty(item, m3d::PROP_INTERNAL_GETMODEL, &model);

            auto const loadPointId = model->GetLoadPointIdByName("LP_GUN");
            auto const& loadPoint = model->GetBoneBounds(loadPointId);
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
            auto const res = animatedModelsServer->GetBoneMatrixByNameFromModelName(
                m_barrelModelName.c_str(), GetFireLp(i), boneMatrix, false);
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
                auto const barrelItem = animatedModelsServer->GetItemByName(m_barrelModelName.c_str(), true);
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

    CStr GunPrototypeInfo::DamageType2Str(DamageType damageType)
    {
        // RVA 0x6E0C20
        for (auto const& entry : l_damageType2Str)
        {
            if (entry.m_type == damageType)
            {
                return entry.m_name;
            }
        }
        return {};
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

    void GunPrototypeInfo::CreateBlastWave(CVector const& pos, int gunId) const
    {
        if (m_blastWavePrototypeId != -1)
        {
            auto const objId = theObjects->CreateNewObject(m_blastWavePrototypeId, "", -1, -1);
            auto* obj = theObjects->GetEntityByObjId(objId);
            if (obj)
            {
                auto* blastWave = RT_DYNCAST(obj, BlastWave);
                blastWave->SetPosition(pos);
                blastWave->SetEmitterId(gunId);
                blastWave->SetRocketExplosionType(m_explosionType);
                auto* gun = theObjects->GetEntityByObjId(gunId);
                if (gun)
                {
                    blastWave->SetBelong(gun->GetBelong());
                }
            }
        }
    }

    CStr GunPrototypeInfo::FiringType2Str(FiringTypes firingType)
    {
        for (auto const& entry : l_firingType2Str)
        {
            if (entry.m_type == firingType)
            {
                return entry.m_name;
            }
        }
        return {};
    }

    FiringTypes GunPrototypeInfo::Str2FiringType(CStr const& firingTypeStr)
    {
        for (auto const& firingType : l_firingType2Str)
        {
            if (firingType.m_name == firingTypeStr)
            {
                return firingType.m_type;
            }
        }
        return FT_NUM_FIRING_TYPES;
    }

    DamageType GunPrototypeInfo::Str2DamageType(CStr const& damageTypeStr)
    {
        for (auto const& damageType : l_damageType2Str)
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
        auto const res = VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
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
        // RVA 0x6E3330 - guns that fire several shells at once override this.
        return m_damage;
    }

    void GunPrototypeInfo::PostLoad()
    {
        m_explosionType = gDynamicScene->GetExplosionType(m_explosionTypeName);
        m_shellPrototypeId = thePrototypeManager->GetPrototypeId(m_shellPrototypeName);
        if (m_shellPrototypeId == -1 && !m_shellPrototypeName.empty())
        {
            M3D_CRITICAL_ERROR(
                "shell prototype '" + m_shellPrototypeName + "' is invalid for '" + GetDebugDescription());
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
        // RVA 0x601110
        return m_explosionType;
    }

    bool GunPrototypeInfo::_bIsRapidFiring() const
    {
        // RVA 0x6DDCA0 - anything faster than 300 rounds a minute is drawn as a continuous stream.
        return m_firingRate > 299.89999;
    }

    m3d::SgNode* Gun::GetBarrelNode() const
    {
        return m_barrelNode;
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
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), GunPrototypeInfo const);
    }

    void Gun::SetShellsInPool(unsigned Shells)
    {
        // RVA 0x2DE040
        m_ShellsInPool = Shells;
    }

    void Gun::RenderGunDebugInfo() const
    {
        // RVA 0x6E2B10 - a green line five metres down the barrel.
        CVector const from = _CalcRoughPosForNextShot();
        CMatrix const shotMatrix = GetMatrixForShot(m_curBarrelIndex);
        CVector to;
        to.x = shotMatrix._21 * 5.0 + from.x;
        to.y = shotMatrix._22 * 5.0 + from.y;
        to.z = shotMatrix._23 * 5.0 + from.z;
        M3D_APP->DrawLine(from, to, 0xFF00FF00);
    }

    float Gun::EstimateDamage(CVector const& pos, retruxx::vector<int, retruxx::allocator<int>> const& exceptions) const
    {
        // RVA 0x6E2D70 - a shot is worth its full damage if it can get there at all.
        return PointIsReachable(pos, exceptions) ? m_damage : 0.0f;
    }

    float Gun::EstimateDamage() const
    {
        return m_damage;
    }

    bool Gun::bIs360DegreesHoriz() const
    {
        // RVA 0x6DDC50
        return m_rightStopAngle - m_leftStopAngle > 6.283184482025146;
    }

    CStr Gun::GetPropertyName(int id) const
    {
        // RVA 0x6E30B0
        for (auto const& property : m_propertiesMap)
        {
            if (property.second == id)
            {
                return property.first;
            }
        }
        return VehiclePart::GetPropertyName(id);
    }

    bool Gun::CanLookAtTarget() const
    {
        return m_rightStopAngle - m_leftStopAngle > 6.283184482025146 ||
            (m_rightStopAngle + 0.1) > m_currentDesiredAlpha && m_currentDesiredAlpha > (m_leftStopAngle - 0.1);
    }

    float Gun::GetRechargingTime() const
    {
        // RVA 0x2DF9A0
        return (getShellsForRecharge() * m_ReChargingTimePerShell) + m_ReChargingTime;
    }

    m3d::Class* Gun::GetClass() const
    {
        // RVA 0x6DDC90
        return RT_CLASS_LOCAL(Gun);
    }

    bool Gun::isLookAtPoint(CVector const& lookAt, float eps) const
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
        float invTargetLength = 1.0f /
            std::sqrt(targetDir.x * targetDir.x + targetDir.y * targetDir.y + targetDir.z * targetDir.z +
                      1.1920929e-7f);
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
        float invGunLength = 1.0f /
            std::sqrt(gunForwardDir.x * gunForwardDir.x + gunForwardDir.y * gunForwardDir.y +
                      gunForwardDir.z * gunForwardDir.z + 1.1920929e-7f);
        CVector normalizedGunForwardDir;
        normalizedGunForwardDir.x = gunForwardDir.x * invGunLength;
        normalizedGunForwardDir.y = gunForwardDir.y * invGunLength;
        normalizedGunForwardDir.z = gunForwardDir.z * invGunLength;

        // Calculate the cross product between gun direction and target direction
        // This gives us the "error" vector - its magnitude indicates how misaligned we are
        CVector crossProduct;
        crossProduct.x =
            normalizedGunForwardDir.y * normalizedTargetDir.z - normalizedGunForwardDir.z * normalizedTargetDir.y;
        crossProduct.y =
            normalizedGunForwardDir.z * normalizedTargetDir.x - normalizedGunForwardDir.x * normalizedTargetDir.z;
        crossProduct.z =
            normalizedGunForwardDir.x * normalizedTargetDir.y - normalizedGunForwardDir.y * normalizedTargetDir.x;

        // Calculate the magnitude of the cross product (alignment error)
        float alignmentError = std::sqrt(
            crossProduct.x * crossProduct.x + crossProduct.y * crossProduct.y + crossProduct.z * crossProduct.z);

        // Return true if the alignment error is within the epsilon tolerance
        // This means the gun is pointing close enough to the target
        return eps > std::fabs(alignmentError);
    }

    void Gun::SetInvisible()
    {
        ai::PhysicBody::SetInvisible();
        m_bIsFiring = 0;
        m_timeFromLastShot = 1000.0f;
        _UpdateNodeFiringAction();
    }

    void Gun::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const
    {
        // RVA 0x6E2FB0
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.first);
        }
        VehiclePart::GetPropertiesNames(Props);
    }

    void Gun::Update(float elapsedTime, unsigned workTime)
    {
        VehiclePart::Update(elapsedTime, workTime);
        if (elapsedTime >= 0.001)
        {
            m_timeFromLastShot = m_timeFromLastShot + elapsedTime;
            _UpdateNodeFiringAction();
            auto const* protoInfo = GetPrototypeInfo();
            if (m_ChargeState == csInCharging)
            {
                m_CurrentReChargingTime = m_CurrentReChargingTime + elapsedTime;
                if (m_CurrentReChargingTime > GetRechargingTime())
                {
                    if (protoInfo->m_WithShellsPoolLimit)
                    {
                        if (m_ShellsInPool)
                        {
                            auto const shellsForRecharge = getShellsForRecharge();
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
        // RVA 0x6DDFB0
        return m_damage;
    }

    bool Gun::CanShotToTarget(int targetId) const
    {
        // RVA 0x6E1FD0 - fire a trace ray down the barrel and see whether the first thing it meets
        // belongs to the object being aimed at. A wheel answers for the vehicle it is fitted to.
        if (targetId == -1)
        {
            return false;
        }

        static scoped_ptr<Ray> shotRay(ai::Ray::CreateObject(nullptr, 1000.0, nullptr));

        CVector const gunPos = _CalcRoughPosForNextShot();
        dGeomSetPosition(shotRay->GetGeomId(), gunPos.x, gunPos.y, gunPos.z);

        CMatrix const shotMatrix = GetMatrixForShot(m_curBarrelIndex);
        CVector fireDir;
        fireDir.x = shotMatrix._21;
        fireDir.y = shotMatrix._22;
        fireDir.z = shotMatrix._23;
        float const invLength =
            1.0 / sqrt(fireDir.z * fireDir.z + fireDir.y * fireDir.y + fireDir.x * fireDir.x + 0.00000011920929);
        fireDir.x = fireDir.x * invLength;
        fireDir.y = fireDir.y * invLength;
        fireDir.z = fireDir.z * invLength;
        shotRay->SetDirection(fireDir);
        shotRay->SetLength(m_firingRange);

        dContact closestContact;
        if (!ai::TraceLine(*shotRay, closestContact, 0, 0, 1, 1, nullptr, 1, 0))
        {
            return false;
        }

        auto* const hit = (m3d::Object*)dGeomGetData(closestContact.geom.g2);
        if (!hit || !hit->IsKindOf(&ai::PhysicBody::m_classPhysicBody))
        {
            return false;
        }
        PhysicObj* const owner = ((PhysicBody*)hit)->GetOwner();
        if (!owner)
        {
            return false;
        }
        if (!owner->IsKindOf(&ai::Wheel::m_classWheel))
        {
            return owner->GetId() == targetId;
        }
        Vehicle const* const vehicle = ((Wheel*)owner)->GetVehicle();
        return vehicle && vehicle->GetId() == targetId;
    }

    void Gun::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const
    {
        // RVA 0x6E3030
        for (auto const& property : m_propertiesMap)
        {
            Props.insert(property.second);
        }
        VehiclePart::GetPropertiesIDs(Props);
    }

    bool Gun::PointIsReachableFromPosition(
        CVector const& newPosition,
        CVector const& target,
        retruxx::vector<int, retruxx::allocator<int>> exceptions) const
    {
        // RVA 0x6E2DB0 - the same test as PointIsReachable, but asking whether the gun could hit the
        // target from somewhere it is not standing yet.
        for (Obj const* obj = this; obj; obj = obj->GetParent())
        {
            exceptions.push_back(obj->GetId());
        }

        CVector const direction = target - newPosition;
        float const distance =
            sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
        if (distance > m_firingRange)
        {
            return false;
        }
        return ai::PointIsReachable(newPosition, direction, target, exceptions);
    }

    void Gun::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x6E0CB0
        VehiclePart::LoadRuntimeValues(xmlFile, xmlNode);

        if (!xmlNode->IsEmpty())
        {
            char const* const targetId = xmlNode->GetAttribute("TargetId");
            if (targetId)
            {
                m_targetObjId = atoi(targetId);
            }
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const curBarrelNum = xmlNode->GetAttribute("CurBarrelNum");
            if (curBarrelNum)
            {
                int const value = atoi(curBarrelNum);
                if (value >= 0)
                {
                    m_curBarrelIndex = value;
                }
            }
        }
        if (m_barrelNode)
        {
            Quaternion barrelNodeRotation = m_barrelNode->GetRotation();
            m3d::SafeQuaternionAttrib(barrelNodeRotation, xmlNode, "BarrelNodeRotation");
            m_barrelNode->SetRotation(barrelNodeRotation);
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const chargeState = xmlNode->GetAttribute("ChargeState");
            if (chargeState)
            {
                m_ChargeState = static_cast<ChargeState>(atoi(chargeState));
            }
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const currentReChargingTime = xmlNode->GetAttribute("CurrentReChargingTime");
            if (currentReChargingTime)
            {
                m_CurrentReChargingTime = static_cast<float>(atof(currentReChargingTime));
            }
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const shellsInCurrentCharge = xmlNode->GetAttribute("ShellsInCurrentCharge");
            if (shellsInCurrentCharge)
            {
                int const value = atoi(shellsInCurrentCharge);
                if (value >= 0)
                {
                    m_ShellsInCurrentCharge = value;
                }
            }
        }
        if (m_ShellsInCurrentCharge > m_ChargeSize)
        {
            m_ShellsInCurrentCharge = m_ChargeSize;
        }
        if (!xmlNode->IsEmpty())
        {
            char const* const shellsInPool = xmlNode->GetAttribute("ShellsInPool");
            if (shellsInPool)
            {
                int const value = atoi(shellsInPool);
                if (value >= 0)
                {
                    m_ShellsInPool = value;
                }
            }
        }
        m3d::SafeBoolAttrib(m_bIsFiring, xmlNode, "IsFiring");
        m3d::SafeBoolAttrib(m_bWasShot, xmlNode, "WasShot");
        m3d::SafeBoolAttrib(m_bJustShot, xmlNode, "JustShot");
    }

    unsigned Gun::GetShellsInPool() const
    {
        // RVA 0x2DE030
        return m_ShellsInPool;
    }

    bool Gun::IsWithCharging() const
    {
        // RVA 0x2DE2C0
        return GetPrototypeInfo()->m_WithCharging;
    }

    float Gun::GetFiringRate() const
    {
        // RVA 0x2DDFD0
        return m_firingRate;
    }

    CMatrix Gun::GetMatrixForShot(unsigned) const
    {
        // RVA 0x2E15F0
        // NOTE: the shipped build ignores the barrelIndex argument and always
        // uses m_curBarrelIndex; every caller passes m_curBarrelIndex anyway.
        // Get the prototype information for this gun
        ai::GunPrototypeInfo const* prototypeInfo = this->GetPrototypeInfo();

        // Create the base fire matrix from prototype data
        CMatrix fireMat = prototypeInfo->m_fireLpMatrices[m_curBarrelIndex];

        // If we have a barrel node, transform the fire matrix by the barrel's current world transform
        if (m_barrelNode != nullptr)
        {
            CMatrix const& barrelXForm = m_barrelNode->GetCurrentMatrix();

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
            fireMat._11 = barrelXForm._11 * orig_11 + barrelXForm._21 * orig_12 + barrelXForm._31 * orig_13 +
                barrelXForm._41 * orig_14;
            fireMat._12 = barrelXForm._12 * orig_11 + barrelXForm._22 * orig_12 + barrelXForm._32 * orig_13 +
                barrelXForm._42 * orig_14;
            fireMat._13 = barrelXForm._13 * orig_11 + barrelXForm._23 * orig_12 + barrelXForm._33 * orig_13 +
                barrelXForm._43 * orig_14;
            fireMat._14 = barrelXForm._14 * orig_11 + barrelXForm._24 * orig_12 + barrelXForm._34 * orig_13 +
                barrelXForm._44 * orig_14;

            // Transform second row [21, 22, 23, 24]
            fireMat._21 = barrelXForm._11 * orig_21 + barrelXForm._21 * orig_22 + barrelXForm._31 * orig_23 +
                barrelXForm._41 * orig_24;
            fireMat._22 = barrelXForm._12 * orig_21 + barrelXForm._22 * orig_22 + barrelXForm._32 * orig_23 +
                barrelXForm._42 * orig_24;
            fireMat._23 = barrelXForm._13 * orig_21 + barrelXForm._23 * orig_22 + barrelXForm._33 * orig_23 +
                barrelXForm._43 * orig_24;
            fireMat._24 = barrelXForm._14 * orig_21 + barrelXForm._24 * orig_22 + barrelXForm._34 * orig_23 +
                barrelXForm._44 * orig_24;

            // Transform third row [31, 32, 33, 34]
            fireMat._31 = barrelXForm._11 * orig_31 + barrelXForm._21 * orig_32 + barrelXForm._31 * orig_33 +
                barrelXForm._41 * orig_34;
            fireMat._32 = barrelXForm._12 * orig_31 + barrelXForm._22 * orig_32 + barrelXForm._32 * orig_33 +
                barrelXForm._42 * orig_34;
            fireMat._33 = barrelXForm._13 * orig_31 + barrelXForm._23 * orig_32 + barrelXForm._33 * orig_33 +
                barrelXForm._43 * orig_34;
            fireMat._34 = barrelXForm._14 * orig_31 + barrelXForm._24 * orig_32 + barrelXForm._34 * orig_33 +
                barrelXForm._44 * orig_34;

            // Transform fourth row [41, 42, 43, 44]
            fireMat._41 = barrelXForm._11 * orig_41 + barrelXForm._21 * orig_42 + barrelXForm._31 * orig_43 +
                barrelXForm._41 * orig_44;
            fireMat._42 = barrelXForm._12 * orig_41 + barrelXForm._22 * orig_42 + barrelXForm._32 * orig_43 +
                barrelXForm._42 * orig_44;
            fireMat._43 = barrelXForm._13 * orig_41 + barrelXForm._23 * orig_42 + barrelXForm._33 * orig_43 +
                barrelXForm._43 * orig_44;
            fireMat._44 = barrelXForm._14 * orig_41 + barrelXForm._24 * orig_42 + barrelXForm._34 * orig_43 +
                barrelXForm._44 * orig_44;
        }

        // Copy the final matrix to the result
        return fireMat;
    }

    float Gun::GetCurrentRechargingTime() const
    {
        // RVA 0x2DE2F0
        return m_CurrentReChargingTime;
    }

    unsigned Gun::GetShellsPoolSize() const
    {
        // RVA 0x2DE050
        return GetPrototypeInfo()->m_ShellsPoolSize;
    }

    int Gun::OnEvent(Event const& evn)
    {
        auto const result = Obj::OnEvent(evn);
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
        // RVA 0x2DE2E0
        return m_ChargeSize;
    }

    float Gun::GetFiringRange() const
    {
        return m_firingRange;
    }

    float Gun::EstimateDamageFromPosition(
        CVector const& position,
        CVector const& pos,
        retruxx::vector<int, retruxx::allocator<int>> const& exceptions)
    {
        // RVA 0x6E2EC0
        return PointIsReachableFromPosition(position, pos, exceptions) ? m_damage : 0.0f;
    }

    bool Gun::PointIsReachable(CVector const& pos, retruxx::vector<int, retruxx::allocator<int>> exceptions) const
    {
        // TODO: generated code Gun::PointIsReachable
        // Add all parent objects to exceptions list (to avoid hitting ourselves)
        ai::Gun const* currentObj = this;
        while (currentObj != nullptr)
        {
            int objId = currentObj->GetId();
            exceptions.push_back(objId);
            currentObj = (ai::Gun*)currentObj->GetParent();
        }

        // Get the gun's firing position and orientation
        CMatrix gunMatrix = GetMatrixForShot(m_curBarrelIndex);

        // Calculate direction from gun to target
        CVector gunPos;
        gunPos.x = gunMatrix._41;
        gunPos.y = gunMatrix._42;
        gunPos.z = gunMatrix._43;

        CVector direction;
        direction.x = pos.x - gunPos.x;
        direction.y = pos.y - gunPos.y;
        direction.z = pos.z - gunPos.z;

        // Calculate distance to target
        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

        bool isReachable = false;

        // Check if target is within firing range
        if (distance <= m_firingRange)
        {
            // Check if there's a clear line of sight to the target
            isReachable = ai::PointIsReachable(gunPos, direction, pos, exceptions);
        }
        else
        {
            // Target is out of range
            isReachable = false;
        }

        return isReachable;
    }

    unsigned Gun::GetShellsInCurrentCharge() const
    {
        // RVA 0x2DE020
        return m_ShellsInCurrentCharge;
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
        // RVA 0x2DFA00
        if (getShellsForRecharge() && m_ChargeState != csInCharging)
        {
            BeginReCharge();
        }
    }

    int Gun::GetPropertyId(char const* propName) const
    {
        auto it = Gun::m_propertiesMap.find(propName);
        if (it != Gun::m_propertiesMap.end())
        {
            return it->second;
        }

        return VehiclePart::GetPropertyId(propName);
    }

    eGObjPropertySaveStatus Gun::GetPropertySaveStatus(int id) const
    {
        // RVA 0x6E2840
        auto const it = m_propertiesSaveStatesMap.find(id);
        if (it != m_propertiesSaveStatesMap.end())
        {
            return it->second;
        }
        return VehiclePart::GetPropertySaveStatus(id);
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
        // RVA 0x6DDE90 - the barrel node belongs to the map being left behind.
        VehiclePart::SetPassedToAnotherMapStatus();
        m_barrelNode = nullptr;
    }

    float Gun::GetDamage() const
    {
        // RVA 0x6DDFA0
        return m_damage;
    }

    void Gun::SetInitialHorizAngle(float angle)
    {
        m_initialHorizAngle = angle;
    }

    int Gun::GetShellPrototypeId() const
    {
        return m_shellPrototypeId;
    }

    Gun::ChargeState Gun::GetChargeState() const
    {
        // RVA 0x2DE060
        return m_ChargeState;
    }

    float Gun::GetTurningSpeed() const
    {
        // RVA 0x6DDF90
        return m_turningSpeed;
    }

    bool Gun::SetPropertyById(int propertyId, m3d::AIParam const& newValue)
    {
        bool result = true;
        switch (propertyId)
        {
        case 28:
            m_damage = newValue.GetAsFloat();
            result = 1;
            break;

        case 29:
            m_firingRate = newValue.GetAsFloat();
            result = 1;
            break;

        case 30:
            m_firingRange = newValue.GetAsFloat();
            result = 1;
            break;

        case 34:
            m_ChargeSize = newValue.GetAsID();
            result = 1;
            break;

        case 35:
            m_ReChargingTime = newValue.GetAsFloat();
            result = 1;
            break;

        case 36:
            m_ShellsInPool = newValue.GetAsID();
            result = 1;
            break;

        default:
            result = VehiclePart::SetPropertyById(propertyId, newValue);
            break;
        }
        return result;
    }

    void Gun::SetShellsInCurrentCharge(unsigned Value)
    {
        // RVA 0x2DF6F0 - clamped to the charge size.
        m_ShellsInCurrentCharge = Value;
        if (m_ShellsInCurrentCharge > m_ChargeSize)
        {
            m_ShellsInCurrentCharge = m_ChargeSize;
        }
    }

    DamageType Gun::GetDamageType() const
    {
        // RVA 0x6DDFC0
        return m_damageType;
    }

    bool Gun::IsDurabilityEnoughForFiring() const
    {
        auto* ownerCompoundVehiclePart = GetOwnerCompoundVehiclePart();
        if (ownerCompoundVehiclePart && IS_KIND_OF(ownerCompoundVehiclePart, CompoundGun))
        {
            auto* gun = RT_DYNCAST(ownerCompoundVehiclePart, CompoundGun const);
            return gun->IsDurabilityEnoughForFiring();
        }
        return Durability().value().get() > 0;
    }

    bool Gun::IsWithShellsPoolLimit() const
    {
        // RVA 0x2DE2D0
        return GetPrototypeInfo()->m_WithShellsPoolLimit;
    }

    void Gun::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x6E0E70
        VehiclePart::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("TargetId", CStr(m_targetObjId).c_str());
        xmlNode->SetAttribute("CurBarrelNum", CStr(m_curBarrelIndex).c_str());
        if (m_barrelNode)
        {
            xmlNode->SetAttribute("BarrelNodeRotation", CStr(m_barrelNode->GetRotation()).c_str());
        }
        xmlNode->SetAttribute("ChargeState", CStr(m_ChargeState).c_str());
        xmlNode->SetAttribute("CurrentReChargingTime", CStr(m_CurrentReChargingTime).c_str());
        xmlNode->SetAttribute("ShellsInCurrentCharge", CStr(m_ShellsInCurrentCharge).c_str());
        xmlNode->SetAttribute("ShellsInPool", CStr(m_ShellsInPool).c_str());
        xmlNode->SetAttribute("IsFiring", CStr(m_bIsFiring).c_str());
        xmlNode->SetAttribute("WasShot", CStr(m_bWasShot).c_str());
        xmlNode->SetAttribute("JustShot", CStr(m_bJustShot).c_str());
    }

    bool Gun::CanFire() const
    {
        return m_ChargeState == csReady && m_ShellsInCurrentCharge &&
            (GetPrototypeInfo()->m_ignoreStopAnglesWhenFire || CanLookAtTarget()) && IsDurabilityEnoughForFiring();
    }

    unsigned Gun::GetPrice(IPriceCoeffProvider const* priceCoeffProvider) const
    {
        // RVA 0x6E1E50 - a gun with a magazine is worth its own price plus whatever ammunition it is
        // carrying.
        unsigned price = VehiclePart::GetPrice(priceCoeffProvider);
        float const priceCoeff = GetPriceCoeff(priceCoeffProvider);

        GunPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (prototypeInfo->m_WithCharging && prototypeInfo->m_WithShellsPoolLimit &&
            prototypeInfo->m_shellPrototypeId != -1)
        {
            // NOTE: the shell prototype is used without a null check.
            PrototypeInfo const* const shellPrototypeInfo =
                thePrototypeManager->GetPrototypeInfo(prototypeInfo->m_shellPrototypeId);
            price += static_cast<int>(
                static_cast<double>((m_ShellsInPool + m_ShellsInCurrentCharge) *
                                    shellPrototypeInfo->GetBasePrice()) *
                priceCoeff);
        }
        return price;
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
        // RVA 0x6DDFF0
        return m_initialHorizAngle;
    }

    void Gun::SetChargeState(int Value)
    {
        // RVA 0x2DE5B0
        m_ChargeState = static_cast<ChargeState>(Value);
    }

    void Gun::SetChargeState(ChargeState Value)
    {
        // RVA 0x2DE070
        m_ChargeState = Value;
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
        if (m_recoilForce > 0.0)
        {
            auto* owner = GetOwner();
            if (owner)
            {
                auto const mat = GetMatrixForShot(m_curBarrelIndex);

                Quaternion rot;
                rot.FromMatrix(mat);

                CVector fireDir;
                float angle = 0.0f;
                rot.ToAxisAngle(fireDir, angle);

                CVector force;
                force.x = (float)(0.0 - fireDir.x) * m_recoilForce;
                force.z = (float)(0.0 - fireDir.z) * m_recoilForce;
                force.y = (float)((float)(0.0 - fireDir.y) * m_recoilForce) - (float)(m_recoilForce * 0.30000001);

                auto const pos = GetNodeRelativePosition();
                dBodyAddForceAtRelPos(owner->GetBody()->id(), force.x, force.y, force.z, pos.x, pos.y, pos.z);
            }
        }
    }

    void Gun::RegisterProperty(char const* Name, int id, eGObjPropertySaveStatus saveStatus)
    {
        // RVA 0x6E35D0 - SAVE_PROP_NORMAL is the default and is not recorded.
        m_propertiesMap[Name] = id;
        if (saveStatus)
        {
            m_propertiesSaveStatesMap[id] = saveStatus;
        }
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

        auto const* protoInfo = GetPrototypeInfo();
        ++m_curBarrelIndex;

        unsigned const barrelCount = static_cast<unsigned>(protoInfo->m_fireLpMatrices.size());
        if (m_curBarrelIndex == barrelCount)
        {
            m_curBarrelIndex = 0;
        }
        DoRecoil();
        return true;
    }

    bool Gun::_IsDirVerticallyReachable(CVector const& dir) const
    {
        // RVA 0x6DF790 - turn the direction into the owner's frame and check it against the gun's
        // elevation limits.
        Quaternion const ownerRotation = GetOwner()->GetRotation();
        CMatrix const ownerMatrix = ownerRotation.getInversed().ToMatrix();

        float const desiredBeta =
            asin(ownerMatrix._12 * dir.x + ownerMatrix._22 * dir.y + ownerMatrix._32 * dir.z);
        return desiredBeta >= m_lowStopAngle && m_highStopAngle >= desiredBeta;
    }

    bool Gun::_GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x6E3CC0 - the defaults are whatever the prototype says.
        GunPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        switch (propertyId)
        {
            case 28:
                retVal = prototypeInfo->m_damage;
                return true;
            case 29:
                retVal = prototypeInfo->m_firingRate;
                return true;
            case 30:
                retVal = prototypeInfo->m_firingRange;
                return true;
            case 34:
                retVal = prototypeInfo->m_ChargeSize;
                return true;
            case 35:
                retVal = prototypeInfo->m_ReChargingTime;
                return true;
            case 36:
                retVal = prototypeInfo->m_ShellsPoolSize;
                return true;
            default:
                return VehiclePart::_GetPropertyDefaultInternal(propertyId, retVal);
        }
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
        // RVA 0x2DF720
        return static_cast<unsigned>(GetPrototypeInfo()->m_fireLpMatrices.size());
    }

    void Gun::_LaunchShells()
    {
        // RVA 0x2E10C0 - the base implementation only books the shot into the
        // player's statistics; derived guns override this to spawn the shells.
        Obj* parentObj = this;
        while (!IS_KIND_OF(parentObj, Vehicle))
        {
            parentObj = parentObj->GetParent();
            if (!parentObj)
            {
                return;
            }
        }

        auto* vehicle = RT_DYNCAST(parentObj, Vehicle);
        if (!vehicle || !vehicle->bIsControlledByPlayer())
        {
            return;
        }

        CStr const& levelName = pServer->GetWorld()->m_level->m_levelName;
        CStr const& gunName = GetPrototypeInfo()->m_prototypeName;

        auto* shellsEmitted =
            static_cast<IntStatistic*>(theStatisticManager->GetStatistic(STATISTIC_SHELLS_EMITTED, "IntStatistic"));
        shellsEmitted->SetGlobalFlag(true);
        shellsEmitted->Increase(1);

        auto* levelShellsEmitted = static_cast<IntStatistic*>(
            theStatisticManager->GetStatistic(STATISTIC_SHELLS_EMITTED + levelName, "IntStatistic"));
        levelShellsEmitted->SetGlobalFlag(false);
        levelShellsEmitted->Increase(1);

        auto* favoriteGun = static_cast<FavoriteStringStatistic*>(
            theStatisticManager->GetStatistic(STATISTIC_FAVORITE_GUN, "FavoriteStringStatistic"));
        favoriteGun->SetGlobalFlag(true);
        favoriteGun->Increase(gunName);

        auto* levelFavoriteGun = static_cast<FavoriteStringStatistic*>(
            theStatisticManager->GetStatistic(STATISTIC_FAVORITE_GUN + levelName, "FavoriteStringStatistic"));
        levelFavoriteGun->SetGlobalFlag(false);
        levelFavoriteGun->Increase(gunName);

        // Every emitted shell bumps the hit-ratio denominator; the numerator is
        // raised elsewhere when a shell actually connects.
        auto* hitRatio = static_cast<IntIntRatioStatistic*>(
            theStatisticManager->GetStatistic(STATISTIC_HIT_RATIO, "IntIntRatioStatistic"));
        hitRatio->SetGlobalFlag(true);
        hitRatio->IncreaseDenominator(1);

        auto* levelHitRatio = static_cast<IntIntRatioStatistic*>(
            theStatisticManager->GetStatistic(STATISTIC_HIT_RATIO + levelName, "IntIntRatioStatistic"));
        levelHitRatio->SetGlobalFlag(false);
        levelHitRatio->IncreaseDenominator(1);
    }

    CVector Gun::_CalcDirForNextShot() const
    {
        auto const mat = GetMatrixForShot(m_curBarrelIndex);
        CVector result;
        result.x = (float)((float)(mat._31 + mat._11) * 0.0) + mat._21;
        result.y = (float)((float)(mat._32 + mat._12) * 0.0) + mat._22;
        result.z = (float)((float)(mat._33 + mat._13) * 0.0) + mat._23;
        return result;
    }

    void Gun::_InternalPostLoad()
    {
        if (thePlayer)
        {
            // TODO: check IE_EV_SM_OBJECT_CREATED
            theProcessManager->PostMessageA(
                GE_SUBSCRIBE, thePlayer->GetId(), GetId(), 0.0, IE_EV_SM_OBJECT_CREATED, {}, 1);
        }
    }

    CVector Gun::_CalcRoughPosForNextShot() const
    {
        // RVA 0x6DF740 - the barrel's own origin when it has one, otherwise the gun's.
        m3d::SgNode* const node = m_barrelNode ? m_barrelNode : m_Node;
        if (node)
        {
            return node->GetOriginWorldAbs();
        }
        return PhysicBody::GetPosition();
    }

    bool Gun::_bIsVolleyFiring() const
    {
        // RVA 0x2DE150
        return false;
    }

    Gun::~Gun() = default;

    CVector Gun::_CalcPosForNextShot() const
    {
        auto const mat = GetMatrixForShot(m_curBarrelIndex);
        return mat.getOrg();
    }

    bool Gun::_GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const
    {
        // RVA 0x6E3BF0
        switch (propertyId)
        {
            case 28:
                retVal = m_damage;
                return true;
            case 29:
                retVal = m_firingRate;
                return true;
            case 30:
                retVal = m_firingRange;
                return true;
            case 34:
                retVal = m_ChargeSize;
                return true;
            case 35:
                retVal = m_ReChargingTime;
                return true;
            case 36:
                retVal = m_ShellsInPool;
                return true;
            default:
                return VehiclePart::_GetPropertyInternal(propertyId, retVal);
        }
    }

    void Gun::_CreateBarrelNode()
    {
        // TODO: implement Gun::_CreateBarrelNode
        if (!m_modelname.empty())
        {
            auto const* prototypeInfo = GetPrototypeInfo();
            CVector scale(1.0, 1.0, 1.0);

            m_barrelNode = CreateNode(prototypeInfo->m_barrelModelName.c_str(), 0, scale, this, false);
            if (!m_barrelNode)
            {
                M3D_CRITICAL_ERROR("Couldn't create barrel for " + GetDebugDescription());
            }

            m_Node->AddChild(m_barrelNode);

            auto* serverAnimatedModels = static_cast<m3d::AnimatedModelsServer*>(&M3D_APP->GetAnimatedModelsServer());

            CMatrix boneMat;
            auto const boneRes =
                serverAnimatedModels->GetBoneMatrixByNameFromModelName(m_modelname.c_str(), "LP_GUN", boneMat, false);
            if (!boneRes)
            {
                M3D_LOG_ERR("Error: LoadPoint not found: LP_GUN for " + GetDebugDescription());
                boneMat.identity();
            }

            Quaternion rot;
            rot.FromMatrix(boneMat);
            m_barrelNode->SetRotation(rot);

            CVector const origin = boneMat.getOrg();
            m_barrelNode->SetOriginAbs(origin);

            m_barrelNode->UpdateXForm(false, true);
            m_barrelNode->SetName(m_Node->GetName() + CStr("Brl"));

            if (fabs(prototypeInfo->m_highStopAngle - prototypeInfo->m_lowStopAngle) < 0.0099999998)
            {
                // A barrel that cannot elevate never moves relative to the gun, so its collision shapes are
                // baked into the gun's own hull at the barrel's fixed place instead of being simulated apart.
                retruxx::vector<CollisionInfo> barrelCollisionInfos;
                int sh = -1;
                m_barrelNode->GetProperty(4360u, &sh);
                ai::GetCollisionInfoByServerHandle(sh, barrelCollisionInfos, m_bCollisionTrimeshAllowed);

                CMatrix const rotMatrix = rot.ToMatrix();
                for (auto& collisionInfo : barrelCollisionInfos)
                {
                    Quaternion relRotation = rot;
                    relRotation *= collisionInfo.m_relRotation;
                    collisionInfo.m_relRotation = relRotation;
                    collisionInfo.m_relTranslation = rotMatrix.vecRot(collisionInfo.m_relTranslation) + origin;
                }

                m_collisionInfos.insert(
                    m_collisionInfos.end(), barrelCollisionInfos.begin(), barrelCollisionInfos.end());
                ChangePhysicBodyByCollisionInfo(m_collisionInfos);
            }
        }
    }

    void Gun::_OnCinematic(Event const&)
    {
        // RVA 0x6DE160 - a gun caught mid-burst when a cutscene starts is told to stop.
        if (!m_Node)
        {
            return;
        }
        int action = 0;
        m_Node->GetProperty(8704u, &action);
        if (action)
        {
            SetNodeAction(0, true);
            m_bIsFiring = false;
        }
    }

    m3d::Object* Gun::CreateObject()
    {
        // RVA 0x6DFE60
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void Gun::_GetCurrentOffsetAngles(float& alpha, float& beta) const
    {
        // RVA 0x6DE7F0 - where the gun is pointing right now: beta from the barrel node's own
        // rotation, alpha from the whole gun's direction in its owner's frame.
        CMatrix const barrelMatrix = m_barrelNode->GetRotation().ToMatrix();

        float dummy = 0.0f;
        barrelMatrix.getYPR(dummy, beta, dummy);
        beta = -beta;

        CVector const oldRelDir = GetNodeRelativeDirection();
        alpha = atan2(oldRelDir.x, oldRelDir.z);
    }

    CMatrix Gun::_CalcMatrixForNextShot() const
    {
        // RVA 0x6E1D40
        return GetMatrixForShot(m_curBarrelIndex);
    }

    void Gun::BeginReCharge()
    {
        auto const* prototypeInfo = GetPrototypeInfo();
        if (prototypeInfo->m_WithCharging)
        {
            if (!prototypeInfo->m_WithShellsPoolLimit || m_ShellsInPool)
            {
                m_ChargeState = csInCharging;
                m_CurrentReChargingTime = 0.0;

                Obj* parentObj = this;
                while (!IS_KIND_OF(parentObj, Vehicle))
                {
                    parentObj = parentObj->GetParent();
                    if (!parentObj)
                        return;
                }
                if (parentObj)
                {
                    auto* vehicle = RT_DYNCAST(parentObj, Vehicle);
                    if (vehicle->bIsControlledByPlayer())
                    {
                        vehicle->PlaySoundOnRechargeWeapon();
                    }
                }
            }
            else
            {
                m_ChargeState = csReady;
            }
        }
        else
        {
            m_ShellsInCurrentCharge = m_ChargeSize;
        }
    }

    unsigned Gun::getShellsForRecharge() const
    {
        // RVA 0x2DF530
        GunPrototypeInfo const* prototypeInfo = GetPrototypeInfo();
        unsigned int shellsNeeded = m_ChargeSize - m_ShellsInCurrentCharge;

        if (!prototypeInfo->m_WithShellsPoolLimit)
        {
            // No shell pool limit - can replenish all needed shells
            return shellsNeeded;
        }

        // With shell pool limit - can only replenish up to available shells in pool
        return (shellsNeeded < m_ShellsInPool) ? shellsNeeded : m_ShellsInPool;
    }

    void Gun::_GetOffsetAngles(CVector const& lookAt, float elapsedTime, float& alpha, float& beta)
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
        float invLength = 1.0f /
            std::sqrt(targetDir.x * targetDir.x + targetDir.y * targetDir.y + targetDir.z * targetDir.z +
                      1.1920929e-7f);
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
            Quaternion const& barrelRot = m_barrelNode->GetRotation();

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
        return m_firingRate > 299.89999;
    }

    void Gun::_UpdateNodeFiringAction()
    {
        // RVA 0x2DE1A0
        if (m_Node)
        {
            int action = 0;
            m_Node->GetProperty(m3d::PROP_DM_ACTION, &action);
            if (_bIsUsingVolley())
            {
                if (_bIsVolleyFiring())
                {
                    if (action != 2)
                    {
                        SetNodeAction(2, 1);
                    }
                    return;
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
            else if (
                m_bWasShot && (m_timeFromLastShot >= (float)(60.0 / m_firingRate) || m_ChargeState == csInCharging))
            {
                m_bWasShot = 0;
                SetNodeAction(0, 1);
            }
        }
    }

    m3d::Object* Gun::Clone()
    {
        // RVA 0x6DFCA0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

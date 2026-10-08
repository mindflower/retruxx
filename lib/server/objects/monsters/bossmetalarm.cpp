#include "bossmetalarm.h"

#include <cmath>

#include "m3dapp.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "core/log.h"
#include "scene/nodes/sgnode.h"
#include "scene/servers/dataserver.h"
#include "server/dynamicscene.h"
#include "server/processmanager.h"
#include "server/relationship.h"
#include "server/utils.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/monsters/bossmetalarmload.h"
#include "server/objects/physicbodies/physichelpers.h"
#include "server/objects/physicbodies/simplephysicbody.h"

RT_CLASS_EXPORT_METHOD_DEFINE(BossMetalArm, Die)
{
    // RVA 0x731E40
    static_cast<ai::BossMetalArm*>(context->asObject(0, "BossMetalArm"))->Die();
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BossMetalArm)
    RT_CLASS_EXPORT(BossMetalArm, m3d::METHOD, Die, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BossMetalArm);

    namespace
    {
        // RVA 0x5CCAF0 - rand() scaled by RAND_MAX+1, so the top bound is exclusive.
        unsigned IntRandom(unsigned highBound)
        {
            return highBound * rand() / 0x8000;
        }

        // v rotated by q (row vector times q's rotation matrix), as expanded inline in the
        // shipped code.
        CVector RotateByQuaternion(CVector const& v, Quaternion const& q)
        {
            float const xz = q.x * q.z;
            float const xw = q.x * q.w;
            float const xx = q.x * q.x;
            float const xy = q.x * q.y;
            float const zy = q.z * q.y;
            float const yy = q.y * q.y;
            float const zz = q.z * q.z;
            float const yw = q.y * q.w;
            float const zw = q.z * q.w;
            float const m11 = 1.0f - (zz + yy) * 2.0f;
            float const m21 = (xy - zw) * 2.0f;
            float const m31 = (yw + xz) * 2.0f;
            float const m12 = (zw + xy) * 2.0f;
            float const m22 = 1.0f - (zz + xx) * 2.0f;
            float const m32 = (zy - xw) * 2.0f;
            float const m13 = (xz - yw) * 2.0f;
            float const m23 = (xw + zy) * 2.0f;
            float const m33 = 1.0f - (yy + xx) * 2.0f;
            return CVector(
                (v.x * m11 + m21 * v.y) + m31 * v.z,
                (v.x * m12 + m22 * v.y) + m32 * v.z,
                (v.x * m13 + m23 * v.y) + m33 * v.z);
        }
    }  // namespace

    BossMetalArmPrototypeInfo::AttackActionInfo::AttackActionInfo()
    {
        // RVA 0x7312B0
        m_frameToReleaseLoad = 0;
        m_action = AT_STAND1;
    }

    void BossMetalArmPrototypeInfo::AttackActionInfo::LoadFromXML(m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x731590
        m3d::SafeIntAttrib(m_frameToReleaseLoad, xmlNode, "FrameToReleaseLoad");
        CStr strAction;
        m3d::SafeStrAttrib(strAction, xmlNode, "Action");
        m_action = m3d::GetActionByName(strAction.c_str());
    }

    BossMetalArmPrototypeInfo::BossMetalArmPrototypeInfo()
    {
        // RVA 0x733820
        m_turningSpeed = 0.5f;
        m_frameToPickUpLoad = 0;
        m_lpIdForLoad = -1;
        m_numExplodedLoadsToDie = 1;
    }

    void BossMetalArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x732490
        // The attack animations chain on to STAND2, which is held; the arm watches for it to
        // tell when an attack or a pick-up has played out.
        SimplePhysicObjPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
        m3d::DataServer* server = &M3D_APP->GetAnimatedModelsServer();
        int const itemId = server->GetItemByName(GetEngineModelName().c_str(), true);
        if (itemId == -1)
        {
            return;
        }
        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(itemId, 16394, &mdl);
        // NOTE: the model is used without a null check.
        for (auto const& attack : m_attacks)
        {
            mdl->SetNextForAnimation(attack.m_action, 1);
        }
        CStr strLoadPoint;
        m3d::SafeStrAttrib(strLoadPoint, xmlNode, "LoadPointForLoad");
        m_lpIdForLoad = mdl->GetLoadPointIdByName(strLoadPoint.c_str());
        mdl->SetNextForAnimation(AT_STAND2, -1);
    }

    bool BossMetalArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7339B0
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (!result)
        {
            return result;
        }
        _SetGeomType(GEOM_TYPE_FROM_MODEL);
        m3d::SafeStrAttrib(m_explosionEffectName, xmlNode, "ExplosionEffect");
        m3d::SafeFloatAttrib(m_turningSpeed, xmlNode, "TurningSpeed");
        m3d::SafeIntAttrib(m_frameToPickUpLoad, xmlNode, "FrameToPickUpLoad");

        ref_ptr attacksNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(attacksNode, "AttackActions");
        m_attacks.clear();
        if (!attacksNode->IsEmpty())
        {
            ref_ptr attackNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            attacksNode->GetFirstChild(attackNode, "Attack");
            while (!attackNode->IsEmpty())
            {
                AttackActionInfo attackInfo;
                attackInfo.LoadFromXML(attackNode);
                m_attacks.push_back(attackInfo);
                attackNode->GetNextSibling(attackNode, "Attack");
            }
        }
        if (m_attacks.empty())
        {
            SYS_ERROR("!m_attacks.empty()");
        }

        CStr strLoadPrototypes;
        m3d::SafeStrAttrib(strLoadPrototypes, xmlNode, "LoadPrototypes");
        m_loadPtototypeNames.clear();
        m3d::Tokenize(strLoadPrototypes, m_loadPtototypeNames, "(), ;\t");
        m3d::SafeIntAttrib(m_numExplodedLoadsToDie, xmlNode, "NumExplodedLoadsToDie");
        return true;
    }

    void BossMetalArmPrototypeInfo::PostLoad()
    {
        // RVA 0x733010
        // NOTE: does not chain to the base PostLoad.
        for (auto const& name : m_loadPtototypeNames)
        {
            m_loadProrotypeIds.push_back(thePrototypeManager->GetPrototypeId(name));
            if (m_loadProrotypeIds.back() == -1)
            {
                SYS_ERROR("m_loadProrotypeIds.back() != INVALID_ID");
            }
            // NOTE: an unknown prototype is dereferenced here after the assertion above.
            if (thePrototypeManager->GetPrototypeInfo(m_loadProrotypeIds.back())->m_className != "BossMetalArmLoad")
            {
                SYS_ERROR("G_PROTOTYPE_MANAGER.GetPrototypeInfo( m_loadProrotypeIds.back() )->m_className == \"BossMetalArmLoad\"");
            }
        }
        if (m_loadProrotypeIds.empty())
        {
            SYS_ERROR("!m_loadProrotypeIds.empty()");
        }
    }

    Obj* BossMetalArmPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7321E0
        return new BossMetalArm(*this);
    }

    m3d::Class* BossMetalArm::GetBaseClass()
    {
        // RVA 0x731290
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    BossMetalArm::BossMetalArm(BossMetalArmPrototypeInfo const& prototype) : SimplePhysicObj(prototype), m_turningSpeed(prototype.m_turningSpeed)
    {
        // RVA 0x731620
        m_loadObjId = -1;
        m_attackState = ATTACK_IDLE;
        m_dirForCharging = ZeroVector;
        m_curAttackAction = -1;
        m_numExplodedLoads = 0;
        m_bCurLoadExploded = false;
        PhysicObj::DisablePhysics();
    }

    void BossMetalArm::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D01B0
        SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_loadObjId, xmlNode, "LoadId");
        m3d::SafeEnumAttrib(m_attackState, xmlNode, "AttackState");
        m3d::SafeIntAttrib(m_curAttackAction, xmlNode, "CurAttackAction");
        m3d::SafeIntAttrib(m_numExplodedLoads, xmlNode, "NumExplodedLoads");
        m3d::SafeBoolAttrib(m_bCurLoadExploded, xmlNode, "CurLoadExploded");
    }

    bool BossMetalArm::CanChildBeAdded(m3d::Class*) const
    {
        // RVA 0x7312D0
        return false;
    }

    BossMetalArmPrototypeInfo const* BossMetalArm::GetPrototypeInfo() const
    {
        // RVA 0x732880
        return static_cast<BossMetalArmPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    float BossMetalArm::GetMaxHealth() const
    {
        // RVA 0x7312E0 - the arm's health is counted in loads blown up in its hand.
        BossMetalArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (!prototypeInfo)
        {
            return 0.0f;
        }
        return static_cast<float>(prototypeInfo->m_numExplodedLoadsToDie);
    }

    float BossMetalArm::GetHealth() const
    {
        // RVA 0x731300
        BossMetalArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        float maxHealth = 0.0f;
        if (prototypeInfo)
        {
            maxHealth = static_cast<float>(prototypeInfo->m_numExplodedLoadsToDie);
        }
        return maxHealth - static_cast<float>(m_numExplodedLoads);
    }

    int BossMetalArm::OnEvent(Event const& evn)
    {
        // RVA 0x734340
        int const result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_OBJECT_DIE)
        {
            _OnObjectDie(evn);
            return 1;
        }
        return result;
    }

    void BossMetalArm::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x732210
        SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("LoadId", CStr(m_loadObjId).c_str());
        xmlNode->SetAttribute("AttackState", CStr(static_cast<int>(m_attackState)).c_str());
        xmlNode->SetAttribute("CurAttackAction", CStr(m_curAttackAction).c_str());
        xmlNode->SetAttribute("NumExplodedLoads", CStr(m_numExplodedLoads).c_str());
        xmlNode->SetAttribute("CurLoadExploded", CStr(static_cast<int>(m_bCurLoadExploded)).c_str());
    }

    m3d::Class* BossMetalArm::GetClass() const
    {
        // RVA 0x7312A0
        return RT_CLASS_LOCAL(BossMetalArm);
    }

    void BossMetalArm::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x733E20
        // Idle until the player's hostile vehicle comes within 100 m, wind up, then loop:
        // charging swings back to the resting direction and picks up a load at
        // FrameToPickUpLoad, attacking turns to the player and throws it at FrameToReleaseLoad.
        SimplePhysicObj::Update(elapsedTime, workTime);
        if (!IsAlive())
        {
            return;
        }
        Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle || theRelationship->CheckTolerance(GetBelong(), playerVehicle->GetBelong()) >= RS_NEUTRAL)
        {
            return;
        }
        CVector const pos = GetPosition();
        CVector const playerPos = playerVehicle->GetPosition();
        CVector toPlayer(playerPos.x - pos.x, playerPos.y - pos.y, playerPos.z - pos.z);

        switch (m_attackState)
        {
        case ATTACK_IDLE:
            if (toPlayer.length() < 100.0)
            {
                _SetAttackState(ATTACK_NOTICED_PLAYER);
            }
            break;

        case ATTACK_NOTICED_PLAYER:
        {
            _TurnToDir(m_dirForCharging, elapsedTime);
            m3d::AnimatedModel::Animation const* const curAnimation = GetNodeAnimInfo(m_physicBody->m_Node)->GetCurAnimation();
            if (_GetCurAnimationFrame() > curAnimation->m_numFrames - 10)
            {
                _SetAttackState(ATTACK_CHARGING);
            }
            break;
        }

        case ATTACK_CHARGING:
        {
            _TurnToDir(m_dirForCharging, elapsedTime);
            BossMetalArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
            if (m_loadObjId == -1 && !m_bCurLoadExploded && _GetCurAnimationFrame() > prototypeInfo->m_frameToPickUpLoad)
            {
                unsigned const idx = IntRandom(static_cast<unsigned>(prototypeInfo->m_loadProrotypeIds.size()));
                m_loadObjId = theObjects->CreateNewObject(prototypeInfo->m_loadProrotypeIds[idx], "", -1, GetBelong());
                theProcessManager->PostMessageA(
                    GE_SUBSCRIBE, m_loadObjId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
                // NOTE: the new object is used without a null or type check.
                static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_loadObjId))->DisablePhysics();
            }
            if (m_loadObjId != -1)
            {
                _PlaceLoadOnLoadpoint();
            }
            if (m_physicBody->GetNodeRealAnimAction() != AT_STAND2)
            {
                _SetAttackState(ATTACK_ATTACKING);
            }
            break;
        }

        case ATTACK_ATTACKING:
        {
            _TurnToDir(toPlayer, elapsedTime);
            if (m_loadObjId != -1)
            {
                _PlaceLoadOnLoadpoint();
            }
            BossMetalArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
            if (m_loadObjId != -1 && _GetCurAnimationFrame() > prototypeInfo->m_attacks[m_curAttackAction].m_frameToReleaseLoad)
            {
                // Throw: the load leaves at 50 m/s straight at the player and explodes on contact.
                // NOTE: assumed to be a BossMetalArmLoad without a type check.
                auto* const load = static_cast<BossMetalArmLoad*>(theObjects->GetEntityByObjId(m_loadObjId));
                if (load)
                {
                    load->EnablePhysics();
                    CVector const loadPos = load->GetPosition();
                    CVector const target = playerVehicle->GetPosition();
                    CVector const dir = CVector(target.x - loadPos.x, target.y - loadPos.y, target.z - loadPos.z).getNormalized();
                    load->SetLinearVelocity(CVector(dir.x * 50.0f, dir.y * 50.0f, dir.z * 50.0f));
                    load->m_collisionMode = BossMetalArmLoad::COLLIDE_EXPLODE;
                }
                theProcessManager->PostMessageA(
                    GE_UNSUBSCRIBE, m_loadObjId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
                m_loadObjId = -1;
            }
            if (m_physicBody->GetNodeRealAnimAction() == AT_STAND2)
            {
                _SetAttackState(ATTACK_CHARGING);
            }
            break;
        }

        default:
            break;
        }
    }

    void BossMetalArm::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D0090
        // The placed direction, flattened, is where the arm rests while charging.
        // NOTE: calls Obj::LoadFromXML directly, skipping the physic object classes.
        Obj::LoadFromXML(xmlFile, xmlNode);
        m_dirForCharging = GetDirection();
        m_dirForCharging.y = 0.0f;
        SetDirection(m_dirForCharging);
    }

    void BossMetalArm::Die()
    {
        // RVA 0x731BB0
        PhysicBody::CreateEffectNode(GetPrototypeInfo()->m_explosionEffectName, GetPosition(), IdentityQuaternion, true, 1.0f);
        Remove();
        m_physicBody->SetNodeEffectAction(8);
        // NOTE: the broken object is built from the arm's own model name.
        CreateBrokenObj(this, GetPrototypeInfo()->GetEngineModelName(), m_physicBody->m_Node);
        m_physicBody->m_Node = nullptr;
    }

    void BossMetalArm::SetPositionSelf(CVector const& pos)
    {
        // RVA 0x7316D0 - the arm stands sunk 3.5 m into the ground.
        CVector const groundPos = GetGroundPos(pos, true, false);
        PhysicObj::SetPositionSelf(CVector(groundPos.x, groundPos.y - 3.5f, groundPos.z));
    }

    BossMetalArm::~BossMetalArm()
    {
        // RVA 0x7312C0
    }

    void BossMetalArm::_TurnToDir(CVector const& desiredDir, float elapsedTime)
    {
        // RVA 0x731750
        // Turns the arm in the horizontal plane towards desiredDir by at most
        // TurningSpeed * elapsedTime radians.
        float invLen = static_cast<float>(1.0 / sqrt(desiredDir.z * desiredDir.z + desiredDir.x * desiredDir.x + 1.1920929e-7));
        CVector const realDesiredDir(desiredDir.x * invLen, invLen * 0.0f, desiredDir.z * invLen);
        CVector dir = GetDirection();
        invLen = static_cast<float>(1.0 / sqrt(dir.z * dir.z + dir.x * dir.x + 1.1920929e-7));
        dir = CVector(dir.x * invLen, invLen * 0.0f, dir.z * invLen);

        CVector const cross(
            realDesiredDir.z * dir.y - dir.z * realDesiredDir.y,
            dir.z * realDesiredDir.x - realDesiredDir.z * dir.x,
            dir.x * realDesiredDir.y - realDesiredDir.x * dir.y);
        float const crossLenSq = cross.z * cross.z + cross.y * cross.y + cross.x * cross.x;
        if (crossLenSq < 0.001)
        {
            return;
        }
        float const invCross = static_cast<float>(1.0 / sqrt(crossLenSq + 1.1920929e-7));
        CVector const axis(cross.x * invCross, cross.y * invCross, cross.z * invCross);
        float const maxStep = m_turningSpeed * elapsedTime;
        float const angle = static_cast<float>(acos(dir.z * realDesiredDir.z + dir.x * realDesiredDir.x + dir.y * realDesiredDir.y));
        float const halfAngle = (angle <= maxStep ? angle : maxStep) * 0.5f;
        float const s = static_cast<float>(sin(halfAngle));
        Quaternion const q(axis.x * s, axis.y * s, axis.z * s, static_cast<float>(cos(halfAngle)));
        SetDirection(RotateByQuaternion(dir, q));
    }

    int BossMetalArm::_GetCurAnimationFrame() const
    {
        // RVA 0x731730
        return GetNodeAnimInfo(m_physicBody->m_Node)->CurAnimFrame();
    }

    m3d::Object* BossMetalArm::CreateObject()
    {
        // RVA 0x732020
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* BossMetalArm::Clone()
    {
        // RVA 0x731E60
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void BossMetalArm::_SetAttackState(AttackState newState)
    {
        // RVA 0x732580
        m_attackState = newState;
        // NOTE: the result is discarded.
        m_physicBody->GetNodeRealAnimAction();
        m_curAttackAction = -1;
        switch (m_attackState)
        {
        case ATTACK_NOTICED_PLAYER:
            M3D_LOG_INFO(CStr("Boss noticed player"));
            if (!GetNodeAnimInfo(m_physicBody->m_Node))
            {
                M3D_LOG_ERR(CStr("Error: BossMetalArm has no animation"));
                _SetAttackState(ATTACK_CHARGING);
            }
            break;

        case ATTACK_CHARGING:
            M3D_LOG_INFO(CStr("Boss is charging"));
            m_physicBody->SetNodeAction(AT_STAND2, false);
            m_bCurLoadExploded = false;
            break;

        case ATTACK_ATTACKING:
        {
            M3D_LOG_INFO(CStr("Boss is attacking"));
            BossMetalArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
            m_curAttackAction = static_cast<int>((static_cast<unsigned>(prototypeInfo->m_attacks.size()) * rand()) >> 15);
            m_physicBody->SetNodeAction(prototypeInfo->m_attacks[m_curAttackAction].m_action, false);
            break;
        }

        default:
            break;
        }
    }

    void BossMetalArm::_PlaceLoadOnLoadpoint()
    {
        // RVA 0x7328B0 - carries the load at the arm's load point.
        PhysicObj* load = nullptr;
        if (m_loadObjId >= 0)
        {
            load = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_loadObjId));
        }
        if (!load)
        {
            m_loadObjId = -1;
            return;
        }
        int const lpId = GetPrototypeInfo()->m_lpIdForLoad;
        CMatrix mat(GetNodeAnimInfo(m_physicBody->m_Node)->GetCurrentLoadpointMatrix(lpId));
        mat = mat * m_physicBody->m_Node->m_currentXForm;
        load->SetPosition(CVector(mat._41, mat._42, mat._43));
        Quaternion rot;
        rot.FromMatrix(mat);
        load->SetRotation(rot);
    }

    void BossMetalArm::_OnObjectDie(Event const& evn)
    {
        // RVA 0x734270
        // The carried load blew up in the arm's hand; enough of them and the arm is destroyed.
        theProcessManager->PostMessageA(
            GE_UNSUBSCRIBE, evn.m_senderObjId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
        ++m_numExplodedLoads;
        m_loadObjId = -1;
        m_bCurLoadExploded = true;
        if (m_numExplodedLoads >= GetPrototypeInfo()->m_numExplodedLoadsToDie)
        {
            Die();
        }
    }
}  // namespace ai

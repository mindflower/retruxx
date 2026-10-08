#include "bossarm.h"

#include <cmath>

#include "config.h"
#include "skelmodel.h"
#include "m3dapp.h"
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

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BossArm)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BossArm);

    namespace
    {
        CVector const INITIAL_OBJECTS_DIRECTION(0.0f, 0.0f, 1.0f);

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

    void BossArmPrototypeInfo::AttackActionInfo::LoadFromXML(m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D0F10
        m3d::SafeIntAttrib(m_frameToReleaseLoad, xmlNode, "FrameToReleaseLoad");
        CStr strAction;
        m3d::SafeStrAttrib(strAction, xmlNode, "Action");
        m_action = m3d::GetActionByName(strAction.c_str());
    }

    BossArmPrototypeInfo::AttackActionInfo::AttackActionInfo()
    {
        // RVA 0x8D0CA0
        m_frameToReleaseLoad = 0;
        m_action = AT_STAND1;
    }

    void BossArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D1690
        // The attack animations chain on to STAND2, which is held; the arm watches for it to
        // tell when an attack or a pick-up has played out.
        VehiclePartPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
        m3d::DataServer* server = &M3D_APP->GetAnimatedModelsServer();
        int const itemId = server->GetItemByName(m_engineModelName.c_str(), true);
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

    BossArmPrototypeInfo::BossArmPrototypeInfo()
    {
        // RVA 0x8D29D0
        m_frameToPickUpLoad = 0;
        m_turningSpeed = 0.5f;
        m_lpIdForLoad = -1;
        m_cruticalNumExplodedLoads = 1;
    }

    Obj* BossArmPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x8D2790
        return new BossArm(*this);
    }

    bool BossArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D2A20
        bool const result = VehiclePartPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (!result)
        {
            return result;
        }
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
        m3d::SafeIntAttrib(m_cruticalNumExplodedLoads, xmlNode, "CriticalNumExplodedLoads");
        return true;
    }

    m3d::Class* BossArm::GetClass() const
    {
        // RVA 0x8D0C90
        return RT_CLASS_LOCAL(BossArm);
    }

    BossArmPrototypeInfo const* BossArm::GetPrototypeInfo() const
    {
        // RVA 0x8D1820
        return static_cast<BossArmPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    BossArm::BossArm(BossArmPrototypeInfo const& prototype) : VehiclePart(prototype), m_turningSpeed(prototype.m_turningSpeed)
    {
        // RVA 0x8D1850
        m_attackState = ATTACK_IDLE;
        m_loadObjId = -1;
        m_dirForCharging = INITIAL_OBJECTS_DIRECTION;
        m_numExplodedLoads = 0;
        m_bCurLoadExploded = false;
        m_curAttackAction = -1;
        m_curLoadVelocity = ZeroVector;
    }

    int BossArm::GetNumExploadedLoads() const
    {
        // RVA 0x8D0D10
        return m_numExplodedLoads;
    }

    void BossArm::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x8D3120
        // Charging: swing back to the resting direction and pick up a fresh load once the
        // animation reaches FrameToPickUpLoad. Attacking: turn to the player and throw the load
        // at FrameToReleaseLoad of the current attack. Nothing happens unless the player's
        // vehicle is hostile.
        VehiclePart::Update(elapsedTime, workTime);
        Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle || theRelationship->CheckTolerance(GetBelong(), playerVehicle->GetBelong()) >= RS_NEUTRAL)
        {
            return;
        }
        CVector const pos = GetPosition();
        CVector const playerPos = playerVehicle->GetPosition();
        CVector toPlayer(playerPos.x - pos.x, playerPos.y - pos.y, playerPos.z - pos.z);

        if (m_attackState == ATTACK_CHARGING)
        {
            toPlayer = RotateByQuaternion(m_dirForCharging, GetOwner()->GetRotation());
            _TurnToDir(toPlayer, elapsedTime);
            BossArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
            if (m_loadObjId == -1)
            {
                if (!m_bCurLoadExploded && _GetCurAnimationFrame() > prototypeInfo->m_frameToPickUpLoad)
                {
                    if (m_loadProrotypeIds.empty())
                    {
                        SYS_ERROR("!m_loadProrotypeIds.empty()");
                    }
                    // NOTE: carries on after the assertion, indexing the empty vector.
                    int const loadPrototypeId = m_loadProrotypeIds[IntRandom(static_cast<unsigned>(m_loadProrotypeIds.size()))];
                    m_loadObjId = theObjects->CreateNewObject(loadPrototypeId, "", -1, GetBelong());
                    theProcessManager->PostMessageA(
                        GE_SUBSCRIBE, m_loadObjId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
                    // NOTE: the new object is used without a null or type check.
                    static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_loadObjId))->DisablePhysics();
                }
            }
            if (m_loadObjId != -1)
            {
                _PlaceLoadOnLoadpoint(elapsedTime);
            }
            if (_GetNodeRealAnimAction() != AT_STAND2)
            {
                _SetAttackState(ATTACK_ATTACKING);
            }
        }
        else if (m_attackState == ATTACK_ATTACKING)
        {
            _TurnToDir(toPlayer, elapsedTime);
            if (m_loadObjId != -1)
            {
                _PlaceLoadOnLoadpoint(elapsedTime);
            }
            BossArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
            if (m_loadObjId != -1 && _GetCurAnimationFrame() > prototypeInfo->m_attacks[m_curAttackAction].m_frameToReleaseLoad)
            {
                // Throw: the load leaves at 50 m/s straight at the player and explodes on contact.
                // NOTE: assumed to be a BossMetalArmLoad without a type check.
                auto* const load = static_cast<BossMetalArmLoad*>(theObjects->GetEntityByObjId(m_loadObjId));
                if (load)
                {
                    load->EnablePhysics();
                    load->SetLinearVelocity(m_curLoadVelocity);
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
            if (_GetNodeRealAnimAction() == AT_STAND2)
            {
                _SetAttackState(ATTACK_CHARGING);
            }
        }
    }

    void BossArm::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D2C20
        VehiclePart::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_loadObjId, xmlNode, "LoadId");
        m3d::SafeEnumAttrib(m_attackState, xmlNode, "AttackState");
        m3d::SafeIntAttrib(m_curAttackAction, xmlNode, "CurAttackAction");
        m3d::SafeIntAttrib(m_numExplodedLoads, xmlNode, "NumExplodedLoads");
        m3d::SafeBoolAttrib(m_bCurLoadExploded, xmlNode, "CurLoadExploded");
        m3d::SafeVectorAttrib(m_curLoadVelocity, xmlNode, "CurLoadVelocity");
        CStr strLoadPrototypes;
        m3d::SafeStrAttrib(strLoadPrototypes, xmlNode, "LoadPrototypes");
        retruxx::vector<CStr> loadPrototypeNames;
        StrToStringVector(strLoadPrototypes, loadPrototypeNames);
        m_loadProrotypeIds.clear();
        m_loadProrotypeIds.reserve(loadPrototypeNames.size());
        for (auto const& name : loadPrototypeNames)
        {
            m_loadProrotypeIds.push_back(thePrototypeManager->GetPrototypeId(name));
        }
    }

    void BossArm::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D0CB0
        // The placed direction, flattened, is where the arm rests while charging.
        // NOTE: calls Obj::LoadFromXML directly, skipping the physic body's own loading.
        Obj::LoadFromXML(xmlFile, xmlNode);
        m_dirForCharging = GetDirection();
        m_dirForCharging.y = 0.0f;
        SetNodeAbsoluteDirection(m_dirForCharging);
    }

    bool BossArm::CanChildBeAdded(m3d::Class*) const
    {
        // RVA 0x807470
        return false;
    }

    int BossArm::OnEvent(Event const& evn)
    {
        // RVA 0x8D3960
        int const result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_OBJECT_DIE)
        {
            _OnObjectDie(evn);
            return 1;
        }
        return result;
    }

    void BossArm::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8D2EA0
        VehiclePart::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("LoadId", CStr(m_loadObjId).c_str());
        xmlNode->SetAttribute("AttackState", CStr(static_cast<int>(m_attackState)).c_str());
        xmlNode->SetAttribute("CurAttackAction", CStr(m_curAttackAction).c_str());
        xmlNode->SetAttribute("NumExplodedLoads", CStr(m_numExplodedLoads).c_str());
        xmlNode->SetAttribute("CurLoadExploded", CStr(static_cast<int>(m_bCurLoadExploded)).c_str());
        xmlNode->SetAttribute("CurLoadVelocity", CStr(m_curLoadVelocity).c_str());
        retruxx::vector<CStr> loadPrototypeNames;
        loadPrototypeNames.reserve(m_loadProrotypeIds.size());
        for (int const id : m_loadProrotypeIds)
        {
            loadPrototypeNames.push_back(thePrototypeManager->GetPrototypeName(id));
        }
        if (!loadPrototypeNames.empty())
        {
            xmlNode->SetAttribute("LoadPrototypes", StringVectorToStr(loadPrototypeNames).c_str());
        }
    }

    m3d::Class* BossArm::GetBaseClass()
    {
        // RVA 0x8D0C80
        return RT_CLASS_LOCAL(VehiclePart);
    }

    void BossArm::_PlaceLoadOnLoadpoint(float elapsedTime)
    {
        // RVA 0x8D1B30
        // Carries the load at the arm's load point, tracking its velocity so a throw inherits
        // the swing.
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
        CMatrix mat(GetNodeAnimInfo(m_Node)->GetCurrentLoadpointMatrix(lpId));
        mat = mat * m_Node->m_currentXForm;
        CVector const oldPos = load->GetPosition();
        CVector const newPos(mat._41, mat._42, mat._43);
        if (elapsedTime > 0.001)
        {
            float const invDt = 1.0f / elapsedTime;
            m_curLoadVelocity = CVector((mat._41 - oldPos.x) * invDt, (mat._42 - oldPos.y) * invDt, (mat._43 - oldPos.z) * invDt);
        }
        load->SetPosition(newPos);
        Quaternion rot;
        rot.FromMatrix(mat);
        load->SetRotation(rot);
    }

    void BossArm::_SetAttackState(BossArm::AttackState newState)
    {
        // RVA 0x8D1950
        m_attackState = newState;
        m_curAttackAction = -1;
        if (newState == ATTACK_IDLE)
        {
            // Drop whatever is being carried.
            if (m_loadObjId >= 0)
            {
                // NOTE: assumed to be a BossMetalArmLoad without a type check.
                auto* const load = static_cast<BossMetalArmLoad*>(theObjects->GetEntityByObjId(m_loadObjId));
                if (load)
                {
                    load->EnablePhysics();
                    load->m_collisionMode = BossMetalArmLoad::COLLIDE_EXPLODE;
                }
            }
            m_loadObjId = -1;
        }
        else if (newState == ATTACK_CHARGING)
        {
            M3D_LOG_INFO(CStr("Boss is charging"));
            SetNodeAction(AT_STAND2, false);
            m_bCurLoadExploded = false;
        }
        else if (newState == ATTACK_ATTACKING)
        {
            M3D_LOG_INFO(CStr("Boss is attacking"));
            BossArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
            // NOTE: with no attacks configured this indexes an empty vector.
            m_curAttackAction = static_cast<int>((static_cast<unsigned>(prototypeInfo->m_attacks.size()) * rand()) >> 15);
            SetNodeAction(prototypeInfo->m_attacks[m_curAttackAction].m_action, false);
        }
    }

    BossArm::AttackState BossArm::_GetAttackState() const
    {
        // RVA 0x807400
        return m_attackState;
    }

    void BossArm::_SetLoadPrototypeIds(retruxx::vector<int, retruxx::allocator<int>> const& loadPrototypeIds)
    {
        // RVA 0x8D2090
        m_loadProrotypeIds = loadPrototypeIds;
    }

    BossArm::~BossArm()
    {
        // RVA 0x8D1780
    }

    void BossArm::_TurnToDir(CVector const& desiredDir, float elapsedTime)
    {
        // RVA 0x8D20A0
        // Turns the arm in the horizontal plane towards desiredDir by at most
        // TurningSpeed * elapsedTime radians.
        float invLen = static_cast<float>(1.0 / sqrt(desiredDir.z * desiredDir.z + desiredDir.x * desiredDir.x + 1.1920929e-7));
        CVector const realDesiredDir(desiredDir.x * invLen, invLen * 0.0f, desiredDir.z * invLen);
        CVector dir = GetNodeAbsoluteDirection();
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
        SetNodeAbsoluteDirection(RotateByQuaternion(dir, q));
        CommonGeomMovedCallback(m_pGeoms.front()->GetGeomId());
    }

    CVector const& BossArm::_GetCurLoadVelocity() const
    {
        // RVA 0x807410
        return m_curLoadVelocity;
    }

    m3d::Object* BossArm::CreateObject()
    {
        // RVA 0x8D13A0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* BossArm::Clone()
    {
        // RVA 0x8D11E0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    void BossArm::_OnObjectDie(Event const& evn)
    {
        // RVA 0x8D3820
        // The carried load blew up; enough of them in a row and the boss is told.
        theProcessManager->PostMessageA(
            GE_UNSUBSCRIBE, evn.m_senderObjId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
        ++m_numExplodedLoads;
        m_loadObjId = -1;
        m_bCurLoadExploded = true;
        if (m_numExplodedLoads >= GetPrototypeInfo()->m_cruticalNumExplodedLoads)
        {
            m_numExplodedLoads = 0;
            CauseEvent(GE_BOSS_CRITICAL_LOADS_EXPLODED, 0.0f, m3d::AIParam(), m3d::AIParam());
        }
    }
}  // namespace ai

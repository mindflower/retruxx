#include "boss02.h"

#include <cmath>

#include "m3dapp.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "core/log.h"
#include "scene/servers/dataserver.h"
#include "server/dynamicscene.h"
#include "server/event.h"
#include "server/utils.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/monsters/boss02arm.h"
#include "server/objects/monsters/bossmetalarmload.h"
#include "server/objects/physicbodies/physichelpers.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Boss02, NextState)
{
    // RVA 0x736640
    static_cast<ai::Boss02*>(context->asObject(0, "Boss02"))->NextState();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss02, SetJustMovedContainerToBlock)
{
    // RVA 0x736660
    static_cast<ai::Boss02*>(context->asObject(0, "Boss02"))->SetJustMovedContainerToBlock();
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss02)
    RT_CLASS_EXPORT(Boss02, m3d::METHOD, NextState, "", "", "")
    RT_CLASS_EXPORT(Boss02, m3d::METHOD, SetJustMovedContainerToBlock, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss02);

    namespace
    {
        // The boss is a platform that drives between the states' positions and an arm on top
        // that throws loads at the player.
        CStr const PLATFORM("PLATFORM");
        CStr const ARM("ARM");

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

    void Boss02PrototypeInfo::StateInfo::SaveToXML(m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x734AF0
        xmlNode->SetAttribute("LoadPrototypes", StringVectorToStr(m_loadPrototypeNames).c_str());
        xmlNode->SetAttribute("RelPos", CStr(m_position).c_str());
    }

    void Boss02PrototypeInfo::StateInfo::PostLoad()
    {
        // RVA 0x735D80
        m_loadPrototypeIds.clear();
        for (auto const& name : m_loadPrototypeNames)
        {
            m_loadPrototypeIds.push_back(thePrototypeManager->GetPrototypeId(name));
        }
    }

    Boss02PrototypeInfo::StateInfo::StateInfo(StateInfo const& other) :
        m_loadPrototypeIds(other.m_loadPrototypeIds),
        m_position(other.m_position),
        m_loadPrototypeNames(other.m_loadPrototypeNames)
    {
        // RVA 0x736A10
    }

    Boss02PrototypeInfo::StateInfo::StateInfo() : m_position(ZeroVector)
    {
        // RVA 0x736970
    }

    void Boss02PrototypeInfo::StateInfo::LoadFromXML(m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x736BB0
        CStr strLoadPrototypeIds;
        m3d::SafeStrAttrib(strLoadPrototypeIds, xmlNode, "LoadPrototypes");
        m3d::Tokenize(strLoadPrototypeIds, m_loadPrototypeNames, "(), ;\t");
        m3d::SafeVectorAttrib(m_position, xmlNode, "RelPos");
    }

    bool Boss02PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7376A0
        bool const result = ComplexPhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (!result)
        {
            return result;
        }
        ref_ptr statesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(statesNode, "States");
        m_stateInfos.clear();
        if (!statesNode->IsEmpty())
        {
            ref_ptr stateNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            statesNode->GetFirstChild(stateNode, "State");
            while (!stateNode->IsEmpty())
            {
                StateInfo info;
                info.LoadFromXML(stateNode);
                m_stateInfos.push_back(info);
                stateNode->GetNextSibling(stateNode, "State");
            }
        }
        // NOTE: an empty state is always appended; the boss dies when it would move on from it.
        m_stateInfos.push_back(StateInfo());
        m3d::SafeFloatAttrib(m_speed, xmlNode, "Speed");
        m3d::SafeStrAttrib(m_containerPrototypeName, xmlNode, "ContainerPrototype");
        return true;
    }

    void Boss02PrototypeInfo::PostLoad()
    {
        // RVA 0x735E20
        ComplexPhysicObjPrototypeInfo::PostLoad();
        for (auto& stateInfo : m_stateInfos)
        {
            stateInfo.PostLoad();
        }
        m_containerPrototypeId = thePrototypeManager->GetPrototypeId(m_containerPrototypeName);
    }

    Boss02PrototypeInfo::Boss02PrototypeInfo()
    {
        // RVA 0x7375C0
        m_speed = 1.0f;
        m_containerPrototypeId = -1;
        m_relPosForContainerPickUp = ZeroVector;
        m_relRotForContainerPickUp = IdentityQuaternion;
        m_relPosForContainerPutDown = ZeroVector;
    }

    void Boss02PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // RVA 0x735E70
        // Takes where the container is picked up (LP_LOAD01) and put down (LP_LOAD02) from the
        // platform's model.
        // NOTE: does not chain to the base RefreshFromXml.
        auto const it = m_partPrototypeIds.find(PLATFORM);
        if (it == m_partPrototypeIds.end())
        {
            SYS_ERROR("toPartPrototypeId != m_partPrototypeIds.end()");
        }
        // NOTE: carries on with the end iterator after the assertion.
        auto const* const platformInfo = static_cast<PhysicBodyPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(it->second));
        CStr const& modelName = platformInfo->m_engineModelName;
        m3d::DataServer* server = &M3D_APP->GetAnimatedModelsServer();
        int const itemId = server->GetItemByName(modelName.c_str(), true);
        if (itemId == -1)
        {
            return;
        }
        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(itemId, 16394, &mdl);
        if (!mdl)
        {
            return;
        }
        CMatrix mat;
        if (mdl->GetBoneMatrixByName(CStr("LP_LOAD01"), mat, false))
        {
            m_relPosForContainerPickUp = CVector(mat._41, mat._42, mat._43);
            m_relRotForContainerPickUp.FromMatrix(mat);
        }
        else
        {
            M3D_LOG_ERR(CStr("Error: LoadPoint not found: LP_LOAD01 for model '") + modelName + CStr("'"));
        }
        if (mdl->GetBoneMatrixByName(CStr("LP_LOAD02"), mat, false))
        {
            m_relPosForContainerPutDown = CVector(mat._41, mat._42, mat._43);
        }
        else
        {
            M3D_LOG_ERR(CStr("Error: LoadPoint not found: LP_LOAD02 for model '") + modelName + CStr("'"));
        }
    }

    Obj* Boss02PrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x7379D0
        return new Boss02(*this);
    }

    void Boss02::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x734D40
        ComplexPhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("NumState", CStr(m_numState).c_str());
        xmlNode->SetAttribute("MoveState", CStr(static_cast<int>(m_moveState)).c_str());
        xmlNode->SetAttribute("Velocity", CStr(m_velocity).c_str());
    }

    void Boss02::CreateChildren()
    {
        // RVA 0x7356E0 - the container the arm uses to block.
        ComplexPhysicObj::CreateChildren();
        int const containerId = theObjects->CreateNewObject(GetPrototypeInfo()->m_containerPrototypeId, "", -1, -1);
        if (containerId >= 0)
        {
            if (Obj* const container = theObjects->GetEntityByObjId(containerId))
            {
                AddChild(container);
            }
        }
    }

    void Boss02::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x734B70
        ComplexPhysicObj::SaveToXML(xmlFile, xmlNode);
        if (m_stateInfos.empty())
        {
            return;
        }
        ref_ptr statesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "States");
        xmlNode->AddChild(statesNode);
        for (auto const& stateInfo : m_stateInfos)
        {
            ref_ptr stateNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "State");
            statesNode->AddChild(stateNode);
            stateInfo.SaveToXML(stateNode);
        }
    }

    m3d::Class* Boss02::GetClass() const
    {
        // RVA 0x734380
        return RT_CLASS_LOCAL(Boss02);
    }

    Boss02::Boss02(Boss02PrototypeInfo const& prototype) : ComplexPhysicObj(prototype), m_stateInfos(prototype.m_stateInfos)
    {
        // RVA 0x7378D0
        m_numState = -2;
        m_moveState = MOVE_IDLE;
        m_containerId = -1;
        ComplexPhysicObj::DisablePhysics();
    }

    void Boss02::NextState()
    {
        // RVA 0x736400
        // State -2 waits for the player, -1 blocks the way with the container, and each state
        // after that drives to its position and attacks from there. Moving past the last state
        // kills the boss.
        int const numState = ++m_numState;
        if (numState == -1)
        {
            _GetArm()->BeginMovingContainerToBlock(m_containerId);
            return;
        }
        int const numStates = static_cast<int>(m_stateInfos.size());
        if (numState == numStates)
        {
            Die();
        }
        else if (numState <= numStates)
        {
            if (numState >= 0)
            {
                _SetMovePosition(m_stateInfos[numState].m_position);
            }
        }
        else
        {
            _SetDeadStatus();
        }
    }

    bool Boss02::CanChildBeAdded(m3d::Class* pClass) const
    {
        // RVA 0x734390
        return pClass->IsKindOf(RT_CLASS_LOCAL(BossMetalArmLoad));
    }

    int Boss02::OnEvent(Event const& evn)
    {
        // RVA 0x736680
        int const result = Obj::OnEvent(evn);
        if (evn.m_eventId == GE_BOSS_CRITICAL_LOADS_EXPLODED || evn.m_eventId == GE_BOSS_ARM_ACTION_FINISHED)
        {
            NextState();
            return 1;
        }
        return result;
    }

    void Boss02::SetJustMovedContainerToBlock()
    {
        // RVA 0x7364F0
        // Skips the blocking animation to 100 frames before the container is let go, starting the
        // block first if it has not begun.
        if (m_numState > -1)
        {
            return;
        }
        Boss02Arm* const arm = _GetArm();
        if (m_numState == -1)
        {
            if (GetNodeCurAnimationFrame(arm->m_Node) >= arm->GetPrototypeInfo()->m_frameToReleaseContainerForBlock - 100)
            {
                return;
            }
        }
        if (m_numState < -1)
        {
            NextState();
        }
        SetNodeCurAnimationFrame(arm->m_Node, arm->GetPrototypeInfo()->m_frameToReleaseContainerForBlock - 100);
    }

    void Boss02::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x7366B0
        // Wakes up when the player comes within 100 m and otherwise drives towards the current
        // state's position, stopping once it has passed it.
        // NOTE: calls PhysicObj::Update directly, skipping ComplexPhysicObj's.
        PhysicObj::Update(elapsedTime, workTime);
        Vehicle* const playerVehicle = gDynamicScene->GetVehicleControlledByPlayer();
        if (!playerVehicle)
        {
            return;
        }
        CVector const pos = GetPosition();
        CVector const playerPos = playerVehicle->GetPosition();
        if (m_numState == -2)
        {
            float const dx = playerPos.x - pos.x;
            float const dy = playerPos.y - pos.y;
            float const dz = playerPos.z - pos.z;
            if (sqrt(dz * dz + dy * dy + dx * dx) < 100.0)
            {
                NextState();
                return;
            }
        }
        if (m_moveState == MOVE_IDLE)
        {
            return;
        }
        if (m_moveState != MOVE_MOVING)
        {
            SYS_ERROR("0");
            return;
        }
        CVector prevPos = GetPosition();
        prevPos.y = 0.0f;
        CVector const step(m_velocity.x * elapsedTime, m_velocity.y * elapsedTime, m_velocity.z * elapsedTime);
        CVector const curPos = GetPosition();
        SetPosition(CVector(curPos.x + step.x, curPos.y + step.y, curPos.z + step.z));
        CVector newPos = GetPosition();
        newPos.y = 0.0f;
        CVector const& target = m_stateInfos[m_numState].m_position;
        // Passed the target once it lies between the old and the new position (in the ground
        // plane; the shipped code multiplies the height term by zero).
        if ((newPos.z - target.z) * (prevPos.z - target.z) + (newPos.x - target.x) * (prevPos.x - target.x) <= 0.0)
        {
            _SetMoveState(MOVE_IDLE);
        }
    }

    m3d::Class* Boss02::GetBaseClass()
    {
        // RVA 0x734370
        return RT_CLASS_LOCAL(ComplexPhysicObj);
    }

    Boss02PrototypeInfo const* Boss02::GetPrototypeInfo() const
    {
        // RVA 0x7356B0
        return static_cast<Boss02PrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    bool Boss02::RemoveChild(Obj* pChild)
    {
        // RVA 0x734FB0
        ComplexPhysicObj::RemoveChild(pChild);
        if (!pChild)
        {
            return false;
        }
        if (m_containerId == -1)
        {
            SYS_ERROR("m_containerId != INVALID_ID");
        }
        m_containerId = -1;
        pChild->m_parentId = -1;
        return true;
    }

    void Boss02::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x734CC0
        ComplexPhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_numState, xmlNode, "NumState");
        m3d::SafeEnumAttrib(m_moveState, xmlNode, "MoveState");
        m3d::SafeVectorAttrib(m_velocity, xmlNode, "Velocity");
    }

    float Boss02::GetHealth() const
    {
        // RVA 0x7352C0 - every state costs CriticalNumExplodedLoads loads blown up on the arm.
        Boss02Arm const* const arm = _GetArm();
        if (!arm)
        {
            return 0.0f;
        }
        BossArmPrototypeInfo const* const armInfo = arm->GetPrototypeInfo();
        if (!armInfo)
        {
            return 0.0f;
        }
        return GetMaxHealth() - static_cast<float>(arm->GetNumExploadedLoads() + armInfo->m_cruticalNumExplodedLoads * m_numState);
    }

    void Boss02::AddChild(Obj* pObj)
    {
        // RVA 0x734E10 - the only child is the container.
        ComplexPhysicObj::AddChild(pObj);
        if (!pObj)
        {
            return;
        }
        if (m_containerId != -1)
        {
            SYS_ERROR("m_containerId == INVALID_ID");
        }
        if (IsAlive())
        {
            // NOTE: the child is assumed to be a physic object without a type check.
            static_cast<PhysicObj*>(pObj)->DisablePhysics();
        }
        m_containerId = pObj->GetId();
        pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
    }

    void Boss02::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8D0820
        // A placed boss either lists its own states or uses the prototype's, which are relative
        // to the placement and are turned into world positions here.
        ComplexPhysicObj::LoadFromXML(xmlFile, xmlNode);
        // NOTE: the result is discarded.
        GetPrototypeInfo();
        CVector const pos = GetPosition();
        Quaternion const rot = GetRotation();
        ref_ptr statesNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlNode->GetFirstChild(statesNode, "States");
        if (statesNode->IsEmpty())
        {
            for (auto& stateInfo : m_stateInfos)
            {
                CVector const offset = RotateByQuaternion(stateInfo.m_position, rot);
                stateInfo.m_position = CVector(offset.x + pos.x, pos.y + offset.y, pos.z + offset.z);
            }
        }
        else
        {
            m_stateInfos.clear();
            ref_ptr stateNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
            statesNode->GetFirstChild(stateNode, "State");
            while (!stateNode->IsEmpty())
            {
                Boss02PrototypeInfo::StateInfo info;
                info.LoadFromXML(stateNode);
                info.PostLoad();
                m_stateInfos.push_back(info);
                stateNode->GetNextSibling(stateNode, "State");
            }
        }
        if (m_containerId == -1 && IsAlive())
        {
            int const containerId =
                theObjects->CreateNewObjectWithSuspendedPostLoad(GetPrototypeInfo()->m_containerPrototypeId, "", -1, -1);
            if (containerId >= 0)
            {
                if (Obj* const container = theObjects->GetEntityByObjId(containerId))
                {
                    AddChild(container);
                }
            }
        }
    }

    void Boss02::SetPositionSelf(CVector const& pos)
    {
        // RVA 0x7343B0
        PhysicObj::SetPositionSelf(GetGroundPos(pos, true, false));
    }

    float Boss02::GetMaxHealth() const
    {
        // RVA 0x735200
        // NOTE: counts the prototype's states, including the empty one appended to them.
        Boss02PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (!prototypeInfo)
        {
            return 0.0f;
        }
        Boss02Arm const* const arm = _GetArm();
        if (!arm)
        {
            return 0.0f;
        }
        BossArmPrototypeInfo const* const armInfo = arm->GetPrototypeInfo();
        if (!armInfo)
        {
            return 0.0f;
        }
        return static_cast<float>(static_cast<int>(prototypeInfo->m_stateInfos.size()) * armInfo->m_cruticalNumExplodedLoads);
    }

    void Boss02::Die()
    {
        // RVA 0x735400
        _GetArm()->BeginDie();
    }

    void Boss02::_InternalPostLoad()
    {
        // RVA 0x7365A0
        // NOTE: calls PhysicObj::_InternalPostLoad directly, skipping ComplexPhysicObj's.
        PhysicObj::_InternalPostLoad();
        if (theObjects->m_SaveType != ObjContainer::SAVE_FULL && m_containerId != -1)
        {
            _PlaceContainerToInitialPosition();
        }
        Boss02PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        _GetArm()->SetRelativePositionsForContainer(prototypeInfo->m_relPosForContainerPickUp, prototypeInfo->m_relPosForContainerPutDown);
    }

    void Boss02::_InternalCreateVisualPart()
    {
        // RVA 0x735750
        // A boss loaded already dead is shown in its final pose, past its last state, and its
        // container is removed.
        ComplexPhysicObj::_InternalCreateVisualPart();
        if (IsAlive() || theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            return;
        }
        _SetToDeadPose();
        m_numState = static_cast<int>(m_stateInfos.size()) + 1;
        if (m_containerId >= 0)
        {
            if (Obj* const container = theObjects->GetEntityByObjId(m_containerId))
            {
                container->Remove();
            }
        }
    }

    Boss02::~Boss02()
    {
        // RVA 0x737930
    }

    void Boss02::_SetMoveState(MoveState moveState)
    {
        // RVA 0x735450
        // Arriving stops the platform and starts the state's attack, or moves on at the last
        // state.
        m_moveState = moveState;
        if (moveState == MOVE_IDLE)
        {
            m_velocity = ZeroVector;
            _GetPlatform()->SetNodeAction(AT_STAND1, true);
            if (m_numState >= 0)
            {
                if (m_numState >= static_cast<int>(m_stateInfos.size()) - 1)
                {
                    NextState();
                }
                else
                {
                    _GetArm()->BeginAttack(m_stateInfos[m_numState].m_loadPrototypeIds);
                }
            }
        }
        else if (moveState == MOVE_MOVING)
        {
            _GetArm()->StopAttack();
            _GetArm()->BeginMove();
            _GetPlatform()->SetNodeAction(AT_MOVE1, true);
        }
        else
        {
            SYS_ERROR("0");
        }
    }

    void Boss02::_PlaceContainerToInitialPosition()
    {
        // RVA 0x735960 - onto the platform's pick-up load point.
        Boss02PrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        PhysicObj* container = nullptr;
        if (m_containerId >= 0)
        {
            container = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_containerId));
        }
        // NOTE: the container is used without a null check.
        CVector const offset = RotateByQuaternion(prototypeInfo->m_relPosForContainerPickUp, GetRotation());
        CVector const pos = GetPosition();
        container->SetPosition(CVector(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z));
        Quaternion const q = GetRotation();
        Quaternion const& r = prototypeInfo->m_relRotForContainerPickUp;
        container->SetRotation(Quaternion(
            q.y * r.z + q.w * r.x + r.w * q.x - q.z * r.y,
            q.z * r.x + q.w * r.y + q.y * r.w - r.z * q.x,
            q.z * r.w + q.w * r.z + q.x * r.y - q.y * r.x,
            q.w * r.w - r.x * q.x - r.y * q.y - r.z * q.z));
    }

    VehiclePart* Boss02::_GetPlatform()
    {
        // RVA 0x7343E0
        return GetPartByName(PLATFORM);
    }

    void Boss02::_OnBossCriticalLoadsExploded(Event const&)
    {
        // RVA 0x736620
        NextState();
    }

    m3d::Object* Boss02::CreateObject()
    {
        // RVA 0x734930
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    void Boss02::_SetMovePosition(CVector const& movePosition)
    {
        // RVA 0x7357F0
        // NOTE: a position within about 0.3 m of the platform is ignored, and the boss then waits
        // there with nothing to move it on.
        CVector const pos = GetPosition();
        float const dx = movePosition.x - pos.x;
        float const dz = movePosition.z - pos.z;
        if ((pos.z - movePosition.z) * (pos.z - movePosition.z) + (pos.x - movePosition.x) * (pos.x - movePosition.x) <= 0.1)
        {
            return;
        }
        float const invLen = static_cast<float>(1.0 / sqrt(dz * dz + dx * dx + 1.1920929e-7));
        float const speed = GetPrototypeInfo()->m_speed;
        m_velocity = CVector(invLen * dx * speed, invLen * 0.0f * speed, dz * invLen * speed);
        m_moveState = MOVE_MOVING;
        _GetArm()->StopAttack();
        _GetArm()->BeginMove();
        _GetPlatform()->SetNodeAction(AT_MOVE1, true);
    }

    m3d::Object* Boss02::Clone()
    {
        // RVA 0x734770
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    Boss02Arm* Boss02::_GetArm()
    {
        // RVA 0x735130
        return static_cast<Boss02Arm*>(GetPartByName(ARM));
    }

    Boss02Arm const* Boss02::_GetArm() const
    {
        // RVA 0x735170
        return static_cast<Boss02Arm const*>(GetPartByName(ARM));
    }

    void Boss02::_SetToDeadPose()
    {
        // RVA 0x7351B0
        _GetArm()->SetToDeadPose();
    }

    void Boss02::_OnBossArmActionFinished(Event const&)
    {
        // RVA 0x736630
        NextState();
    }
}  // namespace ai

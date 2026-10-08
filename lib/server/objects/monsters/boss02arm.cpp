#include "boss02arm.h"

#include "m3dapp.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "core/log.h"
#include "scene/servers/dataserver.h"
#include "server/processmanager.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/base/prototypemanager.h"
#include "server/objects/monsters/bossmetalarmload.h"
#include "server/objects/physicbodies/physichelpers.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Boss02Arm)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Boss02Arm);

    namespace
    {
        CVector const INITIAL_OBJECTS_DIRECTION(0.0f, 0.0f, 1.0f);

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

    bool Boss02ArmPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x807550
        bool const result = BossArmPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeIntAttrib(m_frameToPickUpContainerForBlock, xmlNode, "FrameToPickUpContainerForBlock");
            m3d::SafeIntAttrib(m_frameToReleaseContainerForBlock, xmlNode, "FrameToReleaseContainerForBlock");
            m3d::SafeIntAttrib(m_frameToPickUpContainerForDie, xmlNode, "FrameToPickUpContainerForDie");
            m3d::SafeIntAttrib(m_frameToReleaseContainerForDie, xmlNode, "FrameToReleaseContainerForDie");

            CStr strAction;
            m3d::SafeStrAttrib(strAction, xmlNode, "ActionForBlock");
            m_actionForBlock = m3d::GetActionByName(strAction.c_str());
            strAction = CStr("");
            m3d::SafeStrAttrib(strAction, xmlNode, "ActionForDie");
            m_actionForDie = m3d::GetActionByName(strAction.c_str());

            m3d::SafeStrAttrib(m_blockingContainerPrototypeName, xmlNode, "ContainerPrototype");
        }
        return result;
    }

    Boss02ArmPrototypeInfo::Boss02ArmPrototypeInfo()
    {
        // RVA 0x808240
        m_frameToPickUpContainerForBlock = 0;
        m_frameToReleaseContainerForBlock = 0;
        m_frameToPickUpContainerForDie = 0;
        m_frameToReleaseContainerForDie = 0;
        m_actionForBlock = AT_STAND1;
        m_actionForDie = AT_STAND1;
        m_blockingContainerPrototypeId = -1;
    }

    void Boss02ArmPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8076E0
        // The block and die animations are held on their last frame; the move animation
        // (ATTACK2) loops.
        BossArmPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);
        m3d::DataServer* server = &M3D_APP->GetAnimatedModelsServer();
        int const itemId = server->GetItemByName(m_engineModelName.c_str(), true);
        if (itemId == -1)
        {
            return;
        }
        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(itemId, 16394, &mdl);
        // NOTE: the model is used without a null check.
        mdl->SetNextForAnimation(m_actionForBlock, -1);
        mdl->SetNextForAnimation(m_actionForDie, -1);
        mdl->SetNextForAnimation(AT_ATTACK2, AT_ATTACK2);
    }

    void Boss02ArmPrototypeInfo::PostLoad()
    {
        // RVA 0x807440
        // NOTE: does not chain to the base PostLoad.
        m_blockingContainerPrototypeId = thePrototypeManager->GetPrototypeId(m_blockingContainerPrototypeName);
    }

    Obj* Boss02ArmPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x807EA0
        return new Boss02Arm(*this);
    }

    Boss02Arm::Boss02Arm(Boss02ArmPrototypeInfo const& prototype) : BossArm(prototype)
    {
        // RVA 0x807770
        m_customState = CUSTOM_NONE;
        m_containerId = -1;
        m_relPosForContainerPickUp = ZeroVector;
        m_relPosForContainerPutDown = ZeroVector;
        m_bEffectsEnabled = true;
        PhysicBody::SetNodeAction(AT_ATTACK2, true);
    }

    void Boss02Arm::Update(float elapsedTime, unsigned workTime)
    {
        // RVA 0x8082E0
        // The custom actions: while moving the arm faces forward; to block it picks up the
        // container at one frame of the block animation and lets go of it where it stands at
        // another; when dying it does the same but throws the container with the swing. Each
        // finished action is reported to the boss.
        BossArm::Update(elapsedTime, workTime);
        if (_GetAttackState() != ATTACK_CUSTOM)
        {
            return;
        }
        Boss02ArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        switch (m_customState)
        {
        case CUSTOM_MOVING:
            _TurnToDir(RotateByQuaternion(INITIAL_OBJECTS_DIRECTION, GetOwner()->GetRotation()), elapsedTime);
            break;

        case CUSTOM_MOVING_CONTAINER_TO_BLOCK:
            _TurnToDir(RotateByQuaternion(INITIAL_OBJECTS_DIRECTION, GetOwner()->GetRotation()), elapsedTime);
            if (m_loadObjId == -1 && _GetCurAnimationFrame() > prototypeInfo->m_frameToPickUpContainerForBlock)
            {
                m_loadObjId = m_containerId;
                // NOTE: assumed to be a BossMetalArmLoad without a null or type check.
                auto* const container = static_cast<BossMetalArmLoad*>(theObjects->GetEntityByObjId(m_containerId));
                container->SetCollisionMode(BossMetalArmLoad::COLLIDE_NORMAL);
                container->DisablePhysics();
            }
            if (m_loadObjId != -1)
            {
                _PlaceLoadOnLoadpoint(elapsedTime);
                if (m_loadObjId != -1 && _GetCurAnimationFrame() > prototypeInfo->m_frameToReleaseContainerForBlock)
                {
                    // NOTE: the container is left where it is, physics still disabled.
                    theProcessManager->PostMessageA(
                        GE_UNSUBSCRIBE, m_loadObjId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
                    m_loadObjId = -1;
                    _SetCustomState(CUSTOM_NONE);
                    CauseEvent(GE_BOSS_ARM_ACTION_FINISHED, 0.0f, m3d::AIParam(), m3d::AIParam());
                }
            }
            break;

        case CUSTOM_DYING:
            _TurnToDir(RotateByQuaternion(INITIAL_OBJECTS_DIRECTION, GetOwner()->GetRotation()), elapsedTime);
            if (m_loadObjId == -1 && _GetCurAnimationFrame() > prototypeInfo->m_frameToPickUpContainerForDie)
            {
                m_loadObjId = m_containerId;
                // NOTE: used without a null or type check.
                static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_containerId))->DisablePhysics();
            }
            if (m_loadObjId != -1)
            {
                _PlaceLoadOnLoadpoint(elapsedTime);
                if (m_loadObjId != -1 && _GetCurAnimationFrame() > prototypeInfo->m_frameToReleaseContainerForDie)
                {
                    // NOTE: assumed to be a BossMetalArmLoad without a null or type check.
                    auto* const container = static_cast<BossMetalArmLoad*>(theObjects->GetEntityByObjId(m_loadObjId));
                    container->EnablePhysics();
                    container->SetCollisionMode(BossMetalArmLoad::COLLIDE_NORMAL);
                    container->SetLinearVelocity(_GetCurLoadVelocity());
                    theProcessManager->PostMessageA(
                        GE_UNSUBSCRIBE, m_loadObjId, GetId(), 0.0f, m3d::AIParam(static_cast<int>(GE_OBJECT_DIE)), m3d::AIParam(), 1);
                    m_loadObjId = -1;
                    _SetCustomState(CUSTOM_NONE);
                    _SetEffectsDisabled();
                    CauseEvent(GE_BOSS_ARM_ACTION_FINISHED, 0.0f, m3d::AIParam(), m3d::AIParam());
                }
            }
            break;

        default:
            break;
        }
    }

    void Boss02Arm::BeginMovingContainerToBlock(int containerId)
    {
        // RVA 0x808010
        m_containerId = containerId;
        _SetCustomState(CUSTOM_MOVING_CONTAINER_TO_BLOCK);
    }

    void Boss02Arm::StopAttack()
    {
        // RVA 0x808130
        _SetLoadPrototypeIds(retruxx::vector<int>());
        _SetCustomState(CUSTOM_NONE);
        _SetAttackState(ATTACK_IDLE);
    }

    m3d::Class* Boss02Arm::GetBaseClass()
    {
        // RVA 0x807420
        return RT_CLASS_LOCAL(BossArm);
    }

    void Boss02Arm::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x807ED0
        BossArm::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeEnumAttrib(m_customState, xmlNode, "CustomState");
        m3d::SafeIntAttrib(m_containerId, xmlNode, "ContainerId");
    }

    void Boss02Arm::SetToDeadPose()
    {
        // RVA 0x808080
        // Jumps straight to the end of the die animation, for a boss loaded already dead.
        m_bEffectsEnabled = false;
        SetNodeEffectAction(9);
        _SetCustomState(CUSTOM_DYING);
        m3d::AnimInfo* const animInfo = GetNodeAnimInfo(m_Node);
        if (!animInfo)
        {
            return;
        }
        m3d::AnimatedModel::Animation const* const curAnimation = animInfo->GetCurAnimation();
        if (!curAnimation)
        {
            return;
        }
        SetNodeElapsedAnimationTimeInMs(m_Node, curAnimation->m_numFrames * curAnimation->m_fps);
        m_containerId = -1;
        _SetCustomState(CUSTOM_NONE);
    }

    void Boss02Arm::BeginDie()
    {
        // RVA 0x808070
        _SetCustomState(CUSTOM_DYING);
    }

    m3d::Class* Boss02Arm::GetClass() const
    {
        // RVA 0x807430
        return RT_CLASS_LOCAL(Boss02Arm);
    }

    void Boss02Arm::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x807F40
        // NOTE: calls Obj::SaveToXML directly, skipping the physic body's own saving.
        Obj::SaveToXML(xmlFile, xmlNode);
        if (!m_bEffectsEnabled)
        {
            xmlNode->SetAttribute("EffectsEnabled", CStr(static_cast<int>(m_bEffectsEnabled)).c_str());
        }
    }

    Boss02ArmPrototypeInfo const* Boss02Arm::GetPrototypeInfo() const
    {
        // RVA 0x808100
        return static_cast<Boss02ArmPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Boss02Arm::BeginAttack(retruxx::vector<int, retruxx::allocator<int>> const& loadPrototypeIds)
    {
        // RVA 0x808030
        _SetLoadPrototypeIds(loadPrototypeIds);
        _SetCustomState(CUSTOM_NONE);
        _SetAttackState(ATTACK_ATTACKING);
    }

    void Boss02Arm::SetRelativePositionsForContainer(CVector const& relPosForPickUp, CVector const& relPosForPutDown)
    {
        // RVA 0x807480
        m_relPosForContainerPickUp = relPosForPickUp;
        m_relPosForContainerPutDown = relPosForPutDown;
    }

    void Boss02Arm::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x807820
        BossArm::LoadFromXML(xmlFile, xmlNode);
        m3d::SafeBoolAttrib(m_bEffectsEnabled, xmlNode, "EffectsEnabled");
        if (!m_bEffectsEnabled)
        {
            m_bEffectsEnabled = false;
            SetNodeEffectAction(9);
        }
    }

    void Boss02Arm::BeginMove()
    {
        // RVA 0x808060
        _SetCustomState(CUSTOM_MOVING);
    }

    void Boss02Arm::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x807F80
        BossArm::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("CustomState", CStr(static_cast<int>(m_customState)).c_str());
        xmlNode->SetAttribute("ContainerId", CStr(m_containerId).c_str());
    }

    void Boss02Arm::_InternalCreateVisualPart()
    {
        // RVA 0x8074C0
        VehiclePart::_InternalCreateVisualPart();
    }

    Boss02Arm::~Boss02Arm()
    {
        // RVA 0x807460
    }

    void Boss02Arm::_SetCustomState(CustomState customState)
    {
        // RVA 0x807870
        m_customState = customState;
        if (customState != CUSTOM_NONE)
        {
            _SetAttackState(ATTACK_CUSTOM);
        }
        Boss02ArmPrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        switch (m_customState)
        {
        case CUSTOM_MOVING:
            M3D_LOG_INFO(CStr("Boss begins moving"));
            _SetCustomNodeAction(AT_ATTACK2, false);
            break;
        case CUSTOM_MOVING_CONTAINER_TO_BLOCK:
            M3D_LOG_INFO(CStr("Boss begins moving container for block"));
            _SetCustomNodeAction(prototypeInfo->m_actionForBlock, false);
            break;
        case CUSTOM_DYING:
            M3D_LOG_INFO(CStr("Boss begins dying"));
            _SetCustomNodeAction(prototypeInfo->m_actionForDie, false);
            break;
        default:
            break;
        }
    }

    void Boss02Arm::_SetEffectsDisabled()
    {
        // RVA 0x8074F0
        m_bEffectsEnabled = false;
        SetNodeEffectAction(9);
    }

    void Boss02Arm::_SetCustomNodeAction(int action, bool forceRestartAction)
    {
        // RVA 0x8074D0 - with the effects off only the animation is switched.
        if (m_bEffectsEnabled)
        {
            SetNodeAction(action, forceRestartAction);
        }
        else
        {
            SetNodeAnimAction(action, forceRestartAction);
        }
    }

    m3d::Object* Boss02Arm::CreateObject()
    {
        // RVA 0x807CE0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Boss02Arm::Clone()
    {
        // RVA 0x807B20
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

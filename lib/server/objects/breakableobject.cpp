#include "breakableobject.h"

#include "base/prototypemanager.h"
#include <stdexcept>
#include <server/utils.h>
#include "ode/objects.h"
#include "ode/odecpp.h"
#include "lightobj.h"

#include "blastwave.h"
#include "ropeobj.h"
#include "base/compositeobj.h"
#include "base/jointedobj.h"
#include "base/objcontainer.h"
#include "physicbodies/boxybody.h"
#include "physicbodies/geoms/box.h"
#include "physicbodies/geoms/geomtransform.h"

#include <cmath>
#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>
#include <landscape.h>
#include <m3dapp.h>
#include <scene/nodes/sgnode.h>
#include <scene/servers/serveranimatedmodel.h>
#include <server/server.h>
#include <world.h>

RT_CLASS_EXPORT_METHOD_DEFINE(BreakableObject, SetEnabled)
{
    // RVA 0x855010
    auto* obj = (ai::BreakableObject*)context->asObject(0, "BreakableObject");
    obj->SetState(ai::BreakableObject::ENABLED);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(BreakableObject, SetDestroyed)
{
    // RVA 0x855030
    auto* obj = (ai::BreakableObject*)context->asObject(0, "BreakableObject");
    obj->SetState(ai::BreakableObject::REMOVED);
    return 1;
}

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(BreakableObject)
    RT_CLASS_EXPORT(BreakableObject, m3d::METHOD, SetEnabled, "", "", "")
    RT_CLASS_EXPORT(BreakableObject, m3d::METHOD, SetDestroyed, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(BreakableObject);

    Obj* BreakableObjectPrototypeInfo::CreateTargetObject() const
    {
        return new BreakableObject(*this);
    }

    BreakableObjectPrototypeInfo::BreakableObjectPrototypeInfo()
    {
        m_destroyable = 0;
        m_effectType = "WOOD";
        m_destroyEffectType = "BLOW";
        m_brokenModelName = "brokenTest";
        m_destroyedModelName = "brokenTest";
        m_BlastWavePrototypeId = -1;
        m_bIsUpdating = false;
    }

    void BreakableObjectPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x853B40 - the collision box sits on the ground: raised by half the model's height, unrotated.
        ai::SimplePhysicObjPrototypeInfo::RefreshFromXml(xmlFile, xmlNode);

        auto* serverAnimatedModels = (m3d::AnimatedModelsServer*)&M3D_APP->GetAnimatedModelsServer();
        auto const boundSize = serverAnimatedModels->GetBoundSizes(GetEngineModelName().c_str());

        auto p_m_relTranslation = &this->m_collisionInfos.front().m_relTranslation;
        p_m_relTranslation->x = 0.0;
        p_m_relTranslation->y = boundSize.y * 0.5;
        p_m_relTranslation->z = 0.0;

        auto p_m_relRotation = &this->m_collisionInfos.front().m_relRotation;
        p_m_relRotation->x = 0.0;
        p_m_relRotation->y = 0.0;
        p_m_relRotation->z = 0.0;
        p_m_relRotation->w = 1.0;
    }

    void BreakableObjectPrototypeInfo::PostLoad()
    {
        if (!m_BlastWavePrototypeName.empty())
        {
            m_BlastWavePrototypeId = ai::thePrototypeManager->GetPrototypeId(m_BlastWavePrototypeName);
        }
    }

    bool BreakableObjectPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeIntAttrib(m_destroyable, xmlNode, "Destroyable");
            m3d::SafeFloatAttrib(m_criticalHitEnergy, xmlNode, "CriticalHitEnergy");
            m3d::SafeStrAttrib(m_effectType, xmlNode, "EffectType");
            m3d::SafeStrAttrib(m_destroyEffectType, xmlNode, "DestroyEffectType");
            m3d::SafeStrAttrib(m_brokenModelName, xmlNode, "BrokenModel");
            m3d::SafeStrAttrib(m_destroyedModelName, xmlNode, "DestroyedModel");
            m3d::SafeStrAttrib(m_breakEffect, xmlNode, "BreakEffect");
            m3d::SafeStrAttrib(m_BlastWavePrototypeName, xmlNode, "BlastWave");
        }
        return result;
    }

    BreakableObject::BreakableObject(BreakableObjectPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        PhysicObj::DisablePhysics();
        m_state = DISABLED;
        m_destroyable = prototypeInfo.m_destroyable;
        m_criticalHitEnergy = prototypeInfo.m_criticalHitEnergy;
        m_effectType = gDynamicScene->GetBoEffectTypeByName(prototypeInfo.m_effectType);
        m_destroyEffectType = ai::gDynamicScene->GetBoEffectTypeByName(prototypeInfo.m_destroyEffectType);
        m_jointId = 0;
        m_bPositioningOnGround = true;
        m_causePos = ZeroVector;
        m_causeForce = 0.0;
        m_initVelocities = 0;
        _SetStaticCollision();
    }

    bool BreakableObject::CanChildBeAdded(m3d::Class* pClass) const
    {
        return ai::Obj::CanChildBeAdded(pClass) || pClass->IsKindOf(&ai::LightObj::m_classLightObj);
    }

    int BreakableObject::IsDestroyable()
    {
        // RVA 0x7DEF50
        return m_destroyable;
    }

    void BreakableObject::RenderDebugInfo() const
    {
        // RVA 0x8530E0 - only for objects without a node or whose node was visible this frame.
        m3d::SgNode const* const node = m_physicBody->m_Node;
        if (node && node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame())
        {
            return;
        }
        SimplePhysicObj::RenderDebugInfo();
        if (m_jointId)
        {
            pServer->GetWorld()->GetLandscape().DrawJoint(m_jointId);
        }
    }

    void BreakableObject::SetRemovingEffectName(CStr const& name)
    {
        // RVA 0x7DF130
        m_removingEffectName = name;
    }

    void BreakableObject::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x854DE0 - NOTE: loaded rope ids are added to m_connectedRopes without clearing it first.
        SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        int state = m_state;
        m3d::SafeIntAttrib(state, xmlNode, "State");
        m_state = static_cast<STATES>(state);
        int ropesNum = 0;
        m3d::SafeIntAttrib(ropesNum, xmlNode, "ConnectedRopesNum");
        for (int i = 0; i < ropesNum; ++i)
        {
            int ropeId = -1;
            m3d::SafeIntAttrib(ropeId, xmlNode, (CStr("ConnectedRope_") + CStr(i)).c_str());
            m_connectedRopes.insert(ropeId);
        }
    }

    void BreakableObject::SetCauseForce(float force)
    {
        // RVA 0x7DEFA0
        m_causeForce = force;
    }

    BoxyBody* BreakableObject::_BoxyBody()
    {
        // RVA 0x852CA0
        return static_cast<BoxyBody*>(m_physicBody);
    }

    void BreakableObject::SetEnabled()
    {
        // RVA 0x854FF0
        SetState(ENABLED);
    }

    bool BreakableObject::RemoveChild(Obj* pChild)
    {
        // RVA 0x853140 - reports success only for light children, whatever Obj::RemoveChild returned.
        Obj::RemoveChild(pChild);
        if (!pChild || !pChild->IsKindOf(&LightObj::m_classLightObj))
        {
            return false;
        }
        pChild->m_parentId = -1;
        return true;
    }

    CVector BreakableObject::GetGeometricCenter() const
    {
        // RVA 0x853670 - the position offset by the first collision box's translation, rotated with the object.
        CVector const& relTranslation = m_collisionInfos.front().m_relTranslation;
        CMatrix rot;
        rot.rotTranslate(GetRotation(), CVector(0.0f, 0.0f, 0.0f));
        float const x = relTranslation.y * rot._21 + relTranslation.z * rot._31 + rot._11 * relTranslation.x;
        float const y = relTranslation.y * rot._22 + relTranslation.z * rot._32 + rot._12 * relTranslation.x;
        float const z = relTranslation.y * rot._23 + relTranslation.z * rot._33 + rot._13 * relTranslation.x;
        CVector const pos = GetPosition();
        return CVector(pos.x + x, pos.y + y, pos.z + z);
    }

    bool BreakableObject::DisconnectFromRope(int ropeId)
    {
        // RVA 0x854FA0
        auto const it = m_connectedRopes.find(ropeId);
        if (it == m_connectedRopes.end())
        {
            return false;
        }
        m_connectedRopes.erase(it);
        return true;
    }

    BreakableObjectPrototypeInfo const* BreakableObject::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), BreakableObjectPrototypeInfo const);
    }

    void BreakableObject::SetCausePos(CVector const& pos)
    {
        // RVA 0x7DEF80
        m_causePos = pos;
    }

    float BreakableObject::GetCriticalHitEnergy()
    {
        // RVA 0x7DEF60
        return m_criticalHitEnergy;
    }

    void BreakableObject::SetPosition(CVector const& pos)
    {
        // RVA 0x853CF0 - moves the light children by the same offset.
        CVector const oldPos = GetPosition();
        SetPositionSelf(pos);
        CVector const groundPos = GetGroundPos(pos, false, false);
        CVector const newPos = GetPosition();
        double const dx = newPos.x - groundPos.x;
        double const dy = newPos.y - groundPos.y;
        double const dz = newPos.z - groundPos.z;
        if (std::sqrt(dz * dz + dy * dy + dx * dx) > 0.5)
        {
            M3D_LOG_INFO(CStr("### obj is sucking!!! ") + CStr(GetName()));
        }
        for (auto const& child : m_allChildren)
        {
            if (child.second->IsKindOf(&LightObj::m_classLightObj))
            {
                auto* const light = static_cast<SgNodeObj*>(child.second);
                CVector const lightPos = light->GetPosition();
                light->SetPosition(
                    CVector(pos.x + (lightPos.x - oldPos.x), pos.y + (lightPos.y - oldPos.y), pos.z + (lightPos.z - oldPos.z)));
            }
        }
    }

    void BreakableObject::DoTouch()
    {
        // RVA 0x852EF0 - a touched object that has come to rest is put to sleep and loses its joint.
        if ((GetPhysicState() & BODY_ENABLED_BIT) == 0)
        {
            return;
        }
        CVector const linearVelocity = GetLinearVelocity();
        if (linearVelocity.x * linearVelocity.x + linearVelocity.y * linearVelocity.y + linearVelocity.z * linearVelocity.z >= 0.0099999998f)
        {
            return;
        }
        CVector const angularVelocity = GetAngularVelocity();
        if (angularVelocity.x * angularVelocity.x + angularVelocity.y * angularVelocity.y + angularVelocity.z * angularVelocity.z >= 0.2f)
        {
            return;
        }
        DisablePhysicsWithAutoEnable();
        if (m_jointId)
        {
            dJointDestroy(m_jointId);
            m_jointId = nullptr;
        }
    }

    m3d::Class* BreakableObject::GetClass() const
    {
        return RT_CLASS_LOCAL(BreakableObject);
    }

    BreakableObject::STATES BreakableObject::GetState()
    {
        // RVA 0x8929D0
        return m_state;
    }

    unsigned short BreakableObject::GetDestroyEffectType()
    {
        // RVA 0x7DEF70
        return m_destroyEffectType;
    }

    void BreakableObject::SetJointAnchor(CVector const& vec)
    {
        // RVA 0x852FC0 - hinges an indestructible object to the world at vec, stiffly about the X and Z axes.
        if (m_destroyable || m_jointId)
        {
            return;
        }
        m_jointId = dJointCreateUniversal(gGlobalWorld, nullptr);
        dJointAttach(m_jointId, nullptr, m_body->id());
        dJointSetUniversalAnchor(m_jointId, vec.x, vec.y, vec.z);
        dJointSetUniversalAxis1(m_jointId, 1.0f, 0.0f, 0.0f);
        dJointSetUniversalAxis2(m_jointId, 0.0f, 0.0f, 1.0f);
        dJointSetUniversalParam(m_jointId, dParamBounce, 0.0f);
        dJointSetUniversalParam(m_jointId, dParamBounce2, 0.0f);
        dJointSetUniversalParam(m_jointId, dParamStopCFM, 0.0f);
        dJointSetUniversalParam(m_jointId, dParamStopCFM2, 0.0f);
        dJointSetUniversalParam(m_jointId, dParamStopERP, 0.89999998f);
        dJointSetUniversalParam(m_jointId, dParamStopERP2, 0.89999998f);
        dJointSetUniversalParam(m_jointId, dParamCFM, 0.0f);
        dJointSetUniversalParam(m_jointId, dParamCFM2, 0.0f);
    }

    BlastWave* BreakableObject::CreateBlastWave() const
    {
        // RVA 0x8549A0
        int const prototypeId = GetPrototypeInfo()->m_BlastWavePrototypeId;
        if (prototypeId == -1)
        {
            return nullptr;
        }
        int const objId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
        if (objId == -1)
        {
            return nullptr;
        }
        // NOTE: a stale id would give a null object, which is used without a check.
        auto* const blastWave = static_cast<BlastWave*>(theObjects->GetEntityByObjId(objId));
        blastWave->SetPosition(GetPosition());
        blastWave->SetEmitterId(GetId());
        blastWave->SetBelong(GetBelong());
        return blastWave;
    }

    CVector BreakableObject::GetBounds()
    {
        // RVA 0x853AE0 - the size of the physic body's first (box) geom.
        return static_cast<Box*>(m_physicBody->m_pGeoms.front()->GetGeom())->GetSize();
    }

    m3d::Class* BreakableObject::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    unsigned short BreakableObject::GetEffectType()
    {
        // RVA 0x8929C0
        return m_effectType;
    }

    void BreakableObject::AddChild(Obj* pObj)
    {
        Obj::AddChild(pObj);
        if (pObj)
        {
            if (pObj->IsKindOf(&ai::LightObj::m_classLightObj))
                pObj->LinkToParent(GetId(), HIERARCHY_CHILD);
        }
    }

    void BreakableObject::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8538F0
        SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("State", CStr(static_cast<int>(m_state)).c_str());
        if (m_connectedRopes.empty())
        {
            return;
        }
        xmlNode->SetAttribute("ConnectedRopesNum", CStr(static_cast<unsigned>(m_connectedRopes.size())).c_str());
        unsigned i = 0;
        for (int const ropeId : m_connectedRopes)
        {
            xmlNode->SetAttribute((CStr("ConnectedRope_") + CStr(i)).c_str(), CStr(ropeId).c_str());
            ++i;
        }
    }

    void BreakableObject::SetPositionSelf(CVector const& pos)
    {
        // RVA 0x852E80 - a sleeping object is dropped onto the ground, with its own geoms kept out of the ground probe.
        if ((GetPhysicState() & BODY_ENABLED_BIT) != 0 || !m_bPositioningOnGround)
        {
            PhysicObj::SetPositionSelf(pos);
            return;
        }
        bool const geomEnabled = GetGeomEnabledBit();
        if (geomEnabled)
        {
            _SetGeomEnabledBit(false);
        }
        PhysicObj::SetPositionSelf(GetGroundPos(pos, true, false));
        if (geomEnabled)
        {
            _SetGeomEnabledBit(true);
        }
    }

    void BreakableObject::SetState(STATES state)
    {
        // RVA 0x854AF0 - ENABLED breaks the object (into a broken model, or loose on a joint if indestructible);
        // REMOVED destroys it (into the destroyed model plus a blast wave, or just a removal effect).
        if (state == m_state)
        {
            return;
        }
        m_state = state;
        switch (state)
        {
        case DISABLED:
            SetPostDisablePhysics();
            break;

        case ENABLED:
            if (m_destroyable)
            {
                Remove();
                CompositeObj* const brokenObj = CreateBrokenObj(this, GetPrototypeInfo()->m_brokenModelName, nullptr);
                if (m_initVelocities)
                {
                    brokenObj->InitImpulses(m_causePos, m_causeForce);
                }
            }
            else
            {
                CVector const bounds = GetBounds();
                float const halfSize = (bounds.z > bounds.x ? bounds.z : bounds.x) * 0.5f;
                CVector const groundPos = GetGroundPos(GetPosition(), true, false);
                SetJointAnchor(CVector(groundPos.x, groundPos.y + halfSize, groundPos.z));
                SetPostEnablePhysics();
                _SetSimpleCollision();
            }
            if (!GetPrototypeInfo()->m_breakEffect.empty())
            {
                PhysicBody::CreateEffectNode(GetPrototypeInfo()->m_breakEffect, GetGeometricCenter(), GetRotation(), true, 1.0f);
            }
            _BreakOffConnectedRopes(true);
            _StifleChildLights();
            break;

        case REMOVED:
            Remove();
            if (m_destroyable)
            {
                CompositeObj* const brokenObj = CreateBrokenObj(this, GetPrototypeInfo()->m_destroyedModelName, nullptr);
                if (m_initVelocities)
                {
                    brokenObj->InitImpulses(m_causePos, m_causeForce);
                }
                if (!GetPrototypeInfo()->m_breakEffect.empty())
                {
                    PhysicBody::CreateEffectNode(GetPrototypeInfo()->m_breakEffect, GetGeometricCenter(), GetRotation(), true, 1.0f);
                }
                CreateBlastWave();
            }
            else
            {
                m3d::SgNode* const effectNode =
                    PhysicBody::CreateEffectNode(m_removingEffectName, GetGeometricCenter(), GetRotation(), true, 1.0f);
                if (effectNode)
                {
                    effectNode->Think(500, M3D_KERNEL->GetTimer().GetCurTime());
                }
            }
            _BreakOffConnectedRopes(false);
            _StifleChildLights();
            break;
        }
    }

    void BreakableObject::SetInitVelocities(bool initVelocities)
    {
        // RVA 0x7DEFC0
        m_initVelocities = initVelocities;
    }

    void BreakableObject::ConnectWithRope(int ropeId)
    {
        // RVA 0x854AD0
        m_connectedRopes.insert(ropeId);
    }

    bool BreakableObject::IsExplosive() const
    {
        // RVA 0x8929E0
        return GetPrototypeInfo()->m_BlastWavePrototypeId != -1;
    }

    void BreakableObject::SetDestroyed()
    {
        // RVA 0x855000
        SetState(REMOVED);
    }

    void BreakableObject::SetRotation(Quaternion const& rot)
    {
        // RVA 0x853F50 - light children keep their placement relative to the object.
        CMatrix mat;
        mat.rotTranslate(GetRotation(), GetPosition());
        PhysicObj::SetRotation(rot);
        for (auto const& child : m_allChildren)
        {
            if (!child.second->IsKindOf(&LightObj::m_classLightObj))
            {
                continue;
            }
            auto* const light = static_cast<SgNodeObj*>(child.second);
            CMatrix offset;
            offset.rotTranslate(light->GetRotation(), light->GetPosition());
            offset = offset * mat.getInverseRotTranslate();
            CMatrix final;
            final.rotTranslate(GetRotation(), GetPosition());
            final = offset * final;
            light->SetPosition(CVector(final._41, final._42, final._43));
            Quaternion lightRot;
            lightRot.FromMatrix(final);
            light->SetRotation(lightRot);
        }
    }

    void BreakableObject::_BreakOffConnectedRopes(bool createJoint)
    {
        // RVA 0x853BE0
        for (int const ropeId : m_connectedRopes)
        {
            Obj* const rope = theObjects->GetEntityByObjId(ropeId);
            if (!rope)
            {
                continue;
            }
            if (rope->IsKindOf(&RopeObj::m_classRopeObj))
            {
                static_cast<RopeObj*>(rope)->BreakOff(GetId(), createJoint);
            }
            else if (rope->IsKindOf(&JointedObj::m_classJointedObj))
            {
                static_cast<JointedObj*>(rope)->ReattachExternalJoint(GetId());
            }
        }
        m_connectedRopes.clear();
    }

    void BreakableObject::_StifleChildLights()
    {
        // RVA 0x854A50
        for (auto const& child : m_allChildren)
        {
            if (child.second->IsKindOf(&LightObj::m_classLightObj))
            {
                child.second->Remove();
            }
        }
    }

    void BreakableObject::_Construct()
    {
        SimplePhysicObj::_Construct();
        CVector massCenter;
        massCenter.x = 0.0;
        massCenter.y = m_collisionInfos.front().m_relTranslation.y * 0.5;
        massCenter.z = 0.0;
        _SetMassCenter(massCenter);
    }

    BreakableObject::~BreakableObject()
    {
        if (m_jointId)
        {
            dJointDestroy(m_jointId);
            m_jointId = nullptr;
        }
    }

    m3d::Object* BreakableObject::CreateObject()
    {
        // RVA 0x853340
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* BreakableObject::Clone()
    {
        // RVA 0x853180
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

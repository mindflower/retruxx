#include "wheel.h"

#include "vehicle.h"

#include <stdexcept>
#include <ode/objects.h>

#include "base/prototypemanager.h"
#include "physicbodies/sphericbody.h"
#include "core/log.h"
#include "ode/odecpp.h"
#include "scene/scenegraph.h"
#include "scene/servers/dataserver.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Wheel)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Wheel);

    WheelPrototypeInfo::WheelPrototypeInfo()
    {
        this->m_suspensionRange = 0.5f;
        this->m_suspensionCFM = 0.1f;
        this->m_suspensionERP = 0.80000001f;
        this->m_mU = 1.0f;
        this->m_typeName = "BIG";
        this->m_blowEffectName = "ET_PS_HARD_BLOW";
    }

    ai::Obj* WheelPrototypeInfo::CreateTargetObject() const
    {
        return new Wheel(*this);
    }

    bool WheelPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_SPHERE);
            m3d::SafeFloatAttrib(this->m_suspensionRange, xmlNode, "SuspensionRange");
            m3d::SafeStrAttrib(this->m_suspensionModelName, xmlNode, "SuspensionModelFile");
            m3d::SafeFloatAttrib(this->m_suspensionCFM, xmlNode, "SuspensionCFM");
            m3d::SafeFloatAttrib(this->m_suspensionERP, xmlNode, "SuspensionERP");
            m3d::SafeFloatAttrib(this->m_mU, xmlNode, "mU");
            m3d::SafeStrAttrib(this->m_typeName, xmlNode, "EffectType");
            m3d::SafeStrAttrib(this->m_blowEffectName, xmlNode, "BlowEffect");
        }
        return result;
    }

    void Wheel::BreakModel()
    {
        // RVA 0x5EF470 - a wheel has no per-mesh damage model. It simply steps
        // to the next whole configuration, so its visual damage is a fixed
        // sequence of ever more ruined wheels rather than anything derived from
        // where it was hit.
        m_bModelBroken = true;

        auto* node = m_physicBody->m_Node;
        if (!node)
        {
            return;
        }

        m3d::Configuration* cfg = nullptr;
        node->GetProperty(8707, &cfg);

        m3d::AnimatedModel* mdl = nullptr;
        node->GetServer()->GetItemProperty(node->GetServerHandle(), 16394, &mdl);
        if (!mdl)
        {
            return;
        }

        // A single configuration means there is no damaged variant to step to,
        // and the last one is as broken as the wheel gets.
        unsigned int const cfgSize = mdl->GetCfgSize();
        if (cfgSize == 1 || cfg->m_num == cfgSize - 1)
        {
            return;
        }

        CVector const breakNormal = GetDirection().getNormalized();

        // The effect is pushed half a radius out along the wheel's facing so it
        // sits on the tyre rather than inside the hub.
        float const radius = GetRadius();
        CVector offset;
        offset.x = breakNormal.x * radius * 0.5f;
        offset.y = breakNormal.y * radius * 0.5f;
        offset.z = breakNormal.z * radius * 0.5f;

        CVector const wheelPos = GetPosition();
        CVector breakPos;
        breakPos.x = wheelPos.x + offset.x;
        breakPos.y = wheelPos.y + offset.y;
        breakPos.z = wheelPos.z + offset.z;

        CMatrix rot;
        rot.lookAtLH(CVector(0.0f, 0.0f, 0.0f), breakNormal, CVector(0.0f, 1.0f, 0.0f));
        Quaternion q;
        q.FromMatrix(rot);
        PhysicBody::CreateEffectNode(CStr("ET_PS_VEH_PART_BROKEN"), breakPos, q, true, 1.0f);

        ++cfg->m_num;
        mdl->FromCfgNum(*cfg);
        mdl->CalculateMeshes(*cfg);
    }

    Wheel::Wheel(WheelPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        this->m_jointID = 0;
        this->m_driven = 1;
        this->m_steering = STEERING_NO;
        this->m_SplashEffect = 0;
        this->m_SplashType = 0;
        this->m_MakeSplash = 0;
        this->m_wheelType = ai::gDynamicScene->GetWheelTypeByName(prototypeInfo.m_typeName);
        this->m_bModelBroken = 0;
        this->m_suspensionNode = 0;
        this->m_curAngle = 0.0;
        this->m_initialRotation = {0.0, 0.0, 0.0, 1.0};
    }

    WheelPrototypeInfo const* Wheel::GetPrototypeInfo() const
    {
        return dynamic_cast<WheelPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    void Wheel::RelinkGeomsToCollisionCells()
    {
        SimplePhysicObj::RelinkGeomsToCollisionCells();
    }

    SphericBody const* Wheel::_SphericBody() const
    {
        // RVA 0x5CE7D0 - a wheel's body is always a SphericBody.
        return static_cast<SphericBody const*>(m_physicBody);
    }

    m3d::Class* Wheel::GetClass() const
    {
        return RT_CLASS_LOCAL(Wheel);
    }

    void Wheel::LinkGeomsToCollisionCells()
    {
        SimplePhysicObj::LinkGeomsToCollisionCells();
    }

    bool Wheel::AttachToPhysicObj(PhysicObj const* physicObj)
    {
        // RVA 0x5EEC30 - joins the wheel to the vehicle with a hinge-2 joint at the wheel's current position: axis 1
        // (steering/suspension) is the vehicle's up, axis 2 (spin) is AXIS_FOR_WHEEL in the vehicle's frame.
        if (!physicObj)
        {
            return false;
        }

        auto const* protoInfo = GetPrototypeInfo();
        LinkToParent(physicObj->GetId(), HIERARCHY_CHILD);
        m_jointID = dJointCreateHinge2(ai::gGlobalWorld, 0);
        dJointAttach(m_jointID, physicObj->GetBody()->id(), m_body->id());

        CVector const anchorPos = GetPosition();
        CMatrix rot;
        rot.rotTranslate(physicObj->GetRotation(), ZeroVector);
        dJointSetHinge2Axis1(m_jointID, rot._21, rot._22, rot._23);
        dJointSetHinge2Axis2(
            m_jointID,
            rot._31 * AXIS_FOR_WHEEL.z + rot._21 * AXIS_FOR_WHEEL.y + rot._11 * AXIS_FOR_WHEEL.x,
            rot._32 * AXIS_FOR_WHEEL.z + rot._22 * AXIS_FOR_WHEEL.y + rot._12 * AXIS_FOR_WHEEL.x,
            rot._33 * AXIS_FOR_WHEEL.z + rot._23 * AXIS_FOR_WHEEL.y + rot._13 * AXIS_FOR_WHEEL.x);
        dJointSetHinge2Anchor(m_jointID, anchorPos.x, anchorPos.y, anchorPos.z);
        dJointSetHinge2Param(m_jointID, dParamSuspensionCFM, protoInfo->m_suspensionCFM);
        dJointSetHinge2Param(m_jointID, dParamSuspensionERP, protoInfo->m_suspensionERP);
        dJointSetHinge2Param(m_jointID, dParamFMax, 1000000.0f);

        // A steering wheel may turn freely; the others are locked straight.
        dJointSetHinge2Param(m_jointID, dParamLoStop, m_steering ? -3.1415927f : 0.0f);
        dJointSetHinge2Param(m_jointID, dParamHiStop, m_steering ? 3.1415927f : 0.0f);
        return true;
    }

    float Wheel::GetWidth() const
    {
        return GetPrototypeInfo()->GetSize().x;
    }

    CVector Wheel::GetDirection() const
    {
        // RVA 0x5EE040 - the wheel's axle: AXIS_FOR_WHEEL turned by the wheel's rotation relative to its initial one.
        // NOTE: the shipped build inlines the product with its own summation order (last-bit differences possible).
        CMatrix rot;
        rot.rotTranslate(GetRotation() * m_initialRotation.getInversed(), ZeroVector);
        return CVector(
            rot._31 * AXIS_FOR_WHEEL.z + rot._21 * AXIS_FOR_WHEEL.y + rot._11 * AXIS_FOR_WHEEL.x,
            rot._32 * AXIS_FOR_WHEEL.z + rot._22 * AXIS_FOR_WHEEL.y + rot._12 * AXIS_FOR_WHEEL.x,
            rot._33 * AXIS_FOR_WHEEL.z + rot._23 * AXIS_FOR_WHEEL.y + rot._13 * AXIS_FOR_WHEEL.x);
    }

    void Wheel::CreateSuspensionNode()
    {
        const auto* protoInfo = GetPrototypeInfo();
        if (!protoInfo->m_suspensionModelName.empty())
        {
            CVector scale;
            scale.x = 1.0;
            scale.y = 1.0;
            scale.z = 1.0;

            m_suspensionNode = ai::PhysicBody::CreateNode(protoInfo->m_suspensionModelName, 0, scale, 0, 0);

            int mac = 1;
            m_suspensionNode->SetProperty(8716u, &mac);
        }
        else
        {
            M3D_LOG_INFO("Suspension model name not specified");
            M3D_ASSERT(0);
        }
    }

    void Wheel::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x5EEBA0
        SimplePhysicObj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("CurAngle", CStr(m_curAngle).c_str());
        xmlNode->SetAttribute("Broken", CStr(static_cast<int>(m_bModelBroken)).c_str());
    }

    bool Wheel::CanChildBeAdded(m3d::Class*) const
    {
        // RVA 0x5EDFA0 - a wheel never takes children of any kind.
        return false;
    }

    Vehicle* Wheel::GetVehicle() const
    {
        return RT_DYNCAST(GetParent(), Vehicle);
    }

    void Wheel::SetInitialRotation(Quaternion const& rot)
    {
        m_initialRotation = rot;
    }

    void Wheel::RenderDebugInfo() const
    {
        SimplePhysicObj::RenderDebugInfo();
    }

    void Wheel::Remove()
    {
        if (this->GetParentId() == -1)
        {
            ai::SimplePhysicObj::Remove();
        }
        else
        {
            M3D_LOG_INFO("Warning: attampt to remove " + GetDebugDescription() + ": it's attached to vehicle.");
        }
    }

    void Wheel::Update(float elapsedTime, unsigned workTime)
    {
        SimplePhysicObj::Update(elapsedTime, workTime);

        auto angualarVel = dBodyGetAngularVel(m_body->id());
        if (!m_MakeSplash && m_SplashEffect)
        {
            // Process child hierarchy using stack
            std::vector<m3d::SgNode*> stack;
            stack.push_back(m_SplashEffect);

            while (!stack.empty())
            {
                m3d::SgNode* current = stack.back();
                stack.pop_back();

                // Process all children of current node
                m3d::SgNode* grandChild = static_cast<m3d::SgNode*>(current->GetFirstChild());
                while (grandChild)
                {
                    grandChild->CanBeFree();

                    // If grandchild has children, add to stack for processing
                    if (grandChild->GetFirstChild())
                    {
                        stack.push_back(grandChild);
                    }

                    grandChild = static_cast<m3d::SgNode*>(grandChild->GetNextSibling());
                }
            }

            m_SplashEffect->GetGraph()->InsertInRemoveIfFree(m_SplashEffect);
            m_SplashEffect = nullptr;
        }
        m_MakeSplash = 0;
    }

    void Wheel::HealModel()
    {
        // RVA 0x5EE350 - puts a blown tyre back to its intact mesh by winding the model's
        // configuration back to variant 0.
        m_bModelBroken = false;

        auto* node = m_physicBody->m_Node;
        if (!node)
            return;

        m3d::Configuration* cfg = nullptr;
        node->GetProperty(m3d::PROP_DM_CFG, &cfg);

        m3d::AnimatedModel* mdl = nullptr;
        node->GetServer()->GetItemProperty(node->GetServerHandle(), m3d::PROP_INTERNAL_GETMODEL, &mdl);
        if (!mdl)
            return;

        // A model with a single variant has no intact/broken pair to switch between.
        if (mdl->GetCfgSize() == 1)
            return;

        if (cfg->m_num)
        {
            cfg->m_num = 0;
            mdl->FromCfgNum(*cfg);
            mdl->CalculateMeshes(*cfg);
        }
    }

    void Wheel::DetachFromPhysicObj()
    {
        if (m_jointID)
        {
            dJointDestroy(m_jointID);
            this->m_jointID = 0;
            SetParentInvalid();
        }
    }

    void Wheel::SetPassedToAnotherMapStatus()
    {
        // RVA 0x5EDDE0 - the splash and suspension nodes belong to the map being left, so
        // the wheel drops them rather than carrying dangling pointers across.
        SimplePhysicObj::SetPassedToAnotherMapStatus();
        m_SplashEffect = nullptr;
        m_MakeSplash = false;
        m_suspensionNode = nullptr;
    }

    void Wheel::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x5EDFB0
        SimplePhysicObj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeFloatAttrib(m_curAngle, xmlNode, "CurAngle");
        m3d::SafeBoolAttrib(m_bModelBroken, xmlNode, "Broken");
    }

    void Wheel::UnlinkGeomsFromCollisionCells()
    {
        SimplePhysicObj::UnlinkGeomsFromCollisionCells();
    }

    m3d::Class* Wheel::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    float Wheel::GetRadius() const
    {
        auto* sphere = RT_DYNCAST(m_physicBody->m_pGeoms.front()->GetGeom(), Sphere);
        return sphere->GetRadius();
    }

    void Wheel::_InternalCreateVisualPart()
    {
        ai::SimplePhysicObj::_InternalCreateVisualPart();
        if (this->m_bModelBroken)
            ai::Wheel::BreakModel();
    }

    Wheel::~Wheel()
    {
        if (m_jointID)
        {
            dJointDestroy(m_jointID);
            m_jointID = nullptr;
        }
        if (m_SplashEffect)
        {
            // Process child hierarchy using stack
            std::vector<m3d::SgNode*> stack;
            stack.push_back(m_SplashEffect);

            while (!stack.empty())
            {
                m3d::SgNode* current = stack.back();
                stack.pop_back();

                // Process all children of current node
                m3d::SgNode* grandChild = static_cast<m3d::SgNode*>(current->GetFirstChild());
                while (grandChild)
                {
                    grandChild->CanBeFree();

                    // If grandchild has children, add to stack for processing
                    if (grandChild->GetFirstChild())
                    {
                        stack.push_back(grandChild);
                    }

                    grandChild = static_cast<m3d::SgNode*>(grandChild->GetNextSibling());
                }
            }

            m_SplashEffect->GetGraph()->InsertInRemoveIfFree(m_SplashEffect);
            m_SplashEffect = nullptr;
        }
    }

    m3d::Object* Wheel::CreateObject()
    {
        // RVA 0x5EE5B0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* Wheel::Clone()
    {
        // RVA 0x5EE3F0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}

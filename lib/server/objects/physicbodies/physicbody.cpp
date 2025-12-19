#include "physicbody.h"

#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>
#include <ode/objects.h>

#include "config.h"
#include "core/kernel.h"
#include "game/m3dgame.h"
#include "geoms/box.h"
#include "math/vector.h"
#include "ode/odecpp.h"
#include "scene/scenegraph.h"
#include "scene/nodes/sgnode.h"
#include "scene/servers/DataServer.h"
#include <client.h>

#include "compoundvehiclepart.h"
#include "world.h"
#include "core/log.h"
#include "core/timer.h"
#include "geoms/cylinder.h"
#include "geoms/trimesh.h"
#include "server/dynamicscene.h"
#include <server/server.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(PhysicBody)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(PhysicBody);

    void PhysicBodyPrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        auto itemByName = M3D_APP->GetAnimatedModelsServer().GetItemByName(m_engineModelName.c_str(), 1);
        m_engineModelId = itemByName;
        ai::GetCollisionInfoByServerHandle(itemByName, m_collisionInfos, m_bCollisionTrimeshAllowed);
    }

    bool PhysicBodyPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m_engineModelName = xmlNode->GetAttribute("ModelFile");
            if (this->m_engineModelName.empty() && !IsPrototypeOf(&ai::CompoundVehiclePart::m_classCompoundVehiclePart))
            {
                M3D_CRITICAL_ERROR("no model file is provided for prototype '" + m_prototypeName + "'");
            }

            m3d::SafeBoolAttrib(this->m_bCollisionTrimeshAllowed, xmlNode, "CollisionTrimeshAllowed");
            m3d::SafeFloatAttrib(this->m_massValue, xmlNode, "Mass");

            if (this->m_massValue < 0.001)
            {
                M3D_CRITICAL_ERROR("mass is too low for '" + m_prototypeName + "'");
            }
        }
        return result;
    }

    PhysicBodyPrototypeInfo::PhysicBodyPrototypeInfo()
    {
        this->m_massValue = 1.0;
        this->m_bCollisionTrimeshAllowed = 0;
    }

    void PhysicBody::SetEffectActions(retruxx::vector<ActionType>& actions)
    {
        if (m_Node != nullptr)
        {
            if (!m_ownerPhysicObj || m_ownerPhysicObj->bIsUpdatingByODE())
            {
                m_Node->SetProperty(8710u, &actions);
            }
            else
            {
                static retruxx::vector<ActionType> empty;
                m_Node->SetProperty(8710u, &empty);
            }
        }
    }

    void PhysicBody::SetModelName(CStr const& modelName)
    {
        m_modelname = modelName;
        _ApplyCurrentModelName();
        ApplyCurrentModelCollision();
    }

    void PhysicBody::SetModelNameUnsafe(CStr const& newModelName)
    {
        m_modelname = newModelName;
    }

    int PhysicBody::GetNodeAnimAction() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetNodeAnimAction(int action, bool forceRestartAction)
    {
        m_animAction = action;
        if (action < 0x20)
        {
            auto nodeAnimInfo = GetNodeAnimInfo(m_Node);
            if (forceRestartAction || nodeAnimInfo)
            {
                auto curAnimAction = -1;
                if (!nodeAnimInfo->GetStickToLastFrame() && nodeAnimInfo->GetCurAnimation())
                {
                    curAnimAction = nodeAnimInfo->GetCurAnimation()->m_action;
                }

                if (curAnimAction != m_animAction && m_Node)
                {
                    m_Node->SetProperty(8709, &action);
                    retruxx::vector<m3d::Object*> stack;
                    stack.push_back(m_Node);

                    // Depth-first traversal
                    while (!stack.empty())
                    {
                        // Pop the last node from stack
                        m3d::Object* currentNode = stack.back();
                        stack.pop_back();

                        // Process all children of current node
                        m3d::SgNode* child = dynamic_cast<m3d::SgNode*>(currentNode->GetFirstChild());

                        // TODO: check this
                        while (child != nullptr)
                        {
                            child->SetProperty(8709, &action);

                            // If child has children of its own, push to stack for processing
                            if (child->GetFirstChild() != nullptr)
                            {
                                stack.push_back(child);
                            }

                            // Move to next sibling
                            child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling());
                        }
                    }
                }
            }
        }
    }

    void PhysicBody::SetNextForAnimation(int action, int nextAction)
    {
        if (m_Node)
        {
            if (action < 0x20)
            {
                if (nextAction >= -1 && nextAction < 32)
                {
                    auto& server = M3D_APP->GetAnimatedModelsServer();
                    int sh = -1;
                    m_Node->GetProperty(4360u, &sh);
                    if (sh != -1)
                    {
                        m3d::AnimatedModel* animModel = nullptr;
                        server.GetItemProperty(sh, 16394, &animModel);
                        animModel->SetNextForAnimation((ActionType)action, nextAction);
                    }
                }
            }
        }
    }

    void PhysicBody::ChangePhysicBodyByCollisionInfo(retruxx::vector<CollisionInfo> const& collisionInfos)
    {
        UnlinkGeomFromCollisionCells();
        _ClearGeoms();
        for (auto& collisionInfo : collisionInfos)
        {
            auto obj = GeomTransform::CreateObject(gGlobalSpace, CommonGeomMovedCallback);
            m_pGeoms.push_back(obj);
            switch (collisionInfo.m_geomType)
            {
            case GEOM_TYPE_BOX:
            {
                auto box = Box::CreateObject(0, collisionInfo.m_size, 0);
                obj->SetGeom(box);

                break;
            }
            case GEOM_TYPE_SPHERE:
            {
                auto sphere = Sphere::CreateObject(0, collisionInfo.m_radius, 0);
                obj->SetGeom(sphere);
                break;
            }
            case GEOM_TYPE_CYLINDER:
            {
                // TODO: check this
                auto cylinder = Cylinder::CreateObject(0, collisionInfo.m_radius, collisionInfo.m_size.y, 0);
                obj->SetGeom(cylinder);
                break;
            }
            case GEOM_TYPE_RAY:
            {
                auto ray = Ray::CreateObject(0, 1000.0, 0);
                obj->SetGeom(ray);
                break;
            }
            case GEOM_TYPE_TRIMESH:
            {
                // TODO: check this
                auto trimesh = TriMesh::CreateObject(
                    0,
                    collisionInfo.m_trimeshVertices->GetObjectA().data(),
                    collisionInfo.m_numTrimeshVertices,
                    collisionInfo.m_trimeshIndices->GetObjectA().data(),
                    collisionInfo.m_numTrimeshIndices,
                    nullptr,
                    -1);
                obj->SetGeom(trimesh);
                break;
            }
            default:
            {
                M3D_LOG_ERR("Invalid geom type: " + CStr(collisionInfo.m_geomType) + ", for model: " + GetDebugDescription());
                M3D_ASSERT(0);
            }
            }

            dGeomSetPosition(
                obj->GetGeomId(), collisionInfo.m_relTranslation.x, collisionInfo.m_relTranslation.y, collisionInfo.m_relTranslation.z);

            dQuaternion q;
            q[0] = collisionInfo.m_relRotation.w;
            q[1] = collisionInfo.m_relRotation.x;
            q[2] = collisionInfo.m_relRotation.y;
            q[3] = collisionInfo.m_relRotation.z;
            dGeomSetQuaternion(obj->GetGeomId(), q);
        }
        SetOwnerBodyToGeoms();
        m_collisionInfos = collisionInfos;
    }

    void PhysicBody::SetSgNode(m3d::SgNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicBody::GetPosition() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetPosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::UpdateGeomsByCollisionInfo(retruxx::vector<CollisionInfo> const& collisionInfos)
    {
        auto pos = GetNodeRelativePosition();
        m_collisionInfos = collisionInfos;
        for (int i = 0; i < m_pGeoms.size(); ++i)
        {
            auto geom = m_pGeoms[i]->GetGeom();
            if (geom)
            {
                dGeomSetPosition(
                    geom->GetGeomId(),
                    m_collisionInfos[i].m_relTranslation.x,
                    m_collisionInfos[i].m_relTranslation.y,
                    m_collisionInfos[i].m_relTranslation.z);

                dQuaternion quat;
                quat[0] = m_collisionInfos[i].m_relRotation.w;
                quat[1] = m_collisionInfos[i].m_relRotation.x;
                quat[2] = m_collisionInfos[i].m_relRotation.y;
                quat[3] = m_collisionInfos[i].m_relRotation.z;
                dGeomSetQuaternion(geom->GetGeomId(), quat);

                auto cl = dGeomGetClass(geom->GetGeomId());
                if (cl)
                {
                    if (cl == 1)
                        ((ai::Box*)geom)->SetSize(m_collisionInfos[i].m_size);
                }
                else
                {
                    ((ai::Sphere*)geom)->SetRadius(m_collisionInfos[i].m_radius);
                }
            }
        }
        SetNodeRelativePosition(pos);
    }

    void PhysicBody::SetNodeAction(int action, bool forceRestartAction)
    {
        // TODO: generated code
        // Store the action
        this->m_animAction = action;
        this->m_effectAction = action;

        // Check if we should apply the action to the node hierarchy
        bool shouldApplyAction = forceRestartAction;

        if (!shouldApplyAction)
        {
            // Check if the current animation action doesn't match the new action
            m3d::AnimInfo* animInfo = ai::GetNodeAnimInfo(this->m_Node);
            if (animInfo != nullptr)
            {
                int currentAction = -1;
                if (!animInfo->GetStickToLastFrame() && animInfo->GetCurAnimation() != nullptr)
                {
                    currentAction = animInfo->GetCurAnimation()->m_action;
                }
                shouldApplyAction = (currentAction != this->m_animAction);
            }
            else
            {
                shouldApplyAction = true;  // No anim info, so apply the action
            }
        }

        // Apply the action to the node hierarchy if needed
        if (shouldApplyAction && this->m_Node != nullptr)
        {
            // Set property on the root node
            this->m_Node->SetProperty(8704, &action);

            // Use stack for iterative depth-first traversal of node hierarchy
            // Process children using iterative DFS
            std::vector<m3d::Object*> stack;
            stack.push_back(dynamic_cast<m3d::Object*>(m_Node->GetFirstChild()));

            while (!stack.empty())
            {
                m3d::Object* current = stack.back();
                stack.pop_back();

                // Process all siblings of the current node
                m3d::SgNode* sibling = dynamic_cast<m3d::SgNode*>(current);
                while (sibling)
                {
                    sibling->SetProperty(8704, &action);

                    // If this sibling has children, add to stack for processing
                    if (sibling->GetFirstChild())
                    {
                        stack.push_back(sibling->GetFirstChild());
                    }

                    // Move to next sibling
                    sibling = dynamic_cast<m3d::SgNode*>(sibling->GetNextSibling());
                }
            }
        }
    }

    void PhysicBody::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::SgNode* PhysicBody::CreateNode(CStr const& modelname, int action, CVector const& scale, PhysicBody* owner, bool addToRoot)
    {
        auto modelId = M3D_ENGINE_CFG.GetModelIdByName(modelname);
        auto serverNode = m3d::pClient->CreateServerControlledNode(modelId);
        if (serverNode)
        {
            if (addToRoot)
            {
                m3d::pClient->GetWorld().GetGraph().GetRootNode()->AddChild(serverNode);
            }
            if (action != -1)
            {
                serverNode->SetProperty(8704, &action);
            }
            serverNode->SetProperty(4356, &owner);
            serverNode->SetScale(scale);
            serverNode->SetPersistance(false);
            serverNode->UpdateXForm(false, true);
        }
        return serverNode;
    }

    PhysicBody::PhysicBody(PhysicBodyPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        m_modelname = prototypeInfo.m_engineModelName;
        dMassSetZero(&m_mass);
        m_bCollisionTrimeshAllowed = prototypeInfo.m_bCollisionTrimeshAllowed;

        m_collisionInfos = prototypeInfo.m_collisionInfos;
        auto geomTransform = ai::GeomTransform::CreateObject(ai::gGlobalSpace, ai::CommonGeomMovedCallback);
        m_pGeoms.push_back(geomTransform);

        m_Node = 0;
        m_cfgNum = 0;
        m_ownerPhysicObj = 0;
        m_animAction = 0;
        m_effectAction = 0;
        m_bAnimationIsStopped = 0;
        m_mU = 1.0;
        m_bNeedToRelinkNode = 1;
    }

    m3d::Class* PhysicBody::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void PhysicBody::TransferPhysicParamsToSceneGraphNode()
    {
        if (this->m_Node)
        {
            auto NodeRelativeRotation = ai::PhysicBody::GetNodeRelativeRotation();
            auto Rotation = ai::PhysicBody::GetRotation();
            auto v4 = Rotation.w * NodeRelativeRotation.y;

            Quaternion rot;  // [esp+10h] [ebp-30h] BYREF
            rot.x = (float)((float)((float)(NodeRelativeRotation.z * Rotation.y) + (float)(Rotation.w * NodeRelativeRotation.x)) +
                            (float)(NodeRelativeRotation.w * Rotation.x)) -
                (float)(Rotation.z * NodeRelativeRotation.y);
            auto v5 = (float)((float)((float)(NodeRelativeRotation.w * Rotation.y) + v4) + (float)(Rotation.z * NodeRelativeRotation.x)) -
                (float)(NodeRelativeRotation.z * Rotation.x);
            auto v6 = NodeRelativeRotation.w * Rotation.z;
            rot.y = v5;
            auto v7 = (float)((float)((float)(NodeRelativeRotation.z * Rotation.w) + v6) + (float)(Rotation.x * NodeRelativeRotation.y)) -
                (float)(Rotation.y * NodeRelativeRotation.x);
            auto v8 = Rotation.x * NodeRelativeRotation.x;
            rot.z = v7;
            rot.w = (float)((float)((float)(NodeRelativeRotation.w * Rotation.w) - v8) - (float)(Rotation.y * NodeRelativeRotation.y)) -
                (float)(NodeRelativeRotation.z * Rotation.z);
            auto NodeAbsolutePosition = ai::PhysicBody::GetNodeAbsolutePosition();
            ai::PhysicBody::_SetScenegraphNode(NodeAbsolutePosition, rot);
        }
    }

    void PhysicBody::_ClearGeoms()
    {
        for (auto& geom : m_pGeoms)
        {
            delete geom;
        }
        m_pGeoms.clear();
    }

    CVector PhysicBody::GetDirection() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned PhysicBody::GetNumGeoms() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::SgNode* PhysicBody::CreateEffectNode(
        CStr const& modelname,
        CVector const& pos,
        Quaternion const& rot,
        bool bInsertInRemoveIfFree,
        float scale)
    {
        auto const modelId = M3D_ENGINE_CFG.GetModelIdByName(modelname);
        auto* node = m3d::pClient->CreateServerControlledNode(modelId);
        if (node)
        {
            m3d::pClient->GetWorld().GetGraph().GetRootNode()->AddChild(node);

            // TODO: check this!!!!!
            int nullValue = 0;
            node->SetProperty(m3d::PROP_DM_ACTION, &nullValue);
            node->SetProperty(m3d::PROP_NODE_PHYSICBODY, &nullValue);

            node->SetScale(scale);
            node->SetPersistance(false);
            node->UpdateXForm(0, 1);

            if (bInsertInRemoveIfFree)
            {
                node->GetGraph()->InsertInRemoveIfFree(node);
            }
            node->SetOriginAbs(pos);
            node->SetRotation(rot);
            node->UpdateXForm(0, 1);
            m3d::pClient->GetWorld().GetGraph().LinkNode(node);
        }
        return node;
    }

    Quaternion PhysicBody::GetRotation() const
    {
        if (!m_pGeoms.empty())
        {
            auto& first = m_pGeoms.front();

            float quat[4];
            dGeomGetQuaternion(first->GetGeomId(), quat);

            Quaternion result;
            result.x = quat[1];
            result.y = quat[2];
            result.z = quat[3];
            result.w = quat[0];
            return result;
        }
        return {0.0, 0.0, 0.0, 1.0};
    }

    void PhysicBody::SetRotation(Quaternion const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::UnlinkGeomsFromBody()
    {
        for (auto& geom : m_pGeoms)
        {
            geom->UnlinkFromBody();
        }
    }

    void PhysicBody::SetAnimationStopped(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::DbgCounter* PhysicBody::GetCountNodeRelinks()
    {
        return m_countNodeRelinks;
    }

    Geom* PhysicBody::GetGeom(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::RelinkSceneGraphNode()
    {
        if (this->m_bNeedToRelinkNode)
        {
            if (m_Node)
            {
                auto* graph = m_Node->GetGraph();
                if (graph->IsLinkedNode(m_Node))
                {
                    ai::PhysicBody::m_countNodeRelinks->IncI();
                    graph->RelinkNode(m_Node, 0);
                    this->m_bNeedToRelinkNode = 0;
                }
            }
        }
    }

    void PhysicBody::SetNodeEffectAction(int action)
    {
        m_effectAction = action;
        m_effectAction = action;
        if (action < 0x20)
        {
            if (m_Node)
            {
                m_Node->SetProperty(8708, &action);
                retruxx::vector<m3d::Object*> stack;
                stack.push_back(m_Node);

                // Depth-first traversal
                while (!stack.empty())
                {
                    // Pop the last node from stack
                    m3d::Object* currentNode = stack.back();
                    stack.pop_back();

                    // Process all children of current node
                    m3d::SgNode* child = dynamic_cast<m3d::SgNode*>(currentNode->GetFirstChild());

                    // TODO: check this
                    while (child != nullptr)
                    {
                        child->SetProperty(8708, &action);

                        // If child has children of its own, push to stack for processing
                        if (child->GetFirstChild() != nullptr)
                        {
                            stack.push_back(child);
                        }

                        // Move to next sibling
                        child = dynamic_cast<m3d::SgNode*>(child->GetNextSibling());
                    }
                }
            }
        }
    }

    void PhysicBody::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int PhysicBody::GetNodeEffectAction() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::LinkGeomToCollisionCells()
    {
        for (auto& geom : m_pGeoms)
        {
            int objId = -1;
            if (m_ownerPhysicObj)
            {
                objId = m_ownerPhysicObj->GetId();
            }

            geom->LinkToCollisionCells(objId, nullptr);
        }
    }

    retruxx::vector<CollisionInfo, retruxx::allocator<CollisionInfo>> const& PhysicBody::GetCollisionInfo() const
    {
        return this->m_collisionInfos;
    }

    void PhysicBody::SetNodeAbsolutePosition(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicBody::GetNodeAbsolutePosition() const
    {
        // Get node relative position and body rotation
        CVector relativePos = GetNodeRelativePosition();
        Quaternion rotation = GetRotation();

        // Convert quaternion to rotation matrix
        float xx = rotation.x * rotation.x;
        float xy = rotation.x * rotation.y;
        float xz = rotation.x * rotation.z;
        float xw = rotation.x * rotation.w;
        float yy = rotation.y * rotation.y;
        float yz = rotation.y * rotation.z;
        float yw = rotation.y * rotation.w;
        float zz = rotation.z * rotation.z;
        float zw = rotation.z * rotation.w;

        // Build 3x3 rotation matrix from quaternion
        CMatrix rotMatrix;
        rotMatrix._11 = 1.0f - 2.0f * (yy + zz);
        rotMatrix._12 = 2.0f * (xy + zw);
        rotMatrix._13 = 2.0f * (xz - yw);
        rotMatrix._14 = 0.0f;

        rotMatrix._21 = 2.0f * (xy - zw);
        rotMatrix._22 = 1.0f - 2.0f * (xx + zz);
        rotMatrix._23 = 2.0f * (yz + xw);
        rotMatrix._24 = 0.0f;

        rotMatrix._31 = 2.0f * (xz + yw);
        rotMatrix._32 = 2.0f * (yz - xw);
        rotMatrix._33 = 1.0f - 2.0f * (xx + yy);
        rotMatrix._34 = 0.0f;

        rotMatrix._41 = 0.0f;
        rotMatrix._42 = 0.0f;
        rotMatrix._43 = 0.0f;
        rotMatrix._44 = 1.0f;

        // Transform relative position by rotation matrix
        CVector rotatedPos;
        rotatedPos.x = relativePos.x * rotMatrix._11 + relativePos.y * rotMatrix._21 + relativePos.z * rotMatrix._31;
        rotatedPos.y = relativePos.x * rotMatrix._12 + relativePos.y * rotMatrix._22 + relativePos.z * rotMatrix._32;
        rotatedPos.z = relativePos.x * rotMatrix._13 + relativePos.y * rotMatrix._23 + relativePos.z * rotMatrix._33;

        // Get physics geometry position (assuming ODE physics)
        float const* geomPosition = dGeomGetPosition(m_pGeoms[0]->GetGeomId());

        CVector result;

        // Combine rotated relative position with physics body position
        result.x = geomPosition[0] + rotatedPos.x;
        result.y = geomPosition[1] + rotatedPos.y;
        result.z = geomPosition[2] + rotatedPos.z;

        return result;
    }

    PhysicBody::~PhysicBody()
    {
        _ClearGeoms();
        if (m_Node)
        {
            m_Node->GetGraph()->RemoveNode(this->m_Node);
            m_Node = 0;
        }
    }

    void PhysicBody::SetBelong(int newBelong)
    {
        ai::Obj::SetBelong(newBelong);
        if (m_Node)
            m_Node->SetProperty(4353u, &newBelong);
    }

    void PhysicBody::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int PhysicBody::GetSkin() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetSkin(int skin)
    {
        if (this->m_Node)
            this->m_Node->SetProperty(8706u, &skin);
    }

    int PhysicBody::GetNodeCfgNum() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::RenderDebugInfo() const
    {
        if (!m_Node || m_Node->m_frameVisible == M3D_KERNEL->GetTimer().GetCurFrame())
        {
            for (auto const& geom : m_pGeoms)
            {
                if (dGeomIsEnabled(geom->GetGeomId()))
                {
                    pServer->GetWorld()->GetLandscape().DrawGeom(geom->GetGeomId());
                }
            }
        }
    }

    void PhysicBody::SetNodeCfgNum(int cfgNum)
    {
        this->m_cfgNum = cfgNum;
        if (m_Node)
            m_Node->SetProperty(8707u, &cfgNum);
    }

    void PhysicBody::SetPassedToAnotherMapStatus()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::ApplyCurrentModelCollision()
    {
        if (this->m_Node)
        {
            int sh = -1;
            m_Node->GetProperty(4360u, &sh);
            ai::GetCollisionInfoByServerHandle(sh, this->m_collisionInfos, this->m_bCollisionTrimeshAllowed);
            ai::PhysicBody::ChangePhysicBodyByCollisionInfo(this->m_collisionInfos);
        }
        else
        {
            ai::GetCollisionInfoByModelName(this->m_modelname, this->m_collisionInfos, this->m_bCollisionTrimeshAllowed);
            ai::PhysicBody::ChangePhysicBodyByCollisionInfo(this->m_collisionInfos);
        }
    }

    CVector PhysicBody::GetNodeAbsoluteDirection() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Geom::CellAabb PhysicBody::GetCollisionCellAabb() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetNodeAbsoluteDirection(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* PhysicBody::GetRtClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetOwnerBodyToGeoms()
    {
        m_ownerPhysicObj = this->m_ownerPhysicObj;

        dBodyID id = 0;
        if (m_ownerPhysicObj)
            id = m_ownerPhysicObj->GetBody()->id();

        auto v4 = 0;
        if (m_ownerPhysicObj)
        {
            v4 = 1;
            if ((m_ownerPhysicObj->GetPhysicState() & 1) != 0 || dBodyGetAutoDisableFlag(id))
                v4 = 0;
        }
        for (auto& geom : m_pGeoms)
        {
            dGeomSetData(geom->GetGeomId(), this);
            dGeomSetBody(geom->GetGeomId(), id);
            if (v4)
                dGeomSetBody(geom->GetGeomId(), id);
            geom->UnlinkFromBody();
        }
    }

    void PhysicBody::Registration()
    {
        m_countNodeRelinks = M3D_APP->GetDbgCounterStack().GetCounter(M3D_APP->GetDbgCounterStack().AddCounter("node relinks in server"));
        m_countNodeRelinks->SetI(0);
    }

    int PhysicBody::GetNodeRealAnimAction() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetCollisionTrimeshAllowed(bool bCollisionTrimeshAllowed)
    {
        m_bCollisionTrimeshAllowed = bCollisionTrimeshAllowed;
    }

    Quaternion PhysicBody::GetNodeRelativeRotation() const
    {
        // TODO: generated code
        // Check if we have valid geometry and collision info
        if (!m_pGeoms.empty() && m_pGeoms[0] != nullptr && m_pGeoms[0]->GetGeom() != nullptr)
        {
            // Get the rotation from the inner geometry
            Quaternion geomRotation = m_pGeoms[0]->GetGeom()->GetRotation();

            // Get the inverse of the relative rotation from collision info
            Quaternion inverseRelRot = m_collisionInfos[0].m_relRotation.getInversed();

            Quaternion result;
            // Combine the rotations: result = inverseRelRot * geomRotation
            result.x =
                (inverseRelRot.w * geomRotation.x + inverseRelRot.x * geomRotation.w + inverseRelRot.y * geomRotation.z -
                 inverseRelRot.z * geomRotation.y);

            result.y =
                (inverseRelRot.w * geomRotation.y - inverseRelRot.x * geomRotation.z + inverseRelRot.y * geomRotation.w +
                 inverseRelRot.z * geomRotation.x);

            result.z =
                (inverseRelRot.w * geomRotation.z + inverseRelRot.x * geomRotation.y - inverseRelRot.y * geomRotation.x +
                 inverseRelRot.z * geomRotation.w);

            result.w =
                (inverseRelRot.w * geomRotation.w - inverseRelRot.x * geomRotation.x - inverseRelRot.y * geomRotation.y -
                 inverseRelRot.z * geomRotation.z);

            return result;
        }
        return {0.0, 0.0, 0.0, 1.0};
    }

    void PhysicBody::SetNodeRelativeRotation(Quaternion const& q)
    {
        auto& colInfoRotation = m_collisionInfos.front().m_relRotation;
        for (auto& geom : m_pGeoms)
        {
            auto p_y = &this->m_collisionInfos.front().m_relRotation.y;
            if (auto* inner = geom->GetGeom())
            {
                // TODO: check this
                float quat[4];
                quat[0] = (float)((float)((float)(q.w * p_y[2]) - (float)(*(p_y - 1) * q.x)) - (float)(q.y * *p_y)) - (float)(p_y[1] * q.z);
                quat[1] = (float)((float)((float)(p_y[2] * q.x) + (float)(q.y * p_y[1])) + (float)(q.w * *(p_y - 1))) - (float)(*p_y * q.z);
                quat[2] = (float)((float)((float)(q.w * *p_y) + (float)(q.y * p_y[2])) + (float)(*(p_y - 1) * q.z)) - (float)(p_y[1] * q.x);
                quat[3] = (float)((float)((float)(q.w * p_y[1]) + (float)(q.x * *p_y)) + (float)(p_y[2] * q.z)) - (float)(q.y * *(p_y - 1));
                dGeomSetQuaternion(inner->GetGeomId(), quat);
            }
        }
    }

    int PhysicBody::GetNodeRealAction() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool PhysicBody::bNeedToRelinkNode() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::SetVisible()
    {
        Obj::SetVisible();
        EnableGeometry();
        if (m_Node)
        {
            m3d::SgNode* parent = nullptr;
            m_Node->GetProperty(4359u, &parent);
            if (parent)
            {
                parent->AddChild(m_Node);
                parent = nullptr;
                m_Node->SetProperty(4359, &parent);
            }
        }
    }

    void PhysicBody::EnableGeometry()
    {
        for (auto& geom : m_pGeoms)
        {
            dGeomEnable(geom->GetGeomId());
        }
    }

    void PhysicBody::SetOwner(PhysicObj* owner)
    {
        if (owner != this->m_ownerPhysicObj)
        {
            this->m_ownerPhysicObj = owner;
            SetOwnerBodyToGeoms();
        }
    }

    PhysicObj* PhysicBody::GetOwner() const
    {
        return this->m_ownerPhysicObj;
    }

    void PhysicBody::SetInvisible()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::ReceiveNodesToLink(retruxx::list<m3d::SgNode*>& nodes) const
    {
        if (m_Node)
        {
            nodes.push_back(m_Node);
        }
    }

    void PhysicBody::SetNodeAbsoluteRotation(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Quaternion PhysicBody::GetNodeAbsoluteRotation() const
    {
        auto nodeRelativeRotation = ai::PhysicBody::GetNodeRelativeRotation();
        auto rotation = ai::PhysicBody::GetRotation();
        auto v5 =
            (((rotation.w * nodeRelativeRotation.y) + (nodeRelativeRotation.w * rotation.y)) + (rotation.z * nodeRelativeRotation.x)) -
            (rotation.x * nodeRelativeRotation.z);
        auto v6 =
            (((rotation.w * nodeRelativeRotation.z) + (rotation.x * nodeRelativeRotation.y)) + (rotation.z * nodeRelativeRotation.w)) -
            (nodeRelativeRotation.x * rotation.y);
        auto v7 =
            (((rotation.w * nodeRelativeRotation.w) - (rotation.x * nodeRelativeRotation.x)) - (rotation.y * nodeRelativeRotation.y)) -
            (rotation.z * nodeRelativeRotation.z);
        auto v8 = ((nodeRelativeRotation.z * rotation.y) + (rotation.x * nodeRelativeRotation.w)) + (rotation.w * nodeRelativeRotation.x);
        auto v9 = rotation.z * nodeRelativeRotation.y;

        Quaternion result;
        result.x = v8 - v9;
        result.y = v5;
        result.z = v6;
        result.w = v7;
        return result;
    }

    bool PhysicBody::CanChildBeAdded(m3d::Class*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector PhysicBody::GetNodeRelativeDirection() const
    {
        auto NodeRelativeRotation = GetNodeRelativeRotation();
        auto v3 = NodeRelativeRotation.z * NodeRelativeRotation.w;
        auto v4 = NodeRelativeRotation.z * NodeRelativeRotation.x;
        auto v14 = NodeRelativeRotation.x * NodeRelativeRotation.x;
        auto v5 = NodeRelativeRotation.w * NodeRelativeRotation.x;
        auto v13 = NodeRelativeRotation.y * NodeRelativeRotation.x;
        auto v12 = NodeRelativeRotation.z * NodeRelativeRotation.y;
        auto v11 = NodeRelativeRotation.y * NodeRelativeRotation.w;
        auto v6 = NodeRelativeRotation.z * NodeRelativeRotation.z;
        auto v7 = NodeRelativeRotation.y * NodeRelativeRotation.y;

        CMatrix vv;
        vv._11 = 1.0 - ((v6 + v7) * 2.0);
        vv._21 = (v13 - v3) * 2.0;
        vv._31 = (v11 + v4) * 2.0;
        vv._12 = (v3 + v13) * 2.0;
        vv._22 = 1.0 - ((v6 + v14) * 2.0);
        vv._33 = 1.0 - ((v7 + v14) * 2.0);
        vv._32 = (v12 - v5) * 2.0;
        vv.m[0][2] = ((v4 - v11) * 2.0);
        vv.m[0][3] = 0.0;
        vv.m[1][2] = ((v5 + v12) * 2.0);
        vv.m[1][3] = 0.0;
        memset(&vv.m[2][3], 0, 16);
        vv._44 = 1.0;

        CVector const INITIAL_OBJECTS_DIRECTION_6(0.0, 0.0, 1.0);
        auto v9 = ((vv._32 * INITIAL_OBJECTS_DIRECTION_6.z) + (vv._22 * INITIAL_OBJECTS_DIRECTION_6.y)) +
            (vv._12 * INITIAL_OBJECTS_DIRECTION_6.x);
        auto v10 = ((vv._33 * INITIAL_OBJECTS_DIRECTION_6.z) + (vv._23 * INITIAL_OBJECTS_DIRECTION_6.y)) +
            (vv._13 * INITIAL_OBJECTS_DIRECTION_6.x);

        CVector result;
        result.x = ((vv._31 * INITIAL_OBJECTS_DIRECTION_6.z) + (vv._11 * INITIAL_OBJECTS_DIRECTION_6.x)) +
            (INITIAL_OBJECTS_DIRECTION_6.y * vv._21);
        result.y = v9;
        result.z = v10;
        return result;
    }

    m3d::AnimatedModel* PhysicBody::GetModel() const
    {
        if (m_Node == nullptr)
        {
            return nullptr;
        }

        auto* server = m_Node->GetServer();

        m3d::AnimatedModel* mdl = nullptr;
        server->GetItemProperty(this->m_Node->GetServerHandle(), 16394, &mdl);
        return mdl;
    }

    float PhysicBody::GetMass() const
    {
        return this->m_mass.mass;
    }

    CVector PhysicBody::GetNodeRelativePosition() const
    {
        if (!m_pGeoms.empty() && m_pGeoms.front()->GetGeom() != nullptr)
        {
            auto& relPos = m_collisionInfos.front().m_relTranslation;
            auto inner = m_pGeoms.front()->GetGeom();
            auto pos = dGeomGetPosition(inner->GetGeomId());
            return {pos[0] - relPos.x, pos[1] - relPos.y, pos[2] - relPos.z};
        }
        return {0.0, 0.0, 0.0};
    }

    void PhysicBody::SetNodeRelativePosition(CVector const& pos)
    {
        for (int i = 0; i < m_pGeoms.size(); ++i)
        {
            auto x = m_collisionInfos[i].m_relTranslation.x + pos.x;
            auto y = m_collisionInfos[i].m_relTranslation.y + pos.y;
            auto z = m_collisionInfos[i].m_relTranslation.z + pos.z;
            dGeomSetPosition(m_pGeoms[i]->GetGeom()->GetGeomId(), x, y, z);
        }
    }

    void PhysicBody::UnlinkGeomFromCollisionCells()
    {
        for (auto& geom : m_pGeoms)
        {
            auto objId = -1;
            if (m_ownerPhysicObj)
                objId = m_ownerPhysicObj->GetId();
            geom->UnlinkFromCollisionCells(objId);
        }
    }

    void PhysicBody::RelinkGeomToCollisionCells()
    {
        for (auto& geom : m_pGeoms)
        {
            auto objId = -1;
            if (m_ownerPhysicObj)
                objId = m_ownerPhysicObj->GetId();
            geom->RelinkToCollisionCells(objId);
        }
    }

    void PhysicBody::DisableGeometry()
    {
        for (auto* geom : m_pGeoms)
        {
            dGeomDisable(geom->GetGeomId());
        }
    }

    void PhysicBody::RelinkToSpace(dxSpace* newSpace)
    {
        for (auto& geom : m_pGeoms)
        {
            if (dGeomGetSpace(geom->GetGeomId()))
            {
                auto Space = dGeomGetSpace(geom->GetGeomId());
                dSpaceRemove(Space, geom->GetGeomId());
            }
            dSpaceAdd(newSpace, geom->GetGeomId());
        }
    }

    int PhysicBody::GetOwnerId() const
    {
        if (m_ownerPhysicObj)
            return m_ownerPhysicObj->GetId();
        else
            return -1;
    }

    int PhysicBody::_GetCurAnimationFrame() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::_SetScenegraphNode(CVector const& pos, Quaternion const& rot)
    {
        m_Node->SetOriginAbs(pos);
        m_Node->SetRotation(rot);
        this->m_bNeedToRelinkNode = true;

        auto objId = GetId();
        if (objId == -1)
        {
            if (m_ownerPhysicObj)
            {
                ai::theObjects->AddObjIdToRelinkSceneGraphNode(m_ownerPhysicObj->GetId());
                return;
            }
            objId = -1;
        }
        ai::theObjects->AddObjIdToRelinkSceneGraphNode(objId);
    }

    PhysicBody::PhysicBody()
    {
        dMassSetZero(&m_mass);
        m_modelname = "BOX";
        auto obj = GeomTransform::CreateObject(nullptr, CommonGeomMovedCallback);
        m_pGeoms.push_back(obj);

        CollisionInfo info;
        m_collisionInfos.push_back(std::move(info));

        this->m_bCollisionTrimeshAllowed = 0;
        this->m_Node = 0;
        this->m_ownerPhysicObj = 0;
        this->m_animAction = 0;
        this->m_effectAction = 0;
        this->m_cfgNum = 0;
        this->m_bAnimationIsStopped = 0;
        this->m_loadedAnimTime = 0;
        this->m_mU = 1.0;
        this->m_bNeedToRelinkNode = 1;
    }

    int PhysicBody::_GetNodeRealAnimAction() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PhysicBody::_InternalCreateVisualPart()
    {
        ai::PhysicBody::_ApplyCurrentModelName();
        if (m_loadedAnimTime > 0)
            ai::SetNodeElapsedAnimationTimeInMs(this->m_Node, m_loadedAnimTime);
        if ((this->GetFlags() & 1) == 0)
            this->SetInvisible();
    }

    void PhysicBody::_ApplyCurrentModelName()
    {
        // TODO: generated code
        // Get current node transformations
        CVector nodePos = GetNodeAbsolutePosition();
        Quaternion relativeRot = GetNodeRelativeRotation();
        Quaternion rotation = GetRotation();

        // Calculate combined rotation (applying relative rotation to base rotation)
        Quaternion nodeRot;
        nodeRot.x = (rotation.w * relativeRot.x + relativeRot.z * rotation.y + relativeRot.w * rotation.x) - (rotation.z * relativeRot.y);
        nodeRot.y = (relativeRot.w * rotation.y + rotation.w * relativeRot.y + rotation.z * relativeRot.x) - (relativeRot.z * rotation.x);
        nodeRot.z = (relativeRot.z * rotation.w + relativeRot.w * rotation.z + rotation.x * relativeRot.y) - (relativeRot.x * rotation.y);
        nodeRot.w = (relativeRot.w * rotation.w - rotation.x * relativeRot.x - rotation.y * relativeRot.y - relativeRot.z * rotation.z);

        // Remove existing node from scene graph
        if (m_Node != nullptr)
        {
            m3d::SceneGraph* graph = m_Node->GetGraph();
            graph->RemoveNode(m_Node);
            m_Node = nullptr;
        }

        // Create new node if model name is valid
        if (!m_modelname.empty())
        {
            // Create scale vector
            CVector scale(1.0f, 1.0f, 1.0f);

            // Get engine configuration and create node
            int modelId = M3D_KERNEL->GetEngineCfg().GetModelIdByName(m_modelname);  // Assuming GetModelId based on context

            m3d::SgNode* serverControlledNode = m3d::pClient->CreateServerControlledNode(modelId);
            if (serverControlledNode != nullptr)
            {
                // Add to scene graph
                m3d::pClient->GetWorld().GetGraph().GetRootNode()->AddChild(serverControlledNode);

                // Set properties
                serverControlledNode->SetProperty(4356u, this);  // 4356u appears to be a property ID
                serverControlledNode->SetScale(scale);
                serverControlledNode->SetPersistance(false);
                serverControlledNode->UpdateXForm(false, true);
            }

            m_Node = serverControlledNode;

            // Set transformations
            if (m_Node != nullptr)
            {
                m_Node->SetOriginAbs(nodePos);
                m_Node->SetRotation(nodeRot);

                // Apply animations and effects
                SetNodeAnimAction(m_animAction, true);
                SetNodeEffectAction(m_effectAction);
                SetNodeCfgNum(m_cfgNum);

                auto belong = GetBelong();
                // Set ownership property
                m_Node->SetProperty(4353u, &belong);

                // Link to dynamic scene
                ai::gDynamicScene->LinkNodesFromBodyToSceneGraph(this);
            }
        }
    }

    void PhysicBody::_DeleteNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai

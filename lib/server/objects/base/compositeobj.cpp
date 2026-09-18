#include "compositeobj.h"
#include "geomobj.h"
#include "server/objects/physicbodies/geoms/box.h"
#include "server/server.h"
#include "objcontainer.h"
#include "prototypemanager.h"
#include "server/dynamicscene.h"

#include "server/objects/physicbodies/physicbody.h"

#include <core/kernel.h>
#include <client.h>
#include <core/timer.h>
#include <core/ini.h>
#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>
#include <ode/objects.h>
#include <ode/odecpp.h>
#include <scene/servers/dataserver.h>
#include <skelmodel.h>
#include <landscape.h>
#include <scene/scenegraph.h>
#include <stdexcept>
#include <world.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(CompositeObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(CompositeObj);

    bool CompositeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    CompositeObjPrototypeInfo::CompositeObjPrototypeInfo() = default;

    ai::Obj* CompositeObjPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x815080
        return new CompositeObj(*this);
    }

    CompositeObj::CompositeObj(CompositeObjPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        // RVA 0x8134F0 - nothing is built until Init is called with a model.
        m_modelName = CStr();
        m_mass = 10.0f;
        m_node = nullptr;
        m_Model = nullptr;
        m_Anim = nullptr;
        m_Position = CVector(0.0, 0.0, 0.0);
        m_enabled = false;
        m_initialNodeTransform.identity();
        m_deadTimerActive = false;
        m_deadTimer = 0.0f;
        m_testVisibility = false;
    }

    void CompositeObj::Update(float elapsedTime, unsigned)
    {
        // RVA 0x8112A0 - the pieces are what is really simulated; every frame their transforms are copied
        // back onto the model's bones so that the wreck is drawn in the shape its physics has taken.
        if (!m_enabled)
        {
            return;
        }
        if ((GetFlags() & 8) != 0 || (GetFlags() & 2) != 0 || GetParentRepository())
        {
            return;
        }
        if (!m_Anim)
        {
            return;
        }

        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        RecalcBoundBox();
        RelinkSceneGraphNode();

        // Bone matrices are kept relative to where the wreck now is.
        CMatrix invMatWorld;
        invMatWorld.identity();
        invMatWorld.setOrg(CVector(-m_Position.x, -m_Position.y, -m_Position.z));

        for (unsigned i = 0; i < m_Model->m_HierGeoms.size(); ++i)
        {
            GeomObj* const member = m_Members[i];
            CMatrix geomToWorld;
            geomToWorld.rotTranslate(member->GetRotation(), member->GetPosition());
            m_Anim->SetBoneCurMatrix(m_bonesIndices[i], m_boneToGeom[i] * geomToWorld * invMatWorld);
            m_Anim->GetBoneAnim(m_bonesIndices[i]).m_lastUpdatedFrame = curFrame;
        }

        // Bones that carry no geom of their own follow whichever bone they hang from, and the model's box
        // grows to take in every geom-driven bone.
        for (int i = 0; i < m_Model->m_header.m_numNodes; ++i)
        {
            m3d::BoneAnim& boneAnim = m_Anim->GetBoneAnim(i);
            if (boneAnim.m_lastUpdatedFrame == curFrame)
            {
                Aabb boneBox = m_Model->m_box;
                boneBox.m_box[0] += boneAnim.m_curMatrix._41;
                boneBox.m_box[1] += boneAnim.m_curMatrix._42;
                boneBox.m_box[2] += boneAnim.m_curMatrix._43;
                boneBox.m_box[3] += boneAnim.m_curMatrix._41;
                boneBox.m_box[4] += boneAnim.m_curMatrix._42;
                boneBox.m_box[5] += boneAnim.m_curMatrix._43;
                m_Anim->m_curBox.EmbraceBox(boneBox);
                continue;
            }
            if (boneAnim.m_parentIdx >= 0)
            {
                boneAnim.m_curMatrix = boneAnim.m_curMatrix * m_Anim->GetBoneAnim(boneAnim.m_parentIdx).m_curMatrix;
            }
        }

        m3d::Configuration* cfg = nullptr;
        m_node->GetProperty(8707u, &cfg);
        m_Model->UpdateVertices(m_Anim, true, cfg);

        CheckDisablePhysics();

        if (m_deadTimerActive)
        {
            m_deadTimer -= elapsedTime;
            // Wreckage the player can still see is given longer.
            if (m_deadTimer <= 0.0 &&
                (!m_testVisibility || m_node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame() - 1))
            {
                Remove();
            }
        }
    }

    void CompositeObj::Disable(bool disable)
    {
        // RVA 0x80FEA0
        m_enabled = !disable;
        for (GeomObj* const member : m_Members)
        {
            if (disable)
            {
                member->DisablePhysics();
            }
            else
            {
                member->EnablePhysics();
            }
        }
    }

    CVector CompositeObj::GetPosition()
    {
        // RVA 0x813660
        return m_Position;
    }

    m3d::Class* CompositeObj::GetClass() const
    {
        // RVA 0x80E1E0
        return RT_CLASS_LOCAL(CompositeObj);
    }

    void CompositeObj::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x810090 - everything needed to put the wreck back together: the model it came from and
        // where each piece had got to.
        Obj::SaveRuntimeValues(xmlFile, xmlNode);

        xmlNode->SetAttribute("DeadTimeActive", CStr(m_deadTimerActive).c_str());
        if (m_deadTimerActive)
        {
            xmlNode->SetAttribute("DeadTimer", CStr(m_deadTimer).c_str());
            // m3d::XmlNodeSetAttribute<bool>, inlined.
            xmlNode->SetAttribute("TestVisibility", CStr(static_cast<int>(m_testVisibility)).c_str());
        }
        xmlNode->SetAttribute("Enabled", CStr(m_enabled).c_str());
        xmlNode->SetAttribute("ModelName", m_modelName.c_str());
        xmlNode->SetAttribute("Mass", CStr(m_mass).c_str());
        xmlNode->SetAttribute("Size", CStr(static_cast<unsigned>(m_Members.size())).c_str());

        CVector const initialNodePos = m_initialNodeTransform.getOrg();
        Quaternion initialNodeRot;
        initialNodeRot.FromMatrix(m_initialNodeTransform);
        xmlNode->SetAttribute("InitialNodePos", CStr(initialNodePos).c_str());
        xmlNode->SetAttribute("InitialNodeRot", CStr(initialNodeRot).c_str());

        for (unsigned i = 0; i < m_Members.size(); ++i)
        {
            GeomObj const* const member = m_Members[i];
            xmlNode->SetAttribute((CStr(i) + CStr("_Pos")).c_str(), CStr(member->GetPosition()).c_str());
            xmlNode->SetAttribute((CStr(i) + CStr("_Rot")).c_str(), CStr(member->GetRotation()).c_str());
            xmlNode->SetAttribute(
                (CStr(i) + CStr("_LinVel")).c_str(), CStr(member->GetLinearVelocity()).c_str());
            xmlNode->SetAttribute(
                (CStr(i) + CStr("_AngVel")).c_str(), CStr(member->GetAngularVelocity()).c_str());
        }
    }

    void CompositeObj::RelinkSceneGraphNode()
    {
        // RVA 0x80E230 - NOTE: the node is not checked for null.
        m_node->GetGraph()->RelinkNode(m_node, false);
    }

    void CompositeObj::PostCollide()
    {
        // RVA 0x812A20 - the pieces were created overlapping in the temporary space; this is where they
        // are collided against the world once and then join it for good.
        m_enabled = true;
        for (GeomObj* const member : m_Members)
        {
            member->EnablePhysics();
        }
        for (GeomObj* const member : m_Members)
        {
            dSpaceCollide2((dxGeom*)ai::gGlobalSpace, member->GetGeom()->GetGeomId(), nullptr, ai::NearCallback);
        }
        for (GeomObj* const member : m_Members)
        {
            member->TransferToSpace(ai::gGlobalSpace);
        }
    }

    Quaternion CompositeObj::GetRotation()
    {
        // RVA 0x813630 - the whole is never rotated; each member carries its own rotation.
        return IdentityQuaternion;
    }

    void CompositeObj::SetDeadTimer(int resttime, bool testVisibility)
    {
        // RVA 0x8130B0 - how long the wreckage lies around before it is taken away.
        m_testVisibility = testVisibility;
        m_deadTimerActive = true;
        m_deadTimer = resttime * 0.001;
        if (GetId() != -1)
        {
            theObjects->AddObjToUpdate(this);
        }
    }

    void CompositeObj::Init(
        CStr const& modelName,
        CVector const& pos,
        Quaternion const& rot,
        float mass,
        m3d::SgNode* toAccept)
    {
        // RVA 0x8137E0 - takes a model apart into one physical box per hierarchy geom, each weighing its
        // share of the whole by volume. The model itself stops drawing itself and is driven from the
        // pieces afterwards.
        m_mass = mass;
        m_modelName = modelName;
        m_node = toAccept ? toAccept : PhysicBody::CreateEffectNode(modelName, pos, rot, false, 1.0f);
        m_node->GetProperty(1u, &m_Anim);
        m_node->GetServer()->GetItemProperty(m_node->GetServerHandle(), 16394, &m_Model);

        if (!m_Anim || !m_Model || m_Model->m_HierGeoms.empty() || m_Anim->IsEmpty())
        {
            Remove();
            return;
        }

        m_enabled = true;
        m_initialNodeTransform = m_node->GetCurrentMatrix();
        m_Position = m_initialNodeTransform.getOrg();
        m_Anim->SetEmpty();

        float summarVolume = 0.0f;
        for (unsigned i = 0; i < m_Model->m_HierGeoms.size(); ++i)
        {
            m3d::DRAFT_HierGeom const* const hierGeom = m_Model->GetHierGeom(i);
            summarVolume += hierGeom->geom.Sizes.BoxSizes.x * hierGeom->geom.Sizes.BoxSizes.y *
                hierGeom->geom.Sizes.BoxSizes.z;
        }

        int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("Member"));
        for (unsigned i = 0; i < m_Model->m_HierGeoms.size(); ++i)
        {
            m3d::DRAFT_HierGeom const* const hierGeom = m_Model->GetHierGeom(i);
            CVector const size(
                hierGeom->geom.Sizes.BoxSizes.x,
                hierGeom->geom.Sizes.BoxSizes.y,
                hierGeom->geom.Sizes.BoxSizes.z);
            float const partMass = size.x * size.y * size.z / summarVolume * mass;

            int const objId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
            // NOTE: the new member is not checked for null.
            auto* const member = (GeomObj*)theObjects->GetEntityByObjId(objId);
            member->SetGeom(size, partMass, 1.0f, false);

            // Where the bone that owns this geom currently is, in world space.
            CMatrix const boneToWorld =
                m_Anim->GetBoneAnim(hierGeom->parentBone).m_curMatrix * m_initialNodeTransform;
            member->SetPosition(boneToWorld.vecMul(hierGeom->geom.Translation));

            CMatrix geomToWorld = hierGeom->geom.Rotation.ToMatrix() * boneToWorld;
            Quaternion geomRotation;
            geomRotation.FromMatrix(geomToWorld);
            member->SetRotation(geomRotation);

            m_Members.push_back(member);

            // The piece becomes a child of the wreck, and takes its side.
            Obj* const oldParent = member->GetParent();
            if (oldParent)
            {
                if (member->m_hierarchyType)
                {
                    oldParent->RemoveComponent(member);
                }
                else
                {
                    oldParent->RemoveChild(member);
                }
            }
            else
            {
                member->m_parentId = -1;
            }
            member->m_parentId = GetId();
            member->m_hierarchyType = HIERARCHY_CHILD;
            if (member->GetParent())
            {
                member->SetBelong(member->GetParent()->GetBelong());
            }
        }

        // Remember how to get from each bone back to its geom, so that Update can drive the bones.
        for (unsigned i = 0; i < m_Model->m_HierGeoms.size(); ++i)
        {
            m3d::DRAFT_HierGeom const* const hierGeom = m_Model->GetHierGeom(i);
            CMatrix geomToParent;
            geomToParent.rotTranslate(hierGeom->geom.Rotation, hierGeom->geom.Translation);
            m_boneToGeom.push_back(geomToParent.getInverse());
            m_bonesIndices.push_back(hierGeom->parentBone);
        }

        for (GeomObj* const member : m_Members)
        {
            member->TransferToSpace(ai::gTempSpace);
        }

        // The node no longer animates itself.
        m_node->GetGraph()->m_thinkList.erase(m_node);

        m_Anim->m_curBox = m_Model->m_box;
        for (int i = 0; i < m_Model->m_header.m_numNodes; ++i)
        {
            m_Anim->GetBoneAnim(i).m_lastUpdatedFrame = 0;
        }

        for (GeomObj* const member : m_Members)
        {
            dGeomSetCategoryBits(member->GetGeom()->GetGeomId(), 3u);
            dGeomSetCollideBits(member->GetGeom()->GetGeomId(), 0xFFFCu);
        }
    }

    m3d::Class* CompositeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void CompositeObj::RenderDebugInfo() const
    {
        // RVA 0x80EB50 - the model's own bounding box, moved to where the wreckage is.
        Aabb aabb = m_Anim->m_curBox;
        aabb.m_box[0] += m_Position.x;
        aabb.m_box[1] += m_Position.y;
        aabb.m_box[2] += m_Position.z;
        aabb.m_box[3] += m_Position.x;
        aabb.m_box[4] += m_Position.y;
        aabb.m_box[5] += m_Position.z;
        aabb.Draw(0xFFFF0000);
    }

    void CompositeObj::Remove()
    {
        // RVA 0x810060 - the pieces go with the whole.
        Obj::Remove();
        for (GeomObj* const member : m_Members)
        {
            member->Remove();
        }
    }

    CompositeObjPrototypeInfo const* CompositeObj::GetPrototypeInfo() const
    {
        // RVA 0x811270
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), CompositeObjPrototypeInfo const);
    }

    void CompositeObj::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x8150F0 - only reads the save; the wreck itself is rebuilt later, in _InternalPostLoad.
        Obj::LoadRuntimeValues(xmlFile, xmlNode);

        m3d::SafeBoolAttrib(m_deadTimerActive, xmlNode, "DeadTimeActive");
        if (m_deadTimerActive)
        {
            if (!xmlNode->IsEmpty())
            {
                char const* const deadTimer = xmlNode->GetAttribute("DeadTimer");
                if (deadTimer)
                {
                    m_deadTimer = static_cast<float>(atof(deadTimer));
                }
            }
            m3d::SafeBoolAttrib(m_testVisibility, xmlNode, "TestVisibility");
        }

        unsigned size = 0;
        if (!xmlNode->IsEmpty())
        {
            char const* const sizeStr = xmlNode->GetAttribute("Size");
            if (sizeStr)
            {
                int const value = atoi(sizeStr);
                if (value >= 0)
                {
                    size = value;
                }
            }
        }

        m3d::SafeBoolAttrib(m_enabled, xmlNode, "Enabled");
        m3d::SafeStrAttrib(m_modelName, xmlNode, "ModelName");
        if (!xmlNode->IsEmpty())
        {
            char const* const mass = xmlNode->GetAttribute("Mass");
            if (mass)
            {
                m_mass = static_cast<float>(atof(mass));
            }
        }

        Quaternion initialNodeRot = IdentityQuaternion;
        CVector initialNodePos(0.0, 0.0, 0.0);
        m3d::SafeVectorAttrib(initialNodePos, xmlNode, "InitialNodePos");
        m3d::SafeQuaternionAttrib(initialNodeRot, xmlNode, "InitialNodeRot");
        m_initialNodeTransform.rotTranslate(initialNodeRot, initialNodePos);

        m_dataForLoad.resize(size);
        for (unsigned i = 0; i < size; ++i)
        {
            GoDataForLoad& data = m_dataForLoad[i];
            m3d::SafeVectorAttrib(data.pos, xmlNode, (CStr(i) + CStr("_Pos")).c_str());
            m3d::SafeQuaternionAttrib(data.rot, xmlNode, (CStr(i) + CStr("_Rot")).c_str());
            m3d::SafeVectorAttrib(data.linVel, xmlNode, (CStr(i) + CStr("_LinVel")).c_str());
            m3d::SafeVectorAttrib(data.angVel, xmlNode, (CStr(i) + CStr("_AngVel")).c_str());
        }
    }

    void CompositeObj::InitImpulses(CVector causePos, float force)
    {
        // RVA 0x80F9E0 - throws every piece away from where the blow landed, and gives each a random spin.
        for (GeomObj* const member : m_Members)
        {
            dReal const* const geomPos = dGeomGetPosition(member->GetGeom()->GetGeomId());
            CVector const delta(
                geomPos[0] - causePos.x, geomPos[1] - causePos.y, geomPos[2] - causePos.z);
            float const invLen =
                1.0 / sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z + 0.00000011920929);

            CVector impulse;
            impulse.x = delta.x * invLen * force;
            // NOTE: the upward part of the push is always positive, so a piece below the blow is still
            // thrown upwards.
            impulse.y = fabs(delta.y * invLen) * force;
            impulse.z = delta.z * invLen * force;
            member->AddImpulse(impulse);

            CVector rotVel(static_cast<float>(rand()), static_cast<float>(rand()), static_cast<float>(rand()));
            float const invRotLen =
                1.0 / sqrt(rotVel.x * rotVel.x + rotVel.y * rotVel.y + rotVel.z * rotVel.z + 0.00000011920929);
            float const spin = force / member->GetMass();
            member->SetAngularVelocity(rotVel * (invRotLen * spin));
        }
    }

    CompositeObj::~CompositeObj()
    {
        // RVA 0x813690 - the members are ordinary objects and go through the container; only the scene
        // node is this object's own to drop.
        if (m_node)
        {
            m_node->GetGraph()->RemoveNode(m_node);
            m_node = nullptr;
        }
    }

    void CompositeObj::CreateBreakableJoints()
    {
        // RVA 0x814890 - welds together every pair of pieces that still touch, so the wreck holds its
        // shape until something knocks it apart. Each piece is tested with a copy of its own box grown by
        // a tenth, so pieces that merely sit against one another count as touching.
        static scoped_ptr<Box> box(ai::Box::CreateObject(nullptr, CVector(0.0, 0.0, 0.0), nullptr));
        static scoped_ptr<Box> ground(ai::Box::CreateObject(nullptr, CVector(100.0, 2.0, 100.0), nullptr));
        dGeomSetPosition(ground->GetGeomId(), 0.0, -1.0, 0.0);

        m_connections.clear();
        m_joints.clear();

        for (unsigned i = 0; i < m_Members.size(); ++i)
        {
            for (unsigned j = 0; j < m_Members.size(); ++j)
            {
                if (i == j)
                {
                    continue;
                }
                // The pair may already have been welded the other way round.
                if (std::find(m_connections.begin(), m_connections.end(), std::make_pair((int)j, (int)i)) !=
                    m_connections.end())
                {
                    continue;
                }

                Geom* const memberGeom = m_Members[i]->GetGeom();
                dReal const* const memberPos = dGeomGetPosition(memberGeom->GetGeomId());
                dGeomSetPosition(box->GetGeomId(), memberPos[0], memberPos[1], memberPos[2]);
                dQuaternion memberQuat;
                dGeomGetQuaternion(memberGeom->GetGeomId(), memberQuat);
                dGeomSetQuaternion(box->GetGeomId(), memberQuat);
                box->SetSize(((Box*)memberGeom)->GetSize() * 1.1);

                dContact contact;
                if (!dCollide(box->GetGeomId(), m_Members[j]->GetGeom()->GetGeomId(), 1, &contact.geom, 44))
                {
                    continue;
                }

                dxJoint* const joint = dJointCreateFixed(ai::gGlobalWorld, nullptr);
                m_connections.push_back(std::make_pair((int)i, (int)j));
                m_joints.push_back(joint);
                dJointAttach(joint, m_Members[i]->GetBody()->id(), m_Members[j]->GetBody()->id());
                dJointSetFixed(joint);
            }
        }
    }

    void CompositeObj::_InternalPostLoad()
    {
        // RVA 0x814D20 - a wreck read back from a save is rebuilt from its model and then each piece is
        // put back where the save said it was.
        if (m_dataForLoad.empty())
        {
            return;
        }

        CMatrix const oldNodeInitialTransform = m_initialNodeTransform;
        Init(m_modelName, CVector(0.0, 0.0, 0.0), IdentityQuaternion, m_mass, nullptr);
        m_initialNodeTransform = oldNodeInitialTransform;
        PutToGlobalSpace();

        M3D_ASSERT(m_dataForLoad.size() == m_Members.size());
        for (unsigned i = 0; i < m_dataForLoad.size(); ++i)
        {
            GeomObj* const member = m_Members[i];
            GoDataForLoad const& data = m_dataForLoad[i];
            member->SetPosition(data.pos);
            member->SetRotation(data.rot);
            member->SetLinearVelocity(data.linVel);
            member->SetAngularVelocity(data.angVel);
        }
        m_dataForLoad.clear();
    }

    void CompositeObj::RecalcBoundBox()
    {
        // RVA 0x80FBD0 - the wreck's own box is whatever its pieces currently span. A piece that has left
        // the level is frozen rather than counted.
        Aabb worldBox;
        worldBox.m_box[0] = 10000.0f;
        worldBox.m_box[1] = 10000.0f;
        worldBox.m_box[2] = 10000.0f;
        worldBox.m_box[3] = -10000.0f;
        worldBox.m_box[4] = -10000.0f;
        worldBox.m_box[5] = -10000.0f;

        for (unsigned i = 0; i < m_Model->m_HierGeoms.size(); ++i)
        {
            GeomObj* const member = m_Members[i];
            dReal const* const pos = dGeomGetPosition(member->GetGeom()->GetGeomId());
            if (pos[0] < 0.0 || pos[2] < 0.0 || pos[0] >= pServer->GetLevelSize() ||
                pos[2] >= pServer->GetLevelSize() || pos[1] <= -1000.0 || pos[1] >= 10000.0)
            {
                member->DisablePhysics();
                member->DisableGeometry(true);
                continue;
            }
            worldBox.EmbraceBox(member->GetGeom()->GetAabb());
        }

        m_Position.x = (worldBox.m_box[0] + worldBox.m_box[3]) * 0.5;
        m_Position.y = (worldBox.m_box[1] + worldBox.m_box[4]) * 0.5;
        m_Position.z = (worldBox.m_box[2] + worldBox.m_box[5]) * 0.5;

        // The model's own box is kept relative to that centre.
        m_Anim->m_curBox.m_box[0] = worldBox.m_box[0] - m_Position.x;
        m_Anim->m_curBox.m_box[1] = worldBox.m_box[1] - m_Position.y;
        m_Anim->m_curBox.m_box[2] = worldBox.m_box[2] - m_Position.z;
        m_Anim->m_curBox.m_box[3] = worldBox.m_box[3] - m_Position.x;
        m_Anim->m_curBox.m_box[4] = worldBox.m_box[4] - m_Position.y;
        m_Anim->m_curBox.m_box[5] = worldBox.m_box[5] - m_Position.z;

        m_node->SetOriginAbs(m_Position);
        m_node->SetRotation(IdentityQuaternion);
        m_node->m_isOwnBoundingBoxDirty = true;
    }

    void CompositeObj::PutToGlobalSpace()
    {
        // RVA 0x80FE20
        for (GeomObj* const member : m_Members)
        {
            member->TransferToSpace(ai::gGlobalSpace);
        }
    }

    void CompositeObj::PutToTempSpace()
    {
        // RVA 0x80FE60 - while the pieces are still overlapping they are kept out of the world's space.
        for (GeomObj* const member : m_Members)
        {
            member->TransferToSpace(ai::gTempSpace);
        }
    }

    void CompositeObj::CheckDisablePhysics()
    {
        // RVA 0x80FF10 - a piece that has come to rest is put to sleep. One left hanging above the ground
        // is only allowed to sleep until something touches it again.
        for (unsigned i = 0; i < m_Members.size(); ++i)
        {
            GeomObj* const member = m_Members[i];
            if ((member->GetPhysicState() & 1) == 0)
            {
                continue;
            }

            CVector const vel = member->GetLinearVelocity();
            if (vel.x * vel.x + vel.y * vel.y + vel.z * vel.z >= 0.0080000004)
            {
                continue;
            }
            CVector const rot = member->GetAngularVelocity();
            if (rot.x * rot.x + rot.y * rot.y + rot.z * rot.z >= 0.2)
            {
                continue;
            }

            CVector const pos = member->GetPosition();
            float const lsHeight = m3d::pClient->GetWorld().GetLandscape().GetLsHeight(pos.x, pos.z);
            if (fabs(pos.y - lsHeight) >= 0.2)
            {
                member->DisablePhysicsWithAutoEnable();
                continue;
            }
            member->DisablePhysics();
            member->DisableGeometry(true);
        }
    }

    m3d::Object* CompositeObj::CreateObject()
    {
        // RVA 0x80F390
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* CompositeObj::Clone()
    {
        // RVA 0x80F1D0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai

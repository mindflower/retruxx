#include "jointedobj.h"
#include "compositeobj.h"
#include "geomobj.h"
#include "prototypemanager.h"
#include "server/objects/breakableobject.h"
#include "server/objects/physicbodies/geoms/geom.h"
#include "server/objects/physicbodies/physicbody.h"
#include "server/objects/physicbodies/physichelpers.h"
#include <stdexcept>

#include <core/clazz.h>
#include <core/kernel.h>
#include <landscape.h>
#include <client.h>
#include <skelmodel.h>
#include <scene/nodes/sgnode.h>
#include <scene/servers/dataserver.h>
#include <math/coremath.h>
#include <core/timer.h>
#include <scene/scenegraph.h>
#include <server/dynamicscene.h>
#include <server/server.h>
#include <server/objects/base/objcontainer.h>
#include <world.h>

#include <ode/objects.h>
#include <ode/odecpp.h>

#include <algorithm>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(JointedObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(JointedObj);

    ExternalJointInfo::ExternalJointInfo()
    {
        // RVA 0x816890
        jointPos = ZeroVector;
        connectedToStatics = true;
    }

    ExternalJointInfo::ExternalJointInfo(ExternalJointInfo const& that)
    {
        // RVA 0x816E20 - plain memberwise copy.
        jointPos = that.jointPos;
        lpName = that.lpName;
        connectedToStatics = that.connectedToStatics;
    }

    JointedObjPrototypeInfo::JointedObjPrototypeInfo() = default;

    bool JointedObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
    }

    ai::Obj* JointedObjPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x8256D0
        return new JointedObj(*this);
    }

    void JointedObj::CalcSplineNeighbours()
    {
        // RVA 0x821990 - a rope is simulated with a physical body only at its jointed bones;
        // every bone in between is drawn along a spline through the four nearest of them, and
        // this works out those four control points and where each bone sits between them.
        unsigned const numBones = m_Model->GetNumBones();

        retruxx::vector<CVector> bonesPoses(numBones);
        for (unsigned i = 0; i < numBones; ++i)
        {
            CMatrix const boneMatrix = m_Model->GetBoneMatrix(i);
            bonesPoses[i] = CVector(boneMatrix._41, boneMatrix._42, boneMatrix._43);
        }

        // The bones that are not interpolated: the jointed ones, plus every root and every leaf.
        retruxx::vector<unsigned> notSplineBones(m_jointsIndices);
        for (unsigned i = 0; i < numBones; ++i)
        {
            if (m_Model->GetBone(i).m_parentIdx < 0)
                notSplineBones.push_back(i);
        }
        for (unsigned i = 0; i < numBones; ++i)
        {
            bool hasChild = false;
            for (unsigned j = 0; j < numBones; ++j)
            {
                if (m_Model->GetBone(j).m_parentIdx == static_cast<int>(i))
                    hasChild = true;
            }
            if (!hasChild)
                notSplineBones.push_back(i);
        }

        std::sort(notSplineBones.begin(), notSplineBones.end());
        notSplineBones.erase(
            std::unique(notSplineBones.begin(), notSplineBones.end()), notSplineBones.end());

        m_splineNeighbours.clear();
        for (unsigned bone = 0; bone < numBones; ++bone)
        {
            if (std::find(notSplineBones.begin(), notSplineBones.end(), bone) != notSplineBones.end())
                continue;

            retruxx::vector<unsigned> parents;
            retruxx::vector<unsigned> childs;
            for (auto const notSpline : notSplineBones)
            {
                if (IsParent(notSpline, bone))
                    parents.push_back(notSpline);
            }
            for (auto const notSpline : notSplineBones)
            {
                if (IsParent(bone, notSpline))
                    childs.push_back(notSpline);
            }

            // Nearest first, so that [0] is the neighbour on each side.
            CVector const bonePos = bonesPoses[bone];
            auto const byDistance = [&bonesPoses, &bonePos](unsigned a, unsigned b) {
                return (bonesPoses[a] - bonePos).length() < (bonesPoses[b] - bonePos).length();
            };
            std::sort(parents.begin(), parents.end(), byDistance);
            std::sort(childs.begin(), childs.end(), byDistance);

            auto& splineBones = m_splineNeighbours[bone];

            // The four control points, outermost first: the bone sits between [1] and [2].
            // With only one neighbour on a side that neighbour is used twice.
            if (!parents.empty())
            {
                splineBones.bones.push_back(parents.size() >= 2 ? parents[1] : parents[0]);
                splineBones.bones.push_back(parents[0]);
            }
            if (!childs.empty())
            {
                splineBones.bones.push_back(childs[0]);
                splineBones.bones.push_back(childs.size() >= 2 ? childs[1] : childs[0]);
            }

            // NOTE: the spline parameter is measured on z alone, so a rope strung along x or y
            // divides by zero here.
            splineBones.t = (bonesPoses[bone].z - bonesPoses[splineBones.bones[1]].z) /
                (bonesPoses[splineBones.bones[2]].z - bonesPoses[splineBones.bones[1]].z);
        }
    }

    void JointedObj::Init(CStr const& modelName, CVector const& pos, Quaternion const& rot, float mass,
        m3d::SgNode* toAccept, float strechZ)
    {
        // RVA 0x822490 - builds the ragdoll: one physical box per hierarchy geom of the model,
        // a universal joint wherever two of those sit on bones that are related, and an extra
        // anchor body for any chain that reaches the root without meeting another geom.
        m_mass = mass;
        m_modelName = modelName;

        // Either adopt the node the dying object handed over, or spawn one of our own.
        m_node = toAccept ? toAccept : PhysicBody::CreateEffectNode(modelName, pos, rot, false, 1.0f);
        m_Anim = GetNodeAnimInfo(m_node);
        m_node->GetServer()->GetItemProperty(m_node->GetServerHandle(), m3d::PROP_INTERNAL_GETMODEL, &m_Model);

        if (!m_Anim || !m_Model || m_Model->GetNumHierGeoms() == 0 || m_Anim->IsEmpty())
        {
            // Nothing to build a body out of, so the object is of no use.
            Remove();
            return;
        }

        if (fabs(strechZ - 1.0) > 0.0000099999997)
        {
            // Ropes are stretched by pushing every bone further along its own z.
            for (unsigned i = 0; i < m_Model->GetNumBones(); ++i)
            {
                m_Anim->GetBoneAnim(i).m_curMatrix._43 *= strechZ;
            }
            m_strech = strechZ;
        }

        m_enabled = true;

        CMatrix const matWorld = m_node->GetCurrentMatrix();
        m_Position = CVector(matWorld._41, matWorld._42, matWorld._43);

        // From here the bones are driven by the physics, so the animation system is told the
        // node has nothing left to play and is taken off the think list.
        m_Anim->m_Empty = true;
        m_node->GetGraph()->m_thinkList.erase(m_node);
        m_Anim->m_curBox = m_Model->m_box;

        // The mass is shared out between the members in proportion to their volume.
        float totalVolume = 0.0f;
        for (unsigned i = 0; i < m_Model->GetNumHierGeoms(); ++i)
        {
            auto const& boxSizes = m_Model->GetHierGeom(i)->geom.Sizes.BoxSizes;
            totalVolume += boxSizes.z * boxSizes.y * boxSizes.x;
        }

        for (unsigned i = 0; i < m_Model->GetNumHierGeoms(); ++i)
        {
            auto const* hierGeom = m_Model->GetHierGeom(i);
            CVector const size(
                hierGeom->geom.Sizes.BoxSizes.x, hierGeom->geom.Sizes.BoxSizes.y, hierGeom->geom.Sizes.BoxSizes.z);
            float const partMass = (size.y / totalVolume) * size.z * size.x * mass;

            int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("Member"));
            int const objId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
            auto* member = static_cast<GeomObj*>(theObjects->GetEntityByObjId(objId));

            // A rope's links are packed tighter than a body's limbs.
            member->SetGeom(size, partMass, m_asRope ? 1.5f : 1.8f, false);

            // Where that bone currently sits in the world.
            CMatrix const mat = m_Anim->GetBoneAnim(hierGeom->parentBone).m_curMatrix * matWorld;

            CVector const& translation = hierGeom->geom.Translation;
            member->SetPosition(CVector(
                translation.x * mat._11 + translation.y * mat._21 + translation.z * mat._31 + mat._41,
                translation.x * mat._12 + translation.y * mat._22 + translation.z * mat._32 + mat._42,
                translation.x * mat._13 + translation.y * mat._23 + translation.z * mat._33 + mat._43));

            CMatrix const rotMatr = hierGeom->geom.Rotation.ToMatrix() * mat;
            Quaternion memberRot;
            memberRot.FromMatrix(rotMatr);
            member->SetRotation(memberRot);

            m_Members.push_back(member);
            member->LinkToParent(GetId(), HIERARCHY_CHILD);
        }

        // A chain whose topmost geom still has bones above it has nothing to hang from, so a
        // small anchor body is made for each root bone to joint against.
        for (unsigned i = 0; i < m_Model->GetNumBones(); ++i)
        {
            auto const& bone = m_Model->GetBone(i);
            if (bone.m_parentIdx >= 0)
                continue;

            int const prototypeId = thePrototypeManager->GetPrototypeId(CStr("Member"));
            int const objId = theObjects->CreateNewObject(prototypeId, "", -1, -1);
            auto* anchor = static_cast<GeomObj*>(theObjects->GetEntityByObjId(objId));

            anchor->SetGeom(CVector(0.1f, 0.1f, 0.1f), 1.0f, 1.0f, true);

            CMatrix const& boneMat = m_Anim->GetBoneAnim(bone.m_ownIdx).m_curMatrix;
            anchor->SetPosition(CVector(
                boneMat._41 * matWorld._11 + boneMat._42 * matWorld._21 + boneMat._43 * matWorld._31 +
                    boneMat._44 * matWorld._41,
                boneMat._41 * matWorld._12 + boneMat._42 * matWorld._22 + boneMat._43 * matWorld._32 +
                    boneMat._44 * matWorld._42,
                boneMat._41 * matWorld._13 + boneMat._42 * matWorld._23 + boneMat._43 * matWorld._33 +
                    boneMat._44 * matWorld._43));

            // It exists only to be jointed to, so it never collides with anything.
            anchor->DisableGeometry(true);
            m_ExtraMembers[bone.m_ownIdx] = anchor;
        }

        for (unsigned i = 0; i < m_Model->GetNumHierGeoms(); ++i)
        {
            auto const* hierGeom = m_Model->GetHierGeom(i);

            // Walk up the bone chain looking for the nearest ancestor that owns a geom.
            int connectedGeomNum = -1;
            int parentBone = hierGeom->parentBone;
            for (;;)
            {
                parentBone = m_Model->GetBone(parentBone).m_parentIdx;
                for (unsigned j = 0; j < m_Model->GetNumHierGeoms(); ++j)
                {
                    if (m_Model->GetHierGeom(j)->parentBone == parentBone)
                        connectedGeomNum = j;
                }
                if (parentBone < 0 || connectedGeomNum >= 0)
                    break;
            }

            dBody* otherBody = nullptr;
            if (connectedGeomNum >= 0)
            {
                otherBody = m_Members[connectedGeomNum]->GetBody();
            }
            else
            {
                // Nothing above it owns a geom, so it hangs off the root bone's anchor.
                int rootBone = hierGeom->parentBone;
                while (m_Model->GetBone(rootBone).m_parentIdx >= 0)
                {
                    rootBone = m_Model->GetBone(rootBone).m_parentIdx;
                }
                otherBody = m_ExtraMembers[rootBone]->GetBody();
            }

            auto* joint = dJointCreateUniversal(gGlobalWorld, nullptr);
            m_Joints.push_back(joint);
            m_jointsIndices.push_back(hierGeom->parentBone);
            dJointAttach(joint, m_Members[i]->GetBody()->id(), otherBody->id());

            // The anchor is the bone's own origin, in world space.
            CMatrix const& boneMat = m_Anim->GetBoneAnim(hierGeom->parentBone).m_curMatrix;
            dJointSetUniversalAnchor(joint,
                boneMat._41 * matWorld._11 + boneMat._42 * matWorld._21 + boneMat._43 * matWorld._31 +
                    boneMat._44 * matWorld._41,
                boneMat._41 * matWorld._12 + boneMat._42 * matWorld._22 + boneMat._43 * matWorld._32 +
                    boneMat._44 * matWorld._42,
                boneMat._41 * matWorld._13 + boneMat._42 * matWorld._23 + boneMat._43 * matWorld._33 +
                    boneMat._44 * matWorld._43);

            int const boneParent = m_Anim->GetBoneAnim(hierGeom->parentBone).m_parentIdx;
            if (boneParent >= 0)
            {
                // ODE reads the axes off the bodies as they stand, so the member is posed at the
                // bone's rest orientation while they are set, then put back where it was.
                CVector const activePos = m_Members[i]->GetPosition();
                Quaternion const activeRot = m_Members[i]->GetRotation();

                auto const& restBone = m_Model->GetBone(hierGeom->parentBone);
                CMatrix restMat;
                restMat.rotTranslate(restBone.m_quaternion0, restBone.m_translation0);

                CMatrix const jointMat =
                    restMat * m_Anim->GetBoneAnim(boneParent).m_curMatrix * matWorld;

                m_Members[i]->SetPosition(CVector(jointMat._41, jointMat._42, jointMat._43));
                Quaternion jointRot;
                jointRot.FromMatrix(jointMat);
                m_Members[i]->SetRotation(jointRot);

                // NOTE: both axes are read two floats past the start of their row, so what ends
                // up in them is (_23, _24, _31) and (_33, _34, _41) rather than the y and z axes
                // of the joint frame that the surrounding code plainly intends. Kept as shipped.
                dJointSetUniversalAxis1(joint, restMat._23, restMat._24, restMat._31);
                dJointSetUniversalAxis2(joint, restMat._33, restMat._34, restMat._41);

                // The swing limits come from the model, clamped away from zero so the joint
                // always has somewhere to move.
                auto const& minBounds = m_Model->GetBoneBounds(hierGeom->parentBone);
                float loStop1 = minBounds.MinRot.y;
                float loStop2 = minBounds.MinRot.z;

                auto const& maxBounds = m_Model->GetBoneBounds(hierGeom->parentBone);
                float hiStop1 = maxBounds.MaxRot.y;
                float hiStop2 = maxBounds.MaxRot.z;

                loStop1 = loStop1 < -3.14159265f ? -3.14159265f : (loStop1 > -0.00999999978f ? -0.00999999978f : loStop1);
                loStop2 = loStop2 < -3.14159265f ? -3.14159265f : (loStop2 > -0.00999999978f ? -0.00999999978f : loStop2);
                hiStop1 = hiStop1 < 0.00999999978f ? 0.00999999978f : (hiStop1 > 3.14159265f ? 3.14159265f : hiStop1);
                hiStop2 = hiStop2 < 0.00999999978f ? 0.00999999978f : (hiStop2 > 3.14159265f ? 3.14159265f : hiStop2);

                // NOTE: MinRot.x / MaxRot.x go unused - a universal joint only has two axes.
                dJointSetUniversalParam(joint, dParamLoStop, loStop1);
                dJointSetUniversalParam(joint, dParamLoStop2, loStop2);
                dJointSetUniversalParam(joint, dParamHiStop, hiStop1);
                dJointSetUniversalParam(joint, dParamHiStop2, hiStop2);

                m_Members[i]->SetPosition(activePos);
                m_Members[i]->SetRotation(activeRot);
            }

            SetUniversalJointParams(joint);

            // Remembered so that Update can map a member's body back onto its bone.
            CMatrix geomToParent;
            geomToParent.rotTranslate(hierGeom->geom.Rotation, hierGeom->geom.Translation);
            m_JointToGeom.push_back(geomToParent.getInverse());
        }

        if (m_asRope)
        {
            CalcSplineNeighbours();

            // A rope collides with the world but never with itself or another rope.
            for (auto* member : m_Members)
            {
                dGeomSetCategoryBits(member->GetGeom()->GetGeomId(), 0xFF000000);
                dGeomSetCollideBits(member->GetGeom()->GetGeomId(), 0xFF0000);
            }
            for (auto const& extra : m_ExtraMembers)
            {
                dGeomSetCategoryBits(extra.second->GetGeom()->GetGeomId(), 0xFF000000);
                dGeomSetCollideBits(extra.second->GetGeom()->GetGeomId(), 0xFF0000);
            }

            // The link pinned to the root bone is what holds the rope up, so it must not be
            // pushed around by collisions.
            for (unsigned i = 0; i < m_Model->GetNumHierGeoms(); ++i)
            {
                if (m_Model->GetBone(m_Model->GetHierGeom(i)->parentBone).m_parentIdx < 0)
                {
                    m_Members[i]->DisableGeometry(true);
                }
            }
        }

        PutToNewSpace(gTempSpace);

        // Force every bone to be re-interpolated on the next frame.
        for (unsigned i = 0; i < m_Model->GetNumBones(); ++i)
        {
            m_Anim->GetBoneAnim(i).m_lastUpdatedFrame = 0;
        }
    }

    JointedObj::JointedObj(JointedObjPrototypeInfo const& prototypeInfo) : Obj(prototypeInfo)
    {
        // RVA 0x822290 - everything else is built by Init once the model is known.
        m_MembersSpace = nullptr;
        m_onLoad = false;
        m_node = nullptr;
        m_Model = nullptr;
        m_Anim = nullptr;
        m_mass = 10.0f;
        m_strech = 1.0f;
        m_Position = ZeroVector;
        m_enabled = false;
        m_asRope = false;
        m_deadTimerActive = false;
        m_testVisibility = false;
        m_disableTimerActive = false;
        m_deadTimer = 0.0f;
        m_disableTimer = 0.0f;
    }

    void JointedObj::AddExternalJoint(int IdAttachTo, CVector attachPos, CStr lpName)
    {
        // RVA 0x81F060 - pins the member that hangs off a named load point to another object,
        // or to nothing at all, which leaves it hanging in the air.
        dxBody* attachToBody = nullptr;
        auto* attachTo = theObjects->GetEntityByObjId(IdAttachTo);
        if (IS_KIND_OF(attachTo, BreakableObject))
        {
            attachToBody = static_cast<BreakableObject*>(attachTo)->GetBody()->id();
        }

        int const loadPointId = m_Model->GetLoadPointIdByName(lpName.c_str());
        if (loadPointId == -1)
            return;

        for (unsigned i = 0; i < m_Model->GetNumHierGeoms(); ++i)
        {
            if (m_Model->GetHierGeom(i)->parentBone != loadPointId)
                continue;

            auto* member = m_Members[i];
            if (!member)
                continue;

            auto* joint = dJointCreateBall(gGlobalWorld, nullptr);
            dJointAttach(joint, attachToBody, member->GetBody()->id());
            dJointSetBallAnchor(joint, attachPos.x, attachPos.y, attachPos.z);
            m_externalJoints.push_back(joint);

            // Coming back from a save the info vector was already filled by
            // LoadRuntimeValues, so only the ODE joint itself has to be rebuilt.
            if (!m_onLoad)
            {
                ExternalJointInfo eji;
                eji.jointPos = attachPos;
                eji.lpName = lpName;
                // Nothing to attach to means it hangs off the world itself.
                eji.connectedToStatics = (attachToBody == nullptr);
                m_externalJointsInfo.push_back(eji);
            }
            // Only the first geom on that load point is pinned.
            break;
        }
    }

    void JointedObj::SetDisableTimer(int restTime)
    {
        // RVA 0x815BA0 - the timer is kept in seconds, the argument is milliseconds.
        m_disableTimerActive = true;
        m_disableTimer = restTime * 0.001f;
    }

    void JointedObj::SetDeadTimer(int resttime, bool testVisibility)
    {
        // RVA 0x81DBF0 - the timer is kept in seconds, the argument is milliseconds.
        m_testVisibility = testVisibility;
        m_deadTimerActive = true;
        m_deadTimer = resttime * 0.001f;

        // A ragdoll is created without updating; it has to join the update list to count down.
        theObjects->AddObjToUpdate(this);
    }

    void JointedObj::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x818420 - a ragdoll is rebuilt from its model on load, so what has to survive is
        // where every member ended up and how it was moving.
        Obj::SaveRuntimeValues(xmlFile, xmlNode);

        xmlNode->SetAttribute("DeadTimeActive", CStr(static_cast<int>(m_deadTimerActive)).c_str());
        if (m_deadTimerActive)
        {
            xmlNode->SetAttribute("DeadTimer", CStr(m_deadTimer).c_str());
            xmlNode->SetAttribute("TestVisibility", CStr(static_cast<int>(m_testVisibility)).c_str());
        }

        xmlNode->SetAttribute("DisableTimeActive", CStr(static_cast<int>(m_disableTimerActive)).c_str());
        if (m_disableTimerActive)
        {
            xmlNode->SetAttribute("DisableTimer", CStr(m_disableTimer).c_str());
        }

        xmlNode->SetAttribute("AsRope", CStr(static_cast<int>(m_asRope)).c_str());
        xmlNode->SetAttribute("Enabled", CStr(static_cast<int>(m_enabled)).c_str());
        xmlNode->SetAttribute("ModelName", m_modelName.c_str());
        xmlNode->SetAttribute("Mass", CStr(m_mass).c_str());
        xmlNode->SetAttribute("Size", CStr(static_cast<int>(m_Members.size())).c_str());
        xmlNode->SetAttribute("ESize", CStr(static_cast<int>(m_ExtraMembers.size())).c_str());

        for (unsigned i = 0; i < m_Members.size(); ++i)
        {
            auto* member = m_Members[i];
            CStr const prefix = CStr("Member_") + CStr(static_cast<int>(i));
            xmlNode->SetAttribute((prefix + CStr("_Pos")).c_str(), CStr(member->GetPosition()).c_str());
            xmlNode->SetAttribute((prefix + CStr("_Rot")).c_str(), CStr(member->GetRotation()).c_str());
            xmlNode->SetAttribute((prefix + CStr("_LinVel")).c_str(), CStr(member->GetLinearVelocity()).c_str());
            xmlNode->SetAttribute((prefix + CStr("_Ang_vel")).c_str(), CStr(member->GetAngularVelocity()).c_str());
        }

        // The extra members are keyed by bone index, so that index is written alongside.
        unsigned n = 0;
        for (auto const& extra : m_ExtraMembers)
        {
            auto* member = extra.second;
            CStr const prefix = CStr("EMember_") + CStr(static_cast<int>(n));
            xmlNode->SetAttribute((prefix + CStr("_Pos")).c_str(), CStr(member->GetPosition()).c_str());
            xmlNode->SetAttribute((prefix + CStr("_Rot")).c_str(), CStr(member->GetRotation()).c_str());
            xmlNode->SetAttribute((prefix + CStr("_LinVel")).c_str(), CStr(member->GetLinearVelocity()).c_str());
            xmlNode->SetAttribute((prefix + CStr("_Ang_vel")).c_str(), CStr(member->GetAngularVelocity()).c_str());
            xmlNode->SetAttribute((prefix + CStr("_Idx")).c_str(), CStr(static_cast<int>(extra.first)).c_str());
            ++n;
        }

        xmlNode->SetAttribute("Strech", CStr(m_strech).c_str());

        if (!m_externalJointsInfo.empty())
        {
            xmlNode->SetAttribute("EJSize", CStr(static_cast<int>(m_externalJointsInfo.size())).c_str());
            for (unsigned i = 0; i < m_externalJointsInfo.size(); ++i)
            {
                auto const& info = m_externalJointsInfo[i];
                CStr const prefix = CStr("EJoint_") + CStr(static_cast<int>(i));
                xmlNode->SetAttribute((prefix + CStr("_Pos")).c_str(), CStr(info.jointPos).c_str());
                xmlNode->SetAttribute((prefix + CStr("_LpName")).c_str(), info.lpName.c_str());
                xmlNode->SetAttribute(
                    (prefix + CStr("_Static")).c_str(), CStr(static_cast<int>(info.connectedToStatics)).c_str());
            }
        }
    }

    void JointedObj::Remove()
    {
        // RVA 0x81B940 - the members are separate objects, so each has to be removed too.
        Obj::Remove();

        for (auto* member : m_Members)
        {
            member->Remove();
        }
        for (auto const& extra : m_ExtraMembers)
        {
            extra.second->Remove();
        }
    }

    void JointedObj::RelinkSceneGraphNode()
    {
        // RVA 0x815BC0
        m_node->GetGraph()->RelinkNode(m_node, false);
    }

    m3d::Class* JointedObj::GetClass() const
    {
        // RVA 0x815A70
        return RT_CLASS_LOCAL(JointedObj);
    }

    void JointedObj::SetAsRope(bool b)
    {
        // RVA 0x875A60
        m_asRope = b;
    }

    void JointedObj::RenderDebugInfo() const
    {
        // RVA 0x8183D0
        for (auto* joint : m_Joints)
        {
            ai::pServer->GetWorld()->GetLandscape().DrawJoint(joint);
        }
    }

    JointedObjPrototypeInfo const* JointedObj::GetPrototypeInfo() const
    {
        // RVA 0x81B400
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), JointedObjPrototypeInfo const);
    }

    void JointedObj::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x81F5C0 - only parks the saved state; _InternalPostLoad rebuilds the ragdoll and
        // plays it back, because the members do not exist yet at this point.
        Obj::LoadRuntimeValues(xmlFile, xmlNode);

        m3d::SafeBoolAttrib(m_deadTimerActive, xmlNode, "DeadTimeActive");
        if (m_deadTimerActive)
        {
            m3d::SafeFloatAttrib(m_deadTimer, xmlNode, "DeadTimer");
            m3d::SafeBoolAttrib(m_testVisibility, xmlNode, "TestVisibility");
        }

        m3d::SafeBoolAttrib(m_disableTimerActive, xmlNode, "DisableTimeActive");
        if (m_disableTimerActive)
        {
            m3d::SafeFloatAttrib(m_disableTimer, xmlNode, "DisableTimer");
        }

        m3d::SafeBoolAttrib(m_asRope, xmlNode, "AsRope");

        int size = 0;
        if (m3d::SafeIntAttrib(size, xmlNode, "Size") && size < 0)
            size = 0;

        int esize = 0;
        if (m3d::SafeIntAttrib(esize, xmlNode, "ESize") && esize < 0)
            esize = 0;

        m3d::SafeBoolAttrib(m_enabled, xmlNode, "Enabled");
        m3d::SafeStrAttrib(m_modelName, xmlNode, "ModelName");
        m3d::SafeFloatAttrib(m_mass, xmlNode, "Mass");

        m_dataForLoad.resize(size);
        for (int i = 0; i < size; ++i)
        {
            auto& data = m_dataForLoad[i];
            CStr const prefix = CStr("Member_") + CStr(i);
            m3d::SafeVectorAttrib(data.pos, xmlNode, (prefix + CStr("_Pos")).c_str());
            m3d::SafeQuaternionAttrib(data.rot, xmlNode, (prefix + CStr("_Rot")).c_str());
            m3d::SafeVectorAttrib(data.linVel, xmlNode, (prefix + CStr("_LinVel")).c_str());
            m3d::SafeVectorAttrib(data.angVel, xmlNode, (prefix + CStr("_Ang_vel")).c_str());
        }

        for (int n = 0; n < esize; ++n)
        {
            CStr const prefix = CStr("EMember_") + CStr(n);

            int boneIdx = 0;
            m3d::SafeIntAttrib(boneIdx, xmlNode, (prefix + CStr("_Idx")).c_str());

            auto& data = m_edataForLoad[boneIdx];
            m3d::SafeVectorAttrib(data.pos, xmlNode, (prefix + CStr("_Pos")).c_str());
            m3d::SafeQuaternionAttrib(data.rot, xmlNode, (prefix + CStr("_Rot")).c_str());
            m3d::SafeVectorAttrib(data.linVel, xmlNode, (prefix + CStr("_LinVel")).c_str());
            m3d::SafeVectorAttrib(data.angVel, xmlNode, (prefix + CStr("_Ang_vel")).c_str());
        }

        m3d::SafeFloatAttrib(m_strech, xmlNode, "Strech");

        int ejSize = 0;
        m3d::SafeIntAttrib(ejSize, xmlNode, "EJSize");
        m_externalJointsInfo.resize(ejSize);
        for (int i = 0; i < ejSize; ++i)
        {
            auto& info = m_externalJointsInfo[i];
            CStr const prefix = CStr("EJoint_") + CStr(i);
            m3d::SafeVectorAttrib(info.jointPos, xmlNode, (prefix + CStr("_Pos")).c_str());
            m3d::SafeStrAttrib(info.lpName, xmlNode, (prefix + CStr("_LpName")).c_str());
            m3d::SafeBoolAttrib(info.connectedToStatics, xmlNode, (prefix + CStr("_Static")).c_str());
        }
    }

    void JointedObj::InitImpulses(CVector causePos, float force)
    {
        // RVA 0x81B570 - blows every member away from the cause, along the line from the cause
        // to the object's centre.
        CVector const away = m_Position - causePos;
        float const invLen =
            1.0f / sqrt(away.z * away.z + away.y * away.y + away.x * away.x + 0.00000011920929f);

        float const dirX = invLen * away.x;
        float dirY = away.y * invLen;
        float const dirZ = away.z * invLen;

        // NOTE: the vertical component is forced positive, so a blast from above still throws
        // the body upwards rather than driving it into the ground.
        if (dirY < 0.0f)
            dirY = -dirY;

        CVector const impulse(dirX * force, dirY * force, dirZ * force);
        for (auto* member : m_Members)
        {
            member->AddImpulse(impulse);
        }
        for (auto const& extra : m_ExtraMembers)
        {
            extra.second->AddImpulse(impulse);
        }
    }

    void JointedObj::Disable(bool disable)
    {
        // RVA 0x81B6D0
        m_enabled = !disable;

        for (auto* member : m_Members)
        {
            if (disable)
                member->DisablePhysics();
            else
                member->EnablePhysics();
        }
        for (auto const& extra : m_ExtraMembers)
        {
            if (disable)
                extra.second->DisablePhysics();
            else
                extra.second->EnablePhysics();
        }
    }

    void JointedObj::PostCollide()
    {
        // RVA 0x81C350 - wakes the object up, resolves it against the world once, then gives
        // it a fresh private space.
        Disable(false);

        for (auto* member : m_Members)
        {
            dSpaceCollide2(reinterpret_cast<dxGeom*>(gGlobalSpace), member->GetGeom()->GetGeomId(), nullptr,
                ai::NearCallback);
        }

        PutToNewSpace(gGlobalSpace);
    }

    void JointedObj::Update(float elapsedTime, unsigned)
    {
        // RVA 0x81CA50 - drives the model from the physics: every bone that owns a body takes
        // its pose from that body, the rest follow along, and the mesh is then rebuilt.
        if (!IsAlive())
            return;

        if (!m_Anim || !m_Model || !m_node)
        {
            Remove();
            return;
        }

        if (m_enabled)
        {
            int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();

            RecalcBoundBox();
            RelinkSceneGraphNode();

            // The bone matrices are relative to the node, which RecalcBoundBox just parked at
            // the members' centre, so the world poses have to come back by that much.
            CMatrix invMatWorld;
            invMatWorld.identity();
            invMatWorld._41 = -m_Position.x;
            invMatWorld._42 = -m_Position.y;
            invMatWorld._43 = -m_Position.z;

            for (unsigned i = 0; i < m_Model->GetNumHierGeoms(); ++i)
            {
                auto* member = m_Members[i];

                CMatrix memberMat;
                memberMat.rotTranslate(member->GetRotation(), member->GetPosition());

                // m_JointToGeom undoes the geom's offset within its bone, so what comes out is
                // the bone's own frame rather than the box's.
                CMatrix const boneMat = m_JointToGeom[i] * memberMat * invMatWorld;

                auto const* hierGeom = m_Model->GetHierGeom(i);
                m_Anim->GetBoneAnim(hierGeom->parentBone).m_curMatrix = boneMat;
                m_Anim->GetBoneAnim(hierGeom->parentBone).m_lastUpdatedFrame = curFrame;
            }

            // Bones with no body of their own just hang off their parent in the rest pose.
            for (unsigned i = 0; i < m_Model->GetNumBones(); ++i)
            {
                auto& boneAnim = m_Anim->GetBoneAnim(i);
                if (boneAnim.m_lastUpdatedFrame == curFrame || boneAnim.m_parentIdx < 0)
                    continue;

                CMatrix localMat;
                localMat.rotTranslate(boneAnim.m_rotation, boneAnim.m_translation);
                boneAnim.m_curMatrix = localMat * m_Anim->GetBoneAnim(boneAnim.m_parentIdx).m_curMatrix;
            }

            if (m_asRope)
            {
                // Only the jointed links have bodies; the bones between them ride a spline
                // through the four neighbours CalcSplineNeighbours picked out.
                for (auto const& entry : m_splineNeighbours)
                {
                    int const bone = entry.first;
                    auto const& splineBones = entry.second;

                    auto const& m1 = m_Anim->GetBoneAnim(splineBones.bones[0]).m_curMatrix;
                    auto const& m2 = m_Anim->GetBoneAnim(splineBones.bones[1]).m_curMatrix;
                    auto const& m3 = m_Anim->GetBoneAnim(splineBones.bones[2]).m_curMatrix;
                    auto const& m4 = m_Anim->GetBoneAnim(splineBones.bones[3]).m_curMatrix;

                    CVector const p1(m1._41, m1._42, m1._43);
                    CVector const p2(m2._41, m2._42, m2._43);
                    CVector const p3(m3._41, m3._42, m3._43);
                    CVector const p4(m4._41, m4._42, m4._43);
                    CVector const pos = CatmullRomSubdivide<CVector>(splineBones.t, p1, p2, p3, p4);

                    // The bone keeps its rest rotation relative to its parent.
                    Quaternion parentRot;
                    parentRot.FromMatrix(m_Anim->GetBoneAnim(m_Model->GetBone(bone).m_parentIdx).m_curMatrix);

                    auto& boneAnim = m_Anim->GetBoneAnim(bone);
                    Quaternion boneRot = boneAnim.m_rotation;
                    boneRot *= parentRot;

                    boneAnim.m_curMatrix.rotTranslate(boneRot, pos);
                }
            }

            m3d::Configuration* cfg = nullptr;
            m_node->GetProperty(m3d::PROP_DM_CFG, &cfg);
            m_Model->UpdateVertices(m_Anim, true, cfg);

            // A rope is held up by its joints, so it must never be allowed to fall asleep.
            if (!m_asRope)
            {
                CheckDisablePhysics();
            }

            if (m_disableTimerActive)
            {
                m_disableTimer -= elapsedTime;
                if (m_disableTimer <= 0.0f)
                {
                    Disable(true);
                }
            }
        }

        if (m_deadTimerActive)
        {
            m_deadTimer -= elapsedTime;
            if (m_deadTimer <= 0.0f)
            {
                if (!m_testVisibility)
                {
                    Remove();
                    return;
                }

                // Given the choice, a ragdoll only vanishes while nobody is looking at it.
                if (m_node->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame() - 1)
                {
                    Remove();
                }
            }
        }
    }

    m3d::Class* JointedObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    void JointedObj::ReattachExternalJoint(int IdAttachTo)
    {
        // RVA 0x81B430 - moves the joint that holds this object up onto another body, keeping
        // the anchor where it was.
        dxBody* attachToBody = nullptr;
        auto* attachTo = theObjects->GetEntityByObjId(IdAttachTo);
        if (IS_KIND_OF(attachTo, BreakableObject))
        {
            attachToBody = static_cast<BreakableObject*>(attachTo)->GetBody()->id();
        }

        // NOTE: joint 0 is the starting guess and its own flag is never tested, so a first
        // joint that is not connected to statics is still picked when no later one is.
        unsigned selected = 0;
        auto* joint = m_externalJoints[0];
        for (unsigned i = 1; i < m_externalJoints.size(); ++i)
        {
            if (m_externalJointsInfo[i].connectedToStatics)
            {
                joint = m_externalJoints[i];
                selected = i;
            }
        }

        dxBody* const ownBody = dJointGetBody(joint, 1);
        float anchor[4] = {};
        dJointGetBallAnchor(joint, anchor);
        dJointDestroy(joint);

        auto* newJoint = dJointCreateBall(gGlobalWorld, nullptr);
        dJointAttach(newJoint, attachToBody, ownBody);
        dJointSetBallAnchor(newJoint, anchor[0], anchor[1], anchor[2]);
        m_externalJoints[selected] = newJoint;
    }

    void JointedObj::_InternalPostLoad()
    {
        // RVA 0x825120 - rebuilds the ragdoll from its model and replays the saved pose onto it.
        if (m_dataForLoad.empty())
            return;

        Init(m_modelName, ZeroVector, IdentityQuaternion, m_mass, nullptr, m_strech);
        PutToNewSpace(gGlobalSpace);

        M3D_ASSERT(m_dataForLoad.size() == m_Members.size());
        for (unsigned i = 0; i < m_dataForLoad.size(); ++i)
        {
            auto* member = m_Members[i];
            auto const& data = m_dataForLoad[i];
            member->SetPosition(data.pos);
            member->SetRotation(data.rot);
            member->SetLinearVelocity(data.linVel);
            member->SetAngularVelocity(data.angVel);
        }

        M3D_ASSERT(m_edataForLoad.size() == m_ExtraMembers.size());
        for (auto const& entry : m_edataForLoad)
        {
            auto* member = m_ExtraMembers[entry.first];
            member->SetPosition(entry.second.pos);
            member->SetRotation(entry.second.rot);
            member->SetLinearVelocity(entry.second.linVel);
            member->SetAngularVelocity(entry.second.angVel);
        }

        m_dataForLoad.clear();
        m_edataForLoad.clear();

        // The info was restored by LoadRuntimeValues, so the flag stops AddExternalJoint
        // recording each joint a second time as it recreates them.
        m_onLoad = true;
        for (unsigned i = 0; i < m_externalJointsInfo.size(); ++i)
        {
            AddExternalJoint(-1, m_externalJointsInfo[i].jointPos, m_externalJointsInfo[i].lpName);
        }
        m_onLoad = false;
    }

    JointedObj::~JointedObj()
    {
        // RVA 0x821700 - the members are ordinary objects owned by the container, so only the
        // joints, the private space and the scene graph node are torn down here.
        for (auto* joint : m_Joints)
        {
            dJointDestroy(joint);
        }
        for (auto* joint : m_externalJoints)
        {
            dJointDestroy(joint);
        }

        if (m_MembersSpace)
        {
            dSpaceDestroy(m_MembersSpace);
        }

        if (m_node)
        {
            m_node->GetGraph()->RemoveNode(m_node);
            m_node = nullptr;
        }
    }

    void JointedObj::CheckDisablePhysics()
    {
        // RVA 0x81C200 - physics is switched off only once every member has come to rest on
        // the ground; a single member still moving keeps the whole object awake.
        for (auto* member : m_Members)
        {
            CVector const vel = member->GetLinearVelocity();
            if (vel.z * vel.z + vel.y * vel.y + vel.x * vel.x > 0.0099999998f)
                return;

            CVector const rot = member->GetAngularVelocity();
            if (rot.z * rot.z + rot.y * rot.y + rot.x * rot.x > 0.2f)
                return;

            CVector const pos = member->GetPosition();
            float const lsHeight = m3d::pClient->GetWorld().GetLandscape().GetLsHeight(pos.x, pos.z);
            if (fabs(pos.y - lsHeight) > 0.2)
                return;
        }

        Disable(true);
        for (auto* member : m_Members)
        {
            member->DisableGeometry(true);
        }
    }

    bool JointedObj::IsParent(int i, int j)
    {
        // RVA 0x816930 - walks the bone hierarchy up from j looking for i.
        if (j < 0)
            return false;

        for (int parent = m_Model->GetBone(j).m_parentIdx; parent >= 0;
             parent = m_Model->GetBone(parent).m_parentIdx)
        {
            if (parent == i)
                return true;
        }
        return false;
    }

    void JointedObj::PutToNewSpace(dxSpace* Parent)
    {
        // RVA 0x81B810 - the members share a private space so that the parts of one ragdoll
        // never collide with each other.
        if (m_MembersSpace)
        {
            dSpaceDestroy(m_MembersSpace);
        }

        m_MembersSpace = dSimpleSpaceCreate(Parent);
        // The geoms outlive the space, so it must not free them.
        dSpaceSetCleanup(m_MembersSpace, 0);

        for (auto* member : m_Members)
        {
            member->TransferToSpace(m_MembersSpace);
        }
        for (auto const& extra : m_ExtraMembers)
        {
            extra.second->TransferToSpace(m_MembersSpace);
        }
    }

    void JointedObj::RecalcBoundBox()
    {
        // RVA 0x818180 - the scene graph node follows the members, so its box has to be
        // rebuilt from wherever the physics has taken them.
        Aabb worldBox;
        worldBox.m_box[0] = 10000.0f;
        worldBox.m_box[1] = 10000.0f;
        worldBox.m_box[2] = 10000.0f;
        worldBox.m_box[3] = -10000.0f;
        worldBox.m_box[4] = -10000.0f;
        worldBox.m_box[5] = -10000.0f;

        for (unsigned i = 0; i < m_Model->GetNumHierGeoms(); ++i)
        {
            auto* member = m_Members[i];
            dReal const* posArray = dGeomGetPosition(member->GetGeom()->GetGeomId());
            float const posX = posArray[0];
            float const posY = posArray[1];
            float const posZ = posArray[2];

            // A member that has been flung off the map is switched off rather than dragging
            // the bounding box out with it.
            if (posX < 0.0f || posZ < 0.0f || posX >= ai::pServer->GetLevelSize() ||
                posZ >= ai::pServer->GetLevelSize() || posY <= -1000.0f || posY >= 10000.0f)
            {
                member->DisablePhysics();
                member->DisableGeometry(true);
            }
            else
            {
                worldBox.EmbraceBox(member->GetGeom()->GetAabb());
            }
        }

        m_Position.x = (worldBox.m_box[3] + worldBox.m_box[0]) * 0.5f;
        m_Position.y = (worldBox.m_box[1] + worldBox.m_box[4]) * 0.5f;
        m_Position.z = (worldBox.m_box[2] + worldBox.m_box[5]) * 0.5f;

        // The animation's box is relative to the node, the gathered one is in world space.
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

    m3d::Object* JointedObj::Clone()
    {
        // RVA 0x8174A0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* JointedObj::CreateObject()
    {
        // RVA 0x817660
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }
}  // namespace ai

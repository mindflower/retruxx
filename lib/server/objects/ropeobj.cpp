#include "ropeobj.h"
#include "base/jointedobj.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "breakableobject.h"
#include "physicbodies/physicbody.h"

#include <cmath>
#include <core/ini.h>
#include <core/log.h>
#include <m3dapp.h>
#include <scene/nodes/sgnode.h>
#include <scene/servers/dataserver.h>
#include <scene/scenegraph.h>
#include <server/server.h>
#include <server/utils.h>
#include <skelmodel.h>
#include <world.h>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RopeObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RopeObj);

    namespace
    {
        float const VISCELL_EDGE_LENGTH_INV_2 = 0.0078125f;

        m3d::AnimatedModel* getNodeModel(m3d::SgNode const* node)
        {
            m3d::AnimatedModel* mdl = nullptr;
            M3D_APP->GetAnimatedModelsServer().GetItemProperty(node->GetServerHandle(), 16394, &mdl);
            return mdl;
        }

        CStr loadPointName(int lpNum)
        {
            return CStr("LP_PART0") + CStr(lpNum);
        }

        // Stretches the rope's model along its local Z from `from` to `to`; inlined in SetPosFromTies and HangToPosts.
        void stretchRopeNode(m3d::SgNode* node, m3d::AnimatedModel const* mdl, CVector const& from, CVector const& to, float& strech)
        {
            node->SetOriginAbs(from);
            CMatrix rot;
            rot.lookAtLH(to, from, CVector(0.0f, 1.0f, 0.0f));
            Quaternion qrot;
            qrot.FromMatrix(rot.getInverseRot());
            node->SetRotation(qrot);
            double const dx = to.x - from.x;
            double const dy = to.y - from.y;
            double const dz = to.z - from.z;
            strech = static_cast<float>(std::sqrt(dz * dz + dy * dy + dx * dx) / (mdl->m_box.m_box[5] - mdl->m_box.m_box[2]));
            node->SetScale(CVector(1.0f, 1.0f, strech));
            node->UpdateXForm(true, false);
            node->GetGraph()->RelinkNode(node, true);
        }
    }  // namespace

    RopeObjPrototypeInfo::RopeObjPrototypeInfo()
    {
        m_bIsUpdating = false;
    }

    bool RopeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeStrAttrib(m_brokenModel, xmlNode, "BrokenModel");
        }
        return result;
    }

    ai::Obj* RopeObjPrototypeInfo::CreateTargetObject() const
    {
        return new RopeObj(*this);
    }

    RopeObj::~RopeObj() = default;

    RopeObj::RopeObj(ai::RopeObjPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        m_strech = 1.0f;
        DisablePhysics();
        DisableGeometry(true);
    }

    m3d::Object* RopeObj::Clone()
    {
        // RVA 0x875F20
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* RopeObj::CreateObject()
    {
        // RVA 0x8760E0
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* RopeObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    m3d::Class* RopeObj::GetClass() const
    {
        return RT_CLASS_LOCAL(RopeObj);
    }

    ai::RopeObjPrototypeInfo const* RopeObj::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), RopeObjPrototypeInfo const);
    }

    void RopeObj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x878150 - NOTE: posts are appended to m_posts without clearing it first.
        Obj::LoadFromXML(xmlFile, xmlNode);
        if (xmlNode->GetAttribute("Tie1Pos") && xmlNode->GetAttribute("Tie2Pos"))
        {
            m_tiePoses.resize(2);
            m3d::SafeVectorAttrib(m_tiePoses[0], xmlNode, "Tie1Pos");
            m3d::SafeVectorAttrib(m_tiePoses[1], xmlNode, "Tie2Pos");
        }
        if (xmlNode->GetAttribute("TiedObj1Pos") && xmlNode->GetAttribute("TiedObj2Pos"))
        {
            m_tiedObjPoses.resize(2);
            m3d::SafeVectorAttrib(m_tiedObjPoses[0], xmlNode, "TiedObj1Pos");
            m3d::SafeVectorAttrib(m_tiedObjPoses[1], xmlNode, "TiedObj2Pos");
        }
        else
        {
            m_tiedObjPoses.resize(2);
            m_tiedObjPoses[0] = CVector(0.0f, 0.0f, 0.0f);
            m_tiedObjPoses[1] = CVector(0.0f, 0.0f, 0.0f);
        }

        ref_ptr postNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(postNode, "Post"); !postNode->IsEmpty(); postNode->GetNextSibling(postNode, "Post"))
        {
            Post post;
            m3d::SafeStrAttrib(post.m_serverObjName, postNode, "ServerObjName");
            CStr hierarchy;
            m3d::SafeStrAttrib(hierarchy, postNode, "NodesNameHierarchy");
            m3d::SafeStrAttrib(post.m_lpName, postNode, "LpName");
            m3d::Tokenize(hierarchy, post.m_nodesNamesHierarchy, "(), ;\t");
            m3d::SafeVectorAttrib(post.m_postTiePos, postNode, "PostTiePos");
            m_posts.push_back(post);
        }
    }

    void RopeObj::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x876790 - only the posts are written; the tie positions are not.
        Obj::SaveToXML(xmlFile, xmlNode);
        if (m_posts.size() != 2)
        {
            return;
        }
        for (Post const& post : m_posts)
        {
            ref_ptr postNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Post");
            postNode->SetAttribute("ServerObjName", post.m_serverObjName.c_str());
            postNode->SetAttribute("NodesNameHierarchy", StringVectorToStr(post.m_nodesNamesHierarchy).c_str());
            postNode->SetAttribute("PostTiePos", CStr(post.m_postTiePos).c_str());
            postNode->SetAttribute("LpName", post.m_lpName.c_str());
            xmlNode->AddChild(postNode);
        }
    }

    CVector RopeObj::GetGeometricCenter() const
    {
        // RVA 0x875A90
        return GetPosition();
    }

    void RopeObj::SetPosFromTies()
    {
        // RVA 0x878580 - finds the two nodes the rope is tied to, records them as its posts and stretches the rope's
        // model between the tie positions. The saved tied-object positions give a quick search by node origin; when
        // either is unset, the tie positions are matched against LP_PART0<n> load points instead.
        m3d::SgNode* const node = m_physicBody->m_Node;
        if (!node || m_tiePoses.size() != 2)
        {
            return;
        }

        CVector const& tiedObj1Pos = m_tiedObjPoses[0];
        CVector const& tiedObj2Pos = m_tiedObjPoses[1];
        m3d::SgNode* node1;
        m3d::SgNode* node2;
        if (tiedObj1Pos.x * tiedObj1Pos.x + tiedObj1Pos.y * tiedObj1Pos.y + tiedObj1Pos.z * tiedObj1Pos.z < 0.001f ||
            tiedObj2Pos.x * tiedObj2Pos.x + tiedObj2Pos.y * tiedObj2Pos.y + tiedObj2Pos.z * tiedObj2Pos.z < 0.001f)
        {
            node1 = GetTiedSgNodeNearPos(m_tiePoses[0], false);
            node2 = GetTiedSgNodeNearPos(m_tiePoses[1], false);
        }
        else
        {
            node1 = GetTiedSgNodeNearPos(m_tiedObjPoses[0], true);
            node2 = GetTiedSgNodeNearPos(m_tiedObjPoses[1], true);
        }

        Post post1;
        Post post2;
        m_tieObjects.resize(2, -1);
        if (!node1)
        {
            // NOTE: the tied-object position is reported even when the tie position was searched.
            M3D_LOG_ERR(
                CStr("Error! Couldn't find tied object for RopeObj with name ") + CStr(GetName()) + CStr(" at pos ") +
                CStr::format_("%.3f %.3f %.3f", m_tiedObjPoses[0].x, m_tiedObjPoses[0].y, m_tiedObjPoses[0].z));
        }
        else if (!node2)
        {
            M3D_LOG_ERR(
                CStr("Error! Couldn't find tied object for RopeObj with name ") + CStr(GetName()) + CStr(" at pos ") +
                vecToStr(m_tiedObjPoses[1]));
        }
        else
        {
            m_tiedObjPoses[0] = node1->m_currentWorldOrigin;
            m_tiedObjPoses[1] = node2->m_currentWorldOrigin;

            m3d::SgNode* const tiedNodes[2] = {node1, node2};
            Post* const posts[2] = {&post1, &post2};
            for (int i = 0; i < 2; ++i)
            {
                PhysicBody* body = nullptr;
                tiedNodes[i]->GetProperty(4356, &body);
                if (!body)
                {
                    continue;
                }
                PhysicObj* const owner = body->GetOwner();
                if (owner && IS_KIND_OF(owner, BreakableObject))
                {
                    static_cast<BreakableObject*>(owner)->ConnectWithRope(GetId());
                    m_tieObjects[i] = owner->GetId();
                    posts[i]->m_postObj = theObjects->GetEntityByObjId(m_tieObjects[i]);
                }
            }

            for (int i = 0; i < 2; ++i)
            {
                Post& post = *posts[i];
                m3d::SgNode* const tiedNode = tiedNodes[i];
                if (post.m_postObj)
                {
                    post.m_serverObjName = CStr(post.m_postObj->GetName());
                }
                post.m_postNode = tiedNode;
                if (!post.m_postObj)
                {
                    pServer->GetWorld()->GetGraph().GetNodeNamesHierarchy(tiedNode, post.m_nodesNamesHierarchy);
                }

                m3d::AnimatedModel* const mdl = getNodeModel(tiedNode);
                if (!mdl)
                {
                    // NOTE: returns without storing either post or placing the rope.
                    return;
                }
                // Every load point is checked; a later match overwrites an earlier one.
                for (int lpNum = 1;; ++lpNum)
                {
                    CStr const lpName = loadPointName(lpNum);
                    if (mdl->GetLoadPointIdByName(lpName.c_str()) == -1)
                    {
                        break;
                    }
                    CMatrix tieMatr;
                    mdl->GetBoneMatrixByName(lpName, tieMatr, false);
                    CMatrix const rot(tieMatr * tiedNode->m_currentXForm);
                    CVector const& tiePos = m_tiePoses[i];
                    double const dz = rot._43 - tiePos.z;
                    double const dy = rot._42 - tiePos.y;
                    double const dx = rot._41 - tiePos.x;
                    if (std::sqrt(dz * dz + dy * dy + dx * dx) < 0.01)
                    {
                        post.m_postTiePos = tiePos;
                        post.m_lpName = lpName;
                    }
                }
            }
        }

        SetPost(HP_BEGIN, post1);
        SetPost(HP_END, post2);
        if (m3d::AnimatedModel const* const mdl = getNodeModel(node))
        {
            stretchRopeNode(node, mdl, m_tiePoses[0], m_tiePoses[1], m_strech);
        }
    }

    m3d::SgNode* RopeObj::GetTiedSgNodeNearPos(CVector const& tiePos, bool simpleSearch) const
    {
        // RVA 0x8762E0 - searches the nodes linked directly into the scene graph cell under tiePos. A simple search
        // takes the first node, other than the rope's own, whose origin is within sqrt(0.1) of tiePos in the XZ plane;
        // otherwise the node's animated model must have an LP_PART0<n> load point that close.
        // NOTE: the cell index is not clamped, so a position off the map reads outside the cell array.
        int const cellZ = static_cast<int>(VISCELL_EDGE_LENGTH_INV_2 * tiePos.z);
        int const cellX = static_cast<int>(tiePos.x * VISCELL_EDGE_LENGTH_INV_2);
        auto const* const lists = pServer->GetWorld()->GetGraph().GetCellObjs(cellX, cellZ).GetObjects();
        for (int k = 0; k < 64; ++k)
        {
            for (m3d::Object* const obj : lists[k])
            {
                auto* const node = static_cast<m3d::SgNode*>(obj);
                if (simpleSearch)
                {
                    float const dx = node->m_currentWorldOrigin.x - tiePos.x;
                    float const dz = node->m_currentWorldOrigin.z - tiePos.z;
                    if (dz * dz + dx * dx < 0.1f && m_physicBody->m_Node != node)
                    {
                        return node;
                    }
                    continue;
                }
                if (node->m_srvId == -1)
                {
                    continue;
                }
                m3d::AnimatedModel* const mdl = getNodeModel(node);
                if (!mdl)
                {
                    continue;
                }
                for (int lpNum = 1;; ++lpNum)
                {
                    CStr const lpName = loadPointName(lpNum);
                    if (mdl->GetLoadPointIdByName(lpName.c_str()) == -1)
                    {
                        break;
                    }
                    CMatrix tieMatr;
                    mdl->GetBoneMatrixByName(lpName, tieMatr, false);
                    CMatrix const& xf = node->m_currentXForm;
                    float const dx = xf._21 * tieMatr._42 + xf._31 * tieMatr._43 + xf._41 * tieMatr._44 + xf._11 * tieMatr._41 - tiePos.x;
                    float const dz = xf._13 * tieMatr._41 + xf._23 * tieMatr._42 + xf._33 * tieMatr._43 + xf._43 * tieMatr._44 - tiePos.z;
                    if (dz * dz + dx * dx < 0.1f)
                    {
                        return node;
                    }
                }
            }
        }
        return nullptr;
    }

    void RopeObj::HangToPosts()
    {
        // RVA 0x876980 - resolves the posts loaded from XML and stretches the rope's model between them. A rope whose
        // posts cannot be resolved is removed.
        m3d::SgNode* const node = m_physicBody->m_Node;
        if (!node || m_posts.size() != 2)
        {
            return;
        }

        CVector const& tie1 = m_posts[0].m_postTiePos;
        CVector const& tie2 = m_posts[1].m_postTiePos;
        if (tie1.x * tie1.x + tie1.y * tie1.y + tie1.z * tie1.z < 0.01 || tie2.x * tie2.x + tie2.y * tie2.y + tie2.z * tie2.z < 0.01)
        {
            M3D_LOG_ERR(CStr("Rope ") + CStr(GetName()) + CStr(" has wrong post tie pose(s)"));
            Remove();
            return;
        }

        for (unsigned i = 0; i < m_posts.size(); ++i)
        {
            Post& post = m_posts[i];
            if (!post.m_serverObjName.empty())
            {
                post.m_postObj = theObjects->GetEntityByObjName(post.m_serverObjName);
                if (!post.m_postObj)
                {
                    M3D_LOG_ERR(CStr("Rope ") + CStr(GetName()) + CStr(" has wrong post names"));
                    Remove();
                    return;
                }
                if (IS_KIND_OF(post.m_postObj, BreakableObject))
                {
                    static_cast<BreakableObject*>(post.m_postObj)->ConnectWithRope(GetId());
                }
                if (IS_KIND_OF(post.m_postObj, SimplePhysicObj))
                {
                    // NOTE: the post object's physic body is used without a null check.
                    post.m_postNode = static_cast<SimplePhysicObj*>(post.m_postObj)->GetPhysicBody()->m_Node;
                }
            }
            else
            {
                if (post.m_nodesNamesHierarchy.empty())
                {
                    M3D_LOG_ERR(CStr("Rope ") + CStr(GetName()) + CStr(" has wrong post names"));
                    Remove();
                    return;
                }
                post.m_postNode = pServer->GetWorld()->GetGraph().GetNodeByNamesHierarchy(post.m_nodesNamesHierarchy);
                if (!post.m_postNode)
                {
                    M3D_LOG_ERR(CStr("Rope ") + CStr(GetName()) + CStr(" has wrong post names"));
                    Remove();
                    return;
                }
            }
        }

        m3d::AnimatedModel const* const mdl = getNodeModel(node);
        if (!mdl)
        {
            M3D_LOG_ERR(CStr("Rope ") + CStr(GetName()) + CStr(" couldn't get the model"));
            Remove();
            return;
        }
        stretchRopeNode(node, mdl, m_posts[0].m_postTiePos, m_posts[1].m_postTiePos, m_strech);
    }

    void RopeObj::BreakOff(int breakObj, bool createJointForBreakObj)
    {
        // RVA 0x8770E0 - replaces the rope with a jointed "brokenRope" object hanging from the posts, except that a post
        // which is the broken object gets a free joint end (or none unless createJointForBreakObj).
        m3d::SgNode* const node = m_physicBody->m_Node;
        CStr const& brokenModel = GetPrototypeInfo()->m_brokenModel;
        if (brokenModel.empty() || !node || m_posts.size() != 2)
        {
            return;
        }

        SetPostDisablePhysics();
        int const brokenRopeId = theObjects->CreateNewObject(theObjects->GetPrototypeId("brokenRope"), "", -1, -1);
        // NOTE: the new object is used without checking that it exists or is a JointedObj.
        auto* const brokenRope = static_cast<JointedObj*>(theObjects->GetEntityByObjId(brokenRopeId));
        CVector const pos = node->m_currentWorldOrigin;
        Quaternion const rot = node->m_rotation;
        brokenRope->m_asRope = true;
        brokenRope->Init(GetPrototypeInfo()->m_brokenModel, pos, rot, 1.0f, nullptr, m_strech);
        theObjects->AddObjToPostCollideList(brokenRope);

        for (unsigned j = 0; j < 2; ++j)
        {
            Post const& post = m_posts[j];
            // NOTE: m_postObj's id is read before m_postObj is checked for null.
            if (post.m_postObj->GetId() != breakObj && post.m_postObj && IS_KIND_OF(post.m_postObj, BreakableObject))
            {
                static_cast<BreakableObject*>(post.m_postObj)->ConnectWithRope(brokenRopeId);
            }
            int attachTo = -1;
            if (post.m_postObj && IS_KIND_OF(post.m_postObj, BreakableObject) && post.m_postObj->GetId() == breakObj)
            {
                if (!createJointForBreakObj)
                {
                    continue;
                }
                attachTo = post.m_postObj->GetId();
            }
            brokenRope->AddExternalJoint(attachTo, post.m_postTiePos, CStr("LP_PART0") + CStr(j + 1));
        }
        brokenRope->SetDeadTimer(60000, true);
        Remove();
    }

    RopeObj::Post::Post(ai::RopeObj::Post const& __that) :
        m_serverObjName(__that.m_serverObjName),
        m_nodesNamesHierarchy(__that.m_nodesNamesHierarchy),
        m_postNode(__that.m_postNode),
        m_postObj(__that.m_postObj),
        m_lpName(__that.m_lpName),
        m_postTiePos(__that.m_postTiePos)
    {
        // RVA 0x877430
    }

    RopeObj::Post::Post()
    {
        m_postNode = nullptr;
        m_postObj = nullptr;
        m_postTiePos = ZeroVector;
    }

    ai::RopeObj::Post const& RopeObj::GetPost(ai::RopeObj::HangPoints hp)
    {
        // Not emitted in the shipped binary (never called): the post, or the empty m_dummyPost when there is none.
        if (m_posts.size() == 2 && hp < HP_NUM_POINTS)
        {
            return m_posts[hp];
        }
        return m_dummyPost;
    }

    void RopeObj::SetPost(ai::RopeObj::HangPoints hp, ai::RopeObj::Post const& post)
    {
        // RVA 0x8784B0 - NOTE: HP_NUM_POINTS passes the range check and writes one past the two posts.
        if (hp <= HP_NUM_POINTS)
        {
            if (m_posts.size() != 2)
            {
                m_posts.resize(2, Post());
            }
            m_posts.data()[hp] = post;
        }
    }

    void RopeObj::_InternalCreateVisualPart()
    {
        SimplePhysicObj::_InternalCreateVisualPart();
        SetPosFromTies();
        HangToPosts();
    }
}  // namespace ai

#include <stdexcept>
#include <scene/nodes/sgnode.h>
#include <math/obb.h>

#include "config.h"
#include "core/kernel.h"
#include "core/timer.h"
#include "core/console/cvar.h"
#include "client.h"
#include "world.h"
#include "core/ini.h"
#include "scene/servers/dataserver.h"
#include <core/log.h>
#include "scene/nodes/sgnodesound.h"
#include "world.h"
#include "level.h"
#include "m3dapp.h"
#include "scene/nodes/sgnodesprite.h"
#include "ui/ui.h"

RT_CLASS_EXPORT_METHOD_DEFINE(SgNode, GetOrigin)
{
    // RVA 0x63F430
    auto* const node = static_cast<m3d::SgNode*>(context->asObject(0, "SgNode"));
    context->pushVector(node->GetOrigin());
    return 1;
}

int cntUpdateNeededChecks = 0;
int cntChildUpdates = 0;

namespace m3d
{
    extern CClient* pClient;

    RT_CLASS_EXPORTS_BEGIN(SgNode)
    RT_CLASS_EXPORT(SgNode, m3d::METHOD, GetOrigin, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgNode);

    Object* SgNode::CreateObject()
    {
        return new SgNode;
    }

    Class* SgNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    Obb SgNode::GetObb() const
    {
        CVector max;
        CVector min;

        min.x = this->m_boundingBox.m_box[0];
        min.y = this->m_boundingBox.m_box[1];
        min.z = this->m_boundingBox.m_box[2];
        max.x = this->m_boundingBox.m_box[3];
        max.y = this->m_boundingBox.m_box[4];
        max.z = this->m_boundingBox.m_box[5];

        Obb obb;
        obb.Create(min, max, m_currentXForm, true);
        return obb;
    }

    CVector const& SgNode::GetOrigin() const
    {
        // RVA 0x616D90
        return m_origin;
    }

    CMatrix const& SgNode::GetCurrentMatrix() const
    {
        return this->m_currentXForm;
    }

    int SgNode::GetPrevThinkTime() const
    {
        // RVA 0x634460
        return m_prevThinkTime;
    }

    int SgNode::SetScale(CVector const& scale)
    {
        m_scaling = scale;
        m_isXFormDirty |= 1u;
        for (auto parent = GetParent(); parent; parent = parent->GetParent())
        {
            if (parent->GetChildDirty())
            {
                break;
            }
            parent->SetChildDirty(true);
        }
        GetGraph()->InsertInUpdateXFormList(this);
        return 1;
    }

    void SgNode::SetTransparencyType(TransparencyType tt)
    {
        this->m_transparencyType = tt;
    }

    int SgNode::GetServerItemProperty(unsigned propId, void* property) const
    {
        if (!GetServer() || this->m_srvId == -1)
            return 0;
        return GetServer()->GetItemProperty(this->m_srvId, propId, property);
    }

    bool SgNode::IsXFormUpdateNeeded() const
    {
        return this->m_isXFormDirty || this->m_isOwnBoundingBoxDirty || this->m_isChildDirty;
    }

    int SgNode::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
    {
        // RVA 0x666B70 - only values that differ from the defaults are written.
        if (!Object::WriteToXmlNode(file, writeTo))
        {
            return 1;
        }
        if (m_origin.x != 0.0f || m_origin.y != 0.0f || m_origin.z != 0.0f)
        {
            writeTo->SetAttribute("org", CStr::format_("%.3f %.3f %.3f", m_origin.x, m_origin.y, m_origin.z).c_str());
            writeTo->SetAttribute("orgRel", CStr(static_cast<int>(m_isOriginRelative)).c_str());
        }
        if (m_rotation.x != 0.0f || m_rotation.y != 0.0f || m_rotation.z != 0.0f || 1.0f != m_rotation.w)
        {
            writeTo->SetAttribute(
                "rot",
                CStr::format_("%.4f %.4f %.4f %.4f", m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w).c_str());
        }
        if (1.0f != m_scaling.x || 1.0f != m_scaling.y || 1.0f != m_scaling.z)
        {
            writeTo->SetAttribute(
                "scale", CStr::format_("%.3f %.3f %.3f", m_scaling.x, m_scaling.y, m_scaling.z).c_str());
        }
        if (m_srvId != -1)
        {
            // The server item's name when there is a server, its raw handle otherwise.
            if (GetServer())
            {
                writeTo->SetAttribute("id", GetServer()->GetNameByItem(m_srvId).c_str());
                return 1;
            }
            writeTo->SetAttribute("id", CStr(m_srvId).c_str());
        }
        return 1;
    }

    CVector const& SgNode::GetOriginWorldAbs() const
    {
        return m_currentWorldOrigin;
    }

    void SgNode::SetPrevThinkTime(int t)
    {
        // RVA 0x634470
        m_prevThinkTime = t;
    }

    CVector const& SgNode::GetScale() const
    {
        return this->m_scaling;
    }

    Class* SgNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    int SgNode::SetServerItemProperty(unsigned propId, void* property) const
    {
        // RVA 0x63EBE0
        if (!GetServer() || m_srvId == -1)
        {
            return 0;
        }
        return GetServer()->SetItemProperty(m_srvId, propId, property);
    }

    int SgNode::Think(int, int)
    {
        m_nextThinkTime = 0;
        return 1;
    }

    int SgNode::GetTtl() const
    {
        // RVA 0x634440
        return m_ttl;
    }

    void SgNode::CanBeFree()
    {
    }

    CVector const& SgNode::GetOriginWorldAbsForSphere() const
    {
        // RVA 0x6343E0
        return m_originWorldAbsForSphere;
    }

    int SgNode::RemoveChild(Object* node)
    {
        assert(node);
        UnlinkChild(node);
        return 1;
    }

    int SgNode::GetNextThinkTime() const
    {
        // RVA 0x634450
        return m_nextThinkTime;
    }

    int SgNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        // RVA 0x643C50
        return 0;
    }

    Quaternion const& SgNode::GetRotation() const
    {
        return this->m_rotation;
    }

    int SgNode::GetProperty(unsigned propId, void* prop) const
    {
        if (propId >= 3)
        {
            if (propId == 4360)
            {
                return 0;
            }
            else
            {
                auto v4 = propId - 4352;
                if ((int)(propId - 4352) < 0 || v4 >= 10)
                {
                    if (propId == 4096)
                    {
                        if (m_debugMsg.empty())
                        {
                            *(char*)prop = 0;
                            return 1;
                        }
                        else
                        {
                            strcpy((char*)prop, m_debugMsg.c_str());
                            return 1;
                        }
                    }
                    else
                    {
                        return Object::GetProperty(propId, prop) != 0;
                    }
                }
                else
                {
                    *(int*)prop = this->m_props[v4];
                    return 1;
                }
            }
        }
        else
        {
            *(int*)prop = this->m_properties[propId];
            return 1;
        }
    }

    SceneGraph* SgNode::GetGraph()
    {
        if (m3d::pClient)
            return &m3d::pClient->GetWorld().GetGraph();
        return nullptr;
    }

    int SgNode::GetServerHandle() const
    {
        return m_srvId;
    }

    bool SgNode::IsFree() const
    {
        return true;
    }

    DataServer* SgNode::GetServer() const
    {
        return 0;
    }

    Aabb SgNode::GetAabb() const
    {
        // RVA 0x234400
        return m_boundingBox;
    }

    int SgNode::AddChild(Object* node)
    {
        return m3d::Object::AddChild(node) != 0;
    }

    Object* SgNode::Clone()
    {
        return new SgNode(*this);
    }

    int SgNode::ReadFromXmlNodeAfterAdd(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        auto idAttr = node->GetAttribute("id");
        if (!idAttr)
        {
            return 1;
        }

        auto* server = GetServer();
        if (!server)
        {
            m_srvId = atoi(idAttr);
            return 1;
        }

        auto item = server->GetItemByName(idAttr, true);
        if (item == -1)
        {
            item = 0;
            if (!IsKindOf(&m3d::SgSoundSourceNode::m_classSgSoundSourceNode))
            {
                M3D_LOG_INFO(
                    "ReadFromXmlNode: GetItemByName for name = " + CStr(idAttr) + " failed for node " +
                    CStr(GetName()) + ". Taking a 0 model.");
            }
        }
        this->SetProperty(4360u, &item);
        return 1;
    }

    TransparencyParams& SgNode::GetTransparencyParams()
    {
        // RVA 0x4F1390
        return m_transparencyParams;
    }

    void SgNode::SetBoundingBoxDirty()
    {
        // RVA 0x80EAD0
        m_isOwnBoundingBoxDirty = true;
    }

    void SgNode::RemoveImmediateAfterParent(bool YesOrNo)
    {
        m_removeImmediateAfterParent = YesOrNo;
    }

    unsigned SgNode::GetContourColor()
    {
        return m_contourColor;
    }

    Quaternion const& SgNode::GetRotationWorldAbs() const
    {
        return m_currentWorldRotation;
    }

    int SgNode::SetRotation(Quaternion const& rotation)
    {
        m_rotation = rotation;
        m_isXFormDirty |= 4u;
        for (auto* parent = GetParent(); parent; parent = parent->GetParent())
        {
            if (parent->GetChildDirty())
            {
                break;
            }
            parent->SetChildDirty(true);
        }
        GetGraph()->InsertInUpdateXFormList(this);
        return 1;
    }

    int SgNode::SetProperty(unsigned propId, void* prop)
    {
        if (!Object::SetProperty(propId, prop))
        {
            if (propId == 4360)
                return 0;
            if (propId == 4096)
            {
                this->m_debugMsg = (char*)prop;
                return 1;
            }
            auto v6 = propId - 4352;
            if ((int)(propId - 4352) >= 0 && v6 < 10)
            {
                this->m_props[v6] = *(int*)prop;
                return 1;
            }
            if (propId >= 3)
                return 0;
            this->m_properties[propId] = *(int*)prop;
        }
        return 1;
    }

    float SgNode::GetBoundingRadius() const
    {
        // RVA 0x6343F0
        return m_boundingRadius;
    }

    void SgNode::GetVisCellBounds(PointBase<int>& p0, PointBase<int>& p1) const
    {
        auto v3 = 1.0 / 128.0;
        p0.x = (int)(float)((float)(this->m_originWorldAbsForSphere.x - this->m_boundingRadius) * (float)(1.0 / 128.0));
        p0.y = (int)(float)((float)(this->m_originWorldAbsForSphere.z - this->m_boundingRadius) * v3);
        p1.x = (int)(float)((float)(this->m_boundingRadius + this->m_originWorldAbsForSphere.x) * v3);
        p1.y = (int)(float)((float)(this->m_boundingRadius + this->m_originWorldAbsForSphere.z) * v3);
        auto v4 = m3d::pClient->GetWorld().m_level->land_size - 1;
        if (p0.x < 0)
            p0.x = 0;
        if (p0.x > v4)
            p0.x = v4;
        if (p0.y < 0)
            p0.y = 0;
        if (p0.y > v4)
            p0.y = v4;
        if (p1.x < 0)
            p1.x = 0;
        if (p1.x > v4)
            p1.x = v4;
        if (p1.y < 0)
            p1.y = 0;
        if (p1.y > v4)
            p1.y = v4;
    }

    float SgNode::GetContourWidth()
    {
        return m_contourWidth;
    }

    int SgNode::SetOriginAbs(CVector const& origin)
    {
        m_origin = origin;
        m_isXFormDirty |= 2u;
        m_isOriginRelative = false;
        for (auto* parent = GetParent(); parent; parent = parent->GetParent())
        {
            if (parent->GetChildDirty())
            {
                break;
            }
            parent->SetChildDirty(true);
        }
        GetGraph()->InsertInUpdateXFormList(this);
        return 1;
    }

    int SgNode::UpdateXForm(bool onlyVis, bool parentDirty)
    {
        // RVA 0x642320 - brings the node's own and world transforms, bounding box and bounding sphere up to date,
        // recursing into the children (only the dirty branches when onlyVis).
        if (m_isRootNode)
        {
            cntUpdateNeededChecks = 0;
            cntChildUpdates = 0;
        }
        bool const childWasDirty = m_isChildDirty;
        ++cntChildUpdates;
        bool const boxWasDirty = m_isOwnBoundingBoxDirty || m_isXFormDirty;
        if (boxWasDirty)
        {
            UpdateOwnBoundingBox();
            m_ownBoundingBox.m_box[0] = m_scaling.x * m_ownBoundingBox.m_box[0];
            m_ownBoundingBox.m_box[3] = m_scaling.x * m_ownBoundingBox.m_box[3];
            m_ownBoundingBox.m_box[1] = m_ownBoundingBox.m_box[1] * m_scaling.y;
            m_ownBoundingBox.m_box[4] = m_ownBoundingBox.m_box[4] * m_scaling.y;
            m_ownBoundingBox.m_box[2] = m_ownBoundingBox.m_box[2] * m_scaling.z;
            m_ownBoundingBox.m_box[5] = m_scaling.z * m_ownBoundingBox.m_box[5];
            m_isOwnBoundingBoxDirty = false;
        }

        // Own transform: scale, rotate, then translate (the origin's height relative to the ground if so flagged).
        if (m_isXFormDirty)
        {
            m_isXFormDirty = false;
            CVector origin = m_origin;
            if (m_isOriginRelative)
            {
                origin.y = pClient->GetWorld().GetLandscape().GetHeight(origin.x, origin.z, -1, 1) + origin.y;
            }
            m_ownXForm.rotTranslate(m_rotation, origin);
            float const scale[3] = {m_scaling.x, m_scaling.y, m_scaling.z};
            for (int row = 0; row < 3; ++row)
            {
                for (int col = 0; col < 3; ++col)
                {
                    m_ownXForm.m[row][col] *= scale[row];
                }
            }
            parentDirty = true;
        }

        // World transform, origin and rotation (the rotation from the unscaled world matrix, unless the parent is
        // the root, when it is the own rotation).
        if (parentDirty)
        {
            SgNode* const parent = static_cast<SgNode*>(GetParent());
            m_currentXForm = m_ownXForm;
            if (parent)
            {
                m_currentXForm = m_ownXForm * parent->m_currentXForm;
            }
            m_currentWorldOrigin = CVector(m_currentXForm._41, m_currentXForm._42, m_currentXForm._43);

            if (parent && parent->m_isRootNode)
            {
                m_currentWorldRotation = m_rotation;
            }
            else
            {
                CMatrix unScaled = m_currentXForm;
                float const invScale[3] = {1.0f / m_scaling.x, 1.0f / m_scaling.y, 1.0f / m_scaling.z};
                for (int row = 0; row < 4; ++row)
                {
                    for (int col = 0; col < 3; ++col)
                    {
                        unScaled.m[row][col] *= invScale[col];
                    }
                }
                Quaternion& q = m_currentWorldRotation;
                q.FromMatrix(unScaled);
                float const lengthSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
                if (lengthSq <= 0.0f)
                {
                    q = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
                }
                else
                {
                    float const invLength = static_cast<float>(1.0 / sqrt(lengthSq));
                    q = Quaternion(invLength * q.x, invLength * q.y, invLength * q.z, invLength * q.w);
                }
            }
            pClient->GetWorld().GetLandscape().UpdateNodeCollisionGeoms(this);
        }

        if (onlyVis)
        {
            if (parentDirty || m_isChildDirty)
            {
                m_isChildDirty = false;
                for (auto* child = static_cast<SgNode*>(GetFirstChild()); child;
                     child = static_cast<SgNode*>(child->GetNextSibling()))
                {
                    ++cntUpdateNeededChecks;
                    child->UpdateXForm(onlyVis, parentDirty);
                }
            }
        }
        else
        {
            for (auto* child = static_cast<SgNode*>(GetFirstChild()); child;
                 child = static_cast<SgNode*>(child->GetNextSibling()))
            {
                child->UpdateXForm(false, parentDirty);
            }
        }

        // The bounding box: the own box, embracing (except for the root) the children's boxes moved by their
        // origins, skipping sound sources and sprites.
        if (boxWasDirty || childWasDirty)
        {
            m_boundingBox = m_ownBoundingBox;
            if (!m_isRootNode)
            {
                for (auto* child = static_cast<SgNode*>(GetFirstChild()); child;
                     child = static_cast<SgNode*>(child->GetNextSibling()))
                {
                    if (child->GetClass() == &SgSoundSourceNode::m_classSgSoundSourceNode ||
                        child->GetClass() == &SgSpriteNode::m_classSgSpriteNode)
                    {
                        continue;
                    }
                    Aabb box = child->m_boundingBox;
                    box.m_box[0] += child->m_origin.x;
                    box.m_box[1] += child->m_origin.y;
                    box.m_box[2] += child->m_origin.z;
                    box.m_box[3] += child->m_origin.x;
                    box.m_box[4] += child->m_origin.y;
                    box.m_box[5] += child->m_origin.z;
                    m_boundingBox.EmbraceBox(box);
                }
            }
            float const dx = m_boundingBox.m_box[3] - m_boundingBox.m_box[0];
            float const dy = m_boundingBox.m_box[4] - m_boundingBox.m_box[1];
            float const dz = m_boundingBox.m_box[5] - m_boundingBox.m_box[2];
            m_boundingRadius = static_cast<float>(sqrt(dx * dx + dz * dz + dy * dy) * 0.5);
        }

        // The bounding sphere's centre: the box centre, rotated (not scaled) into the world.
        CVector const center(
            (m_boundingBox.m_box[0] + m_boundingBox.m_box[3]) * 0.5f,
            (m_boundingBox.m_box[1] + m_boundingBox.m_box[4]) * 0.5f,
            (m_boundingBox.m_box[2] + m_boundingBox.m_box[5]) * 0.5f);
        CMatrix worldRotation;
        worldRotation.rotTranslate(m_currentWorldRotation, m_currentWorldOrigin);
        m_originWorldAbsForSphere = worldRotation.vecMul(center);

        // A plain group node's own box is its whole box, taken back out of its world scale.
        if (GetClass() == &SgNode::m_classSgNode)
        {
            m_ownBoundingBox = m_boundingBox;
            float sx = 0.0f;
            float sy = 0.0f;
            float sz = 0.0f;
            m_currentXForm.DecomposeScale(sx, sy, sz);
            m_ownBoundingBox.m_box[0] *= 1.0f / sx;
            m_ownBoundingBox.m_box[3] *= 1.0f / sx;
            m_ownBoundingBox.m_box[1] *= 1.0f / sy;
            m_ownBoundingBox.m_box[4] *= 1.0f / sy;
            m_ownBoundingBox.m_box[2] *= 1.0f / sz;
            m_ownBoundingBox.m_box[5] *= 1.0f / sz;
        }

        if (m_isRootNode)
        {
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(
                ("UpdateXForm checks = " + CStr(cntUpdateNeededChecks)).c_str());
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(("UpdateXForms = " + CStr(cntChildUpdates)).c_str());
        }
        return 1;
    }

    bool SgNode::VisCellBoundsChanged() const
    {
        if (!m_forGraph)
            return true;

        PointBase<int> p0;
        PointBase<int> p1;
        m3d::SgNode::GetVisCellBounds(p0, p1);
        return p0.x != m_forGraph->m_cellsCoveredPoint0.x || p0.y != m_forGraph->m_cellsCoveredPoint0.y ||
            p1.x != m_forGraph->m_cellsCoveredPoint1.x || p1.y != m_forGraph->m_cellsCoveredPoint1.y;
    }

    int SgNode::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* node)
    {
        auto result = Object::ReadFromXmlNode(xmlFile, node);
        if (result)
        {
            m_origin = strToVec(node->GetAttribute("org"));
            m_isOriginRelative = strToBool(node->GetAttribute("orgRel"));
            m_rotation = strToQuat(node->GetAttribute("rot"));
            m_scaling = strToVec(node->GetAttribute("scale"));
            if (m_scaling.x == 0.0)
            {
                m_scaling.z = 1.0;
                m_scaling.y = 1.0;
                m_scaling.x = 1.0;
            }
            result = 1;
            this->m_isXFormDirty = 1;
            this->m_isOwnBoundingBoxDirty = true;
        }
        return result;
    }

    int SgNode::GetPropertiesList(
        retruxx::set<unsigned, retruxx::less<unsigned>, retruxx::allocator<unsigned>>& properties) const
    {
        // RVA 0x643870 - NOTE: of the ten 4352.. properties SetProperty accepts, only 4353 is listed.
        int const result = Object::GetPropertiesList(properties);
        if (!result)
        {
            return result;
        }
        for (unsigned i = 0; i < 3; ++i)
        {
            properties.insert(i);
        }
        properties.insert(4353);
        return 1;
    }

    Aabb SgNode::GetOwnAabb() const
    {
        // RVA 0x512EA0
        return m_ownBoundingBox;
    }

    float SgNode::IntersectRay(CVector const& v0, CVector const& dir, SgNode*& hitNode, Class*)
    {
        // RVA 0x642140 - against the node's own bounds; a ray starting inside them misses (-1).
        CVector const bbMin(m_ownBoundingBox.m_box[0], m_ownBoundingBox.m_box[1], m_ownBoundingBox.m_box[2]);
        CVector const bbMax(m_ownBoundingBox.m_box[3], m_ownBoundingBox.m_box[4], m_ownBoundingBox.m_box[5]);
        Obb obb;
        obb.Create(bbMin, bbMax, m_currentXForm, true);

        float const dy = v0.y - obb.m_origin.y;
        float const dz = v0.z - obb.m_origin.z;
        float const dx = v0.x - obb.m_origin.x;
        float const local[3] = {
            obb.m_basis[2].x * dz + obb.m_basis[1].x * dy + obb.m_basis[0].x * dx,
            obb.m_basis[2].y * dz + obb.m_basis[1].y * dy + obb.m_basis[0].y * dx,
            obb.m_basis[2].z * dz + obb.m_basis[1].z * dy + obb.m_basis[0].z * dx};
        float const obbMin[3] = {obb.m_min.x, obb.m_min.y, obb.m_min.z};
        float const obbMax[3] = {obb.m_max.x, obb.m_max.y, obb.m_max.z};
        int i = 0;
        for (; i < 3; ++i)
        {
            if (obbMin[i] > local[i] || local[i] > obbMax[i])
            {
                break;
            }
        }
        if (i == 3)
        {
            return -1.0f;
        }
        hitNode = this;
        return obb.IntersectRay(v0, dir);
    }

    void SgNode::Restart()
    {
        // RVA 0x5B8700
    }

    namespace
    {
        // The sway applied to "wavy" server items, rebuilt once per frame.
        int s_waveMatrixFrame = -1;
        CMatrix s_waveMatrix;
    }  // namespace

    CMatrix SgNode::MatrixFromFlags(SgNodeRenderFlags nrf, void* data) const
    {
        // RVA 0x63F4A0 - the node's world matrix combined with the matrix passed in (to its right or left), swaying
        // first if the server item is wavy.
        int wavy = 0;
        if (GetServer() && m_srvId != -1)
        {
            GetServer()->GetItemProperty(m_srvId, 1, &wavy);
        }
        int const curFrame = M3D_KERNEL->GetTimer().GetCurFrame();
        if (wavy && s_waveMatrixFrame != curFrame)
        {
            // A shear of x and y by z about the plane z = 25.
            s_waveMatrixFrame = curFrame;
            double const time = M3D_KERNEL->GetTimer().GetFrameStartTimeSec();
            CMatrix shear;
            shear.identity();
            shear._31 = static_cast<float>(sin(time) * 0.0099999998f);
            shear._32 = static_cast<float>(cos(time) * 0.0099999998f);
            CMatrix up;
            up.translation(0.0f, 0.0f, 25.0f);
            CMatrix down;
            down.translation(0.0f, 0.0f, -25.0f);
            s_waveMatrix = up * shear * down;
        }

        CMatrix const& other = *static_cast<CMatrix const*>(data);
        switch (nrf & (NRF_RMUL_BY_MAT | NRF_LMUL_BY_MAT))
        {
        case NRF_RMUL_BY_MAT:
            return wavy ? (s_waveMatrix * m_currentXForm) * other : m_currentXForm * other;
        case NRF_LMUL_BY_MAT:
            return wavy ? (other * s_waveMatrix) * m_currentXForm : other * m_currentXForm;
        default:
            return wavy ? s_waveMatrix * m_currentXForm : m_currentXForm;
        }
    }

    void SgNode::UpdateOwnBoundingBox()
    {
        this->m_ownBoundingBox.m_box[0] = 0.0;
        this->m_ownBoundingBox.m_box[1] = 0.0;
        this->m_ownBoundingBox.m_box[2] = 0.0;
        this->m_ownBoundingBox.m_box[3] = 0.0;
        this->m_ownBoundingBox.m_box[4] = 0.0;
        this->m_ownBoundingBox.m_box[5] = 0.0;
    }

    void SgNode::RitualInConstructor(Ritual rt)
    {
        m_initedWithRitual = rt;
        if ((rt & 2) != 0)
        {
            GetServer()->RegisterNode(this);
        }
        if ((rt & 1) != 0)
        {
            m3d::pClient->GetWorld().GetGraph().LinkThinkNode(this);
        }
    }

    SgNode::~SgNode()
    {
        // RVA 0x643E60 - each child is either deleted with the node (when it asks to be, or the whole graph is being
        // torn down) or handed to the scene root at its world placement, to be removed once everything below it
        // is free.
        SceneGraph* const sceneGraph = pClient ? &pClient->GetWorld().GetGraph() : nullptr;
        while (GetFirstChild())
        {
            auto* child = static_cast<SgNode*>(GetFirstChild());
            UnlinkChild(child);
            if (child->m_removeImmediateAfterParent || (sceneGraph && sceneGraph->IsInUnlinkAndDeleteAll()))
            {
                if (child->m_isInRemoveIfFree)
                {
                    M3D_LOG_WARN(
                        "Warning: deleting node which is in RemoveIfFree, name = '" + CStr(child->GetName()) +
                        "', parent name = '" + CStr(GetName()) + "'");
                }
                delete child;
                continue;
            }

            sceneGraph->LinkThinkNode(child);
            child->m_initedWithRitual = static_cast<Ritual>(child->m_initedWithRitual | 1u);
            sceneGraph->GetRootNode()->AddChild(child);
            sceneGraph->InsertInUpdateXFormList(child);
            child->SetOriginAbs(child->m_currentWorldOrigin);
            child->SetRotation(child->m_currentWorldRotation);
            sceneGraph->LinkNode(child);
            ForEachDescendant(child, [](SgNode* node) { node->CanBeFree(); });
            sceneGraph->InsertInRemoveIfFree(child);
            if (m_isInRemoveIfFree)
            {
                M3D_LOG_WARN(
                    "Adding child in RemoveIfFree in destructor, child name = '" + CStr(child->GetName()) +
                    "', parent name = '" + CStr(GetName()) + "'");
            }
        }

        if (m_isContoured)
        {
            sceneGraph->DeleteFromContourList(this);
        }
        RitualInDestructor();
    }

    SgNode::SgNode(SgNode const& node) : Object(node)
    {
        this->m_nextThinkTime = node.m_nextThinkTime;
        this->m_prevThinkTime = node.m_prevThinkTime;
        this->m_ttl = node.m_ttl;
        this->m_ownXForm = node.m_ownXForm;
        this->m_currentXForm = node.m_currentXForm;
        this->m_origin = node.m_origin;
        this->m_scaling = node.m_scaling;
        this->m_rotation = node.m_rotation;
        this->m_currentWorldOrigin = node.m_currentWorldOrigin;
        this->m_originWorldAbsForSphere = node.m_originWorldAbsForSphere;
        this->m_boundingRadius = node.m_boundingRadius;
        this->m_boundingBox = node.m_boundingBox;
        this->m_ownBoundingBox = node.m_ownBoundingBox;
        this->m_isOriginRelative = node.m_isOriginRelative;
        this->m_isXFormDirty = node.m_isXFormDirty;
        this->m_isOwnBoundingBoxDirty = node.m_isOwnBoundingBoxDirty;
        this->m_removeImmediateAfterParent = node.m_removeImmediateAfterParent;
        this->m_isRemoveIfFree = 0;
        this->m_isInRemoveIfFree = 0;
        this->m_isContoured = 0;
        this->m_contourColor = node.m_contourColor;
        this->m_contourWidth = node.m_contourWidth;
        this->m_transparencyType = TT_NONE;
        this->m_frameTransparent = -1;
        this->m_srvId = node.m_srvId;
        this->m_frameVisible = 0;
        this->m_frameVisible2 = 0;
        this->m_predictIdx = 0;
        this->m_isRootNode = 0;
        this->m_forGraph = 0;
        this->m_isWaitingForRender = 0;
        this->m_initedWithRitual = RITUAL_NONE;
        this->m_onScreenSize = 0.0;
        this->m_properties[0] = node.m_properties[0];
        this->m_properties[1] = node.m_properties[1];
        this->m_properties[2] = node.m_properties[2];
        memcpy(this->m_props, node.m_props, sizeof(this->m_props));
        this->m_properties[1] = 0;
        this->m_properties[2] = 0;
    }

    SgNode::SgNode()
    {
        memset(&this->m_ownXForm, 0, sizeof(this->m_ownXForm));
        this->m_ownXForm._44 = 1.0;
        this->m_ownXForm._33 = 1.0;
        this->m_ownXForm._22 = 1.0;
        this->m_ownXForm._11 = 1.0;
        memset(&this->m_currentXForm, 0, sizeof(this->m_currentXForm));
        this->m_currentXForm._44 = 1.0;
        this->m_currentXForm._33 = 1.0;
        this->m_currentXForm._22 = 1.0;
        this->m_currentXForm._11 = 1.0;
        *&this->m_origin.y = 0i64;
        this->m_origin.x = 0.0;
        this->m_scaling.z = 1.0;
        this->m_scaling.y = 1.0;
        this->m_scaling.x = 1.0;
        this->m_rotation.x = 0.0;
        this->m_rotation.y = 0.0;
        this->m_rotation.z = 0.0;
        this->m_rotation.w = 1.0;
        this->m_boundingRadius = 0.0;
        this->m_isXFormDirty = 0;
        this->m_isOwnBoundingBoxDirty = 1;
        this->m_isChildDirty = 0;
        this->m_ttl = 0;
        this->m_srvId = -1;
        this->m_removeImmediateAfterParent = 1;
        this->m_initedWithRitual = RITUAL_NONE;
        this->m_nextThinkTime = 1;
        this->m_prevThinkTime = 1;
        *&this->m_originWorldAbsForSphere.y = 0i64;
        this->m_originWorldAbsForSphere.x = 0.0;
        this->m_isContoured = 0;
        this->m_transparencyType = TT_NONE;
        this->m_frameTransparent = -1;
        this->m_isRemoveIfFree = 0;
        this->m_isInRemoveIfFree = 0;
        this->m_contourColor = m3d::g_Kernel->GetEngineCfg().m_g_contourColor.GetC();
        this->m_contourWidth = m3d::g_Kernel->GetEngineCfg().m_g_contourWidth.GetF();
        this->m_frameVisible = 0;
        this->m_frameVisible2 = 0;
        this->m_predictIdx = 0;
        this->m_isRootNode = 0;
        this->m_forGraph = 0;
        this->m_isWaitingForRender = 0;
        this->m_initedWithRitual = RITUAL_NONE;
        this->m_onScreenSize = 0.0;
        this->m_properties[0] = 0;
        this->m_properties[1] = 0;
        this->m_properties[2] = 0;
        this->m_props[8] = 1;
        this->m_props[2] = 0;
        this->m_props[4] = 0;
        this->m_props[6] = 0;
        this->m_props[5] = 0;
        this->m_props[7] = 0;
        this->m_props[9] = 0;
        this->m_props[1] = 1000;
    }

    void SgNode::RitualInDestructor()
    {
        if ((this->m_initedWithRitual & 2) != 0)
        {
            GetServer()->UnregisterNode(this);
        }
        if ((this->m_initedWithRitual & 1) != 0)
        {
            m3d::pClient->GetWorld().GetGraph().UnlinkThinkNode(this);
        }
        this->m_initedWithRitual = RITUAL_NONE;
    }

    void SgNode::InternalInit()
    {
        // RVA 0x643940
        m_frameVisible = 0;
        m_frameVisible2 = 0;
        m_onScreenSize = 0.0f;
        m_predictIdx = 0;
        m_isRootNode = false;
        m_forGraph = nullptr;
        m_isWaitingForRender = false;
        m_initedWithRitual = RITUAL_NONE;
    }
}  // namespace m3d

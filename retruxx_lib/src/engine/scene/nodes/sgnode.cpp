#include <stdexcept>
#include <scene/nodes/sgnode.h>
#include <math/obb.h>

#include "config.h"
#include "core/kernel.h"
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
    throw retruxx::logic_error("Not implemented");
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

        max = *(CVector*)&this->m_boundingBox.m_box[3];
        min.x = this->m_boundingBox.m_box[0];
        min.y = this->m_boundingBox.m_box[1];
        min.z = this->m_boundingBox.m_box[2];

        Obb obb;
        obb.Create(min, max, this->m_currentXForm, 1);
        return obb;
    }

    CVector const& SgNode::GetOrigin() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CMatrix const& SgNode::GetCurrentMatrix() const
    {
        return this->m_currentXForm;
    }

    int SgNode::GetPrevThinkTime() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector const& SgNode::GetOriginWorldAbs() const
    {
        return m_currentWorldOrigin;
    }

    void SgNode::SetPrevThinkTime(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector const& SgNode::GetScale() const
    {
        return this->m_scaling;
    }

    Class* SgNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    int SgNode::SetServerItemProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::Think(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::GetTtl() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::CanBeFree()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector const& SgNode::GetOriginWorldAbsForSphere() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::RemoveChild(Object*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::GetNextThinkTime() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Quaternion const& SgNode::GetRotation() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgNode::GetServer() const
    {
        return 0;
    }

    Aabb SgNode::GetAabb() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::AddChild(Object* node)
    {
        return m3d::Object::AddChild(node) != 0;
    }

    Object* SgNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
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

        // TODO: check this 0 models
        auto item = server->GetItemByName(idAttr, true);
        if (item == -1)
        {
            item = 0;
            if (!IsKindOf(&m3d::SgSoundSourceNode::m_classSgSoundSourceNode))
            {
                M3D_LOG_INFO("ReadFromXmlNode: GetItemByName for name = " + CStr(idAttr) + " failed for node " + CStr(GetName()) + ". Taking a 0 model.");
            }
        }
        this->SetProperty(4360u, &item);
        return 1;

    }

    TransparencyParams& SgNode::GetTransparencyParams()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::SetBoundingBoxDirty()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::RemoveImmediateAfterParent(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned SgNode::GetContourColor()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::GetVisCellBounds(PointBase<int>& p0, PointBase<int>& p1) const
    {
        auto v3 = 1.0 / 128.0;
        p0.x = (int)(float)((float)(this->m_originWorldAbsForSphere.x - this->m_boundingRadius)
            * (float)(1.0 / 128.0));
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
        throw retruxx::logic_error("Not implemented");
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
        // TODO: generated code
        if (this->m_isRootNode)
        {
            cntUpdateNeededChecks = 0;
            cntChildUpdates = 0;
        }

        auto v138 = m_isChildDirty;
        ++cntChildUpdates;
        const bool boxWasDirty = m_isOwnBoundingBoxDirty | (m_isXFormDirty & 1);
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

        float v18 = 0.0f;
        if (m_isXFormDirty)
        {
            m_isXFormDirty = false;
            CVector origin = m_origin;

            if (m_isOriginRelative)
            {
                origin.y = m3d::pClient->GetWorld().GetLandscape().GetHeight(origin.x, origin.z, -1, 1) + origin.y;
            }

            CMatrix unScale;
            CMatrix rt;
            auto v8 = this->m_rotation.z;
            auto v9 = this->m_rotation.y;
            auto v10 = v8 * this->m_rotation.w;
            auto v11 = this->m_rotation.x * v8;
            auto z = this->m_rotation.x * this->m_rotation.x;
            auto v12 = this->m_rotation.x * this->m_rotation.w;
            auto y = this->m_rotation.x * this->m_rotation.y;
            auto x = this->m_rotation.z * this->m_rotation.y;
            auto v140 = this->m_rotation.y * this->m_rotation.w;
            auto v13 = v8 * v8;
            auto v14 = v9 * v9;
            unScale._11 = 1.0 - (float)((float)(v13 + v14) * 2.0);
            unScale._21 = (float)(y - v10) * 2.0;
            unScale._31 = (float)(v140 + v11) * 2.0;
            unScale._12 = (float)(v10 + y) * 2.0;
            unScale._22 = 1.0 - (float)((float)(v13 + z) * 2.0);
            unScale._33 = 1.0 - (float)((float)(v14 + z) * 2.0);
            unScale._32 = (float)(x - v12) * 2.0;
            unScale._13 = ((float)(v11 - v140) * 2.0);
            unScale._14 = 0.0;
            unScale._23 = ((float)(v12 + x) * 2.0);
            unScale._24 = 0.0;
            memset(&unScale.m[2][3], 0, 16);
            unScale._44 = 1.0;

            rt = unScale;
            this->m_ownXForm._11 = rt._11 * this->m_scaling.x;
            auto _21 = rt._21;
            this->m_ownXForm._12 = rt._12 * this->m_scaling.x;
            this->m_ownXForm._13 = rt._13 * this->m_scaling.x;
            this->m_ownXForm._14 = 0.0;
            this->m_ownXForm._21 = _21 * this->m_scaling.y;
            this->m_ownXForm._22 = rt._22 * this->m_scaling.y;
            this->m_ownXForm._23 = rt._23 * this->m_scaling.y;
            this->m_ownXForm._24 = 0.0;
            this->m_ownXForm._31 = this->m_scaling.z * rt._31;
            this->m_ownXForm._32 = this->m_scaling.z * rt._32;
            auto v16 = this->m_scaling.z * rt._33;
            this->m_ownXForm._34 = 0.0;
            this->m_ownXForm._41 = origin.x;
            auto v17 = origin.y;
            this->m_ownXForm._33 = v16;
            v18 = 1.0;
            this->m_ownXForm._42 = v17;
            this->m_ownXForm._43 = origin.z;
            this->m_ownXForm._44 = 1.0;
            parentDirty = true;
        }
        else
        {
            if (!parentDirty)
            {
            }
            else
            {
                v18 = 1.0;
                m3d::SgNode* m_parent = (m3d::SgNode*)this->GetParent();
                memcpy(&m_currentXForm, &m_ownXForm, sizeof(this->m_currentXForm));

                if (m_parent)
                {
                    auto _13 = this->m_currentXForm._13;
                    auto _14 = this->m_currentXForm._14;
                    auto _11 = this->m_currentXForm._11;
                    auto _12 = this->m_currentXForm._12;
                    this->m_currentXForm._11 = (float)((float)((float)(_14 * m_parent->m_currentXForm._41)
                                                               + (float)(_13 * m_parent->m_currentXForm._31))
                                                       + (float)(m_parent->m_currentXForm._11 * _11))
                        + (float)(m_parent->m_currentXForm._21 * _12);
                    this->m_currentXForm._12 = (float)((float)((float)(_11 * m_parent->m_currentXForm._12)
                                                               + (float)(m_parent->m_currentXForm._42 * _14))
                                                       + (float)(m_parent->m_currentXForm._32 * _13))
                        + (float)(_12 * m_parent->m_currentXForm._22);
                    this->m_currentXForm._13 = (float)((float)((float)(m_parent->m_currentXForm._23 * _12)
                                                               + (float)(_11 * m_parent->m_currentXForm._13))
                                                       + (float)(m_parent->m_currentXForm._43 * _14))
                        + (float)(m_parent->m_currentXForm._33 * _13);
                    auto v24 = (float)(_14 * m_parent->m_currentXForm._44) + (float)(_13 * m_parent->m_currentXForm._34);
                    auto v25 = m_parent->m_currentXForm._14 * _11;
                    auto v26 = this->m_currentXForm._21;
                    auto v27 = v24 + v25;
                    auto v28 = m_parent->m_currentXForm._24 * _12;
                    auto _22 = this->m_currentXForm._22;
                    auto v30 = v27 + v28;
                    auto _23 = this->m_currentXForm._23;
                    this->m_currentXForm._14 = v30;
                    auto _24 = this->m_currentXForm._24;
                    this->m_currentXForm._21 = (float)((float)((float)(_24 * m_parent->m_currentXForm._41)
                                                               + (float)(_23 * m_parent->m_currentXForm._31))
                                                       + (float)(m_parent->m_currentXForm._11 * v26))
                        + (float)(m_parent->m_currentXForm._21 * _22);
                    this->m_currentXForm._22 = (float)((float)((float)(v26 * m_parent->m_currentXForm._12)
                                                               + (float)(m_parent->m_currentXForm._42 * _24))
                                                       + (float)(m_parent->m_currentXForm._32 * _23))
                        + (float)(_22 * m_parent->m_currentXForm._22);
                    this->m_currentXForm._23 = (float)((float)((float)(m_parent->m_currentXForm._23 * _22)
                                                               + (float)(v26 * m_parent->m_currentXForm._13))
                                                       + (float)(m_parent->m_currentXForm._43 * _24))
                        + (float)(m_parent->m_currentXForm._33 * _23);
                    auto v33 = (float)(_24 * m_parent->m_currentXForm._44) + (float)(_23 * m_parent->m_currentXForm._34);
                    auto v34 = m_parent->m_currentXForm._14 * v26;
                    auto _31 = this->m_currentXForm._31;
                    auto v36 = v33 + v34;
                    auto v37 = m_parent->m_currentXForm._24 * _22;
                    auto _32 = this->m_currentXForm._32;
                    auto v39 = v36 + v37;
                    auto _33 = this->m_currentXForm._33;
                    this->m_currentXForm._24 = v39;
                    auto _34 = this->m_currentXForm._34;
                    this->m_currentXForm._31 = (float)((float)((float)(_34 * m_parent->m_currentXForm._41)
                                                               + (float)(_33 * m_parent->m_currentXForm._31))
                                                       + (float)(m_parent->m_currentXForm._11 * _31))
                        + (float)(m_parent->m_currentXForm._21 * _32);
                    this->m_currentXForm._32 = (float)((float)((float)(_31 * m_parent->m_currentXForm._12)
                                                               + (float)(m_parent->m_currentXForm._42 * _34))
                                                       + (float)(m_parent->m_currentXForm._32 * _33))
                        + (float)(_32 * m_parent->m_currentXForm._22);
                    this->m_currentXForm._33 = (float)((float)((float)(m_parent->m_currentXForm._23 * _32)
                                                               + (float)(_31 * m_parent->m_currentXForm._13))
                                                       + (float)(m_parent->m_currentXForm._43 * _34))
                        + (float)(m_parent->m_currentXForm._33 * _33);
                    auto v42 = (float)(_34 * m_parent->m_currentXForm._44) + (float)(_33 * m_parent->m_currentXForm._34);
                    auto v43 = m_parent->m_currentXForm._14 * _31;
                    auto _41 = this->m_currentXForm._41;
                    auto v45 = v42 + v43;
                    auto v46 = m_parent->m_currentXForm._24 * _32;
                    auto _42 = this->m_currentXForm._42;
                    auto v48 = v45 + v46;
                    auto _43 = this->m_currentXForm._43;
                    this->m_currentXForm._34 = v48;
                    auto _44 = this->m_currentXForm._44;
                    this->m_currentXForm._41 = (float)((float)((float)(_44 * m_parent->m_currentXForm._41)
                                                               + (float)(_43 * m_parent->m_currentXForm._31))
                                                       + (float)(m_parent->m_currentXForm._11 * _41))
                        + (float)(m_parent->m_currentXForm._21 * _42);
                    this->m_currentXForm._42 = (float)((float)((float)(_41 * m_parent->m_currentXForm._12)
                                                               + (float)(m_parent->m_currentXForm._42 * _44))
                                                       + (float)(m_parent->m_currentXForm._32 * _43))
                        + (float)(_42 * m_parent->m_currentXForm._22);
                    this->m_currentXForm._43 = (float)((float)((float)(m_parent->m_currentXForm._23 * _42)
                                                               + (float)(_41 * m_parent->m_currentXForm._13))
                                                       + (float)(m_parent->m_currentXForm._43 * _44))
                        + (float)(m_parent->m_currentXForm._33 * _43);
                    this->m_currentXForm._44 = (float)((float)((float)(_44 * m_parent->m_currentXForm._44)
                                                               + (float)(_43 * m_parent->m_currentXForm._34))
                                                       + (float)(m_parent->m_currentXForm._14 * _41))
                        + (float)(m_parent->m_currentXForm._24 * _42);
                }

                auto v51 = this->m_currentXForm._42;
                auto v52 = this->m_currentXForm._43;
                auto v134 = this->m_currentXForm._41;
                this->m_currentWorldOrigin.x = v134;
                auto v135 = v51;
                this->m_currentWorldOrigin.y = v51;
                auto v136 = v52;
                this->m_currentWorldOrigin.z = v52;

                if (m_parent && m_parent->m_isRootNode)
                {
                    this->m_currentWorldRotation.x = this->m_rotation.x;
                    this->m_currentWorldRotation.y = this->m_rotation.y;
                    auto w = this->m_rotation.w;
                    this->m_currentWorldRotation.z = this->m_rotation.z;
                    this->m_currentWorldRotation.w = w;
                }
                else
                {
                    CMatrix unScale;
                    unScale.zero();

                    CMatrix rt;
                    auto v54 = v18 / this->m_scaling.x;
                    rt._11 = (float)((float)((float)(unScale._21 * this->m_currentXForm._12)
                                             + (float)(unScale._31 * this->m_currentXForm._13))
                                     + (float)(unScale._41 * this->m_currentXForm._14))
                        + (float)(this->m_currentXForm._11 * v54);
                    auto v55 = v18 / this->m_scaling.y;
                    auto v56 = v18 / this->m_scaling.z;
                    rt._12 = (float)((float)((float)(unScale._32 * this->m_currentXForm._13) + (float)(v55 * this->m_currentXForm._12))
                                     + (float)(unScale._42 * this->m_currentXForm._14))
                        + (float)(unScale._12 * this->m_currentXForm._11);
                    auto v57 = unScale._24 * this->m_currentXForm._12;
                    rt._13 = (float)((float)((float)(unScale._23 * this->m_currentXForm._12) + (float)(v56 * this->m_currentXForm._13))
                                     + (float)(unScale._43 * this->m_currentXForm._14))
                        + (float)(unScale._13 * this->m_currentXForm._11);
                    auto v58 = unScale._31 * this->m_currentXForm._23;
                    rt._14 = (float)((float)(v57 + (float)(unScale._34 * this->m_currentXForm._13))
                                     + (float)(unScale._14 * this->m_currentXForm._11))
                        + this->m_currentXForm._14;
                    auto v59 = (float)((float)((float)(v54 * this->m_currentXForm._21) + v58)
                                  + (float)(unScale._21 * this->m_currentXForm._22))
                        + (float)(unScale._41 * this->m_currentXForm._24);
                    auto v60 = unScale._12 * this->m_currentXForm._21;
                    rt._21 = v59;
                    rt._22 = (float)((float)((float)(unScale._32 * this->m_currentXForm._23) + v60)
                                     + (float)(unScale._42 * this->m_currentXForm._24))
                        + (float)(v55 * this->m_currentXForm._22);
                    auto v61 = unScale._14 * this->m_currentXForm._21;
                    rt._23 = (float)((float)((float)(v56 * this->m_currentXForm._23) + (float)(unScale._13 * this->m_currentXForm._21))
                                     + (float)(unScale._23 * this->m_currentXForm._22))
                        + (float)(unScale._43 * this->m_currentXForm._24);
                    auto v62 = (float)((float)((float)(unScale._34 * this->m_currentXForm._23) + v61)
                                  + (float)(unScale._24 * this->m_currentXForm._22))
                        + this->m_currentXForm._24;
                    auto v63 = unScale._31 * this->m_currentXForm._33;
                    rt._24 = v62;
                    auto v64 = (float)((float)((float)(v54 * this->m_currentXForm._31) + v63)
                                  + (float)(unScale._21 * this->m_currentXForm._32))
                        + (float)(unScale._41 * this->m_currentXForm._34);
                    auto v65 = unScale._12 * this->m_currentXForm._31;
                    rt._31 = v64;
                    auto v66 = v54 * this->m_currentXForm._41;
                    auto v67 = unScale._21 * this->m_currentXForm._42;
                    auto v68 = unScale._32 * this->m_currentXForm._43;
                    auto v69 = (float)((float)((float)(unScale._32 * this->m_currentXForm._33) + v65)
                                  + (float)(unScale._42 * this->m_currentXForm._34))
                        + (float)(v55 * this->m_currentXForm._32);
                    auto v70 = unScale._13 * this->m_currentXForm._31;
                    rt._32 = v69;
                    auto v71 = (float)((float)((float)(v56 * this->m_currentXForm._33) + v70)
                                  + (float)(unScale._23 * this->m_currentXForm._32))
                        + (float)(unScale._43 * this->m_currentXForm._34);
                    auto v72 = unScale._14 * this->m_currentXForm._31;
                    rt._33 = v71;
                    rt._34 = (float)((float)((float)(unScale._34 * this->m_currentXForm._33) + v72)
                                     + (float)(unScale._24 * this->m_currentXForm._32))
                        + this->m_currentXForm._34;
                    rt._41 = (float)((float)(v66 + (float)(unScale._31 * this->m_currentXForm._43)) + v67)
                        + (float)(unScale._41 * this->m_currentXForm._44);
                    auto v73 = (float)((float)((float)(v56 * this->m_currentXForm._43)
                                                    + (float)(unScale._13 * this->m_currentXForm._41))
                                            + (float)(unScale._23 * this->m_currentXForm._42))
                        + (float)(unScale._43 * this->m_currentXForm._44);
                    auto v74 = (float)((float)((float)(unScale._34 * this->m_currentXForm._43)
                                                    + (float)(unScale._14 * this->m_currentXForm._41))
                                            + (float)(unScale._24 * this->m_currentXForm._42))
                        + this->m_currentXForm._44;
                    rt._42 = (float)((float)(v68 + (float)(unScale._12 * this->m_currentXForm._41))
                                     + (float)(unScale._42 * this->m_currentXForm._44))
                        + (float)(v55 * this->m_currentXForm._42);

                    // TODO: check order
                    rt._43 = v74;
                    rt._44 = v73;


                    unScale = rt;
                    auto p_m_currentWorldRotation = &this->m_currentWorldRotation;
                    m_currentWorldRotation.FromMatrix(unScale);
                    auto x = (float)((float)((float)(p_m_currentWorldRotation->x * p_m_currentWorldRotation->x)
                                        + (float)(this->m_currentWorldRotation.y * this->m_currentWorldRotation.y))
                                + (float)(this->m_currentWorldRotation.z * this->m_currentWorldRotation.z))
                        + (float)(this->m_currentWorldRotation.w * this->m_currentWorldRotation.w);
                    if (x <= 0.0)
                    {
                        p_m_currentWorldRotation->x = 0.0;
                        this->m_currentWorldRotation.y = 0.0;
                        this->m_currentWorldRotation.z = 0.0;
                        this->m_currentWorldRotation.w = 1.0;
                    }
                    else
                    {
                        auto v76 = 1.0 / sqrt(x);
                        p_m_currentWorldRotation->x = v76 * p_m_currentWorldRotation->x;
                        this->m_currentWorldRotation.y = v76 * this->m_currentWorldRotation.y;
                        this->m_currentWorldRotation.z = v76 * this->m_currentWorldRotation.z;
                        this->m_currentWorldRotation.w = v76 * this->m_currentWorldRotation.w;
                    }
                }

                m3d::pClient->GetWorld().GetLandscape().UpdateNodeCollisionGeoms(this);
            }
        }

        if (onlyVis)
        {
            if (parentDirty || this->m_isChildDirty)
            {
                this->m_isChildDirty = 0;
                for (m3d::SgNode* firstChild = (m3d::SgNode*)this->GetFirstChild(); firstChild; firstChild = (m3d::SgNode*)firstChild->GetNextSibling())
                {
                    ++cntUpdateNeededChecks;
                    firstChild->UpdateXForm(onlyVis, parentDirty);
                }
            }
        }
        else
        {
            for (m3d::SgNode* firstChild = (m3d::SgNode*)this->GetFirstChild(); firstChild; firstChild = (m3d::SgNode*)firstChild->GetNextSibling())
            {
                firstChild->UpdateXForm(false, parentDirty);
            }
        }

        if (boxWasDirty || v138)
        {
            auto v7 = !this->m_isRootNode;
            this->m_boundingBox.m_box[0] = this->m_ownBoundingBox.m_box[0];
            this->m_boundingBox.m_box[1] = this->m_ownBoundingBox.m_box[1];
            this->m_boundingBox.m_box[2] = this->m_ownBoundingBox.m_box[2];
            this->m_boundingBox.m_box[3] = this->m_ownBoundingBox.m_box[3];
            auto v80 = this->m_ownBoundingBox.m_box[5];
            this->m_boundingBox.m_box[4] = this->m_ownBoundingBox.m_box[4];
            this->m_boundingBox.m_box[5] = v80;
            if (v7)
            {
                for (m3d::SgNode* j = (m3d::SgNode*)this->GetFirstChild(); j; j = (m3d::SgNode*)j->GetNextSibling())
                {
                    if (j->GetClass() != &m3d::SgSoundSourceNode::m_classSgSoundSourceNode
                        && j->GetClass() != &m3d::SgSpriteNode::m_classSgSpriteNode)
                    {
                        Aabb box;
                        auto v82 = j->m_boundingBox.m_box[1];
                        box.m_box[0] = j->m_boundingBox.m_box[0];
                        auto v83 = j->m_boundingBox.m_box[2];
                        box.m_box[0] = box.m_box[0] + j->m_origin.x;
                        box.m_box[1] = v82;
                        auto v84 = j->m_boundingBox.m_box[3];
                        box.m_box[1] = box.m_box[1] + j->m_origin.y;
                        box.m_box[2] = v83;
                        auto v85 = v83 + j->m_origin.z;
                        auto v86 = j->m_boundingBox.m_box[4];
                        box.m_box[2] = v85;
                        box.m_box[3] = v84;
                        auto v87 = v84 + j->m_origin.x;
                        auto v88 = j->m_boundingBox.m_box[5];
                        box.m_box[3] = v87;
                        box.m_box[4] = v86;
                        auto v89 = v86 + j->m_origin.y;
                        box.m_box[5] = v88;
                        box.m_box[4] = v89;
                        box.m_box[5] = v88 + j->m_origin.z;
                        this->m_boundingBox.EmbraceBox(box);
                    }
                }
            }
            auto v90 = this->m_boundingBox.m_box[2];
            auto v91 = this->m_boundingBox.m_box[5];
            auto v92 = this->m_boundingBox.m_box[1];
            auto v93 = this->m_boundingBox.m_box[4];
            CVector origin;
            origin.x = this->m_boundingBox.m_box[3] - this->m_boundingBox.m_box[0];
            origin.z = v91 - v90;
            origin.y = v93 - v92;
            this->m_boundingRadius = sqrt(
                origin.x * origin.x
                + (float)(v91 - v90) * (float)(v91 - v90)
                + (float)(v93 - v92) * (float)(v93 - v92))
                * 0.5;
        }

        CVector origin; 
        auto v94 = this->m_boundingBox.m_box[5];
        auto v95 = this->m_boundingBox.m_box[4];
        auto v96 = this->m_boundingBox.m_box[2];
        auto v97 = this->m_boundingBox.m_box[1];
        origin.x = (float)(this->m_boundingBox.m_box[0] + this->m_boundingBox.m_box[3]) * 0.5;
        auto y = this->m_currentWorldRotation.x * this->m_currentWorldRotation.x;
        auto v98 = v96 + v94;
        auto v99 = this->m_currentWorldRotation.z * this->m_currentWorldRotation.w;
        auto v100 = v97 + v95;
        auto v101 = this->m_currentWorldRotation.w * this->m_currentWorldRotation.x;
        auto v142 = this->m_currentWorldRotation.y * this->m_currentWorldRotation.x;
        auto v102 = this->m_currentWorldRotation.z * this->m_currentWorldRotation.y;
        auto v103 = this->m_currentWorldRotation.z * this->m_currentWorldRotation.x;
        origin.z = v98 * 0.5;
        auto v104 = this->m_currentWorldRotation.z;
        auto x = v102;
        auto v105 = this->m_currentWorldRotation.y * this->m_currentWorldRotation.w;
        origin.y = v100 * 0.5;
        auto v106 = this->m_currentWorldRotation.y;
        auto z = v105;
        auto v107 = v106 * v106;

        CMatrix rt;
        rt._21 = (float)(v142 - v99) * 2.0;
        rt._31 = (float)(v105 + v103) * 2.0;
        rt._22 = 1.0 - (float)((float)((float)(v104 * v104) + y) * 2.0);
        rt._23 = (float)(v101 + x) * 2.0;
        auto v108 = (float)((float)((float)((float)(1.0 - (float)((float)((float)(v104 * v104) + v107) * 2.0)) * origin.x)
                               + (float)(origin.z * rt._31))
                       + (float)(origin.y * rt._21))
            + this->m_currentWorldOrigin.x;
        auto v109 = (float)((float)((float)((float)(v103 - v105) * 2.0) * origin.x)
                       + (float)(origin.z * (float)(1.0 - (float)((float)(v107 + y) * 2.0))))
            + (float)(origin.y * rt._23);
        auto v135 = this->m_currentWorldOrigin.y
            + (float)((float)((float)((float)((float)(v99 + v142) * 2.0) * origin.x)
                              + (float)(origin.z * (float)((float)(x - v101) * 2.0)))
                      + (float)(origin.y * rt._22));
        auto v110 = this->m_currentWorldOrigin.z;
        auto v111 = v135;
        auto v134 = v108;
        this->m_originWorldAbsForSphere.x = v108;
        this->m_originWorldAbsForSphere.y = v111;
        auto v136 = v110 + v109;
        this->m_originWorldAbsForSphere.z = v110 + v109;
        if (this->GetClass() == &m3d::SgNode::m_classSgNode)
        {
            auto v112 = this->m_boundingBox.m_box[1];
            this->m_ownBoundingBox.m_box[0] = this->m_boundingBox.m_box[0];
            auto v113 = this->m_boundingBox.m_box[2];
            this->m_ownBoundingBox.m_box[1] = v112;
            auto v114 = this->m_boundingBox.m_box[3];
            this->m_ownBoundingBox.m_box[2] = v113;
            auto v115 = this->m_boundingBox.m_box[4];
            this->m_ownBoundingBox.m_box[3] = v114;
            auto v116 = this->m_boundingBox.m_box[5];
            this->m_ownBoundingBox.m_box[4] = v115;
            this->m_ownBoundingBox.m_box[5] = v116;

            this->m_currentXForm.DecomposeScale(x, y, z);
            auto v117 = this->m_ownBoundingBox.m_box[3] * (float)(1.0 / x);
            auto v118 = 1.0 / y;
            auto v119 = 1.0 / z;
            this->m_ownBoundingBox.m_box[0] = (float)(1.0 / x) * this->m_ownBoundingBox.m_box[0];
            auto v120 = v118 * this->m_ownBoundingBox.m_box[4];
            auto v121 = v118 * this->m_ownBoundingBox.m_box[1];
            this->m_ownBoundingBox.m_box[4] = v120;
            auto v122 = v119 * this->m_ownBoundingBox.m_box[5];
            auto v123 = v119 * this->m_ownBoundingBox.m_box[2];
            this->m_ownBoundingBox.m_box[3] = v117;
            this->m_ownBoundingBox.m_box[1] = v121;
            this->m_ownBoundingBox.m_box[5] = v122;
            this->m_ownBoundingBox.m_box[2] = v123;
        }

        if (this->m_isRootNode)
        {
            CStr checks = "UpdateXForm checks = " + CStr(cntUpdateNeededChecks);
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(checks.c_str());

            CStr updateXForms = "UpdateXForms = " + CStr(cntChildUpdates);
            M3D_APP->GetDbgCounterStack().DrawStringThisFrame(updateXForms.c_str());
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
        return p0.x != m_forGraph->m_cellsCoveredPoint0.x
            || p0.y != m_forGraph->m_cellsCoveredPoint0.y
            || p1.x != m_forGraph->m_cellsCoveredPoint1.x
            || p1.y != m_forGraph->m_cellsCoveredPoint1.y;
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

    int SgNode::GetPropertiesList(retruxx::set<unsigned, retruxx::less<unsigned>, retruxx::allocator<unsigned>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Aabb SgNode::GetOwnAabb() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    float SgNode::IntersectRay(CVector const&, CVector const&, SgNode*&, Class*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::Restart()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CMatrix SgNode::MatrixFromFlags(SgNodeRenderFlags, void*) const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    SgNode::SgNode(SgNode const&)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::InternalInit()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

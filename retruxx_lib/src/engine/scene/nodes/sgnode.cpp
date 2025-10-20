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

        ++cntChildUpdates;
        bool v6 = this->m_isOwnBoundingBoxDirty | (this->m_isXFormDirty & 1);
        bool v136 = m_isChildDirty;
        bool boxWasDirty = v6;

        if (v6)
        {
            this->UpdateOwnBoundingBox();
            this->m_ownBoundingBox.m_box[0] = this->m_scaling.x * this->m_ownBoundingBox.m_box[0];
            this->m_ownBoundingBox.m_box[3] = this->m_scaling.x * this->m_ownBoundingBox.m_box[3];
            this->m_ownBoundingBox.m_box[1] = this->m_ownBoundingBox.m_box[1] * this->m_scaling.y;
            this->m_ownBoundingBox.m_box[4] = this->m_ownBoundingBox.m_box[4] * this->m_scaling.y;
            this->m_ownBoundingBox.m_box[2] = this->m_ownBoundingBox.m_box[2] * this->m_scaling.z;
            this->m_ownBoundingBox.m_box[5] = this->m_scaling.z * this->m_ownBoundingBox.m_box[5];
            this->m_isOwnBoundingBoxDirty = 0;
        }

        bool shouldProcessChildren = true;

        CMatrix rt;
        if (this->m_isXFormDirty)
        {
            bool v7 = !this->m_isOriginRelative;
            this->m_isXFormDirty = 0;
            CVector origin = this->m_origin;

            if (!v7)
            {
                origin.y = m3d::pClient->GetWorld().GetLandscape().GetHeight(origin.x, origin.z, -1, 1) + origin.y;
            }

            float v8 = this->m_rotation.z;
            float v9 = this->m_rotation.y;
            float v10 = v8 * this->m_rotation.w;
            float v11 = this->m_rotation.x * v8;
            float z_temp = this->m_rotation.x * this->m_rotation.x;
            float v12 = this->m_rotation.x * this->m_rotation.w;
            float y_temp = this->m_rotation.x * this->m_rotation.y;
            float x_temp = this->m_rotation.z * this->m_rotation.y;
            float v140_temp = this->m_rotation.y * this->m_rotation.w;
            float v13 = v8 * v8;
            float v14 = v9 * v9;

            CMatrix unScale;
            unScale._11 = 1.0f - (float)((v13 + v14) * 2.0f);
            unScale._21 = (y_temp - v10) * 2.0f;
            unScale._31 = (v140_temp + v11) * 2.0f;
            unScale._12 = (v10 + y_temp) * 2.0f;
            unScale._22 = 1.0f - (float)((v13 + z_temp) * 2.0f);
            unScale._33 = 1.0f - (float)((v14 + z_temp) * 2.0f);
            unScale._32 = (x_temp - v12) * 2.0f;
            unScale._13 = (v11 - v140_temp) * 2.0f;
            unScale._23 = (v12 + x_temp) * 2.0f;
            memset(&unScale.m[2][3], 0, 16);
            unScale._44 = 1.0f;

            rt = unScale;
            this->m_ownXForm._11 = rt._11 * this->m_scaling.x;
            float _21 = rt._21;
            this->m_ownXForm._12 = rt._12 * this->m_scaling.x;
            this->m_ownXForm._13 = rt._13 * this->m_scaling.x;
            this->m_ownXForm._14 = 0.0f;
            this->m_ownXForm._21 = _21 * this->m_scaling.y;
            this->m_ownXForm._22 = rt._22 * this->m_scaling.y;
            this->m_ownXForm._23 = rt._23 * this->m_scaling.y;
            this->m_ownXForm._24 = 0.0f;
            this->m_ownXForm._31 = this->m_scaling.z * rt._31;
            this->m_ownXForm._32 = this->m_scaling.z * rt._32;
            float v16 = this->m_scaling.z * rt._33;
            this->m_ownXForm._34 = 0.0f;
            this->m_ownXForm._41 = origin.x;
            float v17 = origin.y;
            this->m_ownXForm._33 = v16;
            float v18 = 1.0f;
            this->m_ownXForm._42 = v17;
            this->m_ownXForm._43 = origin.z;
            this->m_ownXForm._44 = 1.0f;
            parentDirty = true;
        }
        else
        {
            if (!parentDirty)
            {
                shouldProcessChildren = false;
            }
            else
            {
                float v18 = 1.0f;

                m3d::SgNode* m_parent = (m3d::SgNode*)this->GetParent();
                memcpy(&this->m_currentXForm, &this->m_ownXForm, sizeof(this->m_currentXForm));

                if (m_parent)
                {
                    float _13 = this->m_currentXForm._13;
                    float _14 = this->m_currentXForm._14;
                    float _11 = this->m_currentXForm._11;
                    float _12 = this->m_currentXForm._12;

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
                    float v24 = (float)(_14 * m_parent->m_currentXForm._44) + (float)(_13 * m_parent->m_currentXForm._34);
                    float v25 = m_parent->m_currentXForm._14 * _11;
                    float v26 = this->m_currentXForm._21;
                    float v27 = v24 + v25;
                    float v28 = m_parent->m_currentXForm._24 * _12;
                    float _22 = this->m_currentXForm._22;
                    float v30 = v27 + v28;
                    float _23 = this->m_currentXForm._23;
                    this->m_currentXForm._14 = v30;
                    float _24 = this->m_currentXForm._24;
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
                    float v33 = (float)(_24 * m_parent->m_currentXForm._44) + (float)(_23 * m_parent->m_currentXForm._34);
                    float v34 = m_parent->m_currentXForm._14 * v26;
                    float _31 = this->m_currentXForm._31;
                    float v36 = v33 + v34;
                    float v37 = m_parent->m_currentXForm._24 * _22;
                    float _32 = this->m_currentXForm._32;
                    float v39 = v36 + v37;
                    float _33 = this->m_currentXForm._33;
                    this->m_currentXForm._24 = v39;
                    float _34 = this->m_currentXForm._34;
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
                    float v42 = (float)(_34 * m_parent->m_currentXForm._44) + (float)(_33 * m_parent->m_currentXForm._34);
                    float v43 = m_parent->m_currentXForm._14 * _31;
                    float _41 = this->m_currentXForm._41;
                    float v45 = v42 + v43;
                    float v46 = m_parent->m_currentXForm._24 * _32;
                    float _42 = this->m_currentXForm._42;
                    float v48 = v45 + v46;
                    float _43 = this->m_currentXForm._43;
                    this->m_currentXForm._34 = v48;
                    float _44 = this->m_currentXForm._44;
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

                float v51 = this->m_currentXForm._42;
                float v52 = this->m_currentXForm._43;
                this->m_currentWorldOrigin.x = this->m_currentXForm._41;
                this->m_currentWorldOrigin.y = v51;
                this->m_currentWorldOrigin.z = v52;

                if (m_parent && m_parent->m_isRootNode)
                {
                    this->m_currentWorldRotation.x = this->m_rotation.x;
                    this->m_currentWorldRotation.y = this->m_rotation.y;
                    float w = this->m_rotation.w;
                    this->m_currentWorldRotation.z = this->m_rotation.z;
                    this->m_currentWorldRotation.w = w;
                }
                else
                {
                    CMatrix unScale;
                    float v54 = v18 / this->m_scaling.x;
                    rt._11 = (float)((float)((float)(unScale._21 * this->m_currentXForm._12)
                        + (float)(unScale._31 * this->m_currentXForm._13))
                        + (float)(unScale._41 * this->m_currentXForm._14))
                        + (float)(this->m_currentXForm._11 * v54);
                    float v55 = v18 / this->m_scaling.y;
                    float v56 = v18 / this->m_scaling.z;
                    rt._12 = (float)((float)((float)(unScale._32 * this->m_currentXForm._13) + (float)(v55 * this->m_currentXForm._12))
                        + (float)(unScale._42 * this->m_currentXForm._14))
                        + (float)(unScale._12 * this->m_currentXForm._11);
                    float v57 = unScale._24 * this->m_currentXForm._12;
                    rt._13 = (float)((float)((float)(unScale._23 * this->m_currentXForm._12) + (float)(v56 * this->m_currentXForm._13))
                        + (float)(unScale._43 * this->m_currentXForm._14))
                        + (float)(unScale._13 * this->m_currentXForm._11);
                    float v58 = unScale._31 * this->m_currentXForm._23;
                    rt._14 = (float)((float)(v57 + (float)(unScale._34 * this->m_currentXForm._13))
                        + (float)(unScale._14 * this->m_currentXForm._11))
                        + this->m_currentXForm._14;
                    float v59 = (float)((float)((float)(v54 * this->m_currentXForm._21) + v58)
                        + (float)(unScale._21 * this->m_currentXForm._22))
                        + (float)(unScale._41 * this->m_currentXForm._24);
                    float v60 = unScale._12 * this->m_currentXForm._21;
                    rt._21 = v59;
                    rt._22 = (float)((float)((float)(unScale._32 * this->m_currentXForm._23) + v60)
                        + (float)(unScale._42 * this->m_currentXForm._24))
                        + (float)(v55 * this->m_currentXForm._22);
                    float v61 = unScale._14 * this->m_currentXForm._21;
                    rt._23 = (float)((float)((float)(v56 * this->m_currentXForm._23) + (float)(unScale._13 * this->m_currentXForm._21))
                        + (float)(unScale._23 * this->m_currentXForm._22))
                        + (float)(unScale._43 * this->m_currentXForm._24);
                    float v62 = (float)((float)((float)(unScale._34 * this->m_currentXForm._23) + v61)
                        + (float)(unScale._24 * this->m_currentXForm._22))
                        + this->m_currentXForm._24;
                    float v63 = unScale._31 * this->m_currentXForm._33;
                    rt._24 = v62;
                    float v64 = (float)((float)((float)(v54 * this->m_currentXForm._31) + v63)
                        + (float)(unScale._21 * this->m_currentXForm._32))
                        + (float)(unScale._41 * this->m_currentXForm._34);
                    float v65 = unScale._12 * this->m_currentXForm._31;
                    rt._31 = v64;
                    float v66 = v54 * this->m_currentXForm._41;
                    float v67 = unScale._21 * this->m_currentXForm._42;
                    float v68 = unScale._32 * this->m_currentXForm._43;
                    float v69 = (float)((float)((float)(unScale._32 * this->m_currentXForm._33) + v65)
                        + (float)(unScale._42 * this->m_currentXForm._34))
                        + (float)(v55 * this->m_currentXForm._32);
                    float v70 = unScale._13 * this->m_currentXForm._31;
                    rt._32 = v69;
                    float v71 = (float)((float)((float)(v56 * this->m_currentXForm._33) + v70)
                        + (float)(unScale._23 * this->m_currentXForm._32))
                        + (float)(unScale._43 * this->m_currentXForm._34);
                    float v72 = unScale._14 * this->m_currentXForm._31;
                    rt._33 = v71;
                    rt._34 = (float)((float)((float)(unScale._34 * this->m_currentXForm._33) + v72)
                        + (float)(unScale._24 * this->m_currentXForm._32))
                        + this->m_currentXForm._34;
                    rt._41 = (float)((float)(v66 + (float)(unScale._31 * this->m_currentXForm._43)) + v67)
                        + (float)(unScale._41 * this->m_currentXForm._44);
                    float v73 = (float)((float)((float)(v56 * this->m_currentXForm._43)
                        + (float)(unScale._13 * this->m_currentXForm._41))
                        + (float)(unScale._23 * this->m_currentXForm._42))
                        + (float)(unScale._43 * this->m_currentXForm._44);
                    float v74 = (float)((float)((float)(unScale._34 * this->m_currentXForm._43)
                        + (float)(unScale._14 * this->m_currentXForm._41))
                        + (float)(unScale._24 * this->m_currentXForm._42))
                        + this->m_currentXForm._44;
                    rt._42 = (float)((float)(v68 + (float)(unScale._12 * this->m_currentXForm._41))
                        + (float)(unScale._42 * this->m_currentXForm._44))
                        + (float)(v55 * this->m_currentXForm._42);
                    rt._43 = v73;
                    rt._44 = v74;

                    unScale = rt;
                    m_currentWorldRotation.FromMatrix(unScale);

                    float x = (float)((float)((float)(m_currentWorldRotation.x * m_currentWorldRotation.x)
                        + (float)(this->m_currentWorldRotation.y * this->m_currentWorldRotation.y))
                        + (float)(this->m_currentWorldRotation.z * this->m_currentWorldRotation.z))
                        + (float)(this->m_currentWorldRotation.w * this->m_currentWorldRotation.w);
                    if (x <= 0.0f) {
                        m_currentWorldRotation.x = 0.0f;
                        this->m_currentWorldRotation.y = 0.0f;
                        this->m_currentWorldRotation.z = 0.0f;
                        this->m_currentWorldRotation.w = 1.0f;
                    }
                    else {
                        float v76 = 1.0f / sqrtf(x);
                        m_currentWorldRotation.x = v76 * m_currentWorldRotation.x;
                        this->m_currentWorldRotation.y = v76 * this->m_currentWorldRotation.y;
                        this->m_currentWorldRotation.z = v76 * this->m_currentWorldRotation.z;
                        this->m_currentWorldRotation.w = v76 * this->m_currentWorldRotation.w;
                    }
                }

                m3d::pClient->GetWorld().GetLandscape().UpdateNodeCollisionGeoms(this);
            }
        }

        if (shouldProcessChildren)
        {
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
        }

        if (boxWasDirty || v136)
        {
            bool v7 = !this->m_isRootNode;
            this->m_boundingBox.m_box[0] = this->m_ownBoundingBox.m_box[0];
            this->m_boundingBox.m_box[1] = this->m_ownBoundingBox.m_box[1];
            this->m_boundingBox.m_box[2] = this->m_ownBoundingBox.m_box[2];
            this->m_boundingBox.m_box[3] = this->m_ownBoundingBox.m_box[3];
            float v80 = this->m_ownBoundingBox.m_box[5];
            this->m_boundingBox.m_box[4] = this->m_ownBoundingBox.m_box[4];
            this->m_boundingBox.m_box[5] = v80;

            if (v7)
            {
                for (m3d::SgNode* j = (m3d::SgNode*)this->GetFirstChild(); j; j = (m3d::SgNode*)j->GetNextSibling())
                {
                    if (j->GetClass() != &m3d::SgSoundSourceNode::m_classSgSoundSourceNode &&
                        j->GetClass() != &m3d::SgSpriteNode::m_classSgSpriteNode)
                    {

                        float v82 = j->m_boundingBox.m_box[1];
                        Aabb box;
                        box.m_box[0] = j->m_boundingBox.m_box[0];
                        float v83 = j->m_boundingBox.m_box[2];
                        box.m_box[0] = box.m_box[0] + j->m_origin.x;
                        box.m_box[1] = v82;
                        float v84 = j->m_boundingBox.m_box[3];
                        box.m_box[1] = box.m_box[1] + j->m_origin.y;
                        box.m_box[2] = v83;
                        float v85 = v83 + j->m_origin.z;
                        float v86 = j->m_boundingBox.m_box[4];
                        box.m_box[2] = v85;
                        box.m_box[3] = v84;
                        float v87 = v84 + j->m_origin.x;
                        float v88 = j->m_boundingBox.m_box[5];
                        box.m_box[3] = v87;
                        box.m_box[4] = v86;
                        float v89 = v86 + j->m_origin.y;
                        box.m_box[5] = v88;
                        box.m_box[4] = v89;
                        box.m_box[5] = v88 + j->m_origin.z;
                        m_boundingBox.EmbraceBox(box);
                    }
                }
            }

            float v90 = this->m_boundingBox.m_box[2];
            float v91 = this->m_boundingBox.m_box[5];
            float v92 = this->m_boundingBox.m_box[1];
            float v93 = this->m_boundingBox.m_box[4];
            CVector origin;
            origin.x = this->m_boundingBox.m_box[3] - this->m_boundingBox.m_box[0];
            origin.z = v91 - v90;
            origin.y = v93 - v92;
            this->m_boundingRadius = sqrtf(
                origin.x * origin.x
                + (float)(v91 - v90) * (float)(v91 - v90)
                + (float)(v93 - v92) * (float)(v93 - v92))
                * 0.5f;
        }

        float v94 = this->m_boundingBox.m_box[5];
        float v95 = this->m_boundingBox.m_box[4];
        float v96 = this->m_boundingBox.m_box[2];
        float v97 = this->m_boundingBox.m_box[1];
        CVector origin;
        origin.x = (float)(this->m_boundingBox.m_box[0] + this->m_boundingBox.m_box[3]) * 0.5f;
        float y_rot = this->m_currentWorldRotation.x * this->m_currentWorldRotation.x;
        float v98 = v96 + v94;
        float v99 = this->m_currentWorldRotation.z * this->m_currentWorldRotation.w;
        float v100 = v97 + v95;
        float v101 = this->m_currentWorldRotation.w * this->m_currentWorldRotation.x;
        float v140_rot = this->m_currentWorldRotation.y * this->m_currentWorldRotation.x;
        float v102 = this->m_currentWorldRotation.z * this->m_currentWorldRotation.y;
        float v103 = this->m_currentWorldRotation.z * this->m_currentWorldRotation.x;
        origin.z = v98 * 0.5f;
        float v104 = this->m_currentWorldRotation.z;
        float x_rot = v102;
        float v105 = this->m_currentWorldRotation.y * this->m_currentWorldRotation.w;
        origin.y = v100 * 0.5f;
        float v106 = this->m_currentWorldRotation.y;
        float z_rot = v105;
        float v107 = v106 * v106;
        rt._21 = (v140_rot - v99) * 2.0f;
        rt._31 = (v105 + v103) * 2.0f;
        rt._22 = 1.0f - (float)((float)((v104 * v104) + y_rot) * 2.0f);
        rt._23 = (v101 + x_rot) * 2.0f;
        float v108 = (float)((float)((float)((float)(1.0f - (float)((float)((v104 * v104) + v107) * 2.0f)) * origin.x)
            + (float)(origin.z * rt._31))
            + (float)(origin.y * rt._21))
            + this->m_currentWorldOrigin.x;
        float v109 = (float)((float)((float)((v103 - v105) * 2.0f) * origin.x)
            + (float)(origin.z * (1.0f - (float)((v107 + y_rot) * 2.0f))))
            + (float)(origin.y * rt._23);
        float v134_y = this->m_currentWorldOrigin.y
            + (float)((float)((float)((float)((v99 + v140_rot) * 2.0f) * origin.x)
                + (float)(origin.z * (float)((x_rot - v101) * 2.0f)))
                + (float)(origin.y * rt._22));
        float v110 = this->m_currentWorldOrigin.z;
        this->m_originWorldAbsForSphere.x = v108;
        this->m_originWorldAbsForSphere.y = v134_y;
        this->m_originWorldAbsForSphere.z = v110 + v109;

        if (this->GetClass() == &m3d::SgNode::m_classSgNode)
        {
            float v112 = this->m_boundingBox.m_box[1];
            this->m_ownBoundingBox.m_box[0] = this->m_boundingBox.m_box[0];
            float v113 = this->m_boundingBox.m_box[2];
            this->m_ownBoundingBox.m_box[1] = v112;
            float v114 = this->m_boundingBox.m_box[3];
            this->m_ownBoundingBox.m_box[2] = v113;
            float v115 = this->m_boundingBox.m_box[4];
            this->m_ownBoundingBox.m_box[3] = v114;
            float v116 = this->m_boundingBox.m_box[5];
            this->m_ownBoundingBox.m_box[4] = v115;
            this->m_ownBoundingBox.m_box[5] = v116;

            float scaleX, scaleY, scaleZ;
            m_currentXForm.DecomposeScale(scaleX, scaleY, scaleZ);

            float v117 = this->m_ownBoundingBox.m_box[3] * (1.0f / scaleX);
            float v118 = 1.0f / scaleY;
            float v119 = 1.0f / scaleZ;
            this->m_ownBoundingBox.m_box[0] = (1.0f / scaleX) * this->m_ownBoundingBox.m_box[0];
            float v120 = v118 * this->m_ownBoundingBox.m_box[4];
            float v121 = v118 * this->m_ownBoundingBox.m_box[1];
            this->m_ownBoundingBox.m_box[4] = v120;
            float v122 = v119 * this->m_ownBoundingBox.m_box[5];
            float v123 = v119 * this->m_ownBoundingBox.m_box[2];
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

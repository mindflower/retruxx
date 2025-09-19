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

RT_CLASS_EXPORT_METHOD_DEFINE(SgNode, GetOrigin)
{
    throw retruxx::logic_error("Not implemented");
}

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

    int SgNode::SetScale(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::SetTransparencyType(TransparencyType)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void SgNode::SetPrevThinkTime(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector const& SgNode::GetScale() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    bool SgNode::IsFree() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::SetRotation(Quaternion const&)
    {
        throw retruxx::logic_error("Not implemented");
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

    int SgNode::SetOriginAbs(CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgNode::UpdateXForm(bool, bool)
    {
        //TODO: implement SgNode::UpdateXForm
        return 1;
        //throw retruxx::logic_error("Not implemented");
    }

    bool SgNode::VisCellBoundsChanged() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
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

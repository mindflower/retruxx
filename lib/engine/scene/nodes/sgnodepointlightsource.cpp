#include <scene/nodes/sgnodepointlightsource.h>
#include <m3dapp.h>
#include "scene/servers/dataserver.h"
#include <scene/servers/serverlight.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgPointLightSourceNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgPointLightSourceNode);

    Class* SgPointLightSourceNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgPointLightSourceNode::CreateObject()
    {
        return new SgPointLightSourceNode;
    }

    Object* SgPointLightSourceNode::Clone()
    {
        return new SgPointLightSourceNode(*this);
    }

    DataServer* SgPointLightSourceNode::GetServer() const
    {
        return &M3D_APP->GetLightsServer();
    }

    int SgPointLightSourceNode::GetProperty(unsigned propId, void* property) const
    {
        if (m3d::SgNode::GetProperty(propId, property))
            return 1;
        if (propId == 4360)
        {
            *(int*)property = m_srvId;
            return 1;
        }
        auto v5 = propId - 8448;
        if ((int)(propId - 8448) >= 0 && v5 < 3)
            *(int*)property = this->m_props[v5];
        return 0;
    }

    int SgPointLightSourceNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        // RVA 0x79E1F0 - the radius property holds float bits.
        if (m_srvId == -1)
        {
            return 0;
        }
        m3d::RiForLightsServer ri;
        ri.m_localXForm = m_currentXForm;
        memcpy(&ri.m_radius, &m_props[1], sizeof(ri.m_radius));
        GetServer()->RenderItem(m_srvId, &ri);
        return 1;
    }

    int SgPointLightSourceNode::SetProperty(unsigned propId, void* property)
    {
        if (SgNode::SetProperty(propId, property))
            return 1;

        if (propId == 4360)
        {
            this->m_srvId = *(int*)property;
            return 1;
        }
        int v5 = propId - 8448;
        if ((int)(propId - 8448) >= 0 && v5 < 3)
            this->m_props[v5] = *(int*)property;
        return 0;
    }

    int SgPointLightSourceNode::GetPropertiesList(retruxx::set<unsigned>& props) const
    {
        // RVA 0x79E260
        if (!SgNode::GetPropertiesList(props))
        {
            return 0;
        }
        props.insert(PROP_NODE_HANDLE);
        props.insert(PROP_LS_COLOR);
        props.insert(PROP_LS_RADIUS);
        props.insert(PROP_LS_BRIGHTNESS);
        return 1;
    }

    Class* SgPointLightSourceNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgPointLightSourceNode);
    }

    SgPointLightSourceNode::~SgPointLightSourceNode()
    {
        RitualInDestructor();
    }

    SgPointLightSourceNode::SgPointLightSourceNode()
    {
        *(float*)&this->m_props[1] = 10.0;
        this->m_props[0] = -1;
        this->m_props[2] = -1;
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }

    SgPointLightSourceNode::SgPointLightSourceNode(SgPointLightSourceNode const& node) : SgNode(node)
    {
        m_props[0] = node.m_props[0];
        m_props[1] = node.m_props[1];
        m_props[2] = node.m_props[2];
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }

    void SgPointLightSourceNode::UpdateOwnBoundingBox()
    {
        if (this->m_srvId == -1)
        {
            this->m_ownBoundingBox.m_box[0] = 0.0f;
            this->m_ownBoundingBox.m_box[1] = 0.0f;
            this->m_ownBoundingBox.m_box[3] = 0.0f;
            this->m_ownBoundingBox.m_box[4] = 0.0f;
            this->m_ownBoundingBox.m_box[5] = 0.0f;
            this->m_ownBoundingBox.m_box[2] = 0.0f;
        }
        else
        {
            auto r = 10.0f;
            auto* v3 = GetServer();
            v3->GetItemProperty(this->m_srvId, 8449, &r);
            this->m_ownBoundingBox.m_box[0] = -r;
            this->m_ownBoundingBox.m_box[1] = -r;
            this->m_ownBoundingBox.m_box[2] = -r;
            this->m_ownBoundingBox.m_box[3] =  r;
            this->m_ownBoundingBox.m_box[4] =  r;
            this->m_ownBoundingBox.m_box[5] =  r;
        }
    }
}  // namespace m3d

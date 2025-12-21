#include <scene/nodes/sgnodepointlightsource.h>
#include <m3dapp.h>
#include "script/servers/dataserver.h"

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
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

    int SgPointLightSourceNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    SgPointLightSourceNode::SgPointLightSourceNode(SgPointLightSourceNode const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgPointLightSourceNode::UpdateOwnBoundingBox()
    {
        if (this->m_srvId == -1)
        {
            this->m_ownBoundingBox.m_box[0] = 0.0;
            this->m_ownBoundingBox.m_box[1] = 0.0;
            this->m_ownBoundingBox.m_box[3] = 0.0;
            this->m_ownBoundingBox.m_box[4] = 0.0;
            this->m_ownBoundingBox.m_box[5] = 0.0;
            this->m_ownBoundingBox.m_box[2] = 0.0;
        }
        else
        {
            auto r = 10.0;
            auto* v3 = GetServer();
            v3->GetItemProperty(this->m_srvId, 8449, &r);
            auto v4 = r;
            auto v5 = 0.0 - r;
            this->m_ownBoundingBox.m_box[0] = 0.0 - r;
            this->m_ownBoundingBox.m_box[1] = v5;
            this->m_ownBoundingBox.m_box[3] = v4;
            this->m_ownBoundingBox.m_box[4] = v4;
            this->m_ownBoundingBox.m_box[5] = v4;
            this->m_ownBoundingBox.m_box[2] = v5;
        }
    }
}  // namespace m3d

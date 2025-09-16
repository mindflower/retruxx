#include <stdexcept>
#include <scene/nodes/sgnodepointlightsource.h>
#include <m3dapp.h>

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
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgPointLightSourceNode::GetServer() const
    {
        return &M3D_APP->GetLightsServer();
    }

    int SgPointLightSourceNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgPointLightSourceNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgPointLightSourceNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgPointLightSourceNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgPointLightSourceNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgPointLightSourceNode);
    }

    SgPointLightSourceNode::~SgPointLightSourceNode()
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    void SgPointLightSourceNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

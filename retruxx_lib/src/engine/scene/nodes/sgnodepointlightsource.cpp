#include <stdexcept>
#include <scene/nodes/sgnodepointlightsource.h>

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
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgPointLightSourceNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgPointLightSourceNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    SgPointLightSourceNode::~SgPointLightSourceNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgPointLightSourceNode::SgPointLightSourceNode()
    {
        throw retruxx::logic_error("Not implemented");
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

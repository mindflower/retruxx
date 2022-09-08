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
        throw std::logic_error("Not implemented");
    }

    Object* SgPointLightSourceNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgPointLightSourceNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgPointLightSourceNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgPointLightSourceNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int SgPointLightSourceNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int SgPointLightSourceNode::GetPropertiesList(std::set<unsigned>&) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgPointLightSourceNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    SgPointLightSourceNode::~SgPointLightSourceNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgPointLightSourceNode::SgPointLightSourceNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgPointLightSourceNode::SgPointLightSourceNode(SgPointLightSourceNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    void SgPointLightSourceNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }
}

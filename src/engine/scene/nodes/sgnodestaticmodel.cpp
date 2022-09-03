#include <stdexcept>
#include <scene/nodes/sgnodestaticmodel.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgStaticModelNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgStaticModelNode);

    Object* SgStaticModelNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgStaticModelNode::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgStaticModelNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgStaticModelNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgStaticModelNode::GetPropertiesList(std::set<unsigned>&) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgStaticModelNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgStaticModelNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int SgStaticModelNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgStaticModelNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    SgStaticModelNode::SgStaticModelNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgStaticModelNode::SgStaticModelNode(SgStaticModelNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgStaticModelNode::~SgStaticModelNode()
    {
        throw std::logic_error("Not implemented");
    }

    void SgStaticModelNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }
}

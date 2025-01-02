#include <stdexcept>
#include <scene/nodes/sgnodestaticmodel.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgStaticModelNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgStaticModelNode);

    Object* SgStaticModelNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgStaticModelNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Class* SgStaticModelNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgStaticModelNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgStaticModelNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgStaticModelNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgStaticModelNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgStaticModelNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgStaticModelNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgStaticModelNode::SgStaticModelNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgStaticModelNode::SgStaticModelNode(SgStaticModelNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgStaticModelNode::~SgStaticModelNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgStaticModelNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

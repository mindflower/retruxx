#include <stdexcept>
#include <scene/nodes/sgnodestaticmodel.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgStaticModelNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgStaticModelNode);

    Object* SgStaticModelNode::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* SgStaticModelNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Class* SgStaticModelNode::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer* SgStaticModelNode::GetServer() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgStaticModelNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgStaticModelNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* SgStaticModelNode::Clone()
    {
        return new SgStaticModelNode(*this);
    }

    int SgStaticModelNode::GetProperty(unsigned, void*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgStaticModelNode::SetProperty(unsigned, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgStaticModelNode::SgStaticModelNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgStaticModelNode::SgStaticModelNode(SgStaticModelNode const& node) : SgNode(node)
    {
    }

    SgStaticModelNode::~SgStaticModelNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgStaticModelNode::UpdateOwnBoundingBox()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

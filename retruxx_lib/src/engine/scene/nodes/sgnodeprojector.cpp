#include <stdexcept>
#include <scene/nodes/sgnodeprojector.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgProjectorNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgProjectorNode);

    Class* SgProjectorNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgProjectorNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgProjectorNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgProjectorNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgProjectorNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgProjectorNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgProjectorNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgProjectorNode::GetPropertiesList(retruxx::set<unsigned, retruxx::less<unsigned>, retruxx::allocator<unsigned>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgProjectorNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgProjectorNode::~SgProjectorNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgProjectorNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgProjectorNode::SgProjectorNode(SgProjectorNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgProjectorNode::SgProjectorNode()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

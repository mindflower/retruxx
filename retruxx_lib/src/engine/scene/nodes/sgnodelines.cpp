#include <stdexcept>
#include <scene/nodes/sgnodelines.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgLinesNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgLinesNode);

    Class* SgLinesNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgLinesNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgLinesNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgLinesNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgLinesNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgLinesNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgLinesNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgLinesNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgLinesNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgLinesNode::~SgLinesNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgLinesNode::SgLinesNode(SgLinesNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgLinesNode::SgLinesNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgLinesNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

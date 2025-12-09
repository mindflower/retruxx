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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgLinesNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgLinesNode::SetProperty(unsigned, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer* SgLinesNode::GetServer() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgLinesNode::GetProperty(unsigned, void*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgLinesNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* SgLinesNode::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* SgLinesNode::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgLinesNode::~SgLinesNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgLinesNode::SgLinesNode(SgLinesNode const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgLinesNode::SgLinesNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgLinesNode::UpdateOwnBoundingBox()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

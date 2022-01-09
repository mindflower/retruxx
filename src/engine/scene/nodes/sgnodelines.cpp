#include <stdexcept>
#include <scene/nodes/sgnodelines.h>

namespace m3d
{
    Class* SgLinesNode::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgLinesNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    int SgLinesNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int SgLinesNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgLinesNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgLinesNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgLinesNode::GetPropertiesList(std::set<unsigned>&) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgLinesNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgLinesNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    SgLinesNode::~SgLinesNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgLinesNode::SgLinesNode(SgLinesNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgLinesNode::SgLinesNode()
    {
        throw std::logic_error("Not implemented");
    }

    void SgLinesNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }
}

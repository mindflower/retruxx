#include <stdexcept>
#include <scene/nodes/sgnodeprojector.h>

namespace m3d
{
    Class* SgProjectorNode::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgProjectorNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgProjectorNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgProjectorNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int SgProjectorNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgProjectorNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    int SgProjectorNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    int SgProjectorNode::GetPropertiesList(std::set<unsigned, std::less<unsigned>, std::allocator<unsigned>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgProjectorNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    SgProjectorNode::~SgProjectorNode()
    {
        throw std::logic_error("Not implemented");
    }

    void SgProjectorNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }

    SgProjectorNode::SgProjectorNode(SgProjectorNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgProjectorNode::SgProjectorNode()
    {
        throw std::logic_error("Not implemented");
    }
}

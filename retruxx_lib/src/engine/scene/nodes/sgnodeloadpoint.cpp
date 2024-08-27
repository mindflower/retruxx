#include <stdexcept>
#include <scene/nodes/sgnodeloadpoint.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgLoadpointNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgLoadpointNode);

    Class* SgLoadpointNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgLoadpointNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgLoadpointNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int SgLoadpointNode::Think(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgLoadpointNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    SgLoadpointNode::~SgLoadpointNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgLoadpointNode::SgLoadpointNode(SgLoadpointNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgLoadpointNode::SgLoadpointNode()
    {
        throw std::logic_error("Not implemented");
    }
}

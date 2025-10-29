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
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* SgLoadpointNode::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgLoadpointNode::Think(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* SgLoadpointNode::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgLoadpointNode::~SgLoadpointNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgLoadpointNode::SgLoadpointNode(SgLoadpointNode const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgLoadpointNode::SgLoadpointNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

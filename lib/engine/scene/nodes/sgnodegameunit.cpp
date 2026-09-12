#include <scene/nodes/sgnodegameunit.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgGameUnitNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgGameUnitNode);

    Object* SgGameUnitNode::CreateObject()
    {
        return new SgGameUnitNode;
    }

    Class* SgGameUnitNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgAnimatedModelNode);
    }

    Class* SgGameUnitNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgGameUnitNode);
    }

    Object* SgGameUnitNode::Clone()
    {
        return new SgGameUnitNode(*this);
    }

    SgGameUnitNode::SgGameUnitNode(m3d::SgGameUnitNode const& node) : SgAnimatedModelNode(node)
    {
    }

    SgGameUnitNode::SgGameUnitNode()
    {
    }
}

#include <scene/nodes/sgnodegameunit.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgGameUnitNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgGameUnitNode);

    Object* SgGameUnitNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgGameUnitNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgAnimatedModelNode);
    }

    Class* SgGameUnitNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgGameUnitNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    SgGameUnitNode::~SgGameUnitNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgGameUnitNode::SgGameUnitNode(m3d::SgGameUnitNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgGameUnitNode::SgGameUnitNode()
    {
        throw std::logic_error("Not implemented");
    }
}

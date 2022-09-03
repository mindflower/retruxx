#include <scene/nodes/sgnodedecals.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgDecalsNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgDecalsNode);

    Class* SgDecalsNode::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgDecalsNode::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    int SgDecalsNode::GetProperty(unsigned, void*) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned SgDecalsNode::GetNumDecals()
    {
        throw std::logic_error("Not implemented");
    }

    int SgDecalsNode::SetProperty(unsigned, void*)
    {
        throw std::logic_error("Not implemented");
    }

    DecalInfo const& SgDecalsNode::GetDecal(int)
    {
        throw std::logic_error("Not implemented");
    }

    DataServer* SgDecalsNode::GetServer() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* SgDecalsNode::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    int SgDecalsNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int SgDecalsNode::GetPropertiesList(std::set<unsigned>&) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* SgDecalsNode::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    SgDecalsNode::~SgDecalsNode()
    {
        throw std::logic_error("Not implemented");
    }

    SgDecalsNode::SgDecalsNode(SgDecalsNode const&)
    {
        throw std::logic_error("Not implemented");
    }

    SgDecalsNode::SgDecalsNode()
    {
        throw std::logic_error("Not implemented");
    }

    void SgDecalsNode::UpdateOwnBoundingBox()
    {
        throw std::logic_error("Not implemented");
    }
}

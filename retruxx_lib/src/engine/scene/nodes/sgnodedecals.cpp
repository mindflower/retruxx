#include <scene/nodes/sgnodedecals.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgDecalsNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgDecalsNode);

    Class* SgDecalsNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgDecalsNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgDecalsNode::GetProperty(unsigned, void*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned SgDecalsNode::GetNumDecals()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgDecalsNode::SetProperty(unsigned, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DecalInfo const& SgDecalsNode::GetDecal(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgDecalsNode::GetServer() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgDecalsNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgDecalsNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgDecalsNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgDecalsNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgDecalsNode::~SgDecalsNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgDecalsNode::SgDecalsNode(SgDecalsNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgDecalsNode::SgDecalsNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgDecalsNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

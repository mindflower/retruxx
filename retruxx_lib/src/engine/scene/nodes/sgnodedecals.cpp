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
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgDecalsNode::GetProperty(unsigned, void*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned SgDecalsNode::GetNumDecals()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgDecalsNode::SetProperty(unsigned, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DecalInfo const& SgDecalsNode::GetDecal(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer* SgDecalsNode::GetServer() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* SgDecalsNode::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgDecalsNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgDecalsNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* SgDecalsNode::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgDecalsNode::~SgDecalsNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgDecalsNode::SgDecalsNode(SgDecalsNode const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgDecalsNode::SgDecalsNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgDecalsNode::UpdateOwnBoundingBox()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

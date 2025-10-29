#include <stdexcept>
#include <scene/nodes/sgnodeprojector.h>
#include <m3dapp.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgProjectorNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgProjectorNode);

    Class* SgProjectorNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgProjectorNode::CreateObject()
    {
        return new SgProjectorNode;
    }

    Class* SgProjectorNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgProjectorNode);
    }

    int SgProjectorNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgProjectorNode::SetProperty(unsigned propId, void* property)
    {
        if (SgNode::SetProperty(propId, property))
            return 1;

        if (propId == 4360)
        {
            this->m_srvId = *(int*)property;
            return 1;
        }
        auto v5 = propId - 10240;
        if ((int)(propId - 10240) >= 0 && v5 < 2)
            this->m_props[v5] = *(int*)property;
        return 0;
    }

    DataServer* SgProjectorNode::GetServer() const
    {
        return &M3D_APP->GetProjectorsServer();
    }

    int SgProjectorNode::GetProperty(unsigned, void*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgProjectorNode::GetPropertiesList(retruxx::set<unsigned, retruxx::less<unsigned>, retruxx::allocator<unsigned>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Object* SgProjectorNode::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgProjectorNode::~SgProjectorNode()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgProjectorNode::UpdateOwnBoundingBox()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgProjectorNode::SgProjectorNode(SgProjectorNode const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SgProjectorNode::SgProjectorNode()
    {
        *(float*)&this->m_props[1] = 10.0;
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }
}

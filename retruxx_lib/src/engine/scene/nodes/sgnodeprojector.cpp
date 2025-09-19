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
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int SgProjectorNode::GetPropertiesList(retruxx::set<unsigned, retruxx::less<unsigned>, retruxx::allocator<unsigned>>&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgProjectorNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgProjectorNode::~SgProjectorNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgProjectorNode::UpdateOwnBoundingBox()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgProjectorNode::SgProjectorNode(SgProjectorNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgProjectorNode::SgProjectorNode()
    {
        *(float*)&this->m_props[1] = 10.0;
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }
}

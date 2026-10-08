#include <stdexcept>
#include <scene/nodes/sgnodeprojector.h>
#include <m3dapp.h>
#include <scene/servers/serverprojectors.h>

namespace m3d
{
    namespace
    {
        // What SgProjectorNode::Render hands ProjectorsServer::RenderItem: the
        // node's world transform with the projector's reach appended.
        struct ProjectorRenderInfo
        {
            /* 0x0000 */ CMatrix m_xform;
            /* 0x0040 */ float m_radius;
        };

        // Property ids this node understands beyond SgNode's own.
        unsigned const PROP_SERVER_ID = 4360;
        unsigned const PROP_FIRST = 10240;
        unsigned const PROP_RADIUS = 10241;
        unsigned const PROP_COUNT = 2;
    }  // namespace

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
        // RVA 0x79DDB0 - a projector with no server entry has no texture or
        // radius to project, so there is nothing to draw.
        if (m_srvId == -1)
        {
            return 0;
        }

        ProjectorRenderInfo ri;
        ri.m_xform = m_currentXForm;
        ri.m_radius = *reinterpret_cast<float const*>(&m_props[1]);

        GetServer()->RenderItem(m_srvId, &ri);
        return 1;
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

    int SgProjectorNode::GetProperty(unsigned propId, void* property) const
    {
        // RVA 0x79DC80 - the mirror of SetProperty, including its quirk.
        if (SgNode::GetProperty(propId, property))
        {
            return 1;
        }

        if (propId == PROP_SERVER_ID)
        {
            *static_cast<int*>(property) = m_srvId;
            return 1;
        }

        auto const idx = propId - PROP_FIRST;
        if (static_cast<int>(propId - PROP_FIRST) >= 0 && idx < PROP_COUNT)
        {
            *static_cast<int*>(property) = m_props[idx];
        }
        // NOTE: as shipped, the projector's own properties report failure even
        // when they were read, so callers that check the return value see them
        // as unsupported.
        return 0;
    }

    int SgProjectorNode::GetPropertiesList(
        retruxx::set<unsigned, retruxx::less<unsigned>, retruxx::allocator<unsigned>>& props) const
    {
        // RVA 0x79DE20 - NOTE: only the radius is advertised; m_props[0] has no
        // id in this list even though SetProperty accepts 10240.
        if (!SgNode::GetPropertiesList(props))
        {
            return 0;
        }

        props.insert(PROP_SERVER_ID);
        props.insert(PROP_RADIUS);
        return 1;
    }

    Object* SgProjectorNode::Clone()
    {
        // RVA 0x79DF20
        return new SgProjectorNode(*this);
    }

    SgProjectorNode::~SgProjectorNode()
    {
        RitualInDestructor();
    }

    void SgProjectorNode::UpdateOwnBoundingBox()
    {
        // RVA 0x79DCF0 - the projector's extent is a cube of its radius, which
        // is read back from the server rather than from m_props.
        if (m_srvId == -1)
        {
            m_ownBoundingBox.m_box[0] = 0.0f;
            m_ownBoundingBox.m_box[1] = 0.0f;
            m_ownBoundingBox.m_box[2] = 0.0f;
            m_ownBoundingBox.m_box[3] = 0.0f;
            m_ownBoundingBox.m_box[4] = 0.0f;
            m_ownBoundingBox.m_box[5] = 0.0f;
            return;
        }

        // The 10 stands if the server has nothing to say.
        float r = 10.0f;
        GetServer()->GetItemProperty(m_srvId, PROP_RADIUS, &r);

        m_ownBoundingBox.m_box[0] = -r;
        m_ownBoundingBox.m_box[1] = -r;
        m_ownBoundingBox.m_box[2] = -r;
        m_ownBoundingBox.m_box[3] = r;
        m_ownBoundingBox.m_box[4] = r;
        m_ownBoundingBox.m_box[5] = r;
    }

    SgProjectorNode::SgProjectorNode(SgProjectorNode const& node) : SgNode(node)
    {
        // RVA 0x79DE80
        m_props[0] = node.m_props[0];
        m_props[1] = node.m_props[1];
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }

    SgProjectorNode::SgProjectorNode()
    {
        *(float*)&this->m_props[1] = 10.0;
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }
}

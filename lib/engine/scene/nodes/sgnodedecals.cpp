#include <scene/nodes/sgnodedecals.h>
#include <scene/servers/dataserver.h>
#include <m3dapp.h>

namespace m3d
{
    namespace
    {
        // Everything this node sends the decals server is the payload plus the
        // node it belongs to, so the server can reach the node's DecalsList.
        struct DecalNodeParams
        {
            /* 0x0000 */ void* m_data;
            /* 0x0004 */ SgNode* m_node;
        };

        int const PROP_SERVER_ID = 4360;
        int const PROP_DECALS_LIST = 10496;
        int const PROP_ADD_DECAL = 10497;
        int const PROP_RECALC_DECALS = 10498;
    }  // namespace

    RT_CLASS_EXPORTS_BEGIN(SgDecalsNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgDecalsNode);

    Class* SgDecalsNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgDecalsNode::CreateObject()
    {
        return new SgDecalsNode();
    }

    int SgDecalsNode::GetProperty(unsigned propId, void* property) const
    {
        // RVA 0x730F80
        if (m3d::SgNode::GetProperty(propId, property))
            return 1;
        if (propId == PROP_SERVER_ID)
        {
            *(int*)property = this->m_srvId;
            return 1;
        }
        if (propId == PROP_DECALS_LIST)
        {
            *(DecalsList const**)property = &this->m_decals;
            return 1;
        }
        return 0;
    }

    unsigned SgDecalsNode::GetNumDecals()
    {
        // RVA 0x6CFF40
        return m_decals.GetNumDecals();
    }

    int SgDecalsNode::SetProperty(unsigned propId, void* property)
    {
        // RVA 0x730EF0
        if (m3d::SgNode::SetProperty(propId, property))
            return 1;

        if (propId == PROP_SERVER_ID)
        {
            this->m_srvId = *(int*)property;
            return 1;
        }

        if (propId == PROP_ADD_DECAL || propId == PROP_RECALC_DECALS)
        {
            // The server needs the node as well as the payload, since the decals
            // themselves live on the node rather than in the server's item.
            DecalNodeParams params;
            params.m_data = property;
            params.m_node = this;
            GetServer()->SetItemProperty(m_srvId, propId, &params);
        }

        // NOTE: both decal properties report failure even though they were
        // handled; only the server id returns 1.
        return 0;
    }

    DecalInfo const& SgDecalsNode::GetDecal(int i)
    {
        // RVA 0x6D3AC0
        return m_decals.GetDecal(i);
    }

    DataServer* SgDecalsNode::GetServer() const
    {
        return &M3D_APP->GetDecalsServer();
    }

    Object* SgDecalsNode::Clone()
    {
        return new SgDecalsNode(*this);
    }

    int SgDecalsNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        // RVA 0x730EA0 - a node with no server entry has no decal texture, so
        // there is nothing to draw.
        if (m_srvId == -1)
        {
            return 0;
        }

        DecalNodeParams ri;
        ri.m_data = &m_currentXForm;
        ri.m_node = this;

        GetServer()->RenderItem(m_srvId, &ri);
        return 1;
    }

    int SgDecalsNode::GetPropertiesList(retruxx::set<unsigned>& props) const
    {
        // RVA 0x731050 - NOTE: only the server id is advertised; the decals
        // list property is readable but never listed.
        if (!SgNode::GetPropertiesList(props))
        {
            return 0;
        }
        props.insert(PROP_SERVER_ID);
        return 1;
    }

    Class* SgDecalsNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgDecalsNode);
    }

    SgDecalsNode::~SgDecalsNode()
    {
        // RVA 0x7311E0 - unregisters from the decals server and the think list;
        // the decal list itself is destroyed by the member's own destructor.
        RitualInDestructor();
    }

    SgDecalsNode::SgDecalsNode(SgDecalsNode const& node) : SgNode(node)
    {
        // The decal list is not copied: the clone starts out empty.
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }

    SgDecalsNode::SgDecalsNode()
    {
        RitualInConstructor(RITUAL_REGISTERED_NODE);
    }

    void SgDecalsNode::UpdateOwnBoundingBox()
    {
        float v1 = 0;
        if (this->m_srvId == -1)
        {
            this->m_ownBoundingBox.m_box[0] = 0.0;
            this->m_ownBoundingBox.m_box[1] = 0.0;
            this->m_ownBoundingBox.m_box[2] = 0.0;
        }
        else
        {
            this->m_ownBoundingBox.m_box[0] = -20.0;
            this->m_ownBoundingBox.m_box[1] = -20.0;
            this->m_ownBoundingBox.m_box[2] = -20.0;
            v1 = 20.0;
        }
        this->m_ownBoundingBox.m_box[3] = v1;
        this->m_ownBoundingBox.m_box[4] = v1;
        this->m_ownBoundingBox.m_box[5] = v1;
    }
}  // namespace m3d

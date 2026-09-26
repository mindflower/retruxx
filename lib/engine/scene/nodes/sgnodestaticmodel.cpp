#include <stdexcept>
#include <m3dapp.h>
#include <core/kernel.h>
#include <core/log.h>
#include <scene/scenegraph.h>
#include <scene/nodes/sgnodestaticmodel.h>
#include <scene/servers/dataserver.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgStaticModelNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgStaticModelNode);

    namespace
    {
        // What the node hands to StaticModelsServer::RenderItem (SgStaticModelNode::Render::renderInfo in the PDB).
        struct renderInfo
        {
            /* 0x0000 */ CMatrix* m_localXForm;
            /* 0x0004 */ float m_alpha;
            /* 0x0008 */ rend::Cull m_culling;
        }; /* size: 0x000c */
    }  // namespace

    Object* SgStaticModelNode::CreateObject()
    {
        // RVA 0x79F4E0
        return new SgStaticModelNode();
    }

    Class* SgStaticModelNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Class* SgStaticModelNode::GetClass() const
    {
        // RVA 0x79F370
        return RT_CLASS_LOCAL(SgStaticModelNode);
    }

    DataServer* SgStaticModelNode::GetServer() const
    {
        // RVA 0x79F4D0
        return &M3D_APP->GetStaticModelsServer();
    }

    int SgStaticModelNode::GetPropertiesList(retruxx::set<unsigned>& props) const
    {
        // RVA 0x79F690
        if (!SgNode::GetPropertiesList(props))
        {
            return 0;
        }
        props.insert(PROP_NODE_HANDLE);
        return 1;
    }

    int SgStaticModelNode::Render(SgNodeRenderFlags flags, void* data, int, int)
    {
        // RVA 0x79F400
        if (m_srvId == -1)
        {
            return 0;
        }
        if ((flags & 4) == 0)
        {
            GetGraph()->LightSetupLightsForNode(this);
        }
        CMatrix temp = MatrixFromFlags(flags, data);
        renderInfo ri;
        ri.m_alpha = GetGraph()->GetAlphaForNode(this);
        ri.m_localXForm = &temp;
        ri.m_culling = rend::M3DCULL_CCW;
        GetServer()->RenderItem(m_srvId, &ri);
        return 1;
    }

    Object* SgStaticModelNode::Clone()
    {
        return new SgStaticModelNode(*this);
    }

    int SgStaticModelNode::GetProperty(unsigned propId, void* property) const
    {
        // RVA 0x79F590
        if (SgNode::GetProperty(propId, property))
        {
            return 1;
        }
        if (propId == PROP_NODE_HANDLE)
        {
            *static_cast<int*>(property) = m_srvId;
            return 1;
        }
        if (propId == PROP_SRV_SNAPSHOT)
        {
            if (m_srvId == -1)
            {
                M3D_LOG_INFO(CStr("Error in snapshot, no model found:") + m_name);
                return 1;
            }
            GetServer()->GetItemProperty(m_srvId, PROP_SRV_SNAPSHOT, property);
            return 1;
        }
        return 0;
    }

    int SgStaticModelNode::SetProperty(unsigned propId, void* property)
    {
        // RVA 0x79F3C0
        if (SgNode::SetProperty(propId, property))
        {
            return 1;
        }
        if (propId == PROP_NODE_HANDLE)
        {
            m_srvId = *static_cast<int*>(property);
            return 1;
        }
        return 0;
    }

    SgStaticModelNode::SgStaticModelNode()
    {
        // RVA 0x79F3A0
    }

    SgStaticModelNode::SgStaticModelNode(SgStaticModelNode const& node) : SgNode(node)
    {
    }

    SgStaticModelNode::~SgStaticModelNode()
    {
        // RVA 0x79F6D0
        RitualInDestructor();
    }

    void SgStaticModelNode::UpdateOwnBoundingBox()
    {
        // RVA 0x79F510
        if (m_srvId == -1)
        {
            for (float& v : m_ownBoundingBox.m_box)
            {
                v = 0.0f;
            }
            return;
        }
        PropSrvBoundingBox prop;
        prop.m_node = this;
        prop.m_destBox = &m_ownBoundingBox;
        GetServer()->GetItemProperty(m_srvId, PROP_SRV_BOUNDING_BOX, &prop);
    }
}

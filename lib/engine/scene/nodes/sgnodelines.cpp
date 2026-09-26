#include <stdexcept>
#include <m3dapp.h>
#include <scene/nodes/sgnodelines.h>
#include <scene/servers/dataserver.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgLinesNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgLinesNode);

    namespace
    {
        // What the node hands to LinesServer::RenderItem (SgLinesNode::Render::RenderInfo in the PDB).
        struct RenderInfo
        {
            /* 0x0000 */ int rad;
            /* 0x0004 */ int color;
            /* 0x0008 */ CMatrix matrix;
        }; /* size: 0x0048 */
    }  // namespace

    Class* SgLinesNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgLinesNode::CreateObject()
    {
        // RVA 0x79E960
        return new SgLinesNode();
    }

    int SgLinesNode::Render(SgNodeRenderFlags flags, void* data, int, int)
    {
        // RVA 0x79EA70
        if (m_srvId == -1)
        {
            return 0;
        }

        RenderInfo ri;
        GetProperty(PROP_LINES_RAD, &ri.rad);
        GetProperty(PROP_LINES_COLOR, &ri.color);
        ri.matrix = MatrixFromFlags(flags, data);
        GetServer()->RenderItem(m_srvId, &ri);
        return 1;
    }

    int SgLinesNode::SetProperty(unsigned propId, void* property)
    {
        // RVA 0x79E840
        if (SgNode::SetProperty(propId, property))
        {
            return 1;
        }
        if (propId == PROP_NODE_HANDLE)
        {
            m_srvId = *static_cast<int*>(property);
            return 1;
        }
        // NOTE: the line properties are stored but reported as not handled.
        unsigned const index = propId - PROP_LINES_FIRST;
        if (static_cast<int>(index) >= 0 && index < 2)
        {
            m_props[index] = *static_cast<unsigned*>(property);
        }
        return 0;
    }

    DataServer* SgLinesNode::GetServer() const
    {
        // RVA 0x79E950
        return &M3D_APP->GetLinesServer();
    }

    int SgLinesNode::GetProperty(unsigned propId, void* property) const
    {
        // RVA 0x79E8A0
        if (SgNode::GetProperty(propId, property))
        {
            return 1;
        }
        if (propId == PROP_NODE_HANDLE)
        {
            *static_cast<int*>(property) = m_srvId;
            return 1;
        }
        // NOTE: the line properties are read but reported as not handled.
        unsigned const index = propId - PROP_LINES_FIRST;
        if (static_cast<int>(index) >= 0 && index < 2)
        {
            *static_cast<unsigned*>(property) = m_props[index];
        }
        return 0;
    }

    int SgLinesNode::GetPropertiesList(retruxx::set<unsigned>& props) const
    {
        // RVA 0x79EB10
        if (!SgNode::GetPropertiesList(props))
        {
            return 0;
        }
        props.insert(PROP_LINES_COLOR);
        props.insert(PROP_LINES_RAD);
        return 1;
    }

    Class* SgLinesNode::GetClass() const
    {
        // RVA 0x79E7B0
        return RT_CLASS_LOCAL(SgLinesNode);
    }

    Object* SgLinesNode::Clone()
    {
        return new SgLinesNode(*this);
    }

    SgLinesNode::~SgLinesNode()
    {
        // RVA 0x79E830
    }

    SgLinesNode::SgLinesNode(SgLinesNode const& node) : SgNode(node)
    {
        m_props[0] = node.m_props[0];
        m_props[1] = node.m_props[1];
    }

    SgLinesNode::SgLinesNode()
    {
        // RVA 0x79E800
        m_props[1] = 5;
        m_props[0] = -1;
    }

    void SgLinesNode::UpdateOwnBoundingBox()
    {
        // RVA 0x79E9F0 - a square of the line radius around the origin, one unit up and down.
        int rad = 0;
        GetProperty(PROP_LINES_RAD, &rad);
        float const r = static_cast<float>(rad);
        m_ownBoundingBox.m_box[1] = 0.0f - r;
        m_ownBoundingBox.m_box[2] = -1.0f;
        m_ownBoundingBox.m_box[0] = 0.0f - r;
        m_ownBoundingBox.m_box[3] = r;
        m_ownBoundingBox.m_box[4] = r;
        m_ownBoundingBox.m_box[5] = 1.0f;
    }
}

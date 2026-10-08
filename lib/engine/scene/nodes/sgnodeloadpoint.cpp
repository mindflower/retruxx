#include <stdexcept>
#include <math/quaternion.h>
#include <scene/nodes/sgnodeloadpoint.h>
#include <scene/servers/dataserver.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgLoadpointNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgLoadpointNode);

    Class* SgLoadpointNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Object* SgLoadpointNode::CreateObject()
    {
        // RVA 0x79E6A0
        return new SgLoadpointNode();
    }

    Object* SgLoadpointNode::Clone()
    {
        return new SgLoadpointNode(*this);
    }

    int SgLoadpointNode::Think(int, int)
    {
        // RVA 0x79E510 - follows the loadpoint of the parent's model and thinks again when the parent does.
        if (m_srvId == -1)
        {
            return 0;
        }

        SgNode* const parent = static_cast<SgNode*>(GetParent());
        PropSrvLoadpoint ri;
        ri.m_fromLoadpoint = m_srvId;
        ri.m_node = this;
        ri.m_newOrg = CVector(0.0f, 0.0f, 0.0f);
        ri.m_newRot = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        ri.m_newScale = CVector(1.0f, 1.0f, 1.0f);
        parent->GetServerItemProperty(PROP_SRV_LOADPOINT, &ri);
        SetOriginAbs(ri.m_newOrg);
        SetRotation(ri.m_newRot);
        SetScale(ri.m_newScale);
        m_nextThinkTime = parent->m_nextThinkTime;
        m_isOwnBoundingBoxDirty = true;
        return 1;
    }

    Class* SgLoadpointNode::GetClass() const
    {
        // RVA 0x79E4F0
        return RT_CLASS_LOCAL(SgLoadpointNode);
    }

    SgLoadpointNode::~SgLoadpointNode()
    {
        // RVA 0x79E710
        RitualInDestructor();
    }

    SgLoadpointNode::SgLoadpointNode(SgLoadpointNode const& node) : SgNode(node)
    {
        RitualInConstructor(RITUAL_THINK_NODE);
        m_persistant = false;
    }

    SgLoadpointNode::SgLoadpointNode()
    {
        // RVA 0x79E620
        RitualInConstructor(RITUAL_THINK_NODE);
        m_persistant = false;
    }
}

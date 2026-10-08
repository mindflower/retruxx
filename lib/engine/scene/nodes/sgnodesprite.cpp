#include <stdexcept>
#include <cmath>
#include <m3dapp.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <math/matrix.h>
#include <math/quaternion.h>
#include <scene/nodes/sgnodesprite.h>
#include <scene/servers/dataserver.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgSpriteNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgSpriteNode);

    namespace
    {
        // What the node hands to SpritesServer::RenderItem.
        struct RenderInfo
        {
            /* 0x0000 */ CVector* m_local;
            /* 0x0004 */ float m_scaleX;
            /* 0x0008 */ float m_scaleY;
            /* 0x000c */ float m_rotZ;
            /* 0x0010 */ unsigned int m_color;
            /* 0x0014 */ unsigned int m_mode;
            /* 0x0018 */ float m_tsc;
        }; /* size: 0x001c */

        float const land_scale = 8.0f;
    }  // namespace

    Object* SgSpriteNode::CreateObject()
    {
        // RVA 0x79EDA0
        return new SgSpriteNode();
    }

    Class* SgSpriteNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    Class* SgSpriteNode::GetClass() const
    {
        // RVA 0x79EB80
        return RT_CLASS_LOCAL(SgSpriteNode);
    }

    int SgSpriteNode::GetPropertiesList(retruxx::set<unsigned>& props) const
    {
        // RVA 0x79F190 - NOTE: PROP_SPR_ANGULAR_SPEED is readable but not listed.
        if (!SgNode::GetPropertiesList(props))
        {
            return 0;
        }
        props.insert(PROP_NODE_HANDLE);
        props.insert(PROP_SPR_LOOPED);
        props.insert(PROP_SPR_MODE);
        props.insert(PROP_SPR_COLOR);
        props.insert(PROP_SPR_TIME0);
        props.insert(PROP_SPR_ANIM_LEN);
        return 1;
    }

    int SgSpriteNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        // RVA 0x79EDF0 - the sprite is scaled by the inverse of the node's X and Z axis lengths (in landscape cells),
        // spun either at a constant angular speed or by the node's own roll, and animated from the time it started.
        if (m_srvId == -1)
        {
            return 0;
        }

        RenderInfo ri;
        ri.m_color = m_props[1];
        ri.m_local = &m_currentWorldOrigin;
        ri.m_mode = m_props[0];
        CMatrix const& m = m_currentXForm;
        ri.m_scaleX = 1.0f / (std::sqrt(m._31 * m._31 + m._21 * m._21 + m._11 * m._11) * land_scale);
        ri.m_scaleY = 1.0f / (std::sqrt(m._33 * m._33 + m._23 * m._23 + m._13 * m._13) * land_scale);

        unsigned int const frameStartTime = M3D_KERNEL->GetTimer().GetFrameStartTime();
        unsigned int const elapsed = frameStartTime - m_props[2];
        float const curFrame = static_cast<float>(static_cast<double>(elapsed) / static_cast<double>(m_props[3]));

        int const angularSpeed = static_cast<int>(m_props[4]);
        if (angularSpeed)
        {
            ri.m_rotZ = static_cast<float>(static_cast<int>(frameStartTime) % angularSpeed) /
                static_cast<float>(angularSpeed) * 6.2831855f;
        }
        else
        {
            float y;
            float p;
            float r;
            m_rotation.ToMatrix().getYPR(y, p, r);
            ri.m_rotZ = r;
        }

        // An unlooped sprite stops on its last frame.
        ri.m_tsc = (!m_props[5] && curFrame > 1.0f) ? 1.0f : curFrame;
        GetServer()->RenderItem(m_srvId, &ri);
        return 1;
    }

    DataServer* SgSpriteNode::GetServer() const
    {
        // RVA 0x79ED90
        return &M3D_APP->GetSpritesServer();
    }

    Object* SgSpriteNode::Clone()
    {
        return new SgSpriteNode(*this);
    }

    int SgSpriteNode::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        // RVA 0x667920
        if (!SgNode::ReadFromXmlNode(file, node))
        {
            return 0;
        }

        char const* value;
        if (!node->IsEmpty() && (value = node->GetAttribute("sprColor")) != nullptr)
        {
            m_props[1] = atoi(value);
        }
        if (!node->IsEmpty() && (value = node->GetAttribute("sprAngularSpeed")) != nullptr)
        {
            m_props[4] = atoi(value);
        }
        if (!node->IsEmpty() && (value = node->GetAttribute("sprIfps")) != nullptr)
        {
            m_props[3] = atoi(value);
        }
        if (!node->IsEmpty() && (value = node->GetAttribute("sprTimeStarted")) != nullptr)
        {
            m_props[2] = atoi(value);
        }
        if (!node->IsEmpty() && (value = node->GetAttribute("sprMode")) != nullptr)
        {
            m_props[0] = atoi(value);
        }
        if (!node->IsEmpty() && (value = node->GetAttribute("sprLooped")) != nullptr)
        {
            m_props[5] = atoi(value);
        }
        return 1;
    }

    int SgSpriteNode::SetProperty(unsigned propId, void* property)
    {
        // RVA 0x79EC40
        if (SgNode::SetProperty(propId, property))
        {
            return 1;
        }
        if (propId == PROP_NODE_HANDLE)
        {
            m_srvId = *static_cast<int*>(property);
            return 1;
        }
        // NOTE: the sprite properties are stored but reported as not handled.
        unsigned const index = propId - PROP_SPR_FIRST;
        if (static_cast<int>(index) >= 0 && index < 6)
        {
            m_props[index] = *static_cast<unsigned*>(property);
        }
        return 0;
    }

    int SgSpriteNode::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
    {
        // RVA 0x667230
        if (!SgNode::WriteToXmlNode(file, writeTo))
        {
            return 0;
        }
        writeTo->SetAttribute("sprColor", CStr(m_props[1]).c_str());
        writeTo->SetAttribute("sprAngularSpeed", CStr(m_props[4]).c_str());
        writeTo->SetAttribute("sprIfps", CStr(m_props[3]).c_str());
        writeTo->SetAttribute("sprTimeStarted", CStr(m_props[2]).c_str());
        writeTo->SetAttribute("sprMode", CStr(m_props[0]).c_str());
        writeTo->SetAttribute("sprLooped", CStr(m_props[5]).c_str());
        return 1;
    }

    int SgSpriteNode::GetProperty(unsigned propId, void* property) const
    {
        // RVA 0x79ECA0
        if (SgNode::GetProperty(propId, property))
        {
            return 1;
        }
        if (propId == PROP_NODE_HANDLE)
        {
            *static_cast<int*>(property) = m_srvId;
            return 1;
        }
        // NOTE: the sprite properties are read but reported as not handled.
        unsigned const index = propId - PROP_SPR_FIRST;
        if (static_cast<int>(index) >= 0 && index < 6)
        {
            *static_cast<unsigned*>(property) = m_props[index];
        }
        return 0;
    }

    SgSpriteNode::~SgSpriteNode()
    {
        // RVA 0x79EC30
    }

    void SgSpriteNode::UpdateOwnBoundingBox()
    {
        // RVA 0x79F130
        m_ownBoundingBox.m_box[0] = -5.0f;
        m_ownBoundingBox.m_box[1] = -1.0f;
        m_ownBoundingBox.m_box[2] = -5.0f;
        m_ownBoundingBox.m_box[3] = 5.0f;
        m_ownBoundingBox.m_box[4] = 1.0f;
        m_ownBoundingBox.m_box[5] = 5.0f;
    }

    SgSpriteNode::SgSpriteNode()
    {
        // RVA 0x79EBE0
        m_props[4] = 0;
        m_props[0] = 0;
        m_props[2] = 0;
        m_props[3] = 1500;
        m_props[1] = -1;
        m_props[5] = 1;
    }

    SgSpriteNode::SgSpriteNode(SgSpriteNode const& node) : SgNode(node)
    {
        for (int i = 0; i < 6; ++i)
        {
            m_props[i] = node.m_props[i];
        }
    }
}

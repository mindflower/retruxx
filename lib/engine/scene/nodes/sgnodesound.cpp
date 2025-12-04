#include <stdexcept>
#include <scene/nodes/sgnodesound.h>
#include <m3dapp.h>
#include <core/ini.h>
#include "scene/servers/dataserver.h"
#include <core/kernel.h>
#include <config.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgSoundSourceNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgSoundSourceNode);

    Object* SgSoundSourceNode::CreateObject()
    {
        return new SgSoundSourceNode;
    }

    Class* SgSoundSourceNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    void SgSoundSourceNode::CanBeFree()
    {
        if (M3D_ENGINE_CFG.m_snd_Enable.GetB())
        {
            int channelId = -1;

            int looped = 0;
            GetProperty(PROP_SND_LOOPED, &looped);

            int bIsTripleSound = 0;
            GetServer()->GetItemProperty(m_srvId, PROP_SRV_SND_TRIPLE, &bIsTripleSound);

            int bIsDoubleSound = 0;
            GetServer()->GetItemProperty(m_srvId, PROP_SRV_SND_DOUBLE, &bIsTripleSound);

            // TODO: check all this!!!
            if (bIsTripleSound && m_currentSoundNum < 2)
            {
                m_currentSoundNum = 2;
                Restart();

                GetProperty(PROP_SND_CHANNELID, &channelId);

                looped = 0;
                SetProperty(PROP_SND_LOOPED, &looped);
            }
            else
            {
                if (bIsDoubleSound && m_currentSoundNum < 1)
                {
                    m_currentSoundNum = 1;
                    Restart();
                    GetProperty(PROP_SND_CHANNELID, &channelId);
                }
                else
                {
                    GetProperty(PROP_SND_CHANNELID, &channelId);
                }

                int newLooped = 0;
                SetProperty(PROP_SND_LOOPED, &newLooped);
            }
            if (channelId != -1)
            {
                M3D_APP->m_sound->SetChannelLoopMode(channelId, 0);
            }
        }
    }

    int SgSoundSourceNode::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        auto result = SgNode::ReadFromXmlNode(file, node);
        if (result)
        {
            int looped = 1;
            m3d::SafeIntAttrib(looped, node, "looped");
            this->SetProperty(9728u, &looped);
            return 1;
        }
        return result;
    }

    DataServer* SgSoundSourceNode::GetServer() const
    {
        return &m3d::Application::g_pApp->GetSoundServer();
    }

    int SgSoundSourceNode::GetPropertiesList(retruxx::set<unsigned>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Class* SgSoundSourceNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgSoundSourceNode);
    }

    int SgSoundSourceNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float SgSoundSourceNode::IntersectRay(CVector const&, CVector const&, SgNode*&, Class*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgSoundSourceNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        return _InternalRender();
    }

    Object* SgSoundSourceNode::Clone()
    {
        return new SgSoundSourceNode(*this);
    }

    int SgSoundSourceNode::GetProperty(unsigned propId, void* property) const
    {
        if (SgNode::GetProperty(propId, property))
            return 1;

        if (propId == 4360)
        {
            *(int*)property = this->m_srvId;
            return 1;
        }
        int v5 = propId - 9728;
        if ((int)(propId - 9728) >= 0 && v5 < 6)
            *(int*)property = this->m_props[v5];

        return 0;
    }

    int SgSoundSourceNode::SetProperty(unsigned propId, void* property)
    {
        if (SgNode::SetProperty(propId, property))
            return 1;
        if (propId == 4360)
        {
            GetServer()->UnregisterNode(this);
            m_srvId = *(int*)property;
            GetServer()->RegisterNode(this);
            return 1;
        }

        auto v8 = propId - 9728;
        if ((int)(propId - 9728) >= 0 && v8 < 6)
            this->m_props[v8] = *(int*)property;
        if (propId != 9732)
        {
            if (propId != 9733)
                return 0;
            _InternalRender();
            return 1;
        }

        if (M3D_KERNEL->GetEngineCfg().m_snd_Enable.GetB())
            m3d::Application::g_pApp->m_sound->SetChannelFrequency(this->m_props[1], (int)(float)((float)(int)this->m_props[3] * *(float*)property));
        return 1;
    }

    void SgSoundSourceNode::Restart()
    {
        int soundEnabled = 0;
        SetProperty(PROP_SND_SOUND_ENABLED, &soundEnabled);

        soundEnabled = 1;
        SetProperty(PROP_SND_SOUND_ENABLED, &soundEnabled);
    }

    bool SgSoundSourceNode::IsFree() const
    {
        if (!M3D_ENGINE_CFG.m_snd_Enable.GetB())
        {
            return true;
        }
        if (++m_framesPassed >= 2)
        {
            int channelId = -1;
            GetProperty(PROP_SND_CHANNELID, &channelId);
            auto isPlay = M3D_APP->m_sound->IsChannelPlaying(channelId);
            return channelId == -1 || !isPlay;
        }
        return false;
    }

    SgSoundSourceNode::~SgSoundSourceNode()
    {
        RitualInDestructor();
    }

    SgSoundSourceNode::SgSoundSourceNode()
    {
        this->m_currentSoundNum = 0;
        this->m_framesPassed = 0;
        RitualInConstructor(RITUAL_REGISTERED_NODE);
        this->m_props[2] = 0;
        this->m_props[3] = 0;
        this->m_props[0] = 1;
        this->m_props[4] = 1;
        this->m_props[5] = 1;
        this->m_props[1] = -1;
    }

    SgSoundSourceNode::SgSoundSourceNode(SgSoundSourceNode const& node) : SgNode(node)
    {
        this->m_currentSoundNum = 0;
        this->m_framesPassed = 0;

        RitualInConstructor(RITUAL_REGISTERED_NODE);
        this->m_props[0] = node.m_props[0];
        this->m_props[1] = node.m_props[1];
        this->m_props[2] = node.m_props[2];
        this->m_props[3] = node.m_props[3];
        this->m_props[4] = node.m_props[4];
        this->m_props[5] = node.m_props[5];
    }

    void SgSoundSourceNode::UpdateOwnBoundingBox()
    {
        this->m_ownBoundingBox.m_box[0] = -200.0;
        this->m_ownBoundingBox.m_box[1] = -200.0;
        this->m_ownBoundingBox.m_box[2] = -200.0;
        this->m_ownBoundingBox.m_box[3] = 200.0;
        this->m_ownBoundingBox.m_box[4] = 200.0;
        this->m_ownBoundingBox.m_box[5] = 200.0;
    }

    bool SgSoundSourceNode::_OnSoundStopped()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgSoundSourceNode::_InternalRender()
    {
        if (m_srvId == -1)
        {
            return 0;
        }

        struct RenderInfo
        {
            /* 0x0000 */ m3d::SgNode* m_node;
            /* 0x0004 */ int m_currentSoundNum;
        }; /* size: 0x0008 */

        RenderInfo ri;
        ri.m_node = this;
        ri.m_currentSoundNum = m_currentSoundNum;

        GetServer()->RenderItem(m_srvId, &ri);
        IsFree();
        return 1;
    }
}

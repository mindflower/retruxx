#include <stdexcept>
#include <scene/nodes/sgnodeparticles.h>
#include <m3dapp.h>
#include "scene/servers/dataserver.h"
#include <scene/servers/serverparticles.h>
#include "core/ini.h"
#include <core/kernel.h>
#include <core/timer.h>

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(SgParticlesNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgParticlesNode);

    RT_CLASS_EXPORTS_BEGIN(SgParticlesOpaqueNode)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(SgParticlesOpaqueNode);

    Object* SgParticlesNode::CreateObject()
    {
        return new SgParticlesNode;
    }

    Class* SgParticlesNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    int SgParticlesNode::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgParticlesNode::Think(int dt, int curTime)
    {
        if (this->m_srvId == -1)
            return 0;
        if (this->m_frameVisible != M3D_KERNEL->GetTimer().GetCurFrame() - 1
            && (!this->m_isRemoveIfFree || curTime - this->m_lastTimeUpdated < 500))
        {
            return 0;
        }

        int v6 = 0;
        if (this->m_isRemoveIfFree && (m_lastTimeUpdated = this->m_lastTimeUpdated, m_lastTimeUpdated >= 0))
            v6 = curTime - m_lastTimeUpdated;
        else
            v6 = dt;

        struct
        {
            m3d::SgNode* m_node;
            unsigned int m_dt;
        } ri;

        ri.m_dt = v6;
        ri.m_node = this;
        GetServer()->UpdateItem(this->m_srvId, &ri);
        this->m_lastTimeUpdated = curTime;
        return 1;
    }

    Object* SgParticlesNode::Clone()
    {
        return new SgParticlesNode(*this);
    }

    int SgParticlesNode::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        int result = m3d::SgNode::ReadFromXmlNode(file, node);
        if (result)
        {
            this->m_numMesh = -1;
            m3d::SafeIntAttrib(m_numMesh, node, "psNumEmitterMesh");
            return 1;
        }
        return result;
    }

    Class* SgParticlesNode::GetClass() const
    {
        return &m_classSgParticlesNode;
    }

    bool SgParticlesNode::IsFree() const
    {
        PsInfoForNode* info = nullptr;
        GetProperty(1u, &info);
        if (!info)
        {
            return true;
        }

        if (!info->m_list->m_numParticles)
        {
            ++this->m_Parts0Times;
        }
        return this->m_Parts0Times > 2;
    }

    int SgParticlesNode::GetProperty(unsigned propId, void* property) const
    {
        if (SgNode::GetProperty(propId, property))
            return 1;
        if (propId == 4360)
        {
            *(int*)property = this->m_srvId;
            return 1;
        }
        if (propId == 9472)
        {
            *(int*)property = this->m_numMesh;
            return 1;
        }
        return 0;
    }

    int SgParticlesNode::GetPropertiesList(retruxx::set<unsigned, retruxx::less<unsigned>>&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SgParticlesNode::SetProperty(unsigned propId, void* property)
    {
        if (SgNode::SetProperty(propId, property))
            return 1;

        if (propId == 4360)
        {
            this->m_srvId = *static_cast<int*>(property);
            GetServer()->UnregisterNode(this);
            GetServer()->RegisterNode(this);
            return 1;
        }
        if (propId == 9472)
        {
            this->m_numMesh = *static_cast<int*>(property);
            GetServer()->UnregisterNode(this);
            GetServer()->RegisterNode(this);
            return 1;
        }
        if (propId == 9473)
        {
            auto v7 = dynamic_cast<m3d::ParticlesServer*>(this->GetServer());
            v7->AddParticle(this, static_cast<CVector*>(property));
            return 0;
        }
        else if (propId == 9475)
        {
            auto v8 = dynamic_cast<m3d::ParticlesServer*>(this->GetServer());
            v8->AddParticles(this, static_cast<const retruxx::vector<CVector>*>(property));
            return 0;
        }
        else
        {
            if (propId == 9474)
            {
                auto v9 = dynamic_cast<m3d::ParticlesServer*>(this->GetServer());
                v9->MoveParticles(this, static_cast<const retruxx::vector<CVector>*>(property));
            }
            return 0;
        }
    }

    void SgParticlesNode::Restart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DataServer* SgParticlesNode::GetServer() const
    {
        return &M3D_APP->GetParticlesServer();
    }

    int SgParticlesNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SgParticlesNode::CanBeFree()
    {
        PsInfoForNode* info = nullptr;
        GetProperty(1u, &info);
        if (info)
        {
            info->m_list->m_maxParticles = 0;
        }
    }

    SgParticlesNode::~SgParticlesNode()
    {
        RitualInDestructor();
    }

    SgParticlesNode::SgParticlesNode(SgParticlesNode const& node) : SgNode(node)
    {
        this->m_numMesh = -1;
        this->m_Parts0Times = 0;
        this->m_lastTimeUpdated = -1000;
        RitualInConstructor(RITUAL_THINK_AND_REGISTERED_NODE);
    }

    SgParticlesNode::SgParticlesNode()
    {
        this->m_numMesh = -1;
        this->m_Parts0Times = 0;
        this->m_lastTimeUpdated = -1000;
        RitualInConstructor(RITUAL_THINK_AND_REGISTERED_NODE);
    }

    void SgParticlesNode::UpdateOwnBoundingBox()
    {
        if (this->m_srvId == -1)
        {
            this->m_ownBoundingBox.m_box[0] = 0.0;
            this->m_ownBoundingBox.m_box[1] = 0.0;
            this->m_ownBoundingBox.m_box[2] = 0.0;
            this->m_ownBoundingBox.m_box[3] = 0.0;
            this->m_ownBoundingBox.m_box[4] = 0.0;
            this->m_ownBoundingBox.m_box[5] = 0.0;
        }
        else
        {
            m3d::PropSrvBoundingBox ri;
            ri.m_node = this;
            ri.m_destBox = &this->m_ownBoundingBox;
            GetServer()->GetItemProperty(this->m_srvId, 12288, &ri);
        }
    }

    Class* SgParticlesOpaqueNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgParticlesNode);
    }

    Object* SgParticlesOpaqueNode::CreateObject()
    {
        return new SgParticlesOpaqueNode;
    }

    SgParticlesOpaqueNode::~SgParticlesOpaqueNode() = default;

    Class* SgParticlesOpaqueNode::GetClass() const
    {
        return RT_CLASS_LOCAL(SgParticlesOpaqueNode);
    }

    Object* SgParticlesOpaqueNode::Clone()
    {
        return new SgParticlesOpaqueNode(*this);
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode(SgParticlesOpaqueNode const& node) : SgParticlesNode(node)
    {
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode()
    {
    }
}

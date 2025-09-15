#include <stdexcept>
#include <scene/nodes/sgnodeparticles.h>
#include <m3dapp.h>
#include "scene/servers/dataserver.h"
#include <scene/servers/serverparticles.h>

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
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::Think(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgParticlesNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SgParticlesNode::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgParticlesNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool SgParticlesNode::IsFree() const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    DataServer* SgParticlesNode::GetServer() const
    {
        return &M3D_APP->GetParticlesServer();
    }

    int SgParticlesNode::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SgParticlesNode::CanBeFree()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesNode::~SgParticlesNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesNode::SgParticlesNode(SgParticlesNode const&)
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgParticlesOpaqueNode::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgParticlesNode);
    }

    Object* SgParticlesOpaqueNode::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::~SgParticlesOpaqueNode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* SgParticlesOpaqueNode::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* SgParticlesOpaqueNode::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode(SgParticlesOpaqueNode const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SgParticlesOpaqueNode::SgParticlesOpaqueNode()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

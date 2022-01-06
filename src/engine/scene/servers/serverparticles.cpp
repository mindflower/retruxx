#include <stdexcept>
#include <scene/servers/serverparticles.h>

namespace m3d
{
    int ParticlesServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    ParticlesServer::~ParticlesServer()
    {
        throw std::logic_error("Not implemented");
    }

    int ParticlesServer::RenderNodeSet(SgNode**, unsigned, m3d::RenderNodeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::UpdateItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticlesServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    ParticlesServer::ParticlesServer()
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::MoveParticles(m3d::SgNode*, std::vector<CVector> const*)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticlesServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    int ParticlesServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::SaveAllLoadedEntitiesToBPS()
    {
        throw std::logic_error("Not implemented");
    }

    int ParticlesServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::ResetItem(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::UnregisterNode(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::AddParticle(m3d::SgNode*, CVector const*)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticlesServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int ParticlesServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::RegisterNode(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::AddParticles(m3d::SgNode*, std::vector<CVector> const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::AddItemsByOne(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }

    void ParticlesServer::AddItemsList(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }
}

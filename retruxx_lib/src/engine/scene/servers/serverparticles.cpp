#include <m3dapp.h>
#include <particles.h>
#include <stdexcept>
#include <scene/servers/serverparticles.h>

namespace m3d
{
    int ParticlesServer::SetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ParticlesServer::~ParticlesServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::RenderNodeSet(SgNode**, unsigned, m3d::RenderNodeInfo)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::UpdateItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    ParticlesServer::ParticlesServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("particles", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        id = Application::g_pApp->GetProfilerStack().AddProfiler("particles update", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profilerUpdate = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        QuadPS::CreateIb();
        SpritePS::CreateIb();
        GlowQuadPS::CreateIb();
        PolyPS::CreateIb();
        Poly1PS::CreateIb();
        RainPS::CreateIb();
        StripAllPS::CreateIb();
        StripOnePS::CreateIb();
    }

    void ParticlesServer::MoveParticles(m3d::SgNode*, retruxx::vector<CVector> const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::Init()
    {
        m_valid = true;
        return true;
    }

    int ParticlesServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::RenderItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::SaveAllLoadedEntitiesToBPS()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::ResetItem(m3d::SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::UnregisterNode(m3d::SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::AddParticle(m3d::SgNode*, CVector const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int ParticlesServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::RegisterNode(m3d::SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::AddParticles(m3d::SgNode*, retruxx::vector<CVector> const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::AddItemsByOne(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void ParticlesServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

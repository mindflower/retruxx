#include <m3dapp.h>
#include <scene/servers/serverstaticmodels.h>

namespace m3d
{
    int StaticModelsServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int StaticModelsServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticModelsServer::ReleaseBuffers()
    {
        throw std::logic_error("Not implemented");
    }

    void StaticModelsServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int StaticModelsServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int StaticModelsServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    int StaticModelsServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    StaticModelsServer::StaticModelsServer()
    {
        auto idx = Application::g_pApp->GetProfilerStack().AddProfiler("static", 0x1E);
        if (idx < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(idx);
        }
        else
        {
            m_profiler = 0;
        }
    }

    int StaticModelsServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    StaticModelsServer::~StaticModelsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int StaticModelsServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void StaticModelsServer::AddItemsList(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }

    int StaticModelsServer::PrepareToRender()
    {
        throw std::logic_error("Not implemented");
    }
}

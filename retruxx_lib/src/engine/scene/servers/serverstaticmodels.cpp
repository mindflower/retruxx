#include <m3dapp.h>
#include <scene/servers/serverstaticmodels.h>

namespace m3d
{
    int StaticModelsServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StaticModelsServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticModelsServer::ReleaseBuffers()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticModelsServer::RenderItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StaticModelsServer::SetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StaticModelsServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StaticModelsServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
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
        m_valid = true;
        return 1;
    }

    StaticModelsServer::~StaticModelsServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StaticModelsServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void StaticModelsServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int StaticModelsServer::PrepareToRender()
    {
        throw retruxx::logic_error("Not implemented");
    }
}

#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serversprites.h>

namespace m3d
{
    int SpritesServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    SpritesServer::~SpritesServer()
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void SpritesServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    SpritesServer::SpritesServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("sprites", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
    }

    int SpritesServer::Init()
    {
        m_valid = true;
        return 1;
    }

    void SpritesServer::AddItemsList(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }
}

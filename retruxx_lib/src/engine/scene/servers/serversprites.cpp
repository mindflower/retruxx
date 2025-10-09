#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serversprites.h>

namespace m3d
{
    int SpritesServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SpritesServer::SetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    SpritesServer::~SpritesServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SpritesServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void SpritesServer::RenderItem(int id, void*)
    {
        m_profiler->StartCountdown();

        if (id != -4)
        {
            throw retruxx::logic_error("Not implemented");
        }

        m_profiler->EndCountdown();
    }

    int SpritesServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SpritesServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int SpritesServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
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

    void SpritesServer::AddItemsList(retruxx::vector<m3d::DataServer::ServerItem>& itemsList)
    {
        // TODO: implememnt SpritesServer::AddItemsList
        return;
        for (int i = 0; i < itemsList.size(); ++i)
        {
            throw retruxx::logic_error("Not implemented");
        }
    }
}

#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serversprites.h>

namespace m3d
{
    int SpritesServer::GetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SpritesServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    SpritesServer::~SpritesServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SpritesServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void SpritesServer::RenderItem(int id, void*)
    {
        m_profiler->StartCountdown();

        if (id != -4)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        m_profiler->EndCountdown();
    }

    int SpritesServer::SaveAllLoadedEntities(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SpritesServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int SpritesServer::AddItem(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}

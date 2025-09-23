#include <decals.h>
#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serverdecals.h>

namespace m3d
{
    int DecalsServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DecalsServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DecalsServer::RenderItem(int, void*)
    {
        // TODO: implement DecalsServer::RenderItem
        //throw retruxx::logic_error("Not implemented");
    }

    int DecalsServer::Init()
    {
        m_valid = true;
        return 1;
    }

    int DecalsServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DecalsServer::~DecalsServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DecalsServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int DecalsServer::SetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DecalsServer::DecalsServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("decals", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
        DecalsList::LoadShaders();
    }

    int DecalsServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void DecalsServer::AddItemsList(retruxx::vector<ServerItem>&)
    {
        // TODO: implement DecalsServer::AddItemsList
        //throw retruxx::logic_error("Not implemented");
    }
}

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

    void DecalsServer::RenderItem(int id, void* params)
    {
        m_profiler->StartCountdown();
        if (id == -2)
        {
            throw retruxx::logic_error("Not implemented");
        }
        else if (id == -3)
        {
            throw retruxx::logic_error("Not implemented");
        }
        else
        {
            if (id != -4)
            {
                throw retruxx::logic_error("Not implemented");
            }
        }
        m_profiler->EndCountdown();
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

    void DecalsServer::AddItemsList(retruxx::vector<ServerItem>& itemsList)
    {
        // TODO: implement DecalsServer::AddItemsList
        //for (const auto& item : itemsList)
        //{
        //    throw retruxx::logic_error("Not implemented");
        //}
    }
}

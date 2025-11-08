#include <decals.h>
#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serverdecals.h>

namespace m3d
{
    int DecalsServer::AddItem(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DecalsServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DecalsServer::RenderItem(int id, void* params)
    {
        m_profiler->StartCountdown();
        if (id == -2)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        else if (id == -3)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        else
        {
            if (id != -4)
            {
                RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    DecalsServer::~DecalsServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DecalsServer::Release()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int DecalsServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void DecalsServer::AddItemsList(retruxx::vector<ServerItem>& itemsList)
    {
        // TODO: implement DecalsServer::AddItemsList
        //for (const auto& item : itemsList)
        //{
        //    RETRUXX_NOT_IMPLEMENTED;
        //}
    }
}

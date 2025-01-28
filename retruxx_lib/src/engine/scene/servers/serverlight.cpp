#include <flares.h>
#include <m3dapp.h>
#include <stdexcept>
#include <scene/servers/serverlight.h>

namespace m3d
{
    int LightsServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LightsServer::Init()
    {
        m_valid = true;
        CFlare::Init();
        return 1;
    }

    int LightsServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LightsServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    LightsServer::~LightsServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void LightsServer::RenderItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LightsServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LightsServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    LightsServer::LightsServer()
    {
        auto id = Application::g_pApp->GetProfilerStack().AddProfiler("point lights", 0x1E);
        if (id < Application::g_pApp->GetProfilerStack().GetNumProfilers())
        {
            m_profiler = Application::g_pApp->GetProfilerStack().GetProfiler(id);
        }
    }

    int LightsServer::SetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void LightsServer::AddItemsList(retruxx::vector<ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

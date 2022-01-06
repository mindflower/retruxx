#include <stdexcept>
#include <scene/servers/serverlight.h>

namespace m3d
{
    int LightsServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int LightsServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    int LightsServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    int LightsServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    LightsServer::~LightsServer()
    {
        throw std::logic_error("Not implemented");
    }

    void LightsServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int LightsServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    int LightsServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    LightsServer::LightsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int LightsServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void LightsServer::AddItemsList(std::vector<ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }
}

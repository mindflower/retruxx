#include <stdexcept>
#include <scene/servers/serverdecals.h>

namespace m3d
{
    int DecalsServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int DecalsServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    void DecalsServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int DecalsServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    int DecalsServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    DecalsServer::~DecalsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int DecalsServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    int DecalsServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    DecalsServer::DecalsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int DecalsServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void DecalsServer::AddItemsList(std::vector<ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }
}

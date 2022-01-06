#include <stdexcept>
#include <scene/servers/serverlines.h>

namespace m3d
{
    int LinesServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int LinesServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    LinesServer::~LinesServer()
    {
        throw std::logic_error("Not implemented");
    }

    int LinesServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    int LinesServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    int LinesServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    LinesServer::LinesServer()
    {
        throw std::logic_error("Not implemented");
    }

    void LinesServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int LinesServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int LinesServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void LinesServer::AddItemsList(std::vector<ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }
}

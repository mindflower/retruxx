#include <scene/servers/serverprojectors.h>

namespace m3d
{
    ProjectorsServer::~ProjectorsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int ProjectorsServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int ProjectorsServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int ProjectorsServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int ProjectorsServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    ProjectorsServer::ProjectorsServer()
    {
        throw std::logic_error("Not implemented");
    }

    int ProjectorsServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    int ProjectorsServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ProjectorsServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int ProjectorsServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ProjectorsServer::AddItemsList(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }
}

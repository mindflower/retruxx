#include <stdexcept>
#include <scene/servers/serversprites.h>

namespace m3d
{
    int SpritesServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    SpritesServer::~SpritesServer()
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void SpritesServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    SpritesServer::SpritesServer()
    {
        throw std::logic_error("Not implemented");
    }

    int SpritesServer::Init()
    {
        throw std::logic_error("Not implemented");
    }

    void SpritesServer::AddItemsList(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }
}

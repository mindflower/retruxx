#include <stdexcept>
#include <scene/servers/servermusic.h>

namespace m3d
{
    MusicServer::~MusicServer()
    {
        throw std::logic_error("Not implemented");
    }

    int MusicServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int MusicServer::SetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void MusicServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int MusicServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void MusicServer::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    int MusicServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    int MusicServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int MusicServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void MusicServer::AddItemsList(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }

    int MusicServer::_GetSoundIdByServerHandle(int) const
    {
        throw std::logic_error("Not implemented");
    }
}

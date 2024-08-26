#include <stdexcept>
#include <scene/servers/serversound.h>

namespace m3d
{
    void Sound3DServer::UnregisterNode(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::RemoveItem(int)
    {
        throw std::logic_error("Not implemented");
    }

    Sound3DServer::~Sound3DServer()
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::AddItem(char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Sound3DServer::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::GetItemProperty(int, int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    void Sound3DServer::RegisterNode(m3d::SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::SaveAllLoadedEntities(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Sound3DServer::RenderItem(int, void*)
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void Sound3DServer::AddItemsList(std::vector<m3d::DataServer::ServerItem>&)
    {
        throw std::logic_error("Not implemented");
    }

    void Sound3DServer::_AddItemFromXmlNode(m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::_AddFakeItem()
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::_AddTripleItem(CStr, CStr, CStr, char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::_AddItem(char const*, char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Sound3DServer::_AddDoubleItem(CStr, CStr, char const*, char const*)
    {
        throw std::logic_error("Not implemented");
    }
}

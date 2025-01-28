#include <stdexcept>
#include <scene/servers/serverlines.h>

namespace m3d
{
    int LinesServer::AddItem(char const*, char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LinesServer::SaveAllLoadedEntities(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    LinesServer::~LinesServer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LinesServer::RemoveItem(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LinesServer::Init()
    {
        m_valid = true;
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_CIRCLE" });
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_SIGHT" });
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_FLOWER" });
        m_models.push_back({ nullptr, nullptr, nullptr, "LSI_ARROW" });
        return 1;
    }

    int LinesServer::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    LinesServer::LinesServer()
    {
    }

    void LinesServer::RenderItem(int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LinesServer::SetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int LinesServer::GetItemProperty(int, int, void*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void LinesServer::AddItemsList(retruxx::vector<ServerItem>&)
    {
        throw retruxx::logic_error("Not implemented");
    }
}

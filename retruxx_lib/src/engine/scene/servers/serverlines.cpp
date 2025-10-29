#include <stdexcept>
#include <scene/servers/serverlines.h>

namespace m3d
{
    int LinesServer::AddItem(char const*, char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LinesServer::SaveAllLoadedEntities(char const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    LinesServer::~LinesServer()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LinesServer::RemoveItem(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    LinesServer::LinesServer()
    {
    }

    void LinesServer::RenderItem(int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LinesServer::SetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int LinesServer::GetItemProperty(int, int, void*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void LinesServer::AddItemsList(retruxx::vector<ServerItem>&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}

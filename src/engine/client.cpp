#include <client.h>

namespace m3d
{
    CClient* pClient = nullptr;

    CWorld& CClient::GetWorld()
    {
        return *m_world;
    }
}

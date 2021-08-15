#include "guihelper.h"
#include <client.h>
#include <level.h>
#include <world.h>

namespace m3d
{
    extern CClient* pClient;
}

namespace help
{
    CStr GetCurrentLevelName()
    {
         if (m3d::pClient != nullptr)
         {
             if (auto * level = m3d::pClient->GetWorld().m_level; level != nullptr)
             {
                 return level->GetLevelName();
             }
         }
         return {};
    }
}

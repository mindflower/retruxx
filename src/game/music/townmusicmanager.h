#pragma once
#include <vector>

namespace m3d
{
    class TownMusicManager
    {
    public:
        void Activate();
        int LaunchAmbientWorkshop();
        void StopAmbient();
        void Init();
        int LaunchAmbientShop();

    private:
        std::vector<int> m_ambientShopSounds;
        std::vector<int> m_ambientWorkshopSounds;
        int m_curAmbientChannelId = -1;
    };
}

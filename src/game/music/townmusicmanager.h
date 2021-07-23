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
        TownMusicManager();
        void Init();
        int LaunchAmbientShop();
        ~TownMusicManager();

    private:
        std::vector<int> m_ambientShopSounds;
        std::vector<int> m_ambientWorkshopSounds;
        int m_curAmbientChannelId;
    };
}

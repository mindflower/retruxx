#pragma once
#include <map>
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    class BlockMusicManager
    {
    public:
        enum BlockMusicType
        {
            DRIVING = 0x0,
            ALARM = 0x1,
            BATTLE = 0x2,
            NUM_MUSIC_TYPES = 0x3,
        };

    public:
        void SetMusicType(BlockMusicType);
        bool IsMusicBlockNameValid(CStr const&) const;
        void Reset();
        void Init();
        void InitOnce();
        int PlayCurrentMusic();

    private:
        static void _MusicEndCallback(int);

    private:
        std::vector<std::vector<CStr>> m_blocks;
        int m_curBlockNum = 0;
        BlockMusicType m_curMusicType = NUM_MUSIC_TYPES;
        std::map<CStr, int> m_blockNamesToIds;
        bool m_bMustPlayNewMusic = 0;
        int m_curChannelId = -1;
    };
}

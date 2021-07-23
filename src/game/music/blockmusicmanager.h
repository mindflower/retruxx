#pragma once
#include <map>
#include <vector>

class CStr;

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
        ~BlockMusicManager();
        bool IsMusicBlockNameValid(CStr const&) const;
        void Reset();
        BlockMusicManager();
        void Init();
        void InitOnce();
        int PlayCurrentMusic();

    private:
        static void __fastcall _MusicEndCallback(int);

    private:
        std::vector<std::vector<CStr>> m_blocks;
        int m_curBlockNum;
        BlockMusicType m_curMusicType;
        std::map<CStr, int> m_blockNamesToIds;
        bool m_bMustPlayNewMusic;
        int m_curChannelId;
    };
}

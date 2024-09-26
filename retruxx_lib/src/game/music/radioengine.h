#pragma once
#include <deque>
#include <map>
#include <vector>
#include <core/stringm3d.h>
#include "thirdparty/stl/vector.hpp"

namespace m3d
{
    class RadioEngine
    {
    public:
        class RadioSoundItem
        {
        private:
            CStr sound;
            int type;
        };

        struct SCurId
        {
            /* 0x0000 */ oldstd::vector<int> ids;
            /* 0x0010 */ int probability;
        }; /* size: 0x0014 */

    public:
        static inline RadioEngine* m_instance = nullptr;

    public:
        void Init();
        ~RadioEngine();
        void PlayNextSoundMessage();
        static RadioEngine* GetInstance();
        void PlaySoundMessage(int, int, CStr const&);

    private:
        std::map<int, CStr> m_soundIdName;
        std::map<int, int> m_soundIdSound;
        std::map<int, CStr> m_groupNameByBelong;
        std::map<int, int> m_lastTime;
        std::deque<RadioSoundItem> m_soundDeque;
        std::map<CStr, SCurId> m_correctIds;
        int m_curChannelId = -1;
        CStr m_curTextToShow;
    };
}

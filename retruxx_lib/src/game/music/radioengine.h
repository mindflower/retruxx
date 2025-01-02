#pragma once
#include <core/stringm3d.h>
#include "thirdparty/containers.h"

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
            /* 0x0000 */ retruxx::vector<int> ids;
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
        retruxx::map<int, CStr> m_soundIdName;
        retruxx::map<int, int> m_soundIdSound;
        retruxx::map<int, CStr> m_groupNameByBelong;
        retruxx::map<int, int> m_lastTime;
        retruxx::deque<RadioSoundItem> m_soundDeque;
        retruxx::map<CStr, SCurId> m_correctIds;
        int m_curChannelId = -1;
        CStr m_curTextToShow;
    };
}

#pragma once
// A sound known to the manager: the FMOD object behind it, the group it belongs to, the end-of-music
// callback and the file it was loaded from.
#include <core/stringm3d.h>

#include "fmod/fsound.h"

namespace snd
{
    // Defined in CM3DSoundManager.cpp, filled by FMOD_CreateInstance in the manager's constructor.
    extern FMOD_INSTANCE* pFMOD_INSTANCE;

    enum FmodSoundType
    {
        FST_SAMPLE = 0,
        FST_STREAM = 1,
        FST_SONG = 2,
    };

    struct CSoundItem
    {
        // orig 0x5f0c50 sounditem.h:50
        CSoundItem(FmodSoundType type, void* soundPtr, int group)
        {
            // 51
            m_type = type;
            // 53
            switch (type)
            {
            case FST_SAMPLE:
            case FST_STREAM:
            case FST_SONG:
                // 56
                m_pSample = (FSOUND_SAMPLE*)soundPtr;
                break;
            }
            // 66
            m_group = group;
            // 68
            m_endCallback = 0;
        }

        // orig 0x5efc50 sounditem.h:72
        FSOUND_SAMPLE* GetSample()
        {
            // 73
            switch (m_type)
            {
            default:
                // 76
                return 0;
            case FST_SAMPLE:
                // 79
                return m_pSample;
            case FST_STREAM:
                // 82
                return pFMOD_INSTANCE->FSOUND_Stream_GetSample(m_pStream);
            }
        }

        // orig 0x5efc80 sounditem.h:87
        FSOUND_STREAM* GetStream()
        {
            // 88
            if (m_type == FST_STREAM)
            {
                // 90
                return m_pStream;
            }
            // 94
            return 0;
        }

        FmodSoundType m_type;
        union
        {
            FSOUND_SAMPLE* m_pSample;
            FSOUND_STREAM* m_pStream;
            FMUSIC_MODULE* m_pSong;
        };
        int m_group;
        // The engine's end-of-music callback. The original and Hard Truck Apocalypse's own
        // sound.dll call it with the sound id in ecx (__fastcall); the engine header spells the
        // pointer without a calling convention, so the manager stores it as this type.
        void(__fastcall* m_endCallback)(int);
        CStr m_fileName;
    };
}

// The sound manager: the ISound implementation over FMOD. Sounds are samples (short effects) or
// streams (music and long effects), grouped in a tree of sound groups loaded from an XML file;
// channels are tracked by the id of the sound they play, music fades out before the next track.
// Ported from the original sound/CM3DSoundManager.cpp (CM3DSoundManager.obj); the functions are in
// the order of their original source lines and each carries the RVA and start line of its original.
//
// Differences from the original compiland, all forced by the executable the DLL is loaded into:
//  - The original linked the driver into the executable and reached the kernel, the log and the
//    asserts directly. Hard Truck Apocalypse hands the kernel to createISound and the log callback
//    to Init, which stores it in l_log (the original already had that static, unset); the
//    M3D_LOG_INFO / M3D_LOG_ERR calls go through it, M3D_ASSERT reaches the kernel's
//    SysError(whence, descr) with the original assertion text, file and line.
//  - The original's ISound has GetSoundIdByFilename and AddCustomMusic as virtuals; the HTA
//    interface lacks both, so they are plain members here.
//  - FMOD itself: see fmod/fsound_compat.cpp.
#include "sounditem.h"
#include "soundgroup.h"

#include <core/console/cvar.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
#include <core/stringm3d.h>
#include <core/threadsync.h>
#include <config.h>
#include <file/filestream.h>
#include <file/fileserver.h>
#include <iface.h>
#include <math/vector.h>

#include <map>
#include <vector>

#include "fmod/fsound_errors.h"

extern m3d::Kernel* g_kernel;

// The original assertion texts name the kernel the way the statically linked driver saw it.
using m3d::g_Kernel;

namespace snd
{
    // orig 0x89b938 CM3DSoundManager.cpp (file static)
    FMOD_INSTANCE* pFMOD_INSTANCE = 0;

    namespace
    {
        // orig 0x89b940 CM3DSoundManager.cpp (file static): the log callback. Unset in the original;
        // Hard Truck Apocalypse's Init stores the executable's callback here.
        void(__fastcall* l_log)(CStr const&) = 0;

        void Log(CStr const& msg)
        {
            if (l_log)
            {
                l_log(msg);
            }
        }

        // The failure path of the original M3D_ASSERT: Kernel::SysError(assertion, file, line).
        // HTA's kernel has SysError(whence, descr); the message is assembled the way HTA's
        // SYS_ERROR does.
        void soundAssertFailed(char const* assertion, char const* file, int line)
        {
            g_kernel->SysError(CStr(file) + CStr(":") + CStr(line), CStr(assertion));
        }
    }

// The original M3D_ASSERT with the file name and line number the original binary carries.
#define SOUND_ASSERT(cond, line)                                            \
    if (!(cond))                                                            \
    {                                                                       \
        soundAssertFailed(#cond, ".\\CM3DSoundManager.cpp", (line));        \
    }

    // orig 0x7d8e14 CM3DSoundManager.cpp (file static)
    static int const INVALID_ID = -1;

    // The engine's name for unsigned int, as the assertion texts spell it.
    typedef unsigned int uint;

    namespace
    {
        // The file callbacks FMOD reads the sound files through (defined after the class).
        void* F_CALLBACKAPI OpenFileCallback(const char* fileName);
        int F_CALLBACKAPI ReadFileCallback(void* buffer, int size, void* handle);
        int F_CALLBACKAPI SeekFileCallback(void* handle, int pos, signed char mode);
        int F_CALLBACKAPI TellFileCallback(void* handle);
        void F_CALLBACKAPI CloseFileCallback(void* handle);
    }

    class CM3DSoundManager : public ISound
    {
    public:
        // 37: IBase
        int IncRef() override;
        int DecRef() override;
        void* QueryIface(const char* ifaceName) override;

        CM3DSoundManager();
        ~CM3DSoundManager() override;

        // ISound, in the order of Hard Truck Apocalypse's vtable
        bool Init(void(__fastcall* logFunc)(const CStr&), unsigned int SampleRate, unsigned int BitsPerSample,
                  unsigned char maxSounds, const char* groupsFileName) override;
        int Update(double dT) override;
        void SetMaxVolume(int volume) override;
        void SetMusicFadeTime(float fadeTime) override;
        unsigned char GetChannelVolume(int channelHandle) override;
        bool SetChannelVolume(int channelHandle, unsigned char volume, bool isAbsolute) override;
        bool SetGroupVolume(int group, unsigned char volume) override;
        bool IsChannelPlaying(int channelHandle) override;
        bool IsMusicPlaying(int channelHandle) override;
        bool PauseGroup(int groupId, bool pause) override;
        bool PauseAllSounds(bool pause) override;
        int PlayMusic(int sound_id, bool loop_flag, bool immediate) override;
        int PlaySound2D(int sound_id, bool loop_flag) override;
        int PlaySound3D(int sound_id, const CVector& position, const CVector& velocity, bool loop_flag) override;
        bool SetChannelLoopMode(int channelHandle, bool loop_flag) override;
        int GetChannelFrequency(int channelHandle) override;
        bool SetChannelFrequency(int channelHandle, int frequency) override;
        bool SetPosition(int channelHandle, const CVector& position, const CVector& velocity) override;
        bool SetListenerPosition(const CVector& position, const CVector& velocity, const CVector& orientFront,
                                 const CVector& orientTop) override;
        bool StopChannel(int channelHandle) override;
        bool StopGroup(int group) override;
        int StopAllSounds() override;
        int MuteAllSounds() override;
        int RestoreAllVolumes() override;
        int AddSound(const char* filename, UserSoundType soundType, int group, int maxSounds,
                     SoundPriority priority) override;
        int AddSound(const char* filename, UserSoundType soundType, const char* groupName, int maxSounds,
                     SoundPriority priority) override;
        bool DeleteIdTableSound(int soundId) override;
        bool DeleteAllSounds() override;
        bool SetSoundPriority(int soundId, SoundPriority priority) override;
        bool SetEndMusicCallback(int soundId, void (*endCallback)(int)) override;
        float GetCPUusage() override;
        void GetMemUsage(unsigned int& curAllocated, unsigned int& maxAllocated) override;
        int GetSoundGroupId(int soundId) override;
        bool GetGroupMinDist(int groupId, float& minDist) override;
        bool GetGroupMaxDist(int groupId, float& maxDist) override;
        void DumpSoundInfo() override;

        // original: virtual ISound slots 0x70 and 0x74; not in HTA's interface.
        int GetSoundIdByFilename(CStr const& filename);
        int AddCustomMusic(const char* filename);

    private:
        void _SetSoundIdForChannel(int channelNum, int soundId);
        bool _LoadGroupsFromXmlFile(const char* groupsFileName);
        void _LoadSoundGroupFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode, SoundGroup* parent);
        bool _bIsSoundGroupKindOf(int soundGroupId, int ancestorId);
        CSoundItem* _GetSoundById(int soundId);
        void _PurgeChannels();
        static signed char F_CALLBACKAPI _CommonMusicEndCallback(FSOUND_STREAM* stream, void* buff, int len,
                                                                 void* userdata);

        /* 0x0004 */ int m_refCount;
        /* 0x0008 */ IBase* m_parent;
        /* 0x000c */ std::map<int, CSoundItem*> m_sounds;
        /* 0x0018 */ std::vector<int> m_channels;
        /* 0x0028 */ int m_nextSoundId;
        /* 0x002c */ bool m_bChangeFlag;
        /* 0x002d */ bool m_bNextMusicMustBeLooped;
        /* 0x0030 */ int m_nNextMusic;
        /* 0x0034 */ float m_CurValue;
        /* 0x0038 */ int m_MaxValue;
        /* 0x003c */ float m_musicFadeTime;
        /* 0x0040 */ unsigned int m_bitsPerSample;
        /* 0x0044 */ bool m_isInited;
        static CM3DSoundManager* m_instance;
        /* 0x0048 */ std::map<CStr, SoundGroup*> m_soundGroupMap;
        /* 0x0054 */ std::vector<SoundGroup*> m_soundGroupVector;
        static m3d::CriticalSection m_musicEndCallbackCs;
    }; /* size: 0x0064 */

    // orig 0x89b944 CM3DSoundManager.cpp (static member)
    CM3DSoundManager* CM3DSoundManager::m_instance = 0;
    // orig 0x89b94c CM3DSoundManager.cpp (static member); its InitializeCriticalSection ran as the
    // static initializer $E10 at CM3DSoundManager.cpp:75.
    m3d::CriticalSection CM3DSoundManager::m_musicEndCallbackCs;

    namespace
    {
        // orig 0x5efd40 CM3DSoundManager.cpp:57
        int ChannelNumFromHandle(int channelHandle)
        {
            // 58
            return channelHandle & 0xfff;
        }

        // orig 0x5efd50 CM3DSoundManager.cpp:64
        void MakeLoopedMode(unsigned int& mode, bool loop_flag)
        {
            // 65
            if (loop_flag)
            {
                // 66
                mode = (mode & ~FSOUND_LOOP_OFF) | FSOUND_LOOP_NORMAL;
            }
            else
            {
                // 68
                mode = (mode & ~FSOUND_LOOP_NORMAL) | FSOUND_LOOP_OFF;
            }
        }
    }

    // orig 0x5efcd0 CM3DSoundManager.cpp:37
    int CM3DSoundManager::IncRef()
    {
        if (m_parent)
        {
            m_parent->IncRef();
        }
        return ++m_refCount;
    }

    // orig 0x5efcf0 CM3DSoundManager.cpp:37
    int CM3DSoundManager::DecRef()
    {
        int refs = --m_refCount;
        if (m_parent)
        {
            m_parent->DecRef();
        }
        if (m_refCount <= 0)
        {
            delete this;
        }
        return refs;
    }

    // orig 0x5efd20 CM3DSoundManager.cpp:37
    void* CM3DSoundManager::QueryIface(const char* ifaceName)
    {
        if (m_parent)
        {
            return m_parent->QueryIface(ifaceName);
        }
        return 0;
    }

    // orig 0x5f64a0 CM3DSoundManager.cpp:87
    CM3DSoundManager::CM3DSoundManager()
    {
        m_refCount = 0;
        m_parent = 0;

        // 90
        pFMOD_INSTANCE = FMOD_CreateInstance("fmod.dll");

        // 98
        m_CurValue = 255.0f;

        // 102
        m_nextSoundId = 0;
        m_bChangeFlag = false;
        m_bNextMusicMustBeLooped = false;
        m_isInited = false;
        m_nNextMusic = -1;
        m_MaxValue = 255;
        m_bitsPerSample = 16;
        m_musicFadeTime = 2.0f;

        // 113
        l_log = 0;
    }

    // orig 0x5f69f0 CM3DSoundManager.cpp:125
    // original: bool Init(SampleRate, BitsPerSample, maxSounds, groupsFileName) on the statically
    // linked driver. Hard Truck Apocalypse's interface hands over the log callback as well; its
    // own sound.dll stores it before the original body, which is kept unchanged.
    bool CM3DSoundManager::Init(void(__fastcall* logFunc)(const CStr&), unsigned int SampleRate,
                                unsigned int BitsPerSample, unsigned char maxSounds, const char* groupsFileName)
    {
        // 127-128
        if (m_isInited)
        {
            return false;
        }

        l_log = logFunc;

        // 130
        SOUND_ASSERT(!m_instance, 130);

        // 135
        m_instance = this;

        // 141
        m_bitsPerSample = BitsPerSample;
        if (m_bitsPerSample != 8 && m_bitsPerSample != 16)
        {
            // 142
            m_bitsPerSample = 16;
        }

        // 145
        m_isInited = pFMOD_INSTANCE->FSOUND_Init(SampleRate, maxSounds, 0) != 0;

        // 148
        if (!m_isInited)
        {
            // 150: original M3D_LOG_INFO
            Log(CStr("Cannot initialize sound engine. ") + CStr("Error: ") +
                CStr(FMOD_ErrorString(pFMOD_INSTANCE->FSOUND_GetError())));
            // 151
            m_instance = 0;
            // 152
            return false;
        }

        // 156
        m_channels.assign(pFMOD_INSTANCE->FSOUND_GetMaxChannels(), -1);

        // 158: original M3D_LOG_INFO
        Log(CStr("Max sound channels available: ") + CStr((unsigned int)m_channels.size()));

        // 168
        pFMOD_INSTANCE->FSOUND_File_SetCallbacks(OpenFileCallback, CloseFileCallback, ReadFileCallback,
                                                 SeekFileCallback, TellFileCallback);

        // 171
        if (!_LoadGroupsFromXmlFile(groupsFileName))
        {
            // 174
            m_instance = 0;
            return false;
        }

        // 177: original M3D_LOG_INFO
        Log("Sound is inited successfully");

        // 179
        return true;
    }

    // orig 0x5f5ed0 CM3DSoundManager.cpp:189
    CM3DSoundManager::~CM3DSoundManager()
    {
        // 190: original M3D_LOG_INFO
        Log("Deleting Sound...");

        // 193
        if (m_isInited)
        {
            // 195: original M3D_LOG_INFO
            Log("Stopping all sounds in FMOD...");
            // 198
            pFMOD_INSTANCE->FSOUND_StopSound(FSOUND_ALL);
            // 200: original M3D_LOG_INFO
            Log("Closing FMOD...");
            // 203
            pFMOD_INSTANCE->FSOUND_Close();
        }

        // 206: original M3D_LOG_INFO
        Log("Deleting all sounds...");

        // 209
        DeleteAllSounds();

        // 212: the original releases the channel table's buffer here
        std::vector<int>().swap(m_channels);

        // 217
        for (std::vector<SoundGroup*>::iterator iter = m_soundGroupVector.begin(); iter != m_soundGroupVector.end();
             ++iter)
        {
            // 219
            if (*iter)
            {
                delete *iter;
            }
        }
        // 222
        std::vector<SoundGroup*>().swap(m_soundGroupVector);
        // 223
        m_soundGroupMap.clear();

        // 225
        FMOD_FreeInstance(pFMOD_INSTANCE);

        // 227: original M3D_LOG_INFO
        Log("Sound is uninstalled");
    }

    // orig 0x5f3b10 CM3DSoundManager.cpp:238
    bool CM3DSoundManager::SetGroupVolume(int group, unsigned char volume)
    {
        // 240-241
        if (!m_isInited)
        {
            return false;
        }

        // 244-245
        if (group < 0 || group >= (int)m_soundGroupVector.size())
        {
            return false;
        }

        // 251
        for (std::vector<SoundGroup*>::iterator iter = m_soundGroupVector.begin(); iter != m_soundGroupVector.end();
             ++iter)
        {
            // 253
            if (_bIsSoundGroupKindOf((*iter)->GetId(), group))
            {
                // 255
                (*iter)->SetVolume(volume);
            }
        }

        // 262
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 264
            if (m_channels[i] == -1)
            {
                continue;
            }

            // 267
            SOUND_ASSERT(m_channels[i] >= 0, 267);

            // 270
            std::map<int, CSoundItem*>::iterator it = m_sounds.find(m_channels[i]);
            if (it == m_sounds.end())
            {
                continue;
            }
            CSoundItem* item = it->second;
            // 272
            if (!item)
            {
                continue;
            }

            // 275
            pFMOD_INSTANCE->FSOUND_SetVolume(i, m_soundGroupVector[item->m_group]->GetVolume());
        }

        // 279
        return true;
    }

    // orig 0x5f3c50 CM3DSoundManager.cpp:292
    int CM3DSoundManager::PlayMusic(int sound_id, bool loop_flag, bool immediate)
    {
        // 294-295
        if (!m_isInited)
        {
            return -1;
        }

        // 297
        std::map<int, CSoundItem*>::iterator it = m_sounds.find(sound_id);
        if (it == m_sounds.end())
        {
            return -1;
        }
        CSoundItem* item = it->second;
        // 300
        if (!item || item->m_type != FST_STREAM || !item->m_pStream)
        {
            return -1;
        }

        // 307
        m_nNextMusic = -1;
        m_bChangeFlag = false;
        if (!immediate)
        {
            // 312
            m_bNextMusicMustBeLooped = loop_flag;
            // 313
            m_nNextMusic = sound_id;
            m_bChangeFlag = true;
            m_CurValue = (float)m_MaxValue;
            return -1;
        }

        // 320
        StopGroup(SND_MUSIC_GROUP);

        // 322
        unsigned int mode = pFMOD_INSTANCE->FSOUND_Stream_GetMode(item->m_pStream);
        // 323
        MakeLoopedMode(mode, loop_flag);
        // 334
        pFMOD_INSTANCE->FSOUND_Stream_SetMode(item->m_pStream, mode);

        // 349
        pFMOD_INSTANCE->FSOUND_Stream_SetPosition(item->m_pStream, 0);

        // 359
        int channelHandle = pFMOD_INSTANCE->FSOUND_Stream_PlayEx(FSOUND_FREE, item->m_pStream, 0, 1);

        // 362-367
        if (channelHandle == -1)
        {
            return -1;
        }

        // 370
        SOUND_ASSERT(channelHandle >= 0, 370);

        // 373
        _SetSoundIdForChannel(ChannelNumFromHandle(channelHandle), sound_id);

        // 378
        SetGroupVolume(SND_MUSIC_GROUP, (unsigned char)m_MaxValue);

        // 381
        pFMOD_INSTANCE->FSOUND_SetPaused(channelHandle, 0);

        // 389
        return channelHandle;
    }

    // orig 0x5f3d90 CM3DSoundManager.cpp:400
    int CM3DSoundManager::PlaySound2D(int sound_id, bool loop_flag)
    {
        // 402-403
        if (!m_isInited)
        {
            return -1;
        }

        // 405
        std::map<int, CSoundItem*>::iterator it = m_sounds.find(sound_id);
        if (it == m_sounds.end())
        {
            return -1;
        }
        CSoundItem* item = it->second;
        // 408-409
        if (!item)
        {
            return -1;
        }

        // 411
        FSOUND_SAMPLE* sample = item->GetSample();
        // 412-413
        if (!sample)
        {
            return -1;
        }

        int channelHandle;
        // 415
        if (item->m_type == FST_STREAM && item->m_pStream)
        {
            FSOUND_STREAM* stream = item->m_pStream;

            // 427
            unsigned int mode = pFMOD_INSTANCE->FSOUND_Stream_GetMode(stream);
            // 428
            MakeLoopedMode(mode, loop_flag);
            // 432
            pFMOD_INSTANCE->FSOUND_Stream_SetMode(stream, mode);

            // 435
            pFMOD_INSTANCE->FSOUND_Stream_SetPosition(stream, 0);

            // 438
            channelHandle = pFMOD_INSTANCE->FSOUND_Stream_PlayEx(FSOUND_FREE, stream, 0, 1);
            // 440
        }
        else
        {
            // 442
            unsigned int mode = pFMOD_INSTANCE->FSOUND_Sample_GetMode(item->m_pSample);
            // 443
            MakeLoopedMode(mode, loop_flag);
            // 447
            pFMOD_INSTANCE->FSOUND_Sample_SetMode(item->m_pSample, mode);

            // 450
            channelHandle = pFMOD_INSTANCE->FSOUND_PlaySoundEx(FSOUND_FREE, item->m_pSample, 0, 1);
        }

        // 454-455
        if (channelHandle == -1)
        {
            return -1;
        }

        // 457
        SOUND_ASSERT(channelHandle >= 0, 457);

        // 460
        _SetSoundIdForChannel(ChannelNumFromHandle(channelHandle), sound_id);

        // 464
        pFMOD_INSTANCE->FSOUND_SetVolume(channelHandle, m_soundGroupVector[item->m_group]->GetVolume());

        // 467
        pFMOD_INSTANCE->FSOUND_SetPaused(channelHandle, 0);

        // 469
        return channelHandle;
    }

    // orig 0x5f3f20 CM3DSoundManager.cpp:482
    int CM3DSoundManager::PlaySound3D(int sound_id, const CVector& position, const CVector& velocity, bool loop_flag)
    {
        // 485-486
        if (!m_isInited)
        {
            return -1;
        }

        // 490
        std::map<int, CSoundItem*>::iterator it = m_sounds.find(sound_id);
        CSoundItem* item = (it == m_sounds.end()) ? 0 : it->second;
        // 493-494
        if (!item)
        {
            return -1;
        }

        // 496
        FSOUND_SAMPLE* sample = item->GetSample();
        // 497-498
        if (!sample)
        {
            return -1;
        }

        // 500
        FSOUND_STREAM* stream = (item->m_type == FST_STREAM) ? item->m_pStream : 0;

        // 517
        float pos[3];
        float vel[3];
        pos[0] = position.x;
        pos[1] = position.z;
        pos[2] = -position.y;
        vel[0] = velocity.x;
        vel[1] = velocity.z;
        vel[2] = -velocity.y;

        int channelHandle;
        if (stream)
        {
            // 519
            unsigned int mode = pFMOD_INSTANCE->FSOUND_Stream_GetMode(stream);
            // 520
            MakeLoopedMode(mode, loop_flag);
            // 524
            pFMOD_INSTANCE->FSOUND_Stream_SetMode(stream, mode);
            // 527
            pFMOD_INSTANCE->FSOUND_Stream_SetPosition(stream, 0);
            // 530
            channelHandle = pFMOD_INSTANCE->FSOUND_Stream_PlayEx(FSOUND_FREE, stream, 0, 1);
            // 532
        }
        else
        {
            // 534
            unsigned int mode = pFMOD_INSTANCE->FSOUND_Sample_GetMode(sample);
            // 535
            MakeLoopedMode(mode, loop_flag);
            // 539
            pFMOD_INSTANCE->FSOUND_Sample_SetMode(sample, mode);
            // 542
            channelHandle = pFMOD_INSTANCE->FSOUND_PlaySoundEx(FSOUND_FREE, sample, 0, 1);
        }

        // 546-547
        if (channelHandle == -1)
        {
            return -1;
        }

        // 549
        SOUND_ASSERT(channelHandle >= 0, 549);

        // 552
        _SetSoundIdForChannel(ChannelNumFromHandle(channelHandle), sound_id);

        // 555
        pFMOD_INSTANCE->FSOUND_SetVolume(channelHandle, m_soundGroupVector[item->m_group]->GetVolume());

        // 558
        pFMOD_INSTANCE->FSOUND_3D_SetAttributes(channelHandle, pos, vel);

        // 561
        pFMOD_INSTANCE->FSOUND_SetPaused(channelHandle, 0);

        // 564
        return channelHandle;
    }

    // orig 0x5f0d30 CM3DSoundManager.cpp:570
    bool CM3DSoundManager::SetChannelLoopMode(int channelHandle, bool loop_flag)
    {
        // 572-573
        if (!m_isInited)
        {
            return false;
        }

        // 583-584
        if (channelHandle == -1)
        {
            return false;
        }

        // 586
        SOUND_ASSERT(channelHandle >= 0, 586);
        // 587
        SOUND_ASSERT(ChannelNumFromHandle( channelHandle ) < ( int )m_channels.size(), 587);

        // 589
        return pFMOD_INSTANCE->FSOUND_SetLoopMode(channelHandle, loop_flag ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF) != 0;
    }

    // orig 0x5f0dd0 CM3DSoundManager.cpp:595
    int CM3DSoundManager::GetChannelFrequency(int channelHandle)
    {
        // 597-598
        if (!m_isInited)
        {
            return 0;
        }

        // 600-601
        if (channelHandle == -1)
        {
            return 0;
        }

        // 603
        SOUND_ASSERT(channelHandle >= 0, 603);
        // 604
        SOUND_ASSERT(ChannelNumFromHandle( channelHandle ) < ( int )m_channels.size(), 604);

        // 606
        return pFMOD_INSTANCE->FSOUND_GetFrequency(channelHandle);
    }

    // orig 0x5efd70 CM3DSoundManager.cpp:612
    bool CM3DSoundManager::SetChannelFrequency(int channelHandle, int frequency)
    {
        // 614-615
        if (!m_isInited)
        {
            return false;
        }

        // 617
        SOUND_ASSERT(channelHandle >= -1, 617);

        // 619
        if (channelHandle != -1)
        {
            // 621
            return pFMOD_INSTANCE->FSOUND_SetFrequency(channelHandle, frequency) != 0;
        }

        // 624
        return false;
    }

    // orig 0x5f1e90 CM3DSoundManager.cpp:630
    int CM3DSoundManager::GetSoundIdByFilename(CStr const& filename)
    {
        // 631
        for (std::map<int, CSoundItem*>::const_iterator iter = m_sounds.begin(); iter != m_sounds.end(); ++iter)
        {
            // 633
            if (iter->second->m_fileName == filename)
            {
                // 635
                return iter->first;
            }
        }
        // 638
        return -1;
    }

    // orig 0x5f4d10 CM3DSoundManager.cpp:644
    int CM3DSoundManager::AddCustomMusic(const char* filename)
    {
        // 649-651
        if (!m_isInited)
        {
            return -1;
        }

        // 655
        unsigned int flags = ((m_bitsPerSample == 16) ? FSOUND_16BITS : FSOUND_8BITS) | FSOUND_STEREO;
        FSOUND_STREAM* stream = pFMOD_INSTANCE->FSOUND_Stream_Open(filename, flags, 0, 0);
        // 656
        if (!stream)
        {
            // 659: original M3D_LOG_INFO
            Log(CStr("Cannot open sound stream from file ") + CStr(filename) + CStr(". Error: ") +
                CStr(FMOD_ErrorString(pFMOD_INSTANCE->FSOUND_GetError())) + CStr(", flags: ") + CStr((int)flags));
            // 660
            return -1;
        }

        // 662
        FSOUND_SAMPLE* sample = pFMOD_INSTANCE->FSOUND_Stream_GetSample(stream);
        // 663
        if (sample)
        {
            // 665
            pFMOD_INSTANCE->FSOUND_Sample_SetDefaults(sample, -1, -1, -1, SND_PRIORITY_EXTRAHIGH);
        }

        // 667
        CSoundItem* item = new CSoundItem(FST_STREAM, stream, SND_MUSIC_GROUP);
        // 668
        item->m_fileName = CStr(filename);

        // 669
        pFMOD_INSTANCE->FSOUND_Stream_SetEndCallback(stream, _CommonMusicEndCallback, (void*)m_nextSoundId);

        // 670
        SOUND_ASSERT(m_sounds.find( m_nextSoundId ) == m_sounds.end(), 670);
        // 671
        m_sounds[m_nextSoundId] = item;
        // 672
        return m_nextSoundId++;
    }

    // orig 0x5f50f0 CM3DSoundManager.cpp:690
    int CM3DSoundManager::AddSound(const char* filename, UserSoundType soundType, int group, int maxSounds,
                                   SoundPriority priority)
    {
        // 700
        if (group < 0 || group >= (int)m_soundGroupVector.size())
        {
            // 702-703: original M3D_CRITICAL_ERROR
            Log(CStr("Invalid group: ") + CStr(group) + CStr(" for sound '") + CStr(filename) + CStr("'"));
            SOUND_ASSERT(!"Critical error, see log", 703);
        }

        // 708-709
        if (!m_isInited)
        {
            return -1;
        }

        // 714
        unsigned int flags = (m_bitsPerSample == 16) ? FSOUND_16BITS : FSOUND_8BITS;

        // 724
        CSoundItem* item = 0;

        // 727
        switch (soundType)
        {
        case SND_TYPE_MUSIC:
        {
            // 734
            flags |= FSOUND_STEREO;
            FSOUND_STREAM* stream = pFMOD_INSTANCE->FSOUND_Stream_Open(filename, flags, 0, 0);
            // 735
            if (!stream)
            {
                // 738: original M3D_LOG_INFO
                Log(CStr("Cannot open sound stream from file ") + CStr(filename) + CStr(". Error: ") +
                    CStr(FMOD_ErrorString(pFMOD_INSTANCE->FSOUND_GetError())) + CStr(", flags: ") + CStr((int)flags));
                // 739
                return -1;
            }

            // 743
            FSOUND_SAMPLE* sample = pFMOD_INSTANCE->FSOUND_Stream_GetSample(stream);
            // 744
            if (sample)
            {
                // 746
                pFMOD_INSTANCE->FSOUND_Sample_SetDefaults(sample, -1, -1, -1, priority);
            }

            // 749
            item = new CSoundItem(FST_STREAM, stream, group);

            // 755
            pFMOD_INSTANCE->FSOUND_Stream_SetEndCallback(stream, _CommonMusicEndCallback, (void*)m_nextSoundId);
            // 775
            break;
        }

        case SND_TYPE_2DSOUND:
        case SND_TYPE_3DSOUND:
        {
            // 780
            if (soundType == SND_TYPE_2DSOUND)
            {
                // 785
                flags |= FSOUND_2D | FSOUND_STEREO;
            }

            // 796
            FSOUND_STREAM* stream = pFMOD_INSTANCE->FSOUND_Stream_Open(filename, flags, 0, 0);
            // 797
            if (!stream)
            {
                // 800: original M3D_LOG_INFO
                Log(CStr("Cannot open sound stream from file ") + CStr(filename) + CStr(". Error: ") +
                    CStr(FMOD_ErrorString(pFMOD_INSTANCE->FSOUND_GetError())) + CStr(", flags: ") + CStr((int)flags));
                // 801
                return -1;
            }

            // 804
            unsigned int mode = pFMOD_INSTANCE->FSOUND_Stream_GetMode(stream);
            // 805
            if (soundType == SND_TYPE_3DSOUND && !(mode & FSOUND_MONO))
            {
                // 807: original M3D_LOG_INFO
                Log(CStr("Warning: 3D sound must be mono: '") + CStr(filename) + CStr("'"));
            }

            // 813
            int lengthMs = pFMOD_INSTANCE->FSOUND_Stream_GetLengthMs(stream);

            FSOUND_SAMPLE* sample;
            // 815
            if (lengthMs > 10000 || (lengthMs > 1000 && soundType == SND_TYPE_2DSOUND))
            {
                // 817
                sample = pFMOD_INSTANCE->FSOUND_Stream_GetSample(stream);
                // 819
                item = new CSoundItem(FST_STREAM, stream, group);
            }
            else
            {
                // 823
                pFMOD_INSTANCE->FSOUND_Stream_Close(stream);

                // 826
                sample = pFMOD_INSTANCE->FSOUND_Sample_Load(FSOUND_UNMANAGED, filename, flags, 0, 0);
                // 827
                if (!sample)
                {
                    // 829: original M3D_LOG_INFO
                    Log(CStr("Cannot load sound sample from file ") + CStr(filename) + CStr(". Error: ") +
                        CStr(FMOD_ErrorString(pFMOD_INSTANCE->FSOUND_GetError())));
                    // 830
                    return -1;
                }

                // 839
                int frequency = 0;
                pFMOD_INSTANCE->FSOUND_Sample_GetDefaults(sample, &frequency, 0, 0, 0);
                // 841
                if (frequency != 22050)
                {
                    // 843: original M3D_LOG_INFO
                    Log(CStr("Warning: Invalid frequency: ") + CStr(frequency) + CStr(" for sample '") +
                        CStr(filename) + CStr("'"));
                }

                // 846
                item = new CSoundItem(FST_SAMPLE, sample, group);
            }

            // 850
            if (soundType == SND_TYPE_2DSOUND)
            {
                // 853
                pFMOD_INSTANCE->FSOUND_Sample_SetDefaults(sample, -1, -1, -1, priority);
                // 856
                pFMOD_INSTANCE->FSOUND_Sample_SetMaxPlaybacks(sample, maxSounds);
                // 858
            }
            else
            {
                // 861
                pFMOD_INSTANCE->FSOUND_Sample_SetMinMaxDistance(sample, m_soundGroupVector[group]->GetMinDist(),
                                                                m_soundGroupVector[group]->GetMaxDist());
                // 864
                pFMOD_INSTANCE->FSOUND_Sample_SetDefaults(sample, -1, -1, 127, priority);
            }
            break;
        }

        default:
            break;
        }

        // 877
        SOUND_ASSERT(m_sounds.find( m_nextSoundId ) == m_sounds.end(), 877);

        // 879
        m_sounds[m_nextSoundId] = item;

        // 885
        return m_nextSoundId++;
    }

    // orig 0x5f2b40 CM3DSoundManager.cpp:900
    int CM3DSoundManager::AddSound(const char* filename, UserSoundType soundType, const char* groupName,
                                   int maxSounds, SoundPriority priority)
    {
        // 901
        std::map<CStr, SoundGroup*>::iterator it = m_soundGroupMap.find(CStr(groupName));

        // 903
        if (it == m_soundGroupMap.end())
        {
            // 905: original M3D_LOG_INFO
            Log(CStr("Invalid group name: '") + CStr(groupName) + CStr("' for sound '") + CStr(filename) + CStr("'"));
            // 906
            return -1;
        }

        // 909
        return AddSound(filename, soundType, it->second->GetId(), maxSounds, priority);
    }

    // orig 0x5f47b0 CM3DSoundManager.cpp:919
    bool CM3DSoundManager::DeleteAllSounds()
    {
        bool res = true;

        // 923-924
        if (!m_isInited)
        {
            return false;
        }

        // 926: original M3D_LOG_INFO
        Log("\tStopping all sounds");

        // 929
        if (!pFMOD_INSTANCE->FSOUND_StopSound(FSOUND_ALL))
        {
            // 930
            res = false;
        }

        // 932: original M3D_LOG_INFO
        Log("\tDeleting all sounds from table");

        // 935
        for (std::map<int, CSoundItem*>::iterator it = m_sounds.begin(); it != m_sounds.end(); ++it)
        {
            // 937
            CSoundItem* item = it->second;
            // 938
            if (!item)
            {
                continue;
            }

            // 941
            switch (item->m_type)
            {
            case FST_SAMPLE:
                // 944
                pFMOD_INSTANCE->FSOUND_Sample_Free(item->m_pSample);
                break;

            case FST_STREAM:
                // 947
                if (!pFMOD_INSTANCE->FSOUND_Stream_Close(item->m_pStream))
                {
                    // 948
                    res = false;
                }
                // 949
                break;

            default:
                break;
            }

            // 955
            delete item;
        }

        // 959
        m_sounds.clear();

        // 964
        m_channels.assign(m_channels.size(), -1);

        // 966: original M3D_LOG_INFO
        Log("\tSounds deleted");

        // 968
        return res;
    }

    // orig 0x5f2f40 CM3DSoundManager.cpp:979
    bool CM3DSoundManager::DeleteIdTableSound(int soundId)
    {
        bool res = true;

        // 983-984
        if (!m_isInited)
        {
            return false;
        }

        // 986-987
        if (soundId == -1)
        {
            return false;
        }

        // 990
        std::map<int, CSoundItem*>::iterator it = m_sounds.find(soundId);
        // 991
        if (it == m_sounds.end())
        {
            // 993: original M3D_LOG_INFO
            Log("Error deleting sound: sound not found");
            // 994
            return false;
        }

        // 997
        CSoundItem* item = it->second;
        // 999-1000
        if (!item)
        {
            return false;
        }

        // 1004
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1006
            if (m_channels[i] == soundId)
            {
                // 1009
                pFMOD_INSTANCE->FSOUND_StopSound(i);
                // 1012
                _SetSoundIdForChannel(i, -1);
            }
        }

        // 1017
        switch (item->m_type)
        {
        case FST_SAMPLE:
            // 1020
            pFMOD_INSTANCE->FSOUND_Sample_Free(item->m_pSample);
            break;

        case FST_STREAM:
            // 1023
            if (!pFMOD_INSTANCE->FSOUND_Stream_Close(item->m_pStream))
            {
                // 1024
                res = false;
            }
            // 1025
            break;

        default:
            break;
        }

        // 1036
        delete it->second;
        it->second = 0;
        // 1037
        m_sounds.erase(it);

        // 1039
        return res;
    }

    // orig 0x5f30e0 CM3DSoundManager.cpp:1050
    bool CM3DSoundManager::StopGroup(int group)
    {
        // 1052-1053
        if (!m_isInited)
        {
            return false;
        }

        // 1056-1057
        if (group < 0 || group >= (int)m_soundGroupVector.size())
        {
            return false;
        }

        // 1060
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1063
            std::map<int, CSoundItem*>::iterator it = m_sounds.find(m_channels[i]);
            // 1064
            if (it == m_sounds.end())
            {
                continue;
            }

            // 1067
            CSoundItem* item = it->second;
            // 1068
            if (!item)
            {
                continue;
            }

            // 1072
            if (_bIsSoundGroupKindOf(item->m_group, group))
            {
                // 1074
                if (item->m_type == FST_STREAM)
                {
                    // 1076
                    pFMOD_INSTANCE->FSOUND_Stream_Stop(item->m_pStream);
                }
                // 1079
                pFMOD_INSTANCE->FSOUND_StopSound(i);
                // 1082
                _SetSoundIdForChannel(i, -1);
            }
        }

        // 1086
        return true;
    }

    // orig 0x5f4a10 CM3DSoundManager.cpp:1096
    int CM3DSoundManager::StopAllSounds()
    {
        // 1098-1099
        if (!m_isInited)
        {
            return 0;
        }

        // 1102
        pFMOD_INSTANCE->FSOUND_StopSound(FSOUND_ALL);

        // 1105
        m_channels.assign(m_channels.size(), -1);

        // 1107
        return 1;
    }

    // orig 0x5f31b0 CM3DSoundManager.cpp:1117
    int CM3DSoundManager::MuteAllSounds()
    {
        // 1118
        SOUND_ASSERT(!"obsolete", 1118);

        // 1121-1122
        if (!m_isInited)
        {
            return 0;
        }

        // 1125
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1129
            std::map<int, CSoundItem*>::iterator it = m_sounds.find(m_channels[i]);
            if (it == m_sounds.end())
            {
                continue;
            }

            // 1132
            CSoundItem* item = it->second;
            // 1133
            if (!item || !item->m_group)
            {
                continue;
            }

            // 1136
            pFMOD_INSTANCE->FSOUND_SetVolume(i, 0);
        }

        // 1141
        return 1;
    }

    // orig 0x5f3240 CM3DSoundManager.cpp:1151
    int CM3DSoundManager::RestoreAllVolumes()
    {
        // 1152
        SOUND_ASSERT(!"obsolete", 1152);

        // 1155-1156
        if (!m_isInited)
        {
            return 0;
        }

        // 1159
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1163
            std::map<int, CSoundItem*>::iterator it = m_sounds.find(m_channels[i]);
            if (it == m_sounds.end())
            {
                continue;
            }

            // 1165
            CSoundItem* item = it->second;
            // 1166
            if (!item)
            {
                continue;
            }

            // 1169
            pFMOD_INSTANCE->FSOUND_SetVolume(i, m_soundGroupVector[item->m_group]->GetVolume());
        }

        // 1174
        return 1;
    }

    // orig 0x5f32e0 CM3DSoundManager.cpp:1179
    bool CM3DSoundManager::PauseGroup(int groupId, bool pause)
    {
        // 1181-1182
        if (!m_isInited)
        {
            return false;
        }

        // 1185-1186
        if (groupId < 0 || groupId >= (int)m_soundGroupVector.size())
        {
            return false;
        }

        // 1189
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1192
            std::map<int, CSoundItem*>::iterator it = m_sounds.find(m_channels[i]);
            // 1193
            if (it == m_sounds.end())
            {
                continue;
            }

            // 1196
            CSoundItem* item = it->second;
            // 1197
            if (!item)
            {
                continue;
            }

            // 1201
            if (_bIsSoundGroupKindOf(item->m_group, groupId))
            {
                // 1203
                pFMOD_INSTANCE->FSOUND_SetPaused(i, pause);
            }
        }

        // 1207
        return true;
    }

    // orig 0x5f3390 CM3DSoundManager.cpp:1213
    bool CM3DSoundManager::PauseAllSounds(bool pause)
    {
        // 1215-1216
        if (!m_isInited)
        {
            return false;
        }

        // 1219
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1222
            std::map<int, CSoundItem*>::iterator it = m_sounds.find(m_channels[i]);
            // 1223
            if (it == m_sounds.end())
            {
                continue;
            }

            // 1227
            if (!it->second)
            {
                continue;
            }

            // 1231
            pFMOD_INSTANCE->FSOUND_SetPaused(i, pause);
        }

        // 1234
        return true;
    }

    // orig 0x5f3400 CM3DSoundManager.cpp:1240
    bool CM3DSoundManager::SetSoundPriority(int soundId, SoundPriority priority)
    {
        // 1242-1243
        if (!m_isInited)
        {
            return false;
        }

        // 1246
        std::map<int, CSoundItem*>::iterator it = m_sounds.find(soundId);
        // 1247-1248
        if (it == m_sounds.end())
        {
            return false;
        }

        // 1251
        CSoundItem* item = it->second;
        // 1252-1253
        if (!item)
        {
            return false;
        }

        // 1256
        switch (item->m_type)
        {
        case FST_SAMPLE:
            // 1264-1265
            if (!item->m_pSample)
            {
                return false;
            }
            // 1268
            pFMOD_INSTANCE->FSOUND_Sample_SetDefaults(item->m_pSample, -1, -1, -1, priority);
            break;

        case FST_STREAM:
        {
            // 1274-1275
            if (!item->m_pStream)
            {
                return false;
            }
            // 1278
            FSOUND_SAMPLE* sample = pFMOD_INSTANCE->FSOUND_Stream_GetSample(item->m_pStream);
            // 1279-1280
            if (!sample)
            {
                return false;
            }
            // 1283
            pFMOD_INSTANCE->FSOUND_Sample_SetDefaults(sample, -1, -1, -1, priority);
            // 1286: original M3D_LOG_INFO
            Log("WARNING: priority of music has been lowered");
            break;
        }

        default:
            // 1259
            SOUND_ASSERT(0, 1259);
            break;
        }

        // 1290
        return true;
    }

    // orig 0x5f3520 CM3DSoundManager.cpp:1296
    bool CM3DSoundManager::SetEndMusicCallback(int soundId, void (*endCallback)(int))
    {
        // 1297
        std::map<int, CSoundItem*>::iterator it = m_sounds.find(soundId);
        // 1298-1299
        if (it == m_sounds.end())
        {
            return false;
        }

        // 1301: the engine's callback is invoked with the id in ecx (see CSoundItem::m_endCallback)
        it->second->m_endCallback = (void(__fastcall*)(int))endCallback;
        // 1302
        return true;
    }

    // orig 0x5f0e90 CM3DSoundManager.cpp:1367
    int CM3DSoundManager::Update(double dT)
    {
        // 1369-1370
        if (!m_isInited)
        {
            return 0;
        }

        int result = -1;

        // 1374
        if (g_kernel->GetEngineCfg().m_mus_Enable.GetB())
        {
            // 1376
            if (m_bChangeFlag)
            {
                // 1378
                m_CurValue = (float)(m_CurValue - (double)m_MaxValue / m_musicFadeTime * dT);

                // 1381
                if (m_CurValue <= 0.0f)
                {
                    // 1383
                    result = PlayMusic(m_nNextMusic, m_bNextMusicMustBeLooped, true);
                    // 1384
                    m_bChangeFlag = false;
                }
                else
                {
                    // 1388
                    SetGroupVolume(SND_MUSIC_GROUP, (unsigned char)(int)m_CurValue);
                }
            }
        }

        // 1414
        pFMOD_INSTANCE->FSOUND_Update();

        // 1420
        return result;
    }

    // orig 0x5efdd0 CM3DSoundManager.cpp:1432
    bool CM3DSoundManager::IsChannelPlaying(int channelHandle)
    {
        // 1434-1435
        if (!m_isInited)
        {
            return false;
        }

        // 1437
        SOUND_ASSERT(channelHandle >= -1, 1437);

        // 1439-1440
        if (channelHandle == -1)
        {
            return false;
        }

        // 1443
        return pFMOD_INSTANCE->FSOUND_IsPlaying(channelHandle) != 0;
    }

    // orig 0x5efe30 CM3DSoundManager.cpp:1455
    bool CM3DSoundManager::IsMusicPlaying(int channelHandle)
    {
        // 1457-1458
        if (!m_isInited)
        {
            return false;
        }

        // 1461-1462
        if (m_bChangeFlag)
        {
            return true;
        }

        // 1464
        SOUND_ASSERT(channelHandle >= -1, 1464);

        // 1466-1467
        if (channelHandle == -1)
        {
            return false;
        }

        // 1470
        return pFMOD_INSTANCE->FSOUND_IsPlaying(channelHandle) != 0;
    }

    // orig 0x5efe90 CM3DSoundManager.cpp:1483
    bool CM3DSoundManager::SetPosition(int channelHandle, const CVector& position, const CVector& velocity)
    {
        // 1485-1486
        if (!m_isInited)
        {
            return false;
        }

        // 1488
        SOUND_ASSERT(channelHandle >= -1, 1488);

        // 1490-1491
        if (channelHandle == -1)
        {
            return false;
        }

        // 1495
        float pos[3] = { position.x, position.z, -position.y };
        // 1496
        float vel[3] = { velocity.x, velocity.z, -velocity.y };

        // 1499
        return pFMOD_INSTANCE->FSOUND_3D_SetAttributes(channelHandle, pos, vel) != 0;
    }

    // orig 0x5eff50 CM3DSoundManager.cpp:1512
    bool CM3DSoundManager::SetListenerPosition(const CVector& position, const CVector& velocity,
                                               const CVector& orientFront, const CVector& orientTop)
    {
        // 1514-1515
        if (!m_isInited)
        {
            return false;
        }

        // 1519
        float pos[3] = { position.x, position.z, -position.y };
        // 1520
        float vel[3] = { velocity.x, velocity.z, -velocity.y };
        // 1521
        float front[3] = { orientFront.x, orientFront.z, -orientFront.y };
        // 1522
        float top[3] = { orientTop.x, orientTop.z, -orientTop.y };

        // 1525
        pFMOD_INSTANCE->FSOUND_3D_Listener_SetAttributes(pos, vel, front[0], front[1], front[2], top[0], top[1],
                                                         top[2]);

        // 1527
        return true;
    }

    // orig 0x5f0020 CM3DSoundManager.cpp:1537
    bool CM3DSoundManager::StopChannel(int channelHandle)
    {
        // 1539-1540
        if (!m_isInited)
        {
            return false;
        }

        // 1542
        SOUND_ASSERT(channelHandle >= -1, 1542);

        // 1544-1545
        if (channelHandle == -1)
        {
            return false;
        }

        // 1548
        pFMOD_INSTANCE->FSOUND_StopSound(channelHandle);

        // 1553
        return true;
    }

    // orig 0x5f0070 CM3DSoundManager.cpp:1559
    void CM3DSoundManager::SetMaxVolume(int volume)
    {
        // 1560
        m_MaxValue = volume;
    }

    // orig 0x5f0080 CM3DSoundManager.cpp:1565
    void CM3DSoundManager::SetMusicFadeTime(float fadeTime)
    {
        // 1566
        m_musicFadeTime = fadeTime;
    }

    // orig 0x5f0090 CM3DSoundManager.cpp:1572
    unsigned char CM3DSoundManager::GetChannelVolume(int channelHandle)
    {
        // 1574-1575
        if (!m_isInited)
        {
            return 0;
        }

        // 1577
        SOUND_ASSERT(channelHandle >= -1, 1577);

        // 1579-1580
        if (channelHandle == -1)
        {
            return 0;
        }

        // 1582
        return (unsigned char)pFMOD_INSTANCE->FSOUND_GetVolume(channelHandle);
    }

    // orig 0x5f00e0 CM3DSoundManager.cpp:1588
    bool CM3DSoundManager::SetChannelVolume(int channelHandle, unsigned char volume, bool isAbsolute)
    {
        // 1589
        SOUND_ASSERT(!"not used", 1589);

        // 1592-1593
        if (!m_isInited)
        {
            return false;
        }

        // 1595
        SOUND_ASSERT(channelHandle >= -1, 1595);

        // 1597-1598
        if (channelHandle == -1)
        {
            return false;
        }

        // 1601
        if (isAbsolute)
        {
            // 1603
            return pFMOD_INSTANCE->FSOUND_SetVolumeAbsolute(channelHandle, volume) != 0;
        }

        // 1608
        return pFMOD_INSTANCE->FSOUND_SetVolume(channelHandle, volume) != 0;
    }

    // orig 0x5f0180 CM3DSoundManager.cpp:1615
    float CM3DSoundManager::GetCPUusage()
    {
        // 1616-1617
        if (!m_isInited)
        {
            return -1.0f;
        }
        // 1619
        return pFMOD_INSTANCE->FSOUND_GetCPUUsage();
    }

    // orig 0x5f01a0 CM3DSoundManager.cpp:1625
    void CM3DSoundManager::GetMemUsage(unsigned int& curAllocated, unsigned int& maxAllocated)
    {
        // 1626
        if (!m_isInited)
        {
            // 1628
            curAllocated = 0;
            // 1629
            maxAllocated = 0;
            return;
        }
        // 1634
        pFMOD_INSTANCE->FSOUND_GetMemoryStats(&curAllocated, &maxAllocated);
    }

    // orig 0x5f5d90 CM3DSoundManager.cpp:1640
    int CM3DSoundManager::GetSoundGroupId(int soundId)
    {
        // 1641-1642
        if (!m_isInited)
        {
            return -1;
        }

        // 1644
        if (m_sounds[soundId])
        {
            // 1646
            return m_sounds[soundId]->m_group;
        }

        // 1650
        return -1;
    }

    // orig 0x5f1ee0 CM3DSoundManager.cpp:1657
    bool CM3DSoundManager::GetGroupMinDist(int groupId, float& minDist)
    {
        // 1658-1659
        if (!m_isInited)
        {
            return false;
        }

        // 1661-1662
        if (groupId == -1)
        {
            return false;
        }

        // 1664
        SOUND_ASSERT(groupId >= 0, 1664);

        // 1666
        if (groupId < (int)m_soundGroupVector.size())
        {
            // 1668
            minDist = m_soundGroupVector[groupId]->GetMinDist();
            // 1669
            return true;
        }

        // 1673
        return false;
    }

    // orig 0x5f1f50 CM3DSoundManager.cpp:1680
    bool CM3DSoundManager::GetGroupMaxDist(int groupId, float& maxDist)
    {
        // 1681-1682
        if (!m_isInited)
        {
            return false;
        }

        // 1684-1685
        if (groupId == -1)
        {
            return false;
        }

        // 1687
        SOUND_ASSERT(groupId >= 0, 1687);

        // 1689
        if (groupId < (int)m_soundGroupVector.size())
        {
            // 1691
            maxDist = m_soundGroupVector[groupId]->GetMaxDist();
            // 1692
            return true;
        }

        // 1696
        return false;
    }

    // orig 0x5f4130 CM3DSoundManager.cpp:1703
    void CM3DSoundManager::DumpSoundInfo()
    {
        // 1704: original M3D_LOG_INFO
        Log("Channels playing now: ");

        // 1706
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1708
            if (IsChannelPlaying(i))
            {
                // 1710
                CStr logStr = CStr((int)i) + CStr(": ");

                // 1712
                int soundId = m_channels[i];
                // 1713
                std::map<int, CSoundItem*>::iterator it = m_sounds.find(soundId);
                // 1715
                if (it == m_sounds.end() || !it->second)
                {
                    // 1717
                    logStr += CStr("Invalid sound");
                    // 1719
                }
                else
                {
                    // 1721
                    logStr += CStr(soundId);
                }

                // 1727: original M3D_LOG_INFO
                Log(logStr);
            }
        }
    }

    // orig 0x5f1fc0 CM3DSoundManager.cpp:1735
    void CM3DSoundManager::_SetSoundIdForChannel(int channelNum, int soundId)
    {
        // 1737
        SOUND_ASSERT(( uint )channelNum < m_channels.size(), 1737);

        // 1774
        m_channels[channelNum] = soundId;
    }

    // orig 0x5f65a0 CM3DSoundManager.cpp:1780
    bool CM3DSoundManager::_LoadGroupsFromXmlFile(const char* groupsFileName)
    {
        // 1783
        CStr error;
        ref_ptr<m3d::cmn::XmlFile> xmlFile = m3d::ReadXmlFile(groupsFileName, &error);
        if (!xmlFile)
        {
            // 1786: original M3D_LOG_INFO
            Log(CStr("Error initing sound server: ") + error);
            // 1787
            return false;
        }

        // 1791
        ref_ptr<m3d::cmn::XmlNode> root = xmlFile->CreateNode();
        // 1792
        xmlFile->GetFirstChild(root, "SoundGroups");
        // 1793
        if (root->IsEmpty())
        {
            // 1795: original M3D_LOG_INFO
            Log("Load SoundServer: cannot find sound groups");
            // 1797
            return false;
        }

        // 1801
        ref_ptr<m3d::cmn::XmlNode> node = xmlFile->CreateNode();
        root->GetFirstChild(node, "group");
        while (!node->IsEmpty())
        {
            // 1802
            _LoadSoundGroupFromXml(xmlFile, node, 0);
            node->GetNextSibling(node, "group");
        }

        // 1806
        SOUND_ASSERT(m_soundGroupVector.size() >= 3, 1806);
        // 1807
        SOUND_ASSERT(m_soundGroupVector[ 0 ]->GetName() == "MUSIC", 1807);
        // 1808
        SOUND_ASSERT(m_soundGroupVector[ 1 ]->GetName() == "SOUND2D", 1808);
        // 1809
        SOUND_ASSERT(m_soundGroupVector[ 2 ]->GetName() == "SOUND3D", 1809);

        // 1811
        return true;
    }

    // orig 0x5f6240 CM3DSoundManager.cpp:1817
    void CM3DSoundManager::_LoadSoundGroupFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode,
                                                  SoundGroup* parent)
    {
        // 1818
        SoundGroup* soundGroup = new SoundGroup(parent);
        // 1820
        soundGroup->LoadFromXML(xmlFile, xmlNode);

        // 1822
        if (m_soundGroupMap.find(soundGroup->GetName()) != m_soundGroupMap.end())
        {
            // 1824: original M3D_LOG_INFO
            Log("Error: duplicate soundGroup name");
            return;
        }

        // 1828
        soundGroup->m_id = (int)m_soundGroupVector.size();
        // 1830
        m_soundGroupMap[soundGroup->GetName()] = soundGroup;
        // 1831
        m_soundGroupVector.push_back(soundGroup);

        // 1833
        if (!xmlNode->IsEmpty())
        {
            // 1835
            ref_ptr<m3d::cmn::XmlNode> child = xmlFile->CreateNode();
            xmlNode->GetFirstChild(child, "group");
            while (!child->IsEmpty())
            {
                // 1836
                _LoadSoundGroupFromXml(xmlFile, child, soundGroup);
                child->GetNextSibling(child, "group");
            }
        }
    }

    // orig 0x5f2010 CM3DSoundManager.cpp:1843
    bool CM3DSoundManager::_bIsSoundGroupKindOf(int soundGroupId, int ancestorId)
    {
        // 1844-1845
        if (soundGroupId < 0 || soundGroupId >= (int)m_soundGroupVector.size())
        {
            return false;
        }

        // 1847
        if (ancestorId < 0 || ancestorId >= (int)m_soundGroupVector.size())
        {
            return false;
        }

        // 1850
        while (soundGroupId != -1)
        {
            // 1852
            SOUND_ASSERT(soundGroupId >= 0, 1852);

            // 1854-1855
            if (soundGroupId == ancestorId)
            {
                return true;
            }

            // 1857
            soundGroupId = m_soundGroupVector[soundGroupId]->GetParentId();
        }

        // 1860
        return false;
    }

    // orig 0x5f3560 CM3DSoundManager.cpp:1866
    CSoundItem* CM3DSoundManager::_GetSoundById(int soundId)
    {
        // 1867
        std::map<int, CSoundItem*>::iterator it = m_sounds.find(soundId);
        // 1868
        if (it != m_sounds.end())
        {
            // 1869
            return it->second;
        }
        // 1871
        return 0;
    }

    // orig 0x5f20b0 CM3DSoundManager.cpp:1877
    void CM3DSoundManager::_PurgeChannels()
    {
        // 1878
        for (unsigned int i = 0; i < m_channels.size(); ++i)
        {
            // 1880
            if (!IsChannelPlaying(i))
            {
                // 1881
                _SetSoundIdForChannel(i, -1);
            }
        }
    }

    // orig 0x5f3590 CM3DSoundManager.cpp:1888
    signed char F_CALLBACKAPI CM3DSoundManager::_CommonMusicEndCallback(FSOUND_STREAM* stream, void* buff, int len,
                                                                        void* userdata)
    {
        // 1890
        m3d::AutoLock<m3d::CriticalSection> lock(m_musicEndCallbackCs);

        // 1892
        SOUND_ASSERT(!buff, 1892);
        // 1893
        SOUND_ASSERT(!len, 1893);
        // 1894
        SOUND_ASSERT(m_instance, 1894);

        // 1905
        int soundId = (int)userdata;

        // 1907
        std::map<int, CSoundItem*>& sounds = m_instance->m_sounds;
        // 1908
        std::map<int, CSoundItem*>::iterator soundIter = sounds.find(soundId);
        // 1909
        SOUND_ASSERT(soundIter != sounds.end(), 1909);

        // 1912
        CSoundItem* soundItem = soundIter->second;
        // 1913
        SOUND_ASSERT(soundItem->m_pStream == stream, 1913);

        // 1914
        if (soundItem->m_endCallback)
        {
            // 1916
            soundItem->m_endCallback(soundId);
        }

        // 1920
        return 0;
    }

    namespace
    {
        // orig 0x5f15b0 CM3DSoundManager.cpp:1927
        void* F_CALLBACKAPI OpenFileCallback(const char* fileName)
        {
            // 1932
            m3d::fs::FileStream* fp = g_kernel->GetFileServer().CreateFileStream();
            // 1933
            SOUND_ASSERT(fp, 1933);

            // 1935
            if (!fp->Open(fileName, m3d::fs::IStream::OPEN_READ))
            {
                // 1937
                delete fp;
                // 1938: original M3D_LOG_INFO
                Log(CStr("Error: Can't Open SoundFile: ") + CStr(fileName));
                // 1939
                return 0;
            }

            // 1946
            return fp;
        }

        // orig 0x5f01d0 CM3DSoundManager.cpp:1952
        int F_CALLBACKAPI ReadFileCallback(void* buffer, int size, void* handle)
        {
            // 1953
            SOUND_ASSERT(handle, 1953);

            // 1961
            return (int)((m3d::fs::FileStream*)handle)->ReadBytes(buffer, size);
        }

        // orig 0x5f0210 CM3DSoundManager.cpp:1973
        int F_CALLBACKAPI SeekFileCallback(void* handle, int pos, signed char mode)
        {
            // 1974
            SOUND_ASSERT(handle, 1974);

            // 1988
            return ((m3d::fs::FileStream*)handle)->FSeek(pos, mode);
        }

        // orig 0x5f0250 CM3DSoundManager.cpp:1994
        int F_CALLBACKAPI TellFileCallback(void* handle)
        {
            // 1995
            SOUND_ASSERT(handle, 1995);

            // 2003
            return (int)((m3d::fs::FileStream*)handle)->FTell();
        }

        // orig 0x5f0280 CM3DSoundManager.cpp:2015
        void F_CALLBACKAPI CloseFileCallback(void* handle)
        {
            // 2016
            SOUND_ASSERT(handle, 2016);

            // 2024
            ((m3d::fs::FileStream*)handle)->Close();
            // 2025
            delete (m3d::fs::FileStream*)handle;
        }
    }
}

namespace m3d
{
    // orig 0x5f69d0 CM3DSoundManager.cpp:2035
    snd::ISound* SoundFactory()
    {
        // 2036
        return new snd::CM3DSoundManager;
    }
}

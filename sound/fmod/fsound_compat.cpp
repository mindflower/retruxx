// retruxx adaptation (no original counterpart): the FMOD 3 calls of the sound driver, implemented
// over the FMOD Core API.
//
// The original loaded fmod.dll (FMOD 3.74) at run time and called it through the FMOD_INSTANCE
// table of fmoddyn.h. The driver here is built against the FMOD Core SDK instead, so this file
// gives FMOD_CreateInstance a table whose entries translate the FMOD 3 semantics the driver relies
// on (checked against the game's own fmod.dll where they are not documented):
//  - channels: the driver's channel count is a table kept here; FMOD Core is given more virtual
//    voices than that so it never steals a channel by itself. FSOUND_FREE takes the next free
//    channel round robin; when all are busy it steals the one with the lowest priority not above
//    the new sound's, the oldest among equals, and fails without an error when there is none.
//  - channel handles: FMOD 3 handed out ints that carry the channel index in the low 12 bits and a
//    reuse count above (the driver masks with 0xfff). The same encoding is produced here; an index
//    without a reuse count (the driver's own loop counters) addresses whatever the index holds.
//  - a stream plays on one channel at a time: FSOUND_Stream_PlayEx on a stream that is playing
//    returns the handle it has and does not restart it; FSOUND_Stream_SetPosition rewinds a playing
//    stream, and a stopped one starts from the beginning anyway.
//  - FSOUND_Sample_SetMaxPlaybacks: a play beyond the limit fails without an error.
//  - volumes 0..255 and priorities 0 (lowest) .. 255 become FMOD Core's 0..1 and 256 (lowest) .. 0;
//    a loaded sound's default priority is 255, as in FMOD 3.
//  - samples and streams are 3D unless opened with FSOUND_2D, and, as FMOD 3's software mixer did
//    not position stereo data, a stereo sound is played in 2D whatever its mode says.
//  - the stream end callback fires only when the stream reaches its end, not when it is stopped.
//  - before FSOUND_Init and after FSOUND_Close every call fails with FMOD_ERR_UNINITIALIZED, as in
//    FMOD 3; the driver closes FMOD before it frees its samples and streams.
//
// Everything that allocates lives in a State created by FSOUND_Init and destroyed by FSOUND_Close:
// the DLL's operator new reaches the executable's allocator only once createISound has run, so a
// static container that allocated in its constructor (at DLL load) would be freed through the
// wrong allocator when the executable unloads the DLL.
#include <fmod.h>

#include "fsound.h"

#include <map>
#include <set>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace
{
    // The FMOD 3 error codes (fsound_errors.h), under names that do not collide with FMOD Core's.
    enum Fsound3Error
    {
        F3_ERR_NONE,
        F3_ERR_BUSY,
        F3_ERR_UNINITIALIZED,
        F3_ERR_INIT,
        F3_ERR_ALLOCATED,
        F3_ERR_PLAY,
        F3_ERR_OUTPUT_FORMAT,
        F3_ERR_COOPERATIVELEVEL,
        F3_ERR_CREATEBUFFER,
        F3_ERR_FILE_NOTFOUND,
        F3_ERR_FILE_FORMAT,
        F3_ERR_FILE_BAD,
        F3_ERR_MEMORY,
        F3_ERR_VERSION,
        F3_ERR_INVALID_PARAM,
        F3_ERR_NO_EAX,
        F3_ERR_CHANNEL_ALLOC,
        F3_ERR_RECORD,
        F3_ERR_MEDIAPLAYER,
        F3_ERR_CDDEVICE,
    };

    // FMOD Core's virtual voice limit; the driver's channels are the table below, so this only has
    // to be larger than any channel count the driver asks for.
    const int kVirtualVoices = 4095;

    // The driver's channels: what each one plays, the reuse count that makes the handle, and what
    // the stealing rule needs.
    struct Channel
    {
        FMOD_CHANNEL* chan;
        FMOD_SOUND* sound;
        unsigned int gen;
        unsigned int seq;
        int priority;
    };

    // Per play, handed to the channel as user data: which sound it plays, for the end callback, and
    // whether the driver stopped it itself (then the end callback must not fire).
    struct PlayInfo
    {
        FMOD_SOUND* sound;
        bool stoppedByUser;
    };

    struct EndCallback
    {
        FSOUND_STREAMCALLBACK callback;
        void* userdata;
    };

    struct State
    {
        std::vector<Channel> channels;
        int nextSlot;
        unsigned int playSeq;
        std::map<FMOD_SOUND*, EndCallback> endCallbacks;
        std::map<FMOD_SOUND*, int> maxPlaybacks;
        std::set<FMOD_SOUND*> streams;

        State() : nextSlot(0), playSeq(0)
        {
        }
    };

    FMOD_SYSTEM* g_system = 0;
    State* g_state = 0;
    int g_numChannels = 0;
    int g_lastError = F3_ERR_NONE;

    FSOUND_OPENCALLBACK g_open = 0;
    FSOUND_CLOSECALLBACK g_close = 0;
    FSOUND_READCALLBACK g_read = 0;
    FSOUND_SEEKCALLBACK g_seek = 0;
    FSOUND_TELLCALLBACK g_tell = 0;

    int ToFsoundError(FMOD_RESULT result)
    {
        switch (result)
        {
        case FMOD_OK:
            return F3_ERR_NONE;
        case FMOD_ERR_INITIALIZED:
            return F3_ERR_BUSY;
        case FMOD_ERR_UNINITIALIZED:
            return F3_ERR_UNINITIALIZED;
        case FMOD_ERR_OUTPUT_INIT:
        case FMOD_ERR_OUTPUT_DRIVERCALL:
        case FMOD_ERR_OUTPUT_NODRIVERS:
            return F3_ERR_INIT;
        case FMOD_ERR_OUTPUT_ALLOCATED:
            return F3_ERR_ALLOCATED;
        case FMOD_ERR_OUTPUT_FORMAT:
            return F3_ERR_OUTPUT_FORMAT;
        case FMOD_ERR_OUTPUT_CREATEBUFFER:
            return F3_ERR_CREATEBUFFER;
        case FMOD_ERR_FILE_NOTFOUND:
            return F3_ERR_FILE_NOTFOUND;
        case FMOD_ERR_FORMAT:
        case FMOD_ERR_UNSUPPORTED:
            return F3_ERR_FILE_FORMAT;
        case FMOD_ERR_FILE_BAD:
        case FMOD_ERR_FILE_COULDNOTSEEK:
        case FMOD_ERR_FILE_EOF:
            return F3_ERR_FILE_BAD;
        case FMOD_ERR_MEMORY:
            return F3_ERR_MEMORY;
        case FMOD_ERR_VERSION:
            return F3_ERR_VERSION;
        case FMOD_ERR_INVALID_PARAM:
        case FMOD_ERR_INVALID_HANDLE:
            return F3_ERR_INVALID_PARAM;
        case FMOD_ERR_CHANNEL_ALLOC:
            return F3_ERR_CHANNEL_ALLOC;
        case FMOD_ERR_RECORD:
            return F3_ERR_RECORD;
        default:
            return F3_ERR_PLAY;
        }
    }

    bool Check(FMOD_RESULT result)
    {
        g_lastError = ToFsoundError(result);
        return result == FMOD_OK;
    }

    // Every FMOD 3 call first checked that FSOUND_Init had been called and otherwise failed with
    // FMOD_ERR_UNINITIALIZED. The driver relies on that at shutdown: it closes FMOD first and frees
    // its samples and streams afterwards, so after Close every sound and channel call must fail
    // instead of touching the released system.
    bool Initialised()
    {
        if (!g_system || !g_state)
        {
            g_lastError = F3_ERR_UNINITIALIZED;
            return false;
        }
        return true;
    }

    // The loop bits of an FMOD 3 mode as FMOD Core loop bits; none set means loop off, as in FMOD 3.
    FMOD_MODE LoopBits(unsigned int mode)
    {
        if (mode & FSOUND_LOOP_NORMAL)
        {
            return FMOD_LOOP_NORMAL;
        }
        if (mode & FSOUND_LOOP_BIDI)
        {
            return FMOD_LOOP_BIDI;
        }
        return FMOD_LOOP_OFF;
    }

    FMOD_MODE ToCoreMode(unsigned int mode)
    {
        return LoopBits(mode) | ((mode & FSOUND_2D) ? FMOD_2D : FMOD_3D);
    }

    // The FMOD 3 view of a sound's mode, with the sample format FMOD 3 reported for loaded data.
    unsigned int ToFsoundMode(FMOD_SOUND* sound)
    {
        FMOD_MODE core = 0;
        FMOD_Sound_GetMode(sound, &core);
        unsigned int mode = 0;
        if (core & FMOD_LOOP_NORMAL)
        {
            mode |= FSOUND_LOOP_NORMAL;
        }
        else if (core & FMOD_LOOP_BIDI)
        {
            mode |= FSOUND_LOOP_BIDI;
        }
        else
        {
            mode |= FSOUND_LOOP_OFF;
        }
        if (core & FMOD_2D)
        {
            mode |= FSOUND_2D;
        }
        int channels = 0;
        int bits = 0;
        if (FMOD_Sound_GetFormat(sound, 0, 0, &channels, &bits) == FMOD_OK)
        {
            mode |= (channels == 1) ? FSOUND_MONO : FSOUND_STEREO;
            mode |= (bits == 8) ? FSOUND_8BITS : FSOUND_16BITS;
        }
        return mode;
    }

    // Stereo data is not positioned, as in FMOD 3's software mixer.
    void DemoteStereoTo2D(FMOD_SOUND* sound)
    {
        FMOD_MODE core = 0;
        int channels = 0;
        if (FMOD_Sound_GetMode(sound, &core) == FMOD_OK && (core & FMOD_3D) &&
            FMOD_Sound_GetFormat(sound, 0, 0, &channels, 0) == FMOD_OK && channels != 1)
        {
            FMOD_Sound_SetMode(sound, (core & ~FMOD_3D) | FMOD_2D);
        }
    }

    // FMOD 3 priority (0 lowest .. 255 highest) <-> FMOD Core priority (256 lowest .. 0 highest).
    int ToCorePriority(int defpri)
    {
        return 256 - (defpri < 0 ? 0 : (defpri > 255 ? 255 : defpri));
    }

    int ToFsoundPriority(int priority)
    {
        int defpri = 256 - priority;
        return defpri < 0 ? 0 : (defpri > 255 ? 255 : defpri);
    }

    int PriorityOf(FMOD_SOUND* sound)
    {
        float frequency = 0.0f;
        int priority = 128;
        FMOD_Sound_GetDefaults(sound, &frequency, &priority);
        return ToFsoundPriority(priority);
    }

    FMOD_SOUND* CreateSound(const char* name, unsigned int mode, int offset, int length, FMOD_MODE kind)
    {
        if (!Initialised())
        {
            return 0;
        }
        FMOD_CREATESOUNDEXINFO exinfo;
        memset(&exinfo, 0, sizeof(exinfo));
        exinfo.cbsize = sizeof(exinfo);
        exinfo.fileoffset = (unsigned int)offset;
        exinfo.length = (unsigned int)length;
        FMOD_SOUND* sound = 0;
        if (!Check(FMOD_System_CreateSound(g_system, name, kind | ToCoreMode(mode), (offset || length) ? &exinfo : 0,
                                           &sound)))
        {
            return 0;
        }
        DemoteStereoTo2D(sound);
        // FMOD 3 loaded every sound with the highest priority.
        float frequency = 0.0f;
        int priority = 0;
        if (FMOD_Sound_GetDefaults(sound, &frequency, &priority) == FMOD_OK)
        {
            FMOD_Sound_SetDefaults(sound, frequency, ToCorePriority(255));
        }
        if (kind & FMOD_CREATESTREAM)
        {
            g_state->streams.insert(sound);
        }
        return sound;
    }

    // --- channels ----------------------------------------------------------------------------

    int MakeHandle(int index, unsigned int gen)
    {
        return (int)((gen << 12) | (unsigned int)index);
    }

    // The driver's channel behind a handle, or 0 when the handle is stale or unknown.
    Channel* SlotFor(int handle)
    {
        if (!Initialised() || handle < 0)
        {
            return 0;
        }
        std::vector<Channel>& channels = g_state->channels;
        unsigned int index = (unsigned int)handle & 0xfff;
        unsigned int gen = (unsigned int)handle >> 12;
        if (index >= channels.size())
        {
            return 0;
        }
        if (gen != 0 && gen != channels[index].gen)
        {
            return 0;
        }
        return &channels[index];
    }

    FMOD_CHANNEL* Resolve(int handle)
    {
        Channel* slot = SlotFor(handle);
        return slot ? slot->chan : 0;
    }

    bool Busy(const Channel& c)
    {
        FMOD_BOOL playing = 0;
        return c.chan && FMOD_Channel_IsPlaying(c.chan, &playing) == FMOD_OK && playing;
    }

    // The channel a sound is playing on, or -1; a stream has at most one.
    int SlotOf(FMOD_SOUND* sound)
    {
        std::vector<Channel>& channels = g_state->channels;
        for (size_t i = 0; i < channels.size(); ++i)
        {
            if (channels[i].sound == sound && Busy(channels[i]))
            {
                return (int)i;
            }
        }
        return -1;
    }

    PlayInfo* InfoOf(FMOD_CHANNEL* chan)
    {
        void* userdata = 0;
        if (chan && FMOD_Channel_GetUserData(chan, &userdata) == FMOD_OK)
        {
            return (PlayInfo*)userdata;
        }
        return 0;
    }

    FMOD_RESULT F_CALL ChannelCallback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype,
                                       FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void*, void*)
    {
        if (controltype != FMOD_CHANNELCONTROL_CHANNEL || callbacktype != FMOD_CHANNELCONTROL_CALLBACK_END)
        {
            return FMOD_OK;
        }
        FMOD_CHANNEL* chan = (FMOD_CHANNEL*)channelcontrol;
        PlayInfo* info = InfoOf(chan);
        if (!info)
        {
            return FMOD_OK;
        }
        FMOD_Channel_SetUserData(chan, 0);
        if (!info->stoppedByUser && g_state)
        {
            std::map<FMOD_SOUND*, EndCallback>::iterator it = g_state->endCallbacks.find(info->sound);
            if (it != g_state->endCallbacks.end() && it->second.callback)
            {
                it->second.callback((FSOUND_STREAM*)info->sound, 0, 0, it->second.userdata);
            }
        }
        delete info;
        return FMOD_OK;
    }

    void MarkStoppedByUser(FMOD_CHANNEL* chan)
    {
        PlayInfo* info = InfoOf(chan);
        if (info)
        {
            info->stoppedByUser = true;
        }
    }

    // Stops what a channel plays, without the end callback.
    void StopSlot(Channel& c)
    {
        if (c.chan)
        {
            MarkStoppedByUser(c.chan);
            FMOD_Channel_Stop(c.chan);
            c.chan = 0;
        }
    }

    void StopSound(FMOD_SOUND* sound)
    {
        std::vector<Channel>& channels = g_state->channels;
        for (size_t i = 0; i < channels.size(); ++i)
        {
            if (channels[i].sound == sound)
            {
                StopSlot(channels[i]);
            }
        }
    }

    // FSOUND_PlaySoundEx / FSOUND_Stream_PlayEx with FSOUND_FREE.
    int Play(FMOD_SOUND* sound, signed char startpaused)
    {
        if (!Initialised() || !sound)
        {
            return -1;
        }
        State& s = *g_state;
        std::vector<Channel>& channels = s.channels;

        // A stream plays on one channel at a time: playing it again hands out the channel it has.
        if (s.streams.count(sound))
        {
            int slot = SlotOf(sound);
            if (slot >= 0)
            {
                return MakeHandle(slot, channels[slot].gen);
            }
        }

        // FSOUND_Sample_SetMaxPlaybacks: at the limit the play is refused, without an error.
        std::map<FMOD_SOUND*, int>::iterator mp = s.maxPlaybacks.find(sound);
        if (mp != s.maxPlaybacks.end() && mp->second > 0)
        {
            int playing = 0;
            for (size_t i = 0; i < channels.size(); ++i)
            {
                if (channels[i].sound == sound && Busy(channels[i]))
                {
                    ++playing;
                }
            }
            if (playing >= mp->second)
            {
                return -1;
            }
        }

        int n = (int)channels.size();
        int priority = PriorityOf(sound);
        int slot = -1;

        // The next free channel, round robin as FMOD 3 allocated them.
        for (int k = 0; k < n && slot < 0; ++k)
        {
            int i = (s.nextSlot + k) % n;
            if (!Busy(channels[i]))
            {
                slot = i;
            }
        }

        // All busy: steal the lowest priority not above the new sound's, the oldest among equals;
        // none means the play is refused, without an error.
        if (slot < 0)
        {
            for (int i = 0; i < n; ++i)
            {
                const Channel& c = channels[i];
                if (c.priority > priority)
                {
                    continue;
                }
                if (slot < 0 || c.priority < channels[slot].priority ||
                    (c.priority == channels[slot].priority && c.seq < channels[slot].seq))
                {
                    slot = i;
                }
            }
            if (slot < 0)
            {
                return -1;
            }
        }

        Channel& c = channels[slot];
        StopSlot(c);
        FMOD_CHANNEL* chan = 0;
        if (!Check(FMOD_System_PlaySound(g_system, sound, 0, startpaused ? 1 : 0, &chan)))
        {
            return -1;
        }
        PlayInfo* info = new PlayInfo;
        info->sound = sound;
        info->stoppedByUser = false;
        FMOD_Channel_SetUserData(chan, info);
        FMOD_Channel_SetCallback(chan, ChannelCallback);
        c.chan = chan;
        c.sound = sound;
        c.priority = priority;
        c.seq = ++s.playSeq;
        c.gen = c.gen + 1;
        if (c.gen >= (1u << 19))
        {
            c.gen = 1;
        }
        s.nextSlot = (slot + 1) % n;
        return MakeHandle(slot, c.gen);
    }

    // --- the file callbacks of FMOD 3, forwarded to FMOD Core ------------------------------

    FMOD_RESULT F_CALL FileOpen(const char* name, unsigned int* filesize, void** handle, void*)
    {
        void* h = g_open ? g_open(name) : 0;
        if (!h)
        {
            return FMOD_ERR_FILE_NOTFOUND;
        }
        g_seek(h, 0, 2);
        *filesize = (unsigned int)g_tell(h);
        g_seek(h, 0, 0);
        *handle = h;
        return FMOD_OK;
    }

    FMOD_RESULT F_CALL FileClose(void* handle, void*)
    {
        g_close(handle);
        return FMOD_OK;
    }

    FMOD_RESULT F_CALL FileRead(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread, void*)
    {
        int n = g_read(buffer, (int)sizebytes, handle);
        *bytesread = n < 0 ? 0 : (unsigned int)n;
        return *bytesread < sizebytes ? FMOD_ERR_FILE_EOF : FMOD_OK;
    }

    FMOD_RESULT F_CALL FileSeek(void* handle, unsigned int pos, void*)
    {
        g_seek(handle, (int)pos, 0);
        return FMOD_OK;
    }

    // --- FSOUND_* -----------------------------------------------------------------------

    signed char F_API Init(int mixrate, int maxsoftwarechannels, unsigned int)
    {
        if (g_system)
        {
            g_lastError = F3_ERR_BUSY;
            return 0;
        }
        if (!Check(FMOD_System_Create(&g_system, FMOD_VERSION)))
        {
            g_system = 0;
            return 0;
        }
        // The driver's channels are the table in State; the real voices get a little headroom so
        // that a channel stolen and replayed in the same call never starts virtual while the old
        // one drains.
        if (!Check(FMOD_System_SetSoftwareFormat(g_system, mixrate, FMOD_SPEAKERMODE_DEFAULT, 0)) ||
            !Check(FMOD_System_SetSoftwareChannels(g_system, maxsoftwarechannels + 4)) ||
            !Check(FMOD_System_Init(g_system, kVirtualVoices, FMOD_INIT_NORMAL, 0)))
        {
            FMOD_System_Release(g_system);
            g_system = 0;
            return 0;
        }
        if (g_open)
        {
            FMOD_System_SetFileSystem(g_system, FileOpen, FileClose, FileRead, FileSeek, 0, 0, -1);
        }
        g_state = new State;
        Channel empty = { 0, 0, 0, 0, 0 };
        g_state->channels.assign((size_t)maxsoftwarechannels, empty);
        g_numChannels = maxsoftwarechannels;
        return 1;
    }

    void F_API Close(void)
    {
        if (!g_system)
        {
            return;
        }
        if (g_state)
        {
            // Releasing the system stops every channel; the per-play data goes here, so an end
            // callback raised by the release finds nothing to call.
            std::vector<Channel>& channels = g_state->channels;
            for (size_t i = 0; i < channels.size(); ++i)
            {
                PlayInfo* info = InfoOf(channels[i].chan);
                if (info)
                {
                    FMOD_Channel_SetUserData(channels[i].chan, 0);
                    delete info;
                }
            }
        }
        // Releasing the system also releases every sound; the driver frees its samples and streams
        // after this, and those calls fail with FMOD_ERR_UNINITIALIZED as they did in FMOD 3.
        FMOD_System_Release(g_system);
        g_system = 0;
        delete g_state;
        g_state = 0;
        g_numChannels = 0;
    }

    void F_API Update(void)
    {
        if (g_system)
        {
            FMOD_System_Update(g_system);
        }
    }

    void F_API File_SetCallbacks(FSOUND_OPENCALLBACK useropen, FSOUND_CLOSECALLBACK userclose,
                                 FSOUND_READCALLBACK userread, FSOUND_SEEKCALLBACK userseek,
                                 FSOUND_TELLCALLBACK usertell)
    {
        g_open = useropen;
        g_close = userclose;
        g_read = userread;
        g_seek = userseek;
        g_tell = usertell;
        if (g_system)
        {
            FMOD_System_SetFileSystem(g_system, FileOpen, FileClose, FileRead, FileSeek, 0, 0, -1);
        }
    }

    int F_API GetError(void)
    {
        return g_lastError;
    }

    int F_API GetMaxChannels(void)
    {
        return g_numChannels;
    }

    float F_API GetCPUUsage(void)
    {
        FMOD_CPU_USAGE usage;
        memset(&usage, 0, sizeof(usage));
        if (g_system && Check(FMOD_System_GetCPUUsage(g_system, &usage)))
        {
            return usage.dsp + usage.stream + usage.geometry + usage.update;
        }
        return 0.0f;
    }

    void F_API GetMemoryStats(unsigned int* currentalloced, unsigned int* maxalloced)
    {
        int current = 0;
        int max = 0;
        Check(FMOD_Memory_GetStats(&current, &max, 0));
        if (currentalloced)
        {
            *currentalloced = (unsigned int)current;
        }
        if (maxalloced)
        {
            *maxalloced = (unsigned int)max;
        }
    }

    FSOUND_SAMPLE* F_API Sample_Load(int, const char* name_or_data, unsigned int mode, int offset, int length)
    {
        return (FSOUND_SAMPLE*)CreateSound(name_or_data, mode, offset, length, FMOD_CREATESAMPLE);
    }

    void FreeSound(FMOD_SOUND* sound)
    {
        StopSound(sound);
        g_state->endCallbacks.erase(sound);
        g_state->maxPlaybacks.erase(sound);
        g_state->streams.erase(sound);
        Check(FMOD_Sound_Release(sound));
    }

    void F_API Sample_Free(FSOUND_SAMPLE* sptr)
    {
        if (Initialised() && sptr)
        {
            FreeSound((FMOD_SOUND*)sptr);
        }
    }

    signed char F_API Sample_SetMode(FSOUND_SAMPLE* sptr, unsigned int mode)
    {
        if (!Initialised() || !sptr)
        {
            return 0;
        }
        FMOD_SOUND* sound = (FMOD_SOUND*)sptr;
        FMOD_MODE core = 0;
        FMOD_Sound_GetMode(sound, &core);
        core &= ~(FMOD_LOOP_OFF | FMOD_LOOP_NORMAL | FMOD_LOOP_BIDI | FMOD_2D | FMOD_3D);
        core |= ToCoreMode(mode);
        if (!Check(FMOD_Sound_SetMode(sound, core)))
        {
            return 0;
        }
        DemoteStereoTo2D(sound);
        return 1;
    }

    signed char F_API Sample_SetDefaults(FSOUND_SAMPLE* sptr, int deffreq, int, int, int defpri)
    {
        if (!Initialised() || !sptr)
        {
            return 0;
        }
        FMOD_SOUND* sound = (FMOD_SOUND*)sptr;
        float frequency = 0.0f;
        int priority = 128;
        FMOD_Sound_GetDefaults(sound, &frequency, &priority);
        if (deffreq >= 0)
        {
            frequency = (float)deffreq;
        }
        if (defpri >= 0)
        {
            priority = ToCorePriority(defpri);
        }
        return Check(FMOD_Sound_SetDefaults(sound, frequency, priority)) ? 1 : 0;
    }

    signed char F_API Sample_SetMinMaxDistance(FSOUND_SAMPLE* sptr, float min, float max)
    {
        return Initialised() && sptr && Check(FMOD_Sound_Set3DMinMaxDistance((FMOD_SOUND*)sptr, min, max)) ? 1 : 0;
    }

    signed char F_API Sample_SetMaxPlaybacks(FSOUND_SAMPLE* sptr, int max)
    {
        if (!Initialised() || !sptr)
        {
            return 0;
        }
        if (max > 0)
        {
            g_state->maxPlaybacks[(FMOD_SOUND*)sptr] = max;
        }
        else
        {
            g_state->maxPlaybacks.erase((FMOD_SOUND*)sptr);
        }
        return 1;
    }

    signed char F_API Sample_GetDefaults(FSOUND_SAMPLE* sptr, int* deffreq, int* defvol, int* defpan, int* defpri)
    {
        if (!Initialised() || !sptr)
        {
            return 0;
        }
        float frequency = 0.0f;
        int priority = 128;
        if (!Check(FMOD_Sound_GetDefaults((FMOD_SOUND*)sptr, &frequency, &priority)))
        {
            return 0;
        }
        if (deffreq)
        {
            *deffreq = (int)frequency;
        }
        if (defvol)
        {
            *defvol = 255;
        }
        if (defpan)
        {
            *defpan = 128;
        }
        if (defpri)
        {
            *defpri = ToFsoundPriority(priority);
        }
        return 1;
    }

    unsigned int F_API Sample_GetMode(FSOUND_SAMPLE* sptr)
    {
        return Initialised() && sptr ? ToFsoundMode((FMOD_SOUND*)sptr) : 0;
    }

    int F_API PlaySoundEx(int, FSOUND_SAMPLE* sptr, FSOUND_DSPUNIT*, signed char startpaused)
    {
        return Play((FMOD_SOUND*)sptr, startpaused);
    }

    signed char F_API StopSound(int channel)
    {
        if (!Initialised())
        {
            return 0;
        }
        if (channel == FSOUND_ALL)
        {
            std::vector<Channel>& channels = g_state->channels;
            for (size_t i = 0; i < channels.size(); ++i)
            {
                StopSlot(channels[i]);
            }
            return 1;
        }
        Channel* slot = SlotFor(channel);
        if (!slot || !slot->chan)
        {
            return 0;
        }
        StopSlot(*slot);
        return 1;
    }

    signed char F_API SetFrequency(int channel, int freq)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        return chan && Check(FMOD_Channel_SetFrequency(chan, (float)freq)) ? 1 : 0;
    }

    signed char F_API SetVolume(int channel, int vol)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        float volume = (vol < 0 ? 0 : (vol > 255 ? 255 : vol)) / 255.0f;
        return chan && Check(FMOD_Channel_SetVolume(chan, volume)) ? 1 : 0;
    }

    signed char F_API SetPaused(int channel, signed char paused)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        return chan && Check(FMOD_Channel_SetPaused(chan, paused ? 1 : 0)) ? 1 : 0;
    }

    signed char F_API SetLoopMode(int channel, unsigned int loopmode)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        return chan && Check(FMOD_Channel_SetMode(chan, LoopBits(loopmode))) ? 1 : 0;
    }

    signed char F_API Set3DAttributes(int channel, const float* pos, const float* vel)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        return chan && Check(FMOD_Channel_Set3DAttributes(chan, (const FMOD_VECTOR*)pos, (const FMOD_VECTOR*)vel))
                   ? 1
                   : 0;
    }

    signed char F_API IsPlaying(int channel)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        FMOD_BOOL playing = 0;
        return chan && FMOD_Channel_IsPlaying(chan, &playing) == FMOD_OK && playing ? 1 : 0;
    }

    int F_API GetFrequency(int channel)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        float frequency = 0.0f;
        return chan && Check(FMOD_Channel_GetFrequency(chan, &frequency)) ? (int)frequency : 0;
    }

    int F_API GetVolume(int channel)
    {
        FMOD_CHANNEL* chan = Resolve(channel);
        float volume = 0.0f;
        return chan && Check(FMOD_Channel_GetVolume(chan, &volume)) ? (int)(volume * 255.0f + 0.5f) : 0;
    }

    void F_API Listener_SetAttributes(const float* pos, const float* vel, float fx, float fy, float fz, float tx,
                                      float ty, float tz)
    {
        if (!Initialised())
        {
            return;
        }
        FMOD_VECTOR forward = { fx, fy, fz };
        FMOD_VECTOR up = { tx, ty, tz };
        Check(FMOD_System_Set3DListenerAttributes(g_system, 0, (const FMOD_VECTOR*)pos, (const FMOD_VECTOR*)vel,
                                                  &forward, &up));
    }

    FSOUND_STREAM* F_API Stream_Open(const char* name_or_data, unsigned int mode, int offset, int length)
    {
        return (FSOUND_STREAM*)CreateSound(name_or_data, mode, offset, length, FMOD_CREATESTREAM);
    }

    signed char F_API Stream_Close(FSOUND_STREAM* stream)
    {
        if (!Initialised() || !stream)
        {
            return 0;
        }
        FreeSound((FMOD_SOUND*)stream);
        return g_lastError == F3_ERR_NONE ? 1 : 0;
    }

    int F_API Stream_PlayEx(int, FSOUND_STREAM* stream, FSOUND_DSPUNIT*, signed char startpaused)
    {
        return Play((FMOD_SOUND*)stream, startpaused);
    }

    signed char F_API Stream_Stop(FSOUND_STREAM* stream)
    {
        if (!Initialised() || !stream)
        {
            return 0;
        }
        StopSound((FMOD_SOUND*)stream);
        return 1;
    }

    // FMOD 3 positions in bytes of PCM data; a stream that is not playing starts from the beginning
    // on its next play anyway, which is the only position the driver asks for.
    signed char F_API Stream_SetPosition(FSOUND_STREAM* stream, unsigned int position)
    {
        if (!Initialised() || !stream)
        {
            return 0;
        }
        int slot = SlotOf((FMOD_SOUND*)stream);
        if (slot < 0)
        {
            return position == 0 ? 1 : 0;
        }
        return Check(FMOD_Channel_SetPosition(g_state->channels[slot].chan, position, FMOD_TIMEUNIT_PCMBYTES)) ? 1
                                                                                                                : 0;
    }

    int F_API Stream_GetLengthMs(FSOUND_STREAM* stream)
    {
        unsigned int length = 0;
        return Initialised() && stream && Check(FMOD_Sound_GetLength((FMOD_SOUND*)stream, &length, FMOD_TIMEUNIT_MS))
                   ? (int)length
                   : 0;
    }

    signed char F_API Stream_SetMode(FSOUND_STREAM* stream, unsigned int mode)
    {
        return Sample_SetMode((FSOUND_SAMPLE*)stream, mode);
    }

    unsigned int F_API Stream_GetMode(FSOUND_STREAM* stream)
    {
        return Sample_GetMode((FSOUND_SAMPLE*)stream);
    }

    FSOUND_SAMPLE* F_API Stream_GetSample(FSOUND_STREAM* stream)
    {
        return (FSOUND_SAMPLE*)stream;
    }

    signed char F_API Stream_SetEndCallback(FSOUND_STREAM* stream, FSOUND_STREAMCALLBACK callback, void* userdata)
    {
        if (!Initialised() || !stream)
        {
            return 0;
        }
        EndCallback entry = { callback, userdata };
        g_state->endCallbacks[(FMOD_SOUND*)stream] = entry;
        return 1;
    }
}

FMOD_INSTANCE* FMOD_CreateInstance(char*)
{
    FMOD_INSTANCE* instance = (FMOD_INSTANCE*)calloc(sizeof(FMOD_INSTANCE), 1);
    if (!instance)
    {
        return 0;
    }
    instance->FSOUND_Init = Init;
    instance->FSOUND_Close = Close;
    instance->FSOUND_Update = Update;
    instance->FSOUND_File_SetCallbacks = File_SetCallbacks;
    instance->FSOUND_GetError = GetError;
    instance->FSOUND_GetMaxChannels = GetMaxChannels;
    instance->FSOUND_GetCPUUsage = GetCPUUsage;
    instance->FSOUND_GetMemoryStats = GetMemoryStats;
    instance->FSOUND_Sample_Load = Sample_Load;
    instance->FSOUND_Sample_Free = Sample_Free;
    instance->FSOUND_Sample_SetMode = Sample_SetMode;
    instance->FSOUND_Sample_SetDefaults = Sample_SetDefaults;
    instance->FSOUND_Sample_SetMinMaxDistance = Sample_SetMinMaxDistance;
    instance->FSOUND_Sample_SetMaxPlaybacks = Sample_SetMaxPlaybacks;
    instance->FSOUND_Sample_GetDefaults = Sample_GetDefaults;
    instance->FSOUND_Sample_GetMode = Sample_GetMode;
    instance->FSOUND_PlaySoundEx = PlaySoundEx;
    instance->FSOUND_StopSound = StopSound;
    instance->FSOUND_SetFrequency = SetFrequency;
    instance->FSOUND_SetVolume = SetVolume;
    instance->FSOUND_SetVolumeAbsolute = SetVolume;
    instance->FSOUND_SetPaused = SetPaused;
    instance->FSOUND_SetLoopMode = SetLoopMode;
    instance->FSOUND_3D_SetAttributes = Set3DAttributes;
    instance->FSOUND_IsPlaying = IsPlaying;
    instance->FSOUND_GetFrequency = GetFrequency;
    instance->FSOUND_GetVolume = GetVolume;
    instance->FSOUND_3D_Listener_SetAttributes = Listener_SetAttributes;
    instance->FSOUND_Stream_Open = Stream_Open;
    instance->FSOUND_Stream_Close = Stream_Close;
    instance->FSOUND_Stream_PlayEx = Stream_PlayEx;
    instance->FSOUND_Stream_Stop = Stream_Stop;
    instance->FSOUND_Stream_SetPosition = Stream_SetPosition;
    instance->FSOUND_Stream_GetLengthMs = Stream_GetLengthMs;
    instance->FSOUND_Stream_SetMode = Stream_SetMode;
    instance->FSOUND_Stream_GetMode = Stream_GetMode;
    instance->FSOUND_Stream_GetSample = Stream_GetSample;
    instance->FSOUND_Stream_SetEndCallback = Stream_SetEndCallback;
    return instance;
}

void FMOD_FreeInstance(FMOD_INSTANCE* instance)
{
    if (instance)
    {
        free(instance);
    }
}

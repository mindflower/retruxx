#pragma once
#include <core/stringm3d.h>

#include <cstdint>

struct CVector;

struct IBase
{
    virtual ~IBase() = default;
    virtual int DecRef() = 0;
    virtual int IncRef() = 0;
    virtual void* QueryIface(char const*) = 0;
};

namespace m3d
{
    class Kernel;

    struct IDeviceResetCallback
    {
        ;
        virtual char const* GetCallbackName() const = 0;
        virtual void OnBeforeDeviceReset() = 0;
        virtual void OnAfterDeviceReset() = 0;
    };

    namespace input
    {
        enum DeviceParam : int32_t
        {
            DP_MOUSE_X = 0x0,
            DP_MOUSE_Y = 0x1,
            DP_MOUSE_Z = 0x2,
            DP_MOUSE_B = 0x3,
            DP_JOY_X = 0x4,
            DP_JOY_Y = 0x5,
            DP_JOY_Z = 0x6,
            DP_JOY_RX = 0x7,
            DP_JOY_RY = 0x8,
            DP_JOY_RZ = 0x9,
            DP_JOY_B = 0xA,
            DP_JOY_S0 = 0xB,
            DP_JOY_S1 = 0xC,
            DP_JOY_P0 = 0xD,
            DP_JOY_P1 = 0xE,
            DP_JOY_P2 = 0xF,
            DP_JOY_P3 = 0x10,
            DP_NUM_PARAMS = 0x11,
        };

        enum Language : int32_t
        {
            LANGUAGE_BASE = 0x0,
            LANGUAGE_ADDITIONAL = 0x1,
        };

        struct IInput : public IBase
        {
            virtual int Init(m3d::Kernel*, void (*)(CStr const&)) = 0;
            virtual void NewFrame() = 0;
            virtual void SetAutorepeatTime(int) = 0;
            virtual void ClearBuffer() = 0;
            virtual bool GetLastKbdEvent(unsigned short&, unsigned char&, bool&, double&, bool) = 0;
            virtual int GetMouseX() = 0;
            virtual int GetMouseY() = 0;
            virtual int GetMouseZ() = 0;
            virtual int GetMouseB(int) = 0;
            virtual int GetParam(m3d::input::DeviceParam) = 0;
            virtual int SetActiveState(int) = 0;
            virtual void ChangeLanguage() = 0;
            virtual void SetLanguage(m3d::input::Language) = 0;
            virtual m3d::input::Language GetLanguage() const = 0;
        };
    }  // namespace input
}  // namespace m3d

namespace snd
{
    enum UserSoundType : int32_t
    {
        SND_TYPE_2DSOUND = 0x0,
        SND_TYPE_3DSOUND = 0x1,
        SND_TYPE_MUSIC = 0x2,
    };

    enum SoundGroupType : int32_t
    {
        SND_MUSIC_GROUP = 0x0,
        SND_SOUND_GROUP = 0x1,
        SND_3DSOUND_GROUP = 0x2,
        SND_MAX_GROUP = 0x10,
    };

    enum SoundPriority : int32_t
    {
        SND_PRIORITY_EXTRALOW = 0x32,
        SND_PRIORITY_LOW = 0x64,
        SND_PRIORITY_NORMAL = 0x80,
        SND_PRIORITY_HIGH = 0xC8,
        SND_PRIORITY_EXTRAHIGH = 0xFF,
    };

    struct ISound : public IBase
    {
        virtual bool Init(void (*)(CStr const&), unsigned int, unsigned int, unsigned char, char const*) = 0;
        virtual int Update(double) = 0;
        virtual void SetMaxVolume(int) = 0;
        virtual void SetMusicFadeTime(float) = 0;
        virtual unsigned char GetChannelVolume(int) = 0;
        virtual bool SetChannelVolume(int, unsigned char, bool) = 0;
        virtual bool SetGroupVolume(int, unsigned char) = 0;
        virtual bool IsChannelPlaying(int) = 0;
        virtual bool IsMusicPlaying(int) = 0;
        virtual bool PauseGroup(int, bool) = 0;
        virtual bool PauseAllSounds(bool) = 0;
        virtual int PlayMusic(int, bool, bool) = 0;
        virtual int PlaySound2D(int, bool) = 0;
        virtual int PlaySound3D(int, CVector const&, CVector const&, bool) = 0;
        virtual bool SetChannelLoopMode(int, bool) = 0;
        virtual int GetChannelFrequency(int) = 0;
        virtual bool SetChannelFrequency(int, int) = 0;
        virtual bool SetPosition(int, CVector const&, CVector const&) = 0;
        virtual bool SetListenerPosition(CVector const&, CVector const&, CVector const&, CVector const&) = 0;
        virtual bool StopChannel(int) = 0;
        virtual bool StopGroup(int) = 0;
        virtual int StopAllSounds() = 0;
        virtual int MuteAllSounds() = 0;
        virtual int RestoreAllVolumes() = 0;
        virtual int AddSound(char const*, snd::UserSoundType, int, int, snd::SoundPriority) = 0;
        virtual int AddSound(char const*, snd::UserSoundType, char const*, int, snd::SoundPriority) = 0;
        virtual bool DeleteIdTableSound(int) = 0;
        virtual bool DeleteAllSounds() = 0;
        virtual bool SetSoundPriority(int, snd::SoundPriority) = 0;
        virtual bool SetEndMusicCallback(int, void (*)(int)) = 0;
        virtual float GetCPUusage() = 0;
        virtual void GetMemUsage(unsigned int&, unsigned int&) = 0;
        virtual int GetSoundGroupId(int) = 0;
        virtual bool GetGroupMinDist(int, float&) = 0;
        virtual bool GetGroupMaxDist(int, float&) = 0;
        virtual void DumpSoundInfo() = 0;
    };
}  // namespace snd

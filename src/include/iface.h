#pragma once
#include <core/stringm3d.h>

class CVector;

class IBase
{
public:
    virtual int DecRef();
    virtual int IncRef();
    virtual void* QueryIface(const char*);

protected:
    virtual ~IBase();

private:
    //IBase_vtbl* __vftable /*VFT*/;
};

namespace m3d
{
    class IDeviceResetCallback
    {
    public:
        IDeviceResetCallback();

    private:
       // m3d::IDeviceResetCallback_vtbl* __vftable /*VFT*/;
    };

    namespace input
    {
        class IInput : public IBase
        {
        };
    }
}

namespace snd
{
    enum UserSoundType
    {
        SND_TYPE_2DSOUND = 0x0,
        SND_TYPE_3DSOUND = 0x1,
        SND_TYPE_MUSIC = 0x2,
    };

    enum SoundGroupType
    {
        SND_MUSIC_GROUP = 0x0,
        SND_SOUND_GROUP = 0x1,
        SND_3DSOUND_GROUP = 0x2,
        SND_MAX_GROUP = 0x10,
    };

    enum SoundPriority
    {
        SND_PRIORITY_EXTRALOW = 0x32,
        SND_PRIORITY_LOW = 0x64,
        SND_PRIORITY_NORMAL = 0x80,
        SND_PRIORITY_HIGH = 0xC8,
        SND_PRIORITY_EXTRAHIGH = 0xFF,
    };

    class ISound : public IBase
    {
    public:
        virtual ~ISound() = default;
        virtual bool Init(void(CStr const&), unsigned int, unsigned int, unsigned __int8, const char*) = 0;
        virtual int Update(long double) = 0;
        virtual void SetMaxVolume(int) = 0;
        virtual void SetMusicFadeTime(float) = 0;
        virtual unsigned __int8 GetChannelVolume(int) = 0;
        virtual bool SetChannelVolume(int, unsigned __int8, bool) = 0;
        virtual bool SetGroupVolume(int, unsigned __int8) = 0;
        virtual bool IsChannelPlaying(int) = 0;
        virtual bool IsMusicPlaying(int) = 0;
        virtual bool PauseGroup(int, bool) = 0;
        virtual bool PauseAllSounds(bool) = 0;
        virtual int PlayMusic(int, bool, bool) = 0;
        virtual int PlaySound2D(int, bool) = 0;
        virtual int PlaySound3D(int, const CVector*, const CVector*, bool) = 0;
        virtual bool SetChannelLoopMode(int, bool) = 0;
        virtual int GetChannelFrequency(int) = 0;
        virtual bool SetChannelFrequency(int, int) = 0;
        virtual bool SetPosition(int, const CVector*, const CVector*) = 0;
        virtual bool SetListenerPosition(const CVector*, const CVector*, const CVector*, const CVector*) = 0;
        virtual bool StopChannel(int) = 0;
        virtual bool StopGroup(int) = 0;
        virtual int StopAllSounds() = 0;
        virtual int MuteAllSounds() = 0;
        virtual int RestoreAllVolumes() = 0;
        virtual int AddSound(const char*, snd::UserSoundType, int, int, snd::SoundPriority) = 0;
        virtual bool DeleteIdTableSound(int) = 0;
        virtual bool DeleteAllSounds() = 0;
        virtual bool SetSoundPriority(int, snd::SoundPriority) = 0;
        virtual bool SetEndMusicCallback(int, void(__fastcall*)(int)) = 0;
        virtual float GetCPUusage() = 0;
        virtual void GetMemUsage(unsigned int*, unsigned int*) = 0;
        virtual int GetSoundGroupId(int) = 0;
        virtual bool GetGroupMinDist(int, float*) = 0;
        virtual bool GetGroupMaxDist(int, float*) = 0;
        virtual void DumpSoundInfo() = 0;
    };
}

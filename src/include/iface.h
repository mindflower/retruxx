#pragma once

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
        bool Init(void(CStr const&), unsigned int, unsigned int, unsigned __int8, const char*);
        int Update(long double);
        void SetMaxVolume(int);
        void SetMusicFadeTime(float);
        unsigned __int8 GetChannelVolume(int);
        bool SetChannelVolume(int, unsigned __int8, bool);
        bool SetGroupVolume(int, unsigned __int8);
        bool IsChannelPlaying(int);
        bool IsMusicPlaying(int);
        bool PauseGroup(int, bool);
        bool PauseAllSounds(bool);
        int PlayMusic(int, bool, bool);
        int PlaySound2D(int, bool);
        int PlaySound3D(int, const CVector*, const CVector*, bool);
        bool SetChannelLoopMode(int, bool);
        int GetChannelFrequency(int);
        bool SetChannelFrequency(int, int);
        bool SetPosition(int, const CVector*, const CVector*);
        bool SetListenerPosition(const CVector*, const CVector*, const CVector*, const CVector*);
        bool StopChannel(int);
        bool StopGroup(int);
        int StopAllSounds();
        int MuteAllSounds();
        int RestoreAllVolumes();
        int AddSound(const char*, snd::UserSoundType, int, int, snd::SoundPriority);
        bool DeleteIdTableSound(int);
        bool DeleteAllSounds();
        bool SetSoundPriority(int, snd::SoundPriority);
        bool SetEndMusicCallback(int, void(__fastcall*)(int));
        float GetCPUusage();
        void GetMemUsage(unsigned int*, unsigned int*);
        int GetSoundGroupId(int);
        bool GetGroupMinDist(int, float*);
        bool GetGroupMaxDist(int, float*);
        void DumpSoundInfo();
    };
}

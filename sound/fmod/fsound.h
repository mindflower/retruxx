#pragma once
// The FMOD 3 API the original sound driver was written against, reduced to what it calls. The
// original loaded fmod.dll (FMOD 3.74) at run time and resolved these entry points into an
// FMOD_INSTANCE table; here the table is filled by fsound_compat.cpp, which implements the same
// calls over the FMOD Core API (retruxx adaptation, see there).

#ifndef F_API
#define F_API __stdcall
#endif
#define F_CALLBACKAPI __stdcall

typedef struct FSOUND_SAMPLE FSOUND_SAMPLE;
typedef struct FSOUND_STREAM FSOUND_STREAM;
typedef struct FSOUND_DSPUNIT FSOUND_DSPUNIT;
typedef struct FMUSIC_MODULE FMUSIC_MODULE;

// Channel / sample index values
#define FSOUND_FREE -1
#define FSOUND_UNMANAGED -2
#define FSOUND_ALL -3
#define FSOUND_STEREOPAN -1

// Sample / stream modes
enum FSOUND_MODES
{
    FSOUND_LOOP_OFF = 0x00000001,
    FSOUND_LOOP_NORMAL = 0x00000002,
    FSOUND_LOOP_BIDI = 0x00000004,
    FSOUND_8BITS = 0x00000008,
    FSOUND_16BITS = 0x00000010,
    FSOUND_MONO = 0x00000020,
    FSOUND_STEREO = 0x00000040,
    FSOUND_UNSIGNED = 0x00000080,
    FSOUND_SIGNED = 0x00000100,
    FSOUND_DELTA = 0x00000200,
    FSOUND_IT214 = 0x00000400,
    FSOUND_IT215 = 0x00000800,
    FSOUND_HW3D = 0x00001000,
    FSOUND_2D = 0x00002000,
    FSOUND_STREAMABLE = 0x00004000,
    FSOUND_LOADMEMORY = 0x00008000,
    FSOUND_LOADRAW = 0x00010000,
    FSOUND_MPEGACCURATE = 0x00020000,
    FSOUND_FORCEMONO = 0x00040000,
    FSOUND_HW2D = 0x00080000,
    FSOUND_ENABLEFX = 0x00100000,
    FSOUND_MPEGHALFRATE = 0x00200000,
    FSOUND_IGNORETAGS = 0x00400000,
    FSOUND_UNICODE = 0x00800000,
    FSOUND_NONBLOCKING = 0x01000000,
};

// The FSOUND_GetError codes are declared in fsound_errors.h with their texts: their names collide
// with the FMOD Core result codes, which fsound_compat.cpp needs as well.

// Callbacks
typedef signed char(F_CALLBACKAPI* FSOUND_STREAMCALLBACK)(FSOUND_STREAM* stream, void* buff, int len, void* userdata);
typedef void*(F_CALLBACKAPI* FSOUND_OPENCALLBACK)(const char* name);
typedef void(F_CALLBACKAPI* FSOUND_CLOSECALLBACK)(void* handle);
typedef int(F_CALLBACKAPI* FSOUND_READCALLBACK)(void* buffer, int size, void* handle);
typedef int(F_CALLBACKAPI* FSOUND_SEEKCALLBACK)(void* handle, int pos, signed char mode);
typedef int(F_CALLBACKAPI* FSOUND_TELLCALLBACK)(void* handle);

// The entry points the driver calls, in the order of the original table.
typedef struct
{
    void* module;

    signed char(F_API* FSOUND_Init)(int mixrate, int maxsoftwarechannels, unsigned int flags);
    void(F_API* FSOUND_Close)(void);
    void(F_API* FSOUND_Update)(void);
    void(F_API* FSOUND_File_SetCallbacks)(FSOUND_OPENCALLBACK useropen, FSOUND_CLOSECALLBACK userclose,
                                          FSOUND_READCALLBACK userread, FSOUND_SEEKCALLBACK userseek,
                                          FSOUND_TELLCALLBACK usertell);
    int(F_API* FSOUND_GetError)(void);
    int(F_API* FSOUND_GetMaxChannels)(void);
    float(F_API* FSOUND_GetCPUUsage)(void);
    void(F_API* FSOUND_GetMemoryStats)(unsigned int* currentalloced, unsigned int* maxalloced);
    FSOUND_SAMPLE*(F_API* FSOUND_Sample_Load)(int index, const char* name_or_data, unsigned int mode, int offset,
                                              int length);
    void(F_API* FSOUND_Sample_Free)(FSOUND_SAMPLE* sptr);
    signed char(F_API* FSOUND_Sample_SetMode)(FSOUND_SAMPLE* sptr, unsigned int mode);
    signed char(F_API* FSOUND_Sample_SetDefaults)(FSOUND_SAMPLE* sptr, int deffreq, int defvol, int defpan,
                                                  int defpri);
    signed char(F_API* FSOUND_Sample_SetMinMaxDistance)(FSOUND_SAMPLE* sptr, float min, float max);
    signed char(F_API* FSOUND_Sample_SetMaxPlaybacks)(FSOUND_SAMPLE* sptr, int max);
    signed char(F_API* FSOUND_Sample_GetDefaults)(FSOUND_SAMPLE* sptr, int* deffreq, int* defvol, int* defpan,
                                                  int* defpri);
    unsigned int(F_API* FSOUND_Sample_GetMode)(FSOUND_SAMPLE* sptr);
    int(F_API* FSOUND_PlaySoundEx)(int channel, FSOUND_SAMPLE* sptr, FSOUND_DSPUNIT* dsp, signed char startpaused);
    signed char(F_API* FSOUND_StopSound)(int channel);
    signed char(F_API* FSOUND_SetFrequency)(int channel, int freq);
    signed char(F_API* FSOUND_SetVolume)(int channel, int vol);
    signed char(F_API* FSOUND_SetVolumeAbsolute)(int channel, int vol);
    signed char(F_API* FSOUND_SetPaused)(int channel, signed char paused);
    signed char(F_API* FSOUND_SetLoopMode)(int channel, unsigned int loopmode);
    signed char(F_API* FSOUND_3D_SetAttributes)(int channel, const float* pos, const float* vel);
    signed char(F_API* FSOUND_IsPlaying)(int channel);
    int(F_API* FSOUND_GetFrequency)(int channel);
    int(F_API* FSOUND_GetVolume)(int channel);
    void(F_API* FSOUND_3D_Listener_SetAttributes)(const float* pos, const float* vel, float fx, float fy, float fz,
                                                  float tx, float ty, float tz);
    FSOUND_STREAM*(F_API* FSOUND_Stream_Open)(const char* name_or_data, unsigned int mode, int offset, int length);
    signed char(F_API* FSOUND_Stream_Close)(FSOUND_STREAM* stream);
    int(F_API* FSOUND_Stream_PlayEx)(int channel, FSOUND_STREAM* stream, FSOUND_DSPUNIT* dsp,
                                     signed char startpaused);
    signed char(F_API* FSOUND_Stream_Stop)(FSOUND_STREAM* stream);
    signed char(F_API* FSOUND_Stream_SetPosition)(FSOUND_STREAM* stream, unsigned int position);
    int(F_API* FSOUND_Stream_GetLengthMs)(FSOUND_STREAM* stream);
    signed char(F_API* FSOUND_Stream_SetMode)(FSOUND_STREAM* stream, unsigned int mode);
    unsigned int(F_API* FSOUND_Stream_GetMode)(FSOUND_STREAM* stream);
    FSOUND_SAMPLE*(F_API* FSOUND_Stream_GetSample)(FSOUND_STREAM* stream);
    signed char(F_API* FSOUND_Stream_SetEndCallback)(FSOUND_STREAM* stream, FSOUND_STREAMCALLBACK callback,
                                                     void* userdata);
} FMOD_INSTANCE;

// orig 0x5ee640 fmoddyn.h:253 / 0x5efb60 fmoddyn.h:529
// The original's FMOD_CreateInstance loaded the DLL named here and resolved the table from its
// exports; this one fills the table with the FMOD Core implementation (fsound_compat.cpp).
FMOD_INSTANCE* FMOD_CreateInstance(char* dllName);
void FMOD_FreeInstance(FMOD_INSTANCE* instance);

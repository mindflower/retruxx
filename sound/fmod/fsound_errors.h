#pragma once
// The FMOD 3 error texts the driver logs after a failed call (the original's fmod_errors.h).
#include "fsound.h"

// FSOUND_GetError
enum FMOD_ERRORS
{
    FMOD_ERR_NONE,
    FMOD_ERR_BUSY,
    FMOD_ERR_UNINITIALIZED,
    FMOD_ERR_INIT,
    FMOD_ERR_ALLOCATED,
    FMOD_ERR_PLAY,
    FMOD_ERR_OUTPUT_FORMAT,
    FMOD_ERR_COOPERATIVELEVEL,
    FMOD_ERR_CREATEBUFFER,
    FMOD_ERR_FILE_NOTFOUND,
    FMOD_ERR_FILE_FORMAT,
    FMOD_ERR_FILE_BAD,
    FMOD_ERR_MEMORY,
    FMOD_ERR_VERSION,
    FMOD_ERR_INVALID_PARAM,
    FMOD_ERR_NO_EAX,
    FMOD_ERR_CHANNEL_ALLOC,
    FMOD_ERR_RECORD,
    FMOD_ERR_MEDIAPLAYER,
    FMOD_ERR_CDDEVICE,
};

// orig 0x5efb80 fmod_errors.h:8
static char* FMOD_ErrorString(int errcode)
{
    switch (errcode)
    {
    case FMOD_ERR_NONE:
        return "No errors";
    case FMOD_ERR_BUSY:
        return "Cannot call this command after FSOUND_Init.  Call FSOUND_Close first.";
    case FMOD_ERR_UNINITIALIZED:
        return "This command failed because FSOUND_Init was not called";
    case FMOD_ERR_PLAY:
        return "Playing the sound failed.";
    case FMOD_ERR_INIT:
        return "Error initializing output device.";
    case FMOD_ERR_ALLOCATED:
        return "The output device is already in use and cannot be reused.";
    case FMOD_ERR_OUTPUT_FORMAT:
        return "Soundcard does not support the features needed for this soundsystem (16bit stereo output)";
    case FMOD_ERR_COOPERATIVELEVEL:
        return "Error setting cooperative level for hardware.";
    case FMOD_ERR_CREATEBUFFER:
        return "Error creating hardware sound buffer.";
    case FMOD_ERR_FILE_NOTFOUND:
        return "File not found";
    case FMOD_ERR_FILE_FORMAT:
        return "Unknown file format";
    case FMOD_ERR_FILE_BAD:
        return "Error loading file";
    case FMOD_ERR_MEMORY:
        return "Not enough memory ";
    case FMOD_ERR_VERSION:
        return "The version number of this file format is not supported";
    case FMOD_ERR_INVALID_PARAM:
        return "An invalid parameter was passed to this function";
    case FMOD_ERR_NO_EAX:
        return "Tried to use an EAX command on a non EAX enabled channel or output.";
    case FMOD_ERR_CHANNEL_ALLOC:
        return "Failed to allocate a new channel";
    case FMOD_ERR_RECORD:
        return "Recording not supported on this device";
    case FMOD_ERR_MEDIAPLAYER:
        return "Required Mediaplayer codec is not installed";
    default:
        return "Unknown error";
    };
}

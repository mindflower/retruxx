#include "globalscriptfuncs.h"
#include "m3dgame.h"
#include "profile.h"
#include <cinematic.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <script/scriptserver.h>
#include <server/objects/base/objcontainer.h>

namespace ai
{
    extern ObjContainer* theObjects;
}

void RegisterGlobalNatives()
{
    auto& scriptServer = m3d::g_Kernel->GetScriptServer();
    scriptServer.registerGlobalFunction(&n_SetCameraPos, "SetCameraPos", "void", "const CVector& cameraPos, float yaw, float pitch, float roll, [optional] GPlayer* player");
    scriptServer.registerGlobalFunction(&n_GetCameraPos, "GetCameraPos", "CVector cameraPos, Quaternion rotation, CVector lookAt");
    scriptServer.registerGlobalFunction(&n_SetCameraZoom, "SetCameraZoom", "void", "float zoom");
    scriptServer.registerGlobalFunction(&n_GetCameraZoom, "GetCameraZoom", "float", "void");
    scriptServer.registerGlobalFunction(&n_SetCameraAngle, "SetCameraAngle", "void", "float angle");
    scriptServer.registerGlobalFunction(&n_SetCameraDirectionToObj, "SetCameraDirectionToObj", "void", "int targetObjId");
    scriptServer.registerGlobalFunction(&n_SetCameraBehindPlayerVehicle, "SetCameraBehindPlayerVehicle", "void", "void");
    scriptServer.registerGlobalFunction(&n_MinimapAddMark, "MinimapAddMark", "void");
    scriptServer.registerGlobalFunction(&n_MinimapDelMark, "MinimapDelMark", "void");
    scriptServer.registerGlobalFunction(&n_GetCinematic, "GetCinematic", "void");
    scriptServer.registerGlobalFunction(&n_ChangeMode, "ChangeMode", "void");
    scriptServer.registerGlobalFunction(&n_SetGameSpeed, "SetGameSpeed");
    scriptServer.registerGlobalFunction(&n_GetGameSpeed, "GetGameSpeed");
    scriptServer.registerGlobalFunction(&n_GetMaxTimescale, "GetMaxTimescale", "float", "returns current max timescale value");
    scriptServer.registerGlobalFunction(&n_SetMaxTimescale, "SetMaxTimescale", "void", "float newMaxTimescale", "sets new max timescale value");
    scriptServer.registerGlobalFunction(&n_GetMinTimescale, "GetMinTimescale", "float", "returns current min timescale value");
    scriptServer.registerGlobalFunction(&n_SetMinTimescale, "SetMinTimescale", "void", "float newMinTimescale", "sets new min timescale value");
    scriptServer.registerGlobalFunction(&n_GetNormalTimescale, "GetNormalTimescale", "float", "returns current normal timescale value");
    scriptServer.registerGlobalFunction(&n_SetNormalTimescale, "SetNormalTimescale", "void", "float newNormalTimescale", "sets new normal timescale value");
    scriptServer.registerGlobalFunction(&n_StartRendering, "StartRendering", "void", "[optional] GPlayer* player", "client does not render anything until this function is called");
    scriptServer.registerGlobalFunction(&n_IsPlayingCampaign, "IsPlayingCampaign", "bool", "void", "are we currently playing campaign?");
    scriptServer.registerGlobalFunction(&n_SetCinematicFadeParams, "SetCinematicFadeParams", "void", "bool StartFade, bool EndFade", "set if fading out or(and) up must be done by cinematic mode");
    scriptServer.registerGlobalFunction(&n_SetCinematicCinemaPanel, "SetCinematicCinemaPanel", "void", "bool VisiblePanel", "set if no need has cinematic panel (two black border) in cinematic mode");
    scriptServer.registerGlobalFunction(&n_ResetFogOfWarFC, "ResetFogOfWar", "void");
    scriptServer.registerGlobalFunction(&n_SetBelongColor, "SetBelongColor", "void", "float StartFade, float EndFade", "remaps given belong to new index");
    scriptServer.registerGlobalFunction(&n_SetSpellBookCheated, "SetSpellBookCheated", "void");
    scriptServer.registerGlobalFunction(&n_UpdateCinematic, "UpdateCinematic", "void", "float time", "Updates current cinematic by given time");
    scriptServer.registerGlobalFunction(&n_PlayVideo, "PlayVideo", "void", "string videoName", "Playing video with given name");
    scriptServer.registerGlobalFunction(&n_CreateEffectTTLed, "CreateEffectTTLed", "Obj*", "string modelname, CVector pos, Quaternion rot, int TTL", "Creates Effect");
    scriptServer.registerGlobalFunction(&n_CreateEffectInsertedInRemove, "CreateEffectInsertedInRemove", "Obj*", "string modelname, CVector pos, Quaternion rot, int bInsertInRemoveIfFree", "Creates Effect");
    scriptServer.registerGlobalFunction(&n_CreateNodeTTLed, "CreateNodeTTLed", "Obj*", "const char* modelname, CVector pos, Quaternion rot, int TTL", "Creates Node");
    scriptServer.registerGlobalFunction(&n_CreateNodeInsertedInRemove, "CreateNodeInsertedInRemove", "Obj*", "const char* modelname, CVector pos, Quaternion rot, int bInsertInRemoveIfFree", "Creates Node");
    scriptServer.registerGlobalFunction(&n_RemoveNode, "RemoveNode", "void", "Obj* node", "Removes Node");
    scriptServer.registerGlobalFunction(&n_PauseRadio, "PauseRadio", "void", "void", "Pause Radio Manager");
    scriptServer.registerGlobalFunction(&n_ResumeRadio, "ResumeRadio", "void", "void", "Resume Radio Manager");
    scriptServer.registerGlobalFunction(&n_EnableCinematicDebug, "EnableCinematicDebug", "void", "string filename, string pathname", "Enables cinematic debug mode");
    scriptServer.registerGlobalFunction(&n_CinematicPathDump, "CinematicPathDump", "void", "string filename", "Dump current cinematic path");
    scriptServer.registerGlobalFunction(&n_AddCurrentPointToCinematicPath, "AddCurrentPointToCinematicPath", "void", "void", "Add camera position and rotation at the end of current cinematic path");
    scriptServer.registerGlobalFunction(&n_InsertCurrentPointToCinematicPath, "InsertCurrentPointToCinematicPath", "void", "void", "Add camera position and rotation to current cinematic path after current point and sets it to current");
    scriptServer.registerGlobalFunction(&n_DisableCinematicDebug, "DisableCinematicDebug", "void", "void", "Disables cinematic debug mode");
    scriptServer.registerGlobalFunction(&n_SetCinematicPoint, "SetCinematicPoint", "void", "void", "Sets current point number to work with");
    scriptServer.registerGlobalFunction(&n_MoveCurrentCinematicPointToCamera, "MoveCurrentCinematicPointToCamera", "void", "void", "Moves current cinematic point to camera");
    scriptServer.registerGlobalFunction(&n_RemoveCurrentCinematicPoint, "RemoveCurrentCinematicPoint", "void", "void", "Removes current cinematic point from path");
    scriptServer.registerGlobalFunction(&n_DumpSceneGraph, "DumpSceneGraph", "void", "string filename", "Dump current scene into file");
    scriptServer.registerGlobalFunction(&n_DumpPhysicInfo, "DumpPhysicInfo", "void", "string filename", "Dump current physic scene into file");
    scriptServer.registerGlobalFunction(&n_GetNodeByName, "GetNodeByName", "void", "const char* name", "Get scene node by name");
    scriptServer.registerGlobalFunction(&n_GetProfileMotionBlur, "GetProfileMotionBlur", "bool", "void", "Returns motionBlur value from player's profile");
    scriptServer.registerGlobalFunction(&n_GetProfileMotionBlurAlpha, "GetProfileMotionBlurAlpha", "float", "Returns motionBlurAlpha value from player's profile");
    scriptServer.registerGlobalFunction(&n_GetProfileBloom, "GetProfileBloom", "bool", "Returns bloom value from player's profile");
    scriptServer.registerGlobalFunction(&n_SetProfileMotionBlur, "SetProfileMotionBlur", "bool bMotionBlur", "Puts new motionBlur value in player's profile");
    scriptServer.registerGlobalFunction(&n_SetProfileMotionBlurAlpha, "SetProfileMotionBlurAlpha", "float fMotionBlurAlpha", "Puts new motionBlurAlpha value in player's profile");
    scriptServer.registerGlobalFunction(&n_SetProfileBloom, "SetProfileBloom", "bool bBloom", "Puts new motionBlur value in player's profile");
    scriptServer.registerGlobalFunction(&n_VTuneResume, "VTuneResume", "Activates VTune data collection");
    scriptServer.registerGlobalFunction(&n_VTunePause, "VTunePause", "bool bBloom", "Pauses VTune data collection");
    scriptServer.registerGlobalFunction(&n_ShowHostileVehicles, "ShowHostileVehicles", "void", "bool", "Shows/hides infection zones vehicles");
    scriptServer.registerGlobalFunction(&n_PassToMap, "PassToMap", "void", "string mapName, CStr locationName, float angle, bool bImmediate", "Passes player vehicle to another map");
    scriptServer.registerGlobalFunction(&n_UpdateWeather, "UpdateWeather", "void", "void", "Set world param according to current time");
    scriptServer.registerGlobalFunction(&n_SetWeather, "SetWeather", "void", "int weatherId", "Set and apply weather");
    scriptServer.registerGlobalFunction(&n_Assert, "Assert", "void", "bool expression", "Crashes if expression is false");
    scriptServer.registerGlobalFunction(&n_ShowDeathMenu, "ShowDeathMenu", "void", "void", "Show menu on player death");
    scriptServer.registerGlobalFunction(&n_AddFadingMsgFormatted, "AddFadingMsgFormatted", "void", "string msg, any number of args of any type", "Add a message to fading list. String parameter is the message itself. AIParams parameters used for formatting pattern string.");
    scriptServer.registerGlobalFunction(&n_AddFadingMsgByStrIdFormatted, "AddFadingMsgByStrIdFormatted", "void", "string msg, any number of args of any type", "Add a message to fading list. String parameter is a string identifier of the message. AIParams parameters used for formatting pattern string.");
    scriptServer.registerGlobalFunction(&n_AddImportantFadingMsgFormatted, "AddImportantFadingMsgFormatted", "void", "string msg, any number of args of any type", "Add a message to 'important' fading list. String parameter is the message itself. AIParams parameters used for formatting pattern string.");
    scriptServer.registerGlobalFunction(&n_AddImportantFadingMsgByStrIdFormatted, "AddImportantFadingMsgByStrIdFormatted", "void", "string msg, any number of args of any type", "Add a message to 'important' fading list. String parameter is a string identifier of the message. AIParams parameters used for formatting pattern string.");
    scriptServer.registerGlobalFunction(&n_PlayCustomMusic, "PlayCustomMusic", "void", "string musicName", "Plays music with given name");
    scriptServer.registerGlobalFunction(&n_StopPlayingCustomMusic, "StopPlayingCustomMusic", "void", "Stops playing custom music");
    scriptServer.registerGlobalFunction(&n_SetCoordinateForQuest, "SetCoordinateForQuest", "void", "string questName, CVector point, string levelName", "Sets coordinate for quest navpoint");
    scriptServer.registerGlobalFunction(&n_DumpSoundInfo, "DumpSoundInfo", "void", "Dumps some info about currently playing sounds to log");
    scriptServer.registerGlobalFunction(&n_DumpOpenFiles, "DumpOpenFiles", "void", "Dumps names of open files");
}

//int n_ShowHostileVehicles(m3d::sArgStack& scriptStack)
//{
//    if (scriptStack.getNumInArgs() != 1)
//    {
//        return -1;
//    }
//
//    auto const* arg = scriptStack.popIn();
//    auto const type = arg->GetType();
//    if (type != m3d::sArg::ARGTYPE_BOOL && type != m3d::sArg::ARGTYPE_FLOAT && type != m3d::sArg::ARGTYPE_INT)
//    {
//        return -1;
//    }
//
//    //TODO:...
//}

int n_SetProfileMotionBlur(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }

    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto* profile = pGame->GetProfileManager()->GetCurProfile();
    if (profile == nullptr)
    {
        return -1;
    }

    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_BOOL)
    {
        return -1;
    }

    m3d::AIParam param = arg->GetB() ? "yes" : "no";
    profile->SetParam(PP_MOTION_BLUR, param);
    return 1;
}

int n_SetProfileMotionBlurAlpha(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }

    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto* profile = pGame->GetProfileManager()->GetCurProfile();
    if (profile == nullptr)
    {
        return -1;
    }

    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_BOOL)
    {
        return -1;
    }

    m3d::AIParam param = arg->GetB() ? "yes" : "no";
    profile->SetParam(PP_MOTION_BLUR_ALPHA, param);
    return 1;
}

int n_GetCinematic(m3d::sArgStack& scriptStack)
{
    scriptStack.newOut()->SetO(m3d::Application::g_pApp->m_cinematic);
    return 1;
}

int n_SetBelongColor(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 2)
    {
        return -1;
    }
    auto const* firstArg = scriptStack.popIn();
    auto const firstArgType = firstArg->GetType();
    if (firstArgType != m3d::sArg::ARGTYPE_INT && firstArgType != m3d::sArg::ARGTYPE_FLOAT)
    {
        return -1;
    }

    auto const* secondArg = scriptStack.popIn();
    auto const secondArgType = secondArg->GetType();
    if (secondArgType != m3d::sArg::ARGTYPE_INT && secondArgType != m3d::sArg::ARGTYPE_FLOAT)
    {
        return -1;
    }

    //TODO: further implementation not found
    return 1;
}

int n_DisableCinematicDebug(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }
    m3d::Application::g_pApp->m_cinematic->SetDebugMode(0);
    return 1;
}

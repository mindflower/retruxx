#include "globalscriptfuncs.h"
#include "m3dgame.h"
#include "profile.h"
#include "uimisc/questinfo.h"
#include <cinematic.h>
#include <config.h>
#include <m3dapp.h>
#include <core/kernel.h>
#include <script/scriptserver.h>
#include <server/objects/base/objcontainer.h>
#include <client.h>
#include <world.h>
#include <core/timer.h>
#include <file/fileserver.h>
#include <scene/nodes/sgnodesound.h>
#include <server/objects/player.h>
#include <server/objects/vehicle.h>

namespace m3d
{
    extern CClient* pClient;
}

namespace ai
{
    extern ObjContainer* theObjects;
    extern Player* thePlayer;
}

namespace
{
    int GetFadingMsgParams(m3d::sArgStack& scriptStack, CStr& msg, std::vector<m3d::AIParam>& params)
    {
        if (scriptStack.getNumInArgs() < 1)
        {
            LOG("GetFadingMsgParams error: at least one argument must be specified", LOG_INFO);
            return -1;
        }

        auto* arg = scriptStack.popIn();
        if  (arg->GetType() != m3d::sArg::ARGTYPE_STRING)
        {
            LOG("GetFadingMsgParams error: first argument must be of string type", LOG_INFO);
            return -1;
        }

        msg = arg->GetS();
        //TODO: check args count
        while(scriptStack.getNumInArgs() > 0)
        {
            arg = scriptStack.popIn();
            switch (arg->GetType())
            {
            case m3d::sArg::ARGTYPE_INT:
            {
                params.emplace_back(arg->GetI());
                break;
            }
            case m3d::sArg::ARGTYPE_FLOAT:
            {
                params.emplace_back(arg->GetF());
                break;
            }
            case m3d::sArg::ARGTYPE_BOOL:
            {
                params.emplace_back(arg->GetB() ? "true" : "false");
                break;
            }
            case m3d::sArg::ARGTYPE_STRING:
            {
                params.emplace_back(arg->GetS());
                break;
            }
            case m3d::sArg::ARGTYPE_VECTOR:
            {
                params.emplace_back(arg->GetV());
                break;
            }
            case m3d::sArg::ARGTYPE_QUATERNION:
            {
                params.emplace_back(arg->GetQ());
                break;
            }
            default:
            {
                LOG("GetFadingMsgParams error: invalid param type", LOG_INFO);
                return -1;
            }
            }
        }
        return 1;
    }
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

int n_AddImportantFadingMsgByStrIdFormatted(m3d::sArgStack& scriptStack)
{
    CStr msgId;
    std::vector<m3d::AIParam> params;
    if (GetFadingMsgParams(scriptStack, msgId, params) == -1)
    {
        return -1;
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    pGame->m_pInterfaceManager->AddImportantFadingMsgByStrId(msgId, params);
    return 1;
}

int n_AddFadingMsgByStrIdFormatted(m3d::sArgStack& scriptStack)
{
    CStr msgId;
    std::vector<m3d::AIParam> params;
    if (GetFadingMsgParams(scriptStack, msgId, params) == -1)
    {
        return -1;
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    pGame->m_pInterfaceManager->AddFadingMsgByStrId(msgId, params);
    return 1;
}

int n_AddFadingMsgFormatted(m3d::sArgStack& scriptStack)
{
    CStr msg;
    std::vector<m3d::AIParam> params;
    if (GetFadingMsgParams(scriptStack, msg, params) == -1)
    {
        return -1;
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    pGame->m_pInterfaceManager->AddFadingMsg(msg, params);
    return 1;
}

int n_GetProfileMotionBlur(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }

    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto* profile = pGame->GetProfileManager()->GetCurProfile();
    if (profile == nullptr)
    {
        return -1;
    }

    m3d::AIParam param;
    profile->GetParam(PP_MOTION_BLUR, param);
    //TODO: check this
    scriptStack.newOut()->SetB(param.GetAsStr() == "yes" ? true : false);
    return 1;
}

int n_CinematicPathDump(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        m3d::g_Kernel->GetEngineCfg().m_console->PrintF("Usage: CinematicPathDump( <fileName> )\n");
        return -1;
    }

    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_STRING)
    {
        return -1;
    }
    auto const* filename = arg->GetS();
    m3d::Application::g_pApp->m_cinematic->DumpCurrentPath(filename);
    return 1;
}

int n_PlayVideo(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }

    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_STRING)
    {
        return -1;
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    pGame->StartPlayingVideo(arg->GetS(), &CMiracle3d::OnFinishVideoPlaying);
    return 1;
}

int n_SetCameraAngle(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }

    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_FLOAT)
    {
        return -1;
    }
    m3d::Application::g_pApp->m_curCamera.m_rotYaw = arg->GetF() * 0.017453292;
    return 1;
}

int n_UpdateWeather(m3d::sArgStack& scriptStack)
{
    if (m3d::pClient == nullptr)
    {
        return -11;
    }
    m3d::pClient->GetWorld().GetWeatherManager().UpdateDayTime();
    return 1;
}

//int n_SetCinematicCinemaPanel(m3d::sArgStack& scriptStack)
//{
//    if (scriptStack.getNumInArgs() != 1)
//    {
//        return -1;
//    }
//
//    auto const* arg = scriptStack.popIn();
//    if (arg->GetType() != m3d::sArg::ARGTYPE_BOOL)
//    {
//        //TODO:...
//    }
//}

int n_RemoveCurrentCinematicPoint(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }
    m3d::Application::g_pApp->m_cinematic->RemoveCurrentDebugPoint();
    return 1;
}

int n_SetCameraBehindPlayerVehicle(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    if (pGame->m_player.m_cameraMode != CM_FOLLOWMODE)
    {
        return -1;
    }
    auto const* vehicle = m3d::pClient->GetWorld().GetVehicleControlledByPlayer();
    if (vehicle == nullptr)
    {
        return -1;
    }
    auto const playerDir = vehicle->GetDirection();
    pGame->m_curCamera.m_rotYaw = atan2(-playerDir.x, playerDir.y);
    return 1;
}

int n_DumpPhysicInfo(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }
    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_STRING)
    {
        return -1;
    }
    auto const* filename = arg->GetS();
    ai::theObjects->DumpPhysicInfo(filename);
    return 1;
}

int n_AddCurrentPointToCinematicPath(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }
    scriptStack.newOut()->SetV(m3d::Application::g_pApp->m_curCamera.m_worldOrigin);
    Quaternion q;
    q.fromYPR(m3d::Application::g_pApp->m_curCamera.m_rotYaw, m3d::Application::g_pApp->m_curCamera.m_rotPitch, m3d::Application::g_pApp->m_curCamera.m_rotRoll);
    m3d::Application::g_pApp->m_cinematic->AddPointToCurrentPath(m3d::Application::g_pApp->m_curCamera.m_worldOrigin, q, 1.0, 1.0);
    return 1;

}

int n_GetNormalTimescale(m3d::sArgStack& scriptStack)
{
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto const timeScale = pGame->GetNormalTimeScale();
    scriptStack.newOut()->SetF(timeScale);
    return 1;
}

int n_StopPlayingCustomMusic(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    pGame->SetCurHackedMusicType(HACKMUSIC_GAME);
    return 1;
}

int n_VTuneResume(m3d::sArgStack& scriptStack)
{
    return 2 * (scriptStack.getNumInArgs() == 0) - 1;
}

int n_DumpOpenFiles(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }

    LOG("********** DUMPING OPEN FILES ********************", LOG_INFO);
    std::vector<CStr> fileList;
    m3d::g_Kernel->GetFileServer().GetOpenFilesList(fileList);
    for (auto const& file : fileList)
    {
        LOG(file, LOG_INFO);
    }
    LOG("********** END DUMPING OPEN FILES ****************", LOG_INFO);
    return 1;
}

int n_RemoveNode(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }
    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_OBJECT)
    {
        return -1;
    }
    auto* obj = arg->GetO();
    if (!obj->IsKindOf(&m3d::SgNode::m_classSgNode))
    {
        return 0;
    }
    auto* sgNode = dynamic_cast<m3d::SgNode*>(obj);
    sgNode->GetGraph()->RemoveNode(sgNode);
    return 1;
}

int n_SetNormalTimescale(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }
    auto const* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_FLOAT)
    {
        return -1;
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    pGame->SetNormalTimeScale(arg->GetF());
    return 1;
}

int n_SetCoordinateForQuest(m3d::sArgStack& scriptStack)
{
    auto const numInArgs = scriptStack.getNumInArgs();
    if (numInArgs != 2 && numInArgs !=3)
    {
        return -1;
    }
    auto* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_STRING)
    {
        return -1;
    }
    CStr const questName(arg->GetS());
    arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_VECTOR)
    {
        return -1;
    }
    CVector const pos(arg->GetV());

    CStr levelName;
    if (numInArgs == 3)
    {
        auto const* arg = scriptStack.popIn();
        if (arg->GetType() != m3d::sArg::ARGTYPE_STRING)
        {
            return -1;
        }
        levelName = arg->GetS();
    }
    else
    {
        levelName = help::GetCurrentLevelName();
    }
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    auto* infoManager = pGame->m_pInterfaceManager->GetQuestInfoManager();
    infoManager->SetCoordinateForQuest(questName, levelName, pos);
    return 1;
}

int n_SetSpellBookCheated(m3d::sArgStack& scriptStack)
{
    return 1;
}

int n_MoveCurrentCinematicPointToCamera(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }
    scriptStack.newOut()->SetV(m3d::Application::g_pApp->m_curCamera.m_worldOrigin);
    Quaternion q;
    q.fromYPR(m3d::Application::g_pApp->m_curCamera.m_rotYaw, m3d::Application::g_pApp->m_curCamera.m_rotPitch, m3d::Application::g_pApp->m_curCamera.m_rotRoll);
    m3d::Application::g_pApp->m_cinematic->MoveCurrentDebugPoint(m3d::Application::g_pApp->m_curCamera.m_worldOrigin, q, 1.0);
    return 1;
}

int n_MinimapAddMark(m3d::sArgStack& scriptStack)
{
    auto const numOfArgs = scriptStack.getNumInArgs();
    if (numOfArgs < 2 || numOfArgs > 4)
    {
        return -1;
    }

    auto* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_VECTOR)
    {
        return -1;
    }
    //TODO: check this
    arg->GetV();

    arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_FLOAT)
    {
        return -1;
    }
    arg->GetF();

    arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_FLOAT)
    {
        return -1;
    }
    arg->GetF();
    return 1;
}

int n_UpdateCinematic(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }

    auto* arg = scriptStack.popIn();
    float value = 0.0;
    if (arg->GetType() == m3d::sArg::ARGTYPE_FLOAT)
    {
        value = arg->GetF();
    }
    else if (arg->GetType() == m3d::sArg::ARGTYPE_INT)
    {
        value = static_cast<float>(arg->GetI());
    }
    m3d::Application::g_pApp->HandleCinematic(value);
    return 1;
}

//int n_ChangeMode(m3d::sArgStack& scriptStack)
//{
//    if (scriptStack.getNumInArgs() != 1)
//    {
//        return -1;
//    }
//    auto* arg = scriptStack.popIn();
//    auto const type = arg->GetType();
//    if (type == m3d::sArg::ARGTYPE_VOID)
//    {
//        return -1;
//    }
//    //TODO:...
//    switch (type)
//    {
//    case m3d::sArg::ARGTYPE_INT: [[fallthrough]];
//    case m3d::sArg::ARGTYPE_FLOAT:
//    {
//        auto const val = arg->GetF();
//        if (val < 0 || val >= 4)
//        {
//            return -1;
//        }
//        break;
//    }
//    case m3d::sArg::ARGTYPE_BOOL:
//    {
//        if (arg->GetB() == false)
//        {
//            SYS_ERROR("exprValue");
//        }
//        break;
//    }
//    default:
//        break;
//    }
//
//}

int n_IsPlayingCampaign(m3d::sArgStack& scriptStack)
{
    return 1;
}

int n_GetMaxTimescale(m3d::sArgStack& scriptStack)
{
    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    scriptStack.newOut()->SetF(pGame->GetMaxTimeScale());
    return 1;
}

int n_VTunePause(m3d::sArgStack& scriptStack)
{
    return 2 * (scriptStack.getNumInArgs() == 0) - 1;
}

int n_DumpSoundInfo(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 0)
    {
        return -1;
    }
    LOG("********** DUMPING SOUND INFO ********************", LOG_INFO);
    if (m3d::Application::g_pApp->m_sound)
    {
        m3d::Application::g_pApp->m_sound->DumpSoundInfo();
    }
    LOG("Current sound nodes rendering: ", LOG_INFO);
    m3d::pClient->GetWorld().GetGraph().DumpRenderingNodesInfoForClass(&m3d::SgSoundSourceNode::m_classSgSoundSourceNode);
    LOG("********** END DUMPING SOUND INFO ****************", LOG_INFO);
    return 1;
}

int n_MinimapDelMark(m3d::sArgStack& scriptStack)
{
    auto const numOfArgs = scriptStack.getNumInArgs();
    if (numOfArgs == 0 || numOfArgs > 2)
    {
        return -1;
    }
    auto* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_FLOAT)
    {
        return -1;
    }
    //TODO: check this
    arg->GetF();
    return 1;
}

int n_Assert(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }
    auto* arg = scriptStack.popIn();
    switch (auto const type = arg->GetType(); type)
    {
    case m3d::sArg::ARGTYPE_VOID:
    {
        SYS_ERROR("exprValue");
        break;
    }
    case m3d::sArg::ARGTYPE_INT: [[fallthrough]];
    case m3d::sArg::ARGTYPE_FLOAT:
    {
        if (arg->GetF() <= 0.000099999997)
        {
            SYS_ERROR("exprValue");
        }
        break;
    }
    case m3d::sArg::ARGTYPE_BOOL:
    {
        if (arg->GetB() == false)
        {
            SYS_ERROR("exprValue");
        }
        break;
    }
    default:
        break;
    }
    return 1;
}

int n_StartRendering(m3d::sArgStack& scriptStack)
{
    return 1;
}

int n_AddImportantFadingMsgFormatted(m3d::sArgStack& scriptStack)
{
    CStr msg;
    std::vector<m3d::AIParam> params;
    if (GetFadingMsgParams(scriptStack, msg, params) == -1)
    {
        return -1;
    }

    auto* pGame = dynamic_cast<CMiracle3d*>(m3d::Application::g_pApp);
    pGame->m_pInterfaceManager->AddImportantFadingMsg(msg, params);
    return 1;
}

int n_GetGameSpeed(m3d::sArgStack& scriptStack)
{
    scriptStack.newOut()->SetF(m3d::g_Kernel->GetTimer().GetTimeScale());
    return 1;
}

//int n_ResumeRadio(m3d::sArgStack& scriptStack)
//{
//    if (!ai::theObjects || !ai::thePlayer)
//    {
//        return -1;
//    }
//    auto const radioManagerId = ai::thePlayer->GetRadioManagerId();
//    if (radioManagerId < 0)
//    {
//        return -1;
//    }
//    auto qwe = ai::theObjects->GetF
//}

int n_DumpSceneGraph(m3d::sArgStack& scriptStack)
{
    if (scriptStack.getNumInArgs() != 1)
    {
        return -1;
    }
    auto* arg = scriptStack.popIn();
    if (arg->GetType() != m3d::sArg::ARGTYPE_STRING)
    {
        return -1;
    }
    m3d::pClient->GetWorld().GetGraph().DumpToFile(arg->GetS());
    return 1;
}


#pragma once

namespace m3d
{
    class sArgStack;
}

void RegisterGlobalNatives();

int n_ShowHostileVehicles(m3d::sArgStack& scriptStack);
int n_SetProfileMotionBlur(m3d::sArgStack& scriptStack);
int n_SetProfileMotionBlurAlpha(m3d::sArgStack& scriptStack);
int n_GetCinematic(m3d::sArgStack& scriptStack);
int n_SetBelongColor(m3d::sArgStack& scriptStack);
int n_DisableCinematicDebug(m3d::sArgStack& scriptStack);
int n_AddImportantFadingMsgByStrIdFormatted(m3d::sArgStack& scriptStack);
int n_AddFadingMsgByStrIdFormatted(m3d::sArgStack& scriptStack);
int n_AddFadingMsgFormatted(m3d::sArgStack& scriptStack);
int n_GetProfileMotionBlur(m3d::sArgStack& scriptStack);
int n_CinematicPathDump(m3d::sArgStack& scriptStack);
int n_PlayVideo(m3d::sArgStack& scriptStack);
int n_SetCameraAngle(m3d::sArgStack& scriptStack);
int n_UpdateWeather(m3d::sArgStack& scriptStack);
int n_SetCinematicCinemaPanel(m3d::sArgStack& scriptStack);
int n_RemoveCurrentCinematicPoint(m3d::sArgStack& scriptStack);
int n_SetCameraBehindPlayerVehicle(m3d::sArgStack& scriptStack);
int n_DumpPhysicInfo(m3d::sArgStack& scriptStack);
int n_AddCurrentPointToCinematicPath(m3d::sArgStack& scriptStack);
int n_GetNormalTimescale(m3d::sArgStack& scriptStack);
int n_CreateNodeInsertedInRemove(m3d::sArgStack& scriptStack);
int n_StopPlayingCustomMusic(m3d::sArgStack& scriptStack);
int n_VTuneResume(m3d::sArgStack& scriptStack);
int n_CreateNodeTTLed(m3d::sArgStack& scriptStack);
int n_DumpOpenFiles(m3d::sArgStack& scriptStack);
int n_RemoveNode(m3d::sArgStack& scriptStack);
int n_SetNormalTimescale(m3d::sArgStack& scriptStack);
int n_SetCoordinateForQuest(m3d::sArgStack& scriptStack);
int n_SetSpellBookCheated(m3d::sArgStack& scriptStack);
int n_MoveCurrentCinematicPointToCamera(m3d::sArgStack& scriptStack);
int n_PassToMap(m3d::sArgStack& scriptStack);
int n_MinimapAddMark(m3d::sArgStack& scriptStack);
int n_UpdateCinematic(m3d::sArgStack& scriptStack);
int n_ResetFogOfWarFC(m3d::sArgStack&);
int n_SetWeather(m3d::sArgStack&);
int n_IsPlayingCampaign(m3d::sArgStack& scriptStack);
int n_GetMaxTimescale(m3d::sArgStack& scriptStack);
int n_VTunePause(m3d::sArgStack& scriptStack);
int n_SetCinematicPoint(m3d::sArgStack&);
int n_CreateEffectInsertedInRemove(m3d::sArgStack&);
int n_GetCameraPos(m3d::sArgStack&);
int n_ChangeMode(m3d::sArgStack& scriptStack);
int n_DumpSoundInfo(m3d::sArgStack& scriptStack);
int n_MinimapDelMark(m3d::sArgStack& scriptStack);
int n_SetCameraPos(m3d::sArgStack&);
int n_Assert(m3d::sArgStack& scriptStack);
int n_PauseRadio(m3d::sArgStack&);
int n_SetMaxTimescale(m3d::sArgStack&);
int n_CreateEffectTTLed(m3d::sArgStack&);
int n_StartRendering(m3d::sArgStack& scriptStack);
int n_AddImportantFadingMsgFormatted(m3d::sArgStack& scriptStack);
int n_GetGameSpeed(m3d::sArgStack& scriptStack);
int n_GetProfileBloom(m3d::sArgStack&);
int n_SetMinTimescale(m3d::sArgStack&);
int n_InsertCurrentPointToCinematicPath(m3d::sArgStack&);
int n_ResumeRadio(m3d::sArgStack& scriptStack);
int n_DumpSceneGraph(m3d::sArgStack& scriptStack);
int n_SetCameraZoom(m3d::sArgStack&);
int n_EnableCinematicDebug(m3d::sArgStack&);
int n_ShowDeathMenu(m3d::sArgStack&);
int n_SetCinematicFadeParams(m3d::sArgStack&);
int n_SetGameSpeed(m3d::sArgStack&);
int n_SetProfileBloom(m3d::sArgStack&);
int n_GetMinTimescale(m3d::sArgStack&);
int n_GetProfileMotionBlurAlpha(m3d::sArgStack&);
int n_GetNodeByName(m3d::sArgStack&);
int n_SetCameraDirectionToObj(m3d::sArgStack&);
int n_PlayCustomMusic(m3d::sArgStack&);
int n_GetCameraZoom(m3d::sArgStack&);

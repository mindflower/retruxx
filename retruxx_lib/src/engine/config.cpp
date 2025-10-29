#include <config.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/console/console.h>
#include <server/objects/monsters/boss02arm.h>

#include "m3dapp.h"
#include "scene/servers/DataServer.h"
#include <client.h>

#include "world.h"
#include "core/log.h"

namespace m3d
{
    CStr EngineConfig::GetNameByModelId(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float EngineConfig::GetHeight(float, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int EngineConfig::Save(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int EngineConfig::GetModelIdByName(CStr const& name)
    {
        auto id = M3D_APP->GetAnimatedModelsServer().GetItemByName(name.c_str(), true);
        if (id != -1)
        {
            return id + 0x200000;
        }

        id = M3D_APP->GetProjectorsServer().GetItemByName(name.c_str(), true);
        if (id != -1)
        {
            return id + 0x100000;
        }

        id = M3D_APP->GetLightsServer().GetItemByName(name.c_str(), true);
        if (id != -1)
        {
            return id + 0x800000;
        }

        id = M3D_APP->GetSpritesServer().GetItemByName(name.c_str(), true);
        if (id != -1)
        {
            return id + 0x1000000;
        }

        id = m3d::pClient->GetWorld().GetFxId(name);
        if (id != -1)
        {
            return id + 0x400000;
        }

        if (!name.empty())
        {
            M3D_LOG_ERR("Error: Cant find server item for name: '" + name + "'");
        }
        return 0x200000;
    }

    EngineConfig::~EngineConfig()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float EngineConfig::GetAttackAnimationFrametime(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    EngineConfig::EngineConfig()
    {
        //TODO: check all this shit!!
        g_Kernel->AddClass(RT_CLASS_LOCAL(IConsole));
        m_console = ConsoleFactory();
        m_console->IncRef();
        g_Kernel->RegisterGlobal(m_console, "console");

        m_UseCinematicUpdate.Init("UseCinematicUpdate", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_UseCinematicUpdate, 0);
        m_CommonTroopLife.Init("CommonTroopLife", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_CommonTroopLife, 0);
        m_StepMode.Init("StepMode", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_StepMode, 0);
        m_IDDebug.Init("IDDebug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_IDDebug, 0);
        m_ai_location_debug.Init("ai_location_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_location_debug, 0);
        m_ai_waypoint_debug.Init("ai_waypoint_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_waypoint_debug, 0);
        m_ai_infection_debug.Init("ai_infection_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_infection_debug, 0);
        m_ai_physicobject_debug.Init("ai_physicobject_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicobject_debug, 0);
        m_ai_team_debug.Init("ai_team_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_team_debug, 0);
        m_ai_passmap_debug.Init("ai_passmap_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_passmap_debug, 0);
        m_ai_playerpassmap_debug.Init("ai_playerpassmap_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_playerpassmap_debug, 0);
        m_ai_mouse_debug.Init("ai_mouse_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_mouse_debug, 0);
        m_ai_obstacles_debug.Init("ai_obstacles_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_obstacles_debug, 0);
        m_ai_compositeobj_debug.Init("ai_compositeobj_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_compositeobj_debug, 0);
        m_ai_guns_debug.Init("ai_guns_debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_guns_debug, 0);
        m_ai_min_frame_time.Init("ai_min_frame_time", "0.05", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_min_frame_time, 0);
        m_ai_turntowheels_allowed.Init("ai_turntowheels_allowed", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_turntowheels_allowed, 0);
        m_ai_tweak_wheel_normals.Init("ai_tweak_wheel_normals", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_tweak_wheel_normals, 0);
        m_ai_tweak_vehicle_normals.Init("ai_tweak_vehicle_normals", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_tweak_vehicle_normals, 0);
        m_ai_vehicle_stats.Init("ai_vehicle_stats", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_vehicle_stats, 0);
        m_ai_clash_coeff.Init("ai_clash_coeff", "0.01", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_clash_coeff, 0);
        m_ai_min_hit_velocity.Init("ai_min_hit_velocity", "40", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_min_hit_velocity, 0);
        m_ai_ragdoll_activate_force.Init("ai_ragdoll_activate_force", "5.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_ragdoll_activate_force, 0);
        m_ai_bo_activate_coeff.Init("ai_bo_activate_coeff", "0.01", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_bo_activate_coeff, 0);
        m_ai_ragdoll_shell_coeff.Init("ai_ragdoll_shell_coeff", "12", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_ragdoll_shell_coeff, 0);
        m_ai_ragdoll_mass_size_coeff.Init("ai_ragdoll_mass_size_coeff", "1.8", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_ragdoll_mass_size_coeff, 0);
        m_ai_bo_mass_size_coeff.Init("ai_bo_mass_size_coeff", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_bo_mass_size_coeff, 0);
        m_ai_enemies_ramming_damage_coeff.Init("ai_enemies_ramming_damage_coeff", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_enemies_ramming_damage_coeff, 0);
        m_ai_player_weapons_damage_coeff.Init("ai_player_weapons_damage_coeff", "1.0", CVar::CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_player_weapons_damage_coeff, 0);
        m_ai_physicstep.Init("ai_physicstep", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep, 0);
        m_ai_physicstep_numiterations.Init("ai_physicstep_numiterations", "5", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_numiterations, 0);
        m_ai_physicstep_w.Init("ai_physicstep_w", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_w, 0);
        m_ai_physicstep_erp.Init("ai_physicstep_erp", "0.1", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_erp, 0);
        m_ai_physicstep_cfm.Init("ai_physicstep_cfm", "0.0001", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_cfm, 0);
        m_ai_physicstep_surfacelayer.Init("ai_physicstep_surfacelayer", "0.01", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_surfacelayer, 0);
        m_ai_physicstep_lineardamping.Init("ai_physicstep_lineardamping", "0.1", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_lineardamping, 0);
        m_ai_physicstep_angulardamping.Init("ai_physicstep_angulardamping","0.3",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_angulardamping, 0);
        m_ai_physicstep_max_correcting_vel.Init("ai_physicstep_max_correcting_vel","10",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_physicstep_max_correcting_vel, 0);
        m_ai_geomobjcollider_mu.Init("ai_geomobjcollider_mu", "0.3", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_geomobjcollider_mu, 0);
        m_ai_geomobjcollider_cfm.Init("ai_geomobjcollider_cfm", "0.01", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_geomobjcollider_cfm, 0);
        m_ai_geomobjcollider_erp.Init("ai_geomobjcollider_erp", "0.1", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_geomobjcollider_erp, 0);
        m_ai_ragdolljoints_erp.Init("ai_ragdolljoints_erp", "0.9", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_ragdolljoints_erp, 0);
        m_ai_ragdolljoints_cfm.Init("ai_ragdolljoints_cfm", "0.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_ragdolljoints_cfm, 0);
        m_ai_ragdolljoints_stopcfm.Init("ai_ragdolljoints_stopcfm", "0.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_ragdolljoints_stopcfm, 0);
        m_ai_wheelcollider_erp.Init("ai_wheelcollider_erp", "0.8", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_wheelcollider_erp, 0);
        m_ai_wheelcollider_cfm.Init("ai_wheelcollider_cfm", "1", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ai_wheelcollider_cfm, 0);
        m_ai_static_obstacles_enabled.Init("ai_static_obstacles_enabled","false",CVar::CVAR_BOOL,CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_ai_static_obstacles_enabled, 0);
        m_debugMode.Init("debugMode", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_debugMode, 0);
        m_consoleScreenSize.Init("consoleScreenSize", "0.5", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_consoleScreenSize, 0);
        m_loadFromGAM.Init("loadFromGAM", "no", CVar::CVAR_BOOL, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_loadFromGAM, 0);
        m_weather_ConfigFile.Init("weather_ConfigFile", "data\\Weather.xml", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_weather_ConfigFile, 0);
        m_weather_PathToTextures.Init("weather_PathToTextures","data\\weatherTexs\\",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_weather_PathToTextures, 0);
        m_weather_AtmoRadius.Init("weather_AtmoRadius", "20000.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_weather_AtmoRadius, 0);
        m_NPatchLevel.Init("NPatchLevel", "0.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_NPatchLevel, 0);
        m_r_d3dVersion.Init("r_d3dVersion", "9", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_d3dVersion, 0);
        m_r_width.Init("r_width", "1024", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_width, 0);
        m_r_height.Init("r_height", "768", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_height, 0);
        m_r_bpp.Init("r_bpp", "32", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_bpp, 0);
        m_r_depthBpp.Init("r_depthBpp", "24", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_depthBpp, 0);
        m_r_fullScreen.Init("r_fullScreen", "yes", CVar::CVAR_BOOL, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_fullScreen, 0);
        m_r_compressedTextures.Init("r_compressedTextures", "1", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_compressedTextures, 0);
        m_r_behavior.Init("r_behavior", "80", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_r_behavior, 0);
        m_r_texLodBias.Init("r_texLodBias", "0.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_texLodBias, 0);
        m_r_depthBias.Init("r_depthBias", "8", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_depthBias, 0);
        m_r_renderToNull.Init("r_renderToNull", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_renderToNull, 0);
        m_r_maxLights.Init("r_maxLights", "8", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_maxLights, 0);
        m_r_renderZGuard.Init("r_renderZGuard", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_renderZGuard, 0);
        m_r_enableFog.Init("r_enableFog", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_enableFog, 0);
        m_r_allowPS20.Init("r_allowPS20", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_allowPS20, 0);
        m_r_allowPS20ForNV30.Init("r_allowPS20ForNV30", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_allowPS20ForNV30, 0);
        m_r_dxcursor.Init("r_dxcursor", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_dxcursor, 0);
        m_r_multiSamplesNum.Init("r_multiSamplesNum", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_multiSamplesNum, 0);
        m_r_waterQuality.Init("r_waterQuality", "3", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_waterQuality, 0);
        m_r_waterInQuery.Init("r_waterInQuery", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_waterInQuery, 0);
        m_r_graphicalStats_Fps.Init("r_graphicalStats_Fps", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_Fps, 0);
        m_r_graphicalStats_Dip.Init("r_graphicalStats_Dip", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_Dip, 0);
        m_r_graphicalStats_Dp.Init("r_graphicalStats_Dp", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_Dp, 0);
        m_r_graphicalStats_Fillrate.Init("r_graphicalStats_Fillrate", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_Fillrate, 0);
        m_r_graphicalStats_Tris.Init("r_graphicalStats_Tris", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_Tris, 0);
        m_r_graphicalStats_MemUsed.Init("r_graphicalStats_MemUsed", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_MemUsed, 0);
        m_r_graphicalStats_MemAlloc.Init("r_graphicalStats_MemAlloc", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_MemAlloc, 0);
        m_r_graphicalStats_MemOverhead.Init("r_graphicalStats_MemOverhead", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_r_graphicalStats_MemOverhead, 0);
        m_g_stencilShadows.Init("g_stencilShadows", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_stencilShadows, 0);
        m_g_twoSidedStencil.Init("g_twoSidedStencil", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_twoSidedStencil, 0);
        m_g_stencilShadowsDebug.Init("g_stencilShadowsDebug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_stencilShadowsDebug, 0);
        m_g_renderMeshesDebug.Init("g_renderMeshesDebug", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_renderMeshesDebug, 0);
        m_g_showReflRefrMaps.Init("g_showReflRefrMaps", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_showReflRefrMaps, 0);
        m_g_drawWater.Init("g_drawWater", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawWater, 0);
        m_g_ps11_water_reflection_amount.Init("g_ps11_water_reflection_amount","0.012",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_ps11_water_reflection_amount, 0);
        m_g_ps11_water_refraction_amount.Init("g_ps11_water_refraction_amount","0.014",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_ps11_water_refraction_amount, 0);
        m_g_grassDrawDist.Init("g_grassDrawDist", "150.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_grassDrawDist, 0);
        m_g_grassAlphatest.Init("g_grassAlphatest", "70", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_grassAlphatest, 0);
        m_g_reflectionDrawDistModifier.Init("g_reflectionDrawDistModifier","3.0",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_reflectionDrawDistModifier, 0);
        m_g_refractionDrawDistModifier.Init("g_refractionDrawDistModifier","1.0",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_refractionDrawDistModifier, 0);
        m_g_drawReflectedTerrain.Init("g_drawReflectedTerrain", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawReflectedTerrain, 0);
        m_g_drawRefractedTerrain.Init("g_drawRefractedTerrain", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawRefractedTerrain, 0);
        m_g_drawReflectedModels.Init("g_drawReflectedModels", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawReflectedModels, 0);
        m_g_drawRefractedModels.Init("g_drawRefractedModels", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawRefractedModels, 0);
        m_g_drawSky.Init("g_drawSky", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawSky, 0);
        m_g_drawRoads.Init("g_drawRoads", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawRoads, 0);
        m_g_drawWheelTraces.Init("g_drawWheelTraces", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawWheelTraces, 0);
        m_g_drawShores.Init("g_drawShores", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_drawShores, 0);
        m_g_impostorThreshold.Init("g_impostorThreshold", "500.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_impostorThreshold, 0);
        m_g_showProjectorsStats.Init("g_showProjectorsStats", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_showProjectorsStats, 0);
        m_g_projectorsFarDist.Init("g_projectorsFarDist", "3", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_projectorsFarDist, 0);
        m_g_showEffectsStats.Init("g_showEffectsStats", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_showEffectsStats, 0);
        m_g_showShadowsStats.Init("g_showShadowsStats", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_showShadowsStats, 0);
        m_g_shadowFarDist.Init("g_shadowFarDist", "3", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shadowFarDist, 0);
        m_g_shadowBlur.Init("g_shadowBlur", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shadowBlur, 0);
        m_g_shadowBlurCoeff.Init("g_shadowBlurCoeff", "0.0015", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shadowBlurCoeff, 0);
        m_g_shadowDetailRadius.Init("g_shadowDetailRadius", "28", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shadowDetailRadius, 0);
        m_g_shadowAlphaTest.Init("g_shadowAlphaTest", "1", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shadowAlphaTest, 0);
        m_g_switchCameraAllow.Init("g_switchCameraAllow", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_switchCameraAllow, 0);
        m_g_altEnterAllow.Init("g_altEnterAllow", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_altEnterAllow, 0);
        m_g_transparentRadius.Init("g_transparentRadius", "30", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_transparentRadius, 0);
        m_g_transparencyValue.Init("g_transparencyValue", "0.4", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_transparencyValue, 0);
        m_testShadowRenderToLandscape.Init("testShadowRenderToLandscape", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_testShadowRenderToLandscape, 0);
        m_testShadowRenderToRoad.Init("testShadowRenderToRoad", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_testShadowRenderToRoad, 0);
        m_testShadowRenderToGrass.Init("testShadowRenderToGrass", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_testShadowRenderToGrass, 0);
        m_testShadowRenderToTexture.Init("testShadowRenderToTexture", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_testShadowRenderToTexture, 0);
        m_testShadowActualRenderToTexture.Init("testShadowActualRenderToTexture","yes",CVar::CVAR_BOOL,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_testShadowActualRenderToTexture, 0);
        m_g_shoresDeep.Init("g_shoresDeep", "3.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shoresDeep, 0);
        m_g_shoresWidth.Init("g_shoresWidth", "7.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shoresWidth, 0);
        m_g_shoresElevation.Init("g_shoresElevation", "2.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shoresElevation, 0);
        m_g_shoresOpaque.Init("g_shoresOpaque", "192", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shoresOpaque, 0);
        m_g_shoresMipBias.Init("g_shoresMipBias", "-2.5", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shoresMipBias, 0);
        m_g_shakeLinearAmplitude.Init("g_shakeLinearAmplitude", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shakeLinearAmplitude, 0);
        m_g_shakeAngleAmplitude.Init("g_shakeAngleAmplitude", "0.3", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shakeAngleAmplitude, 0);
        m_g_shakeMinRange.Init("g_shakeMinRange", "20.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_shakeMinRange, 0);
        m_g_forceWaterPSVersion.Init("g_forceWaterPSVersion", "0", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_g_forceWaterPSVersion, 0);
        m_g_postEffectBloom.Init("g_postEffectBloom", "1", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_postEffectBloom, 0);
        m_levFileName.Init("levFileName", "", CVar::CVAR_STRING, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_levFileName, 0);
        m_alphaTestWorld.Init("alphaTestWorld", "32", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_alphaTestWorld, 0);
        m_alphaTestInterface.Init("alphaTestInterface", "8", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_alphaTestInterface, 0);
        m_ttledObjectFadeoutTime.Init("ttledObjectFadeoutTime", "1000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ttledObjectFadeoutTime, 0);
        m_lsShadows.Init("lsShadows", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lsShadows, 0);
        m_lsWireframe.Init("lsWireframe", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lsWireframe, 0);
        m_lsViewDistanceDivider.Init("lsViewDistanceDivider", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lsViewDistanceDivider, 0);
        m_lsTau.Init("lsTau", "5.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lsTau, 0);
        m_FogOfWar.Init("FogOfWar", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_FogOfWar, 0);
        m_clampCells.Init("clampCells", "2", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_clampCells, 0);
        m_selectOrigin.Init("selectOrigin", "5", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_selectOrigin, 0);
        m_lsTransitionDevider.Init("lsTransitionDevider", "0.65", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lsTransitionDevider, 0);
        m_lsTransitionCFactor.Init("lsTransitionCFactor", "0.35", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lsTransitionCFactor, 0);
        m_skidMinDist.Init("skidMinDist", "2.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_skidMinDist, 0);
        m_skidTexName.Init("skidTexName", "data\\fx\\protector.tga", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_skidTexName, 0);
        m_skidDeltaSpeed.Init("skidDeltaSpeed", "5.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_skidDeltaSpeed, 0);
        m_skidOpaque.Init("skidOpaque", "127", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_skidOpaque, 0);
        m_g_texturesFilter.Init("g_texturesFilter", "4", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_texturesFilter, 0);
        m_g_antiAliasing.Init("g_antiAliasing", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_antiAliasing, 0);
        m_ssRender.Init("ssRender", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ssRender, 0);
        m_cgDraw.Init("cgDraw", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_cgDraw, 0);
        m_dsRender.Init("dsRender", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dsRender, 0);
        m_dsShadows.Init("dsShadows", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dsShadows, 0);
        m_esRender.Init("esRender", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_esRender, 0);
        m_esShadows.Init("esShadows", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_esShadows, 0);
        m_lgtRender.Init("lgtRender", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lgtRender, 0);
        m_lgtHalos.Init("lgtHalos", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lgtHalos, 0);
        m_lgtFlares.Init("lgtFlares", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lgtFlares, 0);
        m_lgtShadows.Init("lgtShadows", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lgtShadows, 0);
        m_lgtShadowTexSz.Init("lgtShadowTexSz", "512", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_lgtShadowTexSz, 0);
        m_detShadowTexSz.Init("detShadowTexSz", "512", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_detShadowTexSz, 0);
        m_g_contourWidth.Init("g_contourWidth", "0.005", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_contourWidth, 0);
        m_g_contourColor.Init("g_contourColor", "255 0 0 255", CVar::CVAR_COLOR, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_g_contourColor, 0);
        m_gammaGamma.Init("gammaGamma", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_gammaGamma, 0);
        m_gammaBrightness.Init("gammaBrightness", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_gammaBrightness, 0);
        m_gammaContrast.Init("gammaContrast", "1.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_gammaContrast, 0);
        m_snd_Enable.Init("snd_Enable", "yes", CVar::CVAR_BOOL, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_snd_Enable, 0);
        m_snd_2dVolume.Init("snd_2dVolume", "18", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_2dVolume, 0);
        m_snd_3dVolume.Init("snd_3dVolume", "18", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_3dVolume, 0);
        m_snd_MaxSounds.Init("snd_MaxSounds", "16", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_snd_MaxSounds, 0);
        m_snd_SampleRate.Init("snd_SampleRate", "44100", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_snd_SampleRate, 0);
        m_snd_BitsPerSample.Init("snd_BitsPerSample", "16", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_snd_BitsPerSample, 0);
        m_mus_Enable.Init("mus_Enable", "yes", CVar::CVAR_BOOL, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_mus_Enable, 0);
        m_mus_Volume.Init("mus_Volume", "1", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_mus_Volume, 0);
        m_autoPlayVideo.Init("autoPlayVideo", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_autoPlayVideo, 0);
        m_video[0].Init("video00", "data/video/logotargem.wmv", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_video[1].Init("video01", "data/video/logobuka.wmv", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_video[2].Init("video02", "", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_video[3].Init("video03", "", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_intro.Init("intro", "", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_intro, 0);
        m_ui_pathToResource.Init("ui_pathToResource","data\\if\\strings\\uiEditStrings.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToResource, 0);
        m_ui_pathToSchema.Init("ui_pathToSchema","data\\if\\frames\\uiSchema2.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToSchema, 0);
        m_ui_pathToFrames.Init("ui_pathToFrames","data\\if\\frames\\frames.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToFrames, 0);
        m_ui_pathToFonts.Init("ui_pathToFonts", "data\\if\\fonts\\fonts.xml", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToFonts, 0);
        m_ui_pathToLanguageTable.Init("ui_pathToLanguageTable","data\\if\\fonts\\LanguageTable.txt",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToLanguageTable, 0);
        m_ui_pathToGameWindows.Init("ui_pathToGameWindows","data\\if\\dialogs\\GameWindows.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToGameWindows, 0);
        m_ui_pathToCharDictionary.Init("ui_pathToCharDictionary","data\\if\\fonts\\charDictionary.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToCharDictionary, 0);
        m_ui_pathToStrings.Init("ui_pathToStrings","data\\if\\diz\\strings.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_pathToStrings, 0);
        m_ui_codePageName.Init("ui_codePageName", "windows-1251", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_codePageName, 0);
        m_ui_dblClickDelay.Init("ui_dblClickDelay", "300", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_dblClickDelay, 0);
        m_ui_forceHieroglyphicFont.Init("ui_forceHieroglyphicFont", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_forceHieroglyphicFont, 0);
        m_ui_defaultWndAnimationShowType.Init("ui_defaultWndAnimationShowType","TO_LEFT",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_defaultWndAnimationShowType, 0);
        m_ui_defaultWndAnimationHideType.Init("ui_defaultWndAnimationHideType","TO_RIGHT",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_defaultWndAnimationHideType, 0);
        m_ui_defaultWndAnimationShowSpeed.Init("ui_defaultWndAnimationShowSpeed","500",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_defaultWndAnimationShowSpeed, 0);
        m_ui_defaultWndAnimationHideSpeed.Init("ui_defaultWndAnimationHideSpeed","500",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_defaultWndAnimationHideSpeed, 0);
        m_ui_defaultWndAnimationShowAccel.Init("ui_defaultWndAnimationShowAccel","0",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_defaultWndAnimationShowAccel, 0);
        m_ui_defaultWndAnimationHideAccel.Init("ui_defaultWndAnimationHideAccel","0",CVar::CVAR_FLOAT,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_ui_defaultWndAnimationHideAccel, 0);
        m_input_defaultInputLanguageBase.Init("input_defaultInputLanguageBase","true",CVar::CVAR_BOOL,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_input_defaultInputLanguageBase, 0);
        m_input_baseKeyboardLayot.Init("input_baseKeyboardLayot", "00000409", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_input_baseKeyboardLayot, 0);
        m_input_additionalKeyboardLayot.Init("input_additionalKeyboardLayot","00000419",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_input_additionalKeyboardLayot, 0);
        m_demoPlaybackSpeed.Init("demoPlaybackSpeed", "200", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_demoPlaybackSpeed, 0);
        m_profile.Init("profile", "", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_profile, 0);
        m_campaign.Init("campaign", "c1", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_campaign, 0);
        m_net_Logging.Init("net_Logging", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_Logging, 0);
        m_net_SequenceSizeMax.Init("net_SequenceSizeMax", "50000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_SequenceSizeMax, 0);
        m_net_SequenceSizeMin.Init("net_SequenceSizeMin", "3000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_SequenceSizeMin, 0);
        m_net_NearDistMax.Init("net_NearDistMax", "2000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_NearDistMax, 0);
        m_net_NearDistMin.Init("net_NearDistMin", "500", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_NearDistMin, 0);
        m_net_FarDistMax.Init("net_FarDistMax", "4000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_FarDistMax, 0);
        m_net_FarDistMin.Init("net_FarDistMin", "1500", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_FarDistMin, 0);
        m_net_TroopDelayMax.Init("net_TroopDelayMax", "3000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_TroopDelayMax, 0);
        m_net_TroopDelayMin.Init("net_TroopDelayMin", "500", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_TroopDelayMin, 0);
        m_net_TownDelayMax.Init("net_TownDelayMax", "5000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_TownDelayMax, 0);
        m_net_TownDelayMin.Init("net_TownDelayMin", "1000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_TownDelayMin, 0);
        m_net_LairDelayMax.Init("net_LairDelayMax", "5000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_LairDelayMax, 0);
        m_net_LairDelayMin.Init("net_LairDelayMin", "1000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_LairDelayMin, 0);
        m_net_CheckVersion.Init("net_CheckVersion", "yes", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_CheckVersion, 0);
        m_net_GameName.Init("net_GameName", "New game", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_GameName, 0);
        m_net_Quality.Init("net_Quality", "1000", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_Quality, 0);
        m_net_Debug.Init("net_Debug", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_net_Debug, 0);
        m_pathToDefaultKeyBindings.Init("pathToDefaultKeyBindings","data\\scripts\\DefaultKeyBindings.lua",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_pathToDefaultKeyBindings, 0);
        m_pathToGlobProps.Init("pathToGlobProps","data\\gamedata\\GlobalProperties.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_pathToGlobProps, 0);
        m_pathToSoilProps.Init("pathToSoilProps", "data\\tiles\\TilesProps.xml", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_pathToSoilProps, 0);
        m_snd_pathToMusic.Init("snd_pathToMusic", "data\\music\\music.xml", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_pathToMusic, 0);
        m_snd_pathToSounds.Init("snd_pathToSounds", "data\\sounds\\sounds.xml", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_pathToSounds, 0);
        m_snd_pathToMusicBlocks.Init("snd_pathToMusicBlocks","data\\music\\blocks\\MusicBlocks.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_pathToMusicBlocks, 0);
        m_snd_pathToTownAmbient.Init("snd_pathToTownAmbient","data\\sounds\\ambience\\TownAmbience.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_pathToTownAmbient, 0);
        m_snd_pathToRadioSounds.Init("snd_pathToRadioSounds","data\\sounds\\radio\\RadioSounds.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_pathToRadioSounds, 0);
        m_snd_pathToRadioSamples.Init("snd_pathToRadioSamples","data\\sounds\\radio\\RadioSamples.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_pathToRadioSamples, 0);
        m_snd_pathToSoundGroups.Init("snd_pathToSoundGroups","data\\sounds\\SoundGroups.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_snd_pathToSoundGroups, 0);
        m_pathToBelongsToLogos.Init("pathToBelongsToLogos","data\\models\\BelongsToLogos.xml",CVar::CVAR_STRING,CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_pathToBelongsToLogos, 0);
        m_timer_fixedtimestep.Init("timer_fixedtimestep", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_timer_fixedtimestep, 0);
        m_timer_timestepvalue.Init("timer_timestepvalue", "40", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_timer_timestepvalue, 0);
        m_cinematic_spring_coeff.Init("cinematic_spring_coeff", "0.7", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_cinematic_spring_coeff, 0);
        m_DoNotLoadMainmenuLevel.Init("DoNotLoadMainmenuLevel", "false", CVar::CVAR_BOOL, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_DoNotLoadMainmenuLevel, 0);
        m_mainMenuLevelName.Init("mainMenuLevelName","data\\maps\\MainMenu.ssl",CVar::CVAR_STRING,CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_mainMenuLevelName, 0);
        m_firstLevel.Init("firstLevel", "r1m1", CVar::CVAR_STRING, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_firstLevel, 0);
        m_camInfo.Init("camInfo", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_camInfo, 0);
        m_mouseMinSensitivity.Init("mouseMinSensitivity", "0.5", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_mouseMinSensitivity, 0);
        m_mouseMaxSensitivity.Init("mouseMaxSensitivity", "3", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_mouseMaxSensitivity, 0);
        m_dbg_doNotUpdateAI.Init("dbg_doNotUpdateAI", "no", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_doNotUpdateAI, 0);
        m_dbg_intVar1.Init("dbg_intVar1", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_intVar1, 0);
        m_dbg_intVar2.Init("dbg_intVar2", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_intVar2, 0);
        m_dbg_intVar3.Init("dbg_intVar3", "0", CVar::CVAR_INT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_intVar3, 0);
        m_dbg_boolVar1.Init("dbg_boolVar1", "false", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_boolVar1, 0);
        m_dbg_boolVar2.Init("dbg_boolVar2", "false", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_boolVar2, 0);
        m_dbg_boolVar3.Init("dbg_boolVar3", "false", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_boolVar3, 0);
        m_dbg_floatVar1.Init("dbg_floatVar1", "0.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_floatVar1, 0);
        m_dbg_floatVar2.Init("dbg_floatVar2", "0.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_floatVar2, 0);
        m_dbg_floatVar3.Init("dbg_floatVar3", "0.0", CVar::CVAR_FLOAT, CVar::CVAR_ARCHIVE);
        m_console->RegisterCVar(&m_dbg_floatVar3, 0);
        m_loc_formsQuantity.Init("loc_formsQuantity", "2", CVar::CVAR_INT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_loc_formsQuantity, 0);
        m_clipCursorWithinRenderWnd.Init("clipCursorWithinRenderWnd", "true", CVar::CVAR_BOOL, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_clipCursorWithinRenderWnd, 0);
        m_hasServers.Init("hasServers", "true", CVar::CVAR_BOOL, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_hasServers, 0);
        m_fadingTimeBeforeNextMap.Init("fadingTimeBeforeNextMap", "2.f", CVar::CVAR_FLOAT, CVar::CVAR_READONLY);
        m_console->RegisterCVar(&m_fadingTimeBeforeNextMap, 0);

        unsigned int markerColors[16] = { 0 };
        markerColors[0] = 0;
        markerColors[1] = 128;
        markerColors[2] = 0x8000;
        markerColors[3] = 32896;
        markerColors[4] = 0x7FFFFF; //TODO: check this
        markerColors[5] = 0x80007F; //TODO: check this
        markerColors[6] = 0x807FFF; //TODO: check this
        markerColors[7] = 0x808080; //TODO: check this
        markerColors[8] = 0x808080; //TODO: check this
        markerColors[9] = 255;
        markerColors[10] = 65280;
        markerColors[11] = 0xFFFF;
        markerColors[12] = 16711680;
        markerColors[13] = 16711935;
        markerColors[14] = 16776960;
        markerColors[15] = 0xFFFFFF;

        int i = 0;
        for (auto& color : m_markerColors)
        {
            auto const name = "markerColor" + CStr(i);
            char sn[32] = { 0 };

            //TODO: check and refactor this shit!!!
            auto colorComponents = reinterpret_cast<unsigned __int8*>(&markerColors[i]);
            sprintf(sn, "%d %d %d %d", colorComponents[2], colorComponents[1], colorComponents[0], colorComponents[3]);
            color.Init(name.c_str(), sn, CVar::CVAR_COLOR, CVar::CVAR_ARCHIVE);

            m_console->RegisterCVar(&color, 0);
            ++i;
        }

        m_console->RegisterCVar(&m_video[0], 0);
        m_console->RegisterCVar(&m_video[1], 0);
        m_console->RegisterCVar(&m_video[2], 0);
        m_console->RegisterCVar(&m_video[3], 0);

        i = 0;
        for (auto& macro : m_r_shadersMacros)
        {
            auto const name = "shaderMacro" + CStr(i);
            macro.Init(name.c_str(), "", CVar::CVAR_STRING, CVar::CVAR_ARCHIVE);
            m_console->RegisterCVar(&macro, 0);
            ++i;
        }
    }

    float EngineConfig::GetAnimationLength(int, int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int EngineConfig::Load(CStr const& fname)
    {
        return m_console->Load(fname);
    }
}

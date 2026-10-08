#include <level.h>
#include <stdexcept>

#include "m3dapp.h"
#include "skelmodel.h"
#include "core/kernel.h"
#include "core/log.h"
#include "core/ref_ptr.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "core/ini.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Level, GetLandSize)
{
    auto const level = dynamic_cast<m3d::Level*>(context->asObject(0, "Level"));
    context->pushInt(level->land_size);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Level, GetLevelName)
{
    auto const level = dynamic_cast<m3d::Level*>(context->asObject(0, "Level"));
    auto levelName = level->GetLevelName();
    context->pushString(levelName);
    return 1;
}

namespace
{
    // "b2 b1 b0" of a colour's low three bytes, as the level file stores it.
    CStr ColorToString(unsigned int color)
    {
        return CStr(static_cast<int>((color >> 16) & 0xFF)) + CStr(" ") + CStr(static_cast<int>((color >> 8) & 0xFF))
            + CStr(" ") + CStr(static_cast<int>(color & 0xFF));
    }
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Level)
        RT_CLASS_EXPORT(Level, m3d::METHOD, GetLandSize, "", "", "")
        RT_CLASS_EXPORT(Level, m3d::METHOD, GetLevelName, "", "", "")
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Level);

    Class* Level::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    char const* Level::GetLevelName() const
    {
        return m_levelName.c_str();
    }

    int Level::GetLandSize() const
    {
        return land_size;
    }

    Object* Level::CreateObject()
    {
        return new Level;
    }

    int Level::Save(CStr const& name, CCamera const& Cam)
    {
        // RVA 0x790130 - writes the level ini that Load reads.
        ref_ptr file = M3D_KERNEL->CreateIniFile();
        file->SetString("LEVEL", "WEATHERDETAIL", m_weatherDetailName);
        file->SetString("LEVEL", "WEATHERTYPE", m_weatherType);
        file->SetInteger("LEVEL", "CURRENTDAYTIME", m_currentDayTime);
        file->SetString("LEVEL", "PATH", m_levelPath);
        file->SetString("LEVEL", "LEVELNAME", m_levelName);
        file->SetString("LEVEL", "HIGHMAP", m_hfName);
        file->SetString("LEVEL", "DETMAP", m_waterName);
        file->SetString("LEVEL", "CAMERAMAP", m_cameraMapName);
        file->SetString("LEVEL", "COLORMAP", m_colormapName);
        file->SetString("LEVEL", "CLIFFMAP", m_cliffmapName);
        file->SetString("LEVEL", "CLIFFSET", m_cliffsetName);
        file->SetString("LEVEL", "ROADMAP", m_roadmapName);
        file->SetString("LEVEL", "ROADSET", m_roadsetName);
        file->SetString("LEVEL", "WAYPOINTS", m_waypointsName);
        file->SetString("LEVEL", "BEACHSETS", m_beachsetsName);
        file->SetString("LEVEL", "SHORELINE", m_shoreLineName);
        file->SetString("LEVEL", "NORMALMAP", m_normalMapName);
        file->SetString("LEVEL", "CUBEMAP", m_cubemapName);
        file->SetInteger("LEVEL", "LEVELSIZE", land_size);
        file->SetFloat("LEVEL", "MAXHEIGHT", m_maxHeight);
        file->SetFloat("LEVEL", "WATERLEVEL", waterlevel);
        file->SetFloat("LEVEL", "BASEWATERLEVEL", m_baseWaterLevel);
        CStr tmpStr = ColorToString(m_reflectionTint);
        file->SetString("LEVEL", "REFLECTIONTINT", tmpStr);
        tmpStr = ColorToString(m_refractionTint);
        file->SetString("LEVEL", "REFRACTIONTINT", tmpStr);
        file->SetFloat("LEVEL", "SKYDOMEDIVIDER", m_skyDomeDivider);
        file->SetString("LEVEL", "WATERSMALLTEX", m_waterTexSmall);
        file->SetString("LEVEL", "WATERBIGTEX", m_waterTexBig);
        file->SetFloat("LEVEL", "WATERABSRED", m_waterAbsorptionRed);
        file->SetFloat("LEVEL", "WATERABSGREEN", m_waterAbsorptionGreen);
        file->SetFloat("LEVEL", "WATERABSBLUE", m_waterAbsorptionBlue);
        file->SetFloat("LEVEL", "MINSAFEX", m_minSafex);
        file->SetFloat("LEVEL", "MINSAFEY", m_minSafey);
        file->SetFloat("LEVEL", "MAXSAFEX", m_maxSafex);
        file->SetFloat("LEVEL", "MAXSAFEY", m_maxSafey);
        file->SetString("LEVEL", "SERVERDYN", m_dsSrvName);
        file->SetString("LEVEL", "SERVERQUESTSTATES", m_questStatesFileName);
        file->SetString("LEVEL", "MODELNAMES", m_prototypeFullNames);
        file->SetString("LEVEL", "OBJECTNAMES", m_ObjectFullNames);
        file->SetString("LEVEL", "SERVEREXTERNALPATHS", m_externalPathsFileName);
        file->SetString("LEVEL", "STATICOBSTACLES", m_staticObstaclesFileName);
        file->SetString("LEVEL", "PLAYERPASSMAP", m_playerPassMapFileName);
        file->SetString("LEVEL", "SERVERS", m_serversname);
        file->SetString("LEVEL", "STATICSERVERS", m_staticServers);
        file->SetString("LEVEL", "PASSMAP", m_passMapName);
        file->SetInteger("LEVEL", "PASSMAPCELLSIZE", m_passMapCellSize);
        file->SetString("LEVEL", "TRIGGERSNAME", m_TriggersName);
        file->SetString("LEVEL", "CINEMATRIGGERSNAME", m_cinemaTriggersName);
        file->SetString("LEVEL", "DLGSTRINGS", m_dialogStrings);
        file->SetInteger("LEVEL", "SKYTYPE", m_skyType);
        file->SetString("LEVEL", "ENV_SKY_Z_POS", m_envMapsNames[4]);
        file->SetString("LEVEL", "ENV_SKY_Z_NEG", m_envMapsNames[5]);
        file->SetString("LEVEL", "ENV_SKY_Y_POS", m_envMapsNames[2]);
        file->SetString("LEVEL", "ENV_SKY_Y_NEG", m_envMapsNames[3]);
        file->SetString("LEVEL", "ENV_SKY_X_POS", m_envMapsNames[0]);
        file->SetString("LEVEL", "ENV_SKY_X_NEG", m_envMapsNames[1]);
        // NOTE: the road map and road set are written a second time.
        file->SetString("LEVEL", "ROADMAP", m_roadmapName);
        file->SetString("LEVEL", "ROADSET", m_roadsetName);
        if (!m_skyType)
        {
            file->SetString("LEVEL", "ENV_BDROP_Z_POS", m_envMapsNames[10]);
            file->SetString("LEVEL", "ENV_BDROP_Z_NEG", m_envMapsNames[11]);
            file->SetString("LEVEL", "ENV_BDROP_Y_POS", m_envMapsNames[8]);
            file->SetString("LEVEL", "ENV_BDROP_Y_NEG", m_envMapsNames[9]);
            file->SetString("LEVEL", "ENV_BDROP_X_POS", m_envMapsNames[6]);
            file->SetString("LEVEL", "ENV_BDROP_X_NEG", m_envMapsNames[7]);
        }
        file->SetFloat("LEVEL", "ENV_SKY_SCALE_S", m_skyScale[0]);
        file->SetFloat("LEVEL", "ENV_SKY_SCALE_T", m_skyScale[1]);
        file->SetFloat("LEVEL", "ENV_SKY_SCROLL_SPEED", m_skyScrollSpeed);
        file->SetFloat("LEVEL", "ENV_SKY_ROTATE_SPEED", m_skyRotateSpeed);
        file->SetInteger("LEVEL", "ENV_SKY_CLOUDS_ENABLE", m_skyCloudsEnable);
        file->SetFloat("LEVEL", "SUN_AZIMUTH_SPEED", m_sunAzimuthSpeed);
        file->SetFloat("LEVEL", "SUN_AZIMUTH", m_sunAzimuth);
        file->SetFloat("LEVEL", "SUN_DAY_ASCENTION", m_sunDayAscention);
        file->SetFloat("LEVEL", "SUN_RISE_ASCENTION", m_sunRiseAscention);
        file->SetFloat("LEVEL", "SUN_SET_ASCENTION", m_sunSetAscention);
        file->SetFloat("CAMERA", "X", Cam.m_worldOrigin.x);
        file->SetFloat("CAMERA", "Y", Cam.m_worldOrigin.y);
        file->SetFloat("CAMERA", "Z", Cam.m_worldOrigin.z);
        file->SetFloat("CAMERA", "A", Cam.m_rotYaw);
        file->SetFloat("CAMERA", "B", Cam.m_rotPitch);
        file->SetFloat("CAMERA", "G", Cam.m_rotRoll);
        file->SetInteger("DEMO", "STARTUP", demostartup);
        file->SetString("DEMO", "FILE", demofile);
        file->SetString("LEVEL", "TILES", m_tilesFileName);
        // The illumination is saved from the original (level file) colours where there are two sets.
        file->SetString("ILLUMINATION", "MODEL_AMBIENT", ColorToString(m_omodelAmbient));
        file->SetString("ILLUMINATION", "MODEL_DIFFUSE", ColorToString(m_modelDiffuse));
        file->SetString("ILLUMINATION", "LS_COLOR", ColorToString(m_olsAmbient));
        file->SetString("ILLUMINATION", "LS_DIFFUSE", ColorToString(m_lsDiffuse));
        file->SetFloat("ILLUMINATION", "LS_TFACTOR", m_lsTFactor);
        file->SetString("ILLUMINATION", "SKY_COLOR", ColorToString(m_olsSkyColor));
        file->SetString("ILLUMINATION", "FOG_COLOR", ColorToString(m_olsFarColor));
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (!stream->Open(name.c_str(), fs::IStream::OPEN_WRITE))
        {
            M3D_LOG_INFO(CStr("Error: could't open") + name + CStr(" file"));
            return 0;
        }
        file->Write(*stream);
        stream->Close();
        return 1;
    }

    Level::~Level()
    {
        // RVA 0x78B930 - only the members are destroyed.
    }

    Object* Level::Clone()
    {
        // RVA 0x78C890
        return new Level(*this);
    }

    CStr Level::GetFullPathNameA(CStr const& name)
    {
        return m_levelPath + "\\" + name;
    }

    int Level::New(CCamera& Cam, int levelsize)
    {
        // RVA 0x78BE90 - the defaults for a new editor level.
        m_levelName = "Untitled";
        m_levelPath = "data\\editor";
        m_hfName = "displace.bin";
        m_waterName = "water.raw";
        m_cameraMapName = "cameramap.raw";
        m_maxHeight = 2500.0f;
        land_size = levelsize;
        waterlevel = 0.0f;
        m_baseWaterLevel = 0.0f;
        m_reflectionTint = 0xFFFFFFFF;
        m_refractionTint = 0xFFFFFFFF;
        float const landScale = 8.0f;
        float const safeMax = static_cast<float>(16 * levelsize) * landScale - 40.0f;
        m_skyDomeDivider = landScale;
        m_minSafex = 40.0f;
        m_minSafey = 40.0f;
        m_maxSafex = safeMax;
        m_maxSafey = safeMax;
        m_dsSrvName = "DynamicScene.xml";
        m_questStatesFileName = "QuestStates.xml";
        m_prototypeFullNames = "data\\if\\diz\\model_names.xml";
        m_ObjectFullNames = "object_names.xml";
        m_externalPathsFileName = "external_paths.xml";
        m_staticObstaclesFileName = "static_obstacles.xml";
        m_playerPassMapFileName = "player_passmap.bin";
        m_serversname = "data\\models\\servers.xml";
        m_staticServers = "data\\models\\commonservers.xml";
        // NOTE: the road map and road set are set twice; the second road map name wins.
        m_roadmapName = "roadMap.xml";
        m_roadsetName = "Roads.xml";
        m_passMapName = "passmap.raw";
        m_colormapName = "colormap.raw";
        m_cliffmapName = "level.cliff";
        m_cliffsetName = "Cliffs.xml";
        m_roadmapName = "LevelRoads.xml";
        m_roadsetName = "Roads.xml";
        m_waypointsName = "ways.xml";
        m_beachsetsName = "Beachsets.xml";
        m_shoreLineName = "ShoreLine.bin";
        m_normalMapName = "NormalMap.bin";
        m_cubemapName = "data/models/textures/lobbycube.dds";
        AnimatedModel::SetCubeMapTexName(m_cubemapName);
        m_passMapCellSize = 16;
        m_TriggersName = "triggers.xml";
        m_cinemaTriggersName = "cinemaTriggers.xml";
        m_dialogStrings = "strings.xml";
        m_weatherDetailName = "WeatherDetail.xml";
        m_tilesFileName = "level.tile";
        m_skyType = 1;
        m_envMapsNames[4] = "data\\env\\day4.front.tga";
        m_envMapsNames[5] = "data\\env\\day4.back.tga";
        m_envMapsNames[2] = "data\\env\\day4.up.tga";
        m_envMapsNames[3] = "";
        m_envMapsNames[0] = "data\\env\\day4.right.tga";
        m_envMapsNames[1] = "data\\env\\day4.left.tga";
        m_skyScale[0] = 0.8f;
        m_skyScale[1] = 0.4f;
        m_skyScrollSpeed = 1.0f;
        m_skyRotateSpeed = 2.0f;
        m_sunAzimuthSpeed = 0.0f;
        m_sunAzimuth = 45.0f;
        m_sunDayAscention = 90.0f;
        m_sunRiseAscention = 53.0f;
        m_skyCloudsEnable = 1;
        m_sunSetAscention = 127.0f;
        m_currentDayTime = 0;
        // NOTE: the weather type is the number 0 made into a string, "0".
        m_weatherType = CStr(0);
        Cam.m_worldOrigin.x = 54.348f;
        Cam.m_worldOrigin.y = 63.34f;
        Cam.m_worldOrigin.z = 354.97f;
        Cam.m_rotYaw = -3.934f;
        Cam.m_rotPitch = 0.376f;
        Cam.m_rotRoll = 0.0f;
        demostartup = 0;
        demofile = "demo.rec";
        return 1;
    }

    Class* Level::GetClass() const
    {
        return RT_CLASS_LOCAL(Level);
    }

    int Level::Load(CStr const& name, CCamera& cam, bool bQuiet)
    {
        ref_ptr file = M3D_KERNEL->CreateIniFile();
        scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
        if (stream->Open(name.c_str(), fs::IStream::OPEN_READ))
        {
            file->Read(*stream);
            stream->Close();
            auto err = file->GetError();
            if (err)
            {
                M3D_LOG_INFO("Error: Parse error file " + name + " err " + CStr(err));
                return 0;
            }
            m_levelName = "TEST";
            m_levelPath = file->GetString("LEVEL", "PATH");
            m_levelName = file->GetString("LEVEL", "LEVELNAME");
            if (!bQuiet)
            {
                M3D_APP->PutSplash(0, M3D_APP->GetStringByStringId0("LoadingDots").c_str());
            }
            m_hfName = file->GetString("LEVEL", "HIGHMAP");
            m_waterName = file->GetString("LEVEL", "DETMAP");
            m_cameraMapName = file->GetString("LEVEL", "CAMERAMAP");
            if (m_cameraMapName.empty())
            {
                m_cameraMapName = "cameraMap.raw";
            }
            m_colormapName = file->GetString("LEVEL", "COLORMAP");
            if (m_colormapName.empty())
            {
                m_colormapName = "colormap.raw";
            }
            m_cliffmapName = file->GetString("LEVEL", "CLIFFMAP");
            if (m_cliffmapName.empty())
            {
                m_cliffmapName = "level.cliff";
            }
            m_cliffsetName = file->GetString("LEVEL", "CLIFFSET");
            m_roadmapName = file->GetString("LEVEL", "ROADMAP");
            if (m_roadmapName.empty())
            {
                m_roadmapName = "roadMap.xml";
            }
            m_roadsetName = file->GetString("LEVEL", "ROADSET");
            if (m_roadsetName.empty())
            {
                m_roadsetName = "Roads.xml";
            }
            m_waypointsName = file->GetString("LEVEL", "WAYPOINTS");
            if (m_waypointsName.empty())
            {
                m_waypointsName = "Ways.xml";
            }
            m_beachsetsName = file->GetString("LEVEL", "BEACHSETS");
            if (m_beachsetsName.empty())
            {
                m_beachsetsName = "Beachsets.xml";
            }
            m_shoreLineName = file->GetString("LEVEL", "SHORELINE");
            if (m_shoreLineName.empty())
            {
                m_shoreLineName = "ShoreLine.bin";
            }
            m_normalMapName = file->GetString("LEVEL", "NORMALMAP");
            if (m_normalMapName.empty())
            {
                m_normalMapName = "NormalMap.bin";
            }
            m_cubemapName = file->GetString("LEVEL", "CUBEMAP");
            if (m_cubemapName.empty())
            {
                m_cubemapName = "data/models/textures/lobbycube.dds";
            }
            m3d::AnimatedModel::SetCubeMapTexName(m_cubemapName);
            land_size = file->GetInteger("LEVEL", "LEVELSIZE");
            m_maxHeight = file->GetFloat("LEVEL", "MAXHEIGHT");
            if (CStr const height = file->GetString("LEVEL", "MAXHEIGHT"); height.empty())
            {
                m_maxHeight = 2500.0;
            }
            m_minSafex = file->GetFloat("LEVEL", "MINSAFEX");
            if (CStr const minSafex = file->GetString("LEVEL", "MINSAFEX"); minSafex.empty())
            {
                m_minSafex = 40.0;
            }
            m_minSafey = file->GetFloat("LEVEL", "MINSAFEY");
            if (CStr const minSafey = file->GetString("LEVEL", "MINSAFEY"); minSafey.empty())
            {
                m_minSafey = 40.0;
            }
            m_maxSafex = file->GetFloat("LEVEL", "MAXSAFEX");
            if (CStr const maxSafex = file->GetString("LEVEL", "MAXSAFEX"); maxSafex.empty())
            {
                m_maxSafex = ((16 * land_size) * 8.0) - 40.0;
            }
            m_maxSafey = file->GetFloat("LEVEL", "MAXSAFEY");
            if (CStr const maxSafey = file->GetString("LEVEL", "MAXSAFEY"); maxSafey.empty())
            {
                m_maxSafey = ((16 * land_size) * 8.0) - 40.0;
            }
            waterlevel = file->GetFloat("LEVEL", "WATERLEVEL");
            m_baseWaterLevel = file->GetFloat("LEVEL", "BASEWATERLEVEL");
            if (CStr const level = file->GetString("LEVEL", "BASEWATERLEVEL"); level.empty())
            {
                m_baseWaterLevel = waterlevel;
            }
            m_skyDomeDivider = file->GetFloat("LEVEL", "SKYDOMEDIVIDER");
            if (CStr const skyDomeDivider = file->GetString("LEVEL", "SKYDOMEDIVIDER"); skyDomeDivider.empty())
            {
                m_skyDomeDivider = 8.0;
            }
            m_reflectionTint = strToColor(file->GetString("LEVEL", "REFLECTIONTINT"), 0xFFFFFFFF);
            m_refractionTint = strToColor(file->GetString("LEVEL", "REFRACTIONTINT"), 0xFFFFFFFF);
            m_waterTexSmall = file->GetString("LEVEL", "WATERSMALLTEX");
            if (m_waterTexSmall.empty())
            {
                m_waterTexSmall = "WaterSm.tga";
            }
            m_waterTexBig = file->GetString("LEVEL", "WATERBIGTEX");
            if (m_waterTexBig.empty())
            {
                m_waterTexBig = "Water.tga";
            }
            m_waterAbsorptionRed = file->GetFloat("LEVEL", "WATERABSRED");
            m_waterAbsorptionGreen = file->GetFloat("LEVEL", "WATERABSGREEN");
            m_waterAbsorptionBlue = file->GetFloat("LEVEL", "WATERABSBLUE");
            m_dsSrvName = file->GetString("LEVEL", "SERVERDYN");
            m_questStatesFileName = file->GetString("LEVEL", "SERVERQUESTSTATES");
            m_externalPathsFileName = file->GetString("LEVEL", "SERVEREXTERNALPATH");
            if (m_externalPathsFileName.empty())
            {
                m_externalPathsFileName = "external_paths.xml";
            }
            m_playerPassMapFileName = file->GetString("LEVEL", "PLAYERPASSMAP");
            if (m_playerPassMapFileName.empty())
            {
                m_playerPassMapFileName = "player_passmap.bin";
            }
            m_prototypeFullNames = file->GetString("LEVEL", "MODELNAMES");
            if (m_prototypeFullNames.empty())
            {
                m_prototypeFullNames = "data\\if\\diz\\model_names.xml";
            }
            m_ObjectFullNames = file->GetString("LEVEL", "OBJECTNAMES");
            if (m_ObjectFullNames.empty())
            {
                m_ObjectFullNames = "object_names.xml";
            }
            m_staticObstaclesFileName = file->GetString("LEVEL", "STATICOBSTACLES");
            if (m_staticObstaclesFileName.empty())
            {
                m_staticObstaclesFileName = "static_obstacles.xml";
            }
            m_serversname = file->GetString("LEVEL", "SERVERS");
            m_staticServers = file->GetString("LEVEL", "STATICSERVERS");
            m_passMapName = file->GetString("LEVEL", "PASSMAP");
            m_passMapCellSize = file->GetInteger("LEVEL", "PASSMAPCELLSIZE");
            m_TriggersName = file->GetString("LEVEL", "TRIGGERSNAME");
            m_cinemaTriggersName = file->GetString("LEVEL", "CINEMATRIGGERSNAME");
            m_dialogStrings = file->GetString("LEVEL", "DLGSTRINGS");
            m_tilesFileName = file->GetString("LEVEL", "TILES");
            m_skyType = file->GetInteger("LEVEL", "SKYTYPE");
            m_envMapsNames[4] = file->GetString("LEVEL", "ENV_SKY_Z_POS");
            m_envMapsNames[5] = file->GetString("LEVEL", "ENV_SKY_Z_NEG");
            m_envMapsNames[2] = file->GetString("LEVEL", "ENV_SKY_Y_POS");
            m_envMapsNames[3] = file->GetString("LEVEL", "ENV_SKY_Y_NEG");
            m_envMapsNames[0] = file->GetString("LEVEL", "ENV_SKY_X_POS");
            m_envMapsNames[1] = file->GetString("LEVEL", "ENV_SKY_X_NEG");
            if (!m_skyType)
            {
                m_envMapsNames[10] = file->GetString("LEVEL", "ENV_BDROP_Z_POS");
                m_envMapsNames[11] = file->GetString("LEVEL", "ENV_BDROP_Z_NEG");
                m_envMapsNames[8] = file->GetString("LEVEL", "ENV_BDROP_Y_POS");
                m_envMapsNames[9] = file->GetString("LEVEL", "ENV_BDROP_Y_NEG");
                m_envMapsNames[6] = file->GetString("LEVEL", "ENV_BDROP_X_POS");
                m_envMapsNames[7] = file->GetString("LEVEL", "ENV_BDROP_X_NEG");
            }
            m_skyScale[0] = file->GetFloat("LEVEL", "ENV_SKY_SCALE_S");
            m_skyScale[1] = file->GetFloat("LEVEL", "ENV_SKY_SCALE_T");
            m_skyScrollSpeed = file->GetFloat("LEVEL", "ENV_SKY_SCROLL_SPEED");
            m_skyRotateSpeed = file->GetFloat("LEVEL", "ENV_SKY_ROTATE_SPEED");
            if (m_skyRotateSpeed == 0.0)
                m_skyRotateSpeed = 1.0;
            if (m_skyScrollSpeed == 0.0)
                m_skyScrollSpeed = 1.0;
            m_skyCloudsEnable = file->GetInteger("LEVEL", "ENV_SKY_CLOUDS_ENABLE");
            m_sunAzimuthSpeed = file->GetFloat("LEVEL", "SUN_AZIMUTH_SPEED");
            m_sunAzimuth = file->GetFloat("LEVEL", "SUN_AZIMUTH");
            if (CStr const sunAzimuth = file->GetString("LEVEL", "SUN_AZIMUTH"); sunAzimuth.empty())
            {
                m_sunAzimuth = 45.0;
            }
            m_sunDayAscention = file->GetFloat("LEVEL", "SUN_DAY_ASCENTION");
            if (CStr const sunDayAscention = file->GetString("LEVEL", "SUN_DAY_ASCENTION"); sunDayAscention.empty())
            {
                m_sunDayAscention = 99.0;
            }
            m_sunRiseAscention = file->GetFloat("LEVEL", "SUN_RISE_ASCENTION");
            if (CStr const sunDayAscention = file->GetString("LEVEL", "SUN_RISE_ASCENTION"); sunDayAscention.empty())
            {
                m_sunRiseAscention = 53.0;
            }
            m_sunSetAscention = file->GetFloat("LEVEL", "SUN_SET_ASCENTION");
            if (CStr const sunSetAscention = file->GetString("LEVEL", "SUN_SET_ASCENTION"); sunSetAscention.empty())
            {
                m_sunSetAscention = 127.0;
            }
            m_weatherDetailName = file->GetString("LEVEL", "WEATHERDETAIL");
            if (m_weatherDetailName.empty())
            {
                m_weatherDetailName = "WeatherDetail.xml";
            }
            m_weatherType = file->GetString("LEVEL", "WEATHERTYPE");
            if (m_weatherType.empty())
            {
                m_weatherType = "";
            }
            CStr const currentDayTime = file->GetString("LEVEL", "CURRENTDAYTIME");
            if (!currentDayTime.empty())
            {
                m_currentDayTime = atoi(currentDayTime.c_str());
            }
            else
            {
                m_currentDayTime = 0;
            }
            cam.m_worldOrigin.x = file->GetFloat("CAMERA", "X");
            cam.m_worldOrigin.y = file->GetFloat("CAMERA", "Y");
            cam.m_worldOrigin.z = file->GetFloat("CAMERA", "Z");
            cam.m_rotYaw = file->GetFloat("CAMERA", "A");
            cam.m_rotPitch = file->GetFloat("CAMERA", "B");
            cam.m_rotRoll = file->GetFloat("CAMERA", "G");
            demostartup = file->GetInteger("DEMO", "STARTUP");
            demofile = file->GetString("DEMO", "FILE");
            return 1;
        }
        M3D_LOG_INFO("Error: could't open " + name + " file");
        return 0;
    }

    Level::Level(Level const& rhs)
        : Object()
    {
        // RVA 0x78B680
        // NOTE: nothing is copied: the strings start empty and the numbers uninitialized, so Clone gives a blank
        // level.
    }

    Level::Level()
    {
        this->land_size = 0;
        this->m_skyType = 0;
        this->m_skyCloudsEnable = 0;
        this->m_lsFarColor = 0;
        this->m_olsFarColor = 0;
        this->m_lsSkyColor = 0;
        this->m_olsSkyColor = 0;
        this->m_skyScrollSpeed = 0.0;
        this->m_skyRotateSpeed = 0.0;
        this->m_sunAzimuthSpeed = 0.0;
        this->m_sunAzimuth = 0.0;
        this->m_sunDayAscention = 0.0;
        this->m_sunRiseAscention = 0.0;
        this->m_sunSetAscention = 0.0;
        this->waterlevel = 0.0;
        this->m_baseWaterLevel = 0.0;
    }
}

#pragma once
#include <core/clazz.h>

class CCamera;

namespace m3d
{
    class Level : public Object
    {
    public:
        static Class * GetBaseClass();
        char const * GetLevelName() const ;
        int GetLandSize() const ;
        static Object * CreateObject();
        int Save(CStr const &,CCamera const &);
        virtual ~Level();
        virtual Object * Clone();
        CStr GetFullPathNameA(CStr const &);
        int New(CCamera &,int);
        virtual Class * GetClass() const ;
        int Load(CStr const &,CCamera &,bool);

    protected:
        Level(Level const &);
        Level();

    public:
        RT_CLASS_DECLARE(Level);

    public:
        int m_passMapCellSize;
        int land_size;
        CStr m_passMapName;
        CStr m_weatherDetailName;
        CStr m_weatherType;
        int m_currentDayTime;
        CStr m_levelPath;
        CStr m_levelName;
        CStr m_hfName;
        CStr m_waterName;
        CStr m_cameraMapName;
        CStr m_colormapName;
        CStr m_cliffsetName;
        CStr m_cliffmapName;
        CStr m_roadsetName;
        CStr m_roadmapName;
        CStr m_waypointsName;
        CStr m_beachsetsName;
        CStr m_shoreLineName;
        CStr m_normalMapName;
        CStr m_cubemapName;
        CStr m_dsSrvName;
        CStr m_questStatesFileName;
        CStr m_externalPathsFileName;
        CStr m_staticObstaclesFileName;
        CStr m_playerPassMapFileName;
        CStr m_prototypeFullNames;
        CStr m_ObjectFullNames;
        CStr m_serversname;
        CStr m_staticServers;
        CStr m_TriggersName;
        CStr m_cinemaTriggersName;
        CStr m_dialogStrings;
        float m_minSafex;
        float m_minSafey;
        float m_maxSafex;
        float m_maxSafey;
        int m_skyType;
        CStr m_envMapsNames[12];
        float m_skyScale[2];
        float m_skyScrollSpeed;
        float m_skyRotateSpeed;
        int m_skyCloudsEnable;
        float m_sunAzimuthSpeed;
        float m_sunAzimuth;
        float m_sunDayAscention;
        float m_sunRiseAscention;
        float m_sunSetAscention;
        unsigned int m_lsFarColor;
        unsigned int m_olsFarColor;
        unsigned int m_lsSkyColor;
        unsigned int m_olsSkyColor;
        float waterlevel;
        float m_baseWaterLevel;
        float m_skyDomeDivider;
        unsigned int m_reflectionTint;
        unsigned int m_refractionTint;
        float m_waterAbsorptionRed;
        float m_waterAbsorptionGreen;
        float m_waterAbsorptionBlue;
        CStr m_waterTexSmall;
        CStr m_waterTexBig;
        int demostartup;
        CStr demofile;
        CStr m_tilesFileName;
        float m_maxHeight;
        unsigned int m_modelAmbient;
        unsigned int m_omodelAmbient;
        unsigned int m_modelDiffuse;
        unsigned int m_omodelDiffuse;
        unsigned int m_lsAmbient;
        unsigned int m_olsAmbient;
        unsigned int m_lsDiffuse;
        unsigned int m_olsDiffuse;
        float m_lsTFactor;
    };
}

#pragma once
#include <core/clazz.h>

class CCamera;

namespace m3d
{
    class Level : public m3d::Object
    {
    protected:
        Level();
        Level(const m3d::Level& rhs);

    public:
        virtual  ~Level() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classLevel;
        /* 0x0034 */ CStr m_weatherDetailName;
        /* 0x0040 */ CStr m_weatherType;
        /* 0x004c */ int m_currentDayTime;
        /* 0x0050 */ CStr m_levelPath;
        /* 0x005c */ CStr m_levelName;
        /* 0x0068 */ CStr m_hfName;
        /* 0x0074 */ CStr m_waterName;
        /* 0x0080 */ CStr m_cameraMapName;
        /* 0x008c */ CStr m_colormapName;
        /* 0x0098 */ CStr m_cliffsetName;
        /* 0x00a4 */ CStr m_cliffmapName;
        /* 0x00b0 */ CStr m_roadsetName;
        /* 0x00bc */ CStr m_roadmapName;
        /* 0x00c8 */ CStr m_waypointsName;
        /* 0x00d4 */ CStr m_beachsetsName;
        /* 0x00e0 */ CStr m_shoreLineName;
        /* 0x00ec */ CStr m_normalMapName;
        /* 0x00f8 */ CStr m_cubemapName;
        /* 0x0104 */ CStr m_dsSrvName;
        /* 0x0110 */ CStr m_questStatesFileName;
        /* 0x011c */ CStr m_externalPathsFileName;
        /* 0x0128 */ CStr m_staticObstaclesFileName;
        /* 0x0134 */ CStr m_playerPassMapFileName;
        /* 0x0140 */ CStr m_prototypeFullNames;
        /* 0x014c */ CStr m_ObjectFullNames;
        /* 0x0158 */ CStr m_serversname;
        /* 0x0164 */ CStr m_staticServers;
        /* 0x0170 */ CStr m_passMapName;
        /* 0x017c */ int m_passMapCellSize;
        /* 0x0180 */ CStr m_TriggersName;
        /* 0x018c */ CStr m_cinemaTriggersName;
        /* 0x0198 */ CStr m_dialogStrings;
        /* 0x01a4 */ int land_size;
        /* 0x01a8 */ float m_minSafex;
        /* 0x01ac */ float m_minSafey;
        /* 0x01b0 */ float m_maxSafex;
        /* 0x01b4 */ float m_maxSafey;
        /* 0x01b8 */ int m_skyType;
        /* 0x01bc */ CStr m_envMapsNames[12];
        /* 0x024c */ float m_skyScale[2];
        /* 0x0254 */ float m_skyScrollSpeed;
        /* 0x0258 */ float m_skyRotateSpeed;
        /* 0x025c */ int m_skyCloudsEnable;
        /* 0x0260 */ float m_sunAzimuthSpeed;
        /* 0x0264 */ float m_sunAzimuth;
        /* 0x0268 */ float m_sunDayAscention;
        /* 0x026c */ float m_sunRiseAscention;
        /* 0x0270 */ float m_sunSetAscention;
        /* 0x0274 */ unsigned int m_lsFarColor;
        /* 0x0278 */ unsigned int m_olsFarColor;
        /* 0x027c */ unsigned int m_lsSkyColor;
        /* 0x0280 */ unsigned int m_olsSkyColor;
        /* 0x0284 */ float waterlevel;
        /* 0x0288 */ float m_baseWaterLevel;
        /* 0x028c */ float m_skyDomeDivider;
        /* 0x0290 */ unsigned int m_reflectionTint;
        /* 0x0294 */ unsigned int m_refractionTint;
        /* 0x0298 */ float m_waterAbsorptionRed;
        /* 0x029c */ float m_waterAbsorptionGreen;
        /* 0x02a0 */ float m_waterAbsorptionBlue;
        /* 0x02a4 */ CStr m_waterTexSmall;
        /* 0x02b0 */ CStr m_waterTexBig;
        /* 0x02bc */ int demostartup;
        /* 0x02c0 */ CStr demofile;
        /* 0x02cc */ CStr m_tilesFileName;
        /* 0x02d8 */ float m_maxHeight;
        /* 0x02dc */ unsigned int m_modelAmbient;
        /* 0x02e0 */ unsigned int m_omodelAmbient;
        /* 0x02e4 */ unsigned int m_modelDiffuse;
        /* 0x02e8 */ unsigned int m_omodelDiffuse;
        /* 0x02ec */ unsigned int m_lsAmbient;
        /* 0x02f0 */ unsigned int m_olsAmbient;
        /* 0x02f4 */ unsigned int m_lsDiffuse;
        /* 0x02f8 */ unsigned int m_olsDiffuse;
        /* 0x02fc */ float m_lsTFactor;
        int New(CCamera& Cam, int levelsize);
        int Load(const CStr& name, CCamera& Cam, bool bQuiet);
        int Save(const CStr& name, const CCamera& Cam);
        CStr GetFullPathNameA(const CStr& name);
        int GetLandSize() const;
        const char* GetLevelName() const;
    }; /* size: 0x0300 */

    static_assert(sizeof(Level) == 0x0300);
}

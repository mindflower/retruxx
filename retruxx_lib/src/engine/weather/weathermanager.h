#pragma once
#include "weather.h"
#include <landscape.h>

namespace m3d
{
    enum GlobalTimeParams
    {
        GTP_SUNRISE_TIME = 0x0,
        GTP_DAY_TIME = 0x1,
        GTP_SUNSET_TIME = 0x2,
        GTP_NIGHT_TIME = 0x3,
        GTP_NUM_PARAMS = 0x4,
    };

    class WeatherManager
    {
    private:
        /* 0x0000 */ retruxx::vector<m3d::Weather*, retruxx::allocator<m3d::Weather*> > m_weatherStorage;
        /* 0x0010 */ retruxx::vector<m3d::Weather*, retruxx::allocator<m3d::Weather*> > m_curWeatherStorage;
        /* 0x0020 */ m3d::Weather* m_currentWeather;
        /* 0x0024 */ float m_cloudsOffset;
        /* 0x0028 */ m3d::rend::TexHandle m_starsTexture;
        /* 0x002c */ CStr m_StarsTextureName;
        /* 0x0038 */ m3d::rend::TexHandle m_cloudTextureHandle;
        /* 0x003c */ m3d::GlobalTimeParams m_curDayTime;
        /* 0x0040 */ float m_globalTimeParams[4];
        /* 0x0050 */ bool m_bEdit;
        /* 0x0051 */ char Padding_90[3];
        /* 0x0054 */ m3d::rend::VbHandle m_vbSky;
        /* 0x0058 */ m3d::rend::IbHandle m_ibSky;
        /* 0x005c */ m3d::CWorld* m_owner;

    public:
        WeatherManager(const m3d::WeatherManager&);
        WeatherManager();
        int ReadFromXmlFile(const char* name);
        int WriteToXmlFile(const char* name);
        int SaveWeatherStateToXMLNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode);
        int LoadWeatherStateFromXMLNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode);
        int CreateSky();
        void DoneSky();
        int SetupSkyParams();
        int RenderWeather(m3d::Landscape::LandRenderMode rendMode);
        int RenderWeatherParticles();
        int UpdateDayTime();
        int UpdateWheatherParticles();
        m3d::GlobalTimeParams GetCurrentDayTime() const;
        unsigned int GetWeatherColor(m3d::ColorItems ci) const;
        float GetFogReduceFactorFromWeather() const;
        float GetShadowTransparencyFromWeather() const;
        bool GetShadowVisibilityFromWeather() const;
        void SetActiveWeather(unsigned int Cur);
        void SetActiveWeatherByName(const CStr& name);
        const m3d::Weather* GetActiveWeather() const;
        m3d::Weather* GetWeather(unsigned int N);
        m3d::Weather* GetWeatherByName(const CStr& name);
        unsigned int GetNumWeathers() const;
        unsigned int GetGlobalTimeParamsNum() const;
        const char* GetGlobalTimeParamName(unsigned int i) const;
        float GetGlobalTimeParam(unsigned int iParamIdx) const;
        float SetGlobalTimeParam(unsigned int iParamIdx, float fValue);
        void SetEdit(bool);
        void ChangeStarsTexture(CStr& Name);
        const CStr& GetStarsTextureName() const;
        void AddWeather(const CStr& Name, const CStr& ClassName);
        int DeleteWeather(unsigned int iWeatherIdx);
        void SetOwner(m3d::CWorld* owner);
        void ChangeCloudsTexture();
        void ChangeLightmapTexture();
    }; /* size: 0x0060 */

    static_assert(sizeof(WeatherManager) == 0x0060);
}

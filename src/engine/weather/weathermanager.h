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
    public:
        int DeleteWeather(unsigned int);
        int CreateSky();
        int WriteToXmlFile(char const *);
        void ChangeCloudsTexture();
        float SetGlobalTimeParam(unsigned int,float);
        int SetupSkyParams();
        float GetShadowTransparencyFromWeather() const ;
        GlobalTimeParams GetCurrentDayTime() const ;
        Weather const * GetActiveWeather() const ;
        unsigned int GetWeatherColor(ColorItems) const ;
        void ChangeLightmapTexture();
        float GetGlobalTimeParam(unsigned int) const ;
        int LoadWeatherStateFromXMLNode(cmn::XmlFile *,cmn::XmlNode *);
        int UpdateDayTime();
        float GetFogReduceFactorFromWeather() const ;
        void SetActiveWeather(unsigned int);
        int RenderWeatherParticles();
        int RenderWeather(Landscape::LandRenderMode);
        int SaveWeatherStateToXMLNode(cmn::XmlFile *,cmn::XmlNode *);
        bool GetShadowVisibilityFromWeather() const ;
        int UpdateWheatherParticles();
        int ReadFromXmlFile(char const *);
        WeatherManager();
        void DoneSky();
        void AddWeather(CStr const &,CStr const &);
        unsigned int GetNumWeathers() const ;
        char const * GetGlobalTimeParamName(unsigned int) const ;
        void SetActiveWeatherByName(CStr const &);
        void ChangeStarsTexture(CStr &);
        Weather * GetWeather(unsigned int);
        Weather * GetWeatherByName(CStr const &);
        void SetOwner(CWorld *);

    private:
        std::vector<Weather *> m_weatherStorage;
        std::vector<Weather *> m_curWeatherStorage;
        Weather *m_currentWeather;
        float m_cloudsOffset;
        rend::TexHandle m_starsTexture;
        CStr m_StarsTextureName;
        rend::TexHandle m_cloudTextureHandle;
        GlobalTimeParams m_curDayTime;
        float m_globalTimeParams[4];
        bool m_bEdit;
        rend::VbHandle m_vbSky;
        rend::IbHandle m_ibSky;
        CWorld *m_owner;
    };
}

#pragma once

namespace m3d
{
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
        void AddWeather(class CStr const &,class CStr const &);
        unsigned int GetNumWeathers() const ;
        char const * GetGlobalTimeParamName(unsigned int) const ;
        void SetActiveWeatherByName(class CStr const &);
        void ChangeStarsTexture(class CStr &);
        Weather * GetWeather(unsigned int);
        Weather * GetWeatherByName(class CStr const &);
        void SetOwner(CWorld *);
    protected:
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

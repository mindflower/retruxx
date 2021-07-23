#pragma once

namespace m3d
{
    class WeatherThunderstorm :  public WeatherInclement
    {
    public:
        virtual void DefaultInitialize();
        virtual int Update(float,int);
        virtual Class * GetClass() const ;
        virtual ~WeatherThunderstorm();
        virtual int Render();
        static Class * __fastcall GetBaseClass();
        static Object * __fastcall CreateObject();
        virtual int UpdateColors(ColorItems,ColorTypes);
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int TurnOffEffects();
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual void SetUp();
        virtual Object * Clone();
    protected:
        WeatherThunderstorm();
        WeatherThunderstorm(class WeatherThunderstorm const &);
    private:
        int m_lastthunderTime;
        int m_thunderTimeOut;
        int m_thunderWorktime;
        int m_curthunderTime;
        bool m_thunderActive;
        float m_thunderLerpValue;
        std::vector<int> m_effectId;
        int m_effectCount;
        std::vector<CStr> m_effectNames;
        int m_maxThunderDuty;
        int m_minThunderDuty;
        int m_minThunderPeriod;
        int m_maxThunderPeriod;
    };
}

namespace m3d
{
    class WeatherClear :  public Weather
    {
    public:
        static Class * __fastcall GetBaseClass();
        virtual Object * Clone();
        virtual ~WeatherClear();
        virtual Class * GetClass() const ;
        static Object * __fastcall CreateObject();
    protected:
        WeatherClear(class WeatherClear const &);
        WeatherClear();
    private:
    };
}

namespace m3d
{
    class WindInfo
    {
    public:
        void Write(class ref_ptr<cmn::XmlNode>);
        void Read(class ref_ptr<cmn::XmlNode>);
        WindInfo();
        struct CVector const & GetCurWind() const ;
        struct CVector const & GetDeltaVel() const ;
        void CalculateCurWind(float);
    protected:
    private:
        float m_maxVel;
        float m_minVel;
        float m_changeVelTime;
        float m_changeDirTime;
        CVector m_prevValue;
        CVector m_curValue;
        CVector m_deltaValue;
        float m_DeltaVelChanged;
        float m_DeltaDirChanged;
    };
}

namespace m3d
{
    class Weather :  public Object
    {
    public:
        class CStr const & GetWeatherName() const ;
        bool GetShadowVisibility(unsigned int) const ;
        static Class * __fastcall GetBaseClass();
        struct CVector const & CurrentColor(unsigned int) const ;
        virtual ~Weather();
        float GetWaveHBig() const ;
        char const * ColorTypeName(unsigned int) const ;
        void ChangeCloudTexture(class CStr &);
        static Object * __fastcall CreateObject();
        virtual int UpdateColors(ColorItems,ColorTypes);
        void SetWeatherName(class CStr const &);
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int TurnOffEffects();
        virtual int WriteDetailToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        float GetWaveSizeBig() const ;
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        float GetWaterSpecularS() const ;
        float GetWaterSpecularM() const ;
        class CStr const & GetLightmapTexName(unsigned int) const ;
        virtual Object * Clone();
        virtual void SetUp();
        virtual int ReadDetailFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        WindInfo const & GetWindInfo() const ;
        float GetWaterSpeed() const ;
        float GetWaterCourseAngle() const ;
        virtual int Update(float,int);
        virtual void DefaultInitialize();
        char const * ColorItemName(unsigned int) const ;
        virtual Class * GetClass() const ;
        float GetWaveHSmall() const ;
        class CStr const & GetCloudsTexName(unsigned int) const ;
        virtual void Release();
        virtual int Render();
        float GetShadowTransparency(unsigned int) const ;
        float GetWaveSizeSmall() const ;
    protected:
        Weather(class Weather const &);
        Weather();
    private:
        CStr m_Name;
        CVector m_colorSets[7][4];
        CVector m_currentColors[7];
        bool m_shadowVisibility[4];
        float m_shadowTransparency[4];
        float m_cloudsSpeed[4];
        float m_reduceDistFactor;
        float m_weatherWeight;
        float m_waterSpeed;
        float m_waterHeightBig;
        float m_waterHeightSmall;
        float m_waterSizeBig;
        float m_waterSizeSmall;
        float m_waterCourseAng;
        float m_waterSpecularM;
        float m_waterSpecularS;
        float m_weatherSkyDomeFactor;
        WindInfo m_wind;
        CStr m_PostEffectName[4];
        CStr m_lightmapTextureName[4];
        CStr m_cloudsTextureName[4];
    };
}

namespace m3d
{
    class WeatherInclement :  public Weather
    {
    public:
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual void SetUp();
        virtual void RecreateEffect();
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        static Class * __fastcall GetBaseClass();
        virtual ~WeatherInclement();
        virtual void DefaultInitialize();
        virtual Class * GetClass() const ;
        static Object * __fastcall CreateObject();
        virtual Object * Clone();
        virtual int Render();
        virtual int TurnOffEffects();
    protected:
        WeatherInclement(class WeatherInclement const &);
        WeatherInclement();
    private:
        CStr m_inclementNodeName;
        CStr m_soundNodeName;
        int m_weatherDensity;
        float m_weatherDist;
        std::vector<SgNode *> m_weatherNodes;
    };
}

namespace m3d
{
    class WeatherFoggy :  public Weather
    {
    public:
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual ~WeatherFoggy();
        static Class * __fastcall GetBaseClass();
        virtual Class * GetClass() const ;
        static Object * __fastcall CreateObject();
        virtual Object * Clone();
    protected:
        WeatherFoggy(class WeatherFoggy const &);
        WeatherFoggy();
    private:
    };
}
